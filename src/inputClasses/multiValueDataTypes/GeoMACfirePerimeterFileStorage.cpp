#include "GeoMACfirePerimeterFileStorage.h"

/***** public functions *****/

/*** constructor functions ***/
GeoMACfirePerimeterFileStorage::GeoMACfirePerimeterFileStorage()
{

}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void GeoMACfirePerimeterFileStorage::reset()
{
    while(!storedGeoMACfirePerimeterFiles.empty())
    {
        storedGeoMACfirePerimeterFiles.pop_back();
    }
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool GeoMACfirePerimeterFileStorage::add_GeoMACfirePerimeterFile(std::string new_GeoMAC_fire_perimeter_file)
{
    printf("adding GeoMAC_fire_perimeter_files \"%s\" to GeoMAC_fire_perimeter_files variable\n",new_GeoMAC_fire_perimeter_file.c_str());
    shapeFileValue new_shapeFileValue("");
    new_shapeFileValue.set_storedShapeFileValue(new_GeoMAC_fire_perimeter_file);
    storedGeoMACfirePerimeterFiles.push_back(new_shapeFileValue);

    // if it reaches here, it worked correctly
    return true;
}
/*** end set value functions ***/

/*** get value functions ***/
std::vector<shapeFileValue> GeoMACfirePerimeterFileStorage::get_storedGeoMACfirePerimeterFiles()
{
    return storedGeoMACfirePerimeterFiles;
}
/*** end get value functions ***/



/***** private functions *****/



/*** reconstructor like functions ***/
/*** end reconstructor like functions ***/
