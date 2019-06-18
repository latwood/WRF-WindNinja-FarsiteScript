#ifndef SINGLEINPUTVARIABLE_H
#define SINGLEINPUTVARIABLE_H


#include <iostream>
#include <vector>

class singleInputVariable
{
public:

    // constructors
    singleInputVariable(std::string new_variableName,std::string new_applicationUseName,std::string new_variableCountType,std::string new_variableDescription);

    // reinit functions (like constructors)
    void resetVariable();     //kind of a reusable constructor/destructor

    // set new value functions
    void set_variableTypeClass(void* new_variableTypeClass);
    void set_isFoundInInputFile(bool new_isFoundInInputFileValue, size_t new_iputFileVariableCountLine);
    void set_wantDefaultValue(bool new_wantDefaultValue);
    void set_variableNameWhiteSpace(std::string new_variableNameWhiteSpace);
    void add_variableDescriptionLineBreaks(unsigned int new_variableDescriptionLineBreak);

    // get value functions
    std::string get_variableName();
    std::string get_applicationUseName();
    std::string get_variableCountType();
    std::string get_variableDescription();
    void* get_variableTypeClass();
    bool get_isFoundInInputFile();
    bool get_wantDefaultValue();
    size_t get_inputFileVariableCountLine();
    std::string get_variableNameWhiteSpace();
    std::vector<unsigned int> get_variableDescriptionLineBreaks();

private:

    // put these here if you want to see all the problems that happen when you don't have a deep copy or deep assignment
    //singleInputVariable(const singleInputVariable &old_singleInputVariable);  // copy constructor
    //singleInputVariable & operator=(const singleInputVariable &old_singleInputVariable);  // assignment operator

    // data members
    std::string variableName;
    std::string applicationUseName;
    std::string variableCountType;
    std::string variableDescription;
    void* variableTypeClass;
    bool isFoundInInputFile;
    bool wantDefaultValue;
    size_t inputFileVariableCountLine;
    std::string variableNameWhiteSpace;
    std::vector<unsigned int> variableDescriptionLineBreaks;

};

#endif // SINGLEINPUTVARIABLE_H
