#ifndef INPUTVARIABLESHANDLER_H
#define INPUTVARIABLESHANDLER_H


#include "inputVariables_valueStorage.h"

#include "inputParser.h"


class inputVariablesHandler
{
public:

    // constructors
    inputVariablesHandler();

    // functions
    void explainInputReqs();
    bool loadScriptInputs(std::string inputFileName);
    void printFoundInput();

    // will probably have to repeat the functions from the inputVariable_valueStorage class unfortunately.
    // Difference is the values won't be stored in this class, just in the one referenced here
    // get variable value functions
    bool get_inputVariableBoolValue(std::string varName);
    size_t get_inputVariableSize_tValue(std::string varName);
    int get_inputVariableIntValue(std::string varName);
    double get_inputVariableDoubleValue(std::string varName);
    std::string get_inputVariableStringValue(std::string varName);
    std::string get_inputVariableFilenameValue(std::string varName);
    dateValue get_inputVariableDateValue(std::string varName);
    int get_inputVariableDateValueYear(std::string varName);
    int get_inputVariableDateValueMonth(std::string varName);
    int get_inputVariableDateValueDay(std::string varName);
    int get_inputVariableDateValueHour(std::string varName);
    int get_inputVariableDateValueMinute(std::string varName);
    // datatype get_inputVariableCountValues(std::string varName); // definitely don't think this kind of generic type idea will work for counts, they are way too complex! Need separate getter for each type of count variable, maybe even multiple getters for said variables if the function that uses them doesn't want to define something to unravel all the values out
    std::vector<std::string> get_wrf_files();

private:

    // functions
    void reset();
    bool checkSetVarNamesForConflictingOptions();

    // class data members
    std::vector<inputVariable_info> inputVariableInfo;
    inputVariables_valueStorage inputVariableValues;
    inputParser theInputParser;

};

#endif // INPUTVARIABLESHANDLER_H
