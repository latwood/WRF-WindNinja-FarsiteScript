#ifndef ADDITIONALWINDNINJAOUTPUTS_PDFSTORAGE_H
#define ADDITIONALWINDNINJAOUTPUTS_PDFSTORAGE_H


#include <iostream>
#include <vector>

#include "../singleValueDataTypes/boolValue.h"
#include "../singleValueDataTypes/doubleValue.h"
#include "../singleValueDataTypes/stringValue.h"

class additionalWindNinjaOutputs_pdfStorage
{

public:

    // constructor
    additionalWindNinjaOutputs_pdfStorage();

    // reconstructor like functions
    void reset();

    // set value functions
    bool add_additionalWindNinjaOutputs_pdfValueSet(std::string new_wrf_file_name, bool new_write_pdf_output, double new_pdf_out_resolution, std::string new_units_pdf_out_resolution,
                                                        double new_pdf_linewidth, std::string new_pdf_basemap, double new_pdf_height, double new_pdf_width, std::string new_pdf_size);

    // get value functions
    std::vector<std::string> get_stored_wrfFileNames();
    std::vector<boolValue> get_stored_write_pdf_output_values();
    std::vector<doubleValue> get_stored_pdf_out_resolution_values();
    std::vector<stringValue> get_stored_units_pdf_out_resolution_values();
    std::vector<doubleValue> get_stored_pdf_linewidth_values();
    std::vector<stringValue> get_stored_pdf_basemap_values();
    std::vector<doubleValue> get_stored_pdf_height_values();
    std::vector<doubleValue> get_stored_pdf_width_values();
    std::vector<stringValue> get_stored_pdf_size_values();

private:

    // reconstructor like functions

    // data members
    std::vector<std::string> stored_wrfFileNames;
    std::vector<boolValue> stored_write_pdf_output_values;
    std::vector<doubleValue> stored_pdf_out_resolution_values;
    std::vector<stringValue> stored_units_pdf_out_resolution_values;
    std::vector<doubleValue> stored_pdf_linewidth_values;
    std::vector<stringValue> stored_pdf_basemap_values;
    std::vector<doubleValue> stored_pdf_height_values;
    std::vector<doubleValue> stored_pdf_width_values;
    std::vector<stringValue> stored_pdf_size_values;

    // default values
    std::string defaultStringValue = "";

};

#endif // ADDITIONALWINDNINJAOUTPUTS_PDFSTORAGE_H
