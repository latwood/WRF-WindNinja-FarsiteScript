#include "wrfGetWeather.h"

/***** public functions *****/

/*** constructor functions ***/
wrfGetWeather::wrfGetWeather()
{

}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
bool wrfGetWeather::load_required_inputs(inputVariablesHandler *inputs)
{
    // first reset all values to default values with reconstructor
    reset();

    // next load all the needed inputs from the input file
    actualCreateInputs_path = inputs->get_actualCreateInputs_path();    // do we even need this? the wrfInterpretation class will combine the RAWS stuff
        // application specific variables
    wrfGetWeather_output_units = inputs->get_inputVariableStringValue("wrfGetWeather_output_units");
        // lcp download variables
    lcp_file_path = inputs->get_inputVariableFilenameValue("lcp_file_path");    // will this be the variable we end up using? Should be a final after downloader instructions to pass around
        // wrfGetWeather and WindNinja variables
    extend_wrf_data = inputs->get_inputVariableBoolValue("extend_wrf_data");
    wrf_files = inputs->get_wrf_files();
        // wrfGetWeather only variables
    use_weather_from_ignition_center = inputs->get_inputVariableBoolValue("use_weather_from_ignition_center");
    use_weather_from_full_ignition_area = inputs->get_inputVariableBoolValue("use_weather_from_full_ignition_area");
    use_weather_from_wrf_center = inputs->get_inputVariableBoolValue("use_weather_from_wrf_center");
    // may need some ignition and lcp related variables too if doing the more complex wrf weather positioning tricks

    // now create any wrfGetWeather specific inputs that are needed from these inputs

    return true;
}
/*** end reconstructor like functions ***/

/*** functions ***/
/*
bool wrfGetWeather::getWeather()
{
    int bandNum =1;

    if(bandNum < 0)
        throw std::runtime_error("Could not match ninjaTime with a band number in the forecast file.");

    GDALDataset* poDS;
    //attempt to grab the projection from the dem?
    //check for member prjString first
    std::string dstWkt;
    //dstWkt = input.dem.prjString;
    if ( dstWkt.empty() ) {
        //try to open original
        poDS = (GDALDataset*)GDALOpen( lcp_file_path.c_str(), GA_ReadOnly );
        if( poDS == NULL ) {
            CPLDebug( "wrfInitialization::setSurfaceGrids()",
                    "Bad projection reference" );
            throw("Cannot open dem file in wrfInitialization::setSurfaceGrids()");
        }
        dstWkt = poDS->GetProjectionRef();
        if( dstWkt.empty() ) {
            CPLDebug( "wrfInitialization::setSurfaceGrids()",
                    "Bad projection reference" );
            throw("Cannot open dem file in wrfInitialization::setSurfaceGrids()");
        }
        GDALClose((GDALDatasetH) poDS );
    }

//==========get global attributes to set projection===========================
    //Acquire a lock to protect the non-thread safe netCDF library
#ifdef _OPENMP
    omp_guard netCDF_guard(netCDF_lock);
#endif
*/
    /*
     * Open the dataset
     */
    /*int status, ncid;
    status = nc_open( wrf_files[0].c_str(), 0, &ncid );
    if ( status != NC_NOERR ) {
        std::ostringstream os;
        os << "The netcdf file: " << wrf_files[0]
           << " cannot be opened\n";
        throw std::runtime_error( os.str() );
    }*/

    /*
     * Get global attribute MAP_PROJ
     * 1 = Lambert Conformal Conic
     * 2 = Polar Stereographic
     * 3 = Mercator
     * 6 = Lat/Long
     */
