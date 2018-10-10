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
    bool set_storedDoubleValue_string(std::string newStringDoubleValue, std::string doubleType);

    // get value functions
    std::string get_variableName();
    double get_storedDoubleValue();

private:

    // type conversion functions
    double strToDbl(std::string inputString, bool &conversionSuccess);
    double strToPositiveDbl(std::string inputString, bool &conversionSuccess);
    double strToSignlessPercent(std::string inputString, bool &conversionSuccess);
    double strToLat_Coord(std::string inputString, bool &conversionSuccess);
    double strToLong_Coord(std::string inputString, bool &conversionSuccess);

    // typechecker functions
    bool isDbl(std::string inputString);

    // data members
    std::string variableName;
    double storedDoubleValue;

    // default values
    double defaultDoubleValue = 0.0;

};

#endif // DOUBLEVALUE_H
