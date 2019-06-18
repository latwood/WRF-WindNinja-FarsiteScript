#include "inputVariablesHandler.h"

/***** public functions *****/

/*** constructor functions ***/
inputVariablesHandler::inputVariablesHandler()
{
    printf("running inputVariablesHandler constructor\n");
    // load classes that define variable names and their conflicting options (maybe eventually error messages? no a case by case basis seems easier)
    inputVariableStorage = initialVariables.get_inputVariableStorage();
    // pass variable information into input parser so it can change the same values
    theInputParser.loadInputVariableStorage(inputVariableStorage, stored_create_ignition_from_latlongs, stored_polygon_ignit_shape_files, stored_GeoMAC_fire_perimeter_files,
                                            stored_farsite_output_fire_perimeter_files, stored_wrf_files, stored_additional_WindNinja_Outputs_google,
                                            stored_additional_WindNinja_Outputs_shapefile, stored_additional_WindNinja_Outputs_pdf);
    printf("finished inputVariablesHandler constructor\n");
}
/*** end constructor functions ***/

/*** data passer between inputVariablesHandler and inputParser functions ***/
void inputVariablesHandler::transferInputVariableStorage()
{
    inputVariableStorage = theInputParser.transfer_inputVariableStorage();
    stored_create_ignition_from_latlongs = theInputParser.transfer_stored_create_ignition_from_latlongs();
    stored_polygon_ignit_shape_files = theInputParser.transfer_stored_polygon_ignit_shape_files();
    stored_GeoMAC_fire_perimeter_files = theInputParser.transfer_stored_GeoMAC_fire_perimeter_files();
    stored_farsite_output_fire_perimeter_files = theInputParser.transfer_stored_farsite_output_fire_perimeter_files();
    stored_wrf_files = theInputParser.transfer_stored_wrf_files();
    stored_additional_WindNinja_Outputs_google = theInputParser.transfer_stored_additional_WindNinja_Outputs_google();
    stored_additional_WindNinja_Outputs_shapefile = theInputParser.transfer_stored_additional_WindNinja_Outputs_shapefile();
    stored_additional_WindNinja_Outputs_pdf = theInputParser.transfer_stored_additional_WindNinja_Outputs_pdf();
}
/*** end data passer between inputVariablesHandler and inputParser functions ***/

/*** functions ***/
void inputVariablesHandler::explainInputReqs()
{
    printf("WRF-WindNinja-FarsiteScript Usage:\n"
           "WRF-WindNinja-FarsiteScript [inputfile]\n"
           "Where:\t[inputfile] is the path to the input file.\n\n");
    printf("The input file can contain the following variables:\n");
    printf("\n%s input variables\n",inputVariableStorage[0].get_applicationUseName().c_str());
    for(size_t varIdx = 0; varIdx < inputVariableStorage.size(); varIdx++)
    {
        if(varIdx != 0 && inputVariableStorage[varIdx].get_applicationUseName() != inputVariableStorage[varIdx-1].get_applicationUseName())
        {
            printf("\n%s input variables\n",inputVariableStorage[varIdx].get_applicationUseName().c_str());
        }
        // need to add a description overall vartype to detect when to split up sections between application type variables
        std::vector<unsigned int> currentLineBreaks = inputVariableStorage[varIdx].get_variableDescriptionLineBreaks();
        printf("%s%s%s\n",inputVariableStorage[varIdx].get_variableName().c_str(),inputVariableStorage[varIdx].get_variableNameWhiteSpace().c_str(),inputVariableStorage[varIdx].get_variableDescription().substr(0,currentLineBreaks[1]).c_str());
        for(size_t lineBreakIdx = 1; lineBreakIdx < currentLineBreaks.size()-1; lineBreakIdx++)
        {
            printf("%s%s\n",initialVariables.get_maxVarNameColumnWhitespace().c_str(),inputVariableStorage[varIdx].get_variableDescription().substr(currentLineBreaks[lineBreakIdx],currentLineBreaks[lineBreakIdx+1]-currentLineBreaks[lineBreakIdx]).c_str());
        }
        printf("\n");
    }
    printf("end description\n");
}

