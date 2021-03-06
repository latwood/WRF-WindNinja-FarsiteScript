#ifndef POLYGONIGNITSHAPEFILESTORAGE_H
#define POLYGONIGNITSHAPEFILESTORAGE_H


#include <iostream>
#include <vector>

#include "../singleValueDataTypes/shapeFileValue.h"

class polygonIgnitShapeFileStorage
{

public:

    // constructor
    polygonIgnitShapeFileStorage();

    // reconstructor like functions
    void reset();

    // set value functions
    bool add_polygonIgnitShapeFile(std::string new_polygon_ignit_shape_file);
    bool add_polygonIgnitShapeFile_Line(std::string inputDataLine);

    // get value functions
    std::vector<shapeFileValue> get_storedPolygonIgnitShapeFiles();

private:

    // reconstructor like functions

    // data members
    std::vector<shapeFileValue> storedPolygonIgnitShapeFiles;

};

#endif // POLYGONIGNITSHAPEFILESTORAGE_H
