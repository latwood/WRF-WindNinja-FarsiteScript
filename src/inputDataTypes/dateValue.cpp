#include "dateValue.h"

/***** public functions *****/

/*** constructor functions ***/
dateValue::dateValue(std::string newVariableName)
{
    variableName = newVariableName;
    storedYearValue = defaultYearValue;
    storedMonthValue = defaultMonthValue;
    storedDayValue = defaultDayValue;
    storedHourValue = defaultHourValue;
    storedMinuteValue = defaultMinuteValue;
}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void dateValue::reset()
{
    storedYearValue = defaultYearValue;
    storedMonthValue = defaultMonthValue;
    storedDayValue = defaultDayValue;
    storedHourValue = defaultHourValue;
    storedMinuteValue = defaultMinuteValue;
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool dateValue::set_storedDateValue(int newYearValue, int newMonthValue, int newDayValue, int newHourValue, int newMinuteValue)
{
    bool success = true;

    storedYearValue = newYearValue;
    storedMonthValue = newMonthValue;
    storedDayValue = newDayValue;
    storedHourValue = newHourValue;
    storedMinuteValue = newMinuteValue;

    return success;
}
/*** end set value functions ***/

/*** get value functions ***/
std::string dateValue::get_variableName()
{
    return variableName;
}

int dateValue::get_storedYearValue()
{
    return storedYearValue;
}

int dateValue::get_storedMonthValue()
{
    return storedMonthValue;
}

int dateValue::get_storedDayValue()
{
    return storedDayValue;
}

int dateValue::get_storedHourValue()
{
    return storedHourValue;
}

int dateValue::get_storedMinuteValue()
{
    return storedMinuteValue;
}
/*** end get value functions ***/



/***** private functions *****/



/*** reconstructor like functions ***/
/*** end reconstructor like functions ***/
