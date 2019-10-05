#include "wrfFileValue.h"

/***** public functions *****/

/*** constructor functions ***/
wrfFileValue::wrfFileValue(std::string newVariableName)
{
    variableName = newVariableName;
    storedWrfFileName = defaultWrfFileName;
}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void wrfFileValue::reset()
{
    storedWrfFileName = defaultWrfFileName;
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool wrfFileValue::set_storedWrfFileName(std::string newFilenameValue)
{
    bool conversionSuccess = isValidNetCDFFilename(newFilenameValue, false);

    if(conversionSuccess == true)
    {
        storedWrfFileName = newFilenameValue;
    }

    return conversionSuccess;
}

/*** end set value functions ***/

/*** get value functions ***/
std::string wrfFileValue::get_variableName()
{
    return variableName;
}

std::string wrfFileValue::get_storedWrfFileName()
{
    return storedWrfFileName;
}
/*** end get value functions ***/



/***** private functions *****/



/*** typechecker functions ***/
// still need to finish editing this function
bool wrfFileValue::isValidNetCDFFilename(std::string inputString, bool suppressWarnings)
{
    bool conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    GDALDatasetH  hDataset;
    std::string netCDFFileString = "NETCDF:\"" + inputString + "\"";
    CPLPushErrorHandler(&CPLQuietErrorHandler);
    hDataset = GDALOpen( netCDFFileString.c_str(), GA_ReadOnly );
    CPLPopErrorHandler();
    if(hDataset == NULL)
    {
        if(suppressWarnings == false)
        {
            printf("could not open string \"%s\" as netcdf file!\n",inputString.c_str());
        }
        conversionSuccess = false;
        GDALClose(hDataset);
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

    if(conversionSuccess == true)
    {
        /* WindNinja wrfSurfInitialization::checkForValidData() stuff */

        // important info about wrf files: https://www.ncl.ucar.edu/Applications/wrfnetcdf.shtml, http://www.openwfm.org/wiki/How_to_interpret_WRF_variables,
        //   http://www.meteo.unican.es/wiki/cordexwrf/OutputVariables, https://wiki.ucar.edu/display/acme/WRF+Ingest

        // setup desired variables from netcdf file list
        std::vector<std::string> wrf_var_list;
        wrf_var_list.push_back("T2"); // 2 m temperature, T by itself is all air temperatures in 3D space. Units are K.
        wrf_var_list.push_back("Q2"); // 2 m specific humidity. Units are 1, so I guess unitless. QVAPOR is the specific humidity for all 3D space
        wrf_var_list.push_back("RAINC"); // hourly precipitation in kg/(m^2*s), RAINC is ACCUMULATED TOTAL CUMULUS PRECIPITATION, RAINNC is ACCUMULATED TOTAL GRID SCALE PRECIPITATION so I think I need to add both to get total precipitation? Sounds like all the different precips from each different physics model technically need added together for this. For now just use RAINC. Also, accumulated is the right type?
        wrf_var_list.push_back("RAINNC");   // sounds like usually you get RAINC and RAINNC and add them both to get total hourly precip. No information on what RAINSH is, but might be another component, but not used so much so let's ignore it for now
        // technically don't need each of these RAINC and RAINNC, just if they exist or not, for now assume need both
        wrf_var_list.push_back("QCLOUD"); // WindNinja says this is the cloud water mixing ratio but uses it for the cloud cover grid. http://www.meteo.unican.es/wiki/cordexwrf/OutputVariables says it is the Column liquid water content?

        GDALDataset *srcDS;
        for(size_t wrfVarIdx = 0; wrfVarIdx < wrf_var_list.size(); wrfVarIdx++)
        {
            netCDFFileString = "NETCDF:\"" + inputString + "\":" + wrf_var_list[wrfVarIdx];
            CPLPushErrorHandler(&CPLQuietErrorHandler);
            srcDS = (GDALDataset*)GDALOpen( netCDFFileString.c_str(), GA_ReadOnly );
            CPLPopErrorHandler();
            if(srcDS == NULL)
            {
                if(suppressWarnings == false)
                {
                    printf("netcdf file \"%s\" does not have data of name \"%s\"!\n",inputString.c_str(),wrf_var_list[wrfVarIdx].c_str());
                }
                conversionSuccess = false;
            } else
            {
                //Get total bands (time steps)
                int nBands = srcDS->GetRasterCount();
                int nXSize = srcDS->GetRasterXSize();
                int nYSize = srcDS->GetRasterYSize();

                GDALRasterBand *poBand;
                double *padfScanline;
                //loop over all bands for this variable (bands are time steps)
                for(size_t bandIdx = 1; bandIdx <= nBands; bandIdx++)
                {
                    poBand = srcDS->GetRasterBand(bandIdx);

                    int pbSuccess = 0;
                    double dfNoData = poBand->GetNoDataValue( &pbSuccess );

                    bool noDataValueExists = false;
                    bool noDataIsNan = false;
                    if( pbSuccess == false )
                        noDataValueExists = false;
                    else
                    {
                        noDataValueExists = true;
                        noDataIsNan = CPLIsNan(dfNoData);
                    }

                    //set the data
                    padfScanline = new double[nXSize*nYSize];
                    pbSuccess = poBand->RasterIO(GF_Read, 0, 0, nXSize, nYSize, padfScanline,
                                     nXSize, nYSize, GDT_Float64, 0, 0);
                    if(pbSuccess == CE_Failure)
                    {
                        if(suppressWarnings == false)
                        {
                            printf("error reading raster band!\n");
                        }
                        conversionSuccess = false;
                        GDALClose((GDALDatasetH) srcDS );
                        break;
                    }
                    for(size_t dataIdx = 0; dataIdx < nXSize*nYSize; dataIdx++)
                    {
                        //Check if value is no data (if no data value was defined in file)
                        if(noDataValueExists == true)
                        {
                            if(noDataIsNan == true)
                            {
                                if(CPLIsNan(padfScanline[dataIdx]))
                                {
                                    if(suppressWarnings == false)
                                    {
                                        printf("netcdf file \"%s\" contains no_data values for data of name \"%s\"!\n",inputString.c_str(),wrf_var_list[wrfVarIdx].c_str());
                                    }
                                    conversionSuccess = false;
                                }
                            } else
                            {
                                if(padfScanline[dataIdx] == dfNoData)
                                {
                                    if(suppressWarnings == false)
                                    {
                                        printf("netcdf file \"%s\" contains no_data values for data of name \"%s\"!\n",inputString.c_str(),wrf_var_list[wrfVarIdx].c_str());
                                    }
                                    conversionSuccess = false;
                                }
                            }
                        }
                        if( wrf_var_list[wrfVarIdx] == "T2" )   //units are Kelvin
                        {
                            if(padfScanline[dataIdx] < 180.0 || padfScanline[dataIdx] > 340.0)  //these are near the most extreme temperatures ever recored on earth
                            {
                                if(suppressWarnings == false)
                                {
                                    printf("temperature is out of range for netcdf file \"%s\"!\n",inputString.c_str());
                                }
                                conversionSuccess = false;
                            }
                        } else if( wrf_var_list[wrfVarIdx] == "Q2" )  //units are unitless
                        {
                            if(padfScanline[dataIdx] < -0.0001 || padfScanline[dataIdx] > 100.0)
                            {
                                if(suppressWarnings == false)
                                {
                                    printf("specific humidity is out of range for netcdf file \"%s\"!\n",inputString.c_str());
                                }
                                conversionSuccess = false;
                            }
                        } else if( wrf_var_list[wrfVarIdx] == "RAINC" )  //units are kg/(m^2*s); units in mm if input from wrf
                        {
                            //if(padfScanline[dataIdx] < -0.0001 || padfScanline[dataIdx] > 100.0) //"Amit"
                            if(padfScanline[dataIdx] < -0.0001 || padfScanline[dataIdx] > 500.0)
                            {
                                if(suppressWarnings == false)
                                {
                                    printf("cumulus precipitation is out of range for netcdf file \"%s\"!\n",inputString.c_str());
                                }
                                conversionSuccess = false;
                            }
                        } else if( wrf_var_list[wrfVarIdx] == "RAINNC" )  //units are kg/(m^2*s); units in mm if input from wrf
                        {
                           // if(padfScanline[dataIdx] < -0.0001 || padfScanline[dataIdx] > 100.0) //"Amit"
                            if(padfScanline[dataIdx] < -0.0001 || padfScanline[dataIdx] > 500.0)
                            {
                                if(suppressWarnings == false)
                                {
                                    printf("grid scale precipitation is out of range for netcdf file \"%s\"!\n",inputString.c_str());
                                }
                                conversionSuccess = false;
                            }
                        } else if( wrf_var_list[wrfVarIdx] == "QCLOUD" )  //units are kg/kg
                        {
                            if(padfScanline[dataIdx] < -0.0001 || padfScanline[dataIdx] > 100.0)
                            {
                                if(suppressWarnings == false)
                                {
                                    printf("total cloud cover is out of range for netcdf file \"%s\"!\n",inputString.c_str());
                                }
                                conversionSuccess = false;
                            }
                        }
                    }

                    delete [] padfScanline;
                }
            }
            GDALClose((GDALDatasetH) srcDS );
        }

        /* end WindNinja wrfSurfInitialization::checkForValidData() stuff */
    }

    return conversionSuccess;
}
/*** end typechecker functions ***/
