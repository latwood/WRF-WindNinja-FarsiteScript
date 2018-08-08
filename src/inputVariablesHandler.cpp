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
    reset();

    //open file and load all variable names with parser
    if(theInputParser.findVariableNames(inputFileName) == false)
    {
        printf("some variable names not found!\n");
        success = false;
    }

    //check conflicting and stop and warn for all conflicts
    if(checkConflictingOptions() == false)
    {
        printf("found conflicting options!\n");
        success = false;
    }

    //if no conflicting options, open file and load each input with parser, error if any cannot be loaded correctly
    if(success == true)
    {
        // okay this is a later insert, this would be calling the parser function on all the variables
        // could probably move the reset thing to the parser, as that is the only time the values should be reset lol
        if(theInputParser.loadAllInputs(inputFileName) == false)
        {
            printf("problem loading inputs!\n");
            success = false;
        }
    }

    return success;
}
/*** end  functions ***/

/*** get variable value functions ***/
bool inputVariablesHandler::get_inputVariableBoolValue(std::string varName)
{
    return inputVariableValues.get_inputVariableBoolValue(varName);
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

std::string inputVariablesHandler::get_inputVariableFilenameValue(std::string varName)
{
    return inputVariableValues.get_inputVariableFilenameValue(varName);
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

std::vector<std::string> inputVariablesHandler::get_wrf_files()
{
    return inputVariableValues.get_wrf_files();
}
/*** end get variable value functions ***/



/***** private functions *****/



/*** other functions ***/
void inputVariablesHandler::reset()
{
    for(size_t varIdx = 0; varIdx < inputVariableInfo.size(); varIdx++)
    {
        inputVariableInfo[varIdx].resetVariable();
        inputVariableValues.resetVariables();
    }
}

// I think super slow cause have to go through every single variable multiple times
bool inputVariablesHandler::checkConflictingOptions()
{
    bool success = true;

    // go through each variable, then through each conflicting option for each variable, find variable with name matching each conflicting option
    // and verify bool for said variable for if it is found in input is not true
    for(size_t firstVarIdx = 0; firstVarIdx < inputVariableInfo.size(); firstVarIdx++)
    {
        if(inputVariableInfo[firstVarIdx].get_conflictingVariables()[0] != "NA" && inputVariableInfo[firstVarIdx].get_isFoundInInputFile() == true)
        {
            for(size_t conflictVarIdx = 0; conflictVarIdx < inputVariableInfo[firstVarIdx].get_conflictingVariables().size(); conflictVarIdx++)
            {
                for(size_t secondVarIdx = 0; secondVarIdx < inputVariableInfo.size(); secondVarIdx++)
                {
                    if(firstVarIdx != secondVarIdx)
                    {
                        if(inputVariableInfo[firstVarIdx].get_conflictingVariables()[conflictVarIdx] == inputVariableInfo[secondVarIdx].get_variableName())
                        {
                            if(inputVariableInfo[secondVarIdx].get_isFoundInInputFile() == true)
                            {
                                // hm, don't actually need doesOptionConflict boolean! All this goes in one step without quitting till all conflicting options are checked
                                // I guess let's finish all the input parsing and stuff before throwing out said variable to make sure everything works
                                inputVariableInfo[firstVarIdx].set_doesVariableConflict(true);
                                inputVariableInfo[secondVarIdx].set_doesVariableConflict(true);
                                printf("variables \"%s\" and \"%s\" found in input file when only one is allowed!\n",inputVariableInfo[firstVarIdx].get_variableName().c_str(),inputVariableInfo[secondVarIdx].get_variableName().c_str());
                            }
                        }
                    }
                }
            }
        }
    }

    return success;
}
/*** end other functions ***/
