#include "inputParser.h"

/***** public functions *****/

/*** constructor functions ***/
inputParser::inputParser()
{
    GDALAllRegister();
}
/*** end constructor functions ***/

/*** reinit functions ***/
void inputParser::reset()
{
    while(!foundInputLines.empty())
    {
        foundInputLines.pop_back();
    }
    while(!foundLineTypes.empty())
    {
        foundLineTypes.pop_back();
    }
    while(!foundVarNames.empty())
    {
        foundVarNames.pop_back();
    }
    while(!foundVarNames_inputVariableInfoIndices.empty())
    {
        foundVarNames_inputVariableInfoIndices.pop_back();
    }
    while(!foundVarCountStrings.empty())
    {
        foundVarCountStrings.pop_back();
    }
}
/*** end reinit functions ***/

/*** variable class getter functions ***/
void inputParser::loadInputVariableStorage(std::vector<singleInputVariable> &new_inputVariableStorage, createIgnitionFromLatLongsStorage &new_stored_create_ignition_from_latlongs,
                                                polygonIgnitShapeFileStorage &new_stored_polygon_ignit_shape_files, GeoMACfirePerimeterFileStorage &new_stored_GeoMAC_fire_perimeter_files,
                                                farsiteOutputFirePerimeterFileStorage &new_stored_farsite_output_fire_perimeter_files, wrfFileStorage &new_stored_wrf_files,
                                                additionalWindNinjaOutputs_googleStorage &new_stored_additional_WindNinja_Outputs_google,
                                                additionalWindNinjaOutputs_shapefileStorage &new_stored_additional_WindNinja_Outputs_shapefile,
                                                additionalWindNinjaOutputs_pdfStorage &new_stored_additional_WindNinja_Outputs_pdf)
{
    inputVariableStorage = new_inputVariableStorage;
    stored_create_ignition_from_latlongs = new_stored_create_ignition_from_latlongs;
    stored_polygon_ignit_shape_files = new_stored_polygon_ignit_shape_files;
    stored_GeoMAC_fire_perimeter_files = new_stored_GeoMAC_fire_perimeter_files;
    stored_farsite_output_fire_perimeter_files;
    stored_wrf_files = new_stored_wrf_files;
    stored_additional_WindNinja_Outputs_google = new_stored_additional_WindNinja_Outputs_google;
    stored_additional_WindNinja_Outputs_shapefile = new_stored_additional_WindNinja_Outputs_shapefile;
    stored_additional_WindNinja_Outputs_pdf = new_stored_additional_WindNinja_Outputs_pdf;
}
/*** end variable class getter functions ***/

/*** data passer between inputParser and inputParser functions ***/
std::vector<singleInputVariable> inputParser::transfer_inputVariableStorage()
{
    return inputVariableStorage;
}

createIgnitionFromLatLongsStorage inputParser::transfer_stored_create_ignition_from_latlongs()
{
    return stored_create_ignition_from_latlongs;
}

polygonIgnitShapeFileStorage inputParser::transfer_stored_polygon_ignit_shape_files()
{
    return stored_polygon_ignit_shape_files;
}

GeoMACfirePerimeterFileStorage inputParser::transfer_stored_GeoMAC_fire_perimeter_files()
{
    return stored_GeoMAC_fire_perimeter_files;
}

farsiteOutputFirePerimeterFileStorage inputParser::transfer_stored_farsite_output_fire_perimeter_files()
{
    return stored_farsite_output_fire_perimeter_files;
}
wrfFileStorage inputParser::transfer_stored_wrf_files()
{
    return stored_wrf_files;
}

additionalWindNinjaOutputs_googleStorage inputParser::transfer_stored_additional_WindNinja_Outputs_google()
{
    return stored_additional_WindNinja_Outputs_google;
}

additionalWindNinjaOutputs_shapefileStorage inputParser::transfer_stored_additional_WindNinja_Outputs_shapefile()
{
    return stored_additional_WindNinja_Outputs_shapefile;
}

additionalWindNinjaOutputs_pdfStorage inputParser::transfer_stored_additional_WindNinja_Outputs_pdf()
{
    return stored_additional_WindNinja_Outputs_pdf;
}
/*** end data passer between inputParser and inputParser functions ***/

