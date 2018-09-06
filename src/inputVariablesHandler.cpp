#include "inputVariablesHandler.h"

/***** public functions *****/

/*** constructor functions ***/
inputVariablesHandler::inputVariablesHandler()
{
    // load classes that define variable names and their conflicting options (maybe eventually error messages? no a case by case basis seems easier)
    inputVariableInfo = inputVariableValues.get_inputVariableInfo();
    // pass variable information into input parser so it can change the same values
    theInputParser.loadVariableInfo(&inputVariableValues);    // not sure if this is the right way to use pointers in this particular case, since setting something equal to the pointer in the function
}
/*** end constructor functions ***/

/*** data passer between inputVariablesHandler and inputParser functions ***/
void inputVariablesHandler::transferVariableInfo()
{
    inputVariableInfo = theInputParser.transferVariableInfo();
}
/*** end data passer between inputVariablesHandler and inputParser functions ***/

/*** functions ***/
void inputVariablesHandler::explainInputReqs()
{
    printf("WRF-WindNinja-FarsiteScript Usage:\n"
           "WRF-WindNinja-FarsiteScript [inputfile]\n"
           "Where:\t[commandfile] is the path to the input file.\n\n");
    printf("The input file can contain the following variables:\n");
    printf("\n%s input variables\n",inputVariableInfo[0].get_applicationUseName().c_str());
    for(size_t varIdx = 0; varIdx < inputVariableInfo.size(); varIdx++)
    {
        if(varIdx != 0 && inputVariableInfo[varIdx].get_applicationUseName() != inputVariableInfo[varIdx-1].get_applicationUseName())
        {
            printf("\n%s input variables\n",inputVariableInfo[varIdx].get_applicationUseName().c_str());
        }
        // need to add a description overall vartype to detect when to split up sections between application type variables
        std::vector<unsigned int> currentLineBreaks = inputVariableInfo[varIdx].get_variableDescriptionLineBreaks();
        printf("%s%s%s\n",inputVariableInfo[varIdx].get_variableName().c_str(),inputVariableInfo[varIdx].get_variableNameWhiteSpace().c_str(),inputVariableInfo[varIdx].get_variableDescription().substr(0,currentLineBreaks[1]).c_str());
        for(size_t lineBreakIdx = 1; lineBreakIdx < currentLineBreaks.size()-1; lineBreakIdx++)
        {
            printf("%s%s\n",inputVariableValues.get_maxVarNameColumnWhitespace().c_str(),inputVariableInfo[varIdx].get_variableDescription().substr(currentLineBreaks[lineBreakIdx],currentLineBreaks[lineBreakIdx+1]-currentLineBreaks[lineBreakIdx]).c_str());
        }
        printf("\n");
    }
    printf("end description\n");
}

bool inputVariablesHandler::loadScriptInputs(std::string inputFileName)
{
    bool success = true;

    /* clean up all data that isn't just defined once for all uses */
    if(reset() == false)
    {
        printf("problem resetting variables!\n");
        success = false;
    }

    //open file and load all variable names with parser
    if(success == true)
    {
        if(theInputParser.findVariableNamesAndCountValues(inputFileName) == false)
        {
            printf("problem loading variable names and variable count values!\n");
            success = false;
        } else
        {
            transferVariableInfo(); // don't be caught off gaurd, runs theInputParser.findVariableNamesAndCountValues if success is true, if that succeeds runs transferVariableInfo
        }
    }

    // need an additional function checking count type "string" info. Oh heck, this assumes I read in count values by this point.
    // maybe should just put it in findVariableNames for if count type is "string" do a bunch of other extra stuff

    //check for conflicting options and stop and warn for all conflicts. Oh heck, this assumes I read in count values by this point.
    if(success == true)
    {
        if(verifyFoundInputCombinations() == false)
        {
            printf("found conflicting options!\n");
            success = false;
        }
    }

    //if no conflicting options, open file and load each input with parser, error if any cannot be loaded correctly
    if(success == true)
    {
        if(theInputParser.loadAllInputs(inputFileName) == false)
        {
            printf("problem loading inputs!\n");
            success = false;
        } else
        {
            transferVariableInfo(); // don't be caught off gaurd, runs theInputParser.loadAllInputs if success is true, if that succeeds runs transferVariableInfo
        }
        // comment this out unless debugging
        printFoundInput();
    }

    // now get the actual output paths for use by everything
    if(success == true)
    {
        if(findActualCreateInputsAndFinalOutputsPaths() == false)
        {
            printf("problem finding actual output paths!\n");
            success = false;
        }
    }

    // now get the actual lcp path and run base name setup for use by everything
    if(success == true)
    {
        if(findActualLcpPathAndBaseName() == false)
        {
            printf("problem finding actual lcp path and run base name!\n");
            success = false;
        }
    }

    return success;
}