bool inputVariablesHandler::loadScriptInputs(std::string inputFileName)
{
    bool success = true;

    // clean up all data that isn't just defined once for all uses
    if(reset() == false)
    {
        printf("problem resetting variables!\n");
        success = false;
    }

    // open file and load all found text into the input parser
    if(success == true)
    {
        printf("loading all found text\n");
        if(theInputParser.readInputFile(inputFileName) == false)
        {
            printf("problem loading all text into the input parser class!\n");
            success = false;
        }
    }

    // load all found variable names and variable count values into input parser as strings
    if(success == true)
    {
        printf("loading all variable names and variable count values into input parser as strings\n");
        if(theInputParser.readVarNamesAndCountValues() == false)
        {
            printf("problem loading all variable names and count values into input parser class as strings!\n");
            success = false;
        }
    }

    //check for conflicting options and stop and warn for all conflicts
    if(success == true)
    {
        printf("checking for conflicting options\n");
        transferInputVariableStorage(); // don't be caught off gaurd, runs theInputParser.loadAllInputs if success is true, if that succeeds runs transferVariableInfo
        if(verifyFoundInputCombinations() == false)
        {
            printf("found conflicting options!\n");
            success = false;
        }
    }

    // now set all count values to their respective input storage classes
    if(success == true)
    {
        printf("loading count values into storage classes\n");
        if(theInputParser.loadCountValues() == false)
        {
            printf("problem setting count values into their respective input storage classes!\n");
            success = false;
        }
    }

    // now make sure there are enough lines of data for all type count variables
    if(success == true)
    {
        printf("checking data validity against count variable values\n");
        if(theInputParser.checkCountVsLinesOfData() == false)
        {
            printf("problem with input number of lines of input data for type count variables!\n");
            success = false;
        }
    }

    // if no conflicting options, open file and load each input with parser, error if any cannot be loaded correctly
    if(success == true)
    {
        printf("load in multiline data to input storage classes\n");
        if(theInputParser.loadLoaderFunctionData() == false)
        {
            printf("problem loading multiple line data inputs!\n");
            success = false;
        }
        // transfer final data back before doing any printing or whatever
        transferInputVariableStorage(); // don't be caught off gaurd, runs theInputParser.loadAllInputs if success is true, if that succeeds runs transferVariableInfo
        // comment this out unless debugging
        //printFoundInput();
    }

    // now get the actual output paths for use by everything
    if(success == true)
    {
        printf("getting actual paths and basename\n");
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
    for(size_t varIdx = 0; varIdx < inputVariableStorage.size(); varIdx++)
    {
        std::string currentVarName = inputVariableStorage[varIdx].get_variableName();
        std::string currentCountType = inputVariableStorage[varIdx].get_variableCountType();
        // setup extra label thingee since default values are hard to tell what they are
        std::string isFoundInInputFile = "";
        if(inputVariableStorage[varIdx].get_wantDefaultValue() == true)
        {
            if(inputVariableStorage[varIdx].get_isFoundInInputFile() == false)
            {
                isFoundInInputFile = "            \"wantDefaultValue, isDefaultValue\"";
            } else
            {
                isFoundInInputFile = "            \"!wantDefaultValue, BUT_IS_FoundInInputFile!\"";
            }
        } else
        {
            if(inputVariableStorage[varIdx].get_isFoundInInputFile() == false)
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
            printf("%s: %zu%s\n",currentVarName.c_str(),get_size_tValue(currentVarName).get_storedSize_tValue(),isFoundInInputFile.c_str());
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
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to set the value of interest
    //((boolValue*)variableTypeClass)->set_storedBoolValue(true);
    // use the pointer to get the value of interest
    // bool foundValue = ((boolValue*)variableTypeClass->get_storedBoolValue();
    // use the pointer to get the value of interest
    boolValue foundValue = *((boolValue*)variableTypeClass);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;
    // return the found value
    return foundValue;

    // if it got here, the variable name doesn't exist in storage
    printf("could not get bool value for variable name \"%s\", variableName \%s\" does not exist in bool value storage!\n",varName.c_str(),varName.c_str());
    exit(1);
}

size_tValue inputVariablesHandler::get_size_tValue(std::string varName)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to get the value of interest
    size_tValue foundValue = *((size_tValue*)variableTypeClass);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;
    // return the found value
    return foundValue;

    // if it got here, the variable name doesn't exist in storage
    printf("could not get bool value for variable name \"%s\", variableName \%s\" does not exist in bool value storage!\n",varName.c_str(),varName.c_str());
    exit(1);
}

intValue inputVariablesHandler::get_intValue(std::string varName)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to get the value of interest
    intValue foundValue = *((intValue*)variableTypeClass);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;
    // return the found value
    return foundValue;

    // if it got here, the variable name doesn't exist in storage
    printf("could not get bool value for variable name \"%s\", variableName \%s\" does not exist in bool value storage!\n",varName.c_str(),varName.c_str());
    exit(1);
}

doubleValue inputVariablesHandler::get_doubleValue(std::string varName)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to get the value of interest
    doubleValue foundValue = *((doubleValue*)variableTypeClass);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;
    // return the found value
    return foundValue;

    // if it got here, the variable name doesn't exist in storage
    printf("could not get bool value for variable name \"%s\", variableName \%s\" does not exist in bool value storage!\n",varName.c_str(),varName.c_str());
    exit(1);
}

stringValue inputVariablesHandler::get_stringValue(std::string varName)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to get the value of interest
    stringValue foundValue = *((stringValue*)variableTypeClass);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;
    // return the found value
    return foundValue;

    // if it got here, the variable name doesn't exist in storage
    printf("could not get bool value for variable name \"%s\", variableName \%s\" does not exist in bool value storage!\n",varName.c_str(),varName.c_str());
    exit(1);
}

pathNameValue inputVariablesHandler::get_pathNameValue(std::string varName)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to get the value of interest
    pathNameValue foundValue = *((pathNameValue*)variableTypeClass);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;
    // return the found value
    return foundValue;

    // if it got here, the variable name doesn't exist in storage
    printf("could not get bool value for variable name \"%s\", variableName \%s\" does not exist in bool value storage!\n",varName.c_str(),varName.c_str());
    exit(1);
}

lcpFileValue inputVariablesHandler::get_lcpFileValue(std::string varName)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to get the value of interest
    lcpFileValue foundValue = *((lcpFileValue*)variableTypeClass);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;
    // return the found value
    return foundValue;

    // if it got here, the variable name doesn't exist in storage
    printf("could not get bool value for variable name \"%s\", variableName \%s\" does not exist in bool value storage!\n",varName.c_str(),varName.c_str());
    exit(1);
}

shapeFileValue inputVariablesHandler::get_shapeFileValue(std::string varName)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to get the value of interest
    shapeFileValue foundValue = *((shapeFileValue*)variableTypeClass);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;
    // return the found value
    return foundValue;

    // if it got here, the variable name doesn't exist in storage
    printf("could not get bool value for variable name \"%s\", variableName \%s\" does not exist in bool value storage!\n",varName.c_str(),varName.c_str());
    exit(1);
}

wrfFileValue inputVariablesHandler::get_wrfFileValue(std::string varName)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to get the value of interest
    wrfFileValue foundValue = *((wrfFileValue*)variableTypeClass);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;
    // return the found value
    return foundValue;

    // if it got here, the variable name doesn't exist in storage
    printf("could not get bool value for variable name \"%s\", variableName \%s\" does not exist in bool value storage!\n",varName.c_str(),varName.c_str());
    exit(1);
}