/*
    int mapProj;
    nc_type type;
    size_t len;
    status = nc_inq_att( ncid, NC_GLOBAL, "MAP_PROJ", &type, &len );
    if( status != NC_NOERR ){
        std::ostringstream os;
        os << "Global attribute 'MAP_PROJ' in the netcdf file: " << wrf_files[0]
        << " cannot be opened\n";
        throw std::runtime_error( os.str() );
    }
    else {
        status = nc_get_att_int( ncid, NC_GLOBAL, "MAP_PROJ", &mapProj );
    }

    //cout<<"MAP_PROJ = "<<mapProj<<endl;
*/
    /*
     * Get global attributes DX, DY
     *
     */
    /*float dx, dy;
    status = nc_inq_att( ncid, NC_GLOBAL, "DX", &type, &len );
    if( status != NC_NOERR ){
        std::ostringstream os;
        os << "Global attribute DX in the netcdf file: " << wrf_files[0]
        << " cannot be opened\n";
        throw std::runtime_error( os.str() );
    }
    else {
        status = nc_get_att_float( ncid, NC_GLOBAL, "DX", &dx );
    }
    status = nc_inq_att( ncid, NC_GLOBAL, "DY", &type, &len );
    if( status != NC_NOERR ){
        std::ostringstream os;
        os << "Global attribute DY in the netcdf file: " << wrf_files[0]
        << " cannot be opened\n";
        throw std::runtime_error( os.str() );
    }
    else {
        status = nc_get_att_float( ncid, NC_GLOBAL, "DY", &dy );
    }
*/
    /*
     * Get global attributes CEN_LAT, CEN_LON
     *
     */
   /* float cenLat, cenLon;
    status = nc_inq_att( ncid, NC_GLOBAL, "CEN_LAT", &type, &len );
    if( status != NC_NOERR ){
        std::ostringstream os;
        os << "Global attribute CEN_LAT in the netcdf file: " << wrf_files[0]
        << " cannot be opened\n";
        throw std::runtime_error( os.str() );
    }
    else {
        status = nc_get_att_float( ncid, NC_GLOBAL, "CEN_LAT", &cenLat );
    }
    status = nc_inq_att( ncid, NC_GLOBAL, "CEN_LON", &type, &len );
    if( status != NC_NOERR ){
        std::ostringstream os;
        os << "Global attribute CEN_LON in the netcdf file: " << wrf_files[0]
        << " cannot be opened\n";
        throw std::runtime_error( os.str() );
    }
    else {
        status = nc_get_att_float( ncid, NC_GLOBAL, "CEN_LON", &cenLon );
    }
*/
    /*
     * Get global attributes MOAD_CEN_LAT, STAND_LON
     *
     */
    /*float moadCenLat, standLon;
    status = nc_inq_att( ncid, NC_GLOBAL, "MOAD_CEN_LAT", &type, &len );
    if( status != NC_NOERR ){
        std::ostringstream os;
        os << "Global attribute MOAD_CEN_LAT in the netcdf file: " << wrf_files[0]
        << " cannot be opened\n";
        throw std::runtime_error( os.str() );
    }
    else {
        status = nc_get_att_float( ncid, NC_GLOBAL, "MOAD_CEN_LAT", &moadCenLat );
    }
    status = nc_inq_att( ncid, NC_GLOBAL, "STAND_LON", &type, &len );
    if( status != NC_NOERR ){
        std::ostringstream os;
        os << "Global attribute STAND_LON in the netcdf file: " << wrf_files[0]
        << " cannot be opened\n";
        throw std::runtime_error( os.str() );
    }
    else {
        status = nc_get_att_float( ncid, NC_GLOBAL, "STAND_LON", &standLon );
    }
*/
    /*
     * Get global attributes TRUELAT1, TRUELAT2
     *
     */
    /*float trueLat1, trueLat2;
    status = nc_inq_att( ncid, NC_GLOBAL, "TRUELAT1", &type, &len );
    if( status != NC_NOERR ){
        std::ostringstream os;
        os << "Global attribute TRUELAT1 in the netcdf file: " << wrf_files[0]
        << " cannot be opened\n";
        throw std::runtime_error( os.str() );
    }
    else {
        status = nc_get_att_float( ncid, NC_GLOBAL, "TRUELAT1", &trueLat1 );
    }
    status = nc_inq_att( ncid, NC_GLOBAL, "TRUELAT2", &type, &len );
    if( status != NC_NOERR ){
        std::ostringstream os;
        os << "Global attribute TRUELAT2 in the netcdf file: " << wrf_files[0]
        << " cannot be opened\n";
        throw std::runtime_error( os.str() );
    }
    else {
        status = nc_get_att_float( ncid, NC_GLOBAL, "TRUELAT2", &trueLat2 );
    }
*/
    /*
     * Get global attribute BOTTOM-TOP_GRID_DIMENSION
     *
     */
    /*int wxModel_nLayers = 0;
    status = nc_inq_att( ncid, NC_GLOBAL, "BOTTOM-TOP_GRID_DIMENSION", &type, &len );
    if( status != NC_NOERR ){
        std::ostringstream os;
        os << "Global attribute BOTTOM-TOP_GRID_DIMENSION  in the netcdf file: "
        << wrf_files[0]
        << " cannot be opened\n";
        throw std::runtime_error( os.str() );
    }
    else {
        status = nc_get_att_int( ncid, NC_GLOBAL, "BOTTOM-TOP_GRID_DIMENSION", &wxModel_nLayers );
    }
*/
    /*
     * Get global attribute WEST-EAST_GRID_DIMENSION
     * Not currently used. WX model x/y dims set based on
     * reprojected image (in DEM space).
     */
    /*status = nc_inq_att( ncid, NC_GLOBAL, "WEST-EAST_GRID_DIMENSION", &type, &len );
    if( status != NC_NOERR ){
        ostringstream os;
        os << "Global attribute WEST-EAST_GRID_DIMENSION  in the netcdf file: "
        << wxModelFileName
        << " cannot be opened\n";
        throw std::runtime_error( os.str() );
    }
    else {
        status = nc_get_att_int( ncid, NC_GLOBAL, "WEST-EAST_GRID_DIMENSION", &wxModel_nCols );
    }*/

    /*
     * Get global attribute SOUTH-NORTH_GRID_DIMENSION
     * Not currently used. WX model x/y dims set based on
     * reprojected image (in DEM space).
     */
    /*status = nc_inq_att( ncid, NC_GLOBAL, "SOUTH-NORTH_GRID_DIMENSION", &type, &len );
    if( status != NC_NOERR ){
        ostringstream os;
        os << "Global attribute SOUTH-NORTH_GRID_DIMENSION  in the netcdf file: "
        << wxModelFileName
        << " cannot be opened\n";
        throw std::runtime_error( os.str() );
    }
    else {
        status = nc_get_att_int( ncid, NC_GLOBAL, "SOUTH-NORTH_GRID_DIMENSION", &wxModel_nRows );
    }*/

    /*
     * Close the dataset
     *
     */
    /*status = nc_close( ncid );
    if( status != NC_NOERR ) {
        std::ostringstream os;
        os << "The netcdf file: " << wrf_files[0]
           << " cannot be closed\n";
        throw std::runtime_error( os.str() );
    }

//======end get global attributes========================================

    CPLPushErrorHandler(&CPLQuietErrorHandler);
    poDS = (GDALDataset*)GDALOpenShared( wrf_files[0].c_str(), GA_ReadOnly );
    CPLPopErrorHandler();
    if( poDS == NULL ) {
        CPLDebug( "wrfInitialization::setSurfaceGrids()",
                 "Bad forecast file");
        //throw badForecastFile("Cannot open forecast file in wrfInitialization::setSurfaceGrids()");
        return false;
    }
    else {
        GDALClose((GDALDatasetH) poDS ); // close original wxModel file
    }

    // open ds one by one, set projection, warp, then write to grid
    GDALDataset *srcDS, *wrpDS;
    std::string temp;

    std::vector<std::string> varList;
    varList.push_back("T2"); // 2 m temperature, T by itself is all air temperatures in 3D space. Units are K.
    varList.push_back("Q2"); // 2 m specific humidity. Units are 1, so I guess unitless. QVAPOR is the specific humidity for all 3D space
    varList.push_back("RAINC"); // hourly precipitation in kg/(m^2*s), RAINC is ACCUMULATED TOTAL CUMULUS PRECIPITATION, RAINNC is ACCUMULATED TOTAL GRID SCALE PRECIPITATION so I think I need to add both to get total precipitation? Sounds like all the different precips from each different physics model technically need added together for this. For now just use RAINC. Also, accumulated is the right type?
    varList.push_back("RAINNC");   // sounds like usually you get RAINC and RAINNC and add them both to get total hourly precip. No information on what RAINSH is, but might be another component, but not used so much so let's ignore it for now
    // technically don't need each of these RAINC and RAINNC, just if they exist or not, for now assume need both
    varList.push_back("QCLOUD"); // WindNinja says this is the cloud water mixing ratio but uses it for the cloud cover grid. http://www.meteo.unican.es/wiki/cordexwrf/OutputVariables says it is the Column liquid water content?
*/
    /*
     * Set the initial values in the warped dataset to no data
     */
   /* GDALWarpOptions* psWarpOptions;

    for( unsigned int i = 0;i < varList.size();i++ ) {

        temp = "NETCDF:\"" + wrf_files[0] + "\":" + varList[i];

        CPLPushErrorHandler(&CPLQuietErrorHandler);
        srcDS = (GDALDataset*)GDALOpenShared( temp.c_str(), GA_ReadOnly );
        CPLPopErrorHandler();
        if( srcDS == NULL ) {
            CPLDebug( "wrfInitialization::setSurfaceGrids()",
                    "Bad forecast file" );
        }

        CPLDebug("WX_MODEL_INITIALIZATION", "varList[i] = %s", varList[i].c_str());
*/
        /*
         * Set up spatial reference stuff for setting projections
         * and geotransformations
         */
