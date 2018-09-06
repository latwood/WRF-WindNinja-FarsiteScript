#include "stringValue.h"

/***** public functions *****/

/*** constructor functions ***/
stringValue::stringValue(std::string newVariableName)
{
    variableName = newVariableName;
    storedStringValue = defaultStringValue;
}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void stringValue::reset()
{
    storedStringValue = defaultStringValue;
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool stringValue::set_storedStringValue(std::string newStringValue)
{
    bool success = true;

    storedStringValue = newStringValue;

    return success;
}
/*** end set value functions ***/

/*** get value functions ***/
std::string stringValue::get_variableName()
{
    return variableName;
}

std::string stringValue::get_storedStringValue()
{
    return storedStringValue;
}
/*** end get value functions ***/



/***** private functions *****/



/*** reconstructor like functions ***/
/*** end reconstructor like functions ***/
