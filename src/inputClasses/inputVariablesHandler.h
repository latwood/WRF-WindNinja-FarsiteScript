#ifndef INPUTVARIABLESHANDLER_H
#define INPUTVARIABLESHANDLER_H


#include "inputVariablesInitializer.h"

#include "inputParser.h"

#include "../utilityClasses/doesFolderExist.h"


class inputVariablesHandler
{
public:

    // constructors
    inputVariablesHandler();

    // data passer between inputVariablesHandler and inputParser functions
    // man I know there is a smarter way and it involves pointers and probably copy constructors, but I'm in a hurry
    void transferInputVariableStorage();

    // functions
    void explainInputReqs();
    bool loadScriptInputs(std::string inputFileName);
    void printFoundInput();
    std::string get_actualCreateInputs_path();
    std::string get_actualFinalOutput_path();
    std::string get_actual_run_base_name();
    std::string get_actualLcpPath();

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

    // functions
    bool reset();
    bool resetSingleVar(singleInputVariable &inputVar);
    bool setSpecializedDefaults();
    bool verifyFoundInputCombinations();
    bool findActualCreateInputsAndFinalOutputsPaths();
    bool findActualLcpPathAndBaseName();

    // input combination checking functions
    size_t findVarInfoIdx(std::string varName);
    void checkUsage_optionalValue(std::string varName);
    void checkUsage_defaultValue(std::string varName);
    void checkUsage_requiredValue(std::string varName, bool &InputCombinationSuccess);
    void checkUsage_chooseOnlyOneValue(std::vector<std::string> varNames, bool &InputCombinationSuccess);
    void checkUsage_chooseAtLeastOneValue(std::vector<std::string> varNames, bool &InputCombinationSuccess);
    void checkUsage_chooseOnlyOneValueOrDefaultValue(std::string defaultVarName, std::vector<std::string> otherVarNames, bool &InputCombinationSuccess);
    void checkUsage_requiredIfCertainValueSet(std::string ifSetVarName, std::vector<std::string> checkVarNames, bool &InputCombinationSuccess);
    void checkUsage_chooseOnlyOneIfCertainValueSet(std::string ifSetVarName, std::vector<std::string> checkVarNames, bool &InputCombinationSuccess);

    // set variable value functions
    bool set_boolValue(std::string varName, bool newBoolValue);
    bool set_size_tValue(std::string varName, size_t newSize_tValue);
    bool set_intValue(std::string varName, int newIntValue);
    bool set_doubleValue(std::string varName, double newDoubleValue, std::string doubleType);
    bool set_stringValue(std::string varName, std::string newStringValue);
    bool set_pathNameValue(std::string varName, std::string newPathNameValue);
    bool set_lcpFileValue(std::string varName, std::string newLcpFileValue);
    bool set_shapeFileValue(std::string varName, std::string newShapeFileValue);
    bool set_wrfFileValue(std::string varName, std::string newWrfFileValue);
    bool set_lat_longValue(std::string varName, double newLatValue, double newLongValue);
    bool set_dateValue(std::string varName, int newYearValue, int newMonthValue, int newDayValue, int newHourValue, int newMinuteValue);
    bool set_hour_minValue(std::string varName, int newHourValue, int newMinuteValue);

    // load function stuff for count type variables
    bool add_createIgnitionLatLongValue(double newLat_CoordValue, double newLong_CoordValue);
    bool add_polygonIgnitShapeFile(std::string new_polygon_ignit_shape_file);
    bool add_GeoMACfirePerimeterFile(std::string new_GeoMAC_fire_perimeter_file);
    bool add_farsiteOutputFirePerimeterFile(std::string new_farsite_output_fire_perimeter_file);
    bool add_wrfFile(std::string new_wrf_file);
    bool add_additionalWindNinjaOutputs_googleValueSet(std::string new_wrf_file_name, bool new_write_wx_model_goog_output, bool new_write_goog_output, double new_goog_out_resolution,
                                                       std::string new_units_goog_out_resolution, std::string new_goog_out_color_scheme, bool new_goog_out_vector_scaling);
    bool add_additionalWindNinjaOutputs_shapefileValueSet(std::string new_wrf_file_name, bool new_write_wx_model_shapefile_output, bool new_write_shapefile_output,
                                                          double new_shape_out_resolution, std::string new_units_shape_out_resolution);
    bool add_additionalWindNinjaOutputs_pdfValueSet(std::string new_wrf_file_name, bool new_write_pdf_output, double new_pdf_out_resolution, std::string new_units_pdf_out_resolution,
                                                        double new_pdf_linewidth, std::string new_pdf_basemap, double new_pdf_height, double new_pdf_width, std::string new_pdf_size);

    // class data members
    inputVariablesInitializer initialVariables;
    std::vector<singleInputVariable> inputVariableStorage;
    inputParser theInputParser;

    // variable value of type count data members
    createIgnitionFromLatLongsStorage stored_create_ignition_from_latlongs;
    polygonIgnitShapeFileStorage stored_polygon_ignit_shape_files;
    GeoMACfirePerimeterFileStorage stored_GeoMAC_fire_perimeter_files;
    farsiteOutputFirePerimeterFileStorage stored_farsite_output_fire_perimeter_files;
    wrfFileStorage stored_wrf_files;
    additionalWindNinjaOutputs_googleStorage stored_additional_WindNinja_Outputs_google;
    additionalWindNinjaOutputs_shapefileStorage stored_additional_WindNinja_Outputs_shapefile;
    additionalWindNinjaOutputs_pdfStorage stored_additional_WindNinja_Outputs_pdf;

    // important for file management
    std::string actualCreateInputs_path;
    std::string actualFinalOutput_path;
    std::string actual_run_base_name;
    std::string actualLcpPath;

};

#endif // INPUTVARIABLESHANDLER_H
