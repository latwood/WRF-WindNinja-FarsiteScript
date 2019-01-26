#include "inputVariables_infoStorage.h"

/***** public functions *****/

/*** constructor functions ***/
inputVariables_infoStorage::inputVariables_infoStorage()
{
    bool success = true;
    // for description printing
    descriptionVariableNameColumnSize = 0;

    // first setup all the needed stuff for the variables
    setupAvailableApplicationUseNames();
    setupAvailableVariableCountTypes();
    setupAvailableVariables();

    // now run checks on the setup of the variables
    if(check_setupForDuplicateVariableNames() == false)
    {
        printf("duplicate option found during setup!\n");
        success = false;
    }
    if(check_setupForValidApplicationUseNames() == false)
    {
        printf("non-allowable application use name found during setup!\n");
        success = false;
    }
    if(check_setupForValidOrderingByApplicationUseNames() == false)
    {
        printf("invalid ordering of variables by application use name found during setup!\n");
        success = false;
    }
    if(check_setupForValidVariableCountTypes() == false)
    {
        printf("non-allowable count type found during setup!\n");
        success = false;
    }
    if(check_setupDescription() == false)
    {
        printf("invalid description found during setup!\n");
        success = false;
    }

    if(success == false)
    {
        printf("exiting program\n");
        exit(1);
    }

    // now do description whitespace and line break calculations, which include setup error checks (but won't exit if fail, just warn programmer to mess with setup)
    if(calculateDescriptionWhiteSpace() == false)
    {
        printf("problem with description white space calculation. Program will continue, but means a programmer needs to improve at least one variable name size\n");
    }
    if(calculateDescriptionLineBreaks() == false)
    {
        printf("problem with description line break calculation. Program will continue, but means a programmer needs to play with description formatting stuff\n");
    }
    calculate_maxVarNameColumnWhitespace();

}
/*** end constructor functions ***/


/*** get value functions ***/
std::vector<inputVariable_info> inputVariables_infoStorage::get_inputVariableInfo()
{
    return inputVariables;
}

std::string inputVariables_infoStorage::get_maxVarNameColumnWhitespace()
{
    return maxVarNameColumnWhitespace;
}
/*** end get value functions ***/



/***** private functions *****/



/*** setup functions ***/
void inputVariables_infoStorage::setupAvailableApplicationUseNames()
{
    // the idea is that each variable is used for different things, and when explaining uses, it is handy to show the applications they are for
    // other functions will verify that the setupAvailableVariables() function only used these types and that they were specified in order of application type, so in this order :)
    allowedApplicationUseNames.push_back("application specific");
    allowedApplicationUseNames.push_back("lcpDownloader");
    allowedApplicationUseNames.push_back("createIgnition");
    allowedApplicationUseNames.push_back("wrfGetWeather and WindNinja");
    allowedApplicationUseNames.push_back("WindNinja only");
    allowedApplicationUseNames.push_back("wrfGetWeather only");
    allowedApplicationUseNames.push_back("farsiteAPI");
    allowedApplicationUseNames.push_back("debugging related");
    allowedApplicationUseNames.push_back("optional WindNinja output settings");
}

void inputVariables_infoStorage::setupAvailableVariableCountTypes()
{
    // this is what will be used in the end, don't need to redefine the count types twice
    allowedVariableCountTypes.push_back("bool");
    allowedVariableCountTypes.push_back("size_t");
    allowedVariableCountTypes.push_back("int");
    allowedVariableCountTypes.push_back("double");
    allowedVariableCountTypes.push_back("positive double");
    allowedVariableCountTypes.push_back("signless percent");
    allowedVariableCountTypes.push_back("string");
    allowedVariableCountTypes.push_back("pathname");
    allowedVariableCountTypes.push_back("lcp filename");
    allowedVariableCountTypes.push_back("shape filename");
    allowedVariableCountTypes.push_back("lat_coord");
    allowedVariableCountTypes.push_back("long_coord");
    allowedVariableCountTypes.push_back("lat_long_point");
    allowedVariableCountTypes.push_back("date");
    allowedVariableCountTypes.push_back("hour_min");
    allowedVariableCountTypes.push_back("count");
}

void inputVariables_infoStorage::addVariable(std::string newVariableName,std::string newApplicationUseName,std::string newVariableCountType,std::string newVariableDescription)
{
    inputVariables.push_back(inputVariable_info(newVariableName,newApplicationUseName,newVariableCountType,newVariableDescription));
}

