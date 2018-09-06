#ifndef LAT_LONGVALUE_H
#define LAT_LONGVALUE_H

#include <iostream>
#include <vector>

class lat_longValue
{

public:

    // constructor
    lat_longValue(std::string newVariableName);

    // reconstructor like functions
    void reset();

    // set value functions
    bool set_storedLatLongValue(double newLatValue, double newLongValue);

    // get value functions
    std::string get_variableName();
    double get_storedLatValue();
    double get_storedLongValue();


private:

    // reconstructor like functions

    // data members
    std::string variableName;
    double storedLatValue;
    double storedLongValue;

    // default values
    double defaultLatValue = 0.0;
    double defaultLongValue = 0.0;

};

#endif // LAT_LONGVALUE_H
