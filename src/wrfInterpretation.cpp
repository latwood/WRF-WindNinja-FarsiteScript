#include "wrfInterpretation.h"

/***** public functions *****/

/*** constructor functions ***/
wrfInterpretation::wrfInterpretation()
{

}
/*** end constructor functions ***/

/*** run everything functions ***/
bool wrfInterpretation::interpretWRFfiles(inputVariablesHandler *inputs)
{
    printf("\nrunning wrfInterpretation class\n");
    // first do WindNinja stuff, then wrfGetWeather stuff

    printf("\nloading inputs into WindNinjaAPI\n");
    // load inputs into WindNinjaApi class
    if(WindNinjaClass.load_required_inputs(inputs) == false)
    {
        printf("failed to load inputs into WindNinjaAPI class!\n");
        return false;
    }

    /*printf("\ncreating WindNinja cfg files\n");
    // now create WindNinja cfg files
    if(WindNinjaClass.create_WindNinja_cfg_files() == false)
    {
        printf("failed to create all WindNinja cfg files!\n");
        return false;
    }*/

    /*printf("\nrunning WindNinja!\n");
    // now run WindNinja
    if(WindNinjaClass.run_WindNinja() == false)
    {
        printf("problems running WindNinja!\n");
        return false;
    }*/

    // okay now ready to do the wrfWeatherInterpretation stuff

    printf("\nloading inputs into wrfGetWeather class\n");
    // first load in the inputs
    if(wrfWeatherClass.load_required_inputs(inputs) == false)
    {
        printf("failed to load inputs into wrfGetWeather class!\n");
        return false;
    }

    printf("\nrunning wrfGetWeather!\n");
    // now run getWeather to get weather data from all netcdf files
    if(wrfWeatherClass.getWeather() == false)
    {
        printf("problems running wrfGetWeather!\n");
        return false;
    }

    printf("\nfinished wrfInterpretation class!\n");
    // got to the end successfully, so return true
    return true;
}
/*** end run everything functions ***/

/*** WindNinjaAPI get value functions ***/
std::vector<std::string> wrfInterpretation::get_atmFilePaths()
{
    return WindNinjaClass.get_atmFilePaths();
}

std::vector<std::string> wrfInterpretation::get_velFilePaths()
{
    return WindNinjaClass.get_velFilePaths();
}

std::vector<std::string> wrfInterpretation::get_angFilePaths()
{
    return WindNinjaClass.get_angFilePaths();
}

std::vector<std::string> wrfInterpretation::get_cldFilePaths()
{
    return WindNinjaClass.get_cldFilePaths();
}
/*** end WindNinjaAPI get value functions ***/

/*** wrfGetWeather get value functions ***/
std::vector<double> wrfInterpretation::get_temperatures()
{
    return wrfWeatherClass.get_temperatures();
}

std::vector<double> wrfInterpretation::get_humidities()
{
    return wrfWeatherClass.get_humidities();
}

std::vector<double> wrfInterpretation::get_totalPrecip()
{
    return wrfWeatherClass.get_totalPrecip();
}

std::vector<double> wrfInterpretation::get_cloudCover()
{
    return wrfWeatherClass.get_cloudCover();
}
/*** end wrfGetWeather get value functions ***/



/***** private functions *****/