void inputVariablesHandler::printFoundInput()
{
    printf("\nprinting found input\n\n");
    for(size_t varIdx = 0; varIdx < inputVariableInfo.size(); varIdx++)
    {
        std::string currentVarName = inputVariableInfo[varIdx].get_variableName();
        std::string currentCountType = inputVariableInfo[varIdx].get_variableCountType();
        // setup extra label thingee since default values are hard to tell what they are
        std::string isFoundInInputFile = "";
        if(inputVariableInfo[varIdx].get_wantDefaultValue() == true)
        {
            if(inputVariableInfo[varIdx].get_isFoundInInputFile() == false)
            {
                isFoundInInputFile = "            \"wantDefaultValue, isDefaultValue\"";
            } else
            {
                isFoundInInputFile = "            \"!wantDefaultValue, BUT_IS_FoundInInputFile!\"";
            }
        } else
        {
            if(inputVariableInfo[varIdx].get_isFoundInInputFile() == false)
            {
                isFoundInInputFile = "            \"!wantInputValue, BUT_IS_DefaultValue!\"";
            }
        }
        if(currentCountType == "bool")
        {
            printf("%s: %d%s\n",currentVarName.c_str(),get_boolValue(currentVarName).get_storedBoolValue(),isFoundInInputFile.c_str());
        } else if(currentCountType == "size_t")
        {
            printf("%s: %zu%s\n",currentVarName.c_str(),get_size_tValue(currentVarName).get_storedSize_tValue(),isFoundInInputFile.c_str());
        } else if(currentCountType == "int")
        {
            printf("%s: %d%s\n",currentVarName.c_str(),get_intValue(currentVarName).get_storedIntValue(),isFoundInInputFile.c_str());
        } else if(currentCountType == "double")
        {
            printf("%s: %f%s\n",currentVarName.c_str(),get_doubleValue(currentVarName).get_storedDoubleValue(),isFoundInInputFile.c_str());
        } else if(currentCountType == "positive double")
        {
            printf("%s: %f%s\n",currentVarName.c_str(),get_doubleValue(currentVarName).get_storedDoubleValue(),isFoundInInputFile.c_str());
        } else if(currentCountType == "signless percent")
        {
            printf("%s: %f%s\n",currentVarName.c_str(),get_doubleValue(currentVarName).get_storedDoubleValue(),isFoundInInputFile.c_str());
        } else if(currentCountType == "string")
        {
            printf("%s: %s%s\n",currentVarName.c_str(),get_stringValue(currentVarName).get_storedStringValue().c_str(),isFoundInInputFile.c_str());
        } else if(currentCountType == "pathname")
        {
            printf("%s: %s%s\n",currentVarName.c_str(),get_pathNameValue(currentVarName).get_storedPathNameValue().c_str(),isFoundInInputFile.c_str());
        } else if(currentCountType == "lcp filename")
        {
            printf("%s: %s%s\n",currentVarName.c_str(),get_lcpFileValue(currentVarName).get_storedLcpFileValue().c_str(),isFoundInInputFile.c_str());
        } else if(currentCountType == "shape filename")
        {
            printf("%s: %s%s\n",currentVarName.c_str(),get_shapeFileValue(currentVarName).get_storedShapeFileValue().c_str(),isFoundInInputFile.c_str());
        } else if(currentCountType == "wrf filename")
        {
            printf("%s: %s%s\n",currentVarName.c_str(),get_wrfFileValue(currentVarName).get_storedWrfFileName().c_str(),isFoundInInputFile.c_str());
        } else if(currentCountType == "lat_coord")
        {
            printf("%s: %f%s\n",currentVarName.c_str(),get_doubleValue(currentVarName).get_storedDoubleValue(),isFoundInInputFile.c_str());
        } else if(currentCountType == "long_coord")
        {
            printf("%s: %f%s\n",currentVarName.c_str(),get_doubleValue(currentVarName).get_storedDoubleValue(),isFoundInInputFile.c_str());
        } else if(currentCountType == "lat_long_point")
        {
            printf("%s: %f %f%s\n",currentVarName.c_str(),get_lat_longValue(currentVarName).get_storedLatValue(),get_lat_longValue(currentVarName).get_storedLongValue(),isFoundInInputFile.c_str());
        } else if(currentCountType == "date")
        {
            dateValue currentDate = get_dateValue(currentVarName);
            printf("%s: %d %d %d %d:%d%s\n",currentVarName.c_str(),currentDate.get_storedMonthValue(),currentDate.get_storedDayValue(),currentDate.get_storedYearValue(),currentDate.get_storedHourValue(),currentDate.get_storedMinuteValue(),isFoundInInputFile.c_str());
        } else if(currentCountType == "hour_min")
        {
            printf("%s: %d%d%s\n",currentVarName.c_str(),get_hour_minValue(currentVarName).get_storedHourValue(),get_hour_minValue(currentVarName).get_storedMinuteValue(),isFoundInInputFile.c_str());
        } else if(currentCountType == "count")
        {
            // hm, this function is revealing the complexities and confusingness of even more input stuff
            // do I try to repeat all the rest of the inputs checking that WindNinja and Farsite do?
            printf("%s: %zu%s\n",currentVarName.c_str(),inputVariableValues.getTypeCount_CountVar(currentVarName),isFoundInInputFile.c_str());
            if(currentVarName == "create_ignition_from_latlongs")
            {
                for(size_t countVarIdx = 0; countVarIdx < get_createIgnitionFromLatLongsStorage().get_storedCreateIgnitionLatLongValues().size(); countVarIdx++)
                {
                    printf("%f %f\n",get_createIgnitionFromLatLongsStorage().get_storedCreateIgnitionLatLongValues()[countVarIdx].get_storedLatValue(),get_createIgnitionFromLatLongsStorage().get_storedCreateIgnitionLatLongValues()[countVarIdx].get_storedLongValue());
                }
            } else if(currentVarName == "polygon_ignit_shape_files")
            {
                for(size_t countVarIdx = 0; countVarIdx < get_polygonIgnitShapeFileStorage().get_storedPolygonIgnitShapeFiles().size(); countVarIdx++)
                {
                    printf("%s\n",get_polygonIgnitShapeFileStorage().get_storedPolygonIgnitShapeFiles()[countVarIdx].get_storedShapeFileValue().c_str());
                }
            } else if(currentVarName == "GeoMAC_fire_perimeter_files")
            {
                for(size_t countVarIdx = 0; countVarIdx < get_GeoMACfirePerimeterFileStorage().get_storedGeoMACfirePerimeterFiles().size(); countVarIdx++)
                {
                    printf("%s\n",get_GeoMACfirePerimeterFileStorage().get_storedGeoMACfirePerimeterFiles()[countVarIdx].get_storedShapeFileValue().c_str());
                }
            } else if(currentVarName == "farsite_output_fire_perimeter_files")
            {
                for(size_t countVarIdx = 0; countVarIdx < get_farsiteOutputFirePerimeterFileStorage().get_storedFarsiteOutputFirePerimeterFiles().size(); countVarIdx++)
                {
                    printf("%s\n",get_farsiteOutputFirePerimeterFileStorage().get_storedFarsiteOutputFirePerimeterFiles()[countVarIdx].get_storedShapeFileValue().c_str());
                }
            } else if(currentVarName == "wrf_files")
            {
                for(size_t countVarIdx = 0; countVarIdx < get_wrfFileStorage().get_storedWrfFiles().size(); countVarIdx++)
                {
                    printf("%s\n",get_wrfFileStorage().get_storedWrfFiles()[countVarIdx].get_storedWrfFileName().c_str());
                }
            } else if(currentVarName == "additional_WindNinja_outputs_google")
            {
                std::vector<std::string> current_wrf_files = get_additionalWindNinjaOutputs_googleStorage().get_stored_wrfFileNames();
                std::vector<boolValue> current_write_wx_model_goog_output_values = get_additionalWindNinjaOutputs_googleStorage().get_stored_write_wx_model_goog_output_values();
                std::vector<boolValue> current_write_goog_output_values = get_additionalWindNinjaOutputs_googleStorage().get_stored_write_goog_output_values();
                std::vector<doubleValue> current_goog_out_resolution_values = get_additionalWindNinjaOutputs_googleStorage().get_stored_goog_out_resolution_values();
                std::vector<stringValue> current_units_goog_out_resolution_values = get_additionalWindNinjaOutputs_googleStorage().get_stored_units_goog_out_resolution_values();
                std::vector<stringValue> current_goog_out_color_scheme_values = get_additionalWindNinjaOutputs_googleStorage().get_stored_goog_out_color_scheme_values();
                std::vector<boolValue> current_goog_out_vector_scaling_values = get_additionalWindNinjaOutputs_googleStorage().get_stored_goog_out_vector_scaling_values();
                for(size_t countVarIdx = 0; countVarIdx < current_wrf_files.size(); countVarIdx++)
                {
                    printf("%s %d %d %f %s %s %d\n",current_wrf_files[countVarIdx].c_str(),current_write_wx_model_goog_output_values[countVarIdx].get_storedBoolValue(),
                           current_write_goog_output_values[countVarIdx].get_storedBoolValue(),current_goog_out_resolution_values[countVarIdx].get_storedDoubleValue(),
                           current_units_goog_out_resolution_values[countVarIdx].get_storedStringValue().c_str(),current_goog_out_color_scheme_values[countVarIdx].get_storedStringValue().c_str(),
                           current_goog_out_vector_scaling_values[countVarIdx].get_storedBoolValue());
                }
            } else if(currentVarName == "additional_WindNinja_outputs_shapefile")
            {
                std::vector<std::string> current_wrf_files = get_additionalWindNinjaOutputs_shapefileStorage().get_stored_wrfFileNames();
                std::vector<boolValue> current_write_wx_model_shapefile_output_values = get_additionalWindNinjaOutputs_shapefileStorage().get_stored_write_wx_model_shapefile_output_values();
                std::vector<boolValue> current_write_shapefile_output_values = get_additionalWindNinjaOutputs_shapefileStorage().get_stored_write_shapefile_output_values();
                std::vector<doubleValue> current_shape_out_resolution_values = get_additionalWindNinjaOutputs_shapefileStorage().get_stored_shape_out_resolution_values();
                std::vector<stringValue> current_units_shape_out_resolution_values = get_additionalWindNinjaOutputs_shapefileStorage().get_stored_units_shape_out_resolution_values();
                for(size_t countVarIdx = 0; countVarIdx < current_wrf_files.size(); countVarIdx++)
                {
                    printf("%s %d %d %f %s\n",current_wrf_files[countVarIdx].c_str(),current_write_wx_model_shapefile_output_values[countVarIdx].get_storedBoolValue(),
                           current_write_shapefile_output_values[countVarIdx].get_storedBoolValue(),current_shape_out_resolution_values[countVarIdx].get_storedDoubleValue(),
                           current_units_shape_out_resolution_values[countVarIdx].get_storedStringValue().c_str());
                }
            } else if(currentVarName == "additional_WindNinja_outputs_pdf")
            {
                std::vector<std::string> current_wrf_files = get_additionalWindNinjaOutputs_pdfStorage().get_stored_wrfFileNames();
                std::vector<boolValue> current_write_pdf_output_values = get_additionalWindNinjaOutputs_pdfStorage().get_stored_write_pdf_output_values();
                std::vector<doubleValue> current_pdf_out_resolution_values = get_additionalWindNinjaOutputs_pdfStorage().get_stored_pdf_out_resolution_values();
                std::vector<stringValue> current_units_pdf_out_resolution_values = get_additionalWindNinjaOutputs_pdfStorage().get_stored_units_pdf_out_resolution_values();
                std::vector<doubleValue> current_pdf_linewidth_values = get_additionalWindNinjaOutputs_pdfStorage().get_stored_pdf_linewidth_values();
                std::vector<stringValue> current_pdf_basemap_values = get_additionalWindNinjaOutputs_pdfStorage().get_stored_pdf_basemap_values();
                std::vector<doubleValue> current_pdf_height_values = get_additionalWindNinjaOutputs_pdfStorage().get_stored_pdf_height_values();
                std::vector<doubleValue> current_pdf_width_values = get_additionalWindNinjaOutputs_pdfStorage().get_stored_pdf_width_values();
                std::vector<stringValue> current_pdf_size_values = get_additionalWindNinjaOutputs_pdfStorage().get_stored_pdf_size_values();
                for(size_t countVarIdx = 0; countVarIdx < current_wrf_files.size(); countVarIdx++)
                {
                    printf("%s %d %f %s %f %s %f %f %s\n",current_wrf_files[countVarIdx].c_str(),current_write_pdf_output_values[countVarIdx].get_storedBoolValue(),
                           current_pdf_out_resolution_values[countVarIdx].get_storedDoubleValue(),current_units_pdf_out_resolution_values[countVarIdx].get_storedStringValue().c_str(),
                           current_pdf_linewidth_values[countVarIdx].get_storedDoubleValue(),current_pdf_basemap_values[countVarIdx].get_storedStringValue().c_str(),
                           current_pdf_height_values[countVarIdx].get_storedDoubleValue(),current_pdf_width_values[countVarIdx].get_storedDoubleValue(),
                           current_pdf_size_values[countVarIdx].get_storedStringValue().c_str());
                }
            } else
            {
                printf("loader function stuff for variable \"%s\" has not been implemented in the code yet!\n",currentVarName.c_str());
            }
        } else
        {
            printf("count type \"%s\" for variable \"%s\" has not been implemented in the code yet!\n",currentCountType.c_str(),currentVarName.c_str());
        }
    }
    printf("\nfinished printing found input\n\n");
}

