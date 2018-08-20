#include "farsiteAPI.h"

/***** public functions *****/

/*** constructor functions ***/
farsiteAPI::farsiteAPI()
{

}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
bool farsiteAPI::load_required_inputs(inputVariablesHandler *inputs, createIgnitions *ignitions, WindNinjaAPI *windAPI, wrfGetWeather *weatherAPI)
{
    // first reset all values to default values with reconstructor
    reset();

    // next load all the needed inputs from the input file
    actual_run_base_name = inputs->get_actual_run_base_name();
    actualCreateInputs_path = inputs->get_actualCreateInputs_path();
    actualFinalOutput_path = inputs->get_actualFinalOutput_path();
    actualLcpPath = inputs->get_actualLcpPath();
        // application specific variables
    createIgnition_output_units = inputs->get_inputVariableStringValue("createIgnition_output_units");
    WindNinja_required_output_units = inputs->get_inputVariableStringValue("WindNinja_required_output_units");
    wrfGetWeather_output_units = inputs->get_inputVariableStringValue("wrfGetWeather_output_units");
    farsite_output_units = inputs->get_inputVariableStringValue("farsite_output_units");
    use_native_timezone = inputs->get_inputVariableBoolValue("use_native_timezone");
        // WindNinjaAPI variables
    WindNinja_number_of_threads = inputs->get_inputVariableSize_tValue("WindNinja_number_of_threads");
        // farsiteAPI variables
    burn_start_month = inputs->get_inputVariableDateValueMonth("burn_start_time");
    burn_start_day = inputs->get_inputVariableDateValueDay("burn_start_time");
    burn_start_year = inputs->get_inputVariableDateValueYear("burn_start_time");
    burn_start_hour = inputs->get_inputVariableDateValueHour("burn_start_time");
    burn_start_minute = inputs->get_inputVariableDateValueMinute("burn_start_time");
    burn_end_month = inputs->get_inputVariableDateValueMonth("burn_end_time");
    burn_end_day = inputs->get_inputVariableDateValueDay("burn_end_time");
    burn_end_year = inputs->get_inputVariableDateValueYear("burn_end_time");
    burn_end_hour = inputs->get_inputVariableDateValueHour("burn_end_time");
    burn_end_minute = inputs->get_inputVariableDateValueMinute("burn_end_time");
    farsite_barrier_shapefile = inputs->get_inputVariableFilenameValue("farsite_barrier_shapefile");
    farsite_spot_probability = inputs->get_inputVariableDoubleValue("farsite_spot_probability");
    farsite_spot_ignition_delay = inputs->get_inputVariableSize_tValue("farsite_spot_ignition_delay");
    farsite_spotting_seed = inputs->get_inputVariableSize_tValue("farsite_spotting_seed");
    farsite_earliest_burn_time_hour = inputs->get_inputVariableHour_MinValueHour("farsite_earliest_burn_time");
    farsite_earliest_burn_time_minute = inputs->get_inputVariableHour_MinValueMinute("farsite_earliest_burn_time");
    farsite_latest_burn_time_hour = inputs->get_inputVariableHour_MinValueHour("farsite_latest_burn_time");
    farsite_latest_burn_time_minute = inputs->get_inputVariableHour_MinValueMinute("farsite_latest_burn_time");
    farsite_foliar_moisture_content = inputs->get_inputVariableDoubleValue("farsite_foliar_moisture_content");
    farsite_crown_fire_method = inputs->get_inputVariableStringValue("farsite_crown_fire_method");

    // now load all variables needed from the createIgnitions class
    ignitionShapefilesForSimulations = ignitions->get_ignitionShapefilesForSimulations();

    // now load all variables needed from the WindNinjaAPI class
    atmFiles = windAPI->get_atmFilePaths();
    velFiles = windAPI->get_velFilePaths();
    angFiles = windAPI->get_angFilePaths();
    cldFiles = windAPI->get_cldFilePaths();
    wrfYears = windAPI->get_wrfYears();
    wrfMonths = windAPI->get_wrfMonths();
    wrfDays = windAPI->get_wrfDays();
    wrfHours = windAPI->get_wrfHours();
    wrfMinutes = windAPI->get_wrfMinutes();
    wrfSeconds = windAPI->get_wrfSeconds();
    wrfTimeZones = windAPI->get_wrfTimeZones();

    // now load all variables needed from the wrfGetWeather class
    temperatures = weatherAPI->get_temperatures();
    humidities = weatherAPI->get_humidities();
    totalPrecip = weatherAPI->get_totalPrecip();
    cloudCover = weatherAPI->get_cloudCover();

    // data members created from inputs that are farsiteAPI specific
    farsite_start_time = std::to_string(burn_start_month) + " " + std::to_string(burn_start_day) + " " + std::to_string(burn_start_hour) + std::to_string(burn_start_minute);
    farsite_end_time =  std::to_string(burn_end_month) + " " + std::to_string(burn_end_day) + " " + std::to_string(burn_end_hour) + std::to_string(burn_end_minute);
    farsite_timestep = "60";
    farsite_distance_res = "30.0";
    farsite_perimeter_res = "60.0";
    farsite_min_ignition_vertex_distance = "15.0";
    farsite_spot_grid_resolution = "15.0";
    farsite_minimum_spot_distance = "30";
    farsite_acceleration_on = "1";
    farsite_fill_barriers = "1";
    farsite_default_fuel_mositures_data = "0 3 4 6 70 100";

    // extra farsiteAPI variables
    farsiteMainCreateInputsFolderPath = actualCreateInputs_path + "/farsite";
    for(size_t ignitCount = 0; ignitCount < ignitionShapefilesForSimulations.size(); ignitCount++)
    {
        // probably should do the naming check during the inputs checking stuff to make sure no duplications by this point or something
        std::string ignitionFileBaseName = findBaseName(ignitionShapefilesForSimulations[ignitCount].c_str());
        printf("ignitionFileBaseName[%zu] = \"%s\"\n",ignitCount,ignitionFileBaseName.c_str());

        std::string farsiteCreateInputsFolderPath = farsiteMainCreateInputsFolderPath + "/" + ignitionFileBaseName;
        farsiteCreateInputRunFolderPaths.push_back(farsiteCreateInputsFolderPath);
        printf("farsiteCreateInputsFolderPath[%zu] = \"%s\"\n",ignitCount,farsiteCreateInputsFolderPath.c_str());

        std::string farsiteFinalOutputFolderPath = actualFinalOutput_path + "/" + ignitionFileBaseName;
        farsiteFinalOutputRunFolderPaths.push_back(farsiteFinalOutputFolderPath);
        printf("farsiteFinalOutputFolderPath[%zu] = \"%s\"\n",ignitCount,farsiteFinalOutputFolderPath.c_str());

        std::string farsiteCommandFileName = farsiteCreateInputsFolderPath + "/run_" + ignitionFileBaseName + ".txt";
        farsiteCommandFiles.push_back(farsiteCommandFileName);
        printf("farsiteCommandFileName[%zu] = \"%s\"\n",ignitCount,farsiteCommandFileName.c_str());

        std::string farsiteLcpFileName = farsiteCreateInputsFolderPath + "/" + findBaseName(actualLcpPath);
        farsiteLcpFiles.push_back(farsiteLcpFileName);
        printf("farsiteLcpFileName[%zu] = \"%s\"\n",ignitCount,farsiteLcpFileName.c_str());

        std::string farsiteInputFileName = farsiteCreateInputsFolderPath + "/" + ignitionFileBaseName + ".input";
        farsiteInputFiles.push_back(farsiteInputFileName);
        printf("farsiteInputFileName[%zu] = \"%s\"\n",ignitCount,farsiteInputFileName.c_str());

        std::string farsiteIgnitionFileName = farsiteCreateInputsFolderPath + "/" + findBaseName(ignitionShapefilesForSimulations[ignitCount]);
        farsiteIgnitionFiles.push_back(farsiteIgnitionFileName);
        printf("farsiteIgnitionFileName[%zu] = \"%s\"\n",ignitCount,farsiteIgnitionFileName.c_str());

        if(farsite_barrier_shapefile != "") // is optional, should be this value if it was never filled
        {
            std::string farsiteBarrierFileName = farsiteCreateInputsFolderPath + "/" + findBaseName(farsite_barrier_shapefile);
            farsiteBarrierFiles.push_back(farsiteBarrierFileName);
            printf("farsiteBarrierFileName[%zu] = \"%s\"\n",ignitCount,farsiteBarrierFileName.c_str());
        }

        std::string farsiteRawsFileName = farsiteCreateInputsFolderPath + "/" + ignitionFileBaseName + ".raws";
        farsiteRawsFiles.push_back(farsiteRawsFileName);
        printf("farsiteRawsFileName[%zu] = \"%s\"\n",ignitCount,farsiteRawsFileName.c_str());

        std::string farsiteAtmFileName = farsiteCreateInputsFolderPath + "/" + ignitionFileBaseName + ".atm";
        farsiteAtmFiles.push_back(farsiteAtmFileName);
        printf("farsiteAtmFileName[%zu] = \"%s\"\n",ignitCount,farsiteAtmFileName.c_str());

        std::vector<std::string> farsiteVelFileNameVector;
        for(size_t fileIdx = 0; fileIdx < velFiles.size(); fileIdx++)
        {
            std::string farsiteVelFileName = farsiteCreateInputsFolderPath + "/" + findBaseName(velFiles[fileIdx]);
            farsiteVelFileNameVector.push_back(farsiteVelFileName);
            printf("farsiteVelFileName[%zu] = \"%s\"\n",ignitCount,farsiteVelFileName.c_str());
        }
        farsiteVelFiles.push_back(farsiteVelFileNameVector);

        std::vector<std::string> farsiteAngFileNameVector;
        for(size_t fileIdx = 0; fileIdx < angFiles.size(); fileIdx++)
        {
            std::string farsiteAngFileName = farsiteCreateInputsFolderPath + "/" + findBaseName(angFiles[fileIdx]);
            farsiteAngFileNameVector.push_back(farsiteAngFileName);
            printf("farsiteAngFileName[%zu] = \"%s\"\n",ignitCount,farsiteAngFileName.c_str());
        }
        farsiteAngFiles.push_back(farsiteAngFileNameVector);

        std::vector<std::string> farsiteCldFileNameVector;
        for(size_t fileIdx = 0; fileIdx < cldFiles.size(); fileIdx++)
        {
            std::string farsiteCldFileName = farsiteCreateInputsFolderPath + "/" + findBaseName(cldFiles[fileIdx]);
            farsiteCldFileNameVector.push_back(farsiteCldFileName);
            printf("farsiteCldFileName[%zu] = \"%s\"\n",ignitCount,farsiteCldFileName.c_str());
        }
        farsiteCldFiles.push_back(farsiteCldFileNameVector);

    }
    farsiteApplicationPath = "/home/atw09001/src/farsite/src/TestFARSITE";

    // for farsite burn period stuff
    // this can run now cause wrf file times has been found
    uniqueDates = findUniqueDates();

    // if it gets here, everything went well
    return true;
}
/*** end reconstructor like functions ***/

