#include "inputParser.h"

/***** public functions *****/

/*** constructor functions ***/
inputParser::inputParser()
{
    inputFileTotalLines = 0;
    GDALAllRegister();
}
/*** end constructor functions ***/

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
bool inputParser::findVariableNamesAndCountValues(std::string inputFileName)
{
    bool success = true;

    success = findInputTotalLineCount(inputFileName);
    if(success == false)
    {
        printf("problem finding input file number of lines!\n");
    } else
    {
        for(size_t varIdx = 0; varIdx < inputVariablesInfo.size(); varIdx++)
        {
            // search in file for each variable name, which also finds the count variable value and the line in the input file with said variable name and count value
            // set isFoundInInputs variable in class to resulting bool of findVariable(). Also set the variable line location in the class
            // process found count variable to also add that to the storage
            std::string currentVarName = inputVariablesInfo[varIdx].get_variableName();
            std::string currentVarCountString = "";
            size_t varNameLine = 0;
            bool foundVar = false;
            std::string currentCountType = inputVariablesInfo[varIdx].get_variableCountType();
            success = variableNameAndCountFinder(inputFileName,currentVarName,currentVarCountString,varNameLine,foundVar);
            if(success == false)
            {
                printf("error running variableNameAndCountFinder function for variable \"%s\"!\n",currentVarName.c_str());
            } else
            {
                if(foundVar == true)
                {
                    inputVariablesInfo[varIdx].set_isFoundInInputFile(true,varNameLine);
                    inputVariablesInfo[varIdx].get_isFoundInInputFile();    // added for now, delete later

                    // now add on the count variable processing part
                    bool conversionSuccess = false;
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
                }   // no else, cause this lets all be optional. The checkSetVarNamesForConflictingOptions function in inputVariablesHandler is for actually choosing if is optional or not
            }
        }
    }

    return success;
}

bool inputParser::loadAllInputs(std::string inputFileName)
{
    bool success = true;

    for(size_t varIdx = 0; varIdx < inputVariablesInfo.size(); varIdx++)
    {
        if(inputVariablesInfo[varIdx].get_variableName() == "wrfGetWeather_output_units")
            inputVariablesInfo[varIdx].get_isFoundInInputFile();
        std::string currentCountType = inputVariablesInfo[varIdx].get_variableCountType();
        if(currentCountType == "count")
        {
            // if is found in input file, big if statement checking variableCountType and using the proper load function
            //  If it reaches the else, no loader function is used so give a warning to the user/programmer that it is not an allowed/required input
            std::string currentVarName = inputVariablesInfo[varIdx].get_variableName();
            size_t currentCountLine = inputVariablesInfo[varIdx].get_inputFileVariableCountLine();
            std::string currentVarCountString = "";
            if(inputVariablesInfo[varIdx].get_isFoundInInputFile() == true)
            {
                size_t currentVarCount = inputVariableValueStorage->get_size_tValue(currentVarName).get_storedSize_tValue();
                if(currentVarName == "create_ignition_from_latlongs")
                {
                    if(load_create_ignition_from_latlongs(inputFileName,currentVarCount,currentCountLine) == false)
                    {
                        printf("failed to load create_ignition_from_latlongs. Make sure the count number matches the number of lines of data and data is valid!\n");
                        success = false;
                    }
                } else if(currentVarName == "polygon_ignit_shape_files")
                {
                    if(load_polygon_ignit_shape_files(inputFileName,currentVarCount,currentCountLine) == false)
                    {
                        printf("failed to load polygon_ignit_shape_files. Make sure the count number matches the number of lines of data and data is valid!\n");
                        success = false;
                    }
                } else if(currentVarName == "GeoMAC_fire_perimeter_files")
                {
                    if(load_GeoMAC_fire_perimeter_files(inputFileName,currentVarCount,currentCountLine) == false)
                    {
                        printf("failed to load GeoMAC_fire_perimeter_files. Make sure the count number matches the number of lines of data and data is valid!\n");
                        success = false;
                    }
                } else if(currentVarName == "farsite_output_fire_perimeter_files")
                {
                    if(load_farsite_output_fire_perimeter_files(inputFileName,currentVarCount,currentCountLine) == false)
                    {
                        printf("failed to load farsite_output_fire_perimeter_files. Make sure the count number matches the number of lines of data and data is valid!\n");
                        success = false;
                    }
                } else if(currentVarName == "wrf_files")
                {
                    if(load_wrf_files(inputFileName,currentVarCount,currentCountLine) == false)
                    {
                        printf("failed to load wrf_files. Make sure the count number matches the number of lines of data and data is valid!\n");
                        success = false;
                    }
                } else if(currentVarName == "additional_WindNinja_outputs_google")
                {
                    if(load_additional_WindNinja_outputs_google(inputFileName,currentVarCount,currentCountLine) == false)
                    {
                        printf("failed to load additional_WindNinja_outputs_google. Make sure the count number matches the number of lines of data and data is valid!\n");
                        success = false;
                    }
                } else if(currentVarName == "additional_WindNinja_outputs_shapefile")
                {
                    if(load_additional_WindNinja_outputs_shapefile(inputFileName,currentVarCount,currentCountLine) == false)
                    {
                        printf("failed to load additional_WindNinja_outputs_shapefile. Make sure the count number matches the number of lines of data and data is valid!\n");
                        success = false;
                    }
                } else if(currentVarName == "additional_WindNinja_outputs_pdf")
                {
                    if(load_additional_WindNinja_outputs_pdf(inputFileName,currentVarCount,currentCountLine) == false)
                    {
                        printf("failed to load additional_WindNinja_outputs_pdf. Make sure the count number matches the number of lines of data and data is valid!\n");
                        success = false;
                    }
                } else
                {
                    printf("loader function for variable \"%s\" has not been implemented in the code yet!\n",currentVarName.c_str());
                    success = false;
                }
            }
        }
    }

    return success;
}
/*** end main loader functions ***/



/***** private functions *****/



/*** parsing utility functions ***/
bool inputParser::doesRegularFileExist(std::string fileName, bool suppressWarnings)
{
    bool success = true;

    FILE *fzTemp = fopen(fileName.c_str(),"r");    // use "w" for write
    if( fzTemp == NULL && suppressWarnings == false)
    {
        printf("file \"%s\" could not be opened!\n",fileName.c_str());
        success = false;
    }
    fclose(fzTemp);

    // do we need to delete pointer or does fclose do that?
    //delete(fzTemp);

    return success;
}

bool inputParser::doesFilenameExist(std::string fileName)
{
    // found this here: https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
    // hopefully it works for files as well as directories as this is the same thing used to check a file path. More interesting hopefully this works for .prj files!
    struct stat buffer;
    return (stat (fileName.c_str(), &buffer) == 0);
}

bool inputParser::openIsStream(std::string fileName,std::ifstream &fzStream)
{
    bool success = true;

    // run does file exist function on inputFileName. If so, open this input file as the file to pass around to all the loader functions
    if(doesRegularFileExist(fileName,false) == false)
    {
        printf("failed to open input file!\n");
        success = false;
    } else
    {
        fzStream.open(fileName.c_str());
    }

    return success;
}

void inputParser::closeIsStream(std::ifstream &fzStream)
{
    // close file and delete pointer
    fzStream.close();
    // do we need to delete pointer or does fclose do that?
    //delete(fzInput);
}

bool inputParser::findInputTotalLineCount(std::string inputFileName)
{
    bool success = true;
    std::ifstream fzInput;

    success = openIsStream(inputFileName,fzInput);
    if(success == true)
    {
        std::string currentLine;
        size_t lineCount = 0;
        while(std::getline(fzInput,currentLine))
        {
            lineCount = lineCount + 1;
        }

        inputFileTotalLines = lineCount;

        closeIsStream(fzInput);
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

void inputParser::removeSeparator(std::string &inputString, std::string &varNamePart)
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
            inputString = inputString.substr(charIdx+1,inputString.length());
            removeLeadingWhitespace(inputString);   // only possible place left with whitespace is at the beginning
            break;
        }
    }
}

// basically the same as the variableNameFinder, but now it actually does stuff with the rest of the information just in case
bool inputParser::variableNameAndCountFinder(std::string inputFileName, std::string varName, std::string &varCountString, size_t &varNameLine, bool &foundVar)
{
    bool success = true;
    foundVar = false;

    std::ifstream fzInput;   // input file stream, using pointer to a FILE was not accepted by std::getline
    success = openIsStream(inputFileName,fzInput);
    if(success == true)
    {
        std::string currentLine;
        size_t lineCount = 0;
        while(std::getline(fzInput,currentLine))
        {
            removeLeadingWhitespace(currentLine);

            // check to see if it is a comment line
            if(currentLine.substr(0,1) == "#")
            {
                lineCount = lineCount + 1;
                continue;
            }

            // search line for separator, if found set varName to everything before the separator, what is left replaces the currentLine variable for later processing
            std::string currentVarName = "";
            removeSeparator(currentLine,currentVarName);    // returns with no whitespace on either side of each output now

            // compare the variableName that is found with the desired variableName
            // if it matches, process what is left of the currentLine to get the variableCount value as a string for processing by external functions if needed
            if(currentVarName == varName)
            {
                foundVar = true;
                varNameLine = lineCount;
                varCountString = currentLine;
                break;
            }
            lineCount = lineCount + 1;
        }
        closeIsStream(fzInput);
    }

    return success;
}
/*** end parsing utility functions ***/

