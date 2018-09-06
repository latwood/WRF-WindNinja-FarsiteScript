#include "intValue.h"

/***** public functions *****/

/*** constructor functions ***/
intValue::intValue(std::string newVariableName)
{
    variableName = newVariableName;
    storedIntValue = defaultIntValue;
}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void intValue::reset()
{
    storedIntValue = defaultIntValue;
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool intValue::set_storedIntValue(int newIntValue)
{
    bool success = true;

    storedIntValue = newIntValue;

    return success;
}
/*** end set value functions ***/

/*** get value functions ***/
std::string intValue::get_variableName()
{
    return variableName;
}

int intValue::get_storedIntValue()
{
    return storedIntValue;
}
/*** end get value functions ***/



/***** private functions *****/



/*** reconstructor like functions ***/
/*** end reconstructor like functions ***/