/*** functions ***/
bool farsiteAPI::createAllFarsiteInputs()
{
    // first make needed overall directory
    if(doesFolderExist(farsiteMainCreateInputsFolderPath) == false)
    {
            // now make the directory
        VSIMkdir( farsiteMainCreateInputsFolderPath.c_str(), 0777 );
    }

    // now for each run, make all the necessary files and folders
    for(size_t runIdx = 0; runIdx < farsiteCreateInputRunFolderPaths.size(); runIdx++)
    {
        // first make the input and output directories for the farsite run
        if(doesFolderExist(farsiteCreateInputRunFolderPaths[runIdx]) == false)
        {
                // now make the directory
            VSIMkdir( farsiteCreateInputRunFolderPaths[runIdx].c_str(), 0777 );
        }
        if(doesFolderExist(farsiteFinalOutputRunFolderPaths[runIdx]) == false)
        {
                // now make the directory
            VSIMkdir( farsiteFinalOutputRunFolderPaths[runIdx].c_str(), 0777 );
        }

       // now copy files
        // first the velocity files
        for(size_t fileIdx = 0; fileIdx < farsiteVelFiles[runIdx].size(); fileIdx++)
        {
            std::string velFileOutput = farsiteVelFiles[runIdx][fileIdx] + ".asc";
            if(copyFile(velFiles[fileIdx],velFileOutput) == false)
            {
                printf("problem copying velocity .asc file!\n");
                return false;
            }
            std::string prjFileInput = findBaseNameWithPath(velFiles[fileIdx]) + ".prj";
            std::string prjFileOutput = farsiteVelFiles[runIdx][fileIdx] + ".prj";
            if(copyFile(prjFileInput,prjFileOutput) == false)
            {
                printf("problem copying velocity .prj file!\n");
                return false;
            }
        }

        // now the angle files
        for(size_t fileIdx = 0; fileIdx < farsiteAngFiles[runIdx].size(); fileIdx++)
        {
            std::string angFileOutput = farsiteAngFiles[runIdx][fileIdx] + ".asc";
            if(copyFile(angFiles[fileIdx],angFileOutput) == false)
            {
                printf("problem copying angle .asc file!\n");
                return false;
            }
            std::string prjFileInput = findBaseNameWithPath(angFiles[fileIdx]) + ".prj";
            std::string prjFileOutput = farsiteAngFiles[runIdx][fileIdx] + ".prj";
            if(copyFile(prjFileInput,prjFileOutput) == false)
            {
                printf("problem copying angle .prj file!\n");
                return false;
            }
        }

        // now the cloud cover files
        for(size_t fileIdx = 0; fileIdx < farsiteCldFiles[runIdx].size(); fileIdx++)
        {
            std::string cldFileOutput = farsiteCldFiles[runIdx][fileIdx] + ".asc";
            if(copyFile(cldFiles[fileIdx],cldFileOutput) == false)
            {
                printf("problem copying cloud cover .asc file!\n");
                return false;
            }
            std::string prjFileInput = findBaseNameWithPath(cldFiles[fileIdx]) + ".prj";
            std::string prjFileOutput = farsiteCldFiles[runIdx][fileIdx] + ".prj";
            if(copyFile(prjFileInput,prjFileOutput) == false)
            {
                printf("problem copying cloud cover .prj file!\n");
                return false;
            }
        }

        // now the barrier file if it exists (is optional)
        if(farsiteBarrierFiles.size() != 0)
        {
            std::string inputBarrierBaseNameAndPath = findBaseNameWithPath(farsite_barrier_shapefile);
            std::string barrierShpFileOutput = farsiteBarrierFiles[runIdx] + ".shp";
            if(copyFile(farsite_barrier_shapefile,barrierShpFileOutput) == false)
            {
                printf("problem copying barrier .shp file!\n");
                return false;
            }
            std::string barrierDbfFileInput = inputBarrierBaseNameAndPath + ".dbf";
            std::string barrierDbfFileOutput = farsiteBarrierFiles[runIdx] + ".dbf";
            if(copyFile(barrierDbfFileInput,barrierDbfFileOutput) == false)
            {
                printf("problem copying barrier .dbf file!\n");
                return false;
            }
            std::string barrierShxFileInput = inputBarrierBaseNameAndPath + ".shx";
            std::string barrierShxFileOutput = farsiteBarrierFiles[runIdx] + ".shx";
            if(copyFile(barrierShxFileInput,barrierShxFileOutput) == false)
            {
                printf("problem copying barrier .shx file!\n");
                return false;
            }
            std::string barrierPrjFileInput = inputBarrierBaseNameAndPath + ".prj";
            std::string barrierPrjFileOutput = farsiteBarrierFiles[runIdx] + ".prj";    // this one might be optional
            if(doesFilenameExist(barrierPrjFileInput) == true)
            {
                if(copyFile(barrierPrjFileInput,barrierPrjFileOutput) == false)
                {
                    printf("problem copying barrier .prj file!\n");
                    return false;
                }
            }
        }

        // now the ignition files
        std::string inputIgnitionBaseNameAndPath = findBaseNameWithPath(ignitionShapefilesForSimulations[runIdx]);
        std::string ignitionShpFileOutput = farsiteIgnitionFiles[runIdx] + ".shp";
        if(copyFile(ignitionShapefilesForSimulations[runIdx],ignitionShpFileOutput) == false)
        {
            printf("problem copying ignition .shp file!\n");
            return false;
        }
        std::string ignitionDbfFileInput = inputIgnitionBaseNameAndPath + ".dbf";
        std::string ignitionDbfFileOutput = farsiteIgnitionFiles[runIdx] + ".dbf";
        if(copyFile(ignitionDbfFileInput,ignitionDbfFileOutput) == false)
        {
            printf("problem copying ignition .dbf file!\n");
            return false;
        }
        std::string ignitionShxFileInput = inputIgnitionBaseNameAndPath + ".shx";
        std::string ignitionShxFileOutput = farsiteIgnitionFiles[runIdx] + ".shx";
        if(copyFile(ignitionShxFileInput,ignitionShxFileOutput) == false)
        {
            printf("problem copying ignition .shx file!\n");
            return false;
        }
        std::string ignitionPrjFileInput = inputIgnitionBaseNameAndPath + ".prj";
        std::string ignitionPrjFileOutput = farsiteIgnitionFiles[runIdx] + ".prj";    // this one might be optional
        if(doesFilenameExist(ignitionPrjFileInput) == true)
        {
            if(copyFile(ignitionPrjFileInput,ignitionPrjFileOutput) == false)
            {
                printf("problem copying ignition .prj file!\n");
                return false;
            }
        }

        // now the lcp file
        std::string lcpFileOutput = farsiteLcpFiles[runIdx] + ".lcp";
        if(copyFile(actualLcpPath,lcpFileOutput) == false)
        {
            printf("problem copying lcp .lcp file!\n");
            return false;
        }
        std::string prjFileInput = findBaseNameWithPath(actualLcpPath) + ".prj";
        std::string prjFileOutput = farsiteLcpFiles[runIdx] + ".prj";
        if(copyFile(prjFileInput,prjFileOutput) == false)
        {
            printf("problem copying lcp .prj file!\n");
            return false;
        }

       // now write the combination atm file
        if(setupFinalAtmFiles(runIdx) == false)
        {
            printf("problem setting up final .atm file!\n");
            return false;
        }

       // now write Raws file
        if(createRawsFile(runIdx) == false)
        {
            printf("problem creating raws file!\n");
            return false;
        }

       // now write the input file
        if(writeFarsiteInputFile(runIdx) == false)
        {
            printf("problem writing farsite input file!\n");
            return false;
        }

       // now write the command line file
        if(writeFarsiteCommandFile(runIdx) == false)
        {
            printf("problem writing farsite command file!\n");
            return false;
        }

    }

    // if it gets here, everything went well
    return true;
}

