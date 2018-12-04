#ifndef FARSITEAPI_H
#define FARSITEAPI_H


#include "inputClasses/inputVariablesHandler.h"
#include "createIgnitions.h"
#include "WindNinjaAPI.h"
#include "wrfGetWeather.h"


class farsiteAPI
{

public:

    // constructors
    farsiteAPI();

    // reconstructor like functions
    bool load_required_inputs(inputVariablesHandler *inputs, createIgnitions *ignitions, WindNinjaAPI *windAPI, wrfGetWeather *weatherAPI, std::string farsitePath);

    // functions
    bool createAllFarsiteInputs();
    bool runFarsite();
    bool createAdditionalFarsiteResults();

private:

//#include <iostream>
//#include <stdexcept>
//#include <stdio.h>
//#include <string>
    // I found this code online to replace my system calls
    // I made some changes so that it outputs, and while I think it is safer than system calls,
    // it might still not be the right way to do this in a safe and efficient manner
    // but it does seem to work for now
    // see this article for why system commands are bad: http://www.cplusplus.com/forum/articles/11153/
    // I found this function here: https://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c-using-posix
    // reconstructor like functions
    void reset();

    // create farsite input functions
    bool setupFinalAtmFiles(size_t runIdx);
    bool createRawsFile(size_t runIdx);
    bool writeFarsiteInputFile(size_t runIdx);
    bool writeFarsiteCommandFile(size_t runIdx);
    std::vector<std::string> findUniqueDates();

    // special execute external script commands
    std::string exec_cmd(const char* cmd);

    // useful utility functions
    bool doesFilenameExist(std::string fileName);
    bool doesFolderExist(std::string pathName);
    bool isExecutable(const char *file);
    std::string findFileBaseNameAndExtension(std::string filePath);
    std::string findBaseName(std::string filePath);
    std::string findBaseNameWithPath(std::string filePath);
    bool copyFile(std::string inputFilename, std::string outputFilename);
    std::string monthTextToNumber(std::string inputMonthText);
    std::string monthDayHourMinSecAddZero(int time);


    // data members needed from the inputs class
    std::string actual_run_base_name;
    std::string actualCreateInputs_path;
    std::string actualFinalOutput_path;
    std::string actualLcpPath;
        // application specific variables
    std::string createIgnition_output_units; // maybe need this one
    std::string WindNinja_required_output_units; // maybe need this one
    std::string wrfGetWeather_output_units; // maybe need this one
    std::string farsite_output_units;
    bool use_native_timezone;
        // WindNinja only variables
    size_t WindNinja_number_of_threads;
        // farsiteAPI variables
    int burn_start_month;
    int burn_start_day;
    int burn_start_year;
    int burn_start_hour;
    int burn_start_minute;
    int burn_end_month;
    int burn_end_day;
    int burn_end_year;
    int burn_end_hour;
    int burn_end_minute;
    std::string farsite_barrier_shapefile;
    double farsite_spot_probability;
    size_t farsite_spot_ignition_delay;
    size_t farsite_spotting_seed;
    int farsite_earliest_burn_time_hour;
    int farsite_earliest_burn_time_minute;
    int farsite_latest_burn_time_hour;
    int farsite_latest_burn_time_minute;
    double farsite_foliar_moisture_content;
    std::string farsite_crown_fire_method;

    // data members needed from createIgnitions class
    std::vector<std::string> ignitionShapefilesForSimulations;

    // data members needed from WindNinjaAPI class
    std::vector<std::string> atmFiles;
    std::vector<std::string> velFiles;
    std::vector<std::string> angFiles;
    std::vector<std::string> cldFiles;
    std::vector<std::string> wrfYears;
    std::vector<std::string> wrfMonths;
    std::vector<std::string> wrfDays;
    std::vector<std::string> wrfHours;
    std::vector<std::string> wrfMinutes;
    std::vector<std::string> wrfSeconds;
    std::vector<std::string> wrfTimeZones;

    // data members needed from wrfGetWeather class
    std::vector<double> temperatures;
    std::vector<double> humidities;
    std::vector<double> totalPrecip;
    std::vector<double> cloudCover;

    // data members created from inputs that are farsiteAPI specific.
    // Many of these will be preset by this script and not by inputs
    std::string farsite_start_time;
    std::string farsite_end_time;
    std::string farsite_timestep;
    std::string farsite_distance_res;
    std::string farsite_perimeter_res;
    std::string farsite_min_ignition_vertex_distance;
    std::string farsite_spot_grid_resolution;
    //std::string farsite_spot_probability;
    //std::string farsite_spot_ignition_delay;
    std::string farsite_minimum_spot_distance;
    std::string farsite_acceleration_on;
    std::string farsite_fill_barriers;
    //std::string spotting_seed;
    std::string farsite_default_fuel_mositures_data;
    //std::string foliar_moisture_content;
    //std::string crown_fire_method;
    //std::string number_of_processors;

    // extra farsiteAPI variables
    std::string farsiteMainCreateInputsFolderPath;
    std::vector<std::string> farsiteCreateInputRunFolderPaths;
    std::vector<std::string> farsiteFinalOutputRunFolderPaths;
    std::vector<std::string> farsiteCommandFiles;
    std::vector<std::string> farsiteLcpFiles;    // is the same file, just different paths
    std::vector<std::string> farsiteInputFiles;
    std::vector<std::string> farsiteIgnitionFiles;
    std::vector<std::string> farsiteBarrierFiles;   // is the same file, just different paths
    std::vector<std::string> farsiteRawsFiles;  // is the same file, just different paths
    std::vector<std::string> farsiteAtmFiles;
    std::vector<std::vector<std::string> > farsiteVelFiles; // is the same set of files, just different paths
    std::vector<std::vector<std::string> > farsiteAngFiles; // is the same set of files, just different paths
    std::vector<std::vector<std::string> > farsiteCldFiles; // is the same set of files, just different paths
    std::string farsiteApplicationPath; // this should probably be an input variable, with a default value being the nice farsite location

    // for farsite burn period stuff
    std::vector<std::string> uniqueDates;

};

#endif // FARSITEAPI_H
