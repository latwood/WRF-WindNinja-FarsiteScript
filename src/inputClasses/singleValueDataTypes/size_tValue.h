#ifndef SIZE_TVALUE_H
#define SIZE_TVALUE_H

#include <iostream>
#include <vector>

class size_tValue
{

public:

    // constructors
    size_tValue();
    size_tValue(std::string newVariableName);

    // reconstructor like functions
    void reset();

    // set value functions
    void set_variableName(std::string newVariableName);
    bool set_storedSize_tValue(size_t newSize_tValue);
    bool set_storedSize_tValue_string(std::string newStringSize_tValue);

    // get value functions
    std::string get_variableName();
    size_t get_storedSize_tValue();

private:

    // type conversion functions
    size_t strToSize_t(std::string inputString, bool &conversionSuccess);

    // typechecker functions
    bool isSize_t(std::string inputString);

    // data members
    std::string variableName;
    size_t storedSize_tValue;

    // default values
    size_t defaultSize_tValue = 0;

};

#endif // SIZE_TVALUE_H
