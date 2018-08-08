#include "inputVariables_valueStorage.h"

/***** public functions *****/

/*** constructor functions ***/
inputVariables_valueStorage::inputVariables_valueStorage()
{
    bool success = true;

    inputVariableInfo = setupInputVariableInfo.get_inputVariableInfo();

    if(sortInputVariableInfo() == false)
    {
        printf("invalid input variable storage value options found during setup!\n");
        success = false;
    }

    if(success == false)
    {
        printf("exiting program\n");
        exit(1);
    }
}
/*** end constructor functions ***/

/*** reinit functions (like constructors) ***/
void inputVariables_valueStorage::resetVariables()
{
    // reset values for all single count types to default values (keep the stuff with string names from setup)
    for(size_t boolVarIdx = 0; boolVarIdx < boolValues.size(); boolVarIdx++)
    {
        boolValues[boolVarIdx].variableValue = defaultBoolValue;
    }
    for(size_t intVarIdx = 0; intVarIdx < intValues.size(); intVarIdx++)
    {
        intValues[intVarIdx].variableValue = defaultIntValue;
    }
    for(size_t doubleVarIdx = 0; doubleVarIdx < doubleValues.size(); doubleVarIdx++)
    {
        doubleValues[doubleVarIdx].variableValue = defaultDoubleValue;
    }
    for(size_t stringVarIdx = 0; stringVarIdx < stringValues.size(); stringVarIdx++)
    {
        stringValues[stringVarIdx].variableValue = defaultStringValue;
    }
    for(size_t filenameVarIdx = 0; filenameVarIdx < filenameValues.size(); filenameVarIdx++)
    {
        filenameValues[filenameVarIdx].variableValue = defaultFilenameValue;
    }
    for(size_t dateVarIdx = 0; dateVarIdx < dateValues.size(); dateVarIdx++)
    {
        dateValues[dateVarIdx].year = defaultYearValue;
        dateValues[dateVarIdx].month = defaultMonthValue;
        dateValues[dateVarIdx].day = defaultDayValue;
        dateValues[dateVarIdx].hour = defaultHourValue;
        dateValues[dateVarIdx].minute = defaultMinuteValue;
    }

    // now all the count specific variable resetting
    while(!wrf_files.empty())
    {
        wrf_files.pop_back();
    }

}
/*** end reinit functions (like constructors) ***/

/*** get variable info functions ***/
std::vector<inputVariable_info> inputVariables_valueStorage::get_inputVariableInfo()
{
    return inputVariableInfo;
}

std::string inputVariables_valueStorage::get_maxVarNameColumnWhitespace()
{
    return setupInputVariableInfo.get_maxVarNameColumnWhitespace();
}
/*** end get variable info functions ***/

/*** set variable value functions ***/
bool inputVariables_valueStorage::set_inputVariableBoolValue(std::string varName, bool newBoolValue)
{
    bool success = true;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < boolValues.size(); varIdx++)
    {
        if(boolValues[varIdx].variableName == varName)
        {
            boolValues[varIdx].variableValue = newBoolValue;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not set bool value \"%d\" for variable name \"%s\", variableName \%s\" does not exist in bool value storage!",newBoolValue,varName.c_str(),varName.c_str());
        success = false;
    }

    return success;
}

bool inputVariables_valueStorage::set_inputVariableIntValue(std::string varName, int newIntValue)
{
    bool success = true;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < intValues.size(); varIdx++)
    {
        if(intValues[varIdx].variableName == varName)
        {
            intValues[varIdx].variableValue = newIntValue;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not set int value \"%d\" for variable name \"%s\", variableName \%s\" does not exist in int value storage!",newIntValue,varName.c_str(),varName.c_str());
        success = false;
    }

    return success;
}
bool inputVariables_valueStorage::set_inputVariableDoubleValue(std::string varName, double newDoubleValue)
{
    bool success = true;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < doubleValues.size(); varIdx++)
    {
        if(doubleValues[varIdx].variableName == varName)
        {
            doubleValues[varIdx].variableValue = newDoubleValue;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not set double value \"%f\" for variable name \"%s\", variableName \%s\" does not exist in double value storage!",newDoubleValue,varName.c_str(),varName.c_str());
        success = false;
    }

    return success;
}

bool inputVariables_valueStorage::set_inputVariableStringValue(std::string varName, std::string newStringValue)
{
    bool success = true;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < stringValues.size(); varIdx++)
    {
        if(stringValues[varIdx].variableName == varName)
        {
            stringValues[varIdx].variableValue = newStringValue;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not set string value \"%s\" for variable name \"%s\", variableName \%s\" does not exist in string value storage!",newStringValue.c_str(),varName.c_str(),varName.c_str());
        success = false;
    }

    return success;
}

