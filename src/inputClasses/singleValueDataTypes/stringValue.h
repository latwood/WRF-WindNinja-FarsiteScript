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
    bool set_storedStringValue(std::string newParsedStringValue, std::string varName);

    // get value functions
    std::string get_variableName();
    std::string get_storedStringValue();

private:

    // typechecker functions
    bool isPureString(std::string inputString);
    bool isValidString(std::string inputString, std::string varName);
    bool isPureFilenameString(std::string inputString);

    // data members
    std::string variableName;
    std::string storedStringValue;

    // default values
    std::string defaultStringValue = "";

};

#endif // STRINGVALUE_H
