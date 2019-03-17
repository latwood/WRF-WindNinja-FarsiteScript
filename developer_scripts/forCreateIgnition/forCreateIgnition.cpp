#include <iostream>

/* vector file stuff, so shape files */
#include "ogrsf_frmts.h"

using namespace std;

// use this to run the output in farsite as the ignition file: 
// /home/atw09001/src/WRF-WindNinja-FarsiteScript/include/extraApps/farsite/src/TestFARSITE /home/atw09001/src/WRF-WindNinja-FarsiteScript/developer_scripts/forCreateIgnition/farsiteInputFiles/a_run_allOriginalIgnitions.txt 2>&1 | tee farsiteRun_allOriginalIgnitions.log

int main()
{
    /* vector file stuff, so shape files */
    printf("starting vector dataset, shape file stuff\n");

    const char* pszFilename2 = "/home/atw09001/src/WRF-WindNinja-FarsiteScript/developer_scripts/gdalTestInputFiles/5a-cougarCreek-pointIgnit.shp";
    GDALDataset       *poDS;
    GDALAllRegister();
    poDS = (GDALDataset*) GDALOpenEx( pszFilename2, GDAL_OF_VECTOR, NULL, NULL, NULL );
    if( poDS == NULL )
    {
        printf( "vector dataset is null!\n" );
        exit( 1 );
    }

    /* getting vector dataset information */
    double        adfvGeoTransform[6];
    printf( "Driver: %s/%s\n",
            poDS->GetDriver()->GetDescription(),
            poDS->GetDriver()->GetMetadataItem( GDAL_DMD_LONGNAME ) );
    printf( "Size is %dx%dx%d\n",
            poDS->GetRasterXSize(), poDS->GetRasterYSize(),
            poDS->GetRasterCount() );
    if( poDS->GetProjectionRef()  != NULL )
    {
        printf( "Projection is `%s'\n", poDS->GetProjectionRef() );
    } else
    {
        printf( "Missing Projection\n");
    }
    if( poDS->GetGeoTransform( adfvGeoTransform ) == CE_None )
    {
        printf( "Origin = (%.6f,%.6f)\n",
                adfvGeoTransform[0], adfvGeoTransform[3] );
        printf( "Pixel Size = (%.6f,%.6f)\n",
                adfvGeoTransform[1], adfvGeoTransform[5] );
    } else
    {
        printf( "Missing GeoTransform Info\n");
    }

    /* fetching a vector layer */
    OGRLayer  *poLayer;
    if(poDS->GetLayerCount() == 0) {
        printf("There are no vector layers!\n");
    } else
    {
        printf("There are %d vector layers\n",poDS->GetLayerCount());
        poLayer = poDS->GetLayer(0);    // looks like this count goes from 0 to GetLayerCount()-1
        //poLayer = poDS->GetLayerByName( "point" );
        printf("vector layer name is %s\n",poLayer->GetName());
        printf("vector layer description is %s\n",poLayer->GetDescription());
        //printf("vector layer geometry type is %s\n",poLayer->GetGeomType());

        /* reading feature from a vector layer */
        OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();
        poLayer->ResetReading();
        OGRFeature *poFeature;
        while( (poFeature = poLayer->GetNextFeature()) != NULL )
        {
            for( int iField = 0; iField < poFDefn->GetFieldCount(); iField++ )
            {
                OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( iField );
                switch( poFieldDefn->GetType() )
                {
                    case OFTInteger:
                        printf( "%d,", poFeature->GetFieldAsInteger( iField ) );
                        break;
                    case OFTInteger64:
                        printf( CPL_FRMT_GIB ",", poFeature->GetFieldAsInteger64( iField ) );
                        break;
                    case OFTReal:
                        printf( "%.3f,", poFeature->GetFieldAsDouble(iField) );
                        break;
                    case OFTString:
                        printf( "%s,", poFeature->GetFieldAsString(iField) );
                        break;
                    default:
                        printf( "%s,", poFeature->GetFieldAsString(iField) );
                        break;
                }
            }
            OGRGeometry *poGeometry = poFeature->GetGeometryRef();
            if( poGeometry != NULL
                    && wkbFlatten(poGeometry->getGeometryType()) == wkbPoint )
            {
                OGRPoint *poPoint = (OGRPoint *) poGeometry;
                printf( "%.3f,%3.f\n", poPoint->getX(), poPoint->getY() );
            }
            else
            {
                printf( "no point geometry\n" );
            }
            OGRFeature::DestroyFeature( poFeature );
        }

    }



    /* close raster dataset and clean up raster memory */
    GDALClose( (GDALDatasetH) poDS );

    /* end of vector file stuff, so end of shape files */
    printf("finished vector dataset, shape file stuff\n");

    return 0;
}
