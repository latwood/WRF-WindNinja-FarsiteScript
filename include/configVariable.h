#ifndef CONFIGOPTION_H
#define CONFIGVARIABLE_H


#include <iostream>
#include <vector>

class configVariable
{
public:

    // constructors
    configVariable(std::string newVariableName,std::string newVariableCountType,std::vector<std::string> newConflictingVariables,
                   std::string newLoaderFunctionName,std::string newVariableDescription);

    // reinit functions (like constructors)
    void resetVariable();     //kind of a reusable constructor/destructor

    // set new value functions
    void set_isFoundInInputFile(bool newIsFoundInInputFileValue);
    void set_doesVariableConflict(bool newDoesVariableConflictValue);

    // get value functions
    std::string get_variableName();
    std::string get_variableCountType();
    std::vector<std::string> get_conflictingVariables();
    std::string get_loaderFunctionName();
    std::string get_variableDescription();
    bool get_isFoundInInputFile();
    bool get_doesVariableConflict();

private:

    // data members
    std::string variableName;
    std::string variableCountType;
    std::vector<std::string> conflictingVariables;    //basically fill this with the variable names of other options that can't go along with, a single NA if no other variables conflict with this variable.
    std::string loaderFunctionName;
    std::string variableDescription;
    bool isFoundInInputFile;
    bool doesVariableConflict;

};

#endif // CONFIGVARIABLE_H
