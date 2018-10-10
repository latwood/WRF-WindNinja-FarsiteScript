#include "dateValue.h"

/***** public functions *****/

/*** constructor functions ***/
dateValue::dateValue(std::string newVariableName)
{
    variableName = newVariableName;
    storedYearValue = defaultYearValue;
    storedMonthValue = defaultMonthValue;
    storedDayValue = defaultDayValue;
    storedHourValue = defaultHourValue;
    storedMinuteValue = defaultMinuteValue;
}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void dateValue::reset()
{
    storedYearValue = defaultYearValue;
    storedMonthValue = defaultMonthValue;
    storedDayValue = defaultDayValue;
    storedHourValue = defaultHourValue;
    storedMinuteValue = defaultMinuteValue;
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool dateValue::set_storedDateValue(int newYearValue, int newMonthValue, int newDayValue, int newHourValue, int newMinuteValue)
{
    bool success = true;

    storedYearValue = newYearValue;
    storedMonthValue = newMonthValue;
    storedDayValue = newDayValue;
    storedHourValue = newHourValue;
    storedMinuteValue = newMinuteValue;

    return success;
}

/*bool dateValue::set_storedDateValue_string(std::string newStringYearValue, std::string newStringMonthValue, std::string newStringDayValue, std::string newStringHourValue, std::string newStringMinuteValue)
{
    bool conversionSuccess = false;
    int currentYearValue = strToInt(newStringYearValue,conversionSuccess);
    if(conversionSuccess == false)
    {
        printf("couldn't convert string \"%s\" to year!\n",newStringYearValue.c_str());
    }

    conversionSuccess = false;
    int currentMonthValue = strToInt(newStringMonthValue,conversionSuccess);
    if(conversionSuccess == false)
    {
        printf("couldn't convert string \"%s\" to month!\n",newStringMonthValue.c_str());
    }

    conversionSuccess = false;
    int currentDayValue = strToInt(newStringDayValue,conversionSuccess);
    if(conversionSuccess == false)
    {
        printf("couldn't convert string \"%s\" to day!\n",newStringDayValue.c_str());
    }

    conversionSuccess = false;
    int currentHourValue = strToInt(newStringHourValue,conversionSuccess);
    if(conversionSuccess == false)
    {
        printf("couldn't convert string \"%s\" to hour!\n",newStringHourValue.c_str());
    }

    conversionSuccess = false;
    int currentMinuteValue = strToInt(newStringMinuteValue,conversionSuccess);
    if(conversionSuccess == false)
    {
        printf("couldn't convert string \"%s\" to minute!\n",newStringMinuteValue.c_str());
    } else
    {
        set_storedDateValue(currentYearValue,currentMonthValue,currentDayValue,currentHourValue,currentMinuteValue);
    }

    return conversionSuccess;
}*/

bool dateValue::set_storedDateValue_string(std::string newDateValueString)
{
    int currentYear = 0;
    int currentMonth = 0;
    int currentDay = 0;
    int currentHour = 0;
    int currentMinute = 0;
    bool conversionSuccess = strToDate(newDateValueString,currentYear,currentMonth,currentDay,currentHour,currentMinute);
    if(conversionSuccess == false)
    {
        printf("couldn't convert string \"%s\" to DateValue!\n",newDateValueString.c_str());
    } else
    {
        set_storedDateValue(currentYear,currentMonth,currentDay,currentHour,currentMinute);
    }

    return conversionSuccess;
}
/*** end set value functions ***/

/*** get value functions ***/
std::string dateValue::get_variableName()
{
    return variableName;
}

int dateValue::get_storedYearValue()
{
    return storedYearValue;
}

int dateValue::get_storedMonthValue()
{
    return storedMonthValue;
}

int dateValue::get_storedDayValue()
{
    return storedDayValue;
}

int dateValue::get_storedHourValue()
{
    return storedHourValue;
}

int dateValue::get_storedMinuteValue()
{
    return storedMinuteValue;
}
/*** end get value functions ***/



/***** private functions *****/



/*** type conversion functions ***/
bool dateValue::strToDate(std::string inputString, int &currentYear, int &currentMonth, int &currentDay, int &currentHour, int &currentMinute)
{
    bool conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    bool isValue = true;
    std::vector<std::string> foundValues;
    size_t startValueSpot = 0;
    for(size_t charIdx = 0; charIdx < inputString.length(); charIdx++)
    {
        std::string currentChr = inputString.substr(charIdx,1);
        if((currentChr == " " && isValue == true) || (currentChr == ":" && isValue == true))
        {
            foundValues.push_back(inputString.substr(startValueSpot,charIdx-startValueSpot));
            isValue = false;
        }
        if(currentChr != " " && currentChr != ":" && isValue == false)
        {
            startValueSpot = charIdx;
            isValue = true;
        }
        if(charIdx == inputString.length()-1 && isValue == true)
        {
            foundValues.push_back(inputString.substr(startValueSpot,charIdx-startValueSpot+1));
        }
    }

    /*printf("found values are:");
    for(size_t valIdx = 0; valIdx < foundValues.size(); valIdx++)
    {
        printf(" \"%s\"",foundValues[valIdx].c_str());
    }
    printf("\n\n");*/
    if(foundValues.size() < 5)
    {
        printf("not enough values found in date!\n");
        conversionSuccess = false;
    } else
    {
        bool isInt = false;
        int foundMonth = strToInt(foundValues[0],isInt);
        if(isInt == false)
        {
            printf("problem converting \"%s\" value to int!\n",foundValues[0].c_str());
            conversionSuccess = false;
        }
        isInt = false;
        int foundDay = strToInt(foundValues[1],isInt);
        if(isInt == false)
        {
            printf("problem converting \"%s\" value to int!\n",foundValues[1].c_str());
            conversionSuccess = false;
        }
        isInt = false;
        int foundYear = strToInt(foundValues[2],isInt);
        if(isInt == false)
        {
            printf("problem converting \"%s\" value to int!\n",foundValues[2].c_str());
            conversionSuccess = false;
        }
        isInt = false;
        int foundHour = strToInt(foundValues[3],isInt);
        if(isInt == false)
        {
            printf("problem converting \"%s\" value to int!\n",foundValues[3].c_str());
            conversionSuccess = false;
        }
        isInt = false;
        int foundMinute = strToInt(foundValues[4],isInt);
        if(isInt == false)
        {
            printf("problem converting \"%s\" value to int!\n",foundValues[4].c_str());
            conversionSuccess = false;
        }
        if(conversionSuccess == true)
        {
            if(isValidDate(foundYear,foundMonth,foundDay,foundHour,foundMinute) == false)
            {
                printf("date is not valid!\n");
                conversionSuccess = false;
            } else
            {
                currentYear = foundYear;
                currentMonth = foundMonth;
                currentDay = foundDay;
                currentHour = foundHour;
                currentMinute = foundMinute;
                conversionSuccess = true;
            }
        }
    }

    return conversionSuccess;
}

int dateValue::strToInt(std::string inputString, bool &conversionSuccess)
{
    int outputValue = 0;
    conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    if(isInt(inputString) == true)
    {
        outputValue = atoi(inputString.c_str());
    } else
    {
        conversionSuccess = false;
        printf("inputString \"%s\" is not an int value!\n",inputString.c_str());
    }

    return outputValue;
}
/*** end type conversion functions ***/

/*** typechecker functions ***/
bool dateValue::isValidDate(int currentYear, int currentMonth, int currentDay, int currentHour, int currentMinute)
{
    bool conversionSuccess = true;

    bool isLeapYear = false;
    if(currentYear < 0)
    {
        conversionSuccess = false;
    } else
    {
        if(currentYear % 4 == 0 && currentYear % 100 == 0)
        {
            if(currentYear % 400 != 0)
            {
                isLeapYear = true;
            }
        }
    }

    if(currentMonth < 1 || currentMonth > 12)
    {
        conversionSuccess = false;
    } else
    {
        if(currentMonth == 2 && isLeapYear == true)
        {
            if(currentDay < 1 || currentDay > validDates.daysPerMonth[currentMonth-1]+1)
            {
                conversionSuccess = false;
            }
        } else
        {
            if(currentDay < 1 || currentDay > validDates.daysPerMonth[currentMonth-1])
            {
                conversionSuccess = false;
            }
        }
    }

    if(currentHour < validDates.hour_min || currentHour > validDates.hour_max)
    {
        conversionSuccess = false;
    }

    if(currentMinute < validDates.minute_min || currentMinute > validDates.minute_max)
    {
        conversionSuccess = false;
    }

    return conversionSuccess;
}

bool dateValue::isInt(std::string inputString)
{
    bool conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    size_t digitCounter = 0;
    for(size_t charIdx = 0; charIdx < inputString.length(); charIdx++)
    {
        std::string currentChr = inputString.substr(charIdx,1);
        if(currentChr >= "0" || currentChr <= "9")
        {
            digitCounter = digitCounter + 1;
        } else
        {
            if(charIdx == 0)
            {
                if(inputString.substr(0,1) == "+" || inputString.substr(0,1) == "-")
                {
                     // is okay
                } else
                {
                    conversionSuccess = false;  // only time non number values are allowed is first char and only + or - signs
                    break;
                }
            } else
            {
                conversionSuccess = false; // only numbers allowed at this point
                break;
            }
        }
    }

    if(conversionSuccess == true)
    {
        if(digitCounter <= 0)
        {
            conversionSuccess = false;  // got to this point somehow, but there wasn't actually any number stuff in the string
        }
    }

    return conversionSuccess;
}
/*** end typechecker functions ***/
