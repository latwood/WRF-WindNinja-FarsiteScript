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



/*** reconstructor like functions ***/
/*** end reconstructor like functions ***/
