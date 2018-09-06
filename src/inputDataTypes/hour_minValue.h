#ifndef HOUR_MINVALUE_H
#define HOUR_MINVALUE_H

#include <iostream>
#include <vector>

class hour_minValue
{

public:

    // constructor
    hour_minValue(std::string newVariableName);

    // reconstructor like functions
    void reset();

    // set value functions
    bool set_storedHour_MinValue(int newHourValue, int newMinuteValue);

    // get value functions
    std::string get_variableName();
    int get_storedHourValue();
    int get_storedMinuteValue();

private:

    // reconstructor like functions

    // data members
    std::string variableName;
    int storedHourValue;
    int storedMinuteValue;

    // default values
    int defaultHourValue = 0;
    int defaultMinuteValue = 0;

};

#endif // HOUR_MINVALUE_H
