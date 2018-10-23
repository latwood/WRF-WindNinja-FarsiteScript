#ifndef INPUTPARSER_H
#define INPUTPARSER_H

#include "inputVariables_valueStorage.h"

#include <fstream>

class inputParser
{
public:

    // constructors
    inputParser();

    // variable class getter functions
    void loadVariableInfo(inputVariables_valueStorage* newInputVariables);

    // data passer between inputVariablesHandler and inputParser functions
    // man I know there is a smarter way and it involves pointers and probably copy constructors, but I'm in a hurry
    std::vector<inputVariable_info> transferVariableInfo();

    // main loader functions
    bool readInputFile(std::string inputFileName);
    bool readVarNamesAndCountValues();
    bool loadCountValues();
    bool checkCountVsLinesOfData();
    bool loadLoaderFunctionData();

private:

    // parsing utility functions
    bool doesRegularFileExist(std::string fileName);
    bool openIsStream(std::string fileName,std::ifstream &fzStream);
    void removeLeadingWhitespace(std::string &inputString);
    void removeEndingWhitespace(std::string &inputString);
    void removeSeparator(std::string inputString, std::string &varNamePart, std::string &varCountPart);

    // class related data members
    inputVariables_valueStorage* inputVariableValueStorage;
    std::vector<inputVariable_info> inputVariablesInfo;

    // important parser data members
    std::string inputSeparator = "=";
    std::string commentChar = "#";
    std::vector<std::string> foundInputLines;
    std::vector<std::string> foundLineTypes;
    std::vector<std::string> foundVarNames;
    std::vector<size_t> foundVarNames_inputVariableInfoIndices;
    std::vector<std::string> foundVarCountStrings;

};

#endif // INPUTPARSER_H
