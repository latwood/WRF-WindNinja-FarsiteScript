#ifndef SCRIPTCONFIGVARIABLES_H
#define SCRIPTCONFIGVARIABLES_H

#include <iostream>
#include <vector>

#include "configVariable.h"

class scriptConfigVariables
{
public:

    // constructors
    scriptConfigVariables();

    // get value functions
    std::vector<configVariable> get_theVariables();

private:

    // setup functions
    void setupAvailableVariableCountTypes();
    void addVariable(std::string newVariableName,std::string newVariableCountType,std::vector<std::string> newConflictingVariables,
                     std::string newLoaderFunctionName,std::string newVariableDescription); //might make this private so that it is only called by constructor
    void setupAvailableVariables();

    // check setup functions
    bool check_setupDuplicateVariables();
    bool check_setupVariableCountType();
    bool check_setupConflictingVariables();

    // data members
    std::vector<configVariable> theVariables;
    std::vector<std::string> allowedVariableCountTypes;

};

#endif // SCRIPTCONFIGVARIABLES_H
