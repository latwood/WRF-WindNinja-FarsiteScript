#ifndef INPUTVARIABLES_INFOSTORAGE_H
#define INPUTVARIABLES_INFOSTORAGE_H


#include <iostream>
#include <vector>

#include "inputVariable_info.h"


// largest configVariable name will have this much space between its end and the description
#define MIN_VARNAME_WHITESPACE 4
// minimum allowable words per line for a description
#define MIN_WORDS_PER_DESCRIPTION_LINE 5
// largest allowed variable name and MIN_VARNAME_WHITESPACE size, to allow enough room for the description
#define DESCRIPTION_VARIABLENAME_COLUMNSIZEMAX 30
// this should be the maximum number of characters in ubuntu 16.04 terminal line when the terminal is first opened
#define MAX_DESCRIPTION_LINESIZE 80


class inputVariables_infoStorage
{
public:

    // constructors
    inputVariables_infoStorage();

    // get value functions
    std::vector<inputVariable_info> get_inputVariableInfo();
    std::string get_maxVarNameColumnWhitespace();

private:

    // setup functions
    void setupAvailableApplicationUseNames();
    void setupAvailableVariableCountTypes();
    void addVariable(std::string newVariableName,std::string newApplicationUseName,std::string newVariableCountType,
                     std::string newLoaderFunctionName,std::string newVariableDescription); //make this private so that it is only called by constructor
    void setupAvailableVariables();


    // check setup functions
    bool check_setupForDuplicateVariableNames();
    bool check_setupForValidApplicationUseNames();
    bool check_setupForValidOrderingByApplicationUseNames();
    bool check_setupForValidVariableCountTypes();
    bool check_setupLoaderFunctionNames();
    bool check_setupDescription();

    // description whitespace and line break calculations, with error checking
    bool calculateDescriptionWhiteSpace();
    bool calculateDescriptionLineBreaks();
    void calculate_maxVarNameColumnWhitespace();

    // class data members
    std::vector<inputVariable_info> inputVariables;

    // variable info data members
    std::vector<std::string> allowedApplicationUseNames;
    std::vector<std::string> allowedVariableCountTypes;

    // for description printing
    unsigned int descriptionVariableNameColumnSize;
    std::string maxVarNameColumnWhitespace;

};

#endif // INPUTVARIABLES_INFOSTORAGE_H
