#include "scriptConfigVariables.h"

/***** public functions *****/

/*** constructor functions ***/
scriptConfigVariables::scriptConfigVariables()
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
    if(check_setupConflictingVariables() == false)
    {
        printf("invalid conflicting variables found during setup!\n");
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

    // what level do we reset the option boolean values? Looks like I did it in the config file reader section, which makes sense. So this class is just for setting up the available options and then getting them out once they are needed. Then you use them as you please. Nevermind, the types end up defined and checked in config option

}
/*** end constructor functions ***/


/*** get value functions ***/
std::vector<configVariable> scriptConfigVariables::get_theVariables()
{
    return theVariables;
}

std::string scriptConfigVariables::get_maxVarNameColumnWhitespace()
{
    return maxVarNameColumnWhitespace;
}
/*** end get value functions ***/



/***** private functions *****/



/*** setup functions ***/
void scriptConfigVariables::setupAvailableApplicationUseNames()
{
    // the idea is that each variable is used for different things, and when explaining uses, it is handy to show the applications they are for
    // another function will verify that the setupAvailableVariables() function only used these types and that they were specified in order of application type, so in this order :)
    allowedApplicationUseNames.push_back("WindNinja only");
    allowedApplicationUseNames.push_back("wrfGetWeather only");
    allowedApplicationUseNames.push_back("wrfGetWeather and WindNinja");
    allowedApplicationUseNames.push_back("createIgnition");
    allowedApplicationUseNames.push_back("createFarsiteInput");
}

void scriptConfigVariables::setupAvailableVariableCountTypes()
{
    // add desired variable count types here. Not all have to be used by a configVariable, but these are the possible options
    // The idea is that the input file parser determines how to use these config variables, so it has special instructions of what to do with a given config variable using its variable count type
    allowedVariableCountTypes.push_back("bool");
    allowedVariableCountTypes.push_back("int");
    allowedVariableCountTypes.push_back("double");
    allowedVariableCountTypes.push_back("string");
    allowedVariableCountTypes.push_back("filename");
    allowedVariableCountTypes.push_back("date");
    allowedVariableCountTypes.push_back("count");    // this one is interesting, because it is a type of integer and signifies that the data is more than one value. All others mean data is one value.
}

void scriptConfigVariables::addVariable(std::string newVariableName,std::string newApplicationUseName,std::string newVariableCountType,
                                        std::vector<std::string> newConflictingVariables,std::string newLoaderFunctionName,std::string newVariableDescription)
{
    theVariables.push_back(configVariable(newVariableName,newApplicationUseName,newVariableCountType,newConflictingVariables,newLoaderFunctionName,newVariableDescription));
}

void scriptConfigVariables::setupAvailableVariables()
{
    // add desired variables to be read from the config file. All these will be assumed to be required unless the conflicting options specify otherwise.
    // whether all these variables are actually used by the program even though they are required and at least the names and counts are read by the input file,
    // is up to the programmer linking them into the program. But the load function name means that a warning will be given to the user/programmer if a variable is required
    // as input but has no loading method. Still doesn't mean that it is used, but hopefully this makes deprecation a little less annoying.

    // In fact, the basic needs for each input variable to even check for them are done for all variables, but whether they are even loaded in depends on if there is a load function for the data
    // hmm, makes me realize I should add a function loader name and an is loaded boolean, that way a script can just load all the variables with case or if else checks
    //  to see if the required load function is actually used or not. The end else would set the boolean. Hmm, that doesn't work in time to add as a displayed option to the user.
    //  ahh, I know! keep the function loader name variable, but if it hits the else statement, a print statement saying variable is declared as a possible input, but has no loader function. Brilliant!

    // one big problem with this is that it is tricky to keep it pretty when trying to add in all the stuff. Technically it isn't too bad yet, but especially the description!
    // Fortunately there are some parsing tricks applied using whitespace to hopefully clean up the format of the description so you only need to do a single string for description.
    // If that fails and warnings come a lot, might have to reprogram stuff and manually setup the format of the input variables. If so, wait till all variable names and descriptions
    // are set or you'll be redoing changes a lot.

	// should have a section for each type of input variables, and include the type or use in the description

	// WindNinja only variables
    addVariable("diurnal_winds","WindNinja only","bool",{"NA"},"NA",
                "true or false; the dates, times, temperatures, and cloud covers will be autodetected from the WRF files by WindNinja");
    addVariable("non_neutral_stability","WindNinja only","bool",{"NA"},"NA",
                "true or false; the dates, times, temperatures, and cloud covers will be autodetected from the WRF files by WindNinja");

	// wrfGetWeather only variables
    addVariable("weather_band_names","wrfGetWeather only","count",{"NA"},"load_weather_band_names",
                "use gdalinfo on a few of the wrf files to see what band names are useful. Usual examples are like \"T10\"");
    addVariable("use_firearea_average","wrfGetWeather only","bool",{"use_firearea_center"},"NA",
                "true or false, use the average of all the weather data over the whole fire area or a single value at the center");
    addVariable("use_firearea_center","wrfGetWeather only","bool",{"use_firearea_average"},"NA",
                "true or false, use the center of the fire location or an average of all the weather data over the whole fire area");

	// wrfGetWeather and WindNinja variables
    addVariable("wrf_files","wrfGetWeather and WindNinja","count",{"NA"},"load_wrf_files",
                "Hourly WRF files from 1 day before the FARSITE_BURN_START variable to the time set by the FARSITE_BURN_END variable");

	// createIgnition variables
    addVariable("latlong_position","createIgnition","count",{"NA"},"load_latlong_position",
                "create an ignition file using the following set of lat long positions, so there are a bunch of point sources and this is the first instance of the fire");

	// createFarsiteInput variables
    addVariable("FARSITE_START_TIME","createFarsiteInput","date",{"NA"},"NA",
                "date of the following format: month day hour. Is the start time for the farsite burn. Make sure wrf files cover at least a day before this date");

    // notice that these do NOT include all the possible variables for windninja and farsite, just the ones that will be changing a bunch.
    // So if you need to move needed variables that are just set to be the same thing every time for farsite or windninja, look in
    // wrfInterpretation and createFarsiteInput at what is set to be the same everytime, and create a variable replacement here,
    // but then you need to adjust how that variable is used in wrfInterpretation and createFarsiteInput :)

}
/*** end setup functions ***/