/*** typechecker functions ***/
/*bool inputParser::isValidRegularFilename(std::string inputString)
{
    bool conversionSuccess = true;

    removeLeadingWhitespace(inputString);   // just in case
    removeEndingWhitespace(inputString);    // just in case

    // see if it can open as a filename
    if(doesRegularFileExist(inputString,true) == false)
    {
        conversionSuccess = false;
    }

    return conversionSuccess;
}*/
/*** end typechecker functions ***/

/*** type count loader functions ***/
bool inputParser::load_create_ignition_from_latlongs(std::string inputFileName, size_t varCount, size_t varNameLine)
{
    bool success = true;

    if(inputFileTotalLines < varCount + varNameLine)
    {
        printf("not enough lines in input file for create_ignition_from_latlongs count of \"%zu\"!\n",varCount);
        success = false;
    }

    std::ifstream fzInput;
    if(success == true)
    {
        success = openIsStream(inputFileName,fzInput);
        if(success == false)
        {
            printf("problem opening input file!\n");
        }
    }

    // first find the line with data. Unfortunately ifstream won't allow a function to pass an open ifstream so need to do this manually in a given function every time
    // info for this problem found at: https://arstechnica.com/civis/viewtopic.php?f=20&t=145498
    if(success == true)
    {
        success = false;    // annoying logic to get the right line and the right boolean output
        std::string currentLine;
        size_t lineCount = 0;
        while(std::getline(fzInput,currentLine))
        {
            if(lineCount == varNameLine)
            {
                success = true;
                break;
            }
            lineCount = lineCount + 1;
        }
    }

    // okay now continue on in the ifstream as needed
    if(success == false)
    {
        printf("couldn't find varNameLine \"%zu\"!\n",varNameLine);
    } else
    {
        // should already be at the right line, should grab the next one. Don't allow comment lines in this section
        for(size_t lineCount = 0; lineCount < varCount+1; lineCount++)
        {
            if(varNameLine + lineCount <= inputFileTotalLines)  // stops errors if running one line more than is in file when trying out an extra line to see if too much data
            {
                std::string currentLine;
                std::getline(fzInput,currentLine);

                removeLeadingWhitespace(currentLine);
                removeEndingWhitespace(currentLine);

                bool isValue = true;
                std::vector<std::string> foundValues;
                size_t startValueSpot = 0;
                for(size_t charIdx = 0; charIdx < currentLine.length(); charIdx++)
                {
                    std::string currentChr = currentLine.substr(charIdx,1);
                    if(currentChr == " " && isValue == true)
                    {
                        foundValues.push_back(currentLine.substr(startValueSpot,charIdx-startValueSpot));
                        isValue = false;
                    }
                    if(currentChr != " " && isValue == false)
                    {
                        startValueSpot = charIdx;
                        isValue = true;
                    }
                    if(charIdx == currentLine.length()-1 && isValue == true)
                    {
                        foundValues.push_back(currentLine.substr(startValueSpot,charIdx-startValueSpot+1));
                    }
                }

                /*printf("found values are:");
                for(size_t valIdx = 0; valIdx < foundValues.size(); valIdx++)
                {
                    printf(" \"%s\"",foundValues[valIdx].c_str());
                }
                printf("\n\n");*/
                double foundLatCoord = 0.0;
                double foundLongCoord = 0.0;
                bool conversionSuccess = true;
                if(foundValues.size() != 2)
                {
                    if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                    {
                        printf("not enough values found in create_ignition_from_latlongs!\n");
                    }
                    conversionSuccess = false;
                } else
                {
                    bool isLatCoord = false;
                    foundLatCoord = strToLat_Coord(foundValues[0],isLatCoord);
                    if(isLatCoord == false)
                    {
                        if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                        {
                            printf("problem converting \"%s\" value to lat_coord!\n",foundValues[0].c_str());
                        }
                        conversionSuccess = false;
                    }
                    bool isLongCoord = false;
                    foundLongCoord = strToLong_Coord(foundValues[1],isLongCoord);
                    if(isLongCoord == false)
                    {
                        if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                        {
                            printf("problem converting \"%s\" value to long_coord!\n",foundValues[1].c_str());
                        }
                        conversionSuccess = false;
                    }
                }

                if(conversionSuccess == false)
                {
                    if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                    {
                        printf("not enough create_ignition_from_latlongs for create_ignition_from_latlongs variable count!\n");
                        success = false;
                        break;
                    }
                } else
                {
                    if(lineCount >= varCount)   // should be checking to see if it is varCount+1, so one line extra
                    {
                        printf("too many create_ignition_from_latlongs for create_ignition_from_latlongs variable count!\n");
                        success = false;
                        break;
                    } else
                    {
                        inputVariableValueStorage->add_createIgnitionLatLongValue(foundLatCoord,foundLongCoord);   // only adds files if it is a valid line of data and is valid data
                    }
                }
            }
        }
        closeIsStream(fzInput);
    }

    return success;
}

bool inputParser::load_polygon_ignit_shape_files(std::string inputFileName, size_t varCount, size_t varNameLine)
{
    bool success = true;

    if(inputFileTotalLines < varCount + varNameLine)
    {
        printf("not enough lines in input file for polygon_ignit_shape_files count of \"%zu\"!\n",varCount);
        success = false;
    }

    std::ifstream fzInput;
    if(success == true)
    {
        success = openIsStream(inputFileName,fzInput);
        if(success == false)
        {
            printf("problem opening input file!\n");
        }
    }

    // first find the line with data. Unfortunately ifstream won't allow a function to pass an open ifstream so need to do this manually in a given function every time
    // info for this problem found at: https://arstechnica.com/civis/viewtopic.php?f=20&t=145498
    if(success == true)
    {
        success = false;    // annoying logic to get the right line and the right boolean output
        std::string currentLine;
        size_t lineCount = 0;
        while(std::getline(fzInput,currentLine))
        {
            if(lineCount == varNameLine)
            {
                success = true;
                break;
            }
            lineCount = lineCount + 1;
        }
    }

    // okay now continue on in the ifstream as needed
    if(success == false)
    {
        printf("couldn't find varNameLine \"%zu\"!\n",varNameLine);
    } else
    {
        // should already be at the right line, should grab the next one. Don't allow comment lines in this section
        for(size_t lineCount = 0; lineCount < varCount+1; lineCount++)
        {
            if(varNameLine + lineCount <= inputFileTotalLines)  // stops errors if running one line more than is in file when trying out an extra line to see if too much data
            {
                std::string currentLine;
                std::getline(fzInput,currentLine);

                removeLeadingWhitespace(currentLine);
                removeEndingWhitespace(currentLine);

                bool conversionSuccess = true;
                // this setup here needs fixed, almost need to revamp all these loader functions!
                if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                {
                    conversionSuccess = isValidShapeFilename(currentLine,false);
                } else
                {
                    conversionSuccess = isValidShapeFilename(currentLine,true);
                }
                if(conversionSuccess == false)
                {
                    if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                    {
                        printf("problem converting \"%s\" value to filename!\n",currentLine.c_str());
                    }
                }

                if(conversionSuccess == false)
                {
                    if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                    {
                        printf("not enough polygon_ignit_shape_files for polygon_ignit_shape_files variable count!\n");
                        success = false;
                        break;
                    }
                } else
                {
                    if(lineCount >= varCount)   // should be checking to see if it is varCount+1, so one line extra
                    {
                        printf("too many polygon_ignit_shape_files for polygon_ignit_shape_files variable count!\n");
                        success = false;
                        break;
                    } else
                    {
                        inputVariableValueStorage->add_polygonIgnitShapeFile(currentLine);   // only adds files if it is a valid line of data and is valid data
                    }
                }
            }
        }
        closeIsStream(fzInput);
    }

    return success;
}

