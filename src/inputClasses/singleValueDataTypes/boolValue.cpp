#include "boolValue.h"

/***** public functions *****/

/*** constructor functions ***/
boolValue::boolValue(std::string newVariableName)
{
    variableName = newVariableName;
    storedBoolValue = defaultBoolValue;
}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void boolValue::reset()
{
    storedBoolValue = defaultBoolValue;
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
void boolValue::set_storedBoolValue(bool newBoolValue)
{
    storedBoolValue = newBoolValue;
}

bool boolValue::set_storedBoolValue_string(std::string newStringBoolValue)
{
    bool conversionSuccess = false;

    bool currentValue = strToBool(newStringBoolValue,conversionSuccess);
    if(conversionSuccess == false)
    {
        printf("couldn't convert string \"%s\" to bool!\n",newStringBoolValue.c_str());
    } else
    {
        set_storedBoolValue(currentValue);
    }

    return conversionSuccess;
}
/*** end set value functions ***/

/*** get value functions ***/
std::string boolValue::get_variableName()
{
    return variableName;
}

bool boolValue::get_storedBoolValue()
{
    return storedBoolValue;
}
/*** end get value functions ***/



/***** private functions *****/



/*** type conversion functions ***/
bool boolValue::strToBool(std::string inputString, bool &conversionSuccess)
{
    bool outputValue = false;
    conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    if(isBool(inputString) == true)
    {
        if(inputString == "true" || inputString == "1")
        {
            outputValue = true;
        } else if(inputString == "false" || inputString == "0")
        {
            outputValue = false;
        }
    } else
    {
        conversionSuccess = false;
        printf("inputString \"%s\" is not a bool value!\n",inputString.c_str());
    }

    return outputValue;
}
/*** end type conversion functions ***/

/*** typechecker functions ***/
bool boolValue::isBool(std::string inputString)
{
    bool conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    if(inputString == "true" || inputString == "1")
    {
        conversionSuccess = true;
    } else if(inputString == "false" || inputString == "0")
    {
        conversionSuccess = true;
    } else
    {
        conversionSuccess = false;
    }

    return conversionSuccess;
}
/*** end typechecker functions ***/
