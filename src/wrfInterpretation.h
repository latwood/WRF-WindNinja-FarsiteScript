#ifndef WRFINTERPRETATION_H
#define WRFINTERPRETATION_H


#include "inputVariablesHandler.h"

#include "WindNinjaAPI.h"
#include "wrfGetWeather.h"

class wrfInterpretation
{
public:

    // constructors
    wrfInterpretation();

    // run everything function
    bool interpretWRFfiles(inputVariablesHandler *inputs);

    // these are a repeat of the others
    // WindNinjaAPI get value functions
    std::vector<std::string> get_atmFilePaths();
    std::vector<std::string> get_velFilePaths();
    std::vector<std::string> get_angFilePaths();
    std::vector<std::string> get_cldFilePaths();

    // wrfGetWeather get value functions
    std::vector<double> get_temperatures();
    std::vector<double> get_humidities();
    std::vector<double> get_totalPrecip();
    std::vector<double> get_cloudCover();

private:

    // class type data members
    WindNinjaAPI WindNinjaClass;
    wrfGetWeather wrfWeatherClass;

};

#endif // WRFINTERPRETATION_H