bool farsiteAPI::runFarsite()
{
    if(isExecutable(farsiteApplicationPath.c_str()) == false)
    {
        printf("farsiteApplicationPath \"%s\" is not a valid executable program!\n",farsiteApplicationPath.c_str());
        return false;
    }

    for(size_t runIdx = 0; runIdx < farsiteInputFiles.size(); runIdx++)
    {
        if(doesFilenameExist(farsiteInputFiles[runIdx]) == false)
        {
            printf("farsite input file \"%s\" does not exist!\n",farsiteInputFiles[runIdx].c_str());
            return false;
        }
        std::string execution_command = farsiteApplicationPath + " " + farsiteInputFiles[runIdx];
        exec_cmd(execution_command.c_str());
    }

    // if it gets here, everything went well
    return true;
}

bool farsiteAPI::createAdditionalFarsiteResults()
{
    // if it gets here, everything went well
    return true;
}
/*** end functions ***/



/***** private functions *****/



/*** reconstructor like functions ***/
void farsiteAPI::reset()
{
    // data members needed from the inputs
    actual_run_base_name = "";
    actualCreateInputs_path = "";
    actualFinalOutput_path = "";
    actualLcpPath = "";
        // application specific variables
    createIgnition_output_units = "";
    WindNinja_required_output_units = "";
    wrfGetWeather_output_units = "";
    farsite_output_units = "";
    use_native_timezone = false;
        // WindNinja only variables
    WindNinja_number_of_threads = 0;
        // farsiteAPI variables
    burn_start_month = 0;
    burn_start_day = 0;
    burn_start_year = 0;
    burn_start_hour = 0;
    burn_start_minute = 0;
    burn_end_month = 0;
    burn_end_day = 0;
    burn_end_year = 0;
    burn_end_hour = 0;
    burn_end_minute = 0;
    farsite_barrier_shapefile = "";
    farsite_spot_probability = 0.0;
    farsite_spot_ignition_delay = 0;
    farsite_spotting_seed = 0;
    farsite_earliest_burn_time_hour = 0;
    farsite_earliest_burn_time_minute = 0;
    farsite_latest_burn_time_hour = 0;
    farsite_latest_burn_time_minute = 0;
    farsite_foliar_moisture_content = 0.0;
    farsite_crown_fire_method = "";

    // data members needed from createIgnitions class
    while(!ignitionShapefilesForSimulations.empty())
    {
        ignitionShapefilesForSimulations.pop_back();
    }

    // data members needed from WindNinjaAPI class
    while(!atmFiles.empty())
    {
        atmFiles.pop_back();
    }
    while(!velFiles.empty())
    {
        velFiles.pop_back();
    }
    while(!angFiles.empty())
    {
        angFiles.pop_back();
    }
    while(!cldFiles.empty())
    {
        cldFiles.pop_back();
    }
    while(!wrfYears.empty())
    {
        wrfYears.pop_back();
    }
    while(!wrfMonths.empty())
    {
        wrfMonths.pop_back();
    }
    while(!wrfDays.empty())
    {
        wrfDays.pop_back();
    }
    while(!wrfHours.empty())
    {
        wrfHours.pop_back();
    }
    while(!wrfMinutes.empty())
    {
        wrfMinutes.pop_back();
    }
    while(!wrfSeconds.empty())
    {
        wrfSeconds.pop_back();
    }
    while(!wrfTimeZones.empty())
    {
        wrfTimeZones.pop_back();
    }

    // data members needed from wrfGetWeather class
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

    // data members created from inputs that are farsiteAPI specific
    farsite_start_time = "";
    farsite_end_time = "";
    farsite_timestep = "";
    farsite_distance_res = "";
    farsite_perimeter_res = "";
    farsite_min_ignition_vertex_distance = "";
    farsite_spot_grid_resolution = "";
    farsite_minimum_spot_distance = "";
    farsite_acceleration_on = "";
    farsite_fill_barriers = "";
    farsite_default_fuel_mositures_data = "";

    // extra farsiteAPI variables
    farsiteMainCreateInputsFolderPath = "";
    while(!farsiteCreateInputRunFolderPaths.empty())
    {
        farsiteCreateInputRunFolderPaths.pop_back();
    }
    while(!farsiteFinalOutputRunFolderPaths.empty())
    {
        farsiteFinalOutputRunFolderPaths.pop_back();
    }
    while(!farsiteCommandFiles.empty())
    {
        farsiteCommandFiles.pop_back();
    }
    while(!farsiteLcpFiles.empty())
    {
        farsiteLcpFiles.pop_back();
    }
    while(!farsiteInputFiles.empty())
    {
        farsiteInputFiles.pop_back();
    }
    while(!farsiteIgnitionFiles.empty())
    {
        farsiteIgnitionFiles.pop_back();
    }
    while(!farsiteBarrierFiles.empty())
    {
        farsiteBarrierFiles.pop_back();
    }
    while(!farsiteRawsFiles.empty())
    {
        farsiteRawsFiles.pop_back();
    }
    while(!farsiteAtmFiles.empty())
    {
        farsiteAtmFiles.pop_back();
    }
    while(!farsiteVelFiles.empty())
    {
        farsiteVelFiles.pop_back();
    }
    while(!farsiteAngFiles.empty())
    {
        farsiteAngFiles.pop_back();
    }
    while(!farsiteCldFiles.empty())
    {
        farsiteCldFiles.pop_back();
    }
    farsiteApplicationPath = "";

    // for farsite burn period stuff
    while(!uniqueDates.empty())
    {
        uniqueDates.pop_back();
    }

}
/*** end reconstructor like functions ***/

