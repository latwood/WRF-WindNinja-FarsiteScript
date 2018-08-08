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
    if(check_setupLoaderFunctionNames() == false)
    {
        printf("invalid loader function name found during setup!\n");
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
    allowedApplicationUseNames.push_back("WindNinja only");
    allowedApplicationUseNames.push_back("wrfGetWeather only");
    allowedApplicationUseNames.push_back("wrfGetWeather and WindNinja");
    allowedApplicationUseNames.push_back("createIgnition");
    allowedApplicationUseNames.push_back("createFarsiteInput");
}

void inputVariables_infoStorage::setupAvailableVariableCountTypes()
{
    // this is what will be used in the end, don't need to redefine the count types twice
    allowedVariableCountTypes.push_back("bool");
    allowedVariableCountTypes.push_back("size_t");
    allowedVariableCountTypes.push_back("int");
    allowedVariableCountTypes.push_back("double");
    allowedVariableCountTypes.push_back("string");
    allowedVariableCountTypes.push_back("filename");
    allowedVariableCountTypes.push_back("date");
    allowedVariableCountTypes.push_back("count");
}

void inputVariables_infoStorage::addVariable(std::string newVariableName,std::string newApplicationUseName,std::string newVariableCountType,
                                             std::string newLoaderFunctionName,std::string newVariableDescription)
{
    inputVariables.push_back(inputVariable_info(newVariableName,newApplicationUseName,newVariableCountType,newLoaderFunctionName,newVariableDescription));
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

        // should have a section for each type of input variables, and include the type or use in the description
        // if you need more sections, define section in setupAvailableApplicationUseNames(). Define more types in setupAvailableVariableCountTypes().
        // just know that additional types will also need an additional load method and other input storage functions in other input classes
        // the variable names will be greatly dependent on the variable names used throughout the program as well as the loader/parser and variable storage classes
        // variables may be loaded or no, but to verify if optional or no, need to change verification in the checkSetVarNamesForConflictingOptions function in the inputVariablesHandler class.

        // WindNinja only variables
    addVariable("diurnal_winds","WindNinja only","bool","NA",
                "true or false; the dates, times, temperatures, and cloud covers will be autodetected from the WRF files by WindNinja");
    addVariable("non_neutral_stability","WindNinja only","bool","NA",
                "true or false; the dates, times, temperatures, and cloud covers will be autodetected from the WRF files by WindNinja");

        // wrfGetWeather only variables
    addVariable("weather_band_names","wrfGetWeather only","count","load_weather_band_names",
                "use gdalinfo on a few of the wrf files to see what band names are useful. Usual examples are like \"T10\"");
    addVariable("use_firearea_average","wrfGetWeather only","bool","NA",
                "true or false, use the average of all the weather data over the whole fire area or a single value at the center");
    addVariable("use_firearea_center","wrfGetWeather only","bool","NA",
                "true or false, use the center of the fire location or an average of all the weather data over the whole fire area");

        // wrfGetWeather and WindNinja variables
    addVariable("wrf_files","wrfGetWeather and WindNinja","count","load_wrf_files",
                "Hourly WRF files from 1 day before the FARSITE_BURN_START variable to the time set by the FARSITE_BURN_END variable");

        // createIgnition variables
    addVariable("latlong_position","createIgnition","count","load_latlong_position",
                "create an ignition file using the following set of lat long positions, so there are a bunch of point sources and this is the first instance of the fire");

        // createFarsiteInput variables
    addVariable("FARSITE_START_TIME","createFarsiteInput","date","NA",
                "date of the following format: month day hour. Is the start time for the farsite burn. Make sure wrf files cover at least a day before this date");

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

bool inputVariables_infoStorage::check_setupLoaderFunctionNames()
{
    bool success = true;

    // can't tell if loader functions are valid in that they will be used or not at this point,
    // but can make sure that if the variableCountTypeAmount is for a single value (anything but a type "count") for a given configVariable, that the loaderFunctionName is set to "NA"
    // can also check to make that if the variableCountTypeAmount is a "count" for a given configVariable, that the loaderFunctionName is not "" or whitespace
    // or is a duplicate of another loader function name
    for(size_t varIdx = 0; varIdx < inputVariables.size(); varIdx++)
    {
        // first need to find which of the allowedVariableCountTypes the variableCountType is to get access to the right variableCountTypeAmount
        // because this action is repeated in other places, it was turned into a function
        std::string currentCountType = inputVariables[varIdx].get_variableCountType();
        if(currentCountType != "count")
        {
            if(inputVariables[varIdx].get_loaderFunctionName() != "NA")
            {
                printf("variable \"%s\" loaderFunctionName with variableCountType \"%s\" is not \"NA\" even though variableCountType \"%s\" has countAmount \"single\"!\n",inputVariables[varIdx].get_variableName().c_str(),inputVariables[varIdx].get_variableCountType().c_str(),inputVariables[varIdx].get_variableCountType().c_str());
                success = false;
            }
        }
        if(currentCountType == "count")
        {
            if(inputVariables[varIdx].get_loaderFunctionName() == "NA")
            {
                printf("variable \"%s\" loaderFunctionName with variableCountType \"%s\" is \"NA\" even though variableCountType \"%s\" has countAmount \"multiple\"!\n",inputVariables[varIdx].get_variableName().c_str(),inputVariables[varIdx].get_variableCountType().c_str(),inputVariables[varIdx].get_variableCountType().c_str());
                success = false;
            }
            if(inputVariables[varIdx].get_loaderFunctionName() == "")
            {
                printf("variable \"%s\" loaderFunctionName with variableCountType \"%s\" is \"\" even though variableCountType \"%s\" has countAmount \"multiple\"!\n",inputVariables[varIdx].get_variableName().c_str(),inputVariables[varIdx].get_variableCountType().c_str(),inputVariables[varIdx].get_variableCountType().c_str());
                success = false;
            }
            if(inputVariables[varIdx].get_loaderFunctionName().substr(0,1) == " ")
            {
                printf("variable \"%s\" loaderFunctionName with variableCountType \"%s\" starts with whitespace or is only whitespace!\n",inputVariables[varIdx].get_variableName().c_str(),inputVariables[varIdx].get_variableCountType().c_str());
                success = false;
            }
        }
    }

    // now make sure there are no duplicate loadFunctionNames
    for(size_t firstVarIdx = 0; firstVarIdx < inputVariables.size()-1; firstVarIdx++)
    {
        for(size_t secondVarIdx = firstVarIdx+1; secondVarIdx < inputVariables.size(); secondVarIdx++)
        {
            if(inputVariables[firstVarIdx].get_loaderFunctionName() != "NA" && inputVariables[secondVarIdx].get_loaderFunctionName() != "NA")
            {
                if(inputVariables[firstVarIdx].get_loaderFunctionName() == inputVariables[secondVarIdx].get_loaderFunctionName())
                {
                    printf("found duplicate loaderFunctionName \"%s\"!\n",inputVariables[firstVarIdx].get_loaderFunctionName().c_str());
                    success = false;
                }
            }
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
                    printf("added variableDescriptionLineBreak for description line with only \"%d\" words for lineCount \"%d\". MIN_WORDS_PER_DESCRIPTION_LINE is \"%d\"",lineWordCount,lineCount,MIN_WORDS_PER_DESCRIPTION_LINE);
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
