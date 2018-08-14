#ifndef WRFGETWEATHER_H
#define WRFGETWEATHER_H

#include <iostream>

#include "inputVariablesHandler.h"

#include "netcdf.h"
#include "gdalwarper.h"

class wrfGetWeather
{

public:

    // constructors
    wrfGetWeather();

    // reconstructor like functions
    bool load_required_inputs(inputVariablesHandler *inputs);   // should pass in some kind of data that is the largest fire perimeter lat long coordinates

    // functions
    bool getWeather();

private:

    // reconstructor like functions
    void reset();

    // useful utility functions
    bool getNetCDFGlobalAttributes(std::string wrfFile, int &mapProj, float &dx, float &dy, float &cenLat, float &cenLon, float &moadCenLat,
                                   float &standLon, float &trueLat1, float &trueLat2, int &nLayers);
    bool findWrfCenterValue(GDALDataset *poDS, int band, double &foundValue);

    // data members needed from the inputs
    std::string actualCreateInputs_path;
        // application specific variables
    std::string wrfGetWeather_output_units;
        // lcp download variables
    std::string lcp_file_path;
        // wrfGetWeather and WindNinja variables
    bool extend_wrf_data;
    std::vector<std::string> wrf_files;
        // wrfGetWeather only variables
    bool use_weather_from_ignition_center;
    bool use_weather_from_full_ignition_area;
    bool use_weather_from_wrf_center;
    // may need some ignition and lcp related variables too if doing the more complex wrf weather positioning tricks

    // data members created from inputs that are API specific

    /*
     * turns out it doesn't make sense to create these except in the moment IF needing more than one value from the dataset
    std::vector<std::vector<double> > AllFoundTemperatures;
    std::vector<std::vector<double> > AllFoundHumidities;
    std::vector<std::vector<double> > AllFoundCumulusPrecip;
    std::vector<std::vector<double> > AllFoundGridSaclePrecip;
    std::vector<std::vector<double> > AllFoundCloudCover;
    std::vector<std::vector<double> > totalPrecip;
    */

    std::vector<double> temperatures;
    std::vector<double> humidities;
    std::vector<double> totalPrecip;
    std::vector<double> cloudCover;

};

#endif // WRFGETWEATHER_H
