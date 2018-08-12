#ifndef INPUTVARIABLESHANDLER_H
#define INPUTVARIABLESHANDLER_H


#include "inputVariables_valueStorage.h"

#include "inputParser.h"


class inputVariablesHandler
{
public:

    // constructors
    inputVariablesHandler();

    // data passer between inputVariablesHandler and inputParser functions
    // man I know there is a smarter way and it involves pointers and probably copy constructors, but I'm in a hurry
    void transferVariableInfo();

    // functions
    void explainInputReqs();
    bool loadScriptInputs(std::string inputFileName);
    void printFoundInput();

    // will probably have to repeat the functions from the inputVariable_valueStorage class unfortunately.
    // Difference is the values won't be stored in this class, just in the one referenced here
    // get variable value functions for single values
    bool get_inputVariableBoolValue(std::string varName);
    size_t get_inputVariableSize_tValue(std::string varName);
    int get_inputVariableIntValue(std::string varName);
    double get_inputVariableDoubleValue(std::string varName);
    std::string get_inputVariableStringValue(std::string varName);
    std::string get_inputVariablePathnameValue(std::string varName);
    std::string get_inputVariableFilenameValue(std::string varName);
    double get_inputVariableLat_Long_PointValueLat_Coord(std::string varName);
    double get_inputVariableLat_Long_PointValueLong_Coord(std::string varName);
    dateValue get_inputVariableDateValue(std::string varName);
    int get_inputVariableDateValueYear(std::string varName);
    int get_inputVariableDateValueMonth(std::string varName);
    int get_inputVariableDateValueDay(std::string varName);
    int get_inputVariableDateValueHour(std::string varName);
    int get_inputVariableDateValueMinute(std::string varName);
    int get_inputVariableHour_MinValueHour(std::string varName);
    int get_inputVariableHour_MinValueMinute(std::string varName);

    // get variable value functions for count values
    // datatype get_inputVariableCountValues(std::string varName); // definitely don't think this kind of generic type idea will work for counts, they are way too complex! Need separate getter for each type of count variable, maybe even multiple getters for said variables if the function that uses them doesn't want to define something to unravel all the values out
    std::vector<lat_long_pointValue> get_create_ignition_from_latlongs();
    std::vector<std::string> get_polygon_ignit_shape_files();
    std::vector<std::string> get_GeoMAC_fire_perimeter_files();
    std::vector<std::string> get_farsite_output_fire_perimeter_files();
    std::vector<std::string> get_wrf_files();
    // additional_WindNinja_outputs_google get functions
    std::vector<std::string> get_additional_WindNinja_outputs_googleValues_wrf_file_names();
    bool get_write_wx_model_goog_output(std::string wrf_file_name);
    bool get_write_goog_output(std::string wrf_file_name);
    int get_goog_out_resolution(std::string wrf_file_name);
    std::string get_units_goog_out_resolution(std::string wrf_file_name);
    std::string get_goog_out_color_scheme(std::string wrf_file_name);
    bool get_goog_out_vector_scaling(std::string wrf_file_name);
    // additional_WindNinja_outputs_shapefile get functions
    std::vector<std::string> get_additional_WindNinja_outputs_shapefileValues_wrf_file_names();
    bool get_write_wx_model_shapefile_output(std::string wrf_file_name);
    bool get_write_shapefile_output(std::string wrf_file_name);
    int get_shape_out_resolution(std::string wrf_file_name);
    std::string get_units_shape_out_resolution(std::string wrf_file_name);
    // additional_WindNinja_outputs_pdf get functions
    std::vector<std::string> get_additional_WindNinja_outputs_pdfValues_wrf_file_names();
    bool get_write_pdf_output(std::string wrf_file_name);
    int get_pdf_out_resolution(std::string wrf_file_name);
    std::string get_units_pdf_out_resolution(std::string wrf_file_name);
    size_t get_pdf_linewidth(std::string wrf_file_name);
    std::string get_pdf_basemap(std::string wrf_file_name);
    double get_pdf_height(std::string wrf_file_name);
    double get_pdf_width(std::string wrf_file_name);
    std::string get_pdf_size(std::string wrf_file_name);

private:

    // functions
    bool reset();
    bool verifyFoundInputCombinations();

    // class data members
    std::vector<inputVariable_info> inputVariableInfo;
    inputVariables_valueStorage inputVariableValues;
    inputParser theInputParser;

};

#endif // INPUTVARIABLESHANDLER_H
