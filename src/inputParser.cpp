#include "inputParser.h"

/***** public functions *****/

/*** constructor functions ***/
inputParser::inputParser()
{

}
/*** end constructor functions ***/


/*** variable class getter functions ***/
void inputParser::loadVariableInfo(inputVariables_valueStorage* newInputVariables)
{
    inputVariableValueStorage = newInputVariables;  // is this the right thing to do with pointers?
    inputVariablesInfo = inputVariableValueStorage->get_inputVariableInfo();
}
/*** end variable class getter functions ***/

/*** main loader functions ***/
bool inputParser::findVariableNames(std::string inputFileName)
{
    bool success = true;

    std::ifstream fzInput;   // input file stream, using pointer to a FILE was not accepted by std::getline
    // run does file exist function on inputFileName. If not, warning is in function so exit. If so, open this input file as the file to pass around to all the loader functions
    if(doesFileExist(inputFileName,false) == false)
    {
        printf("failed to open input file!\n");
        success = false;
    } else
    {
        fzInput.open(inputFileName.c_str());
    }

    if(success == true)
    {
        for(size_t varIdx = 0; varIdx < inputVariablesInfo.size(); varIdx++)
        {
            // search in file for each variable name, just the name not the count
            // set variable in class to resulting bool of findVariable();
            // turns out the way to get the count and the variable name are basically the same, so I'm using the same function for both
            // just this time the return values will be ignored. Need to avoid loading anything till the setting of all variables is checked.
            std::string currentVarName = inputVariablesInfo[varIdx].get_variableName();
            std::string currentVarCountString = "";
            size_t varNameLine = 0;
            if(variableNameAndCountFinder(fzInput,currentVarName,currentVarCountString,varNameLine) == true)
            {
                inputVariablesInfo[varIdx].set_isFoundInInputFile(true);
            }   // no else, cause this lets all be optional. The checkSetVarNamesForConflictingOptions function in inputVariablesHandler is for actually choosing if is optional or not
        }
    }

    // close file and delete pointer
    fzInput.close();
    // do we need to delete pointer or does fclose do that?
    //delete(fzInput);

    return success;
}

