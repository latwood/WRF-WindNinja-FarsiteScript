#include "size_tValue.h"

/***** public functions *****/

/*** constructor functions ***/
size_tValue::size_tValue()
{
    variableName = "";
    storedSize_tValue = defaultSize_tValue;
}

size_tValue::size_tValue(std::string newVariableName)
{
    variableName = newVariableName;
    storedSize_tValue = defaultSize_tValue;
}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void size_tValue::reset()
{
    storedSize_tValue = defaultSize_tValue;
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
void size_tValue::set_variableName(std::string newVariableName)
{
    variableName = newVariableName;
}

bool size_tValue::set_storedSize_tValue(size_t newSize_tValue)
{
    bool success = true;

    storedSize_tValue = newSize_tValue;

    return success;
}

bool size_tValue::set_storedSize_tValue_string(std::string newStringSize_tValue)
{
    bool conversionSuccess = false;

    size_t currentValue = strToSize_t(newStringSize_tValue,conversionSuccess);
    if(conversionSuccess == false)
    {
        printf("couldn't convert string \"%s\" to size_t!\n",newStringSize_tValue.c_str());
    } else
    {
        set_storedSize_tValue(currentValue);
    }

    return conversionSuccess;
}
/*** end set value functions ***/

/*** get value functions ***/
std::string size_tValue::get_variableName()
{
    return variableName;
}

size_t size_tValue::get_storedSize_tValue()
{
    return storedSize_tValue;
}
/*** end get value functions ***/



/***** private functions *****/



/*** type conversion functions ***/
size_t size_tValue::strToSize_t(std::string inputString, bool &conversionSuccess)
{
    size_t outputValue = 0;
    conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    if(isSize_t(inputString) == true)
    {
        outputValue = atoi(inputString.c_str());
    } else
    {
        conversionSuccess = false;
        printf("inputString \"%s\" is not a size_t value!\n",inputString.c_str());
    }

    return outputValue;
}
/*** end type conversion functions ***/

/*** typechecker functions ***/
bool size_tValue::isSize_t(std::string inputString)
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
            conversionSuccess = false; // only numbers allowed at this point
            break;
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