/*** create farsite input functions ***/
bool farsiteAPI::setupFinalAtmFiles(size_t runIdx)
{
    FILE *fzout;
    fzout = fopen(farsiteAtmFiles[runIdx].c_str(), "w");
    for(size_t atmFileIdx = 0; atmFileIdx < atmFiles.size(); atmFileIdx++)
    {
        std::string velFileName = velFiles[atmFileIdx] + ".asc";
        std::string angFileName = angFiles[atmFileIdx] + ".asc";
        fprintf(fzout,"%s %s %s%s %s %s\n",wrfMonths[atmFileIdx].c_str(),wrfDays[atmFileIdx].c_str(),wrfHours[atmFileIdx].c_str(),
               wrfMinutes[atmFileIdx].c_str(),velFileName.c_str(),angFileName.c_str());
    }
    fclose(fzout);

    // if it gets here, everything went well
    return true;
}

bool farsiteAPI::createRawsFile(size_t runIdx)
{
    std::string guess_RAWS_ELEVATION = "500"; // 2 m was wrf data, but I don't know if it is safe to go that low
    std::string fake_WindSpeed = "999"; // still have to supply it, but is ignored since gridded winds will be used
    std::string fake_WindDir = "180";   // still have to supply it, but is ignored since gridded winds will be used
    FILE *fzout;
    fzout = fopen(farsiteRawsFiles[runIdx].c_str(), "w");
    fprintf(fzout,"RAWS_ELEVATION: %s\n",guess_RAWS_ELEVATION.c_str());
    fprintf(fzout,"RAWS_UNITS: %s\n",wrfGetWeather_output_units.c_str());
    fprintf(fzout,"RAWS: %zu\n",atmFiles.size());
    for(size_t atmFileIdx = 0; atmFileIdx < atmFiles.size(); atmFileIdx++)
    {
        fprintf(fzout,"%s %s %s%s %f %f %f %s %s %f\n",wrfMonths[atmFileIdx].c_str(),wrfDays[atmFileIdx].c_str(),wrfHours[atmFileIdx].c_str(),
               wrfMinutes[atmFileIdx].c_str(),temperatures[atmFileIdx],humidities[atmFileIdx],totalPrecip[atmFileIdx],
               fake_WindSpeed.c_str(),fake_WindDir.c_str(),cloudCover[atmFileIdx]);
    }
    fclose(fzout);

    // if it gets here, everything went well
    return true;
}

