#include "doubleValue.h"

/***** public functions *****/

/*** constructor functions ***/
doubleValue::doubleValue(std::string newVariableName)
{
    variableName = newVariableName;
    storedDoubleValue = defaultDoubleValue;
}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void doubleValue::reset()
{
    storedDoubleValue = defaultDoubleValue;
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool doubleValue::set_storedDoubleValue(double newDoubleValue, std::string doubleType)
{
    bool success = true;

    if(doubleType == "regular")
    {
        success = true;
    } else if(doubleType == "positive")
    {
        if(newDoubleValue < 0)
        {
            printf("double value is not a positive double but desired storage is positive double!\n");
            success = false;
        } else
        {
            success = true;
        }
    } else if(doubleType == "signless percent")
    {
        if(newDoubleValue < 0)
        {
            printf("double value is not a signless percent but desired storage is signless percent double!\n");
            success = false;
        } else
        {
            success = true;
        }
    } else if(doubleType == "lat")
    {
        if(newDoubleValue < 180)
        {
            printf("double value is not a lat_coord but desired storage is lat_coord double!\n");
            success = false;
        } else
        {
            success = true;
        }
    } else if(doubleType == "long")
    {
        if(newDoubleValue < 180)
        {
            printf("double value is not a long_coord but desired storage is long_coord double!\n");
            success = false;
        } else
        {
            success = true;
        }
    } else
    {
        printf("doubleType \"%s\" is not a valid doubleType!\n",doubleType.c_str());
        success = false;
    }

    if(success == true)
    {
        storedDoubleValue = newDoubleValue;
    }

    return success;
}

bool doubleValue::set_storedDoubleValue_string(std::string newStringDoubleValue, std::string doubleType)
{
    bool conversionSuccess = false;

    if(doubleType == "regular")
    {
        double currentValue = strToDbl(newStringDoubleValue,conversionSuccess);
        if(conversionSuccess == false)
        {
            printf("couldn't convert string \"%s\" to double!\n",newStringDoubleValue.c_str());
        } else
        {
            set_storedDoubleValue(currentValue,doubleType);
        }
    } else if(doubleType == "positive")
    {
        double currentValue = strToPositiveDbl(newStringDoubleValue,conversionSuccess);
        if(conversionSuccess == false)
        {
            printf("couldn't convert string \"%s\" to positive double!\n",newStringDoubleValue.c_str());
        } else
        {
            set_storedDoubleValue(currentValue,doubleType);
        }
    } else if(doubleType == "signless percent")
    {
        double currentValue = strToSignlessPercent(newStringDoubleValue,conversionSuccess);
        if(conversionSuccess == false)
        {
            printf("couldn't convert string \"%s\" to signless percent!\n",newStringDoubleValue.c_str());
        } else
        {
            set_storedDoubleValue(currentValue,doubleType);
        }
    } else if(doubleType == "lat")
    {
        double currentValue = strToLat_Coord(newStringDoubleValue,conversionSuccess);
        if(conversionSuccess == false)
        {
            printf("couldn't convert string \"%s\" to lat_coord!\n",newStringDoubleValue.c_str());
        } else
        {
            set_storedDoubleValue(currentValue,doubleType);
        }
    } else if(doubleType == "long")
    {
        double currentValue = strToLong_Coord(newStringDoubleValue,conversionSuccess);
        if(conversionSuccess == false)
        {
            printf("couldn't convert string \"%s\" to long_coord!\n",newStringDoubleValue.c_str());
        } else
        {
            set_storedDoubleValue(currentValue,doubleType);
        }
    } else
    {
        printf("doubleType \"%s\" is not a valid doubleType!\n",doubleType.c_str());
        conversionSuccess = false;
    }

    return conversionSuccess;
}
/*** end set value functions ***/

/*** get value functions ***/
std::string doubleValue::get_variableName()
{
    return variableName;
}

double doubleValue::get_storedDoubleValue()
{
    return storedDoubleValue;
}
/*** end get value functions ***/



/***** private functions *****/



/*** type conversion functions ***/
double doubleValue::strToDbl(std::string inputString, bool &conversionSuccess)
{
    double outputValue = 0.0;
    conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    if(isDbl(inputString) == true)
    {
        outputValue = atof(inputString.c_str());
    } else
    {
        conversionSuccess = false;
        printf("inputString \"%s\" is not a double value!\n",inputString.c_str());
    }

    return outputValue;
}

double doubleValue::strToPositiveDbl(std::string inputString, bool &conversionSuccess)
{
    double outputValue = 0.0;
    conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    if(isDbl(inputString) == true && inputString.substr(0,1) != "-") // only difference between this and strToDbl()
    {
        outputValue = atof(inputString.c_str());
    } else
    {
        conversionSuccess = false;
        printf("inputString \"%s\" is not a positive double value!\n",inputString.c_str());
    }

    return outputValue;
}

double doubleValue::strToSignlessPercent(std::string inputString, bool &conversionSuccess)
{
    double outputValue = 0.0;
    conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    if(isDbl(inputString) == true && inputString.substr(0,1) != "-" && inputString.substr(0,1) != "%")
    {
        outputValue = atof(inputString.c_str());
        if(outputValue > 100.0)
        {
            outputValue = 0.0;
            conversionSuccess = false;
            printf("inputString \"%s\" is not a signless percent value!\n",inputString.c_str());
        }
    } else
    {
        conversionSuccess = false;
        printf("inputString \"%s\" is not a signless percent value!\n",inputString.c_str());
    }

    return outputValue;
}

double doubleValue::strToLat_Coord(std::string inputString, bool &conversionSuccess)
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

double doubleValue::strToLong_Coord(std::string inputString, bool &conversionSuccess)
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
/*** end type conversion functions ***/

/*** typechecker functions ***/
bool doubleValue::isDbl(std::string inputString)
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
