#ifndef INPUTVARIABLES_VALUESTORAGE_H
#define INPUTVARIABLES_VALUESTORAGE_H


#include "inputVariables_infoStorage.h"

#include "singleValueDataTypes/boolValue.h"
#include "singleValueDataTypes/size_tValue.h"
#include "singleValueDataTypes/intValue.h"
#include "singleValueDataTypes/doubleValue.h"
#include "singleValueDataTypes/stringValue.h"
#include "singleValueDataTypes/pathNameValue.h"
#include "singleValueDataTypes/lcpFileValue.h"
#include "singleValueDataTypes/shapeFileValue.h"
#include "singleValueDataTypes/wrfFileValue.h"
#include "singleValueDataTypes/lat_longValue.h"
#include "singleValueDataTypes/dateValue.h"
#include "singleValueDataTypes/hour_minValue.h"

#include "multiValueDataTypes/createIgnitionFromLatLongsStorage.h"
#include "multiValueDataTypes/polygonIgnitShapeFileStorage.h"
#include "multiValueDataTypes/GeoMACfirePerimeterFileStorage.h"
#include "multiValueDataTypes/farsiteOutputFirePerimeterFileStorage.h"
#include "multiValueDataTypes/wrfFileStorage.h"
#include "multiValueDataTypes/additionalWindNinjaOutputs_googleStorage.h"
#include "multiValueDataTypes/additionalWindNinjaOutputs_shapefileStorage.h"
#include "multiValueDataTypes/additionalWindNinjaOutputs_pdfStorage.h"


class inputVariables_valueStorage
{
public:

    // constructors
    inputVariables_valueStorage();

    // reinit functions (like constructors)
    bool resetVariables();     //kind of a reusable constructor/destructor

    // get variable info functions
    std::vector<inputVariable_info> get_inputVariableInfo();
    std::string get_maxVarNameColumnWhitespace();

    // set variable value functions
    bool set_boolValue(std::string varName, bool newBoolValue);
    bool set_boolValue_string(std::string varName, std::string newStringBoolValue);
    bool set_size_tValue(std::string varName, size_t newSize_tValue);
    bool set_size_tValue_string(std::string varName, std::string newStringSize_tValue);
    bool set_intValue(std::string varName, int newIntValue);
    bool set_intValue_string(std::string varName, std::string newStringIntValue);
    bool set_doubleValue(std::string varName, double newDoubleValue, std::string doubleType);
    bool set_doubleValue_string(std::string varName, std::string newStringDoubleValue, std::string doubleType);
    bool set_stringValue(std::string varName, std::string newStringValue);
    bool set_pathNameValue(std::string varName, std::string newPathNameValue);
    bool set_lcpFileValue(std::string varName, std::string newLcpFileValue);
    bool set_shapeFileValue(std::string varName, std::string newShapeFileValue);
    bool set_wrfFileValue(std::string varName, std::string newWrfFileValue);
    bool set_lat_longValue(std::string varName, double newLatValue, double newLongValue);
    //bool set_lat_longValue_string(std::string varName, std::string newStringLatValue, std::string newStringLongValue);
    bool set_lat_longValue_string(std::string varName, std::string newLatLongValueString);
    bool set_dateValue(std::string varName, int newYearValue, int newMonthValue, int newDayValue, int newHourValue, int newMinuteValue);
    bool set_dateValue_string(std::string varName, std::string newStringYearValue, std::string newStringMonthValue, std::string newStringDayValue, std::string newStringHourValue, std::string newStringMinuteValue);
    bool set_dateValue_string(std::string varName, std::string newDateValueString);
    bool set_hour_minValue(std::string varName, int newHourValue, int newMinuteValue);
    bool set_hour_minValue_string(std::string varName, std::string newStringHourValue, std::string newStringMinuteValue);
    bool set_hour_minValue_string(std::string varName, std::string newHourMinValue);

