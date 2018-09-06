#include "polygonIgnitShapeFileStorage.h"

/***** public functions *****/

/*** constructor functions ***/
polygonIgnitShapeFileStorage::polygonIgnitShapeFileStorage()
{
}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void polygonIgnitShapeFileStorage::reset()
{
    while(!storedPolygonIgnitShapeFiles.empty())
    {
        storedPolygonIgnitShapeFiles.pop_back();
    }
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool polygonIgnitShapeFileStorage::add_polygonIgnitShapeFile(std::string new_polygon_ignit_shape_file)
{
    printf("adding polygon_ignit_shape_files \"%s\" to polygon_ignit_shape_files variable\n",new_polygon_ignit_shape_file.c_str());
    shapeFileValue new_shapeFileValue("");
    new_shapeFileValue.set_storedShapeFileValue(new_polygon_ignit_shape_file);
    storedPolygonIgnitShapeFiles.push_back(new_shapeFileValue);

    // if it reaches here, it worked correctly
    return true;
}
/*** end set value functions ***/

/*** get value functions ***/
std::vector<shapeFileValue> polygonIgnitShapeFileStorage::get_storedPolygonIgnitShapeFiles()
{
    return storedPolygonIgnitShapeFiles;
}
/*** end get value functions ***/



/***** private functions *****/



/*** reconstructor like functions ***/
/*** end reconstructor like functions ***/