bool farsiteAPI::writeFarsiteInputFile(size_t runIdx)
{
    FILE *fzout;
    fzout = fopen(farsiteInputFiles[runIdx].c_str(), "w");
    fprintf(fzout,"FARSITE INPUTS FILE VERSION 1.0\n");
    fprintf(fzout,"FARSITE_START_TIME: %s\n",farsite_start_time.c_str());
    fprintf(fzout,"FARSITE_END_TIME: %s\n",farsite_end_time.c_str());
    fprintf(fzout,"FARSITE_TIMESTEP: %s\n",farsite_timestep.c_str());
    fprintf(fzout,"FARSITE_DISTANCE_RES: %s\n",farsite_distance_res.c_str());
    fprintf(fzout,"FARSITE_PERIMETER_RES: %s\n",farsite_perimeter_res.c_str());
    fprintf(fzout,"FARSITE_MIN_IGNITION_VERTEX_DISTANCE: %s\n",farsite_min_ignition_vertex_distance.c_str());
    fprintf(fzout,"FARSITE_SPOT_GRID_RESOLUTION: %s\n",farsite_spot_grid_resolution.c_str());
    fprintf(fzout,"FARSITE_SPOT_PROBABILITY: %f\n",farsite_spot_probability);
    fprintf(fzout,"FARSITE_SPOT_IGNITION_DELAY: %zu\n",farsite_spot_ignition_delay);
    fprintf(fzout,"FARSITE_MINIMUM_SPOT_DISTANCE: %s\n",farsite_minimum_spot_distance.c_str());
    fprintf(fzout,"FARSITE_ACCELERATION_ON: %s\n",farsite_acceleration_on.c_str());
    fprintf(fzout,"FARSITE_FILL_BARRIERS: %s\n",farsite_fill_barriers.c_str());
    fprintf(fzout,"SPOTTING_SEED: %zu\n",farsite_spotting_seed);
    fprintf(fzout,"\n");
    fprintf(fzout,"FARSITE_BURN_PERIODS: %zu\n",uniqueDates.size());
    for(size_t uniqueDatesIdx = 0; uniqueDatesIdx < uniqueDates.size(); uniqueDatesIdx++)
    {
        fprintf(fzout,"%s %d%d %d%d\n",uniqueDates[uniqueDatesIdx].c_str(),farsite_earliest_burn_time_hour,farsite_earliest_burn_time_minute,
               farsite_latest_burn_time_hour,farsite_latest_burn_time_minute);
    }
    fprintf(fzout,"\n");
    fprintf(fzout,"FUEL_MOISTURES_DATA: 1\n");
    fprintf(fzout,"%s\n",farsite_default_fuel_mositures_data.c_str());
    fprintf(fzout,"\n");
    fprintf(fzout,"RAWS_FILE: %s\n",farsiteRawsFiles[runIdx].c_str());
    fprintf(fzout,"\n");
    fprintf(fzout,"FARSITE_ATM_FILE: %s\n",farsiteAtmFiles[runIdx].c_str());
    fprintf(fzout,"\n");
    fprintf(fzout,"FOLIAR_MOISTURE_CONTENT: %f\n",farsite_foliar_moisture_content);
    fprintf(fzout,"CROWN_FIRE_METHOD: %s\n",farsite_crown_fire_method.c_str());
    fprintf(fzout,"NUMBER_PROCESSORS: %zu\n",WindNinja_number_of_threads);
    fclose(fzout);

    // if it gets here, everything went well
    return true;
}

