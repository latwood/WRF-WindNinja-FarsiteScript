#ifndef INPUTVARIABLES_VALUESTORAGE_H
#define INPUTVARIABLES_VALUESTORAGE_H


#include "inputVariables_infoStorage.h"


class inputVariables_valueStorage
{
public:

    // constructors
    inputVariables_valueStorage();

    // get variable info functions
    std::vector<inputVariable_info> get_inputVariableInfo();
    std::string get_maxVarNameColumnWhitespace();

    // set variable value functions
    bool set_inputVariableBoolValue(std::string varName, bool boolValue);
    bool set_inputVariableIntValue(std::string varName, int intValue);
    bool set_inputVariableDoubleValue(std::string varName, double doubleValue);
    bool set_inputVariableStringValue(std::string varName, std::string stringValue);
    bool set_inputVariableFilenameValue(std::string varName, std::string filenameValue);
    // bool set_inputVariableDateValue(std::string varName, int year, int month, int day, int hour);    // something like this, though notice that sometimes year isn't included, so maybe have a -999 value to warn it isn't actually there
    // bool set_inputVariableCountValues(std::string varName, needed data);     // I guess it will need a separate function for each required data, probably can skip varName, the loader function in the input parser class will have to know which variable it is setting it to based off of the varName, won't necessarily need it here
    bool add_wrf_file(std::string new_wrf_file);

    // get variable value functions
    bool get_inputVariableBoolValue(std::string varName);
    int get_inputVariableIntValue(std::string varName);
    double get_inputVariableDoubleValue(std::string varName);
    std::string get_inputVariableStringValue(std::string varName);
    std::string get_inputVariableFilenameValue(std::string varName);
    // date get_inputVariableDateValue(std::string varName);   // not sure how to set this up otherwise to output a single datatype and not need multiple functions for each part of the date
    // datatype get_inputVariableCountValues(std::string varName); // definitely don't think this kind of generic type idea will work for counts, they are way too complex! Need separate getter for each type of count variable, maybe even multiple getters for said variables if the function that uses them doesn't want to define something to unravel all the values out
    std::vector<std::string> get_wrf_files();   // hm, maybe the get value functions will actually be done directly to here then, not the input handler? No the input handler should use a simpler function that grabs these guys, so the complex part will probably go in here. For now assume it is simple and directly grab the values

private:

    // class data members
    inputVariables_infoStorage setupInputVariableInfo;
    std::vector<inputVariable_info> inputVariableInfo;

    // variable value data members
    std::vector<std::string> wrf_files;

};

#endif // INPUTVARIABLES_VALUESTORAGE_H
