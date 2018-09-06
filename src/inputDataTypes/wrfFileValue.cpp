#include "wrfFileValue.h"

/***** public functions *****/

/*** constructor functions ***/
wrfFileValue::wrfFileValue(std::string newVariableName)
{
    variableName = newVariableName;
    storedWrfFileName = defaultWrfFileName;
}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void wrfFileValue::reset()
{
    storedWrfFileName = defaultWrfFileName;
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool wrfFileValue::set_storedWrfFileName(std::string newFilenameValue)
{
    bool success = true;

    storedWrfFileName = newFilenameValue;

    return success;
}

/*** end set value functions ***/

/*** get value functions ***/
std::string wrfFileValue::get_variableName()
{
    return variableName;
}

std::string wrfFileValue::get_storedWrfFileName()
{
    return storedWrfFileName;
}
/*** end get value functions ***/



/***** private functions *****/



/*** reconstructor like functions ***/
/*** end reconstructor like functions ***/
