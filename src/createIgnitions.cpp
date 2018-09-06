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
    createIgnition_output_units = inputs->get_stringValue("createIgnition_output_units").get_storedStringValue();
        // createIgnition variables
    create_ignition_from_latlongs = inputs->get_createIgnitionFromLatLongsStorage();
    polygon_ignit_shape_files = inputs->get_polygonIgnitShapeFileStorage();
    GeoMAC_fire_perimeter_files = inputs->get_GeoMACfirePerimeterFileStorage();
    farsite_output_fire_perimeter_files = inputs->get_farsiteOutputFirePerimeterFileStorage();
    fire_perimeter_widening_factor = inputs->get_doubleValue("fire_perimeter_widening_factor").get_storedDoubleValue();

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
    if(create_ignition_from_latlongs.get_storedCreateIgnitionLatLongValues().size() != 0)
    {
        if(createIgnitionFromLatLongs() == false)
        {
            printf("problem creating ignition files from LatLongs!\n");
            return false;
        }
    }

    // now try to do the polygon ignit shapefiles
    if(polygon_ignit_shape_files.get_storedPolygonIgnitShapeFiles().size() != 0)
    {
        if(addPolygonIgnitShapeFiles() == false)
        {
            printf("problem adding input polygon ignition shape files to list of farsite ignition files!\n");
            return false;
        }
    }

    // now try setting up the GeoMAC fire perimeter files
    if(GeoMAC_fire_perimeter_files.get_storedGeoMACfirePerimeterFiles().size() != 0)
    {
        if(createIgnitionFromGeoMacFirePerimFiles() == false)
        {
            printf("problem creating ignition files from GeoMAC fire perimeter files!\n");
            return false;
        }
    }

    // now try to use past farsite output fire perimeter files
    if(farsite_output_fire_perimeter_files.get_storedFarsiteOutputFirePerimeterFiles().size() != 0)
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
    // just realized, this is not the right place to be using reset on these!
    //create_ignition_from_latlongs.reset();
    //polygon_ignit_shape_files.reset();
    //GeoMAC_fire_perimeter_files.reset();
    //farsite_output_fire_perimeter_files.reset();
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
    std::vector<shapeFileValue> polygonIgnitShapeFileValues = polygon_ignit_shape_files.get_storedPolygonIgnitShapeFiles();
    for(size_t shpFileIdx = 0; shpFileIdx < polygonIgnitShapeFileValues.size(); shpFileIdx++)
    {
        std::string currentShapeFile = polygonIgnitShapeFileValues[shpFileIdx].get_storedShapeFileValue();
        if(checkIgnitionFile(currentShapeFile) == true)
        {
            ignitionShapefilesForSimulations.push_back(currentShapeFile);
        } else
        {
            printf("problem with polygon_ignit_shape_file \"%s\"!",currentShapeFile.c_str());
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
    // should at a minimum make sure all the required side files are there to go with the ignition file, probably during the inputs handling
    printf("checkIgnitionFile ignition file \"%s\"\n",ignitionFile.c_str());
    // if it gets here, everything went well
    return true;
}
/*** end useful utility functions ***/
