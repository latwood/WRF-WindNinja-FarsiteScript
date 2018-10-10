#ifndef WRFFILEVALUE_H
#define WRFFILEVALUE_H

#include <iostream>
#include <vector>

#include "gdal.h"
//#include "cpl_conv.h"   // for CPLMalloc()
#include "gdal_priv.h"

class wrfFileValue
{

public:

    // constructor
    wrfFileValue(std::string newVariableName);

    // reconstructor like functions
    void reset();

    // set value functions
    bool set_storedWrfFileName(std::string newFilenameValue);

    // get value functions
    std::string get_variableName();
    std::string get_storedWrfFileName();

private:

    // typechecker functions
    bool isValidNetCDFFilename(std::string inputString, bool suppressWarnings);

    // data members
    std::string variableName;
    std::string storedWrfFileName;

    // default values
    std::string defaultWrfFileName = "";

};

#endif // WRFFILEVALUE_H
