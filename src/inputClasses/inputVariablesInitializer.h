#ifndef INPUTVARIABLESINITIALIZER_H
#define INPUTVARIABLESINITIALIZER_H


#include <iostream>
#include <vector>

#include "singleInputVariable.h"

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


// largest configVariable name will have this much space between its end and the description
#define MIN_VARNAME_WHITESPACE 3
// minimum allowable words per line for a description
#define MIN_WORDS_PER_DESCRIPTION_LINE 1    // I'm having trouble here cause variable names are big enough to be one line on the description
// largest allowed variable name and MIN_VARNAME_WHITESPACE size, to allow enough room for the description
#define DESCRIPTION_VARIABLENAME_COLUMNSIZEMAX 30
// this should be the maximum number of characters in ubuntu 16.04 terminal line when the terminal is first opened
#define MAX_DESCRIPTION_LINESIZE 80


class inputVariablesInitializer
{
public:

    // constructors
    inputVariablesInitializer();

    // get value functions
    std::vector<singleInputVariable> get_inputVariableStorage();
    std::string get_maxVarNameColumnWhitespace();

private:

    // setup functions
    void setupAvailableApplicationUseNames();
    void setupAvailableVariableCountTypes();
    //void* initializeVariableValueStorage(std::string variableName,std::string variableCountType);
    void initializeVariableValueStorage(singleInputVariable &new_singleInputVariable);
    void addVariable(std::string new_variableName,std::string new_applicationUseName,std::string new_variableCountType,std::string new_variableDescription);
    void setupAvailableVariables();


    // check setup functions
    bool check_setupForDuplicateVariableNames();
    bool check_setupForValidApplicationUseNames();
    bool check_setupForValidOrderingByApplicationUseNames();
    bool check_setupForValidVariableCountTypes();
    bool check_setupDescription();
    bool check_variableTypeClassObjectExistence();

    // description whitespace and line break calculations, with error checking
    bool calculateDescriptionWhiteSpace();
    bool calculateDescriptionLineBreaks();
    void calculate_maxVarNameColumnWhitespace();

    // class data members
    std::vector<singleInputVariable> inputVariableStorage;

    // variable info data members
    std::vector<std::string> allowedApplicationUseNames;
    std::vector<std::string> allowedVariableCountTypes;

    // for description printing
    unsigned int descriptionVariableNameColumnSize;
    std::string maxVarNameColumnWhitespace;

};

#endif // INPUTVARIABLESINITIALIZER_H