void inputVariables_infoStorage::setupAvailableVariables()
{
    // add desired variables to be read from the config file. Set which will be optional or not through the verification in the checkSetVarNamesForConflictingOptions function in the inputVariablesHandler class.
    // whether all these variables are actually used by the program even though they are required; at least the names and counts are read by the input file,
    // is up to the programmer linking the input variables into the program for whether they will be used. But the load function name means that a warning will be given
    // if a variable is required as input but has no loading method. Still doesn't necessaily mean that it is used, but hopefully this makes deprecation a little less annoying.

    // one big problem with this is that it is tricky to keep it pretty when trying to add in all the stuff. Technically it isn't too bad yet, but especially the description!
    // Fortunately there are some parsing tricks applied using whitespace to hopefully clean up the format of the description so you only need to do a single string for description.
    // If that fails and warnings come a lot, might have to reprogram stuff and manually setup the format of the input variables. If so, wait till all variable names and descriptions
    // are set or you'll be redoing changes a lot. Might be handy to eventually include a bool whether to use this smart whitespace formatting or not.
    // Would still need whitespace, just not line breaks.

    // an interesting thing is that the strings need additional and very specific verification, kind of like the loader functions.
    // So if adding a string value usually turn it into a separate type
    // but if that is not possible, just do the verification a bit later, editing inputParser::isValidString()

    // another fun part is that setting default values for anything non-generic immediately becomes a specialized need.
    // specify any specialized need default starting values in inputVariables_valueStorage::setSpecializedDefaults().

        // should have a section for each type of input variables, and include the type or use in the description
        // if you need more sections, define section in setupAvailableApplicationUseNames(). Define more types in setupAvailableVariableCountTypes().
        // just know that additional types will also need an additional load method and other input storage functions in other input classes
        // the variable names will be greatly dependent on the variable names used throughout the program as well as the loader/parser and variable storage classes
        // variables may be loaded or no, but to verify if optional or no, need to change verification in the checkSetVarNamesForConflictingOptions function in the inputVariablesHandler class.

        // application specific variables
    addVariable("run_base_name","application specific","string",
                "The base name for all created files, so example run_base_name is cougarCreek, all files will be cougarCreek.lcp cougarCreek_ignit.shp so stuff like this. "
                "If use_past_lcp is set, this value will be set to the found base name of the lcp file specified in lcp_file_path");
    addVariable("createInputs_path","application specific","pathname",
                "The path where files created by createIgnitions, WindNinja, wrfGetWeather, and createFarsiteInputs go. "
                "createInputs folder Will contain separate folders for each of these outputs");
    addVariable("finalOutput_path","application specific","pathname",
                "The path where farsite run outputs will be generated");
    addVariable("overwrite_past_outputs","application specific","bool",
                "A boolean specifying whether the output files found in createdInputs and finalOutputs folders should be overwritten. "
                "If these output folders exist in createInputs_path or finalOutputs_path and overwrite_past_outputs is true, folders aren't deleted, "
                "just if names are the same they will be overwritten. If overwrite_past_outputs is false and these folders exist, "
                "a search for either createInputs-# or finalOutputs-# will determine the highest # value, add one to it, and use that as the new output. "
                "Default value is false");
    addVariable("createIgnition_output_units","application specific","string",
                "A string specifying the types of units to be used by all files outputted by createIgnitions during the script. "
                "Values can be english, metric, or input where input is the default value");
    addVariable("WindNinja_required_output_units","application specific","string",
                "A string specifying the types of units to be used by all files required for later use outputted by WindNinja during the script. "
                "Values can be english or metric where metric is the default value. output_wind_height is 20 ft for english and 10 m for metric");
    addVariable("wrfGetWeather_output_units","application specific","string",
                "A string specifying the types of units to be used by all files outputted by wrfGetWeather during the script. "
                "Values can be english, or metric where metric is the default value");
    addVariable("farsite_output_units","application specific","string",
                "A string specifying the types of units to be used by all files output by farsite at the very end. "
                "I suspect this isn't actually allowable yet, so it might just be whatever units are used coming in, or default to english, but we shall see. "
                "Values can be english or metric");
    addVariable("use_native_timezone","application specific","bool",
                "A boolean specifying whether the timezone should be native to the area or GMT for all created inputs and final output files. "
                "Default is false so that the GMT timezone is used by default");

        // lcp download variables (WindNinja related)
    addVariable("automate_lcp_download","lcpDownloader","bool",
                "A boolean to know whether the lcp should follow automated features for downloading. "
                "fireperim_to_lcp_scalefactor is used to increase the lcp bounding box size by a multiple of the largest ignition fire perimeter. "
                "Only one of automate_lcp_download, use_past_lcp, and specify_lcp_download can be true and the default to be true if none are specified is automate_lcp_download");
    addVariable("fireperim_to_lcp_scalefactor","lcpDownloader","positive double",
                "A positive integer y used as a scaling factor for the lcp bounding box to make it x*y bigger than the largest ignition fire perimeter, "
                "where x is 200 for ignitions that are basically a single point and 200 for all other ignitions. Default value for y is 1.0 if not specified "
                "and should only be specified if automate_lcp_download is set to true");
    addVariable("use_past_lcp","lcpDownloader","bool",
                "A boolean to know whether lcp downloading should be avoided by using an already predownloaded lcp file. "
                "If this is true, need to also specify lcp_file_path. Only one of automate_lcp_download, use_past_lcp, "
                "and specify_lcp_download can be true and the default to be true if none are specified is automate_lcp_download");
    addVariable("lcp_file_path","lcpDownloader","lcp filename",
                "A path to a predownloaded lcp file to use instead of downloading a new one. Use if use_past_lcp set to true. "
                "File name extension is verified to be .lcp and file is verified to be openable by gdal. Second file with same path and name "
                "but .prj extension is also verified to exist. Specify only if use_past_lcp is set to true");
    addVariable("specify_lcp_download","lcpDownloader","bool",
                "For doing a WindNinja style lat/long point and buffer zone or lat/long bounding box lcp download. "
                "If this is true, need to specify either use_point_lcp_download or use_bounds_lcp_download. "
                "If use_point_lcp_download is specified, all the following need specified: lcp_download_lat_long_point, "
                "lcp_download_northsouth_buffer, lcp_download_westeast_buffer, lcp_download_buffer_units. "
                "If use_bounds_lcp_download is specified, all the following need specified: lcp_download_north_lat_bound, "
                "lcp_download_south_lat_bound, lcp_download_east_long_bound, lcp_download_west_long_bound. "
                "Only one of automate_lcp_download, use_past_lcp, and specify_lcp_download can be true and the default "
                "to be true if none are specified is automate_lcp_download");
    addVariable("use_point_lcp_download","lcpDownloader","bool",
                "A boolean specifying which WindNinja download type to use. Specify only if specify_lcp_download is set to true, "
                "with conditions as explained for that variable");
    addVariable("lcp_download_lat_long_point","lcpDownloader","lat_long_point",
                "A lat long point of format (lat long) and is the center point of a box used for lcp downloading in WindNinja. "
                "Specify only if specify_lcp_download and use_point_lcp_download are set to true");
    addVariable("lcp_download_northsouth_buffer","lcpDownloader","positive double",
                "Half the height of the box with lcp_download_lat_long_point as the center of the box. "
                "Specify only if specify_lcp_download and use_point_lcp_download are set to true");
    addVariable("lcp_download_westeast_buffer","lcpDownloader","positive double",
                "Half the width of the box with lcp_download_lat_long_point as the center of the box. "
                "Specify only if specify_lcp_download and use_point_lcp_download are set to true");
    addVariable("lcp_download_buffer_units","lcpDownloader","string",
                "The units of the buffer box width and height. Can be kilometers or miles. "
                "Specify only if specify_lcp_download and use_point_lcp_download are set to true");
    addVariable("use_bounds_lcp_download","lcpDownloader","bool",
                "A boolean specifying which WindNinja download type to use. Specify only if specify_lcp_download is set to true, "
                "with conditions as explained for that variable");
    addVariable("lcp_download_north_lat_bound","lcpDownloader","lat_coord",
                "The north latitude coordinate for an lcp download of a box with no center. Is a decimal point value. "
                "Specify only if specify_lcp_download and use_bounds_lcp_download are set to true");
    addVariable("lcp_download_south_lat_bound","lcpDownloader","lat_coord",
                "The south latitude coordinate for an lcp download of a box with no center. Is a decimal point value. "
                "Specify only if specify_lcp_download and use_bounds_lcp_download are set to true. "
                "Checks to make sure is less than lcp_download_north_lat_bound");
    addVariable("lcp_download_east_long_bound","lcpDownloader","long_coord",
                "The east latitude coordinate for an lcp download of a box with no center. Is a decimal point value. "
                "Specify only if specify_lcp_download and use_bounds_lcp_download are set to true. "
                "Checks to make sure is less than lcp_download_west_long_bound");
    addVariable("lcp_download_west_long_bound","lcpDownloader","long_coord",
                "The west latitude coordinate for an lcp download of a box with no center. Is a decimal point value. "
                "Specify only if specify_lcp_download and use_bounds_lcp_download are set to true");

        // createIgnition variables
    addVariable("create_ignition_from_latlongs","createIgnition","count",
                "Consists of a list of lat long locations from which a single ignition file will be created of format (lat long). "
                "Need at least one of create_ignition_from_latlongs, polygon_ignit_shape_files, GeoMAC_fire_perimeter_files, "
                "or farsite_output_fire_perimeter_files ignition types to run");
    addVariable("polygon_ignit_shape_files","createIgnition","count",
                "Consists of a list of precreated ignition shape files (so can be past ones created of any other type). "
                "Format of ignition shape files are still checked. Checked to make sure file extension is .shp and that file can be read by ogr/gdal. "
                "Need at least one of create_ignition_from_latlongs, polygon_ignit_shape_files, GeoMAC_fire_perimeter_files, "
                "or farsite_output_fire_perimeter_files ignition types to run");
    addVariable("GeoMAC_fire_perimeter_files","createIgnition","count",
                "Consists of a list of GeoMAC fire perimeters to be used as input ignitions. "
                "New versions of these files are created that are in the necessary projection coordinates as the original files "
                "will not work in Farsite without preprocessing. Files are checked to make sure file extension is .shp and that files can be read by ogr/gdal. "
                "Need at least one of create_ignition_from_latlongs, polygon_ignit_shape_files, GeoMAC_fire_perimeter_files, "
                "or farsite_output_fire_perimeter_files ignition types to run");
    addVariable("farsite_output_fire_perimeter_files","createIgnition","count",
                "Consists of a list of past farsite run output fire perimeter files to use as ignitions for a new farsite run. "
                "These cannot be read by the current version of farsite, so new versions of these files are created by grabbing all fires that occur "
                "at the last output fire perimeter date/time. The new date/time for these fire perimeters is assumed to be the burn_start_time value. "
                "Files are checked to make sure file extension is .shp and that files can be read by ogr/gdal. "
                "Need at least one of create_ignition_from_latlongs, polygon_ignit_shape_files, GeoMAC_fire_perimeter_files, "
                "or farsite_output_fire_perimeter_files ignition types to run");
    addVariable("fire_perimeter_widening_factor","createIgnition","positive double",
                "An integer used to shrink or expand fire perimeters of all created ignition files by multiplying each point in the geometry by this integer. "
                "Defaults to a value of 1.0 unless specified. May need to make this specific to each ignition file if needed long term. "
                "Or can get rid of and if any perimeter is less than say 30 m, just stretch it to that size as that is the default farsite distance res");

        // wrfGetWeather and WindNinja variables
    addVariable("extend_wrf_data","wrfGetWeather and WindNinja","bool",
                "A boolean telling the application whether it should duplicate the wrf data acquired from the earliest and latest wrf files to extend wind "
                "and weather data to the required start and end times for the simulation. But only to a maximum of 24 hours each way. Default value is false"
                "Hm, should this be a days worth of data instead of a single data to extend? Need more thought to do something like that");
    addVariable("wrf_files","wrfGetWeather and WindNinja","count",
                "Consists of a list of paths to each of the WRF files used as wind and weather start points for the application. "
                "Internal WRF file times will be checked to make sure they are sorted by date and hour, where they begin 24 hrs before burn_start_time and end at burn_end_time. "
                "So if burn_start_time is 08 11 2015, 12:00 and burn_end_time is 08 13 2015, 12:00 then the first WRF file should be for 08 10 2015, 12:00 "
                "and the last WRF file should be for 08 13 2015, 12:00. This comes out to be 72 WRF files. Files are assumed to be of timezone UTZ so GMT. "
                "If extend_wrf_date is set to true, then the wrf data acquired from the earliest wrf file will be duplicated to fill to the needed start time "
                "and the same goes for the latest wrf file data duplicated to fill to the needed end time");

        // WindNinja only variables
    addVariable("WindNinja_number_of_threads","WindNinja only","size_t",
                "The number of threads to use for WindNinja simulations. If you specify a greater number than exist, will use max. Default is 8");
    addVariable("WindNinja_mesh_choice","WindNinja only","string",
                "A string specifying the type of WindNinja mesh desired for all simulations. Choices are coarse, medium, fine, or custom. "
                "Default is fine if WindNinja_mesh_choice is not specified");
    addVariable("WindNinja_mesh_resolution","WindNinja only","positive double",
                "A positive double value specifying the exact desired mesh resolution. Only specify this is WindNinja_mesh_choice has a value of custom. "
                "Warning! If this is a lot smaller than what is selected by WindNinja_mesh_choice of fine, could make simulations take a TON longer");
    addVariable("WindNinja_mesh_res_units","WindNinja only","string",
                "A string specifying the units of WindNinja_mesh_resolution and has to be either ft or m. Default is m if WindNinja_mesh_resolution is specified but this is not");
    addVariable("diurnal_winds","WindNinja only","bool",
                "A boolean used to know whether WindNinja should be run with the diurnal wind thermal parameter. Default value is false");
    addVariable("non_neutral_stability","WindNinja only","bool",
                "A boolean used to know whether WindNinja should be run with the stability thermal parameter. Default value is false");

        // wrfGetWeather only variables
    addVariable("use_weather_from_ignition_center","wrfGetWeather only","bool",
                "A boolean to know whether weather data should be pulled from the WRF files by using the average of all ignition centers");
    addVariable("use_weather_from_full_ignition_area","wrfGetWeather only","bool",
                "A boolean to know whether weather data should be pulled from the WRF files by using the average of all points in the ignition perimeters");
    addVariable("use_weather_from_wrf_center","wrfGetWeather only","bool",
                "A boolean to know whether weather data should be pulled from the WRF files by using the center of the WRF file and is the initially preferred method of the three options");

        // farsiteAPI variables
    addVariable("burn_start_time","farsiteAPI","date",
                "Needs to be a single date of format (month day year hour:minute). hour:minute goes from 00:00 to 23:59");
    addVariable("burn_end_time","farsiteAPI","date",
                "Needs to be a single date of format (month day year hour:minute). hour:minute goes from 00:00 to 23:59. "
                "Will be checked to make sure it is after burn_start_time");
    addVariable("farsite_barrier_shapefile","farsiteAPI","shape filename",
                "A path to a barrier file to use with each farsite run. Is an optional input. Checks to make sure file has .shp extension and is openable by ogr/gdal");
    // # farsite_distance_res and farsite_perimeter_res will be 60 meters and 30 meters for now with farsite_min_ignition_vertex_distance and farsite_spot_grid_resolution
    //      as half the farsite_perimeter_res with farsite_minimum_spot_distance the same value as farsite_perimeter_res
    // # farsite_time_step will be 60 minutes for now, and is dependent on the wrf files
    addVariable("farsite_spot_probability","farsiteAPI","signless percent",
                "A percentage given with the % sign missing. The default value is 0.05 so 0.05%");
    addVariable("farsite_spot_ignition_delay","farsiteAPI","size_t",
                "A time in minutes to give a short time delay to when spotting calculations occur. Default value is 0");
    addVariable("farsite_spotting_seed","farsiteAPI","size_t",
                "A tally number for the assumed starting possible number of spotting embers to be generated at a given time. The default value is 1000");
    addVariable("farsite_earliest_burn_time","farsiteAPI","hour_min",
                "A time used for setting the farsite_burn_periods, so the earliest time during a day that fires can burn. "
                "Format is (hour:minute) which can range from 00:00 to 23:59. The default value is 08:00");
    addVariable("farsite_latest_burn_time","farsiteAPI","hour_min",
                "A time used for setting the farsite_burn_periods, so the latest time during a day that fires can burn. "
                "This is usually set with the assumption that fires don't burn well at night. "
                "Format is (hour:minute) which can range from 00:00 to 23:59. The default value is 19:59");
    addVariable("farsite_foliar_moisture_content","farsiteAPI","signless percent",
                "A percentage given with the % sign missing. The default value is 70 so 70%. Can play a large role in crown fire spread");
    addVariable("farsite_crown_fire_method","farsiteAPI","string",
                "A string specifying the crown fire method used in farsite. "
                "Options are Finney or Reinhart where Finney is the default if farsite_crown_fire_method is not specified");

        // debugging related variables
    addVariable("report_largest_ignition_bounds","debugging related","bool",
                "A boolean that specifies whether to output the lat_long bounding box that contains the smallest and largest lat long coordinates that have ignitions in them. "
                "May not be used as an optional debugging statement, probably just output it as looking through the different ignition locations. "
                "Fun, have to do ignitions before and after the lcp download lol");
    addVariable("report_found_inputs","debugging related","bool",
                "A boolean that specifies whether to output what the input file is seen to be by the code. Defaults to false if not specified");

        // optional WindNinja output settings, may not even add these options
    addVariable("additional_WindNinja_outputs_google","optional WindNinja output settings","count",
                "A list specifying all runs that want additional google output files written by using wrf_files. "
                "Format is (wrf_file_name write_wx_model_goog_output write_goog_output goog_out_resolution units_goog_out_resolution goog_out_color_scheme goog_out_vector_scaling) "
                "where you can see what each of these mean looking at WindNinja help. "
                "The default for any wrf_file_names not specified is everything needed to disallow these outputs, "
                "so can piecemeal choose to have these additional outputs for separate WindNinja runs. "
                "Checked to make sure each specified wrf_file exists in list of wrf_files. "
                "Warning! If even a few of these are to be outputted, will make simulations take a TON longer, especially the wx_file_outputs");
    addVariable("additional_WindNinja_outputs_shapefile","optional WindNinja output settings","count",
                "A list specifying all runs that want additional shapefile output files written by using wrf_files. "
                "Format is (wrf_file_name write_wx_model_shapefile_output write_shapefile_output shape_out_resolution units_shape_out_resolution) "
                "where you can see what each of these mean looking at WindNinja help. "
                "The default for any wrf_file_names not specified is everything needed to disallow these outputs, "
                "so can piecemeal choose to have these additional outputs for separate WindNinja runs. "
                "Checked to make sure each specified wrf_file exists in list of wrf_files. "
                "Warning! If even a few of these are to be outputted, will make simulations take a TON longer, especially the wx_file_outputs");
    // ## all the ascii fire behavior files will be output as these are required output
    addVariable("additional_WindNinja_outputs_pdf","optional WindNinja output settings","count",
                "A list specifying all runs that want additional shapefile output files written by using wrf_files. "
                "Format is (wrf_file_name write_pdf_output pdf_out_resolution units_pdf_out_resolution pdf_linewidth pdf_basemap pdf_height pdf_width pdf_size) "
                "where you can see what each of these mean looking at WindNinja help. "
                "The default for any wrf_file_names not specified is everything needed to disallow these outputs, "
                "so can piecemeal choose to have these additional outputs for separate WindNinja runs. "
                "Checked to make sure each specified wrf_file exists in list of wrf_files. "
                "Warning! If even a few of these are to be outputted, will make simulations take a TON longer, especially the wx_file_outputs");


    // notice that these do NOT include all the possible variables for windninja and farsite, just the ones that will be changing a bunch.
    // So if you need to move needed variables that are just set to be the same thing every time for farsite or windninja, look in
    // wrfInterpretation and createFarsiteInput at what is set to be the same everytime, and create a variable replacement here,
    // but then you need to adjust how that variable is used in wrfInterpretation and createFarsiteInputas well as make changes to the input loader/parser and storage classes :)

}
/*** end setup functions ***/