std::string inputVariablesHandler::get_actualCreateInputs_path()
{
    return actualCreateInputs_path;
}

std::string inputVariablesHandler::get_actualFinalOutput_path()
{
    return actualFinalOutput_path;
}

std::string inputVariablesHandler::get_actual_run_base_name()
{
    return actual_run_base_name;
}

std::string inputVariablesHandler::get_actualLcpPath()
{
    return actualLcpPath;
}
/*** end  functions ***/

/*** get variable value functions for single values ***/
boolValue inputVariablesHandler::get_boolValue(std::string varName)
{
    return inputVariableValues.get_boolValue(varName);
}

size_tValue inputVariablesHandler::get_size_tValue(std::string varName)
{
    return inputVariableValues.get_size_tValue(varName);
}

intValue inputVariablesHandler::get_intValue(std::string varName)
{
    return inputVariableValues.get_intValue(varName);
}

doubleValue inputVariablesHandler::get_doubleValue(std::string varName)
{
    return inputVariableValues.get_doubleValue(varName);
}

stringValue inputVariablesHandler::get_stringValue(std::string varName)
{
    return inputVariableValues.get_stringValue(varName);
}

pathNameValue inputVariablesHandler::get_pathNameValue(std::string varName)
{
    return inputVariableValues.get_pathNameValue(varName);
}