lat_longValue inputVariablesHandler::get_lat_longValue(std::string varName)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to get the value of interest
    lat_longValue foundValue = *((lat_longValue*)variableTypeClass);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;
    // return the found value
    return foundValue;

    // if it got here, the variable name doesn't exist in storage
    printf("could not get bool value for variable name \"%s\", variableName \%s\" does not exist in bool value storage!\n",varName.c_str(),varName.c_str());
    exit(1);
}

dateValue inputVariablesHandler::get_dateValue(std::string varName)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to get the value of interest
    dateValue foundValue = *((dateValue*)variableTypeClass);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;
    // return the found value
    return foundValue;

    // if it got here, the variable name doesn't exist in storage
    printf("could not get bool value for variable name \"%s\", variableName \%s\" does not exist in bool value storage!\n",varName.c_str(),varName.c_str());
    exit(1);
}

hour_minValue inputVariablesHandler::get_hour_minValue(std::string varName)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to get the value of interest
    hour_minValue foundValue = *((hour_minValue*)variableTypeClass);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;
    // return the found value
    return foundValue;

    // if it got here, the variable name doesn't exist in storage
    printf("could not get bool value for variable name \"%s\", variableName \%s\" does not exist in bool value storage!\n",varName.c_str(),varName.c_str());
    exit(1);
}
/*** end get variable value functions for single values ***/

/*** get variable value functions for count values ***/
createIgnitionFromLatLongsStorage inputVariablesHandler::get_createIgnitionFromLatLongsStorage()
{
    return stored_create_ignition_from_latlongs;
}

polygonIgnitShapeFileStorage inputVariablesHandler::get_polygonIgnitShapeFileStorage()
{
    return stored_polygon_ignit_shape_files;
}

GeoMACfirePerimeterFileStorage inputVariablesHandler::get_GeoMACfirePerimeterFileStorage()
{
    return stored_GeoMAC_fire_perimeter_files;
}

farsiteOutputFirePerimeterFileStorage inputVariablesHandler::get_farsiteOutputFirePerimeterFileStorage()
{
    return stored_farsite_output_fire_perimeter_files;
}

wrfFileStorage inputVariablesHandler::get_wrfFileStorage()
{
    return stored_wrf_files;
}

additionalWindNinjaOutputs_googleStorage inputVariablesHandler::get_additionalWindNinjaOutputs_googleStorage()
{
    return stored_additional_WindNinja_Outputs_google;
}

additionalWindNinjaOutputs_shapefileStorage inputVariablesHandler::get_additionalWindNinjaOutputs_shapefileStorage()
{
    return stored_additional_WindNinja_Outputs_shapefile;
}

additionalWindNinjaOutputs_pdfStorage inputVariablesHandler::get_additionalWindNinjaOutputs_pdfStorage()
{
    return stored_additional_WindNinja_Outputs_pdf;
}
/*** end get variable value functions for count values ***/



/***** private functions *****/



/*** other functions ***/
bool inputVariablesHandler::reset()
{
    printf("resetting inputs\n");

    bool success = true;

    actualCreateInputs_path = "";
    actualFinalOutput_path = "";
    actual_run_base_name = "";
    actualLcpPath = "";

    // reset values for all single count types to default values (keep the stuff with string names from setup)
    for(size_t varIdx = 0; varIdx < inputVariableStorage.size(); varIdx++)
    {
        inputVariableStorage[varIdx].resetVariable();
        // need to do reset variable of the values too!!
        if(resetSingleVar(inputVariableStorage[varIdx]) == false)
        {
            success = false;
        }
    }


    // now all the count specific variable resetting
    stored_create_ignition_from_latlongs.reset();
    stored_polygon_ignit_shape_files.reset();
    stored_GeoMAC_fire_perimeter_files.reset();
    stored_farsite_output_fire_perimeter_files.reset();
    stored_wrf_files.reset();
    stored_additional_WindNinja_Outputs_google.reset();
    stored_additional_WindNinja_Outputs_shapefile.reset();
    stored_additional_WindNinja_Outputs_pdf.reset();

    if(setSpecializedDefaults() == false)
    {
        printf("problem setting up specialized default values!\n");
        success = false;
    }

    printf("finished resetting inputs\n");

    return success;
}

bool inputVariablesHandler::resetSingleVar(singleInputVariable &inputVar)
{
    printf("resetting variable value for variable type \"%s\" variable name \"%s\"\n",inputVar.get_variableCountType().c_str(),inputVar.get_variableName().c_str());
    bool success = true;

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVar.get_variableTypeClass();

    if(inputVar.get_variableCountType() == "bool")
    {
        // use the pointer to set the value of interest
        ((boolValue*)variableTypeClass)->reset();
    } else if(inputVar.get_variableCountType() == "size_t")
    {
        // use the pointer to set the value of interest
        ((size_tValue*)variableTypeClass)->reset();
    } else if(inputVar.get_variableCountType() == "int")
    {
        // use the pointer to set the value of interest
        ((intValue*)variableTypeClass)->reset();
    } else if(inputVar.get_variableCountType() == "double")
    {
        // use the pointer to set the value of interest
        ((doubleValue*)variableTypeClass)->reset();
    } else if(inputVar.get_variableCountType() == "positive double")
    {
        // use the pointer to set the value of interest
        ((doubleValue*)variableTypeClass)->reset();
    } else if(inputVar.get_variableCountType() == "signless percent")
    {
        // use the pointer to set the value of interest
        ((doubleValue*)variableTypeClass)->reset();
    } else if(inputVar.get_variableCountType() == "string")
    {
        // use the pointer to set the value of interest
        ((stringValue*)variableTypeClass)->reset();
    } else if(inputVar.get_variableCountType() == "pathname")
    {
        // use the pointer to set the value of interest
        ((pathNameValue*)variableTypeClass)->reset();
    } else if(inputVar.get_variableCountType() == "lcp filename")
    {
        // use the pointer to set the value of interest
        ((lcpFileValue*)variableTypeClass)->reset();
    } else if(inputVar.get_variableCountType() == "shape filename")
    {
        // use the pointer to set the value of interest
        ((shapeFileValue*)variableTypeClass)->reset();
    } else if(inputVar.get_variableCountType() == "lat_coord")
    {
        // use the pointer to set the value of interest
        ((doubleValue*)variableTypeClass)->reset();
    } else if(inputVar.get_variableCountType() == "long_coord")
    {
        // use the pointer to set the value of interest
        ((doubleValue*)variableTypeClass)->reset();
    } else if(inputVar.get_variableCountType() == "lat_long_point")
    {
        // use the pointer to set the value of interest
        ((lat_longValue*)variableTypeClass)->reset();
    } else if(inputVar.get_variableCountType() == "date")
    {
        // use the pointer to set the value of interest
        ((dateValue*)variableTypeClass)->reset();
    } else if(inputVar.get_variableCountType() == "hour_min")
    {
        // use the pointer to set the value of interest
        ((hour_minValue*)variableTypeClass)->reset();
    } else if(inputVar.get_variableCountType() == "count")
    {
        // use the pointer to set the value of interest
        ((size_tValue*)variableTypeClass)->reset();
    } else
    {
        printf("count type \"%s\" for variable \"%s\" has not been implemented in the code yet!\n",inputVar.get_variableCountType().c_str(),inputVar.get_variableName().c_str());
        success = false;
    }

    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;

    return success;
}

