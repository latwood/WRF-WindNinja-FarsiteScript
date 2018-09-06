#ifndef CREATEIGNITIONFROMLATLONGSSTORAGE_H
#define CREATEIGNITIONFROMLATLONGSSTORAGE_H


#include <iostream>
#include <vector>

#include "lat_longValue.h"

class createIgnitionFromLatLongsStorage
{

public:

    // constructor
    createIgnitionFromLatLongsStorage();

    // reconstructor like functions
    void reset();

    // set value functions
    bool add_createIgnitionLatLongValue(double newLat_CoordValue, double newLong_CoordValue);

    // get value functions
    std::vector<lat_longValue> get_storedCreateIgnitionLatLongValues();

private:

    // reconstructor like functions

    // data members
    std::vector<lat_longValue> storedCreateIgnitionLatLongValues;

};

#endif // CREATEIGNITIONFROMLATLONGSSTORAGE_H