bool inputParser::load_GeoMAC_fire_perimeter_files(std::string inputFileName, size_t varCount, size_t varNameLine)
{
    bool success = true;

    if(inputFileTotalLines < varCount + varNameLine)
    {
        printf("not enough lines in input file for GeoMAC_fire_perimeter_files count of \"%zu\"!\n",varCount);
        success = false;
    }

    std::ifstream fzInput;
    if(success == true)
    {
        success = openIsStream(inputFileName,fzInput);
        if(success == false)
        {
            printf("problem opening input file!\n");
        }
    }

    // first find the line with data. Unfortunately ifstream won't allow a function to pass an open ifstream so need to do this manually in a given function every time
    // info for this problem found at: https://arstechnica.com/civis/viewtopic.php?f=20&t=145498
    if(success == true)
    {
        success = false;    // annoying logic to get the right line and the right boolean output
        std::string currentLine;
        size_t lineCount = 0;
        while(std::getline(fzInput,currentLine))
        {
            if(lineCount == varNameLine)
            {
                success = true;
                break;
            }
            lineCount = lineCount + 1;
        }
    }

    // okay now continue on in the ifstream as needed
    if(success == false)
    {
        printf("couldn't find varNameLine \"%zu\"!\n",varNameLine);
    } else
    {
        // should already be at the right line, should grab the next one. Don't allow comment lines in this section
        for(size_t lineCount = 0; lineCount < varCount+1; lineCount++)
        {
            if(varNameLine + lineCount <= inputFileTotalLines)  // stops errors if running one line more than is in file when trying out an extra line to see if too much data
            {
                std::string currentLine;
                std::getline(fzInput,currentLine);

                removeLeadingWhitespace(currentLine);
                removeEndingWhitespace(currentLine);

                bool conversionSuccess = true;
                // this setup here needs fixed, almost need to revamp all these loader functions!
                if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                {
                    conversionSuccess = isValidShapeFilename(currentLine,false);
                } else
                {
                    conversionSuccess = isValidShapeFilename(currentLine,true);
                }
                if(conversionSuccess == false)
                {
                    if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                    {
                        printf("problem converting \"%s\" value to filename!\n",currentLine.c_str());
                    }
                }

                if(conversionSuccess == false)
                {
                    if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                    {
                        printf("not enough GeoMAC_fire_perimeter_files for GeoMAC_fire_perimeter_files variable count!\n");
                        success = false;
                        break;
                    }
                } else
                {
                    if(lineCount >= varCount)   // should be checking to see if it is varCount+1, so one line extra
                    {
                        printf("too many GeoMAC_fire_perimeter_files for GeoMAC_fire_perimeter_files variable count!\n");
                        success = false;
                        break;
                    } else
                    {
                        inputVariableValueStorage->add_GeoMACfirePerimeterFile(currentLine);   // only adds files if it is a valid line of data and is valid data
                    }
                }
            }
        }
        closeIsStream(fzInput);
    }

    return success;
}

bool inputParser::load_farsite_output_fire_perimeter_files(std::string inputFileName, size_t varCount, size_t varNameLine)
{
    bool success = true;

    if(inputFileTotalLines < varCount + varNameLine)
    {
        printf("not enough lines in input file for farsite_output_fire_perimeter_files count of \"%zu\"!\n",varCount);
        success = false;
    }

    std::ifstream fzInput;
    if(success == true)
    {
        success = openIsStream(inputFileName,fzInput);
        if(success == false)
        {
            printf("problem opening input file!\n");
        }
    }

    // first find the line with data. Unfortunately ifstream won't allow a function to pass an open ifstream so need to do this manually in a given function every time
    // info for this problem found at: https://arstechnica.com/civis/viewtopic.php?f=20&t=145498
    if(success == true)
    {
        success = false;    // annoying logic to get the right line and the right boolean output
        std::string currentLine;
        size_t lineCount = 0;
        while(std::getline(fzInput,currentLine))
        {
            if(lineCount == varNameLine)
            {
                success = true;
                break;
            }
            lineCount = lineCount + 1;
        }
    }

    // okay now continue on in the ifstream as needed
    if(success == false)
    {
        printf("couldn't find varNameLine \"%zu\"!\n",varNameLine);
    } else
    {
        // should already be at the right line, should grab the next one. Don't allow comment lines in this section
        for(size_t lineCount = 0; lineCount < varCount+1; lineCount++)
        {
            if(varNameLine + lineCount <= inputFileTotalLines)  // stops errors if running one line more than is in file when trying out an extra line to see if too much data
            {
                std::string currentLine;
                std::getline(fzInput,currentLine);

                removeLeadingWhitespace(currentLine);
                removeEndingWhitespace(currentLine);

                bool conversionSuccess = true;
                // this setup here needs fixed, almost need to revamp all these loader functions!
                if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                {
                    conversionSuccess = isValidShapeFilename(currentLine,false);
                } else
                {
                    conversionSuccess = isValidShapeFilename(currentLine,true);
                }
                if(conversionSuccess == false)
                {
                    if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                    {
                        printf("problem converting \"%s\" value to filename!\n",currentLine.c_str());
                    }
                }

                if(conversionSuccess == false)
                {
                    if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                    {
                        printf("not enough farsite_output_fire_perimeter_files for farsite_output_fire_perimeter_files variable count!\n");
                        success = false;
                        break;
                    }
                } else
                {
                    if(lineCount >= varCount)   // should be checking to see if it is varCount+1, so one line extra
                    {
                        printf("too many farsite_output_fire_perimeter_files for farsite_output_fire_perimeter_files variable count!\n");
                        success = false;
                        break;
                    } else
                    {
                        inputVariableValueStorage->add_farsiteOutputFirePerimeterFile(currentLine);   // only adds files if it is a valid line of data and is valid data
                    }
                }
            }
        }
        closeIsStream(fzInput);
    }

    return success;
}

bool inputParser::load_wrf_files(std::string inputFileName, size_t varCount, size_t varNameLine)
{
    bool success = true;

    if(inputFileTotalLines < varCount + varNameLine)
    {
        printf("not enough lines in input file for wrf_files count of \"%zu\"!\n",varCount);
        success = false;
    }

    std::ifstream fzInput;
    if(success == true)
    {
        success = openIsStream(inputFileName,fzInput);
        if(success == false)
        {
            printf("problem opening input file!\n");
        }
    }

    // first find the line with data. Unfortunately ifstream won't allow a function to pass an open ifstream so need to do this manually in a given function every time
    // info for this problem found at: https://arstechnica.com/civis/viewtopic.php?f=20&t=145498
    if(success == true)
    {
        success = false;    // annoying logic to get the right line and the right boolean output
        std::string currentLine;
        size_t lineCount = 0;
        while(std::getline(fzInput,currentLine))
        {
            if(lineCount == varNameLine)
            {
                success = true;
                break;
            }
            lineCount = lineCount + 1;
        }
    }

    // okay now continue on in the ifstream as needed
    if(success == false)
    {
        printf("couldn't find varNameLine \"%zu\"!\n",varNameLine);
    } else
    {
        // should already be at the right line, should grab the next one. Don't allow comment lines in this section
        for(size_t lineCount = 0; lineCount < varCount+1; lineCount++)
        {
            if(varNameLine + lineCount <= inputFileTotalLines)  // stops errors if running one line more than is in file when trying out an extra line to see if too much data
            {
                std::string currentLine;
                std::getline(fzInput,currentLine);

                removeLeadingWhitespace(currentLine);
                removeEndingWhitespace(currentLine);

                bool conversionSuccess = true;
                // this setup here needs fixed, almost need to revamp all these loader functions!
                if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                {
                    conversionSuccess = isValidNetCDFFilename(currentLine,false);
                } else
                {
                    conversionSuccess = isValidNetCDFFilename(currentLine,true);
                }
                if(conversionSuccess == false)
                {
                    if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                    {
                        printf("problem converting \"%s\" value to filename!\n",currentLine.c_str());
                    }
                }

                if(conversionSuccess == false)
                {
                    if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                    {
                        printf("not enough wrf_files for wrf_files variable count!\n");
                        success = false;
                        break;
                    }
                } else
                {
                    if(lineCount >= varCount)   // should be checking to see if it is varCount+1, so one line extra
                    {
                        printf("too many wrf_files for wrf_files variable count!\n");
                        success = false;
                        break;
                    } else
                    {
                        inputVariableValueStorage->add_wrfFile(currentLine);   // only adds files if it is a valid line of data and is valid data
                    }
                }
            }
        }
        closeIsStream(fzInput);
    }

    return success;
}