lcpFileValue inputVariablesHandler::get_lcpFileValue(std::string varName)
{
    return inputVariableValues.get_lcpFileValue(varName);
}

shapeFileValue inputVariablesHandler::get_shapeFileValue(std::string varName)
{
    return inputVariableValues.get_shapeFileValue(varName);
}

wrfFileValue inputVariablesHandler::get_wrfFileValue(std::string varName)
{
    return inputVariableValues.get_wrfFileValue(varName);
}

lat_longValue inputVariablesHandler::get_lat_longValue(std::string varName)
{
    return inputVariableValues.get_lat_longValue(varName);
}

dateValue inputVariablesHandler::get_dateValue(std::string varName)
{
    return inputVariableValues.get_dateValue(varName);
}

hour_minValue inputVariablesHandler::get_hour_minValue(std::string varName)
{
    return inputVariableValues.get_hour_minValue(varName);
}
/*** end get variable value functions for single values ***/

/*** get variable value functions for count values ***/
createIgnitionFromLatLongsStorage inputVariablesHandler::get_createIgnitionFromLatLongsStorage()
{
    return inputVariableValues.get_createIgnitionFromLatLongsStorage();
}

polygonIgnitShapeFileStorage inputVariablesHandler::get_polygonIgnitShapeFileStorage()
{
    return inputVariableValues.get_polygonIgnitShapeFileStorage();
}