/*
        std::string projString;
        if(mapProj == 1){  //lambert conformal conic
            projString = "PROJCS[\"WGC 84 / WRF Lambert\",GEOGCS[\"WGS 84\",DATUM[\"World Geodetic System 1984\",\
                          SPHEROID[\"WGS 84\",6378137.0,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],AUTHORITY[\"EPSG\",\"6326\"]],\
                          PRIMEM[\"Greenwich\",0.0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.017453292519943295],\
                          AXIS[\"Geodetic longitude\",EAST],AXIS[\"Geodetic latitude\",NORTH],AUTHORITY[\"EPSG\",\"4326\"]],\
                          PROJECTION[\"Lambert_Conformal_Conic_2SP\"],\
                          PARAMETER[\"central_meridian\","+boost::lexical_cast<std::string>(standLon)+"],\
                          PARAMETER[\"latitude_of_origin\","+boost::lexical_cast<std::string>(moadCenLat)+"],\
                          PARAMETER[\"standard_parallel_1\","+boost::lexical_cast<std::string>(trueLat1)+"],\
                          PARAMETER[\"false_easting\",0.0],PARAMETER[\"false_northing\",0.0],\
                          PARAMETER[\"standard_parallel_2\","+boost::lexical_cast<std::string>(trueLat2)+"],\
                          UNIT[\"m\",1.0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH]]";
        }
        else if(mapProj == 2){  //polar stereographic
            projString = "PROJCS[\"WGS 84 / Antarctic Polar Stereographic\",GEOGCS[\"WGS 84\",\
                         DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],\
                         AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],\
                         UNIT[\"degree\",0.01745329251994328,AUTHORITY[\"EPSG\",\"9122\"]],\
                         AUTHORITY[\"EPSG\",\"4326\"]],UNIT[\"metre\",1,AUTHORITY[\"EPSG\",\"9001\"]],\
                         PROJECTION[\"Polar_Stereographic\"],\
                         PARAMETER[\"latitude_of_origin\","+boost::lexical_cast<std::string>(moadCenLat)+"],\
                         PARAMETER[\"central_meridian\","+boost::lexical_cast<std::string>(standLon)+"],\
                         PARAMETER[\"scale_factor\",1],\
                         PARAMETER[\"false_easting\",0],\
                         PARAMETER[\"false_northing\",0],AUTHORITY[\"EPSG\",\"3031\"],\
                         AXIS[\"Easting\",UNKNOWN],AXIS[\"Northing\",UNKNOWN]]";
        }
        else if(mapProj == 3){  //mercator
            projString = "PROJCS[\"World_Mercator\",GEOGCS[\"GCS_WGS_1984\",DATUM[\"WGS_1984\",\
                          SPHEROID[\"WGS_1984\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],\
                          UNIT[\"Degree\",0.017453292519943295]],PROJECTION[\"Mercator_1SP\"],\
                          PARAMETER[\"False_Easting\",0],\
                          PARAMETER[\"False_Northing\",0],\
                          PARAMETER[\"Central_Meridian\","+boost::lexical_cast<std::string>(standLon)+"],\
                          PARAMETER[\"latitude_of_origin\","+boost::lexical_cast<std::string>(moadCenLat)+"],\
                          UNIT[\"Meter\",1]]";
        }
        else if(mapProj == 6){  //lat/long
            //throw badForecastFile("Cannot initialize with a forecast file in lat/long spacing. \
                                   Forecast file must be in a projected coordinate system.");
                                           return false;
        }
        else return false;//throw badForecastFile("Cannot determine projection from the forecast file information.");

        OGRSpatialReference oSRS, *poLatLong;
        char *srcWKT = NULL;
        char* prj2 = (char*)projString.c_str();
        oSRS.importFromWkt(&prj2);
        oSRS.exportToWkt(&srcWKT);

        CPLDebug("WX_MODEL_INITIALIZATION", "srcWKT= %s", srcWKT);

        OGRCoordinateTransformation *poCT;
        poLatLong = oSRS.CloneGeogCS();
*/
        /*
         * Transform domain center from lat/long to WRF space
         */
       /* double zCenter;
        zCenter = 0;
        double xCenter, yCenter;
        xCenter = (double)cenLon;
        yCenter = (double)cenLat;

        poCT = OGRCreateCoordinateTransformation(poLatLong, &oSRS);
        //delete poLatLong;
        OSRDestroySpatialReference(poLatLong);

        if(poCT==NULL || !poCT->Transform(1, &xCenter, &yCenter))
            printf("Transformation failed.\n");

        CPLDebug("WX_MODEL_INITIALIZATION", "xCenter, yCenter= %f, %f", xCenter, yCenter);
        printf("xcenter = \"%f\", ycenter = \"%f\"\n",xCenter,yCenter);
*/
        /*
         * Set the geostransform for the WRF file
         * upper corner is calculated from transformed x, y
         * (in WRF space)
         */
