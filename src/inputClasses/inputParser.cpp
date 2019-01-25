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
void inputParser::loadVariableInfo(inputVariables_valueStorage* newInputVariables)
{
    inputVariableValueStorage = newInputVariables;  // is this the right thing to do with pointers?
    inputVariablesInfo = inputVariableValueStorage->get_inputVariableInfo();
}
/*** end variable class getter functions ***/

/*** data passer between inputVariablesHandler and inputParser functions ***/
std::vector<inputVariable_info> inputParser::transferVariableInfo()
{
    return inputVariablesInfo;
}
/*** end data passer between inputVariablesHandler and inputParser functions ***/

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
            for(size_t varIdx = 0; varIdx < inputVariablesInfo.size(); varIdx++)
            {
                if(inputVariablesInfo[varIdx].get_variableName() == foundVarName)
                {
                    foundVarNames.push_back(foundVarName);
                    foundVarNames_inputVariableInfoIndices.push_back(varIdx);
                    inputVariablesInfo[varIdx].set_isFoundInInputFile(true,lineIdx);
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
        std::string currentCountType = inputVariablesInfo[currentInfoIdx].get_variableCountType();
        if(currentCountType == "bool")
        {
            if(inputVariableValueStorage->set_boolValue_string(currentVarName,currentVarCountString) == false)
            {
                printf("couldn't set bool value \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
                success = false;
            }
        } else if(currentCountType == "size_t")
        {
            if(inputVariableValueStorage->set_size_tValue_string(currentVarName,currentVarCountString) == false)
            {
                printf("couldn't set size_t value \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
            }
        } else if(currentCountType == "int")
        {
            if(inputVariableValueStorage->set_intValue_string(currentVarName,currentVarCountString) == false)
            {
                printf("couldn't set int value \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
            }
        } else if(currentCountType == "double")
        {
            if(inputVariableValueStorage->set_doubleValue_string(currentVarName,currentVarCountString,"regular") == false)
            {
                printf("couldn't set double value \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
            }
        } else if(currentCountType == "positive double")
        {
            if(inputVariableValueStorage->set_doubleValue_string(currentVarName,currentVarCountString,"positive") == false)
            {
                printf("couldn't set positive double value \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
            }
        } else if(currentCountType == "signless percent")
        {
            if(inputVariableValueStorage->set_doubleValue_string(currentVarName,currentVarCountString,"signless percent") == false)
            {
                printf("couldn't set signless percent value \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
            }
        } else if(currentCountType == "string")
        {
            if(inputVariableValueStorage->set_stringValue(currentVarName,currentVarCountString) == false)
            {
                printf("couldn't set string \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
            }
        } else if(currentCountType == "pathname")
        {
            if(inputVariableValueStorage->set_pathNameValue(currentVarName,currentVarCountString) == false)
            {
                printf("couldn't set pathname \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
            }
        } else if(currentCountType == "lcp filename")
        {
            if(inputVariableValueStorage->set_lcpFileValue(currentVarName,currentVarCountString) == false)
            {
                printf("couldn't set lcp filename \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
            }
        } else if(currentCountType == "shape filename")
        {
            if(inputVariableValueStorage->set_shapeFileValue(currentVarName,currentVarCountString) == false)
            {
                printf("couldn't set shape filename \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
            }
        } else if(currentCountType == "lat_coord")
        {
            if(inputVariableValueStorage->set_doubleValue_string(currentVarName,currentVarCountString,"lat_coord") == false)
            {
                printf("couldn't set lat_coord value \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
            }
        } else if(currentCountType == "long_coord")
        {
            if(inputVariableValueStorage->set_doubleValue_string(currentVarName,currentVarCountString,"long_coord") == false)
            {
                printf("couldn't set long_coord value \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
            }
        } else if(currentCountType == "lat_long_point")
        {
            if(inputVariableValueStorage->set_lat_longValue_string(currentVarName,currentVarCountString) == false)
            {
                printf("couldn't set lat_long_point value \"%s\" \"(lat long)\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
            }
        } else if(currentCountType == "date")
        {
            if(inputVariableValueStorage->set_dateValue_string(currentVarName,currentVarCountString) == false)
            {
                printf("couldn't set date value \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
            }
        } else if(currentCountType == "hour_min")
        {
            if(inputVariableValueStorage->set_hour_minValue_string(currentVarName,currentVarCountString) == false)
            {
                printf("couldn't set hour_min value \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
            }
        } else if(currentCountType == "count")
        {
            if(inputVariableValueStorage->set_size_tValue_string(currentVarName,currentVarCountString) == false)
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
            std::string currentCountType = inputVariablesInfo[currentInfoIdx].get_variableCountType();
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
                size_t currentVarCount = inputVariableValueStorage->get_size_tValue(currentVarName).get_storedSize_tValue();
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
            std::string currentCountType = inputVariablesInfo[currentInfoIdx].get_variableCountType();
            if(currentCountType == "count")
            {
                size_t currentVarCount = inputVariableValueStorage->get_size_tValue(currentVarName).get_storedSize_tValue();
                if(currentVarName == "create_ignition_from_latlongs")
                {
                    for(size_t dataIdx = 0; dataIdx < currentVarCount; dataIdx++)
                    {
                        lineIdx = lineIdx + 1;  // already read one line, need to move to the data line
                        if(inputVariableValueStorage->add_createIgnitionLatLongValue_Line(foundInputLines[lineIdx]) == false)
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
                        if(inputVariableValueStorage->add_polygonIgnitShapeFile_Line(foundInputLines[lineIdx]) == false)
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
                        if(inputVariableValueStorage->add_GeoMACfirePerimeterFile_Line(foundInputLines[lineIdx]) == false)
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
                        if(inputVariableValueStorage->add_farsiteOutputFirePerimeterFile_Line(foundInputLines[lineIdx]) == false)
                        {
                            printf("couldn't parse line \"%zu\" \"%s\" as a \"%s\" variable!\n",lineIdx+1,foundInputLines[lineIdx].c_str(),currentVarName.c_str());
                            success = false;
                            break;
                        }
                    }
                } else if(currentVarName == "wrf_files")
                {
                    for(size_t dataIdx = 0; dataIdx < currentVarCount; dataIdx++)
                    {
                        lineIdx = lineIdx + 1;  // already read one line, need to move to the data line
                        if(inputVariableValueStorage->add_wrfFile_Line(foundInputLines[lineIdx]) == false)
                        {
                            printf("couldn't parse line \"%zu\" \"%s\" as a \"%s\" variable!\n",lineIdx+1,foundInputLines[lineIdx].c_str(),currentVarName.c_str());
                            success = false;
                            break;
                        }
                    }
                } else if(currentVarName == "additional_WindNinja_outputs_google")
                {
                    for(size_t dataIdx = 0; dataIdx < currentVarCount; dataIdx++)
                    {
                        lineIdx = lineIdx + 1;  // already read one line, need to move to the data line
                        if(inputVariableValueStorage->add_additionalWindNinjaOutputs_googleValue_Line(foundInputLines[lineIdx]) == false)
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
                        if(inputVariableValueStorage->add_additionalWindNinjaOutputs_shapeValue_Line(foundInputLines[lineIdx]) == false)
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
                        if(inputVariableValueStorage->add_additionalWindNinjaOutputs_pdfValue_Line(foundInputLines[lineIdx]) == false)
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