/*** check setup functions ***/
bool inputVariables_infoStorage::check_setupForDuplicateVariableNames()
{
    bool success = true;
    for(size_t firstVarIdx = 0; firstVarIdx < inputVariables.size()-1; firstVarIdx++)
    {
        for(size_t secondVarIdx = firstVarIdx+1; secondVarIdx < inputVariables.size(); secondVarIdx++)
        {
            if(inputVariables[firstVarIdx].get_variableName() == inputVariables[secondVarIdx].get_variableName())
            {
                printf("found duplicate variable \"%s\"!\n",inputVariables[firstVarIdx].get_variableName().c_str());
                success = false;
            }
        }
    }
    return success;
}

bool inputVariables_infoStorage::check_setupForValidApplicationUseNames()
{
    bool success = true;
    for(size_t varIdx = 0; varIdx < inputVariables.size(); varIdx++)
    {
        bool isValidUseName = false;
        for(size_t useNameIdx = 0; useNameIdx < allowedApplicationUseNames.size(); useNameIdx++)
        {
            if(inputVariables[varIdx].get_applicationUseName() == allowedApplicationUseNames[useNameIdx])
            {
                isValidUseName = true;
                break;
            }
        }
        if(isValidUseName == false)
        {
            printf("application use name \"%s\" for variable \"%s\" is not a valid application use name!\n",inputVariables[varIdx].get_applicationUseName().c_str(),inputVariables[varIdx].get_variableName().c_str());
            success = false;
        }
    }
    return success;
}

