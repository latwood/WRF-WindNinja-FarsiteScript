#include "intValue.h"

/***** public functions *****/

/*** constructor functions ***/
intValue::intValue(std::string newVariableName)
{
    variableName = newVariableName;
    storedIntValue = defaultIntValue;
}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void intValue::reset()
{
    storedIntValue = defaultIntValue;
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool intValue::set_storedIntValue(int newIntValue)
{
    bool success = true;

    storedIntValue = newIntValue;

    return success;
}

bool intValue::set_storedIntValue_string(std::string newStringIntValue)
{
    bool conversionSuccess = false;

    int currentValue = strToInt(newStringIntValue,conversionSuccess);
    if(conversionSuccess == false)
    {
        printf("couldn't convert string \"%s\" to int!\n",newStringIntValue.c_str());
    } else
    {
        set_storedIntValue(currentValue);
    }

    return conversionSuccess;
}
/*** end set value functions ***/

/*** get value functions ***/
std::string intValue::get_variableName()
{
    return variableName;
}

int intValue::get_storedIntValue()
{
    return storedIntValue;
}
/*** end get value functions ***/



/***** private functions *****/



/*** type conversion functions ***/
int intValue::strToInt(std::string inputString, bool &conversionSuccess)
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
bool intValue::isInt(std::string inputString)
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
