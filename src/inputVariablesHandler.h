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
    bool loadScriptInputs();

    /* will probably have to repeat the functions from the inputVariable_valueStorage class unfortunately. Difference is the values won't be stored in this class, just in the one referenced here
    // get variable value functions
    bool get_inputVariableBoolValue(std::string varName);
    int get_inputVariableIntValue(std::string varName);
    double get_inputVariableDoubleValue(std::string varName);
    std::string get_inputVariableStringValue(std::string varName);
    std::string get_inputVariableFilenameValue(std::string varName);
    // date get_inputVariableDateValue(std::string varName);   // not sure how to set this up otherwise to output a single datatype and not need multiple functions for each part of the date
    // datatype get_inputVariableCountValues(std::string varName); // definitely don't think this kind of generic type idea will work for counts, they are way too complex! Need separate getter for each type of count variable, maybe even multiple getters for said variables if the function that uses them doesn't want to define something to unravel all the values out
    std::vector<std::string> get_wrf_files();   // hm, maybe the get value functions will actually be done directly to here then, not the input handler? No the input handler should use a simpler function that grabs these guys, so the complex part will probably go in here. For now assume it is simple and directly grab the values
    */

private:

    // functions
    void reset();
    bool findOptionConflicts();
    bool checkConflictingOptions();

    // class data members
    std::vector<inputVariable_info> inputVariableInfo;
    inputVariables_valueStorage inputVariableValues;
    inputParser theInputParser;

};

#endif // INPUTVARIABLESHANDLER_H
