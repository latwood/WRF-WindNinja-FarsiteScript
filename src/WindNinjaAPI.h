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

    // get value functions
    std::vector<std::string> get_atmFilePaths();
    std::vector<std::string> get_velFilePaths();
    std::vector<std::string> get_angFilePaths();
    std::vector<std::string> get_cldFilePaths();

private:

    // reconstructor like functions
    void reset();

    // useful utility functions
    bool doesFolderExist(std::string pathName);
    bool doesNetCDFFileExist(std::string pathName);
    std::vector<std::string> globVector(const std::string& pattern);
    bool doOutputFilesExist(size_t runNumber);

    // data members needed from the inputs
    std::string actualCreateInputs_path;
        // application specific variables
    std::string run_base_name;
    std::string WindNinja_required_output_units;
    bool use_native_timezone;
        // lcp download variables (WindNinja related)
    // going to do this one later after everything is finished. Not sure if use WindNinja api for lcp download or if setup separate lcp class
    bool automate_lcp_download;
    double fireperim_to_lcp_scalefactor;
    bool use_past_lcp;
    std::string lcp_file_path;
    bool specify_lcp_download;
    // others get hairy quickly, was thinking just pull everything, then a string says which of the three methods to use to know which of the pulled data actually matters.
    // but when a datatype is non-standard like lat long point, makes it confusing how to pull it out and use it.
        // WindNinja and getWeather
    bool extend_wrf_data;
    std::vector<std::string> wrf_files;
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
    std::vector<bool> write_wx_model_goog_output;
    std::vector<bool> write_goog_output;
    std::vector<double> goog_out_resolution;
    std::vector<std::string> units_goog_out_resolution;
    std::vector<std::string> goog_out_color_scheme;
    std::vector<bool> goog_out_vector_scaling;
        // additional_WindNinja_outputs_shapefile
    std::vector<bool> write_wx_model_shapefile_output;
    std::vector<bool> write_shapefile_output;
    std::vector<double> shape_out_resolution;
    std::vector<std::string> units_shape_out_resolution;
        // additional_WindNinja_outputs_pdf
    std::vector<bool> write_pdf_output;
    std::vector<double> pdf_out_resolution;
    std::vector<std::string> units_pdf_out_resolution;
    std::vector<double> pdf_linewidth;
    std::vector<std::string> pdf_basemap;
    std::vector<double> pdf_height;
    std::vector<double> pdf_width;
    std::vector<std::string> pdf_size;

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

};

#endif // WINDNINJAAPI_H
