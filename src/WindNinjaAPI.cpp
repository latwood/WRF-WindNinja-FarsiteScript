#include "WindNinjaAPI.h"

/***** public functions *****/

/*** constructor functions ***/
WindNinjaAPI::WindNinjaAPI()
{

}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
bool WindNinjaAPI::load_required_inputs(inputVariablesHandler *inputs, std::string WindNinjaPath)
{
    // first reset all values to default values with reconstructor
    reset();

    // next load all the needed inputs from the input file
    actual_run_base_name = inputs->get_actual_run_base_name();
    actualCreateInputs_path = inputs->get_actualCreateInputs_path();
    actualLcpFilePath = inputs->get_actualLcpPath();
        // application specific variables
    WindNinja_required_output_units = inputs->get_stringValue("WindNinja_required_output_units").get_storedStringValue();
    use_native_timezone = inputs->get_boolValue("use_native_timezone").get_storedBoolValue();
        // WindNinja and getWeather
    extend_wrf_data = inputs->get_boolValue("extend_wrf_data").get_storedBoolValue();
    wrf_files = inputs->get_wrfFileStorage().get_storedWrfFiles();
        // WindNinja only required vars
    WindNinja_number_of_threads = inputs->get_size_tValue("WindNinja_number_of_threads").get_storedSize_tValue();
    WindNinja_mesh_choice = inputs->get_stringValue("WindNinja_mesh_choice").get_storedStringValue();
    WindNinja_mesh_resolution = inputs->get_doubleValue("WindNinja_mesh_resolution").get_storedDoubleValue();
    WindNinja_mesh_res_units = inputs->get_stringValue("WindNinja_mesh_res_units").get_storedStringValue();
    diurnal_winds = inputs->get_boolValue("diurnal_winds").get_storedBoolValue();
    non_neutral_stability = inputs->get_boolValue("non_neutral_stability").get_storedBoolValue();
        // optional WindNinja variables
    // will do these ones later, as they need special care and are not necessary till the end after this script is beautiful
    // okay I think the trick is that at some point in the process, these have to be generated for each and every wrf file even if not specified by the user,
    // makes it easier to keep stuff of correct sizes and such
        // additional_WindNinja_outputs_google
    stored_additional_WindNinja_Outputs_google = inputs->get_additionalWindNinjaOutputs_googleStorage();
        // additional_WindNinja_outputs_shapefile
    stored_additional_WindNinja_Outputs_shapefile = inputs->get_additionalWindNinjaOutputs_shapefileStorage();
        // additional_WindNinja_outputs_pdf
    stored_additional_WindNinja_Outputs_pdf = inputs->get_additionalWindNinjaOutputs_pdfStorage();

    // now create any WindNinjaAPI specific inputs that are needed from these inputs
    WindNinjaOutputFolderPath = actualCreateInputs_path + "/WindNinja";
    for(size_t wrfCount = 0; wrfCount < wrf_files.size(); wrfCount++)
    {
        std::string wrfBaseName = CPLGetBasename(wrf_files[wrfCount].get_storedWrfFileName().c_str());
        std::string runFolderPath = WindNinjaOutputFolderPath + "/" + wrfBaseName;
        WindNinjaRunFolderPaths.push_back(runFolderPath);
        std::string cfgFilePath = runFolderPath + "/" + wrfBaseName + ".cfg";
        WindNinjaCfgFileNames.push_back(cfgFilePath);   // if this doesn't work, can always do a for loop of chars till hit "/"
    }

    if(use_native_timezone == true)
    {
        WindNinja_timezone = "auto-detect";
    } else
    {
        WindNinja_timezone = "America/Danmarkshavn";
    }

    if(WindNinja_required_output_units == "english")
    {
        WindNinja_output_speed_units = "mph";
        WindNinja_output_wind_height = "20";
        WindNinja_units_output_wind_height = "ft";
        WindNinja_units_ascii_out_resolution = "ft";
    } else
    {
        WindNinja_output_speed_units = "kph";
        WindNinja_output_wind_height = "10";
        WindNinja_units_output_wind_height = "m";
        WindNinja_units_ascii_out_resolution = "m";
    }

    if(diurnal_winds == true)
    {
        WindNinja_diurnal_winds_string = "true";
    } else
    {
        WindNinja_diurnal_winds_string = "false";
    }

    if(non_neutral_stability == true)
    {
        WindNinja_non_neutral_stability_string = "true";
    } else
    {
        WindNinja_non_neutral_stability_string = "false";
    }

    WindNinjaApplicationPath = WindNinjaPath;

    // if it gets here, everything went well
    return true;
}
/*** end reconstructor like functions ***/

