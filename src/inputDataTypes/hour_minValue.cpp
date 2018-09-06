#include "hour_minValue.h"

/***** public functions *****/

/*** constructor functions ***/
hour_minValue::hour_minValue(std::string newVariableName)
{
    variableName = newVariableName;
    storedHourValue = defaultHourValue;
    storedMinuteValue = defaultHourValue;
}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void hour_minValue::reset()
{
    storedHourValue = defaultHourValue;
    storedMinuteValue = defaultHourValue;
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool hour_minValue::set_storedHour_MinValue(int newHourValue, int newMinuteValue)
{
    bool success = true;

    storedHourValue = newHourValue;
    storedMinuteValue = newMinuteValue;

    return success;
}

/*** end set value functions ***/

/*** get value functions ***/
std::string hour_minValue::get_variableName()
{
    return variableName;
}

int hour_minValue::get_storedHourValue()
{
    return storedHourValue;
}

int hour_minValue::get_storedMinuteValue()
{
    return storedMinuteValue;
}
/*** end get value functions ***/



/***** private functions *****/



/*** reconstructor like functions ***/
/*** end reconstructor like functions ***/