bool inputParser::loadAllInputs(std::string inputFileName)
{
    bool success = true;

    std::ifstream fzInput;  // input file stream, using pointer to a FILE was not accepted by std::getline
    // run does file exist function on inputFileName. If not, warning is in function so exit. If so, open this input file as the file to pass around to all the loader functions
    if(doesFileExist(inputFileName,false) == false)
    {
        printf("failed to open input file!\n");
        success = false;
    } else
    {
        fzInput.open(inputFileName.c_str());
    }

    // now go through each input variable obtaining each variable count or determining which loader function to use and loading with the needed loader function
    if(success == true)
    {
        for(size_t varIdx = 0; varIdx < inputVariablesInfo.size(); varIdx++)
        {
            if(inputVariablesInfo[varIdx].get_isFoundInInputFile() == true)
            {
                // big if statement checking variableCountType and using the proper load function.
                //  If type is count, which comes last, then another big if statement using loader function names to determine which loader function to use.
                //   If it reaches the else, no loader function is used so give a warning to the user/programmer that it is not an allowed/required input,
                std::string currentVarName = inputVariablesInfo[varIdx].get_variableName();
                std::string currentCountType = inputVariablesInfo[varIdx].get_variableCountType();
                size_t varNameLine = 0;
                std::string currentVarCountString = "";
                if(variableNameAndCountFinder(fzInput,currentVarName,currentVarCountString,varNameLine) == false)
                {
                    printf("problem finding variable count for variable \"%s\" of count type \"%s\"\n",currentVarName.c_str(),currentCountType.c_str());
                    success = false;
                } else
                {
                    bool conversionSuccess = false;
                    if(currentCountType == "bool")
                    {
                        bool currentVarCount = strToBool(currentVarCountString,conversionSuccess);
                        if(conversionSuccess == false || inputVariableValueStorage->set_inputVariableBoolValue(currentVarName,currentVarCount) == false)
                        {
                            printf("couldn't set bool value \"%d\" to variable \"%s\"!\n",currentVarCount,currentVarName.c_str());
                            success = false;
                        }
                    } else if(currentCountType == "size_t")
                    {
                        size_t currentVarCount = strToSize_t(currentVarCountString,conversionSuccess);
                        if(conversionSuccess == false || inputVariableValueStorage->set_inputVariableSize_tValue(currentVarName,currentVarCount) == false)
                        {
                            printf("couldn't set size_t value \"%zu\" to variable \"%s\"!\n",currentVarCount,currentVarName.c_str());
                            success = false;
                        }
                    } else if(currentCountType == "int")
                    {
                        int currentVarCount = strToInt(currentVarCountString,conversionSuccess);
                        if(conversionSuccess == false || inputVariableValueStorage->set_inputVariableIntValue(currentVarName,currentVarCount) == false)
                        {
                            printf("couldn't set int value \"%d\" to variable \"%s\"!\n",currentVarCount,currentVarName.c_str());
                            success = false;
                        }
                    } else if(currentCountType == "double")
                    {
                        double currentVarCount = strToDbl(currentVarCountString,conversionSuccess);
                        if(conversionSuccess == false || inputVariableValueStorage->set_inputVariableDoubleValue(currentVarName,currentVarCount) == false)
                        {
                            printf("couldn't set double value \"%f\" to variable \"%s\"!\n",currentVarCount,currentVarName.c_str());
                            success = false;
                        }
                    } else if(currentCountType == "string")
                    {
                        conversionSuccess = isPureString(currentVarCountString);
                        if(conversionSuccess == false || inputVariableValueStorage->set_inputVariableStringValue(currentVarName,currentVarCountString) == false)
                        {
                            printf("couldn't set string value \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
                            success = false;
                        }
                    } else if(currentCountType == "filename")
                    {
                        conversionSuccess = isValidFilename(currentVarCountString);
                        if(conversionSuccess == false || inputVariableValueStorage->set_inputVariableFilenameValue(currentVarName,currentVarCountString) == false)
                        {
                            printf("couldn't set bool value \"%s\" to variable \"%s\"!\n",currentVarCountString.c_str(),currentVarName.c_str());
                            success = false;
                        }
                    } else if(currentCountType == "date")
                    {
                        int currentYear = 0;
                        int currentMonth = 0;
                        int currentDay = 0;
                        int currentHour = 0;
                        int currentMinute = 0;
                        conversionSuccess = strToDate(currentVarCountString,currentYear,currentMonth,currentDay,currentHour,currentMinute);
                        if(conversionSuccess == false || inputVariableValueStorage->set_inputVariableDateValue(currentVarName,currentYear,currentMonth,currentDay,currentHour,currentMinute) == false)
                        {
                            printf("couldn't set date value \"%d %d %d %d %d\" \"(year month day hour minute)\" to variable \"%s\"!\n",currentYear,currentMonth,currentDay,currentHour,currentMinute,currentVarName.c_str());
                            success = false;
                        }
                    } else if(currentCountType == "count")
                    {
                        // a side effect of the variableNameAndCountFinder function is that it left with a getline of the last location, the variable name line
                        // so the loader functions should just be able to continue on reading the input file where that function left off
                        std::string currentLoaderFunction = inputVariablesInfo[varIdx].get_loaderFunctionName();
                        size_t currentVarCount = strToSize_t(currentVarCountString,conversionSuccess);
                        if(conversionSuccess == false)
                        {
                            printf("variable count value \"%s\" for variable \"%s\" of count type \"count\" is not a size_t type!\n",currentVarCountString.c_str(),currentVarName.c_str());
                            success = false;
                        } else
                        {
                            if(currentLoaderFunction == "load_wrf_files")
                            {
                                if(load_wrf_files(fzInput,currentVarCount,varNameLine) == false)
                                {
                                    printf("failed to load wrf_files. Make sure the count number matches the number of files!\n");
                                    success = false;
                                }
                            } else if(currentLoaderFunction == "")
                            {

                            } else
                            {
                                printf("loader function \"%s\" for variable \"%s\" has not been implemented in the code yet!\n",currentLoaderFunction.c_str(),currentVarName.c_str());
                                success = false;
                            }
                        }
                    } else
                    {
                        printf("count type \"%s\" for variable \"%s\" has not been implemented in the code yet!\n",currentCountType.c_str(),currentVarName.c_str());
                        success = false;
                    }
                }
            }
        }
    }

    return success;
}
/*** end main loader functions ***/



/***** private functions *****/



/*** parsing utility functions ***/
bool inputParser::doesFileExist(std::string fileName, bool suppressWarnings)
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
    for(size_t charIdx = inputString.length(); charIdx > 0; charIdx++)
    {
        std::string currentChr = inputString.substr(charIdx,1);
        if(currentChr != " ")
        {
            inputString = inputString.substr(charIdx,inputString.length());
            break;
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
    for(size_t charIdx = 0; charIdx < inputString.length(); charIdx++)
    {
        std::string currentChr = inputString.substr(charIdx,1);
        if(currentChr == inputSeparator)
        {
            varNamePart = inputString.substr(0,charIdx-1);
            inputString = inputString.substr(charIdx+1,inputString.length());
            break;
        }
    }
}

// basically the same as the variableNameFinder, but now it actually does stuff with the rest of the information just in case
bool inputParser::variableNameAndCountFinder(std::ifstream &inputFile, std::string varName, std::string &varCountString, size_t &varNameLine)
{
    bool foundVar = false;

    std::string currentLine;
    size_t lineCount = 0;
    while(std::getline(inputFile,currentLine))
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
        removeSeparator(currentLine,currentVarName);

        // compare the variableName that is found with the desired variableName
        // if it matches, process what is left of the currentLine to get the variableCount value as a string for processing by external functions if needed
        if(currentVarName == varName)
        {
            foundVar = true;
            varNameLine = lineCount;
            removeLeadingWhitespace(currentLine);   // just in case
            removeEndingWhitespace(currentLine);    // just in case
            varCountString = currentLine;
            break;
        }
        lineCount = lineCount + 1;
    }

    return foundVar;
}
/*** end parsing utility functions ***/

/*** typechecker functions ***/
bool inputParser::isBool(std::string inputString)
{
    bool conversionSuccess = true;

    removeLeadingWhitespace(inputString);   // just in case
    removeEndingWhitespace(inputString);    // just in case

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

bool inputParser::isSize_t(std::string inputString)
{
    bool conversionSuccess = true;

    removeLeadingWhitespace(inputString);   // just in case
    removeEndingWhitespace(inputString);    // just in case

    size_t digitCounter = 0;
    for(size_t charIdx = 0; charIdx < inputString.length(); charIdx++)
    {
        std::string currentChr = inputString.substr(charIdx,1);
        if(currentChr >= "0" || currentChr <= "9")
        {
            digitCounter = digitCounter + 1;
        } else
        {
            conversionSuccess = false; // only numbers allowed at this point
            break;
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

bool inputParser::isInt(std::string inputString)
{
    bool conversionSuccess = true;

    removeLeadingWhitespace(inputString);   // just in case
    removeEndingWhitespace(inputString);    // just in case

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

bool inputParser::isDlb(std::string inputString)
{
    bool conversionSuccess = true;

    removeLeadingWhitespace(inputString);   // just in case
    removeEndingWhitespace(inputString);    // just in case

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

bool inputParser::isPureString(std::string inputString)
{
    bool conversionSuccess = true;

    removeLeadingWhitespace(inputString);   // just in case
    removeEndingWhitespace(inputString);    // just in case

    // separator characters bug me, but at this point I think that only spaces should be disallowed
    for(size_t charIdx = 0; charIdx < inputString.length(); charIdx++)
    {
        std::string currentChr = inputString.substr(charIdx,1);
        if(currentChr != " ")
        {
            conversionSuccess = false;
            break;
        }
    }

    return conversionSuccess;
}

bool inputParser::isValidFilename(std::string inputString)
{
    bool conversionSuccess = true;

    removeLeadingWhitespace(inputString);   // just in case
    removeEndingWhitespace(inputString);    // just in case

    // first validate there is no whitespace
    if(isPureString(inputString) == false)
    {
        conversionSuccess = false;
    } else
    {
        // now see if it can open as a filename
        if(doesFileExist(inputString,true) == false)
        {
            conversionSuccess = false;
        }
    }

    return conversionSuccess;
}

bool inputParser::isValidDate(int currentYear, int currentMonth, int currentDay, int currentHour, int currentMinute)
{
    bool conversionSuccess = true;

    bool isLeapYear = false;
    if(currentYear < 0)
    {
        conversionSuccess = false;
    } else
    {
        if(currentYear % 4 == 0 && currentYear % 100 == 0)
        {
            if(currentYear % 400 != 0)
            {
                isLeapYear = true;
            }
        }
    }

    if(currentMonth < 0 || currentMonth > 12)
    {
        conversionSuccess = false;
    } else
    {
        if(currentMonth == 2 && isLeapYear == true)
        {
            if(currentDay < 0 || currentDay > validDates.daysPerMonth[currentMonth]+1)
            {
                conversionSuccess = false;
            }
        } else
        {
            if(currentDay < 0 || currentDay > validDates.daysPerMonth[currentMonth])
            {
                conversionSuccess = false;
            }
        }
    }

    if(currentHour < validDates.hour_min || currentHour > validDates.hour_max)
    {
        conversionSuccess = false;
    }

    if(currentMinute < validDates.minute_min || currentMinute > validDates.minute_max)
    {
        conversionSuccess = false;
    }

    return conversionSuccess;
}
/*** end typechecker functions ***/

/*** type conversion functions ***/
bool inputParser::strToBool(std::string inputString, bool &conversionSuccess)
{
    bool outputValue = false;
    conversionSuccess = true;

    removeLeadingWhitespace(inputString);   // just in case
    removeEndingWhitespace(inputString);    // just in case

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

size_t inputParser::strToSize_t(std::string inputString, bool &conversionSuccess)
{
    size_t outputValue = 0;
    conversionSuccess = true;

    removeLeadingWhitespace(inputString);   // just in case
    removeEndingWhitespace(inputString);    // just in case

    if(isSize_t(inputString) == true)
    {
        outputValue = atoi(inputString.c_str());
    } else
    {
        conversionSuccess = false;
        printf("inputString \"%s\" is not a size_t value!\n",inputString.c_str());
    }

    return outputValue;
}

int inputParser::strToInt(std::string inputString, bool &conversionSuccess)
{
    int outputValue = -999;
    conversionSuccess = true;

    removeLeadingWhitespace(inputString);   // just in case
    removeEndingWhitespace(inputString);    // just in case

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

double inputParser::strToDbl(std::string inputString, bool &conversionSuccess)
{
    double outputValue = -999;
    conversionSuccess = true;

    removeLeadingWhitespace(inputString);   // just in case
    removeEndingWhitespace(inputString);    // just in case

    if(conversionSuccess == true)
    {
        outputValue = atof(inputString.c_str());
    } else
    {
        conversionSuccess = false;
        printf("inputString \"%s\" is not a double value!\n",inputString.c_str());
    }

    return outputValue;
}

bool inputParser::strToDate(std::string inputString, int &currentYear, int &currentMonth, int &currentDay, int &currentHour, int &currentMinute)
{
    bool conversionSuccess = true;

    removeLeadingWhitespace(inputString);   // just in case
    removeEndingWhitespace(inputString);    // just in case

    bool isValue = true;
    std::vector<std::string> foundValues;
    size_t startValueSpot = 0;
    for(size_t charIdx = 0; charIdx < inputString.length(); charIdx++)
    {
        std::string currentChr = inputString.substr(charIdx,1);
        if((currentChr == " " && isValue == true) || (currentChr == "\\" && isValue == true))
        {
            foundValues.push_back(inputString.substr(startValueSpot,charIdx-startValueSpot));
            isValue = false;
        }
        if(currentChr != " " && currentChr != "\\" && isValue == false)
        {
            startValueSpot = charIdx;
            isValue = true;
        }
        if(charIdx == inputString.length() && isValue == true)
        {
            foundValues.push_back(inputString.substr(startValueSpot,charIdx-startValueSpot));
        }
    }

    if(foundValues.size() < 5)
    {
        printf("not enough values found in date!\n");
        conversionSuccess = false;
    } else
    {
        bool isInt = false;
        int foundYear = strToInt(foundValues[0],isInt);
        int foundMonth = strToInt(foundValues[1],isInt);
        int foundDay = strToInt(foundValues[2],isInt);
        int foundHour = strToInt(foundValues[3],isInt);
        int foundMinute = strToInt(foundValues[4],isInt);
        if(isInt == false)
        {
            printf("problem converting date stuff to int!\n");
            conversionSuccess = false;
        } else
        {
            if(isValidDate(foundYear,foundMonth,foundDay,foundHour,foundMinute) == false)
            {
                printf("date is not valid!\n");
                conversionSuccess = false;
            } else
            {
                currentYear = foundYear;
                currentMonth = foundMonth;
                currentDay = foundDay;
                currentHour = foundHour;
                currentMinute = foundMinute;
                conversionSuccess = true;
            }
        }
    }

    return conversionSuccess;
}
/*** end type conversion functions ***/

/*** type count loader functions ***/
bool inputParser::load_wrf_files(std::ifstream &inputFile, size_t varCount, size_t varNameLine)
{
    bool success = true;

    // should already be at the right line, should grab the next one. Don't allow comment lines in this section
    for(size_t lineCount = 0; lineCount < varCount+1; lineCount++)
    {
        std::string currentLine;
        std::getline(inputFile,currentLine);

        removeLeadingWhitespace(currentLine);
        removeEndingWhitespace(currentLine);

        bool conversionSuccess = isValidFilename(currentLine);
        if(conversionSuccess == false)
        {
            if(lineCount < varCount)
            {
                printf("not enough wrf_files for wrf_file variable count!\n");
            }
            success = false;
            break;
        } else
        {
            if(lineCount >= varCount)   // should be checking to see if it is varCount+1
            {
                printf("too many wrf_files for wrf_file variable count!\n");
                success = false;
                break;
            } else
            {
                inputVariableValueStorage->add_wrf_file(currentLine);
            }
        }
        lineCount = lineCount + 1;
    }

    return success;
}
/*** end type count loader functions ***/
