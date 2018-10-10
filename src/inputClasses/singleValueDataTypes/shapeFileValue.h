#ifndef SHAPEFILEVALUE_H
#define SHAPEFILEVALUE_H

#include <iostream>
#include <vector>

#include "gdal.h"

class shapeFileValue
{

public:

    // constructor
    shapeFileValue(std::string newVariableName);

    // reconstructor like functions
    void reset();

    // set value functions
    bool set_storedShapeFileValue(std::string newFilenameValue);

    // get value functions
    std::string get_variableName();
    std::string get_storedShapeFileValue();

private:

    // typechecker functions
    bool isValidShapeFilename(std::string inputString, bool suppressWarnings);

    // data members
    std::string variableName;
    std::string storedShapeFileValue;

    // default values
    std::string defaultShapeFileValue = "";

};

#endif // SHAPEFILEVALUE_H