bool inputParser::load_additional_WindNinja_outputs_google(std::string inputFileName, size_t varCount, size_t varNameLine)
{
    bool success = true;

    if(inputFileTotalLines < varCount + varNameLine)
    {
        printf("not enough lines in input file for additional_WindNinja_outputs_google count of \"%zu\"!\n",varCount);
        success = false;
    }

    std::ifstream fzInput;
    if(success == true)
    {
        success = openIsStream(inputFileName,fzInput);
        if(success == false)
        {
            printf("problem opening input file!\n");
        }
    }

    // first find the line with data. Unfortunately ifstream won't allow a function to pass an open ifstream so need to do this manually in a given function every time
    // info for this problem found at: https://arstechnica.com/civis/viewtopic.php?f=20&t=145498
    if(success == true)
    {
        success = false;    // annoying logic to get the right line and the right boolean output
        std::string currentLine;
        size_t lineCount = 0;
        while(std::getline(fzInput,currentLine))
        {
            if(lineCount == varNameLine)
            {
                success = true;
                break;
            }
            lineCount = lineCount + 1;
        }
    }

    // okay now continue on in the ifstream as needed
    if(success == false)
    {
        printf("couldn't find varNameLine \"%zu\"!\n",varNameLine);
    } else
    {
        // should already be at the right line, should grab the next one. Don't allow comment lines in this section
        for(size_t lineCount = 0; lineCount < varCount+1; lineCount++)
        {
            if(varNameLine + lineCount <= inputFileTotalLines)  // stops errors if running one line more than is in file when trying out an extra line to see if too much data
            {
                std::string currentLine;
                std::getline(fzInput,currentLine);

                removeLeadingWhitespace(currentLine);
                removeEndingWhitespace(currentLine);

                bool isValue = true;
                std::vector<std::string> foundValues;
                size_t startValueSpot = 0;
                for(size_t charIdx = 0; charIdx < currentLine.length(); charIdx++)
                {
                    std::string currentChr = currentLine.substr(charIdx,1);
                    if(currentChr == " " && isValue == true)
                    {
                        foundValues.push_back(currentLine.substr(startValueSpot,charIdx-startValueSpot));
                        isValue = false;
                    }
                    if(currentChr != " " && isValue == false)
                    {
                        startValueSpot = charIdx;
                        isValue = true;
                    }
                    if(charIdx == currentLine.length()-1 && isValue == true)
                    {
                        foundValues.push_back(currentLine.substr(startValueSpot,charIdx-startValueSpot+1));
                    }
                }

                /*printf("found values are:");
                for(size_t valIdx = 0; valIdx < foundValues.size(); valIdx++)
                {
                    printf(" \"%s\"",foundValues[valIdx].c_str());
                }
                printf("\n\n");*/
                std::string found_wrf_file_name = "";
                bool found_write_wx_model_goog_output = false;
                bool found_write_goog_output = false;
                double found_goog_out_resolution = 0;
                std::string found_units_goog_out_resolution = "";
                std::string found_goog_out_color_scheme = "";
                bool found_goog_out_vector_scaling = false;
                bool conversionSuccess = true;
                if(foundValues.size() != 7)
                {
                    if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                    {
                        printf("not enough values found in additional_WindNinja_outputs_google!\n");
                    }
                    conversionSuccess = false;
                } else
                {
                    bool isNetCDFfile = false;
                    // this setup here needs fixed, almost need to revamp all these loader functions!
                    if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                    {
                        isNetCDFfile = isValidNetCDFFilename(foundValues[0],false);
                    } else
                    {
                        isNetCDFfile = isValidNetCDFFilename(foundValues[0],true);
                    }
                    if(isNetCDFfile == false)
                    {
                        if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                        {
                            printf("problem converting \"%s\" value to filename!\n",foundValues[0].c_str());
                        }
                        conversionSuccess = false;
                    } else
                    {
                        found_wrf_file_name = foundValues[0];
                    }
                    bool isBool = false;
                    found_write_wx_model_goog_output = strToBool(foundValues[1],isBool);
                    if(isBool == false)
                    {
                        if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                        {
                            printf("problem converting \"%s\" value to bool!\n",foundValues[1].c_str());
                        }
                        conversionSuccess = false;
                    }
                    isBool = false;
                    found_write_goog_output = strToBool(foundValues[2],isBool);
                    if(isBool == false)
                    {
                        if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                        {
                            printf("problem converting \"%s\" value to bool!\n",foundValues[2].c_str());
                        }
                        conversionSuccess = false;
                    }
                    bool isDouble = false;
                    found_goog_out_resolution = strToDbl(foundValues[3],isDouble);
                    if(isDouble == false)
                    {
                        if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                        {
                            printf("problem converting \"%s\" value to double!\n",foundValues[3].c_str());
                        }
                        conversionSuccess = false;
                    } else
                    {
                        if(found_goog_out_resolution < 0)
                        {
                            if(foundValues[3] != "-1")
                            {
                                if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                                {
                                    printf("goog_out_resolution \"%s\" is negative but not of value \"-1\"!\n",foundValues[3].c_str());
                                }
                                conversionSuccess = false;
                            }
                        }
                    }
                    bool isGoodString = isValidString(foundValues[4],"units_goog_out_resolution");
                    if(isGoodString == false)
                    {
                        if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                        {
                            printf("\"%s\" value is not a valid units_goog_out_resolution string value!\n",foundValues[4].c_str());
                        }
                        conversionSuccess = false;
                    } else
                    {
                        found_units_goog_out_resolution = foundValues[4];
                    }
                    isGoodString = isValidString(foundValues[5],"goog_out_color_scheme");
                    if(isGoodString == false)
                    {
                        if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                        {
                            printf("\"%s\" value is not a valid goog_out_color_scheme string value!\n",foundValues[5].c_str());
                        }
                        conversionSuccess = false;
                    } else
                    {
                        found_goog_out_color_scheme = foundValues[5];
                    }
                    isBool = false;
                    found_goog_out_vector_scaling = strToBool(foundValues[6],isBool);
                    if(isBool == false)
                    {
                        if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                        {
                            printf("problem converting \"%s\" value to bool!\n",foundValues[6].c_str());
                        }
                        conversionSuccess = false;
                    }
                }

                if(conversionSuccess == false)
                {
                    if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                    {
                        printf("not enough additional_WindNinja_outputs_google for additional_WindNinja_outputs_google variable count!\n");
                        success = false;
                        break;
                    }
                } else
                {
                    if(lineCount >= varCount)   // should be checking to see if it is varCount+1, so one line extra
                    {
                        printf("too many additional_WindNinja_outputs_google for additional_WindNinja_outputs_google variable count!\n");
                        success = false;
                        break;
                    } else
                    {
                        inputVariableValueStorage->add_additionalWindNinjaOutputs_googleValueSet(found_wrf_file_name,found_write_wx_model_goog_output,found_write_goog_output,
                                                                                                 found_goog_out_resolution,found_units_goog_out_resolution,found_goog_out_color_scheme,
                                                                                                 found_goog_out_vector_scaling);   // only adds files if it is a valid line of data and is valid data
                    }
                }
            }
        }
        closeIsStream(fzInput);
    }

    return success;
}

