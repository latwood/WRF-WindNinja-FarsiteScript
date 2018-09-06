#include "lcpFileValue.h"

/***** public functions *****/

/*** constructor functions ***/
lcpFileValue::lcpFileValue(std::string newVariableName)
{
    variableName = newVariableName;
    storedLcpFileValue = defaultLcpFileValue;
}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void lcpFileValue::reset()
{
    storedLcpFileValue = defaultLcpFileValue;
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool lcpFileValue::set_storedLcpFileValue(std::string newFilenameValue)
{
    bool success = true;

    storedLcpFileValue = newFilenameValue;

    return success;
}

/*** end set value functions ***/

/*** get value functions ***/
std::string lcpFileValue::get_variableName()
{
    return variableName;
}

std::string lcpFileValue::get_storedLcpFileValue()
{
    return storedLcpFileValue;
}
/*** end get value functions ***/



/***** private functions *****/



/*** reconstructor like functions ***/
/*** end reconstructor like functions ***/