bool farsiteAPI::writeFarsiteCommandFile(size_t runIdx)
{
    std::string lcpFile = farsiteLcpFiles[runIdx] + ".lcp";
    std::string ignitionFile = farsiteIgnitionFiles[runIdx] + ".shp";
    std::string outputFilePath = farsiteFinalOutputRunFolderPaths[runIdx] + "/" + findBaseName(actual_run_base_name);
    FILE *fzout;
    fzout = fopen(farsiteCommandFiles[runIdx].c_str(), "w");
    if(farsiteBarrierFiles.size() == 0)
    {
        fprintf(fzout,"%s %s %s 0 %s 0\n",lcpFile.c_str(),farsiteInputFiles[runIdx].c_str(),ignitionFile.c_str(),outputFilePath.c_str());
    } else
    {
        std::string barrierFile = farsiteBarrierFiles[runIdx] + ".shp";
        fprintf(fzout,"%s %s %s %s %s 0\n",lcpFile.c_str(),farsiteInputFiles[runIdx].c_str(),ignitionFile.c_str(),barrierFile.c_str(),outputFilePath.c_str());
    }
    fclose(fzout);

    // if it gets here, everything went well
    return true;
}

std::vector<std::string> farsiteAPI::findUniqueDates()
{
    std::vector<std::string> outputFiles;

    if(wrfYears.size() >= 1)
    {
        std::string foundUniqueDate = wrfMonths[0] + " " + wrfDays[0];
        outputFiles.push_back(foundUniqueDate);
    }
    if(wrfYears.size() > 1)
    {
        for(size_t wrfFileIdx = 1; wrfFileIdx < wrfYears.size(); wrfFileIdx++)
        {
            if(wrfYears[wrfFileIdx-1] != wrfYears[wrfFileIdx] && wrfMonths[wrfFileIdx-1] != wrfMonths[wrfFileIdx] && wrfDays[wrfFileIdx-1] != wrfDays[wrfFileIdx])
            {
                std::string foundUniqueDate = wrfMonths[wrfFileIdx] + " " + wrfDays[wrfFileIdx];
                outputFiles.push_back(foundUniqueDate);
            }
        }
    }

    return outputFiles;
}
/*** end create farsite input functions ***/