bool inputParser::load_additional_WindNinja_outputs_shapefile(std::string inputFileName, size_t varCount, size_t varNameLine)
{
    bool success = true;

    if(inputFileTotalLines < varCount + varNameLine)
    {
        printf("not enough lines in input file for additional_WindNinja_outputs_shapefile count of \"%zu\"!\n",varCount);
        success = false;
    }

    std::ifstream fzInput;
    if(success == true)
    {
        success = openIsStream(inputFileName,fzInput);
        if(success == false)
        {
            printf("problem opening input file!\n");
        }
    }

    // first find the line with data. Unfortunately ifstream won't allow a function to pass an open ifstream so need to do this manually in a given function every time
    // info for this problem found at: https://arstechnica.com/civis/viewtopic.php?f=20&t=145498
    if(success == true)
    {
        success = false;    // annoying logic to get the right line and the right boolean output
        std::string currentLine;
        size_t lineCount = 0;
        while(std::getline(fzInput,currentLine))
        {
            if(lineCount == varNameLine)
            {
                success = true;
                break;
            }
            lineCount = lineCount + 1;
        }
    }

    // okay now continue on in the ifstream as needed
    if(success == false)
    {
        printf("couldn't find varNameLine \"%zu\"!\n",varNameLine);
    } else
    {
        // should already be at the right line, should grab the next one. Don't allow comment lines in this section
        for(size_t lineCount = 0; lineCount < varCount+1; lineCount++)
        {
            if(varNameLine + lineCount <= inputFileTotalLines)  // stops errors if running one line more than is in file when trying out an extra line to see if too much data
            {
                std::string currentLine;
                std::getline(fzInput,currentLine);

                removeLeadingWhitespace(currentLine);
                removeEndingWhitespace(currentLine);

                bool isValue = true;
                std::vector<std::string> foundValues;
                size_t startValueSpot = 0;
                for(size_t charIdx = 0; charIdx < currentLine.length(); charIdx++)
                {
                    std::string currentChr = currentLine.substr(charIdx,1);
                    if(currentChr == " " && isValue == true)
                    {
                        foundValues.push_back(currentLine.substr(startValueSpot,charIdx-startValueSpot));
                        isValue = false;
                    }
                    if(currentChr != " " && isValue == false)
                    {
                        startValueSpot = charIdx;
                        isValue = true;
                    }
                    if(charIdx == currentLine.length()-1 && isValue == true)
                    {
                        foundValues.push_back(currentLine.substr(startValueSpot,charIdx-startValueSpot+1));
                    }
                }

                /*printf("found values are:");
                for(size_t valIdx = 0; valIdx < foundValues.size(); valIdx++)
                {
                    printf(" \"%s\"",foundValues[valIdx].c_str());
                }
                printf("\n\n");*/
                std::string found_wrf_file_name = "";
                bool found_write_wx_model_shapefile_output = false;
                bool found_write_shapefile_output = false;
                double found_shape_out_resolution = 0;
                std::string found_units_shape_out_resolution = "";
                bool conversionSuccess = true;
                if(foundValues.size() != 5)
                {
                    if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                    {
                        printf("not enough values found in additional_WindNinja_outputs_shapefile!\n");
                    }
                    conversionSuccess = false;
                } else
                {
                    bool isNetCDFfile = false;
                    // this setup here needs fixed, almost need to revamp all these loader functions!
                    if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                    {
                        isNetCDFfile = isValidNetCDFFilename(foundValues[0],false);
                    } else
                    {
                        isNetCDFfile = isValidNetCDFFilename(foundValues[0],true);
                    }
                    if(isNetCDFfile == false)
                    {
                        if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                        {
                            printf("problem converting \"%s\" value to filename!\n",foundValues[0].c_str());
                        }
                        conversionSuccess = false;
                    } else
                    {
                        found_wrf_file_name = foundValues[0];
                    }
                    bool isBool = false;
                    found_write_wx_model_shapefile_output = strToBool(foundValues[1],isBool);
                    if(isBool == false)
                    {
                        if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                        {
                            printf("problem converting \"%s\" value to bool!\n",foundValues[1].c_str());
                        }
                        conversionSuccess = false;
                    }
                    isBool = false;
                    found_write_shapefile_output = strToBool(foundValues[2],isBool);
                    if(isBool == false)
                    {
                        if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                        {
                            printf("problem converting \"%s\" value to bool!\n",foundValues[2].c_str());
                        }
                        conversionSuccess = false;
                    }
                    bool isDouble = false;
                    found_shape_out_resolution = strToDbl(foundValues[3],isDouble);
                    if(isDouble == false)
                    {
                        if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                        {
                            printf("problem converting \"%s\" value to double!\n",foundValues[3].c_str());
                        }
                        conversionSuccess = false;
                    } else
                    {
                        if(found_shape_out_resolution < 0)
                        {
                            if(foundValues[3] != "-1")
                            {
                                if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                                {
                                    printf("found_shape_out_resolution \"%s\" is negative but not of value \"-1\"!\n",foundValues[3].c_str());
                                }
                                conversionSuccess = false;
                            }
                        }
                    }
                    bool isGoodString = isValidString(foundValues[4],"units_shape_out_resolution");
                    if(isGoodString == false)
                    {
                        if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                        {
                            printf("\"%s\" value is not a valid units_shape_out_resolution string value!\n",foundValues[4].c_str());
                        }
                        conversionSuccess = false;
                    } else
                    {
                        found_units_shape_out_resolution = foundValues[4];
                    }
                }

                if(conversionSuccess == false)
                {
                    if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                    {
                        printf("not enough additional_WindNinja_outputs_shapefile for additional_WindNinja_outputs_shapefile variable count!\n");
                        success = false;
                        break;
                    }
                } else
                {
                    if(lineCount >= varCount)   // should be checking to see if it is varCount+1, so one line extra
                    {
                        printf("too many additional_WindNinja_outputs_shapefile for additional_WindNinja_outputs_shapefile variable count!\n");
                        success = false;
                        break;
                    } else
                    {
                        inputVariableValueStorage->add_additionalWindNinjaOutputs_shapefileValueSet(found_wrf_file_name,found_write_wx_model_shapefile_output,found_write_shapefile_output,
                                                                                                    found_shape_out_resolution,found_units_shape_out_resolution);   // only adds files if it is a valid line of data and is valid data
                    }
                }
            }
        }
        closeIsStream(fzInput);
    }

    return success;
}

bool inputParser::load_additional_WindNinja_outputs_pdf(std::string inputFileName, size_t varCount, size_t varNameLine)
{
    bool success = true;

    if(inputFileTotalLines < varCount + varNameLine)
    {
        printf("not enough lines in input file for additional_WindNinja_outputs_pdf count of \"%zu\"!\n",varCount);
        success = false;
    }

    std::ifstream fzInput;
    if(success == true)
    {
        success = openIsStream(inputFileName,fzInput);
        if(success == false)
        {
            printf("problem opening input file!\n");
        }
    }

    // first find the line with data. Unfortunately ifstream won't allow a function to pass an open ifstream so need to do this manually in a given function every time
    // info for this problem found at: https://arstechnica.com/civis/viewtopic.php?f=20&t=145498
    if(success == true)
    {
        success = false;    // annoying logic to get the right line and the right boolean output
        std::string currentLine;
        size_t lineCount = 0;
        while(std::getline(fzInput,currentLine))
        {
            if(lineCount == varNameLine)
            {
                success = true;
                break;
            }
            lineCount = lineCount + 1;
        }
    }

    // okay now continue on in the ifstream as needed
    if(success == false)
    {
        printf("couldn't find varNameLine \"%zu\"!\n",varNameLine);
    } else
    {
        // should already be at the right line, should grab the next one. Don't allow comment lines in this section
        for(size_t lineCount = 0; lineCount < varCount+1; lineCount++)
        {
            if(varNameLine + lineCount <= inputFileTotalLines)  // stops errors if running one line more than is in file when trying out an extra line to see if too much data
            {
                std::string currentLine;
                std::getline(fzInput,currentLine);

                removeLeadingWhitespace(currentLine);
                removeEndingWhitespace(currentLine);

                bool isValue = true;
                std::vector<std::string> foundValues;
                size_t startValueSpot = 0;
                for(size_t charIdx = 0; charIdx < currentLine.length(); charIdx++)
                {
                    std::string currentChr = currentLine.substr(charIdx,1);
                    if(currentChr == " " && isValue == true)
                    {
                        foundValues.push_back(currentLine.substr(startValueSpot,charIdx-startValueSpot));
                        isValue = false;
                    }
                    if(currentChr != " " && isValue == false)
                    {
                        startValueSpot = charIdx;
                        isValue = true;
                    }
                    if(charIdx == currentLine.length()-1 && isValue == true)
                    {
                        foundValues.push_back(currentLine.substr(startValueSpot,charIdx-startValueSpot+1));
                    }
                }

                /*printf("found values are:");
                for(size_t valIdx = 0; valIdx < foundValues.size(); valIdx++)
                {
                    printf(" \"%s\"",foundValues[valIdx].c_str());
                }
                printf("\n\n");*/
                std::string found_wrf_file_name = "";
                bool found_write_pdf_output = false;
                double found_pdf_out_resolution = 0;
                std::string found_units_pdf_out_resolution = "";
                double found_pdf_linewidth = 0;
                std::string found_pdf_basemap = "";
                double found_pdf_height = 0.0;
                double found_pdf_width = 0.0;
                std::string found_pdf_size = "";
                bool conversionSuccess = true;
                if(foundValues.size() != 9)
                {
                    if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                    {
                        printf("not enough values found in additional_WindNinja_outputs_pdf!\n");
                    }
                    conversionSuccess = false;
                } else
                {
                    bool isNetCDFfile = false;
                    // this setup here needs fixed, almost need to revamp all these loader functions!
                    if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                    {
                        isNetCDFfile = isValidNetCDFFilename(foundValues[0],false);
                    } else
                    {
                        isNetCDFfile = isValidNetCDFFilename(foundValues[0],true);
                    }
                    if(isNetCDFfile == false)
                    {
                        if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                        {
                            printf("problem converting \"%s\" value to filename!\n",foundValues[0].c_str());
                        }
                        conversionSuccess = false;
                    } else
                    {
                        found_wrf_file_name = foundValues[0];
                    }
                    bool isBool = false;
                    found_write_pdf_output = strToBool(foundValues[1],isBool);
                    if(isBool == false)
                    {
                        if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                        {
                            printf("problem converting \"%s\" value to bool!\n",foundValues[1].c_str());
                        }
                        conversionSuccess = false;
                    }
                    bool isDouble = false;
                    found_pdf_out_resolution = strToInt(foundValues[2],isDouble);
                    if(isDouble == false)
                    {
                        if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                        {
                            printf("problem converting \"%s\" value to double!\n",foundValues[2].c_str());
                        }
                        conversionSuccess = false;
                    } else
                    {
                        if(found_pdf_out_resolution < 0)
                        {
                            if(foundValues[2] != "-1")
                            {
                                if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                                {
                                    printf("found_pdf_out_resolution \"%s\" is negative but not of value \"-1\"!\n",foundValues[2].c_str());
                                }
                                conversionSuccess = false;
                            }
                        }
                    }
                    bool isGoodString = isValidString(foundValues[3],"units_pdf_out_resolution");
                    if(isGoodString == false)
                    {
                        if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                        {
                            printf("\"%s\" value is not a valid units_pdf_out_resolution string value!\n",foundValues[3].c_str());
                        }
                        conversionSuccess = false;
                    } else
                    {
                        found_units_pdf_out_resolution = foundValues[3];
                    }
                    bool isPositiveDouble = false;
                    found_pdf_linewidth = strToPositiveDbl(foundValues[4],isPositiveDouble);
                    if(isPositiveDouble == false)
                    {
                        if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                        {
                            printf("problem converting \"%s\" value to positive double!\n",foundValues[4].c_str());
                        }
                        conversionSuccess = false;
                    }
                    isGoodString = false;
                    isGoodString = isValidString(foundValues[5],"pdf_basemap");
                    if(isGoodString == false)
                    {
                        if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                        {
                            printf("\"%s\" value is not a valid pdf_basemap string value!\n",foundValues[5].c_str());
                        }
                        conversionSuccess = false;
                    } else
                    {
                        found_pdf_basemap = foundValues[5];
                    }
                    isPositiveDouble = false;
                    found_pdf_height = strToPositiveDbl(foundValues[6],isPositiveDouble);
                    if(isPositiveDouble == false)
                    {
                        if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                        {
                            printf("problem converting \"%s\" value to positive double!\n",foundValues[6].c_str());
                        }
                        conversionSuccess = false;
                    }
                    isPositiveDouble = false;
                    found_pdf_width = strToPositiveDbl(foundValues[7],isPositiveDouble);
                    if(isPositiveDouble == false)
                    {
                        if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                        {
                            printf("problem converting \"%s\" value to positive double!\n",foundValues[7].c_str());
                        }
                        conversionSuccess = false;
                    }
                    isGoodString = false;
                    isGoodString = isValidString(foundValues[8],"pdf_size");
                    if(isGoodString == false)
                    {
                        if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                        {
                            printf("\"%s\" value is not a valid pdf_size string value!\n",foundValues[8].c_str());
                        }
                        conversionSuccess = false;
                    } else
                    {
                        found_pdf_size = foundValues[8];
                    }
                }

                if(conversionSuccess == false)
                {
                    if(lineCount < varCount)    // only error if is a valid line as loop goes one extra to check for extra files
                    {
                        printf("not enough additional_WindNinja_outputs_pdf for additional_WindNinja_outputs_pdf variable count!\n");
                        success = false;
                        break;
                    }
                } else
                {
                    if(lineCount >= varCount)   // should be checking to see if it is varCount+1, so one line extra
                    {
                        printf("too many additional_WindNinja_outputs_pdf for additional_WindNinja_outputs_pdf variable count!\n");
                        success = false;
                        break;
                    } else
                    {
                        inputVariableValueStorage->add_additionalWindNinjaOutputs_pdfValueSet(found_wrf_file_name,found_write_pdf_output,found_pdf_out_resolution,
                                                                                              found_units_pdf_out_resolution,found_pdf_linewidth,found_pdf_basemap,
                                                                                              found_pdf_height,found_pdf_width,found_pdf_size);   // only adds files if it is a valid line of data and is valid data
                    }
                }
            }
        }
        closeIsStream(fzInput);
    }

    return success;
}
/*** end type count loader functions ***/




