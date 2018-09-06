#include "boolValue.h"

/***** public functions *****/

/*** constructor functions ***/
boolValue::boolValue(std::string newVariableName)
{
    variableName = newVariableName;
    storedBoolValue = defaultBoolValue;
}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void boolValue::reset()
{
    storedBoolValue = defaultBoolValue;
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
void boolValue::set_storedBoolValue(bool newBoolValue)
{
    storedBoolValue = newBoolValue;
}
/*** end set value functions ***/

/*** get value functions ***/
std::string boolValue::get_variableName()
{
    return variableName;
}

bool boolValue::get_storedBoolValue()
{
    return storedBoolValue;
}
/*** end get value functions ***/



/***** private functions *****/



/*** reconstructor like functions ***/
/*** end reconstructor like functions ***/