bool inputVariables_infoStorage::check_setupForValidOrderingByApplicationUseNames()
{
    bool success = true;

    // make sure the variable order matches the application use names
    unsigned int applicationUseNameTypeCount = 0;
    bool foundValidApplicationUseName = false;
    for(size_t varIdx = 0; varIdx < inputVariables.size(); varIdx++)
    {
        if(applicationUseNameTypeCount >= allowedApplicationUseNames.size())
        {
            printf("variables are not ordered by applicationUseName! Found at variable \"%s\"!\n",inputVariables[varIdx].get_variableName().c_str());
            success = false;
            break;
        }
        if(inputVariables[varIdx].get_applicationUseName() == allowedApplicationUseNames[applicationUseNameTypeCount])
        {
            foundValidApplicationUseName = true;
        } else
        {
            if(foundValidApplicationUseName == true)
            {
                applicationUseNameTypeCount = applicationUseNameTypeCount + 1;
            } else
            {
                printf("variables are not ordered by applicationUseName! Found at variable \"%s\"!\n",inputVariables[varIdx].get_variableName().c_str());
                success = false;
                break;
            }
        }
    }

    // if failed, print the order that is needed
    if(success == false)
    {
        printf("applicationUseNames are of the following order:\n");
        for(size_t appUseNameIdx; appUseNameIdx < allowedApplicationUseNames.size(); appUseNameIdx++)
        {
            printf("%s\n",allowedApplicationUseNames[appUseNameIdx].c_str());
        }
        printf("make sure variables are setup so that they are organized in order of these applicationUseNames!\n");
    }

    return success;
}

