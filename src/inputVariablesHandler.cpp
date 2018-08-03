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

bool inputVariablesHandler::loadScriptInputs()
{
    bool success = true;

    /* clean up all data that isn't just defined once for all uses */
    reset();

    // open file for use in everything (this section might leave as this is done each time as needed by parser class functions)


    //load all variable names with parser
    if(theInputParser.findVariableNames("inputFileName") == false)
    {
        printf("some variable names not found!\n");
        success = false;
    }

    //check conflicting and stop and warn for all conflicts
    if(findOptionConflicts() == false)
    {
        printf("found conflicting options!\n");
        success = false;
    }

    //if no conflicting options, load each input with parser, error if any cannot be loaded correctly
    if(success == true)
    {
        // okay this is a later insert, this would be calling the parser function on all the variables
        // could probably move the reset thing to the parser, as that is the only time the values should be reset lol

        if(theInputParser.loadAllInputs("inputFileName") == false)
        {
            printf("problem loading inputs!\n");
            success = false;
        }
    }

    // now should close the file (this section might leave as this is done each time as needed by parser class functions)


    return success;
}
/*** end  functions ***/



/***** private functions *****/



/*** other functions ***/
void inputVariablesHandler::reset()
{
    for(size_t varIdx = 0; varIdx < inputVariableInfo.size(); varIdx++)
    {
        inputVariableInfo[varIdx].resetVariable();
    }
}

bool inputVariablesHandler::findOptionConflicts()
{
    bool success = true;

    // go through each variable, then through each conflicting option for each variable, find variable with name matching each conflicting option and verify bool for said variable for if it is found in input is not true
    for(size_t varIdx = 0; varIdx < inputVariableInfo.size(); varIdx++)
    {

    }

    return success;
}
/*** end other functions ***/