/*** check setup functions ***/
bool scriptConfigVariables::check_setupForDuplicateVariableNames()
{
    bool success = true;
    for(size_t i = 0; i < theVariables.size()-1; i++)
    {
        for(size_t j = i+1; j < theVariables.size(); j++)
        {
            if(theVariables[i].get_variableName() == theVariables[j].get_variableName())
            {
                printf("found duplicate variable \"%s\"!\n",theVariables[i].get_variableName().c_str());
                success = false;
            }
        }
    }
    return success;
}

bool scriptConfigVariables::check_setupForValidApplicationUseNames()
{
    bool success = true;
    for(size_t variableIdx = 0; variableIdx < theVariables.size(); variableIdx++)
    {
        bool isValidUseName = false;
        for(size_t useNameIdx = 0; useNameIdx < allowedApplicationUseNames.size(); useNameIdx++)
        {
            if(theVariables[variableIdx].get_applicationUseName() == allowedApplicationUseNames[useNameIdx])
            {
                isValidUseName = true;
                break;
            }
        }
        if(isValidUseName == false)
        {
            printf("application use name \"%s\" for variable \"%s\" is not a valid application use name!\n",theVariables[variableIdx].get_applicationUseName().c_str(),theVariables[variableIdx].get_variableName().c_str());
            success = false;
        }
    }
    return success;
}

