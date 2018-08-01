#include "configVariable.h"

/***** public functions *****/

/*** constructor functions ***/
configVariable::configVariable(std::string newVariableName,std::string newApplicationUseName,std::string newVariableCountType,
                               std::vector<std::string> newConflictingVariables,std::string newLoaderFunctionName,std::string newVariableDescription)
{
    variableName = newVariableName;
    applicationUseName = newApplicationUseName;
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
    isFoundInInputFile = false;
    doesVariableConflict = false;
    variableNameWhiteSpace = "";
    while(!variableDescriptionLineBreaks.empty())
    {
        variableDescriptionLineBreaks.pop_back();
    }

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

void configVariable::set_variableNameWhiteSpace(std::string newVariableNameWhiteSpace)
{
    variableNameWhiteSpace = newVariableNameWhiteSpace;
}

void configVariable::add_variableDescriptionLineBreaks(unsigned int newVariableDescriptionLineBreak)
{
    variableDescriptionLineBreaks.push_back(newVariableDescriptionLineBreak);
}
/*** end set new value functions ***/


/*** get value functions ***/
std::string configVariable::get_variableName()
{
    return variableName;
}

std::string configVariable::get_applicationUseName()
{
    return applicationUseName;
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

std::string configVariable::get_variableNameWhiteSpace()
{
    return variableNameWhiteSpace;
}

std::vector<unsigned int> configVariable::get_variableDescriptionLineBreaks()
{
    return variableDescriptionLineBreaks;
}
/*** end get value functions ***/



/***** private functions *****/


