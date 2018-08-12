#include "inputVariable_info.h"

/***** public functions *****/

/*** constructor functions ***/
inputVariable_info::inputVariable_info(std::string newVariableName,std::string newApplicationUseName,std::string newVariableCountType,
                                       std::string newLoaderFunctionName,std::string newVariableDescription)
{
    variableName = newVariableName;
    applicationUseName = newApplicationUseName;
    // type is allowed to be abstract, so type setting and checking is handled by class that creates or uses a vector of these configVariables
    variableCountType = newVariableCountType;
    // no conflictingVariables because this sort of verification will be checked by the classes that use vectors of configVariables
    loaderFunctionName = newLoaderFunctionName;
    variableDescription = newVariableDescription;
    isFoundInInputFile = false;
    wantDefaultValue = false;
    inputFileVariableCountLine = 0;
}
/*** end constructor functions ***/

/*** reinit functions ***/
void inputVariable_info::resetVariable()
{
    isFoundInInputFile = false;
    wantDefaultValue = false;
    inputFileVariableCountLine = 0;
    variableNameWhiteSpace = "";
    while(!variableDescriptionLineBreaks.empty())
    {
        variableDescriptionLineBreaks.pop_back();
    }

}
/*** end reinit functions ***/

/*** set new value functions ***/
void inputVariable_info::set_isFoundInInputFile(bool newIsFoundInInputFileValue, size_t newInputFileVariableCountLine)
{
    isFoundInInputFile = newIsFoundInInputFileValue;    //note don't need to check because it is already a bool coming in or it fails
    inputFileVariableCountLine = newInputFileVariableCountLine;
}

void inputVariable_info::set_wantDefaultValue(bool newWantDefaultValue)
{
    wantDefaultValue = newWantDefaultValue;
}

void inputVariable_info::set_variableNameWhiteSpace(std::string newVariableNameWhiteSpace)
{
    variableNameWhiteSpace = newVariableNameWhiteSpace;
}

void inputVariable_info::add_variableDescriptionLineBreaks(unsigned int newVariableDescriptionLineBreak)
{
    variableDescriptionLineBreaks.push_back(newVariableDescriptionLineBreak);
}
/*** end set new value functions ***/


/*** get value functions ***/
std::string inputVariable_info::get_variableName()
{
    return variableName;
}

std::string inputVariable_info::get_applicationUseName()
{
    return applicationUseName;
}

std::string inputVariable_info::get_variableCountType()
{
    return variableCountType;
}

std::string inputVariable_info::get_loaderFunctionName()
{
    return loaderFunctionName;
}

std::string inputVariable_info::get_variableDescription()
{
    return variableDescription;
}

bool inputVariable_info::get_isFoundInInputFile()
{
    return isFoundInInputFile;
}

bool inputVariable_info::get_wantDefaultValue()
{
    return wantDefaultValue;
}

size_t inputVariable_info::get_inputFileVariableCountLine()
{
    return inputFileVariableCountLine;
}

std::string inputVariable_info::get_variableNameWhiteSpace()
{
    return variableNameWhiteSpace;
}

std::vector<unsigned int> inputVariable_info::get_variableDescriptionLineBreaks()
{
    return variableDescriptionLineBreaks;
}
/*** end get value functions ***/



/***** private functions *****/
