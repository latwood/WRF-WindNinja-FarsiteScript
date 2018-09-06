#ifndef DATEVALUE_H
#define DATEVALUE_H


#include <iostream>
#include <vector>

class dateValue
{

public:

    // constructor
    dateValue(std::string newVariableName);

    // reconstructor like functions
    void reset();

    // set value functions
    bool set_storedDateValue(int newYearValue, int newMonthValue, int newDayValue, int newHourValue, int newMinuteValue);

    // get value functions
    std::string get_variableName();
    int get_storedYearValue();
    int get_storedMonthValue();
    int get_storedDayValue();
    int get_storedHourValue();
    int get_storedMinuteValue();

private:

    // reconstructor like functions

    // data members
    std::string variableName;
    int storedYearValue;
    int storedMonthValue;
    int storedDayValue;
    int storedHourValue;
    int storedMinuteValue;

    // default values
    int defaultYearValue = 0;
    int defaultMonthValue = 0;
    int defaultDayValue = 0;
    int defaultHourValue = 0;
    int defaultMinuteValue = 0;

};

#endif // DATEVALUE_H