GeoMACfirePerimeterFileStorage inputVariablesHandler::get_GeoMACfirePerimeterFileStorage()
{
    return inputVariableValues.get_GeoMACfirePerimeterFileStorage();
}

farsiteOutputFirePerimeterFileStorage inputVariablesHandler::get_farsiteOutputFirePerimeterFileStorage()
{
    return inputVariableValues.get_farsiteOutputFirePerimeterFileStorage();
}

wrfFileStorage inputVariablesHandler::get_wrfFileStorage()
{
    return inputVariableValues.get_wrfFileStorage();
}

additionalWindNinjaOutputs_googleStorage inputVariablesHandler::get_additionalWindNinjaOutputs_googleStorage()
{
    return inputVariableValues.get_additionalWindNinjaOutputs_googleStorage();
}

additionalWindNinjaOutputs_shapefileStorage inputVariablesHandler::get_additionalWindNinjaOutputs_shapefileStorage()
{
    return inputVariableValues.get_additionalWindNinjaOutputs_shapefileStorage();
}

additionalWindNinjaOutputs_pdfStorage inputVariablesHandler::get_additionalWindNinjaOutputs_pdfStorage()
{
    return inputVariableValues.get_additionalWindNinjaOutputs_pdfStorage();
}
/*** end get variable value functions for count values ***/