/*** main loader functions ***/
bool inputParser::readInputFile(std::string inputFileName)
{
    bool success = true;

    reset();

    std::ifstream fzInput;   // input file stream, using pointer to a FILE was not accepted by std::getline
    success = openIsStream(inputFileName,fzInput);
    if(success == true)
    {
        std::string currentLine;
        size_t lineCount = 0;
        while(std::getline(fzInput,currentLine))
        {
            foundInputLines.push_back(currentLine);

            // now find line type
            if(currentLine.length() == 0)
            {
                foundLineTypes.push_back("blank");
            } else if(currentLine.substr(0,1) == commentChar)
            {
                foundLineTypes.push_back("comment");
            } else
            {
                // first clean up line
                removeLeadingWhitespace(currentLine);
                removeEndingWhitespace(currentLine);
                // now look for the input separator, if too many it is an error, if one, it is a variable name line, if none, it is data
                size_t separatorCount = 0;
                for(size_t charIdx = 0; charIdx < currentLine.length(); charIdx++)
                {
                    std::string currentChr = currentLine.substr(charIdx,1);
                    if(currentChr == inputSeparator)
                    {
                        separatorCount = separatorCount + 1;
                    }
                }
                // now determine what to do with the line whether an input separator is found or not
                if(separatorCount > 1)
                {
                    printf("too many input separator \"%s\" chars found on line \"%zu\"!",inputSeparator.c_str(),lineCount+1);
                    success = false;
                    break;
                } else if(separatorCount == 1)
                {
                    foundLineTypes.push_back("varName");
                } else  // isn't a blank line, a comment line, or a variable name line so it has to be variable data. Could still be a badly written line, that will be found when parsing values
                {
                    foundLineTypes.push_back("varData");
                    // checking if there is a varName (varCount) line before the data. Unfortunately have to deal with comment lines
                    // disallowing blank or comment lines between the varData and the varName lines
                    for(size_t lineIdx = foundLineTypes.size()-1; lineIdx >= 0; lineIdx--)  // just added one so it has to always have at least size 1
                    {
                        if(foundLineTypes[lineIdx] == "varName")
                        {
                            // found a varName associated with the varData, so is good
                            break;
                        } else if(foundLineTypes[lineIdx] == "varData")
                        {
                            if(lineIdx == 0)
                            {
                                printf("found varData line without a varName/varCount line right before it! errored on line %zu\n",lineIdx+1);
                                success = false;
                                break;
                            } else
                            {
                                // is varData so not sure if good yet so continue
                                continue;
                            }
                        } else
                        {
                            if(foundLineTypes[lineIdx] == "blank")
                            {
                                printf("found blank line between varData line and varName line! errored on line %zu\n",lineIdx+1);
                                success = false;
                                break;
                            } else if(foundLineTypes[lineIdx] == "comment")
                            {
                                printf("found comment line between varData line and varName line! errored on line %zu\n",lineIdx+1);
                                success = false;
                                break;
                            } else
                            {
                                printf("found variableDataLine without a varName/varCount line right before it! errored on line %zu\n",lineIdx+1);
                                success = false;
                                break;
                            }
                        }
                    }
                }
            }
            lineCount = lineCount + 1;
            if(success == false)    // make sure it exits if an error is found
            {
                break;
            }
        }
        fzInput.close();
    }

    return success;
}

bool inputParser::readVarNamesAndCountValues()
{
    bool success = true;

    // first pull out the individual variable names and variable counts,
    // then check to see if the variable names are valid and store the found strings as needed
    for(size_t lineIdx = 0; lineIdx < foundInputLines.size(); lineIdx++)
    {
        if(foundLineTypes[lineIdx] == "varName")
        {
            // first extract out the parts
            std::string currentLine = foundInputLines[lineIdx];
            std::string foundVarName = "";
            std::string foundCountValue = "";
            removeSeparator(currentLine,foundVarName,foundCountValue);
            removeLeadingWhitespace(foundCountValue);   // just in case
            // now determine if it is a valid variable name, if so, store the values
            bool foundVar = false;
            for(size_t varIdx = 0; varIdx < inputVariableStorage.size(); varIdx++)
            {
                if(inputVariableStorage[varIdx].get_variableName() == foundVarName)
                {
                    foundVarNames.push_back(foundVarName);
                    foundVarNames_inputVariableInfoIndices.push_back(varIdx);
                    inputVariableStorage[varIdx].set_isFoundInInputFile(true,lineIdx);
                    foundVarCountStrings.push_back(foundCountValue);
                    foundVar = true;
                    break;
                }
            }
            if(foundVar == false)
            {
                printf("foundVarName \"%s\" is not a valid variable name!",foundVarName.c_str());
                success = false;
                break;
            }
        }
    }

    return success;
}