/*** functions ***/
bool WindNinjaAPI::create_WindNinja_cfg_files()
{
    bool success = true;

    // create the main WindNinja outputs folder if overwrite outputs is false.
    // Hm, I think technically the given folder name found by the application will be what determines whether overwrite outputs is true or false
    // so if it is false, trying to see if the folder or file already exists will result in a false, so make one.
    // so if it is true, trying to see if the folder or file already exists will result in a true, so don't make one, just overwrite
    // so in the end, just need to know whether it exists or not to know if overwrite or make a new one, names will be the same either way

    // create temporary additionalWindNinjaOutput variables
        // additional_WindNinja_outputs_google
    std::vector<std::string> google_wrfFileNames = stored_additional_WindNinja_Outputs_google.get_stored_wrfFileNames();
    std::vector<boolValue> write_wx_model_goog_output_values = stored_additional_WindNinja_Outputs_google.get_stored_write_wx_model_goog_output_values();
    std::vector<boolValue> write_goog_output_values = stored_additional_WindNinja_Outputs_google.get_stored_write_goog_output_values();
    std::vector<doubleValue> goog_out_resolution_values = stored_additional_WindNinja_Outputs_google.get_stored_goog_out_resolution_values();
    std::vector<stringValue> units_goog_out_resolution_values = stored_additional_WindNinja_Outputs_google.get_stored_units_goog_out_resolution_values();
    std::vector<stringValue> goog_out_color_scheme_values = stored_additional_WindNinja_Outputs_google.get_stored_goog_out_color_scheme_values();
    std::vector<boolValue> goog_out_vector_scaling_values = stored_additional_WindNinja_Outputs_google.get_stored_goog_out_vector_scaling_values();
        // additional_WindNinja_outputs_shapefile
    std::vector<std::string> shapefile_wrfFileNames = stored_additional_WindNinja_Outputs_shapefile.get_stored_wrfFileNames();
    std::vector<boolValue> write_wx_model_shapefile_output_values = stored_additional_WindNinja_Outputs_shapefile.get_stored_write_wx_model_shapefile_output_values();
    std::vector<boolValue> write_shapefile_output_values = stored_additional_WindNinja_Outputs_shapefile.get_stored_write_shapefile_output_values();
    std::vector<doubleValue> shape_out_resolution_values = stored_additional_WindNinja_Outputs_shapefile.get_stored_shape_out_resolution_values();
    std::vector<stringValue> units_shape_out_resolution_values = stored_additional_WindNinja_Outputs_shapefile.get_stored_units_shape_out_resolution_values();
        // additional_WindNinja_outputs_pdf
    std::vector<std::string> pdf_wrfFileNames = stored_additional_WindNinja_Outputs_pdf.get_stored_wrfFileNames();
    std::vector<boolValue> write_pdf_output_values = stored_additional_WindNinja_Outputs_pdf.get_stored_write_pdf_output_values();
    std::vector<doubleValue> pdf_out_resolution_values = stored_additional_WindNinja_Outputs_pdf.get_stored_pdf_out_resolution_values();
    std::vector<stringValue> units_pdf_out_resolution_values = stored_additional_WindNinja_Outputs_pdf.get_stored_units_pdf_out_resolution_values();
    std::vector<doubleValue> pdf_linewidth_values = stored_additional_WindNinja_Outputs_pdf.get_stored_pdf_linewidth_values();
    std::vector<stringValue> pdf_basemap_values = stored_additional_WindNinja_Outputs_pdf.get_stored_pdf_basemap_values();
    std::vector<doubleValue> pdf_height_values = stored_additional_WindNinja_Outputs_pdf.get_stored_pdf_height_values();
    std::vector<doubleValue> pdf_width_values = stored_additional_WindNinja_Outputs_pdf.get_stored_pdf_width_values();
    std::vector<stringValue> pdf_size_values = stored_additional_WindNinja_Outputs_pdf.get_stored_pdf_size_values();

    // all paths up to this point should end without a "/" character
    if(doesFolderExist(WindNinjaOutputFolderPath) == false)
    {
            // now make the directory
        VSIMkdir( WindNinjaOutputFolderPath.c_str(), 0777 );
    }

    for(size_t wrfCount = 0; wrfCount < wrf_files.size(); wrfCount++)
    {
        // make folder for the run if it doesn't yet exist
        if(doesFolderExist(WindNinjaRunFolderPaths[wrfCount]) == false)
        {
                // now make the directory
            VSIMkdir( WindNinjaRunFolderPaths[wrfCount].c_str(), 0777 );
        }

        // next write the cfg files, will always overwrite these if not exist as the actualCreateInputs path takes care of overwriting when not supposed to
        FILE *fzout;
        fzout = fopen(WindNinjaCfgFileNames[wrfCount].c_str(), "w");
        fprintf(fzout,"num_threads                       =   %zu\n",WindNinja_number_of_threads);
        fprintf(fzout,"elevation_file                    =   %s\n",actualLcpFilePath.c_str());
        fprintf(fzout,"initialization_method             =   wxModelInitialization\n");
        fprintf(fzout,"time_zone                         =   %s\n",WindNinja_timezone.c_str());
        fprintf(fzout,"forecast_filename                 =   %s\n",wrf_files[wrfCount].get_storedWrfFileName().c_str());
        fprintf(fzout,"match_points                      =   true\n");
        fprintf(fzout,"output_speed_units                =   %s\n",WindNinja_output_speed_units.c_str());
        fprintf(fzout,"output_wind_height                =   %s\n",WindNinja_output_wind_height.c_str());
        fprintf(fzout,"units_output_wind_height          =   %s\n",WindNinja_units_output_wind_height.c_str());
        fprintf(fzout,"diurnal_winds                     =   %s\n",WindNinja_diurnal_winds_string.c_str());
        if(WindNinja_mesh_choice != "custom")
        {
            fprintf(fzout,"mesh_choice                       =   %s\n",WindNinja_mesh_choice.c_str());
        } else
        {
            fprintf(fzout,"mesh_resolution                   =   %f\n",WindNinja_mesh_resolution);
            fprintf(fzout,"units_mesh_resolution             =   %s\n",WindNinja_mesh_res_units.c_str());
        }
        //fprintf(fzout,"write_wx_model_goog_output        =   %d\n",write_wx_model_goog_output_values[wrfCount].get_storedBoolValue());
        //fprintf(fzout,"write_goog_output                 =   %d\n",write_goog_output_values[wrfCount].get_storedBoolValue());
        //fprintf(fzout,"goog_out_resolution               =   %f\n",goog_out_resolution_values[wrfCount].get_storedDoubleValue());
        //fprintf(fzout,"units_goog_out_resolution         =   %s\n",units_goog_out_resolution_values[wrfCount].get_storedStringValue().c_str());
        //fprintf(fzout,"goog_out_color_scheme             =   %s\n",goog_out_color_scheme_values[wrfCount].get_storedStringValue().c_str());
        //fprintf(fzout,"goog_out_vector_scaling           =   %d\n",goog_out_vector_scaling_values[wrfCount].get_storedBoolValue());
        //fprintf(fzout,"write_wx_model_shapefile_output   =   %d\n",write_wx_model_shapefile_output_values[wrfCount].get_storedBoolValue());
        //fprintf(fzout,"write_shapefile_output            =   %d\n",write_shapefile_output_values[wrfCount].get_storedBoolValue());
        //fprintf(fzout,"shape_out_resolution              =   %f\n",shape_out_resolution_values[wrfCount].get_storedDoubleValue());
        //fprintf(fzout,"units_shape_out_resolution        =   %s\n",units_shape_out_resolution_values[wrfCount].get_storedStringValue().c_str());
        fprintf(fzout,"write_ascii_output                =   true\n");
        fprintf(fzout,"ascii_out_resolution              =   -1\n");
        fprintf(fzout,"units_ascii_out_resolution        =   %s\n",WindNinja_units_ascii_out_resolution.c_str());
        //fprintf(fzout,"write_vtk_output                  =   %d\n",WindNinja_write_vtk_output);
        fprintf(fzout,"write_farsite_atm                 =   true\n");
        //fprintf(fzout,"write_pdf_output                  =   %d\n",write_pdf_output_values[wrfCount].get_storedBoolValue());
        //fprintf(fzout,"pdf_out_resolution                =   %f\n",pdf_out_resolution_values[wrfCount].get_storedDoubleValue());
        //fprintf(fzout,"units_pdf_out_resolution          =   %s\n",units_pdf_out_resolution_values[wrfCount].get_storedStringValue().c_str());
        //fprintf(fzout,"pdf_linewidth                     =   %f\n",pdf_linewidth_values[wrfCount].get_storedDoubleValue());
        //fprintf(fzout,"pdf_basemap                       =   %s\n",pdf_basemap_values[wrfCount].get_storedStringValue().c_str());
        //fprintf(fzout,"pdf_height                        =   %f\n",pdf_height_values[wrfCount].get_storedDoubleValue());
        //fprintf(fzout,"pdf_width                         =   %f\n",pdf_width_values[wrfCount].get_storedDoubleValue());
        //fprintf(fzout,"pdf_size                          =   %s\n",pdf_size_values[wrfCount].get_storedStringValue().c_str());
        fprintf(fzout,"output_path                       =   %s/\n",WindNinjaRunFolderPaths[wrfCount].c_str()); // extra "/" cause that gets lost, can't add it on earlier cause it is one way or the other
        fprintf(fzout,"non_neutral_stability             =   %s\n",WindNinja_non_neutral_stability_string.c_str());
        //if(use_momentum_solver == true)   // if I added this in at some point, would have to do all kinds of fun things with if statements for filenaming stuff
        //{
        //    fprintf(fzout,"momentum_flag                     =   %d\n",use_momentum_solver);
        //    fprintf(fzout,"non_equilibrium_boundary_conditions     =   true\n");    // yes this is optional, but seems like you always want this
        //}
        fclose(fzout);
    }

    return success;
}

