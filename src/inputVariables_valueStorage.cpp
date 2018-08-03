#include "inputVariables_valueStorage.h"

/***** public functions *****/

/*** constructor functions ***/
inputVariables_valueStorage::inputVariables_valueStorage()
{
     inputVariableInfo = setupInputVariableInfo.get_inputVariableInfo();
     // now should run a bunch of functions that setup storage containers for all the different variables that include the variable names to help tell them apart.
     // Even if storage containers for count types end up having the same name as the variables, can still store a string of the variable name for error handling and printing stuff
     //   maybe could even setup a function that goes through all the structs for the needed variable name, wait the data return changes each time even though structs
     //   are the same datatype so that's why too restrictive to try this idea instead of separate functions for each count type variable

     //   I just tried mixing this class with the base inputVariablesHandler and it got ugly disorganized real fast. I think it will be easier to just repeat the get functions that will
     //   be created in this class, unless I can find a workaround. It just keeps it way way more organized I think, even though there will be some annoying repetition
}
/*** end constructor functions ***/


/*** get variable info functions ***/
std::vector<inputVariable_info> inputVariables_valueStorage::get_inputVariableInfo()
{
    return inputVariableInfo;
}

std::string inputVariables_valueStorage::get_maxVarNameColumnWhitespace()
{
    return setupInputVariableInfo.get_maxVarNameColumnWhitespace();
}
/*** end get variable info functions ***/

/*** set variable value functions ***/
/*** end set variable value functions ***/

/*** get variable value functions ***/
/*** end get variable value functions ***/



/***** private functions *****/



/*** functions ***/
/*** end functions ***/
