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
    //bool set_storedLatLongValue_string(std::string newStringLatValue, std::string newStringLongValue);
    bool set_storedLatLongValue_string(std::string newLatLongValueString);

    // get value functions
    std::string get_variableName();
    double get_storedLatValue();
    double get_storedLongValue();


private:

    // type conversion functions
    double strToLat_Coord(std::string inputString, bool &conversionSuccess);
    double strToLong_Coord(std::string inputString, bool &conversionSuccess);
    bool strToLat_Long_Point(std::string inputString, double &currentLat_Coord, double &currentLong_Coord);

    // typechecker functions
    bool isDbl(std::string inputString);

    // data members
    std::string variableName;
    double storedLatValue;
    double storedLongValue;

    // default values
    double defaultLatValue = 0.0;
    double defaultLongValue = 0.0;

};

#endif // LAT_LONGVALUE_H