bool WindNinjaAPI::run_WindNinja()
{
    bool success = true;

    for(size_t wrfCount = 0; wrfCount < wrf_files.size(); wrfCount++)
    {

        int nRet = -1;

        const char *const papszArgv[] = { "WindNinja_cli",
                                          WindNinjaCfgFileNames[wrfCount].c_str(),
                                          NULL };

        // this method appears to be limited by whatever is the current application in the command line. Doesn't appear possible to supply a path to an executable
        // would like to replace a lot of stuff with WindNinja API
        VSILFILE *fout = VSIFOpenL(CPLFormFilename(WindNinjaRunFolderPaths[wrfCount].c_str(), "log.WindNinjaRun", ""), "w");

        nRet = CPLSpawn(papszArgv, NULL, fout, TRUE);

        VSIFCloseL(fout);

        if(nRet == -1)
        {
            success = false;
            break;
        }
    }

    return success;
}

bool WindNinjaAPI::findFinalRunFiles()
{
    for(size_t runCount = 0; runCount < WindNinjaRunFolderPaths.size(); runCount++)
    {
        std::string currentFolder = WindNinjaRunFolderPaths[runCount];
        if(doesFolderExist(currentFolder) == false)
        {
            printf("folder \"%s\" does not exist!\n",currentFolder.c_str());
            return false;
        }

        std::string atmGlobPattern = currentFolder + "/*.atm";
        std::vector<std::string> foundAtmFiles = globVector(atmGlobPattern);
        if(foundAtmFiles.size() == 0)
        {
            return false;
        } else if(foundAtmFiles.size() > 1)
        {
            printf("found too many atm files for WindNinja run \"%zu\" in path \"%s\"!\n",runCount,currentFolder.c_str());
            return false;
        } else
        {
            atmFiles.push_back(foundAtmFiles[0]);
        }

        std::string velGlobPattern = currentFolder + "/*vel.asc";
        std::vector<std::string> foundVelFiles = globVector(velGlobPattern);
        if(foundVelFiles.size() == 0)
        {
            return false;
        } else if(foundVelFiles.size() > 1)
        {
            printf("found too many velocity files for WindNinja run \"%zu\" in path \"%s\"!\n",runCount,currentFolder.c_str());
            return false;
        } else
        {
            velFiles.push_back(foundVelFiles[0]);
        }

        std::string angGlobPattern = currentFolder + "/*ang.asc";
        std::vector<std::string> foundAngFiles = globVector(angGlobPattern);
        if(foundAngFiles.size() == 0)
        {
            return false;
        } else if(foundAngFiles.size() > 1)
        {
            printf("found too many angle files for WindNinja run \"%zu\" in path \"%s\"!\n",runCount,currentFolder.c_str());
            return false;
        } else
        {
            angFiles.push_back(foundAngFiles[0]);
        }

        std::string cldGlobPattern = currentFolder + "/*cld.asc";
        std::vector<std::string> foundCldFiles = globVector(cldGlobPattern);
        if(foundCldFiles.size() == 0)
        {
            return false;
        } else if(foundCldFiles.size() > 1)
        {
            printf("found too many cloud cover files for WindNinja run \"%zu\" in path \"%s\"!\n",runCount,currentFolder.c_str());
            return false;
        } else
        {
            cldFiles.push_back(foundCldFiles[0]);
        }

    }

    printf("\nFound Wind Files:\n");
    for(size_t fileIdx = 0; fileIdx < atmFiles.size(); fileIdx++)
    {
        printf("atmFiles[%zu] = %s\n",fileIdx,atmFiles[fileIdx].c_str());
    }
    for(size_t fileIdx = 0; fileIdx < velFiles.size(); fileIdx++)
    {
        printf("velFiles[%zu] = %s\n",fileIdx,velFiles[fileIdx].c_str());
    }
    for(size_t fileIdx = 0; fileIdx < angFiles.size(); fileIdx++)
    {
        printf("angFiles[%zu] = %s\n",fileIdx,angFiles[fileIdx].c_str());
    }
    for(size_t fileIdx = 0; fileIdx < cldFiles.size(); fileIdx++)
    {
        printf("cldFiles[%zu] = %s\n",fileIdx,cldFiles[fileIdx].c_str());
    }

    // if it gets here, everything went well
    return true;
}

