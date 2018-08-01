#ifndef SCRIPTCONFIGVARIABLES_H
#define SCRIPTCONFIGVARIABLES_H

#include <iostream>
#include <vector>

#include "configVariable.h"


// largest configVariable name will have this much space between its end and the description
#define MIN_VARNAME_WHITESPACE 4
// minimum allowable words per line for a description
#define MIN_WORDS_PER_DESCRIPTION_LINE 5
// largest allowed variable name and MIN_VARNAME_WHITESPACE size, to allow enough room for the description
#define DESCRIPTION_VARIABLENAME_COLUMNSIZEMAX 30
// this should be the maximum number of characters in ubuntu 16.04 terminal line when the terminal is first opened
#define MAX_DESCRIPTION_LINESIZE 80

class scriptConfigVariables
{
public:

    // constructors
    scriptConfigVariables();

    // get value functions
    std::vector<configVariable> get_theVariables();
    std::string get_maxVarNameColumnWhitespace();

private:

    // setup functions
    void setupAvailableApplicationUseNames();
    void setupAvailableVariableCountTypes();
    void addVariable(std::string newVariableName,std::string newApplicationUseName,std::string newVariableCountType,
                     std::vector<std::string> newConflictingVariables,std::string newLoaderFunctionName,std::string newVariableDescription); //make this private so that it is only called by constructor
    void setupAvailableVariables();


    // check setup functions
    bool check_setupForDuplicateVariableNames();
    bool check_setupForValidApplicationUseNames();
    bool check_setupForValidOrderingByApplicationUseNames();
    bool check_setupForValidVariableCountTypes();
    bool check_setupConflictingVariables();
    bool check_setupDescription();

    // description whitespace and line break calculations, with error checking
    bool calculateDescriptionWhiteSpace();
    bool calculateDescriptionLineBreaks();
    void calculate_maxVarNameColumnWhitespace();

    // data members
    std::vector<configVariable> theVariables;
    std::vector<std::string> allowedVariableCountTypes;
    std::vector<std::string> allowedApplicationUseNames;
    // for description printing
    unsigned int descriptionVariableNameColumnSize;
    std::string maxVarNameColumnWhitespace;


};

#endif // SCRIPTCONFIGVARIABLES_H