// temporary stuff, till improve parsing for type count loader functions
double inputParser::strToLat_Coord(std::string inputString, bool &conversionSuccess)
{
    double outputValue = 0.0;
    conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    if(isDbl(inputString) == true)
    {
        outputValue = atof(inputString.c_str());
        if(outputValue < -90.0 || outputValue > 90.0)
        {
            outputValue = 0.0;
            conversionSuccess = false;
            printf("inputString \"%s\" is not a lat_coord value!\n",inputString.c_str());
        }
    } else
    {
        conversionSuccess = false;
        printf("inputString \"%s\" is not a lat_coord value!\n",inputString.c_str());
    }

    return outputValue;
}

double inputParser::strToLong_Coord(std::string inputString, bool &conversionSuccess)
{
    double outputValue = 0.0;
    conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    if(isDbl(inputString) == true)
    {
        outputValue = atof(inputString.c_str());
        if(outputValue < -180.0 || outputValue > 180.0)
        {
            outputValue = 0.0;
            conversionSuccess = false;
            printf("inputString \"%s\" is not a long_coord value!\n",inputString.c_str());
        }
    } else
    {
        conversionSuccess = false;
        printf("inputString \"%s\" is not a long_coord value!\n",inputString.c_str());
    }

    return outputValue;
}

bool inputParser::isValidShapeFilename(std::string inputString, bool suppressWarnings)
{
    bool conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    if(inputString.length() < 5)
    {
        if(suppressWarnings == false)
        {
            printf("shape filename not long enough to have .shp extension!\n");
        }
        conversionSuccess = false;
    }
    if(conversionSuccess == true)
    {
        if(inputString.substr(inputString.length()-4,inputString.length()) != ".shp")
        {
            if(suppressWarnings == false)
            {
                printf("shape filename does not have .shp extension!\n");
            }
            conversionSuccess = false;
        }
    }
    if(conversionSuccess == true)
    {
        GDALDatasetH  hDS;
        hDS = GDALOpenEx( inputString.c_str(), GDAL_OF_VECTOR, NULL, NULL, NULL );
        if(hDS == NULL)
        {
            if(suppressWarnings == false)
            {
                printf("could not open string \"%s\" as shape file!\n",inputString.c_str());
            }
            conversionSuccess = false;
        } else
        {
            /* tutorial stuff /

            // getting dataset information
            OGRLayerH hLayer;
            OGRFeatureH hFeature;
            OGRFeatureDefnH hFDefn;

            hLayer = GDALDatasetGetLayerByName( hDS, "point" );
            hFDefn = OGR_L_GetLayerDefn(hLayer);
            OGR_L_ResetReading(hLayer);
            while( (hFeature = OGR_L_GetNextFeature(hLayer)) != NULL )
            {
                int iField;
                OGRGeometryH hGeometry;
                for( iField = 0; iField < OGR_FD_GetFieldCount(hFDefn); iField++ )
                {
                    OGRFieldDefnH hFieldDefn = OGR_FD_GetFieldDefn( hFDefn, iField );
                    switch( OGR_Fld_GetType(hFieldDefn) )
                    {
                        case OFTInteger:
                            printf( "%d,", OGR_F_GetFieldAsInteger( hFeature, iField ) );
                            break;
                        case OFTInteger64:
                            printf( CPL_FRMT_GIB ",", OGR_F_GetFieldAsInteger64( hFeature, iField ) );
                            break;
                        case OFTReal:
                            printf( "%.3f,", OGR_F_GetFieldAsDouble( hFeature, iField) );
                            break;
                        case OFTString:
                            printf( "%s,", OGR_F_GetFieldAsString( hFeature, iField) );
                            break;
                        default:
                            printf( "%s,", OGR_F_GetFieldAsString( hFeature, iField) );
                            break;
                    }
                }
                hGeometry = OGR_F_GetGeometryRef(hFeature);
                if( hGeometry != NULL
                    && wkbFlatten(OGR_G_GetGeometryType(hGeometry)) == wkbPoint )
                {
                    printf( "%.3f,%3.f\n", OGR_G_GetX(hGeometry, 0), OGR_G_GetY(hGeometry, 0) );
                }
                else
                {
                    printf( "no point geometry\n" );
                }
                OGR_F_Destroy( hFeature );
            }
            GDALClose( hDS );

            / end tutorial stuff */
        }
        GDALClose( hDS );
    }
    // need to check for all other file types that go along with, or does gdal already deal with that type of problem?

    return conversionSuccess;
}

