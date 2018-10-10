#ifndef INTVALUE_H
#define INTVALUE_H

#include <iostream>
#include <vector>

class intValue
{

public:

    // constructor
    intValue(std::string newVariableName);

    // reconstructor like functions
    void reset();

    // set value functions
    bool set_storedIntValue(int newIntValue);
    bool set_storedIntValue_string(std::string newStringIntValue);

    // get value functions
    std::string get_variableName();
    int get_storedIntValue();

private:

    // type conversion functions
    int strToInt(std::string inputString, bool &conversionSuccess);

    // typechecker functions
    bool isInt(std::string inputString);

    // data members
    std::string variableName;
    int storedIntValue;

    // default values
    int defaultIntValue = 0;

};

#endif // INTVALUE_H
