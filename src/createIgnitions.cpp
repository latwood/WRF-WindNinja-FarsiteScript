#include "createIgnitions.h"

/***** public functions *****/

/*** constructor functions ***/
createIgnitions::createIgnitions()
{

}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
bool createIgnitions::load_required_inputs(inputVariablesHandler *inputs)
{
    // first reset all values to default values with reconstructor
    reset();

    // next load all the needed inputs from the input file
    actual_run_base_name = inputs->get_actual_run_base_name();
    actualCreateInputs_path = inputs->get_actualCreateInputs_path();
        // application specific variables
    createIgnition_output_units = inputs->get_inputVariableStringValue("createIgnition_output_units");
        // createIgnition variables
    burn_start_month = inputs->get_inputVariableDateValueMonth("burn_start_time");
    burn_start_day = inputs->get_inputVariableDateValueDay("burn_start_time");
    burn_start_year = inputs->get_inputVariableDateValueYear("burn_start_time");
    burn_start_hour = inputs->get_inputVariableDateValueHour("burn_start_time");
    burn_start_minute = inputs->get_inputVariableDateValueMinute("burn_start_time");
    burn_end_month = inputs->get_inputVariableDateValueMonth("burn_end_time");
    burn_end_day = inputs->get_inputVariableDateValueDay("burn_end_time");
    burn_end_year = inputs->get_inputVariableDateValueYear("burn_end_time");
    burn_end_hour = inputs->get_inputVariableDateValueHour("burn_end_time");
    burn_end_minute = inputs->get_inputVariableDateValueMinute("burn_end_time");
    create_ignition_from_latlongs = inputs->get_create_ignition_from_latlongs();
    polygon_ignit_shape_files = inputs->get_polygon_ignit_shape_files();
    GeoMAC_fire_perimeter_files = inputs->get_GeoMAC_fire_perimeter_files();
    farsite_output_fire_perimeter_files = inputs->get_farsite_output_fire_perimeter_files();
    fire_perimeter_widening_factor = inputs->get_inputVariableDoubleValue("fire_perimeter_widening_factor");

    // data members created from inputs that are lcpManager specific

    // if it gets here, everything went well
    return true;
}
/*** end reconstructor like functions ***/

/*** functions ***/
bool createIgnitions::findLargestFirePerimeter()
{
    // if it gets here, everything went well
    return true;
}

bool createIgnitions::createAllIgnitions()
{
    // first try to setup the ignitions from latlong points
    if(create_ignition_from_latlongs.size() != 0)
    {
        if(createIgnitionFromLatLongs() == false)
        {
            printf("problem creating ignition files from LatLongs!\n");
            return false;
        }
    }

    // now try to do the polygon ignit shapefiles
    if(polygon_ignit_shape_files.size() != 0)
    {
        if(addPolygonIgnitShapeFiles() == false)
        {
            printf("problem adding input polygon ignition shape files to list of farsite ignition files!\n");
            return false;
        }
    }

    // now try setting up the GeoMAC fire perimeter files
    if(GeoMAC_fire_perimeter_files.size() != 0)
    {
        if(createIgnitionFromGeoMacFirePerimFiles() == false)
        {
            printf("problem creating ignition files from GeoMAC fire perimeter files!\n");
            return false;
        }
    }

    // now try to use past farsite output fire perimeter files
    if(farsite_output_fire_perimeter_files.size() != 0)
    {
        if(createIgnitionFromPastFarsiteOutputs() == false)
        {
            printf("problem creating ignition files from past farsite output fire perimeter files!\n");
            return false;
        }
    }

    printf("found ignition files:\n");
    for(size_t ignitIdx = 0; ignitIdx < ignitionShapefilesForSimulations.size(); ignitIdx++)
    {
        printf("ignitionShapefilesForSimulations[%zu] = \"%s\"\n",ignitIdx,ignitionShapefilesForSimulations[ignitIdx].c_str());
    }

    // if it gets here, everything went well
    return true;
}
/*** end functions ***/

/*** get value functions ***/
double createIgnitions::get_firePerimMinLat()
{
    return firePerimMinLat;
}

double createIgnitions::get_firePerimMinLong()
{
    return firePerimMinLong;
}

double createIgnitions::get_firePerimMaxLat()
{
    return firePerimMaxLat;
}

double createIgnitions::get_firePerimMaxLong()
{
    return firePerimMaxLong;
}
/*** end get value functions ***/

/*** get desired output value functions ***/
std::vector<std::string> createIgnitions::get_ignitionShapefilesForSimulations()
{
    return ignitionShapefilesForSimulations;
}
/*** end get desired output value functions ***/



/***** private functions *****/



/*** reconstructor like functions ***/
void createIgnitions::reset()
{
    // data members needed from the inputs
    actual_run_base_name = "";
    actualCreateInputs_path = "";
        // application specific variables
    createIgnition_output_units = "";
        // createIgnition variables
    burn_start_month = 0;
    burn_start_day = 0;
    burn_start_year = 0;
    burn_start_hour = 0;
    burn_start_minute = 0;
    burn_end_month = 0;
    burn_end_day = 0;
    burn_end_year = 0;
    burn_end_hour = 0;
    burn_end_minute = 0;
    while(!create_ignition_from_latlongs.empty())
    {
        create_ignition_from_latlongs.pop_back();
    }
    while(!polygon_ignit_shape_files.empty())
    {
        polygon_ignit_shape_files.pop_back();
    }
    while(!GeoMAC_fire_perimeter_files.empty())
    {
        GeoMAC_fire_perimeter_files.pop_back();
    }
    while(!farsite_output_fire_perimeter_files.empty())
    {
        farsite_output_fire_perimeter_files.pop_back();
    }
    fire_perimeter_widening_factor = 1.0;

    // data members created from inputs that are lcpManager specific

    // desired output values
    firePerimMinLat = 0.0;
    firePerimMinLong = 0.0;
    firePerimMaxLat = 0.0;
    firePerimMaxLong = 0.0;
    while(!ignitionShapefilesForSimulations.empty())
    {
        ignitionShapefilesForSimulations.pop_back();
    }
}
/*** end reconstructor like functions ***/

/*** create ignition shapefile functions ***/
bool createIgnitions::createIgnitionFromLatLongs()
{
    // if it gets here, everything went well
    return true;
}

bool createIgnitions::addPolygonIgnitShapeFiles()
{
    for(size_t shpFileIdx = 0; shpFileIdx < polygon_ignit_shape_files.size(); shpFileIdx++)
    {
        if(checkIgnitionFile(polygon_ignit_shape_files[shpFileIdx]) == true)
        {
            ignitionShapefilesForSimulations.push_back(polygon_ignit_shape_files[shpFileIdx]);
        } else
        {
            printf("problem with polygon_ignit_shape_file \"%s\"!",polygon_ignit_shape_files[shpFileIdx].c_str());
            return false;
        }
    }

    // if it gets here, everything went well
    return true;
}

bool createIgnitions::createIgnitionFromGeoMacFirePerimFiles()
{
    // if it gets here, everything went well
    return true;
}

bool createIgnitions::createIgnitionFromPastFarsiteOutputs()
{
    // if it gets here, everything went well
    return true;
}
/*** end create ignition shapefile functions ***/

/*** useful utility functions ***/
bool createIgnitions::checkIgnitionFile(std::string ignitionFile)
{
    // if it gets here, everything went well
    return true;
}
/*** end useful utility functions ***/