/*
        double ncols, nrows;
        int nXSize = srcDS->GetRasterXSize();
        int nYSize = srcDS->GetRasterYSize();
        ncols = (double)(nXSize)/2;
        nrows = (double)(nYSize)/2;

        double adfGeoTransform[6] = {(xCenter-(ncols*dx)), dx, 0,
                                    (yCenter+(nrows*dy)),
                                    0, -(dy)};

        CPLDebug("WX_MODEL_INITIALIZATION", "ulcornerX, ulcornerY= %f, %f", (xCenter-(ncols*dx)), (yCenter+(nrows*dy)));
        CPLDebug("WX_MODEL_INITIALIZATION", "nXSize, nYsize= %d, %d", nXSize, nYSize);
        CPLDebug("WX_MODEL_INITIALIZATION", "dx= %f", dx);

        srcDS->SetGeoTransform(adfGeoTransform);

*/
        /*if( varList[i] == "U10" ) {  // just a check
            AsciiGrid<double> tempSrcGrid;
            GDAL2AsciiGrid( srcDS, 12, tempSrcGrid );
            tempSrcGrid.write_Grid("before_warp", 2);
        }*/

        /*
         * Grab the first band to get the nodata value for the variable,
         * assume all bands have the same ndv
         */
      /*  GDALRasterBand *poBand = srcDS->GetRasterBand( 1 );
        int pbSuccess;
        double dfNoData = poBand->GetNoDataValue( &pbSuccess );
        printf("dfNoData = \"%f\"\n",dfNoData);

        psWarpOptions = GDALCreateWarpOptions();

        int nBandCount = srcDS->GetRasterCount();

        printf("nBandCount = \"%d\"\n",nBandCount);
        psWarpOptions->nBandCount = nBandCount;

        CPLDebug("WX_MODEL_INITIALIZATION", "band count = %d", nBandCount);

        psWarpOptions->padfDstNoDataReal =
            (double*) CPLMalloc( sizeof( double ) * nBandCount );
        psWarpOptions->padfDstNoDataImag =
            (double*) CPLMalloc( sizeof( double ) * nBandCount );

        for( int b = 0;b < srcDS->GetRasterCount();b++ ) {
            psWarpOptions->padfDstNoDataReal[b] = dfNoData;
            psWarpOptions->padfDstNoDataImag[b] = dfNoData;
        }

        if( pbSuccess == false )
            dfNoData = -9999.0;

        psWarpOptions->papszWarpOptions =
            CSLSetNameValue( psWarpOptions->papszWarpOptions,
                            "INIT_DEST", "NO_DATA" );
        char **temp = psWarpOptions->papszWarpOptions;
        printf("printing psWarpOptions options\n");
        while (*temp != NULL) {
            printf("%s ", *temp);
            temp++;
        }


        printf("\nsrcWkt = \"%s\"\n\n",srcWKT);
        printf("prjRef = \"%s\"\n\n",dstWkt.c_str());
        wrpDS = (GDALDataset*) GDALAutoCreateWarpedVRT( srcDS, srcWKT,
                                                        dstWkt.c_str(),
                                                        GRA_NearestNeighbour,
                                                        1.0, psWarpOptions );

        //=======for testing==================================//
        /*AsciiGrid<double> tempGrid;
        AsciiGrid<double> temp2Grid;

        if( varList[i] == "U10" ) {
            GDAL2AsciiGrid( wrpDS, 12, tempGrid );
            if( CPLIsNan( dfNoData ) ) {
                tempGrid.set_noDataValue(-9999.0);
                tempGrid.replaceNan( -9999.0 );
            }
            tempGrid.write_Grid("after_warp", 2);


            //Make final grids with same header as dem
            temp2Grid.set_headerData(input.dem);
            temp2Grid.interpolateFromGrid(tempGrid, AsciiGrid<double>::order1);
            temp2Grid.set_noDataValue(-9999.0);
            temp2Grid.write_Grid("after_interpolation", 2);
        }*/
        //=======end testing=================================//

       /* CPLDebug("WX_MODEL_INITIALIZATION", "band number to write = %d", bandNum);

        CPLFree(srcWKT);
        //delete poCT;
        OCTDestroyCoordinateTransformation(poCT);
        GDALDestroyWarpOptions( psWarpOptions );
        GDALClose((GDALDatasetH) srcDS );
        GDALClose((GDALDatasetH) wrpDS );
    }

    return true;
}*/

// expects there to be only 1 times worth of data