bool inputVariables_infoStorage::check_setupForValidVariableCountTypes()
{
    bool success = true;
    for(size_t varIdx = 0; varIdx < inputVariables.size(); varIdx++)
    {
        bool isValidType = false;
        for(size_t countTypeIdx = 0; countTypeIdx < allowedVariableCountTypes.size(); countTypeIdx++)
        {
            if(inputVariables[varIdx].get_variableCountType() == allowedVariableCountTypes[countTypeIdx])
            {
                isValidType = true;
                break;
            }
        }
        if(isValidType == false)
        {
            printf("variable count type \"%s\" for variable \"%s\" is not a valid type!\n",inputVariables[varIdx].get_variableCountType().c_str(),inputVariables[varIdx].get_variableName().c_str());
            success = false;
        }
    }
    return success;
}

bool inputVariables_infoStorage::check_setupDescription()
{
    bool success = true;

    // first add up the number of starting characters that are whitespace. If there are any, it is an error so warn
    // and say how many extra characters and there should be no whitespace at the start of a description
    for(size_t varIdx = 0; varIdx < inputVariables.size(); varIdx++)
    {
        unsigned int whiteSpaceCount = 0;
        std::string currentDescription = inputVariables[varIdx].get_variableDescription();
        for(size_t descriptionVarIdx = 0; descriptionVarIdx < currentDescription.length(); descriptionVarIdx++)
        {
            std::string currentChr = currentDescription.substr(descriptionVarIdx,1);
            if(currentChr == " ")
            {
                whiteSpaceCount = whiteSpaceCount + 1;
            } else
            {
                break;
            }
        }
        if(whiteSpaceCount > 0)
        {
            printf("variable \"%s\" description starts with %d white space characters. variable descriptions should not start with whitespace!\n",inputVariables[varIdx].get_variableName().c_str(),whiteSpaceCount);
            success = false;
        }
    }

    return success;
}
/*** end check setup functions ***/