bool WindNinjaAPI::findWrfTimes()
{
    for(size_t wrfCount = 0; wrfCount < wrf_files.size(); wrfCount++)
    {
        std::string logFileName = WindNinjaRunFolderPaths[wrfCount] + "/log.WindNinjaRun";
        std::ifstream fzInput;
        if(doesFilenameExist(logFileName) == false)
        {
            printf("failed to open WindNinja log file \"%s\"!\n",logFileName.c_str());
            return false;
        }
        fzInput.open(logFileName.c_str());
        std::string currentLine;
        while(std::getline(fzInput,currentLine))
        {
            if(currentLine.substr(0,26) == "Run 0: Simulation time is ")
            {
                std::string timeString = currentLine.substr(26,currentLine.length());

                size_t startValueSpot = 0;
                for(size_t charIdx = 0; charIdx < timeString.length(); charIdx++)
                {
                    std::string currentChr = timeString.substr(charIdx,1);
                    if(currentChr == "-")
                    {
                        std::string foundTime = timeString.substr(startValueSpot,charIdx-startValueSpot);
                        wrfYears.push_back(foundTime);
                        timeString = timeString.substr(charIdx-startValueSpot+1,timeString.length());
                        break;
                    }
                }

                startValueSpot = 0;
                for(size_t charIdx = 0; charIdx < timeString.length(); charIdx++)
                {
                    std::string currentChr = timeString.substr(charIdx,1);
                    if(currentChr == "-")
                    {
                        std::string foundTime = timeString.substr(startValueSpot,charIdx-startValueSpot);
                        wrfMonths.push_back(foundTime);
                        timeString = timeString.substr(charIdx-startValueSpot+1,timeString.length());
                        break;
                    }
                }

                startValueSpot = 0;
                for(size_t charIdx = 0; charIdx < timeString.length(); charIdx++)
                {
                    std::string currentChr = timeString.substr(charIdx,1);
                    if(currentChr == " ")
                    {
                        std::string foundTime = timeString.substr(startValueSpot,charIdx-startValueSpot);
                        wrfDays.push_back(foundTime);
                        timeString = timeString.substr(charIdx-startValueSpot+1,timeString.length());
                        break;
                    }
                }

                startValueSpot = 0;
                for(size_t charIdx = 0; charIdx < timeString.length(); charIdx++)
                {
                    std::string currentChr = timeString.substr(charIdx,1);
                    if(currentChr == ":")
                    {
                        std::string foundTime = timeString.substr(startValueSpot,charIdx-startValueSpot);
                        wrfHours.push_back(foundTime);
                        timeString = timeString.substr(charIdx-startValueSpot+1,timeString.length());
                        break;
                    }
                }

                startValueSpot = 0;
                for(size_t charIdx = 0; charIdx < timeString.length(); charIdx++)
                {
                    std::string currentChr = timeString.substr(charIdx,1);
                    if(currentChr == ":")
                    {
                        std::string foundTime = timeString.substr(startValueSpot,charIdx-startValueSpot);
                        wrfMinutes.push_back(foundTime);
                        timeString = timeString.substr(charIdx-startValueSpot+1,timeString.length());
                        break;
                    }
                }

                startValueSpot = 0;
                for(size_t charIdx = 0; charIdx < timeString.length(); charIdx++)
                {
                    std::string currentChr = timeString.substr(charIdx,1);
                    if(currentChr == " ")
                    {
                        std::string foundTime = timeString.substr(startValueSpot,charIdx-startValueSpot);
                        wrfSeconds.push_back(foundTime);
                        timeString = timeString.substr(charIdx-startValueSpot+1,timeString.length());
                        break;
                    }
                }

                // the leftover is the timezone
                wrfTimeZones.push_back(timeString);
                break;
            }
        }
        fzInput.close();
    }

    // if it gets here, everything went well
    return true;
}
/*** end functions ***/

