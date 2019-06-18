#ifndef INPUTPARSER_H
#define INPUTPARSER_H


#include <fstream>

#include "inputVariablesInitializer.h"

#include "../utilityClasses/calcTime.h"

class inputParser
{
public:

    // constructors
    inputParser();

    // reinit functions (like constructors)
    void reset();     //kind of a reusable constructor/destructor

    // variable class getter functions
    void loadInputVariableStorage(std::vector<singleInputVariable> &new_inputVariableStorage, createIgnitionFromLatLongsStorage &new_stored_create_ignition_from_latlongs,
                                  polygonIgnitShapeFileStorage &new_stored_polygon_ignit_shape_files, GeoMACfirePerimeterFileStorage &new_stored_GeoMAC_fire_perimeter_files,
                                  farsiteOutputFirePerimeterFileStorage &new_stored_farsite_output_fire_perimeter_files, wrfFileStorage &new_stored_wrf_files,
                                  additionalWindNinjaOutputs_googleStorage &new_stored_additional_WindNinja_Outputs_google,
                                  additionalWindNinjaOutputs_shapefileStorage &new_stored_additional_WindNinja_Outputs_shapefile,
                                  additionalWindNinjaOutputs_pdfStorage &new_stored_additional_WindNinja_Outputs_pdf);

    // data passer between inputVariablesHandler and inputParser functions
    // man I know there is a smarter way and it involves pointers and probably copy constructors, but I'm in a hurry
    std::vector<singleInputVariable> transfer_inputVariableStorage();
    createIgnitionFromLatLongsStorage transfer_stored_create_ignition_from_latlongs();
    polygonIgnitShapeFileStorage transfer_stored_polygon_ignit_shape_files();
    GeoMACfirePerimeterFileStorage transfer_stored_GeoMAC_fire_perimeter_files();
    farsiteOutputFirePerimeterFileStorage transfer_stored_farsite_output_fire_perimeter_files();
    wrfFileStorage transfer_stored_wrf_files();
    additionalWindNinjaOutputs_googleStorage transfer_stored_additional_WindNinja_Outputs_google();
    additionalWindNinjaOutputs_shapefileStorage transfer_stored_additional_WindNinja_Outputs_shapefile();
    additionalWindNinjaOutputs_pdfStorage transfer_stored_additional_WindNinja_Outputs_pdf();

    // main loader functions
    bool readInputFile(std::string inputFileName);
    bool readVarNamesAndCountValues();
    bool loadCountValues();
    bool checkCountVsLinesOfData();
    bool loadLoaderFunctionData();

private:

    // parsing utility functions
    bool openIsStream(std::string fileName,std::ifstream &fzStream);
    void removeLeadingWhitespace(std::string &inputString);
    void removeEndingWhitespace(std::string &inputString);
    void removeSeparator(std::string inputString, std::string &varNamePart, std::string &varCountPart);

    // set variable value functions
    bool set_boolValue_string(std::string varName, std::string newStringBoolValue);
    bool set_size_tValue_string(std::string varName, std::string newStringSize_tValue);
    bool set_intValue_string(std::string varName, std::string newStringIntValue);
    bool set_doubleValue_string(std::string varName, std::string newStringDoubleValue, std::string doubleType);
    //bool set_lat_longValue_string(std::string varName, std::string newStringLatValue, std::string newStringLongValue);
    bool set_lat_longValue_string(std::string varName, std::string newLatLongValueString);
    bool set_dateValue_string(std::string varName, std::string newStringYearValue, std::string newStringMonthValue, std::string newStringDayValue, std::string newStringHourValue, std::string newStringMinuteValue);
    bool set_dateValue_string(std::string varName, std::string newDateValueString);
    bool set_hour_minValue_string(std::string varName, std::string newStringHourValue, std::string newStringMinuteValue);
    bool set_hour_minValue_string(std::string varName, std::string newHourMinValue);

    // load function stuff for count type variables
    bool add_createIgnitionLatLongValue_Line(std::string inputDataLine);
    bool add_polygonIgnitShapeFile_Line(std::string inputDataLine);
    bool add_GeoMACfirePerimeterFile_Line(std::string inputDataLine);
    bool add_farsiteOutputFirePerimeterFile_Line(std::string inputDataLine);
    bool add_wrfFile_Line(std::string inputDataLine);
    bool add_additionalWindNinjaOutputs_googleValue_Line(std::string inputDataLine);
    bool add_additionalWindNinjaOutputs_shapeValue_Line(std::string inputDataLine);
    bool add_additionalWindNinjaOutputs_pdfValue_Line(std::string inputDataLine);

    // other set variable value functions (repeats from input variables handler)
    bool set_stringValue(std::string varName, std::string newStringValue);
    bool set_pathNameValue(std::string varName, std::string newPathNameValue);
    bool set_lcpFileValue(std::string varName, std::string newLcpFileValue);
    bool set_shapeFileValue(std::string varName, std::string newShapeFileValue);
    bool set_wrfFileValue(std::string varName, std::string newWrfFileValue);

    // get variable value functions for single values
    size_tValue get_size_tValue(std::string varName);
    size_t findVarInfoIdx(std::string varName);

    // class related data members
    std::vector<singleInputVariable> inputVariableStorage;

    // variable value of type count data members
    createIgnitionFromLatLongsStorage stored_create_ignition_from_latlongs;
    polygonIgnitShapeFileStorage stored_polygon_ignit_shape_files;
    GeoMACfirePerimeterFileStorage stored_GeoMAC_fire_perimeter_files;
    farsiteOutputFirePerimeterFileStorage stored_farsite_output_fire_perimeter_files;
    wrfFileStorage stored_wrf_files;
    additionalWindNinjaOutputs_googleStorage stored_additional_WindNinja_Outputs_google;
    additionalWindNinjaOutputs_shapefileStorage stored_additional_WindNinja_Outputs_shapefile;
    additionalWindNinjaOutputs_pdfStorage stored_additional_WindNinja_Outputs_pdf;

    // important parser data members
    std::string inputSeparator = "=";
    std::string commentChar = "#";
    std::vector<std::string> foundInputLines;
    std::vector<std::string> foundLineTypes;
    std::vector<std::string> foundVarNames;
    std::vector<size_t> foundVarNames_inputVariableInfoIndices;
    std::vector<std::string> foundVarCountStrings;

    // debug data members
    calcTime debugTimers;

};

#endif // INPUTPARSER_H
