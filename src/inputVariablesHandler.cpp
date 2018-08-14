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
            printf("%s: %d%s\n",currentVarName.c_str(),get_inputVariableBoolValue(currentVarName),isFoundInInputFile.c_str());
        } else if(currentCountType == "size_t")
        {
            printf("%s: %zu%s\n",currentVarName.c_str(),get_inputVariableSize_tValue(currentVarName),isFoundInInputFile.c_str());
        } else if(currentCountType == "int")
        {
            printf("%s: %d%s\n",currentVarName.c_str(),get_inputVariableIntValue(currentVarName),isFoundInInputFile.c_str());
        } else if(currentCountType == "double")
        {
            printf("%s: %f%s\n",currentVarName.c_str(),get_inputVariableDoubleValue(currentVarName),isFoundInInputFile.c_str());
        } else if(currentCountType == "positive double")
        {
            printf("%s: %f%s\n",currentVarName.c_str(),get_inputVariableDoubleValue(currentVarName),isFoundInInputFile.c_str());
        } else if(currentCountType == "signless percent")
        {
            printf("%s: %f%s\n",currentVarName.c_str(),get_inputVariableDoubleValue(currentVarName),isFoundInInputFile.c_str());
        } else if(currentCountType == "string")
        {
            printf("%s: %s%s\n",currentVarName.c_str(),get_inputVariableStringValue(currentVarName).c_str(),isFoundInInputFile.c_str());
        } else if(currentCountType == "pathname")
        {
            printf("%s: %s%s\n",currentVarName.c_str(),get_inputVariablePathnameValue(currentVarName).c_str(),isFoundInInputFile.c_str());
        } else if(currentCountType == "lcp filename")
        {
            printf("%s: %s%s\n",currentVarName.c_str(),get_inputVariableFilenameValue(currentVarName).c_str(),isFoundInInputFile.c_str());
        } else if(currentCountType == "shape filename")
        {
            printf("%s: %s%s\n",currentVarName.c_str(),get_inputVariableFilenameValue(currentVarName).c_str(),isFoundInInputFile.c_str());
        } else if(currentCountType == "lat_coord")
        {
            printf("%s: %f%s\n",currentVarName.c_str(),get_inputVariableDoubleValue(currentVarName),isFoundInInputFile.c_str());
        } else if(currentCountType == "long_coord")
        {
            printf("%s: %f%s\n",currentVarName.c_str(),get_inputVariableDoubleValue(currentVarName),isFoundInInputFile.c_str());
        } else if(currentCountType == "lat_long_point")
        {
            printf("%s: %f %f%s\n",currentVarName.c_str(),get_inputVariableLat_Long_PointValueLat_Coord(currentVarName),get_inputVariableLat_Long_PointValueLong_Coord(currentVarName),isFoundInInputFile.c_str());
        } else if(currentCountType == "date")
        {
            printf("%s: %d %d %d %d:%d%s\n",currentVarName.c_str(),get_inputVariableDateValueMonth(currentVarName),get_inputVariableDateValueDay(currentVarName),get_inputVariableDateValueYear(currentVarName),get_inputVariableDateValueHour(currentVarName),get_inputVariableDateValueMinute(currentVarName),isFoundInInputFile.c_str());
        } else if(currentCountType == "hour_min")
        {
            printf("%s: %d%d%s\n",currentVarName.c_str(),get_inputVariableHour_MinValueHour(currentVarName),get_inputVariableHour_MinValueMinute(currentVarName),isFoundInInputFile.c_str());
        } else if(currentCountType == "count")
        {
            // hm, this function is revealing the complexities and confusingness of even more input stuff
            // do I try to repeat all the rest of the inputs checking that WindNinja and Farsite do?
            std::string currentLoaderFunction = inputVariableInfo[varIdx].get_loaderFunctionName();
            printf("%s: %zu%s\n",currentVarName.c_str(),inputVariableValues.getTypeCount_CountVar(currentVarName,currentLoaderFunction),isFoundInInputFile.c_str());
            if(currentLoaderFunction == "load_create_ignition_from_latlongs")
            {
                for(size_t countVarIdx = 0; countVarIdx < get_create_ignition_from_latlongs().size(); countVarIdx++)
                {
                    printf("%f %f\n",get_create_ignition_from_latlongs()[countVarIdx].lat_coord,get_create_ignition_from_latlongs()[countVarIdx].long_coord);
                }
            } else if(currentLoaderFunction == "load_polygon_ignit_shape_files")
            {
                for(size_t countVarIdx = 0; countVarIdx < get_polygon_ignit_shape_files().size(); countVarIdx++)
                {
                    printf("%s\n",get_polygon_ignit_shape_files()[countVarIdx].c_str());
                }
            } else if(currentLoaderFunction == "load_GeoMAC_fire_perimeter_files")
            {
                for(size_t countVarIdx = 0; countVarIdx < get_GeoMAC_fire_perimeter_files().size(); countVarIdx++)
                {
                    printf("%s\n",get_GeoMAC_fire_perimeter_files()[countVarIdx].c_str());
                }
            } else if(currentLoaderFunction == "load_farsite_output_fire_perimeter_files")
            {
                for(size_t countVarIdx = 0; countVarIdx < get_farsite_output_fire_perimeter_files().size(); countVarIdx++)
                {
                    printf("%s\n",get_farsite_output_fire_perimeter_files()[countVarIdx].c_str());
                }
            } else if(currentLoaderFunction == "load_wrf_files")
            {
                for(size_t countVarIdx = 0; countVarIdx < get_wrf_files().size(); countVarIdx++)
                {
                    printf("%s\n",get_wrf_files()[countVarIdx].c_str());
                }
            } else if(currentLoaderFunction == "load_additional_WindNinja_outputs_google")
            {
                std::vector<std::string> current_wrf_files = get_additional_WindNinja_outputs_googleValues_wrf_file_names();
                for(size_t countVarIdx = 0; countVarIdx < current_wrf_files.size(); countVarIdx++)
                {
                    std::string wrfFile = current_wrf_files[countVarIdx];
                    printf("%s %d %d %f %s %s %d\n",wrfFile.c_str(),get_write_wx_model_goog_output(wrfFile),get_write_goog_output(wrfFile),get_goog_out_resolution(wrfFile),get_units_goog_out_resolution(wrfFile).c_str(),get_goog_out_color_scheme(wrfFile).c_str(),get_goog_out_vector_scaling(wrfFile));
                }
            } else if(currentLoaderFunction == "load_additional_WindNinja_outputs_shapefile")
            {
                std::vector<std::string> current_wrf_files = get_additional_WindNinja_outputs_shapefileValues_wrf_file_names();
                for(size_t countVarIdx = 0; countVarIdx < current_wrf_files.size(); countVarIdx++)
                {
                    std::string wrfFile = current_wrf_files[countVarIdx];
                    printf("%s %d %d %f %s\n",wrfFile.c_str(),get_write_wx_model_shapefile_output(wrfFile),get_write_shapefile_output(wrfFile),get_shape_out_resolution(wrfFile),get_units_shape_out_resolution(wrfFile).c_str());
                }
            } else if(currentLoaderFunction == "load_additional_WindNinja_outputs_pdf")
            {
                std::vector<std::string> current_wrf_files = get_additional_WindNinja_outputs_pdfValues_wrf_file_names();
                for(size_t countVarIdx = 0; countVarIdx < current_wrf_files.size(); countVarIdx++)
                {
                    std::string wrfFile = current_wrf_files[countVarIdx];
                    printf("%s %d %f %s %f %s %f %f %s\n",wrfFile.c_str(),get_write_pdf_output(wrfFile),get_pdf_out_resolution(wrfFile),get_units_pdf_out_resolution(wrfFile).c_str(),get_pdf_linewidth(wrfFile),get_pdf_basemap(wrfFile).c_str(),get_pdf_height(wrfFile),get_pdf_width(wrfFile),get_pdf_size(wrfFile).c_str());
                }
            } else
            {
                printf("loader function \"%s\" for variable \"%s\" has not been implemented in the code yet!\n",currentLoaderFunction.c_str(),currentVarName.c_str());
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
/*** end  functions ***/

/*** get variable value functions for single values ***/
bool inputVariablesHandler::get_inputVariableBoolValue(std::string varName)
{
    return inputVariableValues.get_inputVariableBoolValue(varName);
}

size_t inputVariablesHandler::get_inputVariableSize_tValue(std::string varName)
{
    return inputVariableValues.get_inputVariableSize_tValue(varName);
}

int inputVariablesHandler::get_inputVariableIntValue(std::string varName)
{
    return inputVariableValues.get_inputVariableIntValue(varName);
}

double inputVariablesHandler::get_inputVariableDoubleValue(std::string varName)
{
    return inputVariableValues.get_inputVariableDoubleValue(varName);
}

std::string inputVariablesHandler::get_inputVariableStringValue(std::string varName)
{
    return inputVariableValues.get_inputVariableStringValue(varName);
}

std::string inputVariablesHandler::get_inputVariablePathnameValue(std::string varName)
{
    return inputVariableValues.get_inputVariablePathnameValue(varName);
}

std::string inputVariablesHandler::get_inputVariableFilenameValue(std::string varName)
{
    return inputVariableValues.get_inputVariableFilenameValue(varName);
}

double inputVariablesHandler::get_inputVariableLat_Long_PointValueLat_Coord(std::string varName)
{
    return inputVariableValues.get_inputVariableLat_Long_PointValueLat_Coord(varName);
}

double inputVariablesHandler::get_inputVariableLat_Long_PointValueLong_Coord(std::string varName)
{
    return inputVariableValues.get_inputVariableLat_Long_PointValueLong_Coord(varName);
}

dateValue inputVariablesHandler::get_inputVariableDateValue(std::string varName)
{
    return inputVariableValues.get_inputVariableDateValue(varName);
}

int inputVariablesHandler::get_inputVariableDateValueYear(std::string varName)
{
    return inputVariableValues.get_inputVariableDateValueYear(varName);
}

int inputVariablesHandler::get_inputVariableDateValueMonth(std::string varName)
{
    return inputVariableValues.get_inputVariableDateValueMonth(varName);
}

int inputVariablesHandler::get_inputVariableDateValueDay(std::string varName)
{
    return inputVariableValues.get_inputVariableDateValueDay(varName);
}

int inputVariablesHandler::get_inputVariableDateValueHour(std::string varName)
{
    return inputVariableValues.get_inputVariableDateValueHour(varName);
}

int inputVariablesHandler::get_inputVariableDateValueMinute(std::string varName)
{
    return inputVariableValues.get_inputVariableDateValueMinute(varName);
}

int inputVariablesHandler::get_inputVariableHour_MinValueHour(std::string varName)
{
    return inputVariableValues.get_inputVariableHour_MinValueHour(varName);
}

int inputVariablesHandler::get_inputVariableHour_MinValueMinute(std::string varName)
{
    return inputVariableValues.get_inputVariableHour_MinValueMinute(varName);
}
/*** end get variable value functions for single values ***/

/*** get variable value functions for count values ***/
std::vector<lat_long_pointValue> inputVariablesHandler::get_create_ignition_from_latlongs()
{
    return inputVariableValues.get_create_ignition_from_latlongs();
}

std::vector<std::string> inputVariablesHandler::get_polygon_ignit_shape_files()
{
    return inputVariableValues.get_polygon_ignit_shape_files();
}

std::vector<std::string> inputVariablesHandler::get_GeoMAC_fire_perimeter_files()
{
    return inputVariableValues.get_GeoMAC_fire_perimeter_files();
}

std::vector<std::string> inputVariablesHandler::get_farsite_output_fire_perimeter_files()
{
    return inputVariableValues.get_farsite_output_fire_perimeter_files();
}

std::vector<std::string> inputVariablesHandler::get_wrf_files()
{
    return inputVariableValues.get_wrf_files();
}

/* additional_WindNinja_outputs_google get functions */
std::vector<std::string> inputVariablesHandler::get_additional_WindNinja_outputs_googleValues_wrf_file_names()
{
    return inputVariableValues.get_additional_WindNinja_outputs_googleValues_wrf_file_names();
}

bool inputVariablesHandler::get_write_wx_model_goog_output(std::string wrf_file_name)
{
    return inputVariableValues.get_write_wx_model_goog_output(wrf_file_name);
}

bool inputVariablesHandler::get_write_goog_output(std::string wrf_file_name)
{
    return inputVariableValues.get_write_goog_output(wrf_file_name);
}

double inputVariablesHandler::get_goog_out_resolution(std::string wrf_file_name)
{
    return inputVariableValues.get_goog_out_resolution(wrf_file_name);
}

std::string inputVariablesHandler::get_units_goog_out_resolution(std::string wrf_file_name)
{
    return inputVariableValues.get_units_goog_out_resolution(wrf_file_name);
}

std::string inputVariablesHandler::get_goog_out_color_scheme(std::string wrf_file_name)
{
    return inputVariableValues.get_goog_out_color_scheme(wrf_file_name);
}

bool inputVariablesHandler::get_goog_out_vector_scaling(std::string wrf_file_name)
{
    return inputVariableValues.get_goog_out_vector_scaling(wrf_file_name);
}
/* end additional_WindNinja_outputs_google get functions */

/* additional_WindNinja_outputs_shapefile get functions */
std::vector<std::string> inputVariablesHandler::get_additional_WindNinja_outputs_shapefileValues_wrf_file_names()
{
    return inputVariableValues.get_additional_WindNinja_outputs_shapefileValues_wrf_file_names();
}

bool inputVariablesHandler::get_write_wx_model_shapefile_output(std::string wrf_file_name)
{
    return inputVariableValues.get_write_wx_model_shapefile_output(wrf_file_name);
}

bool inputVariablesHandler::get_write_shapefile_output(std::string wrf_file_name)
{
    return inputVariableValues.get_write_shapefile_output(wrf_file_name);
}

double inputVariablesHandler::get_shape_out_resolution(std::string wrf_file_name)
{
    return inputVariableValues.get_shape_out_resolution(wrf_file_name);
}

std::string inputVariablesHandler::get_units_shape_out_resolution(std::string wrf_file_name)
{
    return inputVariableValues.get_units_shape_out_resolution(wrf_file_name);
}
/* end additional_WindNinja_outputs_shapefile get functions */

/* additional_WindNinja_outputs_pdf get functions */
std::vector<std::string> inputVariablesHandler::get_additional_WindNinja_outputs_pdfValues_wrf_file_names()
{
    return inputVariableValues.get_additional_WindNinja_outputs_pdfValues_wrf_file_names();
}

bool inputVariablesHandler::get_write_pdf_output(std::string wrf_file_name)
{
    return inputVariableValues.get_write_pdf_output(wrf_file_name);
}

double inputVariablesHandler::get_pdf_out_resolution(std::string wrf_file_name)
{
    return inputVariableValues.get_pdf_out_resolution(wrf_file_name);
}

std::string inputVariablesHandler::get_units_pdf_out_resolution(std::string wrf_file_name)
{
    return inputVariableValues.get_units_pdf_out_resolution(wrf_file_name);
}

double inputVariablesHandler::get_pdf_linewidth(std::string wrf_file_name)
{
    return inputVariableValues.get_pdf_linewidth(wrf_file_name);
}

std::string inputVariablesHandler::get_pdf_basemap(std::string wrf_file_name)
{
    return inputVariableValues.get_pdf_basemap(wrf_file_name);
}

double inputVariablesHandler::get_pdf_height(std::string wrf_file_name)
{
    return inputVariableValues.get_pdf_height(wrf_file_name);
}

double inputVariablesHandler::get_pdf_width(std::string wrf_file_name)
{
    return inputVariableValues.get_pdf_width(wrf_file_name);
}

std::string inputVariablesHandler::get_pdf_size(std::string wrf_file_name)
{
    return inputVariableValues.get_pdf_size(wrf_file_name);
}
/* end additional_WindNinja_outputs_pdf get functions */

/*** end get variable value functions for count values ***/



/***** private functions *****/



/*** other functions ***/
bool inputVariablesHandler::reset()
{
    bool success = true;

    actualCreateInputs_path = "";
    actualFinalOutput_path = "";

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

    if(inputVariableValues.get_inputVariableBoolValue("overwrite_past_outputs") == false)
    {
        actualCreateInputs_path = inputVariableValues.get_inputVariablePathnameValue("createInputs_path");
        actualFinalOutput_path = inputVariableValues.get_inputVariablePathnameValue("finalOutput_path");
    } else
    {
        std::string currentCreateInputsPath = inputVariableValues.get_inputVariablePathnameValue("createInputs_path");
        std::string currentFinalOutputPath = inputVariableValues.get_inputVariablePathnameValue("finalOutput_path");

        size_t createInputsCount = 0;
        while(doesFolderExist(currentCreateInputsPath) == true)
        {
            currentCreateInputsPath = std::to_string(createInputsCount) + "_" + inputVariableValues.get_inputVariablePathnameValue("createInputs_path");
            createInputsCount = createInputsCount + 1;
        }

        size_t finalOutputsCount = 0;
        while(doesFolderExist(currentFinalOutputPath) == true)
        {
            currentFinalOutputPath = std::to_string(finalOutputsCount) + "_" + inputVariableValues.get_inputVariablePathnameValue("finalOutput_path");
            finalOutputsCount = finalOutputsCount + 1;
        }

        if(createInputsCount == 0 && finalOutputsCount == 0)
        {
            actualCreateInputs_path = currentCreateInputsPath;
            actualFinalOutput_path = currentFinalOutputPath;
        } else if(createInputsCount >= finalOutputsCount)
        {
            actualCreateInputs_path = std::to_string(createInputsCount) + "_" + inputVariableValues.get_inputVariablePathnameValue("createInputs_path");
            actualFinalOutput_path = std::to_string(createInputsCount) + "_" + inputVariableValues.get_inputVariablePathnameValue("finalOutput_path");
        } else
        {
            actualCreateInputs_path = std::to_string(finalOutputsCount) + "_" + inputVariableValues.get_inputVariablePathnameValue("createInputs_path");
            actualFinalOutput_path = std::to_string(finalOutputsCount) + "_" + inputVariableValues.get_inputVariablePathnameValue("finalOutput_path");
        }
    }

    if(actualCreateInputs_path.substr(actualCreateInputs_path.length()-1,actualCreateInputs_path.length()) == "/")
    {
        actualCreateInputs_path = actualCreateInputs_path.substr(0,actualCreateInputs_path.length()-1);
    }
    if(actualFinalOutput_path.substr(actualFinalOutput_path.length()-1,actualFinalOutput_path.length()) == "/")
    {
        actualFinalOutput_path = actualFinalOutput_path.substr(0,actualFinalOutput_path.length()-1);
    }

    return success;
}
/*** end other functions ***/
