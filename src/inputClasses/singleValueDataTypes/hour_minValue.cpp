#include "hour_minValue.h"

/***** public functions *****/

/*** constructor functions ***/
hour_minValue::hour_minValue(std::string newVariableName)
{
    variableName = newVariableName;
    storedHourValue = defaultHourValue;
    storedMinuteValue = defaultHourValue;
}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void hour_minValue::reset()
{
    storedHourValue = defaultHourValue;
    storedMinuteValue = defaultHourValue;
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool hour_minValue::set_storedHour_MinValue(int newHourValue, int newMinuteValue)
{
    bool success = true;

    storedHourValue = newHourValue;
    storedMinuteValue = newMinuteValue;

    return success;
}

/*bool hour_minValue::set_storedHour_MinValue_string(std::string newStringHourValue, std::string newStringMinuteValue)
{
    bool conversionSuccess = false;
    int currentHour = strToInt(newStringHourValue,conversionSuccess);
    if(conversionSuccess == false)
    {
        printf("couldn't convert string \"%s\" to hour!\n",newStringHourValue.c_str());
    }

    conversionSuccess = false;
    int currentMinute = strToInt(newStringMinuteValue,conversionSuccess);
    if(conversionSuccess == false)
    {
        printf("couldn't convert string \"%s\" to minute!\n",newStringMinuteValue.c_str());
    } else
    {
        set_storedHour_MinValue(currentHour,currentMinute);
    }

    return conversionSuccess;
}*/

bool hour_minValue::set_storedHour_MinValue_string(std::string newHourMinValue)
{
    int currentHour = 0;
    int currentMinute = 0;
    bool conversionSuccess = strToHour_Min(newHourMinValue,currentHour,currentMinute);
    if(conversionSuccess == false)
    {
        printf("couldn't convert string \"%s\" to HourMinValue!\n",newHourMinValue.c_str());
    } else
    {
        set_storedHour_MinValue(currentHour,currentMinute);
    }

    return conversionSuccess;
}
/*** end set value functions ***/

/*** get value functions ***/
std::string hour_minValue::get_variableName()
{
    return variableName;
}

int hour_minValue::get_storedHourValue()
{
    return storedHourValue;
}

int hour_minValue::get_storedMinuteValue()
{
    return storedMinuteValue;
}
/*** end get value functions ***/



/***** private functions *****/



/*** type conversion functions ***/
bool hour_minValue::strToHour_Min(std::string inputString, int &currentHour, int &currentMinute)
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
    if(foundValues.size() < 2)
    {
        printf("not enough values found in hour_min!\n");
        conversionSuccess = false;
    } else
    {
        bool isInt = false;
        int foundHour = strToInt(foundValues[0],isInt);
        if(isInt == false)
        {
            printf("problem converting \"%s\" value to int!\n",foundValues[0].c_str());
            conversionSuccess = false;
        }
        isInt = false;
        int foundMinute = strToInt(foundValues[1],isInt);
        if(isInt == false)
        {
            printf("problem converting \"%s\" value to int!\n",foundValues[1].c_str());
            conversionSuccess = false;
        }
        if(conversionSuccess == true)
        {
            if(isValidDate(0,1,1,foundHour,foundMinute) == false)   // hmm, might have found an issue, should be 0,1,1 as allowed values that won't change anything for year, month, and day
            {
                printf("hour_min is not valid!\n");
                conversionSuccess = false;
            } else
            {
                currentHour = foundHour;
                currentMinute = foundMinute;
                conversionSuccess = true;
            }
        }
    }

    return conversionSuccess;
}

int hour_minValue::strToInt(std::string inputString, bool &conversionSuccess)
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
bool hour_minValue::isValidDate(int currentYear, int currentMonth, int currentDay, int currentHour, int currentMinute)
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

bool hour_minValue::isInt(std::string inputString)
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
