#include "configVariable.h"

/***** public functions *****/

/*** constructor functions ***/
configVariable::configVariable(std::string newVariableName,std::string newVariableCountType,std::vector<std::string> newConflictingVariables,
                               std::string newLoaderFunctionName,std::string newVariableDescription)
{
    variableName = newVariableName;
    // type is allowed to be abstract, so type setting and checking is handled by class that creates a vector of these configVariables
    variableCountType = newVariableCountType;
    // no initial check on conflictingVariables because value will be a string meaning a reference to another variable. This will be checked by the classes that setup and use bectors of configVariables
    conflictingVariables = newConflictingVariables;
    loaderFunctionName = newLoaderFunctionName;
    variableDescription = newVariableDescription;
    isFoundInInputFile = false;
    doesVariableConflict = false;
}
/*** end constructor functions ***/

/*** reinit functions ***/
void configVariable::resetVariable()
{
    /* in this case, no values are stored, so it should just be resetting the booleans. */
    /* Would be nice to add in type stuff at some point, but it is just too complex. Would need some kind of lookup table
     *  to determine the type of value coming in and store something accordingly, and value could be a class or struct as well as simple stuff */
    isFoundInInputFile = false;
    doesVariableConflict = false;
}
/*** end reinit functions ***/

/*** set new value functions ***/
void configVariable::set_isFoundInInputFile(bool newIsFoundInInputFileValue)
{
    isFoundInInputFile = newIsFoundInInputFileValue;    //note don't need to check because it is already a bool coming in or it fails
}

void configVariable::set_doesVariableConflict(bool newDoesVariableConflictValue)
{
    doesVariableConflict = newDoesVariableConflictValue;  //note don't need to check because it is already a bool coming in or it fails
}
/*** end set new value functions ***/


/*** get value functions ***/
std::string configVariable::get_variableName()
{
    return variableName;
}

std::string configVariable::get_variableCountType()
{
    return variableCountType;
}

std::vector<std::string> configVariable::get_conflictingVariables()
{
    return conflictingVariables;
}

std::string configVariable::get_loaderFunctionName()
{
    return loaderFunctionName;
}

std::string configVariable::get_variableDescription()
{
    return variableDescription;
}

bool configVariable::get_isFoundInInputFile()
{
    return isFoundInInputFile;
}

bool configVariable::get_doesVariableConflict()
{
    return doesVariableConflict;
}
/*** end get value functions ***/



/***** private functions *****/