    // load function stuff for count type variables
    bool add_createIgnitionLatLongValue(double newLat_CoordValue, double newLong_CoordValue);
    bool add_createIgnitionLatLongValue_Line(std::string inputDataLine);
    bool add_polygonIgnitShapeFile(std::string new_polygon_ignit_shape_file);
    bool add_polygonIgnitShapeFile_Line(std::string inputDataLine);
    bool add_GeoMACfirePerimeterFile(std::string new_GeoMAC_fire_perimeter_file);
    bool add_GeoMACfirePerimeterFile_Line(std::string inputDataLine);
    bool add_farsiteOutputFirePerimeterFile(std::string new_farsite_output_fire_perimeter_file);
    bool add_farsiteOutputFirePerimeterFile_Line(std::string inputDataLine);
    bool add_wrfFile(std::string new_wrf_file);
    bool add_wrfFile_Line(std::string inputDataLine);
    bool add_additionalWindNinjaOutputs_googleValueSet(std::string new_wrf_file_name, bool new_write_wx_model_goog_output, bool new_write_goog_output, double new_goog_out_resolution,
                                                       std::string new_units_goog_out_resolution, std::string new_goog_out_color_scheme, bool new_goog_out_vector_scaling);
    bool add_additionalWindNinjaOutputs_googleValue_Line(std::string inputDataLine);
    bool add_additionalWindNinjaOutputs_shapefileValueSet(std::string new_wrf_file_name, bool new_write_wx_model_shapefile_output, bool new_write_shapefile_output,
                                                          double new_shape_out_resolution, std::string new_units_shape_out_resolution);
    bool add_additionalWindNinjaOutputs_shapeValue_Line(std::string inputDataLine);
    bool add_additionalWindNinjaOutputs_pdfValueSet(std::string new_wrf_file_name, bool new_write_pdf_output, double new_pdf_out_resolution, std::string new_units_pdf_out_resolution,
                                                        double new_pdf_linewidth, std::string new_pdf_basemap, double new_pdf_height, double new_pdf_width, std::string new_pdf_size);
    bool add_additionalWindNinjaOutputs_pdfValue_Line(std::string inputDataLine);


    // get variable value functions for single values
    boolValue get_boolValue(std::string varName);
    size_tValue get_size_tValue(std::string varName);
    intValue get_intValue(std::string varName);
    doubleValue get_doubleValue(std::string varName);
    stringValue get_stringValue(std::string varName);
    pathNameValue get_pathNameValue(std::string varName);
    lcpFileValue get_lcpFileValue(std::string varName);
    shapeFileValue get_shapeFileValue(std::string varName);
    wrfFileValue get_wrfFileValue(std::string varName);
    lat_longValue get_lat_longValue(std::string varName);
    dateValue get_dateValue(std::string varName);
    hour_minValue get_hour_minValue(std::string varName);


    // get variable value functions for count values
    createIgnitionFromLatLongsStorage get_createIgnitionFromLatLongsStorage();
    polygonIgnitShapeFileStorage get_polygonIgnitShapeFileStorage();
    GeoMACfirePerimeterFileStorage get_GeoMACfirePerimeterFileStorage();
    farsiteOutputFirePerimeterFileStorage get_farsiteOutputFirePerimeterFileStorage();
    wrfFileStorage get_wrfFileStorage();
    additionalWindNinjaOutputs_googleStorage get_additionalWindNinjaOutputs_googleStorage();
    additionalWindNinjaOutputs_shapefileStorage get_additionalWindNinjaOutputs_shapefileStorage();
    additionalWindNinjaOutputs_pdfStorage get_additionalWindNinjaOutputs_pdfStorage();

private:

    // setup functions
    bool sortInputVariableInfo();
    bool setSpecializedDefaults();

    // class data members
    inputVariables_infoStorage setupInputVariableInfo;
    std::vector<inputVariable_info> inputVariableInfo;

    // data members of single count
    std::vector<boolValue> boolValues;
    std::vector<size_tValue> size_tValues;
    std::vector<intValue> intValues;
    std::vector<doubleValue> doubleValues;
    std::vector<stringValue> stringValues;
    std::vector<pathNameValue> pathNameValues;
    std::vector<lcpFileValue> lcpFileValues;
    std::vector<shapeFileValue> shapeFileValues;
    std::vector<wrfFileValue> wrfFileValues;
    std::vector<lat_longValue> lat_longValues;
    std::vector<dateValue> dateValues;
    std::vector<hour_minValue> hour_minValues;

    // variable value of type count data members
    createIgnitionFromLatLongsStorage stored_create_ignition_from_latlongs;
    polygonIgnitShapeFileStorage stored_polygon_ignit_shape_files;
    GeoMACfirePerimeterFileStorage stored_GeoMAC_fire_perimeter_files;
    farsiteOutputFirePerimeterFileStorage stored_farsite_output_fire_perimeter_files;
    wrfFileStorage stored_wrf_files;
    additionalWindNinjaOutputs_googleStorage stored_additional_WindNinja_Outputs_google;
    additionalWindNinjaOutputs_shapefileStorage stored_additional_WindNinja_Outputs_shapefile;
    additionalWindNinjaOutputs_pdfStorage stored_additional_WindNinja_Outputs_pdf;

};

#endif // INPUTVARIABLES_VALUESTORAGE_H
