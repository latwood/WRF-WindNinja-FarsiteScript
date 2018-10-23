#ifndef WRFFILESTORAGE_H
#define WRFFILESTORAGE_H


#include <iostream>
#include <vector>

#include "../singleValueDataTypes/wrfFileValue.h"

class wrfFileStorage
{

public:

    // constructor
    wrfFileStorage();

    // reconstructor like functions
    void reset();

    // set value functions
    bool add_wrfFile(std::string new_wrf_file);
    bool add_wrfFile_Line(std::string inputDataLine);

    // get value functions
    std::vector<wrfFileValue> get_storedWrfFiles();

private:

    // reconstructor like functions

    // data members
    std::vector<wrfFileValue> storedWrfFiles;

};

#endif // WRFFILESTORAGE_H
