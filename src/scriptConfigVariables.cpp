#include "scriptConfigVariables.h"

/***** public functions *****/

/*** constructor functions ***/
scriptConfigVariables::scriptConfigVariables()
{
    bool success = true;

    // first setup all the needed stuff for the variables
    setupAvailableVariableCountTypes();
    setupAvailableVariables();

    // now run checks on the setup of the variables
    if(check_setupDuplicateVariables() == false)
    {
        printf("duplicate option found during setup!\n");
        success = false;
    }
    if(check_setupVariableCountType() == false)
    {
        printf("non-allowable count type found during setup!\n");
        success = false;
    }
    if(check_setupConflictingVariables() == false)
    {
        printf("invalid conflicting variables found during setup!\n");
        success = false;
    }

    if(success == false)
    {
        printf("exiting program\n");
        exit(1);
    }

    // what level do we reset the option boolean values? Looks like I did it in the config file reader section, which makes sense. So this class is just for setting up the available options and then getting them out once they are needed. Then you use them as you please. Nevermind, the types end up defined and checked in config option

}
/*** end constructor functions ***/


/*** get value functions ***/
std::vector<configVariable> scriptConfigVariables::get_theVariables()
{
    return theVariables;
}
/*** end get value functions ***/



/***** private functions *****/



/*** setup functions ***/
void scriptConfigVariables::setupAvailableVariableCountTypes()
{
    // add desired variable count types here. Not all have to be used by a configVariable, but these are the possible options
    // The idea is that the input file parser determines how to use these config variables, so it has special instructions of what to do with a given config variable using its variable count type
    allowedVariableCountTypes.push_back("int");
    allowedVariableCountTypes.push_back("double");
    allowedVariableCountTypes.push_back("string");
    allowedVariableCountTypes.push_back("filename");
    allowedVariableCountTypes.push_back("count");    // this one is interesting, because it is a type of integer and signifies that the data is more than one value. All others mean data is one value.
}

void scriptConfigVariables::addVariable(std::string newVariableName,std::string newVariableCountType,std::vector<std::string> newConflictingVariables,
                                        std::string newLoaderFunctionName,std::string newVariableDescription)
{
    theVariables.push_back(configVariable(newVariableName,newVariableCountType,newConflictingVariables,newLoaderFunctionName,newVariableDescription));
}

void scriptConfigVariables::setupAvailableVariables()
{
    // add desired variables to be read from the config file. All these will be assumed to be required unless the conflicting options specify otherwise.
    // whether all these variables are actually used by the program even though they are required by the input file is up to the programmer linking them into the program
    // In fact, the basic needs for each input variable to even check for them are done for all variables, but whether they are even loaded in depends on if there is a load function for the data
    // hmm, makes me realize I should add a function loader name and an is loaded boolean, that way a script can just load all the variables with case or if else checks
    //  to see if the required load function is actually used or not. The end else would set the boolean. Hmm, that doesn't work in time to add as a displayed option to the user.
    //  ahh, I know! keep the function loader name variable, but if it hits the else statement, a print statement saying variable is declared as a possible input, but has no loader function. Brilliant!

    // one big problem with this is that it is tricky to keep it pretty when trying to add in all the stuff. Technically it isn't too bad yet, but especially the description!
    //  Yeah trying to get the format right on that requires breaking up the input lines

	// should have a section for each type of input variables, and include the type or use in the description

	// WindNinja only variables

	// wrfGetWeather only variables

	// wrfGetWeather and WindNinja variables
    addVariable("WRF_FILES","count",{"NA"},"load_WRF_FILES",
                "Hourly WRF files from 1 day before the FARSITE_BURN_START variable to the time set by the FARSITE_BURN_END variable");

	// createIgnition variables

	// createFarsiteInput variables

	// looks like I need to add another type to each variable as a flag to which program type they are lol, maybe another setup saying what program types are available? That way the input description can be nicely broken up into types of inputs
	// notice that these do NOT include all the possible variables for windninja and farsite, just the ones that will be changing a bunch. So if you need to move needed variables that are just set to be the same thing every time for farsite or windninja, look in wrfInterpretation and createFarsiteInput at what is set to be the same everytime, and create a variable replacement here, but then you need to adjust how that variable is used in wrfInterpretation and createFarsiteInput :)

}
/*** end setup functions ***/

/*** check setup functions ***/
bool scriptConfigVariables::check_setupDuplicateVariables()
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

bool scriptConfigVariables::check_setupVariableCountType()
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
                printf("variable: %s specified it's own name as a conflicting variable!\n",theVariables[variableIdx].get_variableName().c_str());
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
/*** end check setup functions ***/
