#ifndef INPUTPARSER_H
#define INPUTPARSER_H

#include "inputVariables_valueStorage.h"


#include <fstream>


class inputParser
{
public:

    // constructors
    inputParser();

    // variable class getter functions
    void loadVariableInfo(inputVariables_valueStorage* newInputVariables);

    // main loader functions
    bool findVariableNames(std::string inputFileName);
    bool loadAllInputs(std::string inputFileName);

private:

    // parsing utility functions
    bool doesFileExist(std::string fileName);
    void removeLeadingWhitespace(std::string &inputString);
    void removeEndingWhitespace(std::string &inputString);
    void removeSeparator(std::string &inputString, std::string &varNamePart);   // returns varName and whatever is to the right of the separator
    bool variableNameAndCountFinder(std::ifstream &inputFile, std::string varName, std::string &varCountString, size_t &varNameLine);

    // typechecker functions
    bool isBool(std::string inputString);
    bool isInt(std::string inputString);
    bool isDlb(std::string inputString);
    bool isPureString(std::string inputString);
    bool isValidFilename(std::string inputString);
    bool isValidDate(int currentYear, int currentMonth, int currentDay, int currentHour, int currentMinute);

    // type conversion functions
    bool strToBool(std::string inputString, bool &conversionSuccess);
    int strToInt(std::string inputString, bool &conversionSuccess);
    double strToDbl(std::string inputString, bool &conversionSuccess);
    bool strToDate(std::string inputString, int &currentYear, int &currentMonth, int &currentDay, int &currentHour, int &currentMinute);

    // type count loader functions
    bool load_wrf_files(std::ifstream &inputFile, int varCount, size_t varNameLine);

    // class related data members
    inputVariables_valueStorage* inputVariableValueStorage;
    std::vector<inputVariable_info> inputVariablesInfo;

    // important parser data members
    std::string inputSeparator = ":";
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
