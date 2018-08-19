#ifndef CREATEIGNITIONS_H
#define CREATEIGNITIONS_H


#include "inputVariablesHandler.h"

class createIgnitions
{

public:

    // constructors
    createIgnitions();

    // reconstructor like functions
    bool load_required_inputs(inputVariablesHandler *inputs);

    // functions
    bool findLargestFirePerimeter();
    bool createAllIgnitions();

    // get value functions
    double get_firePerimMinLat();
    double get_firePerimMinLong();
    double get_firePerimMaxLat();
    double get_firePerimMaxLong();

    // get desired output value functions
    std::vector<std::string> get_ignitionShapefilesForSimulations();

private:

    // reconstructor like functions
    void reset();

    // create ignition shapefile functions
    bool createIgnitionFromLatLongs();
    bool addPolygonIgnitShapeFiles();
    bool createIgnitionFromGeoMacFirePerimFiles();
    bool createIgnitionFromPastFarsiteOutputs();

    // useful utility functions
    bool checkIgnitionFile(std::string ignitionFile);

    // data members needed from the inputs
    std::string actual_run_base_name;
    std::string actualCreateInputs_path;
        // application specific variables
    std::string createIgnition_output_units;
        // createIgnition variables
    std::vector<lat_long_pointValue> create_ignition_from_latlongs;
    std::vector<std::string> polygon_ignit_shape_files;
    std::vector<std::string> GeoMAC_fire_perimeter_files;
    std::vector<std::string> farsite_output_fire_perimeter_files;
    double fire_perimeter_widening_factor;

    // desired output values
    double firePerimMinLat;
    double firePerimMinLong;
    double firePerimMaxLat;
    double firePerimMaxLong;
    std::vector<std::string> ignitionShapefilesForSimulations;

};

#endif // CREATEIGNITIONS_H