bool wrfGetWeather::getWeather()
{
    // setup desired variables from netcdf file list
    std::vector<std::string> wrf_var_list;
    wrf_var_list.push_back("T2"); // 2 m temperature, T by itself is all air temperatures in 3D space. Units are K.
    wrf_var_list.push_back("Q2"); // 2 m specific humidity. Units are 1, so I guess unitless. QVAPOR is the specific humidity for all 3D space
    wrf_var_list.push_back("RAINC"); // hourly precipitation in kg/(m^2*s), RAINC is ACCUMULATED TOTAL CUMULUS PRECIPITATION, RAINNC is ACCUMULATED TOTAL GRID SCALE PRECIPITATION so I think I need to add both to get total precipitation? Sounds like all the different precips from each different physics model technically need added together for this. For now just use RAINC. Also, accumulated is the right type?
    wrf_var_list.push_back("RAINNC");   // sounds like usually you get RAINC and RAINNC and add them both to get total hourly precip. No information on what RAINSH is, but might be another component, but not used so much so let's ignore it for now
    // technically don't need each of these RAINC and RAINNC, just if they exist or not, for now assume need both
    wrf_var_list.push_back("QCLOUD"); // WindNinja says this is the cloud water mixing ratio but uses it for the cloud cover grid. http://www.meteo.unican.es/wiki/cordexwrf/OutputVariables says it is the Column liquid water content?

    // first get the lcp projection as the projection the wrf files will be transformed to
    GDALDataset* poDS;
    std::string dstWkt;
    poDS = (GDALDataset*)GDALOpen( lcp_file_path.c_str(), GA_ReadOnly );
    if( poDS == NULL )
    {
        printf("could not open lcp file \"%s\" to get projection info for wrf netcdf files!\n",lcp_file_path.c_str());
        return false;
    }
    dstWkt = poDS->GetProjectionRef();
    if( dstWkt.empty() )
    {
        printf("could not get projection info from lcp file \"%s\" for wrf netcdf files!\n",lcp_file_path.c_str());
        return false;
    }
    GDALClose((GDALDatasetH) poDS );    // not sure why they just check to open, unless the projection string is what they were really after


    // go through each wrf file, then each dataset as needed
    for(size_t wrfFileIdx = 0; wrfFileIdx < wrf_files.size(); wrfFileIdx++)
    {
      // now use netcdf opening functions to get the netcdf projection info from the wrf files, which is the projection to be transformed from
      // long and honestly it really needs this much extra processing?
        int mapProj = 0;
        float dx = 0.0;
        float dy = 0.0;
        float cenLat = 0.0;
        float cenLon = 0.0;
        float moadCenLat = 0.0;
        float standLon = 0.0;
        float trueLat1 = 0.0;
        float trueLat2 = 0.0;
        int nLayers = 0;
        if(getNetCDFGlobalAttributes(wrf_files[wrfFileIdx], mapProj, dx, dy, cenLat, cenLon, moadCenLat, standLon, trueLat1, trueLat2, nLayers) == false)
        {
            printf("problem getting netcdf global attributes!\n");
            return false;
        }

        // try opening it just for the heck of it
        CPLPushErrorHandler(&CPLQuietErrorHandler);
        poDS = (GDALDataset*)GDALOpenShared( wrf_files[wrfFileIdx].c_str(), GA_ReadOnly );
        CPLPopErrorHandler();
        if( poDS == NULL )
        {
            printf("problem opening wrf file \"%s\" is a bad forecast!\n",wrf_files[wrfFileIdx].c_str());
            return false;
        } else {
            GDALClose((GDALDatasetH) poDS ); // close original wxModel file
        }

      // now finally open the dataset in gdal for processing :)
        GDALDataset *srcDS;
        GDALDataset *wrpDS;
        GDALWarpOptions *psWarpOptions;
        for(size_t wrfVarIdx = 0; wrfVarIdx < wrf_var_list.size(); wrfVarIdx++)
        {
            std::string netCDFFileString = "NETCDF:\"" + wrf_files[wrfFileIdx] + "\":" + wrf_var_list[wrfVarIdx];
            CPLPushErrorHandler(&CPLQuietErrorHandler);
            srcDS = (GDALDataset*)GDALOpenShared( netCDFFileString.c_str(), GA_ReadOnly );
            CPLPopErrorHandler();
            if(srcDS == NULL)
            {
                printf("netcdf file \"%s\" does not have data of name \"%s\"!\n",wrf_files[wrfFileIdx].c_str(),wrf_var_list[wrfVarIdx].c_str());
                return false;
            }

          // setup possible projections based on found mapProj for the netcdf file
            // Global attribute MAP_PROJ
            // 1 = Lambert Conformal Conic
            // 2 = Polar Stereographic
            // 3 = Mercator
            // 6 = Lat/Long
            std::string projString;
            if(mapProj == 1)    //lambert conformal conic
            {
                projString = "PROJCS[\"WGC 84 / WRF Lambert\",GEOGCS[\"WGS 84\",DATUM[\"World Geodetic System 1984\",\
                              SPHEROID[\"WGS 84\",6378137.0,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],AUTHORITY[\"EPSG\",\"6326\"]],\
                              PRIMEM[\"Greenwich\",0.0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.017453292519943295],\
                              AXIS[\"Geodetic longitude\",EAST],AXIS[\"Geodetic latitude\",NORTH],AUTHORITY[\"EPSG\",\"4326\"]],\
                              PROJECTION[\"Lambert_Conformal_Conic_2SP\"],\
                              PARAMETER[\"central_meridian\","+boost::lexical_cast<std::string>(standLon)+"],\
                              PARAMETER[\"latitude_of_origin\","+boost::lexical_cast<std::string>(moadCenLat)+"],\
                              PARAMETER[\"standard_parallel_1\","+boost::lexical_cast<std::string>(trueLat1)+"],\
                              PARAMETER[\"false_easting\",0.0],PARAMETER[\"false_northing\",0.0],\
                              PARAMETER[\"standard_parallel_2\","+boost::lexical_cast<std::string>(trueLat2)+"],\
                              UNIT[\"m\",1.0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH]]";
            } else if(mapProj == 2)   //polar stereographic
            {
                projString = "PROJCS[\"WGS 84 / Antarctic Polar Stereographic\",GEOGCS[\"WGS 84\",\
                             DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],\
                             AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],\
                             UNIT[\"degree\",0.01745329251994328,AUTHORITY[\"EPSG\",\"9122\"]],\
                             AUTHORITY[\"EPSG\",\"4326\"]],UNIT[\"metre\",1,AUTHORITY[\"EPSG\",\"9001\"]],\
                             PROJECTION[\"Polar_Stereographic\"],\
                             PARAMETER[\"latitude_of_origin\","+boost::lexical_cast<std::string>(moadCenLat)+"],\
                             PARAMETER[\"central_meridian\","+boost::lexical_cast<std::string>(standLon)+"],\
                             PARAMETER[\"scale_factor\",1],\
                             PARAMETER[\"false_easting\",0],\
                             PARAMETER[\"false_northing\",0],AUTHORITY[\"EPSG\",\"3031\"],\
                             AXIS[\"Easting\",UNKNOWN],AXIS[\"Northing\",UNKNOWN]]";
            } else if(mapProj == 3)  //mercator
            {
                projString = "PROJCS[\"World_Mercator\",GEOGCS[\"GCS_WGS_1984\",DATUM[\"WGS_1984\",\
                              SPHEROID[\"WGS_1984\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],\
                              UNIT[\"Degree\",0.017453292519943295]],PROJECTION[\"Mercator_1SP\"],\
                              PARAMETER[\"False_Easting\",0],\
                              PARAMETER[\"False_Northing\",0],\
                              PARAMETER[\"Central_Meridian\","+boost::lexical_cast<std::string>(standLon)+"],\
                              PARAMETER[\"latitude_of_origin\","+boost::lexical_cast<std::string>(moadCenLat)+"],\
                              UNIT[\"Meter\",1]]";
            } else if(mapProj == 6)  //lat/long
            {
                printf("Cannot initialize wrf file \"%s\" in lat/long spacing! Forecast file must be in a projected coordinate system!\n",wrf_files[wrfFileIdx].c_str());
                return false;
            } else
            {
                printf("Cannot determine projection from the forecast file information for wrf file \"%s\"!\n",wrf_files[wrfFileIdx].c_str());
                return false;
            }

            // now do projection transform :)
            OGRSpatialReference oSRS, *poLatLong;
            char *srcWKT = NULL;
            char* prj2 = (char*)projString.c_str();
            oSRS.importFromWkt(&prj2);
            oSRS.exportToWkt(&srcWKT);

            OGRCoordinateTransformation *poCT;
            poLatLong = oSRS.CloneGeogCS();


            // Transform copy of domain center points from lat/long to WRF space
            double zCenter = 0;
            double xCenter = (double)cenLon;
            double yCenter = (double)cenLat;

            poCT = OGRCreateCoordinateTransformation(poLatLong, &oSRS);
            delete poLatLong;

            if(poCT==NULL || !poCT->Transform(1, &xCenter, &yCenter))
            {
                printf("projection transformation failed for wrf file \"%s\"!\n",wrf_files[wrfFileIdx].c_str());
                return false;
            }

            // now manually calculate the new needed geoTransform information and set the transform
            int nXSize = srcDS->GetRasterXSize();
            int nYSize = srcDS->GetRasterYSize();
            double ncols = (double)(nXSize)/2;
            double nrows = (double)(nYSize)/2;

            double adfGeoTransform[6] = {(xCenter-(ncols*dx)), dx, 0,
                                        (yCenter+(nrows*dy)),
                                        0, -(dy)};
            srcDS->SetGeoTransform(adfGeoTransform);

            // get other information needed for the final transformation
                // Grab the first band to get the nodata value for the variable, assume all bands have the same ndv
            GDALRasterBand *poBand = srcDS->GetRasterBand( 1 );
            int pbSuccess;
            double dfNoData = poBand->GetNoDataValue( &pbSuccess );

            // now getting information for setting up and setting up warpOptions (so the options needed by transform utility call)
            psWarpOptions = GDALCreateWarpOptions();

            int nBandCount = srcDS->GetRasterCount();

            psWarpOptions->nBandCount = nBandCount; // set the count size

            psWarpOptions->padfDstNoDataReal =
                (double*) CPLMalloc( sizeof( double ) * nBandCount );   // allocate the needed data slots
            psWarpOptions->padfDstNoDataImag =
                (double*) CPLMalloc( sizeof( double ) * nBandCount );   // allocate the needed data slots

            for( int psWarpBandIdx = 0; psWarpBandIdx < srcDS->GetRasterCount(); psWarpBandIdx++ )
            {
                psWarpOptions->padfDstNoDataReal[psWarpBandIdx] = dfNoData;   // fill new data with noData value, probably replacing nulls and has to do with making sure they are all filled
                psWarpOptions->padfDstNoDataImag[psWarpBandIdx] = dfNoData;
            }

            if( pbSuccess == false )    // not sure what this is for as dfNoData is not used past this point, except to replace nan values with this value
            {   // so this might not be needed for what I'm doing with the data
                dfNoData = -9999.0;
            }

            psWarpOptions->papszWarpOptions =
                CSLSetNameValue( psWarpOptions->papszWarpOptions,
                                "INIT_DEST", "NO_DATA" );

            /*char **temp = psWarpOptions->papszWarpOptions;
            printf("printing psWarpOptions options\n");
            while (*temp != NULL) {
                printf("%s ", *temp);
                temp++;
            }*/


            // now perform final transformation on whole dataset (the warping lol)
            wrpDS = (GDALDataset*) GDALAutoCreateWarpedVRT( srcDS, srcWKT, dstWkt.c_str(), GRA_NearestNeighbour, 1.0, psWarpOptions );


          // now finally do stuff with the data phew

            // okay so the data has been transformed to be in the same coordinate system as the lcp file
            // looks like the normal thing is to get the new sizes and geotransform coordinates, find the corner, grab header info as needed,
            // then do a scanline to read the data one line at a time, putting it into a grid of values, basically make sure it is within the allowed range,
            // and just throw it in using a desired access and allocation order

            // I need to either find the value at the center of the wrf file, easiest
            // find the value that is at the center of the lcp file, more correct but means finding the center of lcp file, then which point is closest to that in wrf file
            // from ignition center would be rough, what constitutes an ignition center?
            // an average of all locations that would be included as the ignition seems harder than the above, but more plausible than the ignition center one

            // so I think I need to use some form of gdallocationinfo, or copy its tricks. Makes sense for a single point, but not so much for multiple points.
            // for multiple points would be easier to transform into lat long (another transformation, yeah! but now it IS transformable so data will be right)
            // then would have to figure out a logical way to find closest lat long points for multiple places,
            // I guess would want to grab all points that are within a certain range, then narrow it down based off of what else is being used

            // hm, center of wrf file would be halfway through the bands, halfway through the line of data? Let's do that for now
            // what if not evenly divisible by 2? average of what I get, weighted somehow by the point. Man this still would need lat long transform then

            // gdal also appears to have some handy grid interpolation functions

            // yeah for now let's just throw something together that can get a single value
            // so far, assuming data is always in the first band
            // also, haven't taken into account unit conversions yet!!! Will see what happens when it runs lol
            double foundVar = 0.0;
            if(findWrfCenterValue( wrpDS, 1, foundVar ) == false)
            {
                printf("problem finding wrf center \"%s\" value for wrf file \"%s\"!\n",wrf_var_list[wrfVarIdx].c_str(),wrf_files[wrfFileIdx].c_str());
                return false;
            } else
            {
                if( wrf_var_list[wrfVarIdx] == "T2" )
                {
                    temperatures.push_back(foundVar);
                } else if( wrf_var_list[wrfVarIdx] == "Q2" )
                {
                    if(foundVar < 0.0)
                    {
                        foundVar = 0.0;
                    }
                    humidities.push_back(foundVar);
                } else if( wrf_var_list[wrfVarIdx] == "RAINC" )
                {
                    if(foundVar < 0.0)
                    {
                        foundVar = 0.0;
                    }
                    totalPrecip.push_back(foundVar);
                } else if( wrf_var_list[wrfVarIdx] == "RAINNC" )
                {
                    if(foundVar < 0.0)
                    {
                        foundVar = 0.0;
                    }
                    totalPrecip[wrfFileIdx] = totalPrecip[wrfFileIdx] + foundVar;
                } else if( wrf_var_list[wrfVarIdx] == "QCLOUD" )
                {
                    if(foundVar < 0.0)
                    {
                        foundVar = 0.0;
                    }
                    foundVar = foundVar/100.0;
                    cloudCover.push_back(foundVar);
                }
            }
            CPLFree(srcWKT);
            delete poCT;
            GDALDestroyWarpOptions( psWarpOptions );
            GDALClose((GDALDatasetH) srcDS );
            GDALClose((GDALDatasetH) wrpDS );
        }
    }

    printf("\nFound Weather values:\n");
    for(size_t varIdx = 0; varIdx < temperatures.size(); varIdx++)
    {
        printf("temperatures[%zu] = \"%f\"\n",varIdx,temperatures[varIdx]);
    }
    for(size_t varIdx = 0; varIdx < humidities.size(); varIdx++)
    {
        printf("humidities[%zu] = \"%f\"\n",varIdx,humidities[varIdx]);
    }
    for(size_t varIdx = 0; varIdx < totalPrecip.size(); varIdx++)
    {
        printf("totalPrecip[%zu] = \"%f\"\n",varIdx,totalPrecip[varIdx]);
    }
    for(size_t varIdx = 0; varIdx < cloudCover.size(); varIdx++)
    {
        printf("cloudCover[%zu] = \"%f\"\n",varIdx,cloudCover[varIdx]);
    }

    // got here somehow so should have gone well
    return true;
}
/*** end functions ***/