/*** special execute external script commands ***/
std::string farsiteAPI::exec_cmd(const char* cmd)
{
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
                std::cout << buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}
/*** end special execute external script commands ***/

/*** useful utility functions ***/
bool farsiteAPI::doesFilenameExist(std::string fileName)
{
    // found this here: https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
    // hopefully it works for files as well as directories as this is the same thing used to check a file path. More interesting hopefully this works for .prj files!
    struct stat buffer;
    return (stat (fileName.c_str(), &buffer) == 0);
}

bool farsiteAPI::doesFolderExist(std::string pathName)
{
    bool exists = true;

    struct stat info;
    if( stat( pathName.c_str(), &info ) != 0 )
    {
        //printf( "cannot access %s\n", inputString.c_str() );
        exists = false;
    } else if( info.st_mode & S_IFDIR )  // S_ISDIR() doesn't exist on my windows
    {
        //printf( "%s is a directory\n", inputString.c_str() );
        exists = true;
    } else
    {
        //printf( "%s is no directory\n", inputString.c_str() );
        exists = false;
    }

    return exists;
}

// found this function here: https://stackoverflow.com/questions/5719694/having-a-path-to-file-how-to-check-if-it-is-executable
// so it could use some extra stuff to test the owner and group and if you have Secure Linux then the security settings which stat does not give you
bool farsiteAPI::isExecutable(const char *file)
{
    struct stat  st;

        if (stat(file, &st) < 0)
            return false;
        if ((st.st_mode & S_IEXEC) != 0)
            return true;
        return false;
}

