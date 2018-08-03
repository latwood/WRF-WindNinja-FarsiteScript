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

    // need to adjust to have a file for input
    for(size_t varIdx = 0; varIdx < inputVariablesInfo.size(); varIdx++)
    {
        // search in file for each variable name, just the name not the count
        // set variable in class to resulting bool of findVariable();
    }

    return success;
}

bool inputParser::loadAllInputs(std::string inputFileName)
{

    // run does file exist function on inputFileName. If not, warning is in function so exit. If so, open this input file as the file to pass around to all the loader functions

    // now go through each input variable determining which loader function to use and loading with the needed loader function

    // hm, it might feel nicer to always pass around the filename instead of the file. Need to see how they do it in farsite

    bool success = true;

    for(size_t varIdx = 0; varIdx < inputVariablesInfo.size(); varIdx++)
    {
        // big if statement checking variableCountType and using the proper load function.
        //  If type is count, which comes last, then another big if statement using loader function names to determine which loader function to use.
        //   If it reaches the else, no loader function is used so give a warning to the user/programmer that it is an allowed/required input,
        //   but it is not really loaded or used anymore. So any format would go, just need to trick the input loader.

    }

    return success;
}
/*** end main loader functions ***/



/***** private functions *****/



/*** other loader functions ***/
/*** end other loader functions ***/
