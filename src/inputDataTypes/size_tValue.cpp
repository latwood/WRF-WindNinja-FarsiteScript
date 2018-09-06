#include "size_tValue.h"

/***** public functions *****/

/*** constructor functions ***/
size_tValue::size_tValue(std::string newVariableName)
{
    variableName = newVariableName;
    storedSize_tValue = defaultSize_tValue;
}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void size_tValue::reset()
{
    storedSize_tValue = defaultSize_tValue;
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool size_tValue::set_storedSize_tValue(size_t newSize_tValue)
{
    bool success = true;

    storedSize_tValue = newSize_tValue;

    return success;
}
/*** end set value functions ***/

/*** get value functions ***/
std::string size_tValue::get_variableName()
{
    return variableName;
}

size_t size_tValue::get_storedSize_tValue()
{
    return storedSize_tValue;
}
/*** end get value functions ***/



/***** private functions *****/



/*** reconstructor like functions ***/
/*** end reconstructor like functions ***/
