#ifndef GEOMACFIREPERIMETERFILESTORAGE_H
#define GEOMACFIREPERIMETERFILESTORAGE_H

#include <iostream>
#include <vector>

#include "../singleValueDataTypes/shapeFileValue.h"

class GeoMACfirePerimeterFileStorage
{

public:

    // constructor
    GeoMACfirePerimeterFileStorage();

    // reconstructor like functions
    void reset();

    // set value functions
    bool add_GeoMACfirePerimeterFile(std::string new_GeoMAC_fire_perimeter_file);

    // get value functions
    std::vector<shapeFileValue> get_storedGeoMACfirePerimeterFiles();

private:

    // reconstructor like functions

    // data members
    std::vector<shapeFileValue> storedGeoMACfirePerimeterFiles;

};

#endif // GEOMACFIREPERIMETERFILESTORAGE_H
