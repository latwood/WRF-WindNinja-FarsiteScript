#include "createIgnitionFromLatLongsStorage.h"

/***** public functions *****/

/*** constructor functions ***/
createIgnitionFromLatLongsStorage::createIgnitionFromLatLongsStorage()
{

}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void createIgnitionFromLatLongsStorage::reset()
{
    while(!storedCreateIgnitionLatLongValues.empty())
    {
        storedCreateIgnitionLatLongValues.pop_back();
    }
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool createIgnitionFromLatLongsStorage::add_createIgnitionLatLongValue(double newLat_CoordValue, double newLong_CoordValue)
{
    printf("adding create_ignition_from_latlongs \"%f %f\" (lat long) to create_ignition_from_latlongs variable\n",newLat_CoordValue,newLong_CoordValue);
    lat_longValue new_lat_longValue("");
    new_lat_longValue.set_storedLatLongValue(newLat_CoordValue,newLong_CoordValue);
    storedCreateIgnitionLatLongValues.push_back(new_lat_longValue);

    // if it reaches here, it worked correctly
    return true;
}
/*** end set value functions ***/

/*** get value functions ***/
std::vector<lat_longValue> createIgnitionFromLatLongsStorage::get_storedCreateIgnitionLatLongValues()
{
    return storedCreateIgnitionLatLongValues;
}
/*** end get value functions ***/



/***** private functions *****/



/*** reconstructor like functions ***/
/*** end reconstructor like functions ***/