/*** get variable functions ***/
std::vector<double> wrfGetWeather::get_temperatures()
{
    return temperatures;
}

std::vector<double> wrfGetWeather::get_humidities()
{
    return humidities;
}

std::vector<double> wrfGetWeather::get_totalPrecip()
{
    return totalPrecip;
}

std::vector<double> wrfGetWeather::get_cloudCover()
{
    return cloudCover;
}
/*** end get variable functions ***/



/***** private functions *****/



/*** reconstructor like functions ***/
void wrfGetWeather::reset()
{
    // data members needed from the inputs
    actualCreateInputs_path = "";
        // application specific variables
    wrfGetWeather_output_units = "";
        // lcp download variables
    lcp_file_path = "";
        // wrfGetWeather and WindNinja variables
    extend_wrf_data = false;
    while(!wrf_files.empty())
    {
        wrf_files.pop_back();
    }
        // wrfGetWeather only variables
    use_weather_from_ignition_center = false;
    use_weather_from_full_ignition_area = false;
    use_weather_from_wrf_center = false;
    // may need some ignition and lcp related variables too if doing the more complex wrf weather positioning tricks

    // data members created from inputs that are API specific
    while(!temperatures.empty())
    {
        temperatures.pop_back();
    }
    while(!humidities.empty())
    {
        humidities.pop_back();
    }
    while(!totalPrecip.empty())
    {
        totalPrecip.pop_back();
    }
    while(!cloudCover.empty())
    {
        cloudCover.pop_back();
    }

}
/*** end reconstructor like functions ***/

