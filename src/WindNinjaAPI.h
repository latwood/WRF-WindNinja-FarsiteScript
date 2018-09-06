#ifndef WINDNINJAAPI_H
#define WINDNINJAAPI_H


#include <iostream>

#include "inputVariablesHandler.h"

#include "cpl_spawn.h"
#include <glob.h>

class WindNinjaAPI
{

public:

    // constructors
    WindNinjaAPI();

    // reconstructor like functions
    bool load_required_inputs(inputVariablesHandler *inputs);

    // functions
    bool create_WindNinja_cfg_files();
    bool run_WindNinja();
    bool findFinalRunFiles();
    bool findWrfTimes();

    // get value functions
    std::vector<std::string> get_atmFilePaths();
    std::vector<std::string> get_velFilePaths();
    std::vector<std::string> get_angFilePaths();
    std::vector<std::string> get_cldFilePaths();
    std::vector<std::string> get_wrfYears();
    std::vector<std::string> get_wrfMonths();
    std::vector<std::string> get_wrfDays();
    std::vector<std::string> get_wrfHours();
    std::vector<std::string> get_wrfMinutes();
    std::vector<std::string> get_wrfSeconds();
    std::vector<std::string> get_wrfTimeZones();


private:

    // reconstructor like functions
    void reset();

    // useful utility functions
    bool doesFolderExist(std::string pathName);
    bool doesFilenameExist(std::string fileName);
    bool doesNetCDFFileExist(std::string pathName);
    std::vector<std::string> globVector(const std::string& pattern);
    bool doOutputFilesExist(size_t runNumber);

    // data members needed from the inputs
    std::string actual_run_base_name;
    std::string actualCreateInputs_path;
    std::string actualLcpFilePath;
        // application specific variables
    std::string WindNinja_required_output_units;
    bool use_native_timezone;
        // WindNinja and getWeather
    bool extend_wrf_data;
    std::vector<wrfFileValue> wrf_files;
        // WindNinja only required vars
    size_t WindNinja_number_of_threads;
    std::string WindNinja_mesh_choice;
    double WindNinja_mesh_resolution;
    std::string WindNinja_mesh_res_units;
    bool diurnal_winds;
    bool non_neutral_stability;
        // optional WindNinja variables
    // will do these ones later, as they need special care and are not necessary till the end after this script is beautiful
    // okay I think the trick is that at some point in the process, these have to be generated for each and every wrf file even if not specified by the user,
    // makes it easier to keep stuff of correct sizes and such
        // additional_WindNinja_outputs_google
    additionalWindNinjaOutputs_googleStorage stored_additional_WindNinja_Outputs_google;
        // additional_WindNinja_outputs_shapefile
    additionalWindNinjaOutputs_shapefileStorage stored_additional_WindNinja_Outputs_shapefile;
        // additional_WindNinja_outputs_pdf
    additionalWindNinjaOutputs_pdfStorage stored_additional_WindNinja_Outputs_pdf;

    // data members created from inputs that are API specific
    std::string WindNinjaOutputFolderPath;
    std::vector<std::string> WindNinjaRunFolderPaths;
    std::vector<std::string> WindNinjaCfgFileNames;
    // too hard to do a specific expected filename, will just look for the basename and the vel.asc extension
    std::string WindNinja_timezone;
    std::string WindNinja_output_speed_units;
    std::string WindNinja_output_wind_height;
    std::string WindNinja_units_output_wind_height;
    std::string WindNinja_diurnal_winds_string;
    std::string WindNinja_non_neutral_stability_string;
    std::string WindNinja_units_ascii_out_resolution;

    // desired output values
    // don't need to check for prj files with this version of everything
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

};

#endif // WINDNINJAAPI_H