std::string farsiteAPI::findFileBaseNameAndExtension(std::string filePath)
{
    std::string fileName = "";

    for(size_t charIdx = filePath.length()-1; charIdx > 0; charIdx--)
    {
        std::string currentChr = filePath.substr(charIdx,1);
        if(currentChr == "/")
        {
            fileName = filePath.substr(charIdx+1,filePath.length());
            break;
        }
        if(charIdx == 1)    // was no extra path on it
        {
            fileName = filePath;
        }
    }

    return fileName;
}

std::string farsiteAPI::findBaseName(std::string filePath)
{
    // first pull off the path part
    std::string fileBaseName = findFileBaseNameAndExtension(filePath);

    // now pull of the extension
    for(size_t charIdx = fileBaseName.length()-1; charIdx > 0; charIdx--)
    {
        std::string currentChr = fileBaseName.substr(charIdx,1);
        if(currentChr == ".")
        {
            fileBaseName = fileBaseName.substr(0,charIdx);
            break;
        }
    }

    return fileBaseName;
}

std::string farsiteAPI::findBaseNameWithPath(std::string filePath)
{
    // first pull off the path part
    std::string fileBaseName = "";

    // now pull of the extension
    for(size_t charIdx = filePath.length()-1; charIdx > 0; charIdx--)
    {
        std::string currentChr = filePath.substr(charIdx,1);
        if(currentChr == ".")
        {
            fileBaseName = filePath.substr(0,charIdx);
            break;
        }
        if(charIdx == 1)    // was no extension on it
        {
            fileBaseName = filePath;
        }
    }

    return fileBaseName;
}

// found this function from: https://stackoverflow.com/questions/10195343/copy-a-file-in-a-sane-safe-and-efficient-way
bool farsiteAPI::copyFile(std::string inputFilename, std::string outputFilename)
{
    std::ifstream  src(inputFilename.c_str(), std::ios::binary);
    std::ofstream  dst(outputFilename.c_str(),   std::ios::binary);
    dst << src.rdbuf();

    // if it gets here, everything went well
    return true;
}
/*** end useful utility functions ***/