/*** useful utility functions ***/
bool wrfGetWeather::getNetCDFGlobalAttributes(std::string wrfFile, int &mapProj, float &dx, float &dy, float &cenLat, float &cenLon,
                                              float &moadCenLat,float &standLon, float &trueLat1, float &trueLat2, int &nLayers)
{
    bool success = true;

    // attempt to open the file
    int ncid = 0;
    int status = nc_open( wrfFile.c_str(), 0, &ncid );
    if ( status != NC_NOERR )
    {
      printf("The netcdf file \"%s\" cannot be opened using netcdf stuff!\n",wrfFile.c_str());
      success = false;
    }

    // Get global attribute MAP_PROJ
    // 1 = Lambert Conformal Conic
    // 2 = Polar Stereographic
    // 3 = Mercator
    // 6 = Lat/Long
    nc_type type;
    size_t len = 0;
    status = nc_inq_att( ncid, NC_GLOBAL, "MAP_PROJ", &type, &len );
    if( status != NC_NOERR )
    {
      printf("Global attribute 'MAP_PROJ' cannot be opened for netcdf file \"%s\"!\n",wrfFile.c_str());
      success = false;
    } else
    {
      status = nc_get_att_int( ncid, NC_GLOBAL, "MAP_PROJ", &mapProj );
    }

    // get dx
    status = nc_inq_att( ncid, NC_GLOBAL, "DX", &type, &len );
    if( status != NC_NOERR )
    {
      printf("Global attribute 'DX' cannot be opened for netcdf file \"%s\"!\n",wrfFile.c_str());
      success = false;
    } else
    {
      status = nc_get_att_float( ncid, NC_GLOBAL, "DX", &dx );
    }

    // get dy
    status = nc_inq_att( ncid, NC_GLOBAL, "DY", &type, &len );
    if( status != NC_NOERR )
    {
      printf("Global attribute 'DY' cannot be opened for netcdf file \"%s\"!\n",wrfFile.c_str());
      success = false;
    } else
    {
      status = nc_get_att_float( ncid, NC_GLOBAL, "DY", &dy );
    }

    // get center latitude
    status = nc_inq_att( ncid, NC_GLOBAL, "CEN_LAT", &type, &len );
    if( status != NC_NOERR )
    {
      printf("Global attribute 'CEN_LAT' cannot be opened for netcdf file \"%s\"!\n",wrfFile.c_str());
      success = false;
    } else
    {
      status = nc_get_att_float( ncid, NC_GLOBAL, "CEN_LAT", &cenLat );
    }

    // get center long
    status = nc_inq_att( ncid, NC_GLOBAL, "CEN_LON", &type, &len );
    if( status != NC_NOERR )
    {
      printf("Global attribute 'CEN_LON' cannot be opened for netcdf file \"%s\"!\n",wrfFile.c_str());
      success = false;
    } else
    {
      status = nc_get_att_float( ncid, NC_GLOBAL, "CEN_LON", &cenLon );
    }

    // get whatever MOAD_CEN_LAT is
    status = nc_inq_att( ncid, NC_GLOBAL, "MOAD_CEN_LAT", &type, &len );
    if( status != NC_NOERR )
    {
      printf("Global attribute 'MOAD_CEN_LAT' cannot be opened for netcdf file \"%s\"!\n",wrfFile.c_str());
      success = false;
    } else
    {
      status = nc_get_att_float( ncid, NC_GLOBAL, "MOAD_CEN_LAT", &moadCenLat );
    }

    // get whatever standlon longitude is
    status = nc_inq_att( ncid, NC_GLOBAL, "STAND_LON", &type, &len );
    if( status != NC_NOERR )
    {
      printf("Global attribute 'STAND_LON' cannot be opened for netcdf file \"%s\"!\n",wrfFile.c_str());
      success = false;
    } else
    {
      status = nc_get_att_float( ncid, NC_GLOBAL, "STAND_LON", &standLon );
    }

    // get TRUE_LAT1
    status = nc_inq_att( ncid, NC_GLOBAL, "TRUELAT1", &type, &len );
    if( status != NC_NOERR )
    {
      printf("Global attribute 'TRUELAT1' cannot be opened for netcdf file \"%s\"!\n",wrfFile.c_str());
      success = false;
    } else
    {
      status = nc_get_att_float( ncid, NC_GLOBAL, "TRUELAT1", &trueLat1 );
    }

    // get TRUE_LAT2
    status = nc_inq_att( ncid, NC_GLOBAL, "TRUELAT2", &type, &len );
    if( status != NC_NOERR )
    {
      printf("Global attribute 'TRUELAT2' cannot be opened for netcdf file \"%s\"!\n",wrfFile.c_str());
      success = false;
    } else
    {
      status = nc_get_att_float( ncid, NC_GLOBAL, "TRUELAT2", &trueLat2 );
    }

    // get global attribute BOTTOM-TOP_GRID_DIMENSION
    status = nc_inq_att( ncid, NC_GLOBAL, "BOTTOM-TOP_GRID_DIMENSION", &type, &len );
    if( status != NC_NOERR )
    {
      printf("Global attribute 'BOTTOM-TOP_GRID_DIMENSION' cannot be opened for netcdf file \"%s\"!\n",wrfFile.c_str());
      success = false;
    } else
    {
      status = nc_get_att_int( ncid, NC_GLOBAL, "BOTTOM-TOP_GRID_DIMENSION", &nLayers );
    }

    // close the dataset
    status = nc_close( ncid );
    if( status != NC_NOERR )
    {
      printf("netcdf file \"%s\" cannot be closed!\n",wrfFile.c_str());
      success = false;
    }

    // got here somehow so it was fine
    return success;
}

