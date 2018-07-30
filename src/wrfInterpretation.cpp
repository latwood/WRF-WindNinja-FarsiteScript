#include "wrfInterpretation.h"

wrfInterpretation::wrfInterpretation()
{

}

bool wrfInterpretation::interpretWRFfiles(scriptInputs *inputs)
{
    bool success = true;
    /* first take necessary input information and store it somehow, probably validating that it is good in the moment if that isn't already done with scriptInputs loading */

    /* need a for loop to go over all the WRF files, running windninja first time (probably outside loop) to download lcp file, other times using current lcp file. Also does gdal/netcdf grab weather info */
    if(!runWindNinja())
    {
        printf("WindNinja run for wrf file . . . failed!\n");
        success = false;
    }
    if(!getWeatherInfo())
    {
        printf("getWeatherInfo for wrf file . . . failed!\n");
        success = false;
    }
    /* end for loop stuff */
    return success;
}

bool wrfInterpretation::runWindNinja()
{
    bool success = true;

    return success;
}

bool wrfInterpretation::getWeatherInfo()
{
    bool success = true;

    return success;
}