/*** get value functions ***/
std::vector<std::string> WindNinjaAPI::get_atmFilePaths()
{
    return atmFiles;
}

std::vector<std::string> WindNinjaAPI::get_velFilePaths()
{
    return velFiles;
}

std::vector<std::string> WindNinjaAPI::get_angFilePaths()
{
    return angFiles;
}

std::vector<std::string> WindNinjaAPI::get_cldFilePaths()
{
    return cldFiles;
}

std::vector<std::string> WindNinjaAPI::get_wrfYears()
{
    return wrfYears;
}

std::vector<std::string> WindNinjaAPI::get_wrfMonths()
{
    return wrfMonths;
}

std::vector<std::string> WindNinjaAPI::get_wrfDays()
{
    return wrfDays;
}

std::vector<std::string> WindNinjaAPI::get_wrfHours()
{
    return wrfHours;
}

std::vector<std::string> WindNinjaAPI::get_wrfMinutes()
{
    return wrfMinutes;
}

std::vector<std::string> WindNinjaAPI::get_wrfSeconds()
{
    return wrfSeconds;
}

std::vector<std::string> WindNinjaAPI::get_wrfTimeZones()
{
    return wrfTimeZones;
}
/*** end get value functions ***/



/***** private functions *****/



/*** reconstructor like functions ***/
void WindNinjaAPI::reset()
{
    actual_run_base_name = "";
    actualCreateInputs_path = "";
    actualLcpFilePath = "";
        // application specific variables
    WindNinja_required_output_units = "";
    use_native_timezone = false;
        // WindNinja and getWeather
    extend_wrf_data = false;
    while(!wrf_files.empty())
    {
        wrf_files.pop_back();
    }
        // WindNinja only required vars
    WindNinja_number_of_threads = 0;
    WindNinja_mesh_choice = "";
    WindNinja_mesh_resolution = 0.0;
    WindNinja_mesh_res_units = "";
    diurnal_winds = false;
    non_neutral_stability = false;
        // optional WindNinja variables
    // will do these ones later, as they need special care and are not necessary till the end after this script is beautiful
    // okay I think the trick is that at some point in the process, these have to be generated for each and every wrf file even if not specified by the user,
    // makes it easier to keep stuff of correct sizes and such
        // additional_WindNinja_outputs_google
        // additional_WindNinja_outputs_shapefile
        // additional_WindNinja_outputs_pdf

    // data members created from inputs that are API specific
    WindNinjaOutputFolderPath = "";
    while(!WindNinjaRunFolderPaths.empty())
    {
        WindNinjaRunFolderPaths.pop_back();
    }
    while(!WindNinjaCfgFileNames.empty())
    {
        WindNinjaCfgFileNames.pop_back();
    }

    // too hard to do a specific expected filename, will just look for the basename and the vel.asc extension
    WindNinja_timezone = "";
    WindNinja_output_speed_units = "";
    WindNinja_output_wind_height = "";
    WindNinja_units_output_wind_height = "";
    WindNinja_diurnal_winds_string = "";
    WindNinja_non_neutral_stability_string = "";
    WindNinja_units_ascii_out_resolution = "";

    // desired output values
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

}
/*** end reconstructor like functions ***/

