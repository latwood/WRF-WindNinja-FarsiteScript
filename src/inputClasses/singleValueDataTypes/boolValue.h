#ifndef BOOLVALUE_H
#define BOOLVALUE_H

#include <iostream>
#include <vector>

class boolValue
{

public:

    // constructor
    boolValue(std::string newVariableName);

    // reconstructor like functions
    void reset();

    // set value functions
    void set_storedBoolValue(bool newBoolValue);
    bool set_storedBoolValue_string(std::string newStringBoolValue);

    // get value functions
    std::string get_variableName();
    bool get_storedBoolValue();

private:

    // type conversion functions
    bool strToBool(std::string inputString, bool &conversionSuccess);

    // typechecker functions
    bool isBool(std::string inputString);

    // data members
    std::string variableName;
    bool storedBoolValue;

    // default values
    bool defaultBoolValue = false;

};

#endif // BOOLVALUE_H
