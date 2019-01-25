#ifndef INPUTVARIABLESHANDLER_H
#define INPUTVARIABLESHANDLER_H


#include "inputVariables_valueStorage.h"

#include "inputParser.h"

#include "../utilityClasses/doesFolderExist.h"


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
    std::string get_actualCreateInputs_path();
    std::string get_actualFinalOutput_path();
    std::string get_actual_run_base_name();
    std::string get_actualLcpPath();

    // will probably have to repeat the functions from the inputVariable_valueStorage class unfortunately.
    // Difference is the values won't be stored in this class, just in the one referenced here
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
    bool verifyFoundInputCombinations();
    bool findActualCreateInputsAndFinalOutputsPaths();
    bool findActualLcpPathAndBaseName();

    // class data members
    std::vector<inputVariable_info> inputVariableInfo;
    inputVariables_valueStorage inputVariableValues;
    inputParser theInputParser;

    // important for file management
    std::string actualCreateInputs_path;
    std::string actualFinalOutput_path;
    std::string actual_run_base_name;
    std::string actualLcpPath;

};

#endif // INPUTVARIABLESHANDLER_H
