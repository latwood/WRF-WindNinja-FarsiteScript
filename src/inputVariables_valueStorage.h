#ifndef INPUTVARIABLES_VALUESTORAGE_H
#define INPUTVARIABLES_VALUESTORAGE_H


#include "inputVariables_infoStorage.h"

struct dateValue // yeah, definitely going to either declare all types globally, or just avoid this weird problem of passing strange types between classes. Now I've got the type internal to the class and this external for all classes so I can pass the type on to others. Originally just did one line without defining this type, and was fine till I got to try an output function with this type, then compiler got really really angry. Ugh, declaring this section cleared many more errors, but heck now I've got issues with type conversions between two definitions. Has to be global or nothing, not two copies one global and one in class seems like.
{
    std::string variableName;
    int year;
    int month;
    int day;
    int hour;
    int minute;
};    // might have accidentally placed this into the global scope, might actually be better to have all these data member declarations done this way actually, as then I wouldn't need a second copy of the data storage declaration struct stuff when the values are desired

class inputVariables_valueStorage
{
public:

    // constructors
    inputVariables_valueStorage();

    // reinit functions (like constructors)
    void resetVariables();     //kind of a reusable constructor/destructor

    // get variable info functions
    std::vector<inputVariable_info> get_inputVariableInfo();
    std::string get_maxVarNameColumnWhitespace();

    // set variable value functions
    bool set_inputVariableBoolValue(std::string varName, bool newBoolValue);
    bool set_inputVariableIntValue(std::string varName, int newIntValue);
    bool set_inputVariableDoubleValue(std::string varName, double newDoubleValue);
    bool set_inputVariableStringValue(std::string varName, std::string newStringValue);
    bool set_inputVariableFilenameValue(std::string varName, std::string newFilenameValue);
    bool set_inputVariableDateValue(std::string varName, int year, int month, int day, int hour, int minute);    // something like this, though notice that sometimes year isn't included, so maybe have a -999 value to warn it isn't actually there
    // bool set_inputVariableCountValues(std::string varName, needed data);     // I guess it will need a separate function for each required data, probably can skip varName, the loader function in the input parser class will have to know which variable it is setting it to based off of the varName, won't necessarily need it here

    // load function stuff for count type variables
    bool add_wrf_file(std::string new_wrf_file);

    // get variable value functions
    bool get_inputVariableBoolValue(std::string varName);
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

    // setup functions
    bool sortInputVariableInfo();

    // class data members
    inputVariables_infoStorage setupInputVariableInfo;
    std::vector<inputVariable_info> inputVariableInfo;

    // boolean variable value struct and vector of this struct
    struct boolValue
    {
        std::string variableName;
        bool variableValue;
    };
    bool defaultBoolValue = false;
    std::vector<boolValue> boolValues;

    // int variable value struct and vector of this struct
    struct intValue
    {
        std::string variableName;
        int variableValue;
    };
    int defaultIntValue = 0;
    std::vector<intValue> intValues;

    // double variable value struct and vector of this struct
    struct doubleValue
    {
        std::string variableName;
        double variableValue;
    };
    double defaultDoubleValue = 0.0;
    std::vector<doubleValue> doubleValues;

    // string variable value struct and vector of this struct
    struct stringValue
    {
        std::string variableName;
        std::string variableValue;
    };
    std::string defaultStringValue = "";
    std::vector<stringValue> stringValues;

    // filename variable value struct and vector of this struct
    struct filenameValue
    {
        std::string variableName;
        std::string variableValue;
    };
    std::string defaultFilenameValue = "";
    std::vector<filenameValue> filenameValues;

    // date variable value struct and vector of this struct
    // commented out this type because while it is great when used locally, trying to use it for get functions causes tons of obnoxious compiler errors
    // basically it says type is not completely defined and splits between the global and class type. If you copy this define twice,
    // it then complains about type conversion between the two. So I guess you have either in class or global, not both, of a given type.
    // So not easy to setup special self declared pass types between classes, unless type is defined globally.
    // So get more inventive, split this type getter function into multiple getter functions for each part of the type!!!
    /*struct dateValue
    {
        std::string variableName;
        int year;
        int month;
        int day;
        int hour;
        int minute;
    };*/
    int defaultYearValue = 0;
    int defaultMonthValue = 0;
    int defaultDayValue = 0;
    int defaultHourValue = 0;
    int defaultMinuteValue = 0;
    std::vector<dateValue> dateValues;


    // variable value of type count data members, all kinds of data types from vectors to structs
    // I tried to keep variable value of type count data member names to be the same as the variable info variable names
    std::vector<std::string> wrf_files;

};

#endif // INPUTVARIABLES_VALUESTORAGE_H