bool scriptConfigVariables::check_setupForValidOrderingByApplicationUseNames()
{
    bool success = true;

    // make sure the variable order matches the application use names
    unsigned int applicationUseNameTypeCount = 0;
    bool foundValidApplicationUseName = false;
    for(size_t varIdx = 0; varIdx < theVariables.size(); varIdx++)
    {
        if(applicationUseNameTypeCount >= allowedApplicationUseNames.size())
        {
            printf("variables are not ordered by applicationUseName! Found at variable \"%s\"!\n",theVariables[varIdx].get_variableName().c_str());
            success = false;
            break;
        }
        if(theVariables[varIdx].get_applicationUseName() == allowedApplicationUseNames[applicationUseNameTypeCount])
        {
            foundValidApplicationUseName = true;
        } else
        {
            if(foundValidApplicationUseName == true)
            {
                applicationUseNameTypeCount = applicationUseNameTypeCount + 1;
            } else
            {
                printf("variables are not ordered by applicationUseName! Found at variable \"%s\"!\n",theVariables[varIdx].get_variableName().c_str());
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

bool scriptConfigVariables::check_setupForValidVariableCountTypes()
{
    bool success = true;
    for(size_t variableIdx = 0; variableIdx < theVariables.size(); variableIdx++)
    {
        bool isValidType = false;
        for(size_t countTypesIdx = 0; countTypesIdx < allowedVariableCountTypes.size(); countTypesIdx++)
        {
            if(theVariables[variableIdx].get_variableCountType() == allowedVariableCountTypes[countTypesIdx])
            {
                isValidType = true;
                break;
            }
        }
        if(isValidType == false)
        {
            printf("variable count type \"%s\" for variable \"%s\" is not a valid type!\n",theVariables[variableIdx].get_variableCountType().c_str(),theVariables[variableIdx].get_variableName().c_str());
            success = false;
        }
    }
    return success;
}

//make sure each conflicting option really does exist as a variable name
bool scriptConfigVariables::check_setupConflictingVariables()
{
    bool success = true;
    for(size_t variableIdx = 0; variableIdx < theVariables.size(); variableIdx++)
    {
        std::vector<std::string> currentConflictingVariables = theVariables[variableIdx].get_conflictingVariables();
        for(size_t conflictVarIdx = 0; conflictVarIdx < currentConflictingVariables.size(); conflictVarIdx++)
        {
            if(currentConflictingVariables[conflictVarIdx] == "NA")
            {
                if(currentConflictingVariables.size() > 1)
                {
                    printf("variable \"%s\" has conflicting variable \"NA\" but size is not 1!\n",theVariables[variableIdx].get_variableName().c_str());
                    success = false;
                    break;
                }
            } else if(theVariables[variableIdx].get_variableName() == currentConflictingVariables[conflictVarIdx])
            {
                printf("variable \"%s\" specified it's own name as a conflicting variable!\n",theVariables[variableIdx].get_variableName().c_str());
                success = false;
            } else
            {
                // go through all variables to see if conflicting option exists as one of the other variable names
                bool foundVariableName = false;
                for(size_t secondVariableIdx = 0; secondVariableIdx < theVariables.size(); secondVariableIdx++)
                {
                    if(secondVariableIdx != variableIdx)
                    {
                        if(currentConflictingVariables[conflictVarIdx] == theVariables[secondVariableIdx].get_variableName())
                        {
                            foundVariableName = true;
                            break;
                        }
                    }
                }
                if(foundVariableName == false)
                {
                    printf("conflicting option \"%s\" for variable \"%s\" does not exist as another variable name!\n",currentConflictingVariables[conflictVarIdx].c_str(),theVariables[variableIdx].get_variableName().c_str());
                    success = false;
                }
            }
        }
    }
    return success;
}

bool scriptConfigVariables::check_setupDescription()
{
    bool success = true;

    // first add up the number of starting characters that are whitespace. If there are any, it is an error so warn
    // and say how many extra characters and there should be no whitespace at the start of a description
    for(size_t varIdx = 0; varIdx < theVariables.size(); varIdx++)
    {
        unsigned int whiteSpaceCount = 0;
        std::string currentDescription = theVariables[varIdx].get_variableDescription();
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
            printf("variable \"%s\" description starts with %d white space characters. variable descriptions should not start with whitespace!\n",theVariables[varIdx].get_variableName().c_str(),whiteSpaceCount);
            success = false;
        }
    }

    return success;
}
/*** end check setup functions ***/

/*** description whitespace and line break calculations, with error checking ***/
bool scriptConfigVariables::calculateDescriptionWhiteSpace()
{
    bool success = true;

    // first calculate the biggest string for the variable names
    unsigned int biggestString = 0;
    for(size_t varIdx = 0; varIdx < theVariables.size(); varIdx++)
    {
        if(theVariables[varIdx].get_variableName().size() > biggestString)
        {
            biggestString = theVariables[varIdx].get_variableName().size();
        }
    }
    descriptionVariableNameColumnSize = biggestString + MIN_VARNAME_WHITESPACE;

    // now calculate variable name whitespace for each variable
    unsigned int neededWhiteSpace = 0;
    std::string createdWhiteSpace = "";
    for(size_t varIdx = 0; varIdx < theVariables.size(); varIdx++)
    {
        neededWhiteSpace = descriptionVariableNameColumnSize - theVariables[varIdx].get_variableName().size(); // defined in header file
        createdWhiteSpace = "";
        for(size_t m = 0; m < neededWhiteSpace; m++)
        {
            createdWhiteSpace = createdWhiteSpace + " ";
        }
        theVariables[varIdx].set_variableNameWhiteSpace(createdWhiteSpace);
    }

    return success;
}

bool scriptConfigVariables::calculateDescriptionLineBreaks()
{
    bool success = true;

    for(size_t varIdx = 0; varIdx < theVariables.size(); varIdx++)
    {
        std::vector<unsigned int> wordBreaks;
        // first find all whitespace locations in the description
        // notice there were checks which end the program if the description starts with whitespace, so we can assume all whitespace is after the first character of the description
        std::string currentDescription = theVariables[varIdx].get_variableDescription();
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
        theVariables[varIdx].add_variableDescriptionLineBreaks(0);  // start out with the first break as 0, also avoids breaking problems later
        unsigned int lineCount = 0;
        unsigned int descriptionMaxSize = MAX_DESCRIPTION_LINESIZE - descriptionVariableNameColumnSize;
        for(size_t wordIdx = 0; wordIdx < wordBreaks.size(); wordIdx++)
        {
            unsigned int currentLineSize = wordBreaks[wordIdx] - theVariables[varIdx].get_variableDescriptionLineBreaks()[lineCount];
            if(currentLineSize >= descriptionMaxSize)
            {
                theVariables[varIdx].add_variableDescriptionLineBreaks(wordBreaks[wordIdx-1]);
                lineCount = lineCount + 1;
            } else if(wordIdx == wordBreaks.size()-1)
            {
                theVariables[varIdx].add_variableDescriptionLineBreaks(wordBreaks[wordIdx]);
            }
        }
    }

    return success;
}

void scriptConfigVariables::calculate_maxVarNameColumnWhitespace()
{
    for(size_t whitespaceCount = 0; whitespaceCount < descriptionVariableNameColumnSize; whitespaceCount++)
    {
        maxVarNameColumnWhitespace = maxVarNameColumnWhitespace + " ";
    }
}
/*** end description whitespace and line break calculations, with error checking ***/