bool inputParser::isValidNetCDFFilename(std::string inputString, bool suppressWarnings)
{
    bool conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    GDALDatasetH  hDataset;
    std::string netCDFFileString = "NETCDF:\"" + inputString + "\"";
    CPLPushErrorHandler(&CPLQuietErrorHandler);
    hDataset = GDALOpen( netCDFFileString.c_str(), GA_ReadOnly );
    CPLPopErrorHandler();
    if(hDataset == NULL)
    {
        if(suppressWarnings == false)
        {
            printf("could not open string \"%s\" as netcdf file!\n",inputString.c_str());
        }
        conversionSuccess = false;
        GDALClose(hDataset);
    } else
    {
        /* tutorial stuff /

        // getting dataset information
        GDALDriverH   hDriver;
        double        adfGeoTransform[6];
        printf( "Driver: %s/%s\n",
                GDALGetDriverShortName( hDriver ),
                GDALGetDriverLongName( hDriver ) );
        printf( "Size is %dx%dx%d\n",
                GDALGetRasterXSize( hDataset ),
                GDALGetRasterYSize( hDataset ),
                GDALGetRasterCount( hDataset ) );
        if( GDALGetProjectionRef( hDataset ) != NULL )
            printf( "Projection is `%s'\n", GDALGetProjectionRef( hDataset ) );
        if( GDALGetGeoTransform( hDataset, adfGeoTransform ) == CE_None )
        {
            printf( "Origin = (%.6f,%.6f)\n",
                    adfGeoTransform[0], adfGeoTransform[3] );
            printf( "Pixel Size = (%.6f,%.6f)\n",
                    adfGeoTransform[1], adfGeoTransform[5] );
        }
        // finished getting dataset information

        // fetching a band
        GDALRasterBandH hBand;
        int             nBlockXSize, nBlockYSize;
        int             bGotMin, bGotMax;
        double          adfMinMax[2];
        hBand = GDALGetRasterBand( hDataset, 1 );
        GDALGetBlockSize( hBand, &nBlockXSize, &nBlockYSize );
        printf( "Block=%dx%d Type=%s, ColorInterp=%s\n",
                nBlockXSize, nBlockYSize,
                GDALGetDataTypeName(GDALGetRasterDataType(hBand)),
                GDALGetColorInterpretationName(
                    GDALGetRasterColorInterpretation(hBand)) );
        adfMinMax[0] = GDALGetRasterMinimum( hBand, &bGotMin );
        adfMinMax[1] = GDALGetRasterMaximum( hBand, &bGotMax );
        if( ! (bGotMin && bGotMax) )
            GDALComputeRasterMinMax( hBand, TRUE, adfMinMax );
        printf( "Min=%.3fd, Max=%.3f\n", adfMinMax[0], adfMinMax[1] );
        if( GDALGetOverviewCount(hBand) > 0 )
            printf( "Band has %d overviews.\n", GDALGetOverviewCount(hBand));
        if( GDALGetRasterColorTable( hBand ) != NULL )
            printf( "Band has a color table with %d entries.\n",
                     GDALGetColorEntryCount(
                         GDALGetRasterColorTable( hBand ) ) );
        // end fetching a band

        // reading raster data
        float *pafScanline;
        int   nXSize = GDALGetRasterBandXSize( hBand );
        pafScanline = (float *) CPLMalloc(sizeof(float)*nXSize);
        GDALRasterIO( hBand, GF_Read, 0, 0, nXSize, 1,
                      pafScanline, nXSize, 1, GDT_Float32,
                      0, 0 );
        // end reading raster data

        // closing dataset
        CPLFree(pafScanline);
        GDALClose(hDataset);
        // end closing dataset

        / end tutorial stuff */
    }
    GDALClose(hDataset);

    if(conversionSuccess == true)
    {
        /* WindNinja wrfSurfInitialization::checkForValidData() stuff */

        // important info about wrf files: https://www.ncl.ucar.edu/Applications/wrfnetcdf.shtml, http://www.openwfm.org/wiki/How_to_interpret_WRF_variables,
        //   http://www.meteo.unican.es/wiki/cordexwrf/OutputVariables, https://wiki.ucar.edu/display/acme/WRF+Ingest

        // setup desired variables from netcdf file list
        std::vector<std::string> wrf_var_list;
        wrf_var_list.push_back("T2"); // 2 m temperature, T by itself is all air temperatures in 3D space. Units are K.
        wrf_var_list.push_back("Q2"); // 2 m specific humidity. Units are 1, so I guess unitless. QVAPOR is the specific humidity for all 3D space
        wrf_var_list.push_back("RAINC"); // hourly precipitation in kg/(m^2*s), RAINC is ACCUMULATED TOTAL CUMULUS PRECIPITATION, RAINNC is ACCUMULATED TOTAL GRID SCALE PRECIPITATION so I think I need to add both to get total precipitation? Sounds like all the different precips from each different physics model technically need added together for this. For now just use RAINC. Also, accumulated is the right type?
        wrf_var_list.push_back("RAINNC");   // sounds like usually you get RAINC and RAINNC and add them both to get total hourly precip. No information on what RAINSH is, but might be another component, but not used so much so let's ignore it for now
        // technically don't need each of these RAINC and RAINNC, just if they exist or not, for now assume need both
        wrf_var_list.push_back("QCLOUD"); // WindNinja says this is the cloud water mixing ratio but uses it for the cloud cover grid. http://www.meteo.unican.es/wiki/cordexwrf/OutputVariables says it is the Column liquid water content?

        GDALDataset *srcDS;
        for(size_t wrfVarIdx = 0; wrfVarIdx < wrf_var_list.size(); wrfVarIdx++)
        {
            netCDFFileString = "NETCDF:\"" + inputString + "\":" + wrf_var_list[wrfVarIdx];
            CPLPushErrorHandler(&CPLQuietErrorHandler);
            srcDS = (GDALDataset*)GDALOpen( netCDFFileString.c_str(), GA_ReadOnly );
            CPLPopErrorHandler();
            if(srcDS == NULL)
            {
                if(suppressWarnings == false)
                {
                    printf("netcdf file \"%s\" does not have data of name \"%s\"!\n",inputString.c_str(),wrf_var_list[wrfVarIdx].c_str());
                }
                conversionSuccess = false;
            } else
            {
                //Get total bands (time steps)
                int nBands = srcDS->GetRasterCount();
                int nXSize = srcDS->GetRasterXSize();
                int nYSize = srcDS->GetRasterYSize();

                GDALRasterBand *poBand;
                double *padfScanline;
                //loop over all bands for this variable (bands are time steps)
                for(size_t bandIdx = 1; bandIdx <= nBands; bandIdx++)
                {
                    poBand = srcDS->GetRasterBand(bandIdx);

                    int pbSuccess = 0;
                    double dfNoData = poBand->GetNoDataValue( &pbSuccess );

                    bool noDataValueExists = false;
                    bool noDataIsNan = false;
                    if( pbSuccess == false )
                        noDataValueExists = false;
                    else
                    {
                        noDataValueExists = true;
                        noDataIsNan = CPLIsNan(dfNoData);
                    }

                    //set the data
                    padfScanline = new double[nXSize*nYSize];
                    pbSuccess = poBand->RasterIO(GF_Read, 0, 0, nXSize, nYSize, padfScanline,
                                     nXSize, nYSize, GDT_Float64, 0, 0);
                    if(pbSuccess == CE_Failure)
                    {
                        if(suppressWarnings == false)
                        {
                            printf("error reading raster band!\n");
                        }
                        conversionSuccess = false;
                        GDALClose((GDALDatasetH) srcDS );
                        break;
                    }
                    for(size_t dataIdx = 0; dataIdx < nXSize*nYSize; dataIdx++)
                    {
                        //Check if value is no data (if no data value was defined in file)
                        if(noDataValueExists == true)
                        {
                            if(noDataIsNan == true)
                            {
                                if(CPLIsNan(padfScanline[dataIdx]))
                                {
                                    if(suppressWarnings == false)
                                    {
                                        printf("netcdf file \"%s\" contains no_data values for data of name \"%s\"!\n",inputString.c_str(),wrf_var_list[wrfVarIdx].c_str());
                                    }
                                    conversionSuccess = false;
                                }
                            } else
                            {
                                if(padfScanline[dataIdx] == dfNoData)
                                {
                                    if(suppressWarnings == false)
                                    {
                                        printf("netcdf file \"%s\" contains no_data values for data of name \"%s\"!\n",inputString.c_str(),wrf_var_list[wrfVarIdx].c_str());
                                    }
                                    conversionSuccess = false;
                                }
                            }
                        }
                        if( wrf_var_list[wrfVarIdx] == "T2" )   //units are Kelvin
                        {
                            if(padfScanline[dataIdx] < 180.0 || padfScanline[dataIdx] > 340.0)  //these are near the most extreme temperatures ever recored on earth
                            {
                                if(suppressWarnings == false)
                                {
                                    printf("temperature is out of range for netcdf file \"%s\"!\n",inputString.c_str());
                                }
                                conversionSuccess = false;
                            }
                        } else if( wrf_var_list[wrfVarIdx] == "Q2" )  //units are unitless
                        {
                            if(padfScanline[dataIdx] < -0.0001 || padfScanline[dataIdx] > 100.0)
                            {
                                if(suppressWarnings == false)
                                {
                                    printf("specific humidity is out of range for netcdf file \"%s\"!\n",inputString.c_str());
                                }
                                conversionSuccess = false;
                            }
                        } else if( wrf_var_list[wrfVarIdx] == "RAINC" )  //units are kg/(m^2*s)
                        {
                            if(padfScanline[dataIdx] < -0.0001 || padfScanline[dataIdx] > 100.0)
                            {
                                if(suppressWarnings == false)
                                {
                                    printf("cumulus precipitation is out of range for netcdf file \"%s\"!\n",inputString.c_str());
                                }
                                conversionSuccess = false;
                            }
                        } else if( wrf_var_list[wrfVarIdx] == "RAINNC" )  //units are kg/(m^2*s)
                        {
                            if(padfScanline[dataIdx] < -0.0001 || padfScanline[dataIdx] > 100.0)
                            {
                                if(suppressWarnings == false)
                                {
                                    printf("grid scale precipitation is out of range for netcdf file \"%s\"!\n",inputString.c_str());
                                }
                                conversionSuccess = false;
                            }
                        } else if( wrf_var_list[wrfVarIdx] == "QCLOUD" )  //units are kg/kg
                        {
                            if(padfScanline[dataIdx] < -0.0001 || padfScanline[dataIdx] > 100.0)
                            {
                                if(suppressWarnings == false)
                                {
                                    printf("total cloud cover is out of range for netcdf file \"%s\"!\n",inputString.c_str());
                                }
                                conversionSuccess = false;
                            }
                        }
                    }

                    delete [] padfScanline;
                }
            }
            GDALClose((GDALDatasetH) srcDS );
        }

        /* end WindNinja wrfSurfInitialization::checkForValidData() stuff */
    }

    return conversionSuccess;
}

