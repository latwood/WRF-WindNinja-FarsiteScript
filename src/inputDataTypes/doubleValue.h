#ifndef DOUBLEVALUE_H
#define DOUBLEVALUE_H

#include <iostream>
#include <vector>

class doubleValue
{

public:

    // constructor
    doubleValue(std::string newVariableName);

    // reconstructor like functions
    void reset();

    // set value functions
    bool set_storedDoubleValue(double newDoubleValue, std::string doubleType);

    // get value functions
    std::string get_variableName();
    double get_storedDoubleValue();

private:

    // reconstructor like functions

    // data members
    std::string variableName;
    double storedDoubleValue;

    // default values
    double defaultDoubleValue = 0.0;

};

#endif // DOUBLEVALUE_H