/***** private functions *****/



/*** other functions ***/
bool inputVariablesHandler::reset()
{
    bool success = true;

    actualCreateInputs_path = "";
    actualFinalOutput_path = "";
    actual_run_base_name = "";
    actualLcpPath = "";

    for(size_t varIdx = 0; varIdx < inputVariableInfo.size(); varIdx++)
    {
        inputVariableInfo[varIdx].resetVariable();
    }
    success = inputVariableValues.resetVariables();
    return success;
}

// I think super slow cause have to go through every single variable multiple times
bool inputVariablesHandler::verifyFoundInputCombinations()
{
    bool success = true;

    // setup whatever you need to set if input variables are optional or not, or how they are optional or required
    // set_wantDefaultValue
    // I would keep the order of these checkers as close to the input variable list order as possible to keep it hopefully less confusing




    return success;
}

bool inputVariablesHandler::doesFolderExist(std::string pathName)
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

bool inputVariablesHandler::findActualCreateInputsAndFinalOutputsPaths()
{
    bool success = true;

    std::string foundCreateInputs_path = get_pathNameValue("createInputs_path").get_storedPathNameValue() + "/";
    std::string foundFinalOutput_path = get_pathNameValue("finalOutput_path").get_storedPathNameValue() + "/";

    // check for "/" symbols
    if(foundCreateInputs_path.substr(foundCreateInputs_path.length()-2,foundCreateInputs_path.length()-1) == "/")
    {
        foundCreateInputs_path = foundCreateInputs_path.substr(0,foundCreateInputs_path.length()-1);
    }
    if(foundFinalOutput_path.substr(foundFinalOutput_path.length()-2,foundFinalOutput_path.length()-1) == "/")
    {
        foundFinalOutput_path = foundFinalOutput_path.substr(0,foundFinalOutput_path.length()-1);
    }

    foundCreateInputs_path = foundCreateInputs_path + "createInputs";
    foundFinalOutput_path = foundFinalOutput_path + "finalOutput";

    if(get_boolValue("overwrite_past_outputs").get_storedBoolValue() == true)
    {
        actualCreateInputs_path = foundCreateInputs_path;
        actualFinalOutput_path = foundFinalOutput_path;
    } else
    {
        std::string currentCreateInputsPath = foundCreateInputs_path;
        std::string currentFinalOutputPath = foundFinalOutput_path;

        if(doesFolderExist(currentCreateInputsPath) == false && doesFolderExist(currentFinalOutputPath) == false)
        {
            actualCreateInputs_path = foundCreateInputs_path;
            actualFinalOutput_path = foundFinalOutput_path;
        } else
        {
            size_t fileIdxCount = 0;
            while(doesFolderExist(currentCreateInputsPath) == true || doesFolderExist(currentFinalOutputPath) == true)
            {
                fileIdxCount = fileIdxCount + 1;
                currentCreateInputsPath = foundCreateInputs_path + "-" + std::to_string(fileIdxCount);
                currentFinalOutputPath = foundFinalOutput_path + "-" + std::to_string(fileIdxCount);
            }

            actualCreateInputs_path = currentCreateInputsPath;
            actualFinalOutput_path = currentFinalOutputPath;
        }
    }

    return success;
}

bool inputVariablesHandler::findActualLcpPathAndBaseName()
{
    bool success = true;

    if(get_boolValue("use_past_lcp").get_storedBoolValue() == true)
    {
        actualLcpPath = get_lcpFileValue("lcp_file_path").get_storedLcpFileValue();
        actual_run_base_name = actualLcpPath.substr(0,actualLcpPath.length()-4);
    } else
    {
        actual_run_base_name = get_stringValue("run_base_name").get_storedStringValue();
        actualLcpPath = get_actualCreateInputs_path() + actual_run_base_name + ".lcp";
    }

    return success;
}
/*** end other functions ***/
