#ifndef STRINGVALUE_H
#define STRINGVALUE_H

#include <iostream>
#include <vector>

class stringValue
{

public:

    // constructor
    stringValue(std::string newVariableName);

    // reconstructor like functions
    void reset();

    // set value functions
    bool set_storedStringValue(std::string newStringValue);

    // get value functions
    std::string get_variableName();
    std::string get_storedStringValue();

private:

    // reconstructor like functions

    // data members
    std::string variableName;
    std::string storedStringValue;

    // default values
    std::string defaultStringValue = "";

};

#endif // STRINGVALUE_H