bool inputVariablesHandler::setSpecializedDefaults()
{
    printf("setting specialized defaults\n");

    bool success = true;

    // basically if they specify the variable, it has to be of a specific type of value. If they do not specify it, the default value needs given here in whatever way possible
    // so this should run slow because it runs the inputVariablesHandler::findVariableIdx() function multiple times


        // application specific variables
    if(set_boolValue("overwrite_past_outputs",false) == false)
    {
        success = false;
    }
    if(set_stringValue("createIgnition_output_units","input") == false)
    {
        success = false;
    }
    if(set_stringValue("wrfGetWeather_output_units","metric") == false)
    {
        success = false;
    }
    if(set_stringValue("WindNinja_required_output_units","metric") == false)
    {
        success = false;
    }
    if(set_stringValue("farsite_output_units","english") == false)
    {
        success = false;
    }
    if(set_boolValue("use_native_timezone",false) == false)
    {
        success = false;
    }

        // lcp download variables (WindNinja related)
    if(set_boolValue("automate_lcp_download",true) == false)
    {
        success = false;
    }
    if(set_doubleValue("fireperim_to_lcp_scalefactor",1.0,"regular") == false)
    {
        success = false;
    }
    if(set_boolValue("use_past_lcp",false) == false)
    {
        success = false;
    }
    if(set_boolValue("specify_lcp_download",false) == false)
    {
        success = false;
    }

        // createIgnition variables
    if(set_doubleValue("fire_perimeter_widening_factor",1.0,"regular") == false)
    {
        success = false;
    }

        // wrfGetWeather and WindNinja variables
    if(set_boolValue("extend_wrf_data",false) == false)
    {
        success = false;
    }

        // WindNinja only variables
    if(set_size_tValue("WindNinja_number_of_threads",8) == false)
    {
        success = false;
    }
    if(set_stringValue("WindNinja_mesh_choice","fine") == false)
    {
        success = false;
    }
    if(set_stringValue("WindNinja_mesh_res_units","m") == false)
    {
        success = false;
    }
    if(set_boolValue("diurnal_winds",false) == false)
    {
        success = false;
    }
    if(set_boolValue("non_neutral_stability",false) == false)
    {
        success = false;
    }

        // wrfGetWeather only variables
    if(set_boolValue("use_weather_from_ignition_center",false) == false)
    {
        success = false;
    }
    if(set_boolValue("use_weather_from_full_ignition_area",false) == false)
    {
        success = false;
    }
    if(set_boolValue("use_weather_from_wrf_center",true) == false)
    {
        success = false;
    }

        // farsiteAPI variables
    if(set_doubleValue("farsite_spot_probability",0.05,"signless percent") == false)   // need to be careful that it is a legit signless percent
    {
        success = false;
    }
    if(set_size_tValue("farsite_spot_ignition_delay",0) == false)
    {
        success = false;
    }
    if(set_size_tValue("farsite_spotting_seed",1000) == false)
    {
        success = false;
    }
    if(set_hour_minValue("farsite_earliest_burn_time",8,00) == false)
    {
        success = false;
    }
    if(set_hour_minValue("farsite_latest_burn_time",19,59) == false)  // need to be careful that this is later than "farsite_earliest_burn_hour"
    {
        success = false;
    }
    if(set_doubleValue("farsite_foliar_moisture_content",70.0,"signless percent") == false)   // need to be careful that it is a legit signless percent
    {
        success = false;
    }
    if(set_stringValue("farsite_crown_fire_method","Finney") == false)   // need to be careful that it is a legit signless percent
    {
        success = false;
    }

        // optional WindNinja output settings, may not even add these options
    // thank goodness, none here

    printf("finished setting specialized defaults\n");

    return success;
}

