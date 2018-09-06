#ifndef SIZE_TVALUE_H
#define SIZE_TVALUE_H

#include <iostream>
#include <vector>

class size_tValue
{

public:

    // constructor
    size_tValue(std::string newVariableName);

    // reconstructor like functions
    void reset();

    // set value functions
    bool set_storedSize_tValue(size_t newSize_tValue);

    // get value functions
    std::string get_variableName();
    size_t get_storedSize_tValue();

private:

    // reconstructor like functions

    // data members
    std::string variableName;
    size_t storedSize_tValue;

    // default values
    size_t defaultSize_tValue = 0;

};

#endif // SIZE_TVALUE_H
