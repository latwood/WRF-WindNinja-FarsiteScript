#ifndef INPUTVARIABLE_INFO_H
#define INPUTVARIABLE_INFO_H


#include <iostream>
#include <vector>

class inputVariable_info
{
public:

    // constructors
    inputVariable_info(std::string newVariableName,std::string newApplicationUseName,std::string newVariableCountType,std::string newVariableDescription);

    // reinit functions (like constructors)
    void resetVariable();     //kind of a reusable constructor/destructor

    // set new value functions
    void set_isFoundInInputFile(bool newIsFoundInInputFileValue, size_t newInputFileVariableCountLine);
    void set_wantDefaultValue(bool newWantDefaultValue);
    void set_variableNameWhiteSpace(std::string newVariableNameWhiteSpace);
    void add_variableDescriptionLineBreaks(unsigned int newVariableDescriptionLineBreak);

    // get value functions
    std::string get_variableName();
    std::string get_applicationUseName();
    std::string get_variableCountType();
    std::string get_variableDescription();
    bool get_isFoundInInputFile();
    bool get_wantDefaultValue();
    size_t get_inputFileVariableCountLine();
    std::string get_variableNameWhiteSpace();
    std::vector<unsigned int> get_variableDescriptionLineBreaks();

private:

    // data members
    std::string variableName;
    std::string applicationUseName;
    std::string variableCountType;
    std::string variableDescription;
    bool isFoundInInputFile;
    bool wantDefaultValue;
    size_t inputFileVariableCountLine;
    std::string variableNameWhiteSpace;
    std::vector<unsigned int> variableDescriptionLineBreaks;

};

#endif // INPUTVARIABLE_INFO_H