bool inputParser::loadCountValues()
{
    bool success = true;

    // now go through and store the variable counts as their appropriate values
    for(size_t varNameIdx = 0; varNameIdx < foundVarCountStrings.size(); varNameIdx++)
    {
        size_t currentInfoIdx = foundVarNames_inputVariableInfoIndices[varNameIdx];
        std::string currentVarName = foundVarNames[varNameIdx];
        std::string currentVarCountString = foundVarCountStrings[varNameIdx];
        std::string currentCountType = inputVariableStorage[currentInfoIdx].get_variableCountType();
        if(currentCountType == "bool")
        {
            if(set_boolValue_string(currentVarName,currentVarCountString) == false)
            {
                printf("couldn't set bool value \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
                success = false;
            }
        } else if(currentCountType == "size_t")
        {
            if(set_size_tValue_string(currentVarName,currentVarCountString) == false)
            {
                printf("couldn't set size_t value \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
            }
        } else if(currentCountType == "int")
        {
            if(set_intValue_string(currentVarName,currentVarCountString) == false)
            {
                printf("couldn't set int value \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
            }
        } else if(currentCountType == "double")
        {
            if(set_doubleValue_string(currentVarName,currentVarCountString,"regular") == false)
            {
                printf("couldn't set double value \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
            }
        } else if(currentCountType == "positive double")
        {
            if(set_doubleValue_string(currentVarName,currentVarCountString,"positive") == false)
            {
                printf("couldn't set positive double value \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
            }
        } else if(currentCountType == "signless percent")
        {
            if(set_doubleValue_string(currentVarName,currentVarCountString,"signless percent") == false)
            {
                printf("couldn't set signless percent value \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
            }
        } else if(currentCountType == "string")
        {
            if(set_stringValue(currentVarName,currentVarCountString) == false)
            {
                printf("couldn't set string \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
            }
        } else if(currentCountType == "pathname")
        {
            if(set_pathNameValue(currentVarName,currentVarCountString) == false)
            {
                printf("couldn't set pathname \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
            }
        } else if(currentCountType == "lcp filename")
        {
            if(set_lcpFileValue(currentVarName,currentVarCountString) == false)
            {
                printf("couldn't set lcp filename \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
            }
        } else if(currentCountType == "shape filename")
        {
            if(set_shapeFileValue(currentVarName,currentVarCountString) == false)
            {
                printf("couldn't set shape filename \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
            }
        } else if(currentCountType == "lat_coord")
        {
            if(set_doubleValue_string(currentVarName,currentVarCountString,"lat_coord") == false)
            {
                printf("couldn't set lat_coord value \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
            }
        } else if(currentCountType == "long_coord")
        {
            if(set_doubleValue_string(currentVarName,currentVarCountString,"long_coord") == false)
            {
                printf("couldn't set long_coord value \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
            }
        } else if(currentCountType == "lat_long_point")
        {
            if(set_lat_longValue_string(currentVarName,currentVarCountString) == false)
            {
                printf("couldn't set lat_long_point value \"%s\" \"(lat long)\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
            }
        } else if(currentCountType == "date")
        {
            if(set_dateValue_string(currentVarName,currentVarCountString) == false)
            {
                printf("couldn't set date value \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
            }
        } else if(currentCountType == "hour_min")
        {
            if(set_hour_minValue_string(currentVarName,currentVarCountString) == false)
            {
                printf("couldn't set hour_min value \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
            }
        } else if(currentCountType == "count")
        {
            if(set_size_tValue_string(currentVarName,currentVarCountString) == false)
            {
                printf("couldn't set size_t count value \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
                success = false;
            }
        } else
        {
            printf("count type \"%s\" for variable \"%s\" has not been implemented in the code yet!\n",currentCountType.c_str(),currentVarName.c_str());
            success = false;
        }
    }

    return success;
}

bool inputParser::checkCountVsLinesOfData()
{
    bool success = true;

    // go through lines of the file, if find varName, see how many lines of varData follow
    // then if too many or too few for a given varName (based on countType then countValue), error
    size_t varNameIdx = 0;
    size_t lineIdx = 0;
    while(lineIdx < foundInputLines.size())
    {
        if(foundLineTypes[lineIdx] == "varName")
        {
            // find number of varData lines for given varName
            size_t dataCount = 0;
            while(lineIdx != foundInputLines.size() && foundLineTypes[lineIdx+1] == "varData")
            {
                dataCount = dataCount + 1;
                lineIdx = lineIdx + 1;
            }

            // now see if it is the right number of lines for the current varName
            size_t currentInfoIdx = foundVarNames_inputVariableInfoIndices[varNameIdx];
            std::string currentVarName = foundVarNames[varNameIdx];
            std::string currentCountType = inputVariableStorage[currentInfoIdx].get_variableCountType();
            if(currentCountType != "count")
            {
                if(dataCount != 0)
                {
                    printf("found \"%zu\" lines of varData for varName \"%s\", should have found no varData!\n",dataCount,currentVarName.c_str());
                    success = false;
                    break;
                }
            } else
            {
                size_t currentVarCount = get_size_tValue(currentVarName).get_storedSize_tValue();
                if(dataCount != currentVarCount)
                {
                    printf("found \"%zu\" lines of varData for varName \"%s\", should have found \"%zu\" lines of varData!\n",dataCount,currentVarName.c_str(),currentVarCount);
                    success = false;
                    break;
                }
            }

            // now be ready for the next varName when found in the file
            varNameIdx = varNameIdx + 1;
        }
        if(success == false)    // make sure it exits if an error is found
        {
            break;
        }
        // increment for the next loop
        lineIdx = lineIdx + 1;
    }

    return success;
}

bool inputParser::loadLoaderFunctionData()
{
    bool success = true;

    // go through lines of the file, if find varName and type is count, attempt to store the lines of data
    size_t varNameIdx = 0;
    size_t lineIdx = 0;
    while(lineIdx < foundInputLines.size())
    {
        if(foundLineTypes[lineIdx] == "varName")
        {
            size_t currentInfoIdx = foundVarNames_inputVariableInfoIndices[varNameIdx];
            std::string currentVarName = foundVarNames[varNameIdx];
            std::string currentCountType = inputVariableStorage[currentInfoIdx].get_variableCountType();
            if(currentCountType == "count")
            {
                size_t currentVarCount = get_size_tValue(currentVarName).get_storedSize_tValue();
                if(currentVarName == "create_ignition_from_latlongs")
                {
                    for(size_t dataIdx = 0; dataIdx < currentVarCount; dataIdx++)
                    {
                        lineIdx = lineIdx + 1;  // already read one line, need to move to the data line
                        if(add_createIgnitionLatLongValue_Line(foundInputLines[lineIdx]) == false)
                        {
                            printf("couldn't parse line \"%zu\" \"%s\" as a \"%s\" variable!\n",lineIdx+1,foundInputLines[lineIdx].c_str(),currentVarName.c_str());
                            success = false;
                            break;
                        }
                    }
                } else if(currentVarName == "polygon_ignit_shape_files")
                {
                    for(size_t dataIdx = 0; dataIdx < currentVarCount; dataIdx++)
                    {
                        lineIdx = lineIdx + 1;  // already read one line, need to move to the data line
                        if(add_polygonIgnitShapeFile_Line(foundInputLines[lineIdx]) == false)
                        {
                            printf("couldn't parse line \"%zu\" \"%s\" as a \"%s\" variable!\n",lineIdx+1,foundInputLines[lineIdx].c_str(),currentVarName.c_str());
                            success = false;
                            break;
                        }
                    }
                } else if(currentVarName == "GeoMAC_fire_perimeter_files")
                {
                    for(size_t dataIdx = 0; dataIdx < currentVarCount; dataIdx++)
                    {
                        lineIdx = lineIdx + 1;  // already read one line, need to move to the data line
                        if(add_GeoMACfirePerimeterFile_Line(foundInputLines[lineIdx]) == false)
                        {
                            printf("couldn't parse line \"%zu\" \"%s\" as a \"%s\" variable!\n",lineIdx+1,foundInputLines[lineIdx].c_str(),currentVarName.c_str());
                            success = false;
                            break;
                        }
                    }
                } else if(currentVarName == "farsite_output_fire_perimeter_files")
                {
                    for(size_t dataIdx = 0; dataIdx < currentVarCount; dataIdx++)
                    {
                        lineIdx = lineIdx + 1;  // already read one line, need to move to the data line
                        if(add_farsiteOutputFirePerimeterFile_Line(foundInputLines[lineIdx]) == false)
                        {
                            printf("couldn't parse line \"%zu\" \"%s\" as a \"%s\" variable!\n",lineIdx+1,foundInputLines[lineIdx].c_str(),currentVarName.c_str());
                            success = false;
                            break;
                        }
                    }
                } else if(currentVarName == "wrf_files")
                {
                    debugTimers.startNewTime("load_wrf_files timing during input loading");
                    for(size_t dataIdx = 0; dataIdx < currentVarCount; dataIdx++)
                    {
                        lineIdx = lineIdx + 1;  // already read one line, need to move to the data line
                        if(add_wrfFile_Line(foundInputLines[lineIdx]) == false)
                        {
                            printf("couldn't parse line \"%zu\" \"%s\" as a \"%s\" variable!\n",lineIdx+1,foundInputLines[lineIdx].c_str(),currentVarName.c_str());
                            success = false;
                            break;
                        }
                    }
                    debugTimers.getAndPrint_time("load_wrf_files timing during input loading");
                } else if(currentVarName == "additional_WindNinja_outputs_google")
                {
                    for(size_t dataIdx = 0; dataIdx < currentVarCount; dataIdx++)
                    {
                        lineIdx = lineIdx + 1;  // already read one line, need to move to the data line
                        if(add_additionalWindNinjaOutputs_googleValue_Line(foundInputLines[lineIdx]) == false)
                        {
                            printf("couldn't parse line \"%zu\" \"%s\" as a \"%s\" variable!\n",lineIdx+1,foundInputLines[lineIdx].c_str(),currentVarName.c_str());
                            success = false;
                            break;
                        }
                    }
                } else if(currentVarName == "additional_WindNinja_outputs_shapefile")
                {
                    for(size_t dataIdx = 0; dataIdx < currentVarCount; dataIdx++)
                    {
                        lineIdx = lineIdx + 1;  // already read one line, need to move to the data line
                        if(add_additionalWindNinjaOutputs_shapeValue_Line(foundInputLines[lineIdx]) == false)
                        {
                            printf("couldn't parse line \"%zu\" \"%s\" as a \"%s\" variable!\n",lineIdx+1,foundInputLines[lineIdx].c_str(),currentVarName.c_str());
                            success = false;
                            break;
                        }
                    }
                } else if(currentVarName == "additional_WindNinja_outputs_pdf")
                {
                    for(size_t dataIdx = 0; dataIdx < currentVarCount; dataIdx++)
                    {
                        lineIdx = lineIdx + 1;  // already read one line, need to move to the data line
                        if(add_additionalWindNinjaOutputs_pdfValue_Line(foundInputLines[lineIdx]) == false)
                        {
                            printf("couldn't parse line \"%zu\" \"%s\" as a \"%s\" variable!\n",lineIdx+1,foundInputLines[lineIdx].c_str(),currentVarName.c_str());
                            success = false;
                            break;
                        }
                    }
                } else
                {
                    printf("loader function for variable \"%s\" has not been implemented in the code yet!\n",currentVarName.c_str());
                    success = false;
                    break;
                }
            }
            // now be ready for the next varName when found in the file
            varNameIdx = varNameIdx + 1;
        }
        if(success == false)    // make sure it exits if an error is found
        {
            break;
        }
        // increment for the next loop
        lineIdx = lineIdx + 1;
    }

    return success;
}
/*** end main loader functions ***/



/***** private functions *****/



/*** parsing utility functions ***/
bool inputParser::openIsStream(std::string fileName,std::ifstream &fzStream)
{
    bool success = true;

    FILE *fzTemp = fopen(fileName.c_str(),"r");    // use "w" for write
    if( fzTemp == NULL)
    {
        printf("File \"%s\" could not be opened! Failed to open input file!\n",fileName.c_str());
        success = false;
    }
    fclose(fzTemp);

    // figure out if file exists. If so, open this input file as the file to pass around to all the loader functions
    if(success == true)
    {
        fzStream.open(fileName.c_str());
    }

    return success;
}

void inputParser::removeLeadingWhitespace(std::string &inputString)
{
    for(size_t charIdx = 0; charIdx < inputString.length(); charIdx++)
    {
        std::string currentChr = inputString.substr(charIdx,1);
        if(currentChr != " ")
        {
            inputString = inputString.substr(charIdx,inputString.length());
            break;
        }
    }
}

void inputParser::removeEndingWhitespace(std::string &inputString)
{
    if(inputString.length() != 0)
    {
        for(size_t charIdx = inputString.length()-1; charIdx > 0; charIdx--)
        {
            std::string currentChr = inputString.substr(charIdx,1);
            if(currentChr != " ")
            {
                inputString = inputString.substr(0,charIdx+1);
                break;
            }
        }
    }
}

void inputParser::removeSeparator(std::string inputString, std::string &varNamePart, std::string &varCountPart)
{
    // assumes that inputString does not start with a comment character
    removeLeadingWhitespace(inputString);   // just in case
    removeEndingWhitespace(inputString);    // just in case

    // Look for the separator, everything before the separator is the variableName
    // set the current string to everything after the separator just in case it is needed for extracting a variable count value
    // if the inputSeparator is used for some input variables, could lead to strange errors, but since values have to be so exact in format, I think it will be fine
    for(size_t charIdx = 0; charIdx < inputString.length(); charIdx++)
    {
        std::string currentChr = inputString.substr(charIdx,1);
        if(currentChr == inputSeparator)
        {
            varNamePart = inputString.substr(0,charIdx);
            removeEndingWhitespace(varNamePart);
            varCountPart = inputString.substr(charIdx+1,inputString.length());
            removeLeadingWhitespace(inputString);   // only possible place left with whitespace is at the beginning
            break;
        }
    }
}
/*** end parsing utility functions ***/

/*** set variable value functions ***/
bool inputParser::set_boolValue_string(std::string varName, std::string newStringBoolValue)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to set the value of interest
    ((boolValue*)variableTypeClass)->set_storedBoolValue_string(newStringBoolValue);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;

    // if it got here, no error occurred
    return 0;
}

bool inputParser::set_size_tValue_string(std::string varName, std::string newStringSize_tValue)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to set the value of interest
    ((size_tValue*)variableTypeClass)->set_storedSize_tValue_string(newStringSize_tValue);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;

    // if it got here, no error occurred
    return 0;
}

bool inputParser::set_intValue_string(std::string varName, std::string newStringIntValue)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to set the value of interest
    ((intValue*)variableTypeClass)->set_storedIntValue_string(newStringIntValue);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;

    // if it got here, no error occurred
    return 0;
}

bool inputParser::set_doubleValue_string(std::string varName, std::string newStringDoubleValue, std::string doubleType)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to set the value of interest
    ((doubleValue*)variableTypeClass)->set_storedDoubleValue_string(newStringDoubleValue,doubleType);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;

    // if it got here, no error occurred
    return 0;
}

/*bool inputParser::set_lat_longValue_string(std::string varName, std::string newStringLatValue, std::string newStringLongValue)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to set the value of interest
    ((lat_longValue*)variableTypeClass)->set_storedLatLongValue_string(newStringLatValue,newStringLongValue);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;

    // if it got here, no error occurred
    return 0;
}*/

bool inputParser::set_lat_longValue_string(std::string varName, std::string newLatLongValueString)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to set the value of interest
    ((lat_longValue*)variableTypeClass)->set_storedLatLongValue_string(newLatLongValueString);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;

    // if it got here, no error occurred
    return 0;
}

/*bool inputParser::set_dateValue_string(std::string varName, std::string newStringYearValue, std::string newStringMonthValue, std::string newStringDayValue, std::string newStringHourValue, std::string newStringMinuteValue)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to set the value of interest
    ((dateValue*)variableTypeClass)->set_storedDateValue_string(newStringYearValue,newStringMonthValue,newStringDayValue,newStringHourValue,newStringMinuteValue);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;

    // if it got here, no error occurred
    return 0;
}*/

bool inputParser::set_dateValue_string(std::string varName, std::string newDateValueString)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to set the value of interest
    ((dateValue*)variableTypeClass)->set_storedDateValue_string(newDateValueString);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;

    // if it got here, no error occurred
    return 0;
}

/*bool inputParser::set_hour_minValue_string(std::string varName, std::string newStringHourValue, std::string newStringMinuteValue)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to set the value of interest
    ((hour_minValue*)variableTypeClass)->set_storedHour_MinValue_string(newStringHourValue,newStringMinuteValue);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;

    // if it got here, no error occurred
    return 0;
}*/

bool inputParser::set_hour_minValue_string(std::string varName, std::string newHourMinValue)
{
    // first look for variable in vector of values, using the name
    size_t varIdx = findVarInfoIdx(varName);

    // create a temporary generic pointer for pulling out the value
    void* variableTypeClass;
    // set the generic pointer to the input variable value pointer
    variableTypeClass = inputVariableStorage[varIdx].get_variableTypeClass();
    // use the pointer to set the value of interest
    ((hour_minValue*)variableTypeClass)->set_storedHour_MinValue_string(newHourMinValue);
    // delete the temporary pointer so no leaking memory. The input variable value pointer will still continue on
    //delete variableTypeClass;

    // if it got here, no error occurred
    return 0;
}
/*** end set variable value functions ***/

/*** load function stuff for count type variables ***/
bool inputParser::add_createIgnitionLatLongValue_Line(std::string inputDataLine)
{
    return stored_create_ignition_from_latlongs.add_createIgnitionLatLongValue_Line(inputDataLine);
}

bool inputParser::add_polygonIgnitShapeFile_Line(std::string inputDataLine)
{
    return stored_polygon_ignit_shape_files.add_polygonIgnitShapeFile_Line(inputDataLine);
}

bool inputParser::add_GeoMACfirePerimeterFile_Line(std::string inputDataLine)
{
    return stored_GeoMAC_fire_perimeter_files.add_GeoMACfirePerimeterFile_Line(inputDataLine);
}

bool inputParser::add_farsiteOutputFirePerimeterFile_Line(std::string inputDataLine)
{
    return stored_farsite_output_fire_perimeter_files.add_farsiteOutputFirePerimeterFile_Line(inputDataLine);
}

bool inputParser::add_wrfFile_Line(std::string inputDataLine)
{
    return stored_wrf_files.add_wrfFile_Line(inputDataLine);
}

bool inputParser::add_additionalWindNinjaOutputs_googleValue_Line(std::string inputDataLine)
{
    return stored_additional_WindNinja_Outputs_google.add_additionalWindNinjaOutputs_googleValue_Line(inputDataLine);
}

bool inputParser::add_additionalWindNinjaOutputs_shapeValue_Line(std::string inputDataLine)
{
    return stored_additional_WindNinja_Outputs_shapefile.add_additionalWindNinjaOutputs_shapeValue_Line(inputDataLine);
}

bool inputParser::add_additionalWindNinjaOutputs_pdfValue_Line(std::string inputDataLine)
{
    return stored_additional_WindNinja_Outputs_pdf.add_additionalWindNinjaOutputs_pdfValue_Line(inputDataLine);
}
/*** end load function stuff for count type variables ***/

/*** other set variable value functions ***/
bool inputParser::set_stringValue(std::string varName, std::string newStringValue)
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

bool inputParser::set_pathNameValue(std::string varName, std::string newPathNameValue)
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

bool inputParser::set_lcpFileValue(std::string varName, std::string newLcpFileValue)
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

bool inputParser::set_shapeFileValue(std::string varName, std::string newShapeFileValue)
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

bool inputParser::set_wrfFileValue(std::string varName, std::string newWrfFileValue)
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
/*** end other set variable value functions ***/

/*** get variable value functions ***/
size_tValue inputParser::get_size_tValue(std::string varName)
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

size_t inputParser::findVarInfoIdx(std::string varName)
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
/*** end get variable value functions ***/
