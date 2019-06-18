#include "singleInputVariable.h"

/***** public functions *****/

/*** constructor functions ***/
singleInputVariable::singleInputVariable(std::string newVariableName,std::string newApplicationUseName,std::string newVariableCountType,std::string newVariableDescription)
{
    variableName = newVariableName;
    applicationUseName = newApplicationUseName;
    // type is allowed to be abstract, so type setting and checking is handled by class that creates or uses a vector of these configVariables
    variableCountType = newVariableCountType;
    // no conflictingVariables because this sort of verification will be checked by the classes that use vectors of configVariables. Uses wantDefaultValue to know if it went well or no
    variableDescription = newVariableDescription;
    variableTypeClass = NULL;
    isFoundInInputFile = false;
    wantDefaultValue = false;
    inputFileVariableCountLine = 0;
}
/*** end constructor functions ***/

/*** reinit functions ***/
void singleInputVariable::resetVariable()
{
    isFoundInInputFile = false;
    wantDefaultValue = false;
    inputFileVariableCountLine = 0;
}
/*** end reinit functions ***/

/*** set new value functions ***/
void singleInputVariable::set_variableTypeClass(void* new_variableTypeClass)
{
    // int firstvalue = 5, secondvalue = 15;
    // int * p1, * p2;
    // p1 = &firstvalue;  // p1 = address of firstvalue
    // p2 = &secondvalue; // p2 = address of secondvalue
    // *p1 = 10;          // value pointed to by p1 = 10
    // p1 = p2;           // p1 = p2 (value of pointer is copied)
    // *p1 = 20;          // value pointed to by p1 = 20
    if(variableTypeClass == NULL)
    {
        variableTypeClass = new_variableTypeClass; // value of pointer is copied, make sure to delete incoming pointer in whatever called this once done
    } else
    {
        printf("variable \"%s\" already has its variableTypeClass pointer set! the variableTypeClass pointer of a variable can only be set at construction time!\n",variableName.c_str());
        printf("exiting program!\n");
        exit(1);   // an error, can only set the pointer once, then it is expected to always point to a specific value storage class of whatever type is first chosen
    }
}

void singleInputVariable::set_isFoundInInputFile(bool new_isFoundInInputFileValue, size_t new_inputFileVariableCountLine)
{
    isFoundInInputFile = new_isFoundInInputFileValue;    //note don't need to check because it is already a bool coming in or it fails
    inputFileVariableCountLine = new_inputFileVariableCountLine;
}

void singleInputVariable::set_wantDefaultValue(bool new_wantDefaultValue)
{
    wantDefaultValue = new_wantDefaultValue;
}

void singleInputVariable::set_variableNameWhiteSpace(std::string new_variableNameWhiteSpace)
{
    variableNameWhiteSpace = new_variableNameWhiteSpace;
}

void singleInputVariable::add_variableDescriptionLineBreaks(unsigned int new_variableDescriptionLineBreak)
{
    variableDescriptionLineBreaks.push_back(new_variableDescriptionLineBreak);
}
/*** end set new value functions ***/


/*** get value functions ***/
std::string singleInputVariable::get_variableName()
{
    return variableName;
}

std::string singleInputVariable::get_applicationUseName()
{
    return applicationUseName;
}

std::string singleInputVariable::get_variableCountType()
{
    return variableCountType;
}

std::string singleInputVariable::get_variableDescription()
{
    return variableDescription;
}

void* singleInputVariable::get_variableTypeClass()
{
    return variableTypeClass;
}

bool singleInputVariable::get_isFoundInInputFile()
{
    return isFoundInInputFile;
}

bool singleInputVariable::get_wantDefaultValue()
{
    return wantDefaultValue;
}

size_t singleInputVariable::get_inputFileVariableCountLine()
{
    return inputFileVariableCountLine;
}

std::string singleInputVariable::get_variableNameWhiteSpace()
{
    return variableNameWhiteSpace;
}

std::vector<unsigned int> singleInputVariable::get_variableDescriptionLineBreaks()
{
    return variableDescriptionLineBreaks;
}
/*** end get value functions ***/



/***** private functions *****/
