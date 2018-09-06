#ifndef PATHNAMEVALUE_H
#define PATHNAMEVALUE_H

#include <iostream>
#include <vector>

class pathNameValue
{

public:

    // constructor
    pathNameValue(std::string newVariableName);

    // reconstructor like functions
    void reset();

    // set value functions
    bool set_storedPathNameValue(std::string newPathNameValue);

    // get value functions
    std::string get_variableName();
    std::string get_storedPathNameValue();

private:

    // reconstructor like functions

    // data members
    std::string variableName;
    std::string storedPathNameValue;

    // default values
    std::string defaultPathNameValue = "";

};

#endif // PATHNAMEVALUE_H