// I think super slow cause have to go through every single variable multiple times
bool inputVariablesHandler::verifyFoundInputCombinations()
{
    bool InputCombinationSuccess = true;

    // setup whatever you need to set if input variables are optional or not, or how they are optional or required
    // set_wantDefaultValue
    // I would keep the order of these checkers as close to the input variable list order as possible to keep it hopefully less confusing

        // application specific variables
    checkUsage_optionalValue("run_base_name");
    checkUsage_requiredValue("createInputs_path",InputCombinationSuccess);
    checkUsage_requiredValue("finalOutput_path",InputCombinationSuccess);
    checkUsage_defaultValue("overwrite_past_outputs");
    checkUsage_defaultValue("createIgnition_output_units");
    checkUsage_defaultValue("WindNinja_required_output_units");
    checkUsage_defaultValue("wrfGetWeather_output_units");
    checkUsage_defaultValue("farsite_output_units");
    checkUsage_defaultValue("use_native_timezone");

        // lcp download variables (WindNinja related)
    checkUsage_chooseOnlyOneValueOrDefaultValue("automate_lcp_download",{"use_past_lcp","specify_lcp_download"},InputCombinationSuccess);
    checkUsage_defaultValue("fireperim_to_lcp_scalefactor");
    checkUsage_requiredIfCertainValueSet("use_past_lcp",{"lcp_file_path"},InputCombinationSuccess);
    checkUsage_chooseOnlyOneIfCertainValueSet("specify_lcp_download",{"use_point_lcp_download","use_bounds_lcp_download"},InputCombinationSuccess);
    checkUsage_requiredIfCertainValueSet("use_point_lcp_download",{"lcp_download_lat_long_point","lcp_download_northsouth_buffer",
                                                                   "lcp_download_westeast_buffer","lcp_download_buffer_units"},InputCombinationSuccess);
    checkUsage_requiredIfCertainValueSet("use_bounds_lcp_download",{"lcp_download_north_lat_bound","lcp_download_south_lat_bound",
                                                                    "lcp_download_east_long_bound","lcp_download_west_long_bound"},InputCombinationSuccess);

        // createIgnition variables
    checkUsage_chooseAtLeastOneValue({"create_ignition_from_latlongs","polygon_ignit_shape_files","GeoMAC_fire_perimeter_files","farsite_output_fire_perimeter_files"},InputCombinationSuccess);
    checkUsage_defaultValue("fire_perimeter_widening_factor");

        // wrfGetWeather and WindNinja variables
    checkUsage_defaultValue("extend_wrf_data");
    checkUsage_requiredValue("wrf_files",InputCombinationSuccess);

        // WindNinja only variables
    checkUsage_defaultValue("WindNinja_number_of_threads");
    checkUsage_chooseOnlyOneValueOrDefaultValue("WindNinja_mesh_choice",{"WindNinja_mesh_resolution"},InputCombinationSuccess);
    checkUsage_requiredIfCertainValueSet("WindNinja_mesh_resolution",{"WindNinja_mesh_res_units"},InputCombinationSuccess);
    checkUsage_defaultValue("diurnal_winds");
    checkUsage_defaultValue("non_neutral_stability");

        // wrfGetWeather only variables
    checkUsage_chooseOnlyOneValue({"use_weather_from_ignition_center","use_weather_from_full_ignition_area","use_weather_from_wrf_center"},InputCombinationSuccess);

        // farsiteAPI variables
    checkUsage_requiredValue("burn_start_time",InputCombinationSuccess);
    checkUsage_requiredValue("burn_end_time",InputCombinationSuccess);
    checkUsage_defaultValue("farsite_barrier_shapefile");
    checkUsage_defaultValue("farsite_spot_probability");
    checkUsage_defaultValue("farsite_spot_ignition_delay");
    checkUsage_defaultValue("farsite_spotting_seed");
    checkUsage_defaultValue("farsite_earliest_burn_time");
    checkUsage_defaultValue("farsite_latest_burn_time");
    checkUsage_defaultValue("farsite_foliar_moisture_content");
    checkUsage_defaultValue("farsite_crown_fire_method");
    checkUsage_defaultValue("farsite_spotting_seed");

        // optional WindNinja output settings, may not even add these options
    checkUsage_optionalValue("additional_WindNinja_outputs_google");
    checkUsage_optionalValue("additional_WindNinja_outputs_shapefile");
    checkUsage_optionalValue("additional_WindNinja_outputs_pdf");

    return InputCombinationSuccess;
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

        if(doesDirExist(currentCreateInputsPath) == false && doesDirExist(currentFinalOutputPath) == false)
        {
            actualCreateInputs_path = foundCreateInputs_path;
            actualFinalOutput_path = foundFinalOutput_path;
        } else
        {
            size_t fileIdxCount = 0;
            while(doesDirExist(currentCreateInputsPath) == true || doesDirExist(currentFinalOutputPath) == true)
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

/*** input combination checking functions ***/
size_t inputVariablesHandler::findVarInfoIdx(std::string varName)
{
    bool foundVar = false;
    size_t infoIdx = 0;

    for(size_t varIdx = 0; varIdx < inputVariableStorage.size(); varIdx++)
    {
        std::string currentVarName = inputVariableStorage[varIdx].get_variableName();
        if(currentVarName == varName)
        {
            infoIdx = varIdx;
            foundVar = true;
            break;
        }
    }

    if(foundVar == false)
    {
        printf("Couldn't find \"%s\" varName in stored var info! Exiting program!\n",varName.c_str());
        exit(1);
    }

    return infoIdx;
}

void inputVariablesHandler::checkUsage_optionalValue(std::string varName)
{
    // is an optional value, so don't want to do anything, but makes for a nice place holder so everything has a nice format
}

void inputVariablesHandler::checkUsage_defaultValue(std::string varName)
{
    size_t infoIdx = findVarInfoIdx(varName);
    if(inputVariableStorage[infoIdx].get_isFoundInInputFile() == false)
    {
        inputVariableStorage[infoIdx].set_wantDefaultValue(true);  // means user doesn't need to specify this, default value was used
        //printf("\"%s\" variable not input, was set to default value\n",varName.c_str());
    }
}

void inputVariablesHandler::checkUsage_requiredValue(std::string varName, bool &InputCombinationSuccess)
{
    size_t infoIdx = findVarInfoIdx(varName);
    if(inputVariableStorage[infoIdx].get_isFoundInInputFile() == false)
    {
        printf("\"%s\" variable is required but is not set in the input file!\n",varName.c_str());
        InputCombinationSuccess = false;    // only thing it can become is false, never turned back to true
    }
    // don't want a default value for this
}

void inputVariablesHandler::checkUsage_chooseOnlyOneValue(std::vector<std::string> varNames, bool &InputCombinationSuccess)
{
    size_t nVars = varNames.size();
    if(nVars == 0)
    {
        printf("error checking variable input usage! varNames has no values! Exiting program!\n");
        exit(1);
    }
    std::vector<bool> isSet(nVars,false);
    std::vector<size_t> infoIdx(nVars,0);
    size_t isSetCounter = 0;
    for(size_t varIdx = 0; varIdx < nVars; varIdx++)
    {
        infoIdx[varIdx] = findVarInfoIdx(varNames[varIdx]);
        if(inputVariableStorage[infoIdx[varIdx]].get_isFoundInInputFile() == true)
        {
            isSet[varIdx] = true;
            isSetCounter = isSetCounter + 1;
        }
    }

    if(isSetCounter == 0)
    {
        printf("Need exactly one of the variables ");
        for(size_t varIdx = 0; varIdx < nVars; varIdx++)
        {
            printf("\"%s\" ",varNames[varIdx].c_str());
        }
        printf("specified in the input file, but none of these variables are specified!\n");
        InputCombinationSuccess = false;    // only thing it can become is false, never turned back to true
    } else if(isSetCounter > 1)
    {
        printf("Need exactly one of the variables ");
        for(size_t varIdx = 0; varIdx < nVars; varIdx++)
        {
            printf("\"%s\" ",varNames[varIdx].c_str());
        }
        printf("specified in the input file, but ");
        for(size_t varIdx = 0; varIdx < nVars; varIdx++)
        {
            if(isSet[varIdx] == true)
            {
                printf("\"%s\" ",varNames[varIdx].c_str());
            }
        }
        printf("are each set!\n");
        InputCombinationSuccess = false;    // only thing it can become is false, never turned back to true
    }
    // don't want a default value for any of these
}

void inputVariablesHandler::checkUsage_chooseAtLeastOneValue(std::vector<std::string> varNames, bool &InputCombinationSuccess)
{
    size_t nVars = varNames.size();
    if(nVars == 0)
    {
        printf("error checking variable input usage! varNames has no values! Exiting program!\n");
        exit(1);
    }
    size_t isSetCounter = 0;
    for(size_t varIdx = 0; varIdx < nVars; varIdx++)
    {
        size_t infoIdx = findVarInfoIdx(varNames[varIdx]);
        if(inputVariableStorage[infoIdx].get_isFoundInInputFile() == true)
        {
            isSetCounter = isSetCounter + 1;
        }
    }

    if(isSetCounter == 0)
    {
        printf("Need at least one of the variables ");
        for(size_t varIdx = 0; varIdx < nVars; varIdx++)
        {
            printf("\"%s\" ",varNames[varIdx].c_str());
        }
        printf("specified in the input file, but none of these variables are specified!\n");
        InputCombinationSuccess = false;    // only thing it can become is false, never turned back to true
    }
    // don't want a default value for any of these
}

void inputVariablesHandler::checkUsage_chooseOnlyOneValueOrDefaultValue(std::string defaultVarName, std::vector<std::string> otherVarNames, bool &InputCombinationSuccess)
{
    size_t defaultVarIdx = findVarInfoIdx(defaultVarName);
    size_t nOtherVars = otherVarNames.size();
    if(nOtherVars == 0)
    {
        printf("error checking variable input usage! otherVarNames has no values! Exiting program!\n");
        exit(1);
    } else
    {
        for(size_t varIdx = 0; varIdx < nOtherVars; varIdx++)
        {
            if(defaultVarName == otherVarNames[varIdx])
            {
                printf("error checking variable input usage! defaultVarName should not be in otherVarName values! Exiting program!\n");
                exit(1);
            }
        }
    }

    std::vector<bool> isSet(nOtherVars,false);
    std::vector<size_t> otherVarIdx(nOtherVars,0);
    size_t isSetCounter = 0;
    if(inputVariableStorage[defaultVarIdx].get_isFoundInInputFile() == true)
    {
        isSetCounter = isSetCounter + 1;
    }
    for(size_t varIdx = 0; varIdx < nOtherVars; varIdx++)
    {
        otherVarIdx[varIdx] = findVarInfoIdx(otherVarNames[varIdx]);
        if(inputVariableStorage[otherVarIdx[varIdx]].get_isFoundInInputFile() == true)
        {
            isSet[varIdx] = true;
            isSetCounter = isSetCounter + 1;
        }
    }

    if(isSetCounter == 0)
    {
        inputVariableStorage[defaultVarIdx].set_wantDefaultValue(true);  // means user doesn't need to specify this, default value was used
    } else if(isSetCounter > 1)
    {
        printf("Need exactly one of the variables \"%s\" ",defaultVarName.c_str());
        for(size_t varIdx = 0; varIdx < nOtherVars; varIdx++)
        {
            printf("\"%s\" ",otherVarNames[varIdx].c_str());
        }
        printf("specified in the input file or none of them, but ");
        for(size_t varIdx = 0; varIdx < nOtherVars; varIdx++)
        {
            if(isSet[varIdx] == true)
            {
                printf("\"%s\" ",otherVarNames[varIdx].c_str());
            }
        }
        if(inputVariableStorage[defaultVarIdx].get_isFoundInInputFile() == true)
        {
            printf("\"%s\" ",defaultVarName.c_str());
        }
        printf("are each set!\n");
        InputCombinationSuccess = false;    // only thing it can become is false, never turned back to true
    }
    // don't want a default value for any of these
}

void inputVariablesHandler::checkUsage_requiredIfCertainValueSet(std::string ifSetVarName, std::vector<std::string> checkVarNames, bool &InputCombinationSuccess)
{
    size_t ifSetVarNameIdx = findVarInfoIdx(ifSetVarName);
    size_t nCheckVars = checkVarNames.size();
    if(nCheckVars == 0)
    {
        printf("error checking variable input usage! varNames has no values! Exiting program!\n");
        exit(1);
    }

    if(inputVariableStorage[ifSetVarNameIdx].get_isFoundInInputFile() == false)
    {
        // expects want default value to be set by something else for isSetVarName
        for(size_t varIdx = 0; varIdx < nCheckVars; varIdx++)
        {
            size_t checkVarIdx = findVarInfoIdx(checkVarNames[varIdx]);
            inputVariableStorage[checkVarIdx].set_wantDefaultValue(true);  // means user doesn't need to specify this, default value was used
            if(inputVariableStorage[checkVarIdx].get_isFoundInInputFile() == true)
            {
                printf("\"%s\" not set so \"%s\" should not be set, but \"%s\" is set!\n",ifSetVarName.c_str(),checkVarNames[varIdx].c_str(),checkVarNames[varIdx].c_str());
                InputCombinationSuccess = false;    // only thing it can become is false, never turned back to true
            }
        }
    } else
    {
        for(size_t varIdx = 0; varIdx < nCheckVars; varIdx++)
        {
            size_t checkVarIdx = findVarInfoIdx(checkVarNames[varIdx]);
            if(inputVariableStorage[checkVarIdx].get_isFoundInInputFile() == false)
            {
                printf("\"%s\" set so \"%s\" should be set, but \"%s\" is not set!\n",ifSetVarName.c_str(),checkVarNames[varIdx].c_str(),checkVarNames[varIdx].c_str());
                InputCombinationSuccess = false;    // only thing it can become is false, never turned back to true
            }
            // don't want a default value for this
        }
    }
}

void inputVariablesHandler::checkUsage_chooseOnlyOneIfCertainValueSet(std::string ifSetVarName, std::vector<std::string> checkVarNames, bool &InputCombinationSuccess)
{
    size_t ifSetVarNameIdx = findVarInfoIdx(ifSetVarName);
    size_t nCheckVars = checkVarNames.size();
    if(nCheckVars == 0)
    {
        printf("error checking variable input usage! varNames has no values! Exiting program!\n");
        exit(1);
    }

    if(inputVariableStorage[ifSetVarNameIdx].get_isFoundInInputFile() == false)
    {
        // expects want default value to be set by something else for isSetVarName
        for(size_t varIdx = 0; varIdx < nCheckVars; varIdx++)
        {
            size_t checkVarIdx = findVarInfoIdx(checkVarNames[varIdx]);
            inputVariableStorage[checkVarIdx].set_wantDefaultValue(true);  // means user doesn't need to specify this, default value was used
            if(inputVariableStorage[checkVarIdx].get_isFoundInInputFile() == true)
            {
                printf("\"%s\" not set so \"%s\" should not be set, but \"%s\" is set!\n",ifSetVarName.c_str(),checkVarNames[varIdx].c_str(),checkVarNames[varIdx].c_str());
                InputCombinationSuccess = false;    // only thing it can become is false, never turned back to true
            }
        }
    } else
    {
        std::vector<bool> isSet(nCheckVars,false);
        std::vector<size_t> checkVarIdx(nCheckVars,0);
        size_t isSetCounter = 0;
        for(size_t varIdx = 0; varIdx < nCheckVars; varIdx++)
        {
            checkVarIdx[varIdx] = findVarInfoIdx(checkVarNames[varIdx]);
            if(inputVariableStorage[checkVarIdx[varIdx]].get_isFoundInInputFile() == true)
            {
                isSet[varIdx] = true;
                isSetCounter = isSetCounter + 1;
            }
        }

        if(isSetCounter == 0)
        {
            printf("\"%s\" set so need exactly one of the variables ",ifSetVarName.c_str());
            for(size_t varIdx = 0; varIdx < nCheckVars; varIdx++)
            {
                printf("\"%s\" ",checkVarNames[varIdx].c_str());
            }
            printf("specified in the input file, but none of these are specified!\n");
            InputCombinationSuccess = false;    // only thing it can become is false, never turned back to true
        } else if(isSetCounter > 1)
        {
            printf("\"%s\" set so need exactly one of the variables ",ifSetVarName.c_str());
            for(size_t varIdx = 0; varIdx < nCheckVars; varIdx++)
            {
                printf("\"%s\" ",checkVarNames[varIdx].c_str());
            }
            printf("specified in the input file, but ");
            for(size_t varIdx = 0; varIdx < nCheckVars; varIdx++)
            {
                if(isSet[varIdx] == true)
                {
                    printf("\"%s\" ",checkVarNames[varIdx].c_str());
                }
            }
            printf("are each set!\n");
            InputCombinationSuccess = false;    // only thing it can become is false, never turned back to true
        }
        // don't want a default value for any of these
    }
}
/*** end input combination checking functions ***/

/*** set variable value functions ***/
bool inputVariablesHandler::set_boolValue(std::string varName, bool newBoolValue)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to set the value of interest
    ((boolValue*)variableTypeClass)->set_storedBoolValue(newBoolValue);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;

    // if it got here, no error occurred
    return 0;
}

bool inputVariablesHandler::set_size_tValue(std::string varName, size_t newSize_tValue)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to set the value of interest
    ((size_tValue*)variableTypeClass)->set_storedSize_tValue(newSize_tValue);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;

    // if it got here, no error occurred
    return 0;
}

