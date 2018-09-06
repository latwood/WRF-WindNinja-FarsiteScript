#include "pathNameValue.h"

/***** public functions *****/

/*** constructor functions ***/
pathNameValue::pathNameValue(std::string newVariableName)
{
    variableName = newVariableName;
    storedPathNameValue = defaultPathNameValue;
}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void pathNameValue::reset()
{
    storedPathNameValue = defaultPathNameValue;
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool pathNameValue::set_storedPathNameValue(std::string newPathnameValue)
{
    bool success = true;

    storedPathNameValue = newPathnameValue;

    return success;
}
/*** end set value functions ***/

/*** get value functions ***/
std::string pathNameValue::get_variableName()
{
    return variableName;
}

std::string pathNameValue::get_storedPathNameValue()
{
    return storedPathNameValue;
}
/*** end get value functions ***/



/***** private functions *****/



/*** reconstructor like functions ***/
/*** end reconstructor like functions ***/