bool inputVariables_valueStorage::set_inputVariableFilenameValue(std::string varName, std::string newFilenameValue)
{
    bool success = true;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < filenameValues.size(); varIdx++)
    {
        if(filenameValues[varIdx].variableName == varName)
        {
            filenameValues[varIdx].variableValue = newFilenameValue;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not set filename value \"%s\" for variable name \"%s\", variableName \%s\" does not exist in filename value storage!",newFilenameValue.c_str(),varName.c_str(),varName.c_str());
        success = false;
    }

    return success;
}

bool inputVariables_valueStorage::set_inputVariableDateValue(std::string varName, int year, int month, int day, int hour, int minute)
{
    bool success = true;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < dateValues.size(); varIdx++)
    {
        if(dateValues[varIdx].variableName == varName)
        {
            dateValues[varIdx].year = year;
            dateValues[varIdx].month = month;
            dateValues[varIdx].day = day;
            dateValues[varIdx].hour = hour;
            dateValues[varIdx].minute = minute;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not set date value \"%d %d %d %d %d\" (year month day hour minute) for variable name \"%s\", variableName \%s\" does not exist in date value storage!",year,month,day,hour,minute,varName.c_str(),varName.c_str());
        success = false;
    }

    return success;
}
/*** end set variable value functions ***/

/*** load function stuff for count type variables ***/
bool inputVariables_valueStorage::add_wrf_file(std::string new_wrf_file)
{
    // might want to check to see if file is a legit file? Here or somewhere else? Format checking?
    // I guess the assumption right now is that the parser handles all the format checking and if it is a legit file, just calls storage functions from here
    wrf_files.push_back(new_wrf_file);
}
/*** end load function stuff for count type variables ***/

/*** get variable value functions ***/
bool inputVariables_valueStorage::get_inputVariableBoolValue(std::string varName)
{
    bool outputValue = defaultBoolValue;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < boolValues.size(); varIdx++)
    {
        if(boolValues[varIdx].variableName == varName)
        {
            outputValue = boolValues[varIdx].variableValue;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not get bool value for variable name \"%s\", variableName \%s\" does not exist in bool value storage!",varName.c_str(),varName.c_str());
        exit(1);
    }

    return outputValue;
}

int inputVariables_valueStorage::get_inputVariableIntValue(std::string varName)
{
    int outputValue = defaultIntValue;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < intValues.size(); varIdx++)
    {
        if(intValues[varIdx].variableName == varName)
        {
            outputValue = intValues[varIdx].variableValue;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not get int value for variable name \"%s\", variableName \%s\" does not exist in int value storage!",varName.c_str(),varName.c_str());
        exit(1);
    }

    return outputValue;
}

double inputVariables_valueStorage::get_inputVariableDoubleValue(std::string varName)
{
    double outputValue = defaultDoubleValue;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < doubleValues.size(); varIdx++)
    {
        if(doubleValues[varIdx].variableName == varName)
        {
            outputValue = doubleValues[varIdx].variableValue;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not get double value for variable name \"%s\", variableName \%s\" does not exist in double value storage!",varName.c_str(),varName.c_str());
        exit(1);
    }

    return outputValue;
}

std::string inputVariables_valueStorage::get_inputVariableStringValue(std::string varName)
{
    std::string outputValue = defaultStringValue;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < stringValues.size(); varIdx++)
    {
        if(stringValues[varIdx].variableName == varName)
        {
            outputValue = stringValues[varIdx].variableValue;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not get string value for variable name \"%s\", variableName \%s\" does not exist in string value storage!",varName.c_str(),varName.c_str());
        exit(1);
    }

    return outputValue;
}

std::string inputVariables_valueStorage::get_inputVariableFilenameValue(std::string varName)
{
    std::string outputValue = defaultFilenameValue;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < filenameValues.size(); varIdx++)
    {
        if(filenameValues[varIdx].variableName == varName)
        {
            outputValue = filenameValues[varIdx].variableValue;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not get filename value for variable name \"%s\", variableName \%s\" does not exist in filename value storage!",varName.c_str(),varName.c_str());
        exit(1);
    }

    return outputValue;
}

dateValue inputVariables_valueStorage::get_inputVariableDateValue(std::string varName)
{
    dateValue outputValue = {0,defaultYearValue,defaultMonthValue,defaultDayValue,defaultHourValue,defaultMinuteValue};

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < dateValues.size(); varIdx++)
    {
        if(dateValues[varIdx].variableName == varName)
        {
            outputValue = dateValues[varIdx];
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not get date value for variable name \"%s\", variableName \%s\" does not exist in date value storage!",varName.c_str(),varName.c_str());
        exit(1);
    }

    return outputValue;
}