bool inputVariablesHandler::set_intValue(std::string varName, int newIntValue)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to set the value of interest
    ((intValue*)variableTypeClass)->set_storedIntValue(newIntValue);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;

    // if it got here, no error occurred
    return 0;
}

bool inputVariablesHandler::set_doubleValue(std::string varName, double newDoubleValue, std::string doubleType)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to set the value of interest
    ((doubleValue*)variableTypeClass)->set_storedDoubleValue(newDoubleValue,doubleType);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;

    // if it got here, no error occurred
    return 0;
}

bool inputVariablesHandler::set_stringValue(std::string varName, std::string newStringValue)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to set the value of interest
    ((stringValue*)variableTypeClass)->set_storedStringValue(newStringValue,varName);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;

    // if it got here, no error occurred
    return 0;
}

bool inputVariablesHandler::set_pathNameValue(std::string varName, std::string newPathNameValue)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to set the value of interest
    ((pathNameValue*)variableTypeClass)->set_storedPathNameValue(newPathNameValue);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;

    // if it got here, no error occurred
    return 0;
}

bool inputVariablesHandler::set_lcpFileValue(std::string varName, std::string newLcpFileValue)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to set the value of interest
    ((lcpFileValue*)variableTypeClass)->set_storedLcpFileValue(newLcpFileValue);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;

    // if it got here, no error occurred
    return 0;
}

