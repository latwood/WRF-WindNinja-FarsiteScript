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

    // get value functions
    std::string get_variableName();
    bool get_storedBoolValue();

private:

    // reconstructor like functions

    // data members
    std::string variableName;
    bool storedBoolValue;

    // default values
    bool defaultBoolValue = false;

};

#endif // BOOLVALUE_H
