#include "lcpFileValue.h"

/***** public functions *****/

/*** constructor functions ***/
lcpFileValue::lcpFileValue(std::string newVariableName)
{
    variableName = newVariableName;
    storedLcpFileValue = defaultLcpFileValue;
}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void lcpFileValue::reset()
{
    storedLcpFileValue = defaultLcpFileValue;
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool lcpFileValue::set_storedLcpFileValue(std::string newFilenameValue)
{
    bool conversionSuccess = isValidLcpFilename(newFilenameValue,false);
    if(conversionSuccess == false)
    {
        printf("string value \"%s\" is not a valid lcp filename!\n",newFilenameValue.c_str());
    } else
    {
        storedLcpFileValue = newFilenameValue;
    }

    return conversionSuccess;
}

/*** end set value functions ***/

/*** get value functions ***/
std::string lcpFileValue::get_variableName()
{
    return variableName;
}

std::string lcpFileValue::get_storedLcpFileValue()
{
    return storedLcpFileValue;
}
/*** end get value functions ***/



/***** private functions *****/



/*** typechecker functions ***/
// still need to finish editing this function
bool lcpFileValue::isValidLcpFilename(std::string inputString, bool suppressWarnings)
{
    bool conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    if(inputString.length() < 5)
    {
        if(suppressWarnings == false)
        {
            printf("lcp filename not long enough to have .lcp extension!\n");
        }
        conversionSuccess = false;
    }
    if(conversionSuccess == true)
    {
        if(inputString.substr(inputString.length()-4,inputString.length()) != ".lcp")
        {
            if(suppressWarnings == false)
            {
                printf("lcp filename does not have .lcp extension!\n");
            }
            conversionSuccess = false;
        }
    }
    if(conversionSuccess == true)
    {
        GDALDatasetH  hDataset;
        hDataset = GDALOpen( inputString.c_str(), GA_ReadOnly );
        if(hDataset == NULL)
        {
            if(suppressWarnings == false)
            {
                printf("could not open string \"%s\" as lcp file!\n",inputString.c_str());
            }
            conversionSuccess = false;
        } else
        {
            /* tutorial stuff /

            // getting dataset information
            GDALDriverH   hDriver;
            double        adfGeoTransform[6];
            printf( "Driver: %s/%s\n",
                    GDALGetDriverShortName( hDriver ),
                    GDALGetDriverLongName( hDriver ) );
            printf( "Size is %dx%dx%d\n",
                    GDALGetRasterXSize( hDataset ),
                    GDALGetRasterYSize( hDataset ),
                    GDALGetRasterCount( hDataset ) );
            if( GDALGetProjectionRef( hDataset ) != NULL )
                printf( "Projection is `%s'\n", GDALGetProjectionRef( hDataset ) );
            if( GDALGetGeoTransform( hDataset, adfGeoTransform ) == CE_None )
            {
                printf( "Origin = (%.6f,%.6f)\n",
                        adfGeoTransform[0], adfGeoTransform[3] );
                printf( "Pixel Size = (%.6f,%.6f)\n",
                        adfGeoTransform[1], adfGeoTransform[5] );
            }
            // finished getting dataset information

            // fetching a band
            GDALRasterBandH hBand;
            int             nBlockXSize, nBlockYSize;
            int             bGotMin, bGotMax;
            double          adfMinMax[2];
            hBand = GDALGetRasterBand( hDataset, 1 );
            GDALGetBlockSize( hBand, &nBlockXSize, &nBlockYSize );
            printf( "Block=%dx%d Type=%s, ColorInterp=%s\n",
                    nBlockXSize, nBlockYSize,
                    GDALGetDataTypeName(GDALGetRasterDataType(hBand)),
                    GDALGetColorInterpretationName(
                        GDALGetRasterColorInterpretation(hBand)) );
            adfMinMax[0] = GDALGetRasterMinimum( hBand, &bGotMin );
            adfMinMax[1] = GDALGetRasterMaximum( hBand, &bGotMax );
            if( ! (bGotMin && bGotMax) )
                GDALComputeRasterMinMax( hBand, TRUE, adfMinMax );
            printf( "Min=%.3fd, Max=%.3f\n", adfMinMax[0], adfMinMax[1] );
            if( GDALGetOverviewCount(hBand) > 0 )
                printf( "Band has %d overviews.\n", GDALGetOverviewCount(hBand));
            if( GDALGetRasterColorTable( hBand ) != NULL )
                printf( "Band has a color table with %d entries.\n",
                         GDALGetColorEntryCount(
                             GDALGetRasterColorTable( hBand ) ) );
            // end fetching a band

            // reading raster data
            float *pafScanline;
            int   nXSize = GDALGetRasterBandXSize( hBand );
            pafScanline = (float *) CPLMalloc(sizeof(float)*nXSize);
            GDALRasterIO( hBand, GF_Read, 0, 0, nXSize, 1,
                          pafScanline, nXSize, 1, GDT_Float32,
                          0, 0 );
            // end reading raster data

            // closing dataset
            CPLFree(pafScanline);
            GDALClose(hDataset);
            // end closing dataset

            / end tutorial stuff */
        }
        GDALClose(hDataset);
    }
    if(conversionSuccess == true)
    {
        std::string lcpBasePath = inputString.substr(0,inputString.length()-4);
        std::string prjFileName = lcpBasePath + ".prj";
        if(doesFileExist(prjFileName) == false)    // not sure if this one will be allowed, maybe do a function that takes in lcp filename that checks for prj specifically
        {
            if(suppressWarnings == false)
            {
                printf("prjFileName \"%s\" does not exist alongside lcp file!\n",prjFileName.c_str());
            }
            conversionSuccess = false;
        }
    }

    return conversionSuccess;
}
/*** end typechecker functions ***/

/*** utility functions ***/
/*** end utility functions ***/
