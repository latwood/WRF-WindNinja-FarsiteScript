#include <iostream>

/* raster file stuff, so netcdf files */
#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()

using namespace std;

int main()
{
    /* raster file stuff, so netcdf files */
    printf("starting raster dataset, netcdf file stuff\n");

    /* open raster file */
    const char* pszFilename1 = "$scriptRoot/WRF-WindNinja-FarsiteScript/developer_scripts/forWrfGetWeather/wrfGetWeatherInputFiles/wrfout_d3.2015080912.f12.0000";
    GDALDataset  *poDataset;
    GDALAllRegister();
    poDataset = (GDALDataset *) GDALOpen( pszFilename1, GA_ReadOnly );
    if( poDataset == NULL )
    {
        printf("raster dataset is null!\n");
        exit(1);
    }

    /* getting raster dataset information */
    double        adfGeoTransform[6];
    printf( "Driver: %s/%s\n",
            poDataset->GetDriver()->GetDescription(),
            poDataset->GetDriver()->GetMetadataItem( GDAL_DMD_LONGNAME ) );
    printf( "Size is %dx%dx%d\n",
            poDataset->GetRasterXSize(), poDataset->GetRasterYSize(),
            poDataset->GetRasterCount() );
    if( poDataset->GetProjectionRef()  != NULL )
    {
        printf( "Projection is `%s'\n", poDataset->GetProjectionRef() );
    } else
    {
        printf( "Missing Projection\n");
    }
    if( poDataset->GetGeoTransform( adfGeoTransform ) == CE_None )
    {
        printf( "Origin = (%.6f,%.6f)\n",
                adfGeoTransform[0], adfGeoTransform[3] );
        printf( "Pixel Size = (%.6f,%.6f)\n",
                adfGeoTransform[1], adfGeoTransform[5] );
    } else
    {
        printf( "Missing GeoTransform Info\n");
    }

    /* fetching a raster band */
    GDALRasterBand  *poBand;
    int             nBlockXSize, nBlockYSize;
    int             bGotMin, bGotMax;
    double          adfMinMax[2];
    if(poDataset->GetRasterCount() == 0)
    {
        printf("Raster dataset has no bands!\n");
    } else
    {
        printf("Raster->GetRasterCount() = %d\n",poDataset->GetRasterCount());
        poBand = poDataset->GetRasterBand( 1 ); // raster bands are numbered 1 through GetRasterCount()
        if( poDataset == NULL )
        {
            printf("raster band is null!\n");
        } else
        {
            printf("raster band is not null!\n");
            printf("band %s\n",poBand->GetDescription());
            poBand->GetBlockSize( &nBlockXSize, &nBlockYSize ); // error is happening starting from here
            printf( "Block=%dx%d Type=%s, ColorInterp=%s\n",
                    nBlockXSize, nBlockYSize,
                    GDALGetDataTypeName(poBand->GetRasterDataType()),
                    GDALGetColorInterpretationName(
                        poBand->GetColorInterpretation()) );
            adfMinMax[0] = poBand->GetMinimum( &bGotMin );
            adfMinMax[1] = poBand->GetMaximum( &bGotMax );
            if( ! (bGotMin && bGotMax) )
                GDALComputeRasterMinMax((GDALRasterBandH)poBand, TRUE, adfMinMax);
            printf( "Min=%.3fd, Max=%.3f\n", adfMinMax[0], adfMinMax[1] );
            if( poBand->GetOverviewCount() > 0 )
                printf( "Band has %d overviews.\n", poBand->GetOverviewCount() );
            if( poBand->GetColorTable() != NULL )
                printf( "Band has a color table with %d entries.\n",
                         poBand->GetColorTable()->GetColorEntryCount() );

            /* reading data from a raster band */
            float *pafScanline;
            int   nXSize = poBand->GetXSize();
            pafScanline = (float *) CPLMalloc(sizeof(float)*nXSize);
            poBand->RasterIO( GF_Read, 0, 0, nXSize, 1,
                              pafScanline, nXSize, 1, GDT_Float32,
                              0, 0 );
            CPLFree(pafScanline);
        }
        CPLFree(poBand);
    }

    /* close raster dataset and clean up raster memory */
    GDALClose( (GDALDatasetH) poDataset );

    /* end of raster file stuff, so end of netcdf files */
    printf("finished raster dataset, netcdf file stuff\n");

	return 0;
}