int inputVariables_valueStorage::get_inputVariableDateValueYear(std::string varName)
{
    int outputValue = defaultYearValue;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < dateValues.size(); varIdx++)
    {
        if(dateValues[varIdx].variableName == varName)
        {
            outputValue = dateValues[varIdx].year;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not get date year value for variable name \"%s\", variableName \%s\" does not exist in date value storage!",varName.c_str(),varName.c_str());
        exit(1);
    }

    return outputValue;
}

int inputVariables_valueStorage::get_inputVariableDateValueMonth(std::string varName)
{
    int outputValue = defaultMonthValue;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < dateValues.size(); varIdx++)
    {
        if(dateValues[varIdx].variableName == varName)
        {
            outputValue = dateValues[varIdx].month;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not get date month value for variable name \"%s\", variableName \%s\" does not exist in date value storage!",varName.c_str(),varName.c_str());
        exit(1);
    }

    return outputValue;
}

int inputVariables_valueStorage::get_inputVariableDateValueDay(std::string varName)
{
    int outputValue = defaultDayValue;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < dateValues.size(); varIdx++)
    {
        if(dateValues[varIdx].variableName == varName)
        {
            outputValue = dateValues[varIdx].day;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not get date day value for variable name \"%s\", variableName \%s\" does not exist in date value storage!",varName.c_str(),varName.c_str());
        exit(1);
    }

    return outputValue;
}

int inputVariables_valueStorage::get_inputVariableDateValueHour(std::string varName)
{
    int outputValue = defaultHourValue;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < dateValues.size(); varIdx++)
    {
        if(dateValues[varIdx].variableName == varName)
        {
            outputValue = dateValues[varIdx].hour;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not get date hour value for variable name \"%s\", variableName \%s\" does not exist in date value storage!",varName.c_str(),varName.c_str());
        exit(1);
    }

    return outputValue;
}

int inputVariables_valueStorage::get_inputVariableDateValueMinute(std::string varName)
{
    int outputValue = defaultHourValue;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < dateValues.size(); varIdx++)
    {
        if(dateValues[varIdx].variableName == varName)
        {
            outputValue = dateValues[varIdx].hour;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not get date hour value for variable name \"%s\", variableName \%s\" does not exist in date value storage!",varName.c_str(),varName.c_str());
        exit(1);
    }

    return outputValue;
}

std::vector<std::string> inputVariables_valueStorage::get_wrf_files()
{
    return wrf_files;
}
/*** end get variable value functions ***/



/***** private functions *****/



/*** setup functions ***/
bool inputVariables_valueStorage::sortInputVariableInfo()
{
    bool success = true;

    for(size_t varIdx = 0; varIdx < inputVariableInfo.size(); varIdx++)
    {
        if(inputVariableInfo[varIdx].get_variableCountType() == "bool")
        {
            boolValues.push_back({inputVariableInfo[varIdx].get_variableName(),defaultBoolValue});
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "int")
        {
            intValues.push_back({inputVariableInfo[varIdx].get_variableName(),defaultIntValue});
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "double")
        {
            doubleValues.push_back({inputVariableInfo[varIdx].get_variableName(),defaultDoubleValue});
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "string")
        {
            stringValues.push_back({inputVariableInfo[varIdx].get_variableName(),defaultStringValue});
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "filename")
        {
            filenameValues.push_back({inputVariableInfo[varIdx].get_variableName(),defaultFilenameValue});
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "date")
        {
            dateValues.push_back({inputVariableInfo[varIdx].get_variableName(),defaultYearValue,defaultMonthValue,defaultDayValue,defaultHourValue,defaultMinuteValue});
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "count")
        {
            /* wait, probably don't even need to do anything in this section, the names won't be important for this section.
             * Unless of coarse all count related data types also get defined to have a variable name, which is not a very bad idea */
            /*// now need to filter through and setup all the loader functions
            if(inputVariableInfo[varIdx].get_loaderFunctionName() == "load_wrf_files")
            {
                if(load_wrf_files() == false)
                {
                    printf("failed to run load_wrf_files loader function during input parsing!\n");
                    success = false;
                }
            } else
            {
                printf("loader function \"%s\" for variable \"%s\" which has countType \"count\" has not been implemented yet!\n",inputVariableInfo[varIdx].get_loaderFunctionName().c_str(),inputVariableInfo[varIdx].get_variableName().c_str());
                success = false;
            }*/
        } else
        {
            printf("count type \"%s\" for variable \"%s\" has not implemented in code yet!",inputVariableInfo[varIdx].get_variableCountType().c_str(),inputVariableInfo[varIdx].get_variableName().c_str());
            success = false;
        }
    }

    return success;
}
/*** end setup functions ***/