/*** utility functions ***/
bool WindNinjaAPI::doesFolderExist(std::string pathName)
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

bool WindNinjaAPI::doesFilenameExist(std::string fileName)
{
    // found this here: https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
    // hopefully it works for files as well as directories as this is the same thing used to check a file path. More interesting hopefully this works for .prj files!
    struct stat buffer;
    return (stat (fileName.c_str(), &buffer) == 0);
}

bool WindNinjaAPI::doesNetCDFFileExist(std::string pathName)
{
    GDALDatasetH  hDataset;
    std::string netCDFFileString = "NETCDF:\"" + pathName + "\"";
    CPLPushErrorHandler(&CPLQuietErrorHandler);
    hDataset = GDALOpen( netCDFFileString.c_str(), GA_ReadOnly );
    CPLPopErrorHandler();
    if(hDataset == NULL)
    {
        return false;
    }
    GDALClose(hDataset);

    // it got here so it was okay
    return true;
}

std::vector<std::string> WindNinjaAPI::globVector(const std::string& pattern){
    glob_t glob_result;
    glob(pattern.c_str(),GLOB_TILDE,NULL,&glob_result);
    std::vector<std::string> files;
    for(unsigned int i=0;i<glob_result.gl_pathc;++i){
        files.push_back(std::string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
    return files;
}

bool WindNinjaAPI::doOutputFilesExist(size_t runNumber)
{
    bool exists = true;

    std::string currentFolder = WindNinjaRunFolderPaths[runNumber];
    if(doesFolderExist(currentFolder) == false)
    {
        exists = false;
    }

    if(exists == true)
    {
        std::string globPattern = WindNinjaOutputFolderPath[runNumber] + "/*.asc";
        std::vector<std::string> foundAsciiFiles = globVector(globPattern);
        if(foundAsciiFiles.size() == 0)
        {
            exists = false;
        } else
        {
            for(size_t asciiCount = 0; asciiCount < foundAsciiFiles.size(); asciiCount++)
            {
                std::string currentFileName = foundAsciiFiles[asciiCount];
                if(currentFileName.length() >= 5+actual_run_base_name.length())
                {
                    if(currentFileName.substr(0,actual_run_base_name.length()) == actual_run_base_name && currentFileName.substr(currentFileName.length()-7,currentFileName.length()) == "vel.asc")
                    {
                        exists = true;
                        break;
                    }
                }
            }
        }
    }

    return exists;
}
/*** end utility functions ***/
