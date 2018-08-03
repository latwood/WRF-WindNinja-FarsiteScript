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
    void setupAvailableVariableCountAmounts();
    void setupAvailableVariableCountTypes();
    void addVariable(std::string newVariableName,std::string newApplicationUseName,std::string newVariableCountType,
                     std::vector<std::string> newConflictingVariables,std::string newLoaderFunctionName,std::string newVariableDescription); //make this private so that it is only called by constructor
    void setupAvailableVariables();


    // check setup functions
    bool check_setupForDuplicateVariableNames();
    bool check_setupForValidApplicationUseNames();
    bool check_setupForValidOrderingByApplicationUseNames();
    bool check_setupAllowableVariableCountAmounts();
    bool check_setupForValidVariableCountTypes();
    bool check_setupConflictingVariables();
    bool check_setupLoaderFunctionNames();
    bool check_setupDescription();

    // description whitespace and line break calculations, with error checking
    bool calculateDescriptionWhiteSpace();
    bool calculateDescriptionLineBreaks();
    void calculate_maxVarNameColumnWhitespace();

    // utility functions used by everything else
    std::string findCountAmountFromCountType(std::string availableCountType);

    // class data members
    std::vector<inputVariable_info> inputVariables;

    // variable info data members
    std::vector<std::string> allowedApplicationUseNames;
    std::vector<std::string> allowedVariableCountAmounts;
        /* start struct type stuff, probs should make this and other parts relating to it to be a class to make it more organized someday */
        struct count
        {
            std::string countType;
            std::string countAmount;
        };
        std::vector<count> allowedVariableCountTypes;
        /* end struct variables */
    // for description printing
    unsigned int descriptionVariableNameColumnSize;
    std::string maxVarNameColumnWhitespace;

};

#endif // INPUTVARIABLES_INFOSTORAGE_H