bool inputParser::strToBool(std::string inputString, bool &conversionSuccess)
{
    bool outputValue = false;
    conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    if(isBool(inputString) == true)
    {
        if(inputString == "true" || inputString == "1")
        {
            outputValue = true;
        } else if(inputString == "false" || inputString == "0")
        {
            outputValue = false;
        }
    } else
    {
        conversionSuccess = false;
        printf("inputString \"%s\" is not a bool value!\n",inputString.c_str());
    }

    return outputValue;
}

bool inputParser::isBool(std::string inputString)
{
    bool conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    if(inputString == "true" || inputString == "1")
    {
        conversionSuccess = true;
    } else if(inputString == "false" || inputString == "0")
    {
        conversionSuccess = true;
    } else
    {
        conversionSuccess = false;
    }

    return conversionSuccess;
}

double inputParser::strToDbl(std::string inputString, bool &conversionSuccess)
{
    double outputValue = 0.0;
    conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    if(isDbl(inputString) == true)
    {
        outputValue = atof(inputString.c_str());
    } else
    {
        conversionSuccess = false;
        printf("inputString \"%s\" is not a double value!\n",inputString.c_str());
    }

    return outputValue;
}

double inputParser::strToPositiveDbl(std::string inputString, bool &conversionSuccess)
{
    double outputValue = 0.0;
    conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    if(isDbl(inputString) == true && inputString.substr(0,1) != "-") // only difference between this and strToDbl()
    {
        outputValue = atof(inputString.c_str());
    } else
    {
        conversionSuccess = false;
        printf("inputString \"%s\" is not a positive double value!\n",inputString.c_str());
    }

    return outputValue;
}

bool inputParser::isDbl(std::string inputString)
{
    bool conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    size_t decimalCounter = 0;
    size_t digitCounter = 0;
    for(size_t charIdx = 0; charIdx < inputString.length(); charIdx++)
    {
        std::string currentChr = inputString.substr(charIdx,1);
        if(currentChr >= "0" || currentChr <= "9")
        {
            digitCounter = digitCounter + 1;
        } else
        {
            if(charIdx == 0)
            {
                if(inputString.substr(0,1) == "+" || inputString.substr(0,1) == "-")
                {
                     // is okay
                } else
                {
                    conversionSuccess = false;  // can't have "." for first char, only a number, +, or - sign
                    break;
                }
            } else
            {
                if(currentChr == ".")
                {
                    if(inputString.substr(charIdx-1,1) >= "0" || inputString.substr(charIdx-1,1) <= "9")
                    {
                        decimalCounter = decimalCounter + 1;
                        if(decimalCounter > 1)
                        {
                            conversionSuccess = false;  // only allow one decimal per string
                            break;
                        }
                    } else
                    {
                        conversionSuccess = false;  // need valid number before a decimal point (avoid hexadecimal stuff with no 0 . stuff)
                        break;
                    }
                } else
                {
                    conversionSuccess = false; // has to be a valid decimal point at this point
                    break;
                }
            }
        }
    }

    if(conversionSuccess == true)
    {
        if(digitCounter <= 0)
        {
            conversionSuccess = false;  // got to this point somehow, but there wasn't actually any number stuff in the string
        }
    }

    return conversionSuccess;
}

bool inputParser::isPureFilenameString(std::string inputString)
{
    bool conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    // separator characters bug me, but at this point I think that only spaces should be disallowed
    for(size_t charIdx = 0; charIdx < inputString.length(); charIdx++)
    {
        std::string currentChr = inputString.substr(charIdx,1);
        if(currentChr == " " || currentChr == ":" || currentChr == "`" || currentChr == "~" || currentChr == "!" || currentChr == "@"
                || currentChr == "#" || currentChr == "$" || currentChr == "%" || currentChr == "^" || currentChr == "&" || currentChr == "*"
                || currentChr == "(" || currentChr == ")" || currentChr == "+" || currentChr == "=" || currentChr == "[" || currentChr == "]"
                || currentChr == "{" || currentChr == "}" || currentChr == "|" || currentChr == ";" || currentChr == "\'" || currentChr == "\""
                || currentChr == "," || currentChr == "<" || currentChr == ">" || currentChr == "?")    // probably an easier way to do this, but for now this works
        {
            conversionSuccess = false;
            break;
        }
    }

    return conversionSuccess;
}

bool inputParser::isValidString(std::string inputString, std::string varName)
{
    bool conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    // ugh, have to be insanely specific with string input

    if(varName == "run_base_name")
    {
        if(isPureFilenameString(inputString) == false)
        {
            conversionSuccess = false;
        }
    } else if(varName == "createIgnition_output_units")
    {
        if(inputString != "english" && inputString != "metric" && inputString != "input")
        {
            conversionSuccess = false;
        }
    } else if(varName == "wrfGetWeather_output_units")
    {
        if(inputString != "english" && inputString != "metric")
        {
            conversionSuccess = false;
        }
    } else if(varName == "WindNinja_required_output_units")
    {
        if(inputString != "english" && inputString != "metric")
        {
            conversionSuccess = false;
        }
    } else if(varName == "farsite_output_units")
    {
        if(inputString != "english" && inputString != "metric")
        {
            conversionSuccess = false;
        }
    } else if(varName == "lcp_download_buffer_units")
    {
        if(inputString != "kilometers" && inputString != "miles")
        {
            conversionSuccess = false;
        }
    } else if(varName == "WindNinja_mesh_choice")
    {
        if(inputString != "coarse" && inputString != "medium" && inputString != "fine" && inputString != "custom")
        {
            conversionSuccess = false;
        }
    } else if(varName == "WindNinja_mesh_res_units")
    {
        if(inputString != "ft" && inputString != "m")
        {
            conversionSuccess = false;
        }
    } else if(varName == "farsite_crown_fire_method")
    {
        if(inputString != "Finney" && inputString != "Reinhart")
        {
            conversionSuccess = false;
        }
    } else if(varName == "wrf_file_name")
    {
        if(isPureFilenameString(inputString) == false)
        {
            conversionSuccess = false;
        }
    } else if(varName == "units_goog_out_resolution")
    {
        if(inputString != "ft" && inputString != "m")
        {
            conversionSuccess = false;
        }
    } else if(varName == "goog_out_color_scheme")
    {
        if(inputString != "default" && inputString != "oranges" && inputString != "blues" && inputString != "greens"
                && inputString != "pinks" && inputString != "magic_beans" && inputString != "pink_to_green" && inputString != "ROPGW")
        {
            conversionSuccess = false;
        }
    } else if(varName == "units_shape_out_resolution")
    {
        if(inputString != "ft" && inputString != "m")
        {
            conversionSuccess = false;
        }
    } else if(varName == "units_pdf_out_resolution")
    {
        if(inputString != "ft" && inputString != "m")
        {
            conversionSuccess = false;
        }
    } else if(varName == "pdf_basemap")
    {
        if(inputString != "topofire" && inputString != "hillshade")   // not sure what other options are available
        {
            conversionSuccess = false;
        }
    } else if(varName == "pdf_size")
    {
        if(inputString != "letter" && inputString != "legal" && inputString != "tabloid")
        {
            conversionSuccess = false;
        }
    } else
    {
        printf("string verification for variable \"%s\" has not been implemented in the code yet!\n",varName.c_str());
        conversionSuccess = false;
    }

    if(conversionSuccess == false)
    {
        printf("count value \"%s\" is not valid for variable \"%s\"!\n",inputString.c_str(),varName.c_str());
    }

    return conversionSuccess;
}

int inputParser::strToInt(std::string inputString, bool &conversionSuccess)
{
    int outputValue = 0;
    conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    if(isInt(inputString) == true)
    {
        outputValue = atoi(inputString.c_str());
    } else
    {
        conversionSuccess = false;
        printf("inputString \"%s\" is not an int value!\n",inputString.c_str());
    }

    return outputValue;
}

bool inputParser::isInt(std::string inputString)
{
    bool conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    size_t digitCounter = 0;
    for(size_t charIdx = 0; charIdx < inputString.length(); charIdx++)
    {
        std::string currentChr = inputString.substr(charIdx,1);
        if(currentChr >= "0" || currentChr <= "9")
        {
            digitCounter = digitCounter + 1;
        } else
        {
            if(charIdx == 0)
            {
                if(inputString.substr(0,1) == "+" || inputString.substr(0,1) == "-")
                {
                     // is okay
                } else
                {
                    conversionSuccess = false;  // only time non number values are allowed is first char and only + or - signs
                    break;
                }
            } else
            {
                conversionSuccess = false; // only numbers allowed at this point
                break;
            }
        }
    }

    if(conversionSuccess == true)
    {
        if(digitCounter <= 0)
        {
            conversionSuccess = false;  // got to this point somehow, but there wasn't actually any number stuff in the string
        }
    }

    return conversionSuccess;
}
// end temporary stuff, till improve parsing for type count loader functions
