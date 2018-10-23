#include "lat_longValue.h"

/***** public functions *****/

/*** constructor functions ***/
lat_longValue::lat_longValue(std::string newVariableName)
{
    variableName = newVariableName;
    storedLatValue = defaultLatValue;
    storedLongValue = defaultLongValue;
}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void lat_longValue::reset()
{
    storedLatValue = defaultLatValue;
    storedLongValue = defaultLongValue;
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool lat_longValue::set_storedLatLongValue(double newLat_CoordValue, double newLong_CoordValue)
{
    bool success = true;

    storedLatValue = newLat_CoordValue;
    storedLongValue = newLong_CoordValue;

    return success;
}

/*bool lat_longValue::set_storedLatLongValue_string(std::string newStringLatValue, std::string newStringLongValue)
{
    bool conversionSuccess = false;
    double currentLat_Coord = strToLat_Coord(newStringLatValue,conversionSuccess);
    if(conversionSuccess == false)
    {
        printf("couldn't convert string \"%s\" to lat_coord!\n",newStringLatValue.c_str());
    }

    conversionSuccess = false;
    double currentLong_Coord = strToLong_Coord(newStringLongValue,conversionSuccess);
    if(conversionSuccess == false)
    {
        printf("couldn't convert string \"%s\" to long_coord!\n",newStringLongValue.c_str());
    } else
    {
        set_storedLatLongValue(currentLat_Coord,currentLong_Coord);
    }

    return conversionSuccess;
}*/

bool lat_longValue::set_storedLatLongValue_string(std::string newLatLongValueString)
{
    double currentLat_Coord = 0.0;
    double currentLong_Coord = 0.0;
    bool conversionSuccess = strToLat_Long_Point(newLatLongValueString,currentLat_Coord,currentLong_Coord);
    if(conversionSuccess == false)
    {
        printf("couldn't convert string \"%s\" to LatLongValue!\n",newLatLongValueString.c_str());
    } else
    {
        set_storedLatLongValue(currentLat_Coord,currentLong_Coord);
    }

    return conversionSuccess;
}
/*** end set value functions ***/

/*** get value functions ***/
std::string lat_longValue::get_variableName()
{
    return variableName;
}

double lat_longValue::get_storedLatValue()
{
    return storedLatValue;
}

double lat_longValue::get_storedLongValue()
{
    return storedLongValue;
}
/*** end get value functions ***/



/***** private functions *****/



/*** type conversion functions ***/
double lat_longValue::strToLat_Coord(std::string inputString, bool &conversionSuccess)
{
    double outputValue = 0.0;
    conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    if(isDbl(inputString) == true)
    {
        outputValue = atof(inputString.c_str());
        if(outputValue < -90.0 || outputValue > 90.0)
        {
            outputValue = 0.0;
            conversionSuccess = false;
            printf("inputString \"%s\" is not a lat_coord value!\n",inputString.c_str());
        }
    } else
    {
        conversionSuccess = false;
        printf("inputString \"%s\" is not a lat_coord value!\n",inputString.c_str());
    }

    return outputValue;
}

double lat_longValue::strToLong_Coord(std::string inputString, bool &conversionSuccess)
{
    double outputValue = 0.0;
    conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    if(isDbl(inputString) == true)
    {
        outputValue = atof(inputString.c_str());
        if(outputValue < -180.0 || outputValue > 180.0)
        {
            outputValue = 0.0;
            conversionSuccess = false;
            printf("inputString \"%s\" is not a long_coord value!\n",inputString.c_str());
        }
    } else
    {
        conversionSuccess = false;
        printf("inputString \"%s\" is not a long_coord value!\n",inputString.c_str());
    }

    return outputValue;
}

bool lat_longValue::strToLat_Long_Point(std::string inputString, double &currentLat_Coord, double &currentLong_Coord)
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
        if(currentChr == " " && isValue == true)
        {
            foundValues.push_back(inputString.substr(startValueSpot,charIdx-startValueSpot));
            isValue = false;
        }
        if(currentChr != " " && isValue == false)
        {
            startValueSpot = charIdx;
            isValue = true;
        }
        if(charIdx == inputString.length()-1 && isValue == true)
        {
            foundValues.push_back(inputString.substr(startValueSpot,charIdx-startValueSpot));
        }
    }

    /*
    printf("found values are:");
    for(size_t valIdx = 0; valIdx < foundValues.size(); valIdx++)
    {
        printf(" \"%s\"",foundValues[valIdx].c_str());
    }
    printf("\n\n");*/

    if(foundValues.size() < 2)
    {
        printf("not enough values found in lat_long_point!\n");
        conversionSuccess = false;
    } else
    {
        bool isDouble = false;
        double foundLat_Coord = strToLat_Coord(foundValues[0],isDouble);   // this checks to make sure it is a valid Lat_Coord too
        if(isDouble == false)
        {
            printf("problem converting \"%s\" value to double!\n",foundValues[0].c_str());
            conversionSuccess = false;
        }
        double foundLong_Coord = strToLong_Coord(foundValues[1],isDouble); // this checks to make sure it is a valid Long_Coord too
        if(isDouble == false)
        {
            printf("problem converting \"%s\" value to double!\n",foundValues[1].c_str());
            conversionSuccess = false;
        }
        if(conversionSuccess == true)
        {
            currentLat_Coord = foundLat_Coord;
            currentLong_Coord = foundLong_Coord;
            conversionSuccess = true;
        }
    }

    return conversionSuccess;
}
/*** end type conversion functions ***/

/*** typechecker functions ***/
bool lat_longValue::isDbl(std::string inputString)
{
    bool conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    size_t decimalCounter = 0;
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
                    conversionSuccess = false;  // can't have "." for first char, only a number, +, or - sign
                    break;
                }
            } else
            {
                if(currentChr == ".")
                {
                    if(inputString.substr(charIdx-1,1) >= "0" || inputString.substr(charIdx-1,1) <= "9")
                    {
                        decimalCounter = decimalCounter + 1;
                        if(decimalCounter > 1)
                        {
                            conversionSuccess = false;  // only allow one decimal per string
                            break;
                        }
                    } else
                    {
                        conversionSuccess = false;  // need valid number before a decimal point (avoid hexadecimal stuff with no 0 . stuff)
                        break;
                    }
                } else
                {
                    conversionSuccess = false; // has to be a valid decimal point at this point
                    break;
                }
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