bool inputVariablesHandler::set_shapeFileValue(std::string varName, std::string newShapeFileValue)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to set the value of interest
    ((shapeFileValue*)variableTypeClass)->set_storedShapeFileValue(newShapeFileValue);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;

    // if it got here, no error occurred
    return 0;
}

bool inputVariablesHandler::set_wrfFileValue(std::string varName, std::string newWrfFileValue)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to set the value of interest
    ((wrfFileValue*)variableTypeClass)->set_storedWrfFileName(newWrfFileValue);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;

    // if it got here, no error occurred
    return 0;
}

bool inputVariablesHandler::set_lat_longValue(std::string varName, double newLatValue, double newLongValue)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to set the value of interest
    ((lat_longValue*)variableTypeClass)->set_storedLatLongValue(newLatValue,newLongValue);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;

    // if it got here, no error occurred
    return 0;
}

bool inputVariablesHandler::set_dateValue(std::string varName, int newYearValue, int newMonthValue, int newDayValue, int newHourValue, int newMinuteValue)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to set the value of interest
    ((dateValue*)variableTypeClass)->set_storedDateValue(newYearValue,newMonthValue,newDayValue,newHourValue,newMinuteValue);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;

    // if it got here, no error occurred
    return 0;
}

bool inputVariablesHandler::set_hour_minValue(std::string varName, int newHourValue, int newMinuteValue)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to set the value of interest
    ((hour_minValue*)variableTypeClass)->set_storedHour_MinValue(newHourValue,newMinuteValue);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;

    // if it got here, no error occurred
    return 0;
}
/*** end set variable value functions ***/

