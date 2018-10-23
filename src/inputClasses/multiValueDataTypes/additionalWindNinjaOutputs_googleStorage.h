#ifndef ADDITIONALWINDNINJAOUTPUTS_GOOGLESTORAGE_H
#define ADDITIONALWINDNINJAOUTPUTS_GOOGLESTORAGE_H

#include <iostream>
#include <vector>

#include "../singleValueDataTypes/boolValue.h"
#include "../singleValueDataTypes/doubleValue.h"
#include "../singleValueDataTypes/stringValue.h"

class additionalWindNinjaOutputs_googleStorage
{

public:

    // constructor
    additionalWindNinjaOutputs_googleStorage();

    // reconstructor like functions
    void reset();

    // set value functions
    bool add_additionalWindNinjaOutputs_googleValueSet(std::string new_wrf_file_name, bool new_write_wx_model_goog_output, bool new_write_goog_output, double new_goog_out_resolution,
                                                       std::string new_units_goog_out_resolution, std::string new_goog_out_color_scheme, bool new_goog_out_vector_scaling);
    bool add_additionalWindNinjaOutputs_googleValue_Line(std::string inputDataLine);

    // get value functions
    std::vector<std::string> get_stored_wrfFileNames();
    std::vector<boolValue> get_stored_write_wx_model_goog_output_values();
    std::vector<boolValue> get_stored_write_goog_output_values();
    std::vector<doubleValue> get_stored_goog_out_resolution_values();
    std::vector<stringValue> get_stored_units_goog_out_resolution_values();
    std::vector<stringValue> get_stored_goog_out_color_scheme_values();
    std::vector<boolValue> get_stored_goog_out_vector_scaling_values();

private:

    // reconstructor like functions

    // data members
    std::vector<std::string> stored_wrfFileNames;
    std::vector<boolValue> stored_write_wx_model_goog_output_values;
    std::vector<boolValue> stored_write_goog_output_values;
    std::vector<doubleValue> stored_goog_out_resolution_values;
    std::vector<stringValue> stored_units_goog_out_resolution_values;
    std::vector<stringValue> stored_goog_out_color_scheme_values;
    std::vector<boolValue> stored_goog_out_vector_scaling_values;

    // default values
    std::string defaultStringValue = "";

};

#endif // ADDITIONALWINDNINJAOUTPUTS_GOOGLESTORAGE_H
