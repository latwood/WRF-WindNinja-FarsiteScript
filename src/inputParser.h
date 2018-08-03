#ifndef INPUTPARSER_H
#define INPUTPARSER_H

#include "inputVariables_valueStorage.h"


#define  input_Sep  ':'  /* used on the end of every switch. Since this has to do with how input is parsed, this is the right class location as methods for parsing are defined here  */


class inputParser
{
public:

    // constructors
    inputParser();

    // variable class getter functions
    void loadVariableInfo(inputVariables_valueStorage* newInputVariables);

    // main loader functions
    bool findVariableNames(std::string inputFileName);
    bool loadAllInputs(std::string inputFileName);

private:

    // other loader functions
    //bool findVariableCount(char variableCountType, bool isIntTypeCountVar);   // something similar to this, will be used by loadAllInputs()

    // class related data members
    inputVariables_valueStorage* inputVariableValueStorage;
    std::vector<inputVariable_info> inputVariablesInfo;

};

#endif // INPUTPARSER_H