/*** load function stuff for count type variables ***/
bool inputVariablesHandler::add_createIgnitionLatLongValue(double newLat_CoordValue, double newLong_CoordValue)
{
    return stored_create_ignition_from_latlongs.add_createIgnitionLatLongValue(newLat_CoordValue,newLong_CoordValue);
}

bool inputVariablesHandler::add_polygonIgnitShapeFile(std::string new_polygon_ignit_shape_file)
{
    return stored_polygon_ignit_shape_files.add_polygonIgnitShapeFile(new_polygon_ignit_shape_file);
}

bool inputVariablesHandler::add_GeoMACfirePerimeterFile(std::string new_GeoMAC_fire_perimeter_file)
{
    return stored_GeoMAC_fire_perimeter_files.add_GeoMACfirePerimeterFile(new_GeoMAC_fire_perimeter_file);
}

bool inputVariablesHandler::add_farsiteOutputFirePerimeterFile(std::string new_farsite_output_fire_perimeter_file)
{
    return stored_farsite_output_fire_perimeter_files.add_farsiteOutputFirePerimeterFile(new_farsite_output_fire_perimeter_file);
}

bool inputVariablesHandler::add_wrfFile(std::string new_wrf_file)
{
    return stored_wrf_files.add_wrfFile(new_wrf_file);
}

bool inputVariablesHandler::add_additionalWindNinjaOutputs_googleValueSet(std::string new_wrf_file_name, bool new_write_wx_model_goog_output, bool new_write_goog_output, double new_goog_out_resolution,
                                                   std::string new_units_goog_out_resolution, std::string new_goog_out_color_scheme, bool new_goog_out_vector_scaling)
{
    return stored_additional_WindNinja_Outputs_google.add_additionalWindNinjaOutputs_googleValueSet(new_wrf_file_name,new_write_wx_model_goog_output,new_write_goog_output,
                                                                                             new_goog_out_resolution,new_units_goog_out_resolution,new_goog_out_color_scheme,
                                                                                             new_goog_out_vector_scaling);
}

bool inputVariablesHandler::add_additionalWindNinjaOutputs_shapefileValueSet(std::string new_wrf_file_name, bool new_write_wx_model_shapefile_output, bool new_write_shapefile_output,
                                                      double new_shape_out_resolution, std::string new_units_shape_out_resolution)
{
    return stored_additional_WindNinja_Outputs_shapefile.add_additionalWindNinjaOutputs_shapefileValueSet(new_wrf_file_name,new_write_wx_model_shapefile_output,
                                                                                                   new_write_shapefile_output,new_shape_out_resolution,new_units_shape_out_resolution);
}

bool inputVariablesHandler::add_additionalWindNinjaOutputs_pdfValueSet(std::string new_wrf_file_name, bool new_write_pdf_output, double new_pdf_out_resolution, std::string new_units_pdf_out_resolution,
                                                    double new_pdf_linewidth, std::string new_pdf_basemap, double new_pdf_height, double new_pdf_width, std::string new_pdf_size)
{
    return stored_additional_WindNinja_Outputs_pdf.add_additionalWindNinjaOutputs_pdfValueSet(new_wrf_file_name,new_write_pdf_output,new_pdf_out_resolution,
                                                                                       new_units_pdf_out_resolution,new_pdf_linewidth,new_pdf_basemap,new_pdf_height,
                                                                                       new_pdf_width,new_pdf_size);
}
/*** end load function stuff for count type variables ***/