/*** description whitespace and line break calculations, with error checking ***/
bool inputVariables_infoStorage::calculateDescriptionWhiteSpace()
{
    bool success = true;

    // first calculate the biggest string for the variable names
    unsigned int biggestString = 0;
    for(size_t varIdx = 0; varIdx < inputVariables.size(); varIdx++)
    {
        if(inputVariables[varIdx].get_variableName().size() > biggestString)
        {
            biggestString = inputVariables[varIdx].get_variableName().size();
        }
    }
    descriptionVariableNameColumnSize = biggestString + MIN_VARNAME_WHITESPACE;

    // now calculate variable name whitespace for each variable
    unsigned int neededWhiteSpace = 0;
    std::string createdWhiteSpace = "";
    for(size_t varIdx = 0; varIdx < inputVariables.size(); varIdx++)
    {
        neededWhiteSpace = descriptionVariableNameColumnSize - inputVariables[varIdx].get_variableName().size(); // defined in header file
        createdWhiteSpace = "";
        for(size_t m = 0; m < neededWhiteSpace; m++)
        {
            createdWhiteSpace = createdWhiteSpace + " ";
        }
        inputVariables[varIdx].set_variableNameWhiteSpace(createdWhiteSpace);
    }

    return success;
}

bool inputVariables_infoStorage::calculateDescriptionLineBreaks()
{
    bool success = true;

    for(size_t varIdx = 0; varIdx < inputVariables.size(); varIdx++)
    {
        std::vector<unsigned int> wordBreaks;
        // first find all whitespace locations in the description
        // notice there were checks which end the program if the description starts with whitespace, so we can assume all whitespace is after the first character of the description
        std::string currentDescription = inputVariables[varIdx].get_variableDescription();
        bool isWord = true;
        for(size_t descriptionVarIdx = 0; descriptionVarIdx < currentDescription.length(); descriptionVarIdx++)
        {
            std::string currentChr = currentDescription.substr(descriptionVarIdx,1);
            if(currentChr == " ")
            {
                isWord = false;
            }
            if(descriptionVarIdx == currentDescription.length()-1 && isWord == true)
            {
                wordBreaks.push_back(descriptionVarIdx+1);
            }
            if(currentChr != " " && isWord == false)
            {
                wordBreaks.push_back(descriptionVarIdx);
                isWord = true;
            }
        }

        // now find the closest whitespace location before the max column size and add that location to the description line breaks
        inputVariables[varIdx].add_variableDescriptionLineBreaks(0);  // start out with the first break as 0, also avoids breaking problems later
        unsigned int lineCount = 0;
        unsigned int lineWordCount = 0;
        unsigned int descriptionMaxSize = MAX_DESCRIPTION_LINESIZE - descriptionVariableNameColumnSize;
        for(size_t wordIdx = 0; wordIdx < wordBreaks.size(); wordIdx++)
        {
            unsigned int currentLineSize = wordBreaks[wordIdx] - inputVariables[varIdx].get_variableDescriptionLineBreaks()[lineCount];
            lineWordCount = lineWordCount + 1;
            if(currentLineSize >= descriptionMaxSize)
            {
                if(currentLineSize > MAX_DESCRIPTION_LINESIZE)
                {
                    printf("found description word for variable \"%s\" bigger than MAX_DESCRIPTION_LINESIZE of %d! Need to have programmer revise description!\n This problem will kill program, so exiting program!\n",inputVariables[varIdx].get_variableDescription().c_str(),MAX_DESCRIPTION_LINESIZE);
                }
                inputVariables[varIdx].add_variableDescriptionLineBreaks(wordBreaks[wordIdx-1]);
                if(lineWordCount < MIN_WORDS_PER_DESCRIPTION_LINE)
                {
                    printf("added variableDescriptionLineBreak for description line with only \"%d\" words for lineCount \"%d\". MIN_WORDS_PER_DESCRIPTION_LINE is \"%d\"\n",lineWordCount,lineCount,MIN_WORDS_PER_DESCRIPTION_LINE);
                }
                lineCount = lineCount + 1;
                lineWordCount = 0;
            } else if(wordIdx == wordBreaks.size()-1)
            {
                inputVariables[varIdx].add_variableDescriptionLineBreaks(wordBreaks[wordIdx]);
                // don't care if fewer words per line than MIN_WORDS_PER_DESCRIPTION_LINE for the last line
            }
        }
    }

    return success;
}

void inputVariables_infoStorage::calculate_maxVarNameColumnWhitespace()
{
    for(size_t whitespaceCount = 0; whitespaceCount < descriptionVariableNameColumnSize; whitespaceCount++)
    {
        maxVarNameColumnWhitespace = maxVarNameColumnWhitespace + " ";
    }
}
/*** end description whitespace and line break calculations, with error checking ***/
