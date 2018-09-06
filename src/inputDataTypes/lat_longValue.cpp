#include "lat_longValue.h"

/***** public functions *****/

/*** constructor functions ***/
lat_longValue::lat_longValue(std::string newVariableName)
{
    variableName = newVariableName;
    storedLatValue = defaultLatValue;
    storedLongValue = defaultLongValue;
}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void lat_longValue::reset()
{
    storedLatValue = defaultLatValue;
    storedLongValue = defaultLongValue;
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool lat_longValue::set_storedLatLongValue(double newLat_CoordValue, double newLong_CoordValue)
{
    bool success = true;

    storedLatValue = newLat_CoordValue;
    storedLongValue = newLong_CoordValue;

    return success;
}
/*** end set value functions ***/

/*** get value functions ***/
std::string lat_longValue::get_variableName()
{
    return variableName;
}

double lat_longValue::get_storedLatValue()
{
    return storedLatValue;
}

double lat_longValue::get_storedLongValue()
{
    return storedLongValue;
}
/*** end get value functions ***/



/***** private functions *****/



/*** reconstructor like functions ***/
/*** end reconstructor like functions ***/
