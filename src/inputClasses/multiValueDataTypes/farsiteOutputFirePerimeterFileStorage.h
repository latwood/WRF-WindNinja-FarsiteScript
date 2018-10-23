#ifndef FARSITEOUTPUTFIREPERIMETERFILESTORAGE_H
#define FARSITEOUTPUTFIREPERIMETERFILESTORAGE_H

#include <iostream>
#include <vector>

#include "../singleValueDataTypes/shapeFileValue.h"

class farsiteOutputFirePerimeterFileStorage
{

public:

    // constructor
    farsiteOutputFirePerimeterFileStorage();

    // reconstructor like functions
    void reset();

    // set value functions
    bool add_farsiteOutputFirePerimeterFile(std::string new_farsite_output_fire_perimeter_file);
    bool add_farsiteOutputFirePerimeterFile_Line(std::string inputDataLine);

    // get value functions
    std::vector<shapeFileValue> get_storedFarsiteOutputFirePerimeterFiles();

private:

    // reconstructor like functions

    // data members
    std::vector<shapeFileValue> storedFarsiteOutputFirePerimeterFiles;

};

#endif // FARSITEOUTPUTFIREPERIMETERFILESTORAGE_H
