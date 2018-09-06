#include "shapeFileValue.h"

/***** public functions *****/

/*** constructor functions ***/
shapeFileValue::shapeFileValue(std::string newVariableName)
{
    variableName = newVariableName;
    storedShapeFileValue = defaultShapeFileValue;
}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void shapeFileValue::reset()
{
    storedShapeFileValue = defaultShapeFileValue;
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool shapeFileValue::set_storedShapeFileValue(std::string newFilenameValue)
{
    bool success = true;

   storedShapeFileValue = newFilenameValue;

   return success;
}

/*** end set value functions ***/

/*** get value functions ***/
std::string shapeFileValue::get_variableName()
{
    return variableName;
}

std::string shapeFileValue::get_storedShapeFileValue()
{
    return storedShapeFileValue;
}
/*** end get value functions ***/



/***** private functions *****/



/*** reconstructor like functions ***/
/*** end reconstructor like functions ***/
