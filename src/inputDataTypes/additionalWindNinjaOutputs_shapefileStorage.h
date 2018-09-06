#ifndef ADDITIONALWINDNINJAOUTPUTS_SHAPEFILESTORAGE_H
#define ADDITIONALWINDNINJAOUTPUTS_SHAPEFILESTORAGE_H


#include <iostream>
#include <vector>

#include "boolValue.h"
#include "doubleValue.h"
#include "stringValue.h"

class additionalWindNinjaOutputs_shapefileStorage
{

public:

    // constructor
    additionalWindNinjaOutputs_shapefileStorage();

    // reconstructor like functions
    void reset();

    // set value functions
    bool add_additionalWindNinjaOutputs_shapefileValueSet(std::string new_wrf_file_name, bool new_write_wx_model_shapefile_output, bool new_write_shapefile_output,
                                                          double new_shape_out_resolution, std::string new_units_shape_out_resolution);

    // get value functions
    std::vector<std::string> get_stored_wrfFileNames();
    std::vector<boolValue> get_stored_write_wx_model_shapefile_output_values();
    std::vector<boolValue> get_stored_write_shapefile_output_values();
    std::vector<doubleValue> get_stored_shape_out_resolution_values();
    std::vector<stringValue> get_stored_units_shape_out_resolution_values();

private:

    // reconstructor like functions

    // data members
    std::vector<std::string> stored_wrfFileNames;
    std::vector<boolValue> stored_write_wx_model_shapefile_output_values;
    std::vector<boolValue> stored_write_shapefile_output_values;
    std::vector<doubleValue> stored_shape_out_resolution_values;
    std::vector<stringValue> stored_units_shape_out_resolution_values;

    // default values
    std::string defaultStringValue = "";

};

#endif // ADDITIONALWINDNINJAOUTPUTS_SHAPEFILESTORAGE_H
