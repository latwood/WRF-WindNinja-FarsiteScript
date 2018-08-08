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

    //check for conflicting options and stop and warn for all conflicts
    if(checkSetVarNamesForConflictingOptions() == false)
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
        // comment this out unless debugging
        printFoundInput();
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
        if(currentCountType == "bool")
        {
            printf("%s: %d\n",currentVarName.c_str(),get_inputVariableBoolValue(currentVarName));
        } else if(currentCountType == "size_t")
        {
            printf("%s: %zu\n",currentVarName.c_str(),get_inputVariableSize_tValue(currentVarName));
        } else if(currentCountType == "int")
        {
            printf("%s: %d\n",currentVarName.c_str(),get_inputVariableIntValue(currentVarName));
        } else if(currentCountType == "double")
        {
            printf("%s: %f\n",currentVarName.c_str(),get_inputVariableDoubleValue(currentVarName));
        } else if(currentCountType == "string")
        {
            printf("%s: %s\n",currentVarName.c_str(),get_inputVariableStringValue(currentVarName).c_str());
        } else if(currentCountType == "filename")
        {
            printf("%s: %s\n",currentVarName.c_str(),get_inputVariableFilenameValue(currentVarName).c_str());
        } else if(currentCountType == "date")
        {
            printf("%s: %d %d %d %d %d\n",currentVarName.c_str(),get_inputVariableDateValueYear(currentVarName),get_inputVariableDateValueMonth(currentVarName),get_inputVariableDateValueDay(currentVarName),get_inputVariableDateValueHour(currentVarName),get_inputVariableDateValueMinute(currentVarName));
        } else if(currentCountType == "count")
        {
            // hm, this function is revealing the complexities and confusingness of even more input stuff
            // do I try to repeat all the rest of the inputs checking that WindNinja and Farsite do?
        } else
        {
            printf("count type \"%s\" for variable \"%s\" has not been implemented in the code yet!\n",currentCountType.c_str(),currentVarName.c_str());
        }
    }
    printf("\nfinished printing found input\n\n");
}
/*** end  functions ***/

/*** get variable value functions ***/
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
bool inputVariablesHandler::checkSetVarNamesForConflictingOptions()
{
    bool success = true;

    // setup whatever you need to set if input variables are optional or not, or how they are optional or required


    return success;
}
/*** end other functions ***/
