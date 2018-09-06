#ifndef LCPFILEVALUE_H
#define LCPFILEVALUE_H

#include <iostream>
#include <vector>

class lcpFileValue
{

public:

    // constructor
    lcpFileValue(std::string newVariableName);

    // reconstructor like functions
    void reset();

    // set value functions
    bool set_storedLcpFileValue(std::string newFilenameValue);

    // get value functions
    std::string get_variableName();
    std::string get_storedLcpFileValue();

private:

    // reconstructor like functions

    // data members
    std::string variableName;
    std::string storedLcpFileValue;

    // default values
    std::string defaultLcpFileValue = "";

};

#endif // LCPFILEVALUE_H
