#include "farsiteOutputFirePerimeterFileStorage.h"

/***** public functions *****/

/*** constructor functions ***/
farsiteOutputFirePerimeterFileStorage::farsiteOutputFirePerimeterFileStorage()
{
}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void farsiteOutputFirePerimeterFileStorage::reset()
{
    while(!storedFarsiteOutputFirePerimeterFiles.empty())
    {
        storedFarsiteOutputFirePerimeterFiles.pop_back();
    }
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool farsiteOutputFirePerimeterFileStorage::add_farsiteOutputFirePerimeterFile(std::string new_farsite_output_fire_perimeter_file)
{
    printf("adding farsite_output_fire_perimeter_files \"%s\" to farsite_output_fire_perimeter_files variable\n",new_farsite_output_fire_perimeter_file.c_str());
    shapeFileValue new_shapeFileValue("");
    new_shapeFileValue.set_storedShapeFileValue(new_farsite_output_fire_perimeter_file);
    storedFarsiteOutputFirePerimeterFiles.push_back(new_shapeFileValue);

    // if it reaches here, it worked correctly
    return true;
}
/*** end set value functions ***/

/*** get value functions ***/
std::vector<shapeFileValue> farsiteOutputFirePerimeterFileStorage::get_storedFarsiteOutputFirePerimeterFiles()
{
    return storedFarsiteOutputFirePerimeterFiles;
}
/*** end get value functions ***/



/***** private functions *****/



/*** reconstructor like functions ***/
/*** end reconstructor like functions ***/