bool wrfGetWeather::findWrfCenterValue(GDALDataset *poDS, int band, double &foundValue)
{
    bool success = true;

    if (poDS == NULL)
    {
        return false;
    }

    // get number of data points and geotranform information (corners)
    int nXSize = poDS->GetRasterXSize();
    int nYSize = poDS->GetRasterYSize();

    int xCenterStart = floor(nXSize/2.0);
    int xCenterEnd = ceil(nXSize/2.0);
    double xCenterCount = xCenterEnd - xCenterStart;

    int yCenterStart = floor(nYSize/2.0);
    int yCenterEnd = ceil(nYSize/2.0);
    double yCenterCount = yCenterEnd - yCenterStart;

    GDALRasterBand *poBand;
    poBand = poDS->GetRasterBand( band );
    int pbSuccess;
    double dfNoDataValue = poBand->GetNoDataValue( &pbSuccess );

    double *padfScanline;
    padfScanline = new double[nXSize];
    double avgValue = 0;
    size_t nanCount = 0;
    size_t validValueCount = 0;
    if(xCenterCount == 0 && yCenterCount == 0)
    {
        pbSuccess = poBand->RasterIO(GF_Read, 0, yCenterStart, nXSize, 1, padfScanline, nXSize, 1, GDT_Float64, 0, 0);
        if(pbSuccess == CE_Failure)
        {
            printf("error reading raster band!\n");
            return false;
        }
        if(padfScanline[xCenterStart] == dfNoDataValue)
        {
            nanCount = nanCount + 1;
        } else
        {
            avgValue = avgValue + padfScanline[xCenterStart];
            validValueCount = validValueCount + 1;
        }
    } else if(xCenterCount != 0 && yCenterCount == 0)
    {
        pbSuccess = poBand->RasterIO(GF_Read, 0, yCenterStart, nXSize, 1, padfScanline, nXSize, 1, GDT_Float64, 0, 0);
        if(pbSuccess == CE_Failure)
        {
            printf("error reading raster band!\n");
            return false;
        }
        for(size_t xCount = xCenterStart; xCount < xCenterEnd; xCount++)
        {
            if(padfScanline[xCount] == dfNoDataValue)
            {
                nanCount = nanCount + 1;
            } else
            {
                avgValue = avgValue + padfScanline[xCount];
                validValueCount = validValueCount + 1;
            }
        }
    } else if(xCenterCount == 0 && yCenterCount != 0)
    {
        pbSuccess = poBand->RasterIO(GF_Read, 0, xCenterStart, nXSize, 1, padfScanline, nXSize, 1, GDT_Float64, 0, 0);
        if(pbSuccess == CE_Failure)
        {
            printf("error reading raster band!\n");
            return false;
        }
        for(size_t yCount = yCenterStart; yCount < yCenterEnd; yCount++)
        {
            if(padfScanline[yCount] == dfNoDataValue)
            {
                nanCount = nanCount + 1;
            } else
            {
                avgValue = avgValue + padfScanline[yCount];
                validValueCount = validValueCount + 1;
            }
        }
    } else
    {
        for(size_t yCount = yCenterStart; yCount < yCenterEnd; yCount++)
        {
            pbSuccess = poBand->RasterIO(GF_Read, 0, yCount, nXSize, 1, padfScanline, nXSize, 1, GDT_Float64, 0, 0);
            if(pbSuccess == CE_Failure)
            {
                printf("error reading raster band!\n");
                return false;
            }

            for(size_t xCount = xCenterStart; xCount < xCenterEnd; xCount++)
            {
                if(padfScanline[xCount] == dfNoDataValue)
                {
                    nanCount = nanCount + 1;
                } else
                {
                    avgValue = avgValue + padfScanline[xCount];
                    validValueCount = validValueCount + 1;
                }
            }
        }
    }

    if(xCenterCount + yCenterCount - nanCount > 0)
    {
        avgValue = avgValue/(xCenterCount + yCenterCount);
    }

    if(validValueCount == 0)
    {
        printf("no valid values in band! nanCount = %zu\n",nanCount);
        success = false;
    }

    delete [] padfScanline;

    foundValue = avgValue;

    return success;
}
/*** end useful utility functions ***/
