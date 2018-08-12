#ifndef INPUTPARSER_H
#define INPUTPARSER_H

#include "inputVariables_valueStorage.h"


#include <fstream>

#include <sys/types.h>
#include <sys/stat.h>

#include "gdal.h"
#include "cpl_conv.h"   // for CPLMalloc()


class inputParser
{
public:

    // constructors
    inputParser();

    // variable class getter functions
    void loadVariableInfo(inputVariables_valueStorage* newInputVariables);

    // data passer between inputVariablesHandler and inputParser functions
    // man I know there is a smarter way and it involves pointers and probably copy constructors, but I'm in a hurry
    std::vector<inputVariable_info> transferVariableInfo();

    // main loader functions
    bool findVariableNamesAndCountValues(std::string inputFileName);
    bool loadAllInputs(std::string inputFileName);

private:

    // parsing utility functions
    bool doesRegularFileExist(std::string fileName, bool suppressWarnings);
    bool doesFilenameExist(std::string fileName);
    bool openIsStream(std::string fileName,std::ifstream &fzStream);
    void closeIsStream(std::ifstream &fzStream);
    bool findInputTotalLineCount(std::string inputFileName);
    void removeLeadingWhitespace(std::string &inputString);
    void removeEndingWhitespace(std::string &inputString);
    void removeSeparator(std::string &inputString, std::string &varNamePart);   // returns varName and whatever is to the right of the separator without whitespace before or after
    bool variableNameAndCountFinder(std::string inputFileName, std::string varName, std::string &varCountString, size_t &varNameLine, bool &foundVar);

    // typechecker functions
    bool isBool(std::string inputString);
    bool isSize_t(std::string inputString);
    bool isInt(std::string inputString);
    bool isDbl(std::string inputString);
    bool isPureString(std::string inputString);
    bool isPureFilenameString(std::string inputString);
    bool isValidString(std::string inputString, std::string varName);
    bool isValidPathname(std::string inputString);
    bool isValidRegularFilename(std::string inputString);
    bool isValidLcpFilename(std::string inputString, bool suppressWarnings);
    bool isValidShapeFilename(std::string inputString, bool suppressWarnings);
    bool isValidNetCDFFilename(std::string inputString, bool suppressWarnings);
    bool isValidDate(int currentYear, int currentMonth, int currentDay, int currentHour, int currentMinute);

    // type conversion functions
    bool strToBool(std::string inputString, bool &conversionSuccess);
    size_t strToSize_t(std::string inputString, bool &conversionSuccess);
    int strToInt(std::string inputString, bool &conversionSuccess);
    double strToDbl(std::string inputString, bool &conversionSuccess);
    double strToPositiveDbl(std::string inputString, bool &conversionSuccess);
    double strToSignlessPercent(std::string inputString, bool &conversionSuccess);
    double strToLat_Coord(std::string inputString, bool &conversionSuccess);
    double strToLong_Coord(std::string inputString, bool &conversionSuccess);
    bool strToLat_Long_Point(std::string inputString, double &currentLat_Coord, double &currentLong_Coord);
    bool strToDate(std::string inputString, int &currentYear, int &currentMonth, int &currentDay, int &currentHour, int &currentMinute);
    bool strToHour_Min(std::string inputString, int &currentHour, int &currentMinute);

    // type count loader functions
    bool load_create_ignition_from_latlongs(std::string inputFileName, size_t varCount, size_t varNameLine);
    bool load_polygon_ignit_shape_files(std::string inputFileName, size_t varCount, size_t varNameLine);
    bool load_GeoMAC_fire_perimeter_files(std::string inputFileName, size_t varCount, size_t varNameLine);
    bool load_farsite_output_fire_perimeter_files(std::string inputFileName, size_t varCount, size_t varNameLine);
    bool load_wrf_files(std::string inputFileName, size_t varCount, size_t varNameLine);
    bool load_additional_WindNinja_outputs_google(std::string inputFileName, size_t varCount, size_t varNameLine);
    bool load_additional_WindNinja_outputs_shapefile(std::string inputFileName, size_t varCount, size_t varNameLine);
    bool load_additional_WindNinja_outputs_pdf(std::string inputFileName, size_t varCount, size_t varNameLine);

    // class related data members
    inputVariables_valueStorage* inputVariableValueStorage;
    std::vector<inputVariable_info> inputVariablesInfo;

    // important parser data members
    std::string inputSeparator = ":";
    size_t inputFileTotalLines;
    struct dateInfo
    {
        std::vector<int> daysPerMonth = {31,28,31,30,31,30,31,31,30,31,30,31};
        // simplified case: leap years are when the year mod 4 is zero, except if year mod 100 is 0 and year mod 400 is not 0
        // this is true for all years past probably sometime in the 1600's. Also not sure how this works with foreign calendars
        int hour_min = 0;
        int hour_max = 23;
        int minute_min = 0;
        int minute_max = 59;
    };
    dateInfo validDates;

};

#endif // INPUTPARSER_H
