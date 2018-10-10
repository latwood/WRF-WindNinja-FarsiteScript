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
    //bool set_storedHour_MinValue_string(std::string newStringHourValue, std::string newStringMinuteValue);
    bool set_storedHour_MinValue_string(std::string newHourMinValue);

    // get value functions
    std::string get_variableName();
    int get_storedHourValue();
    int get_storedMinuteValue();

private:

    // type conversion functions
    bool strToHour_Min(std::string inputString, int &currentHour, int &currentMinute);
    int strToInt(std::string inputString, bool &conversionSuccess);

    // typechecker functions
    bool isValidDate(int currentYear, int currentMonth, int currentDay, int currentHour, int currentMinute);
    bool isInt(std::string inputString);

    // data members
    std::string variableName;
    int storedHourValue;
    int storedMinuteValue;

    // default values
    int defaultHourValue = 0;
    int defaultMinuteValue = 0;

    struct dateInfo
    {
        std::vector<int> daysPerMonth = {31,28,31,30,31,30,31,31,30,31,30,31};
        // simplified case: leap years are when the year mod 4 is zero, except if year mod 100 is 0 and year mod 400 is not 0
        // this is true for all years past probably sometime in the 1600's. Also not sure how this works with foreign calendars
        int hour_min = 0;
        int hour_max = 23;
        int minute_min = 0;
        int minute_max = 59;
    };
    dateInfo validDates;

};

#endif // HOUR_MINVALUE_H
