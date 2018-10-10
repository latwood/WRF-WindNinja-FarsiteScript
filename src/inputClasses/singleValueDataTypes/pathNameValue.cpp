#include "pathNameValue.h"

/***** public functions *****/

/*** constructor functions ***/
pathNameValue::pathNameValue(std::string newVariableName)
{
    variableName = newVariableName;
    storedPathNameValue = defaultPathNameValue;
}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void pathNameValue::reset()
{
    storedPathNameValue = defaultPathNameValue;
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool pathNameValue::set_storedPathNameValue(std::string newPathnameValue)
{
    bool conversionSuccess = isValidPathname(newPathnameValue);
    if(conversionSuccess == false)
    {
        printf("string value \"%s\" is not a valid pathname!\n",newPathnameValue.c_str());
    } else
    {
        storedPathNameValue = newPathnameValue;
    }

    return conversionSuccess;
}
/*** end set value functions ***/

/*** get value functions ***/
std::string pathNameValue::get_variableName()
{
    return variableName;
}

std::string pathNameValue::get_storedPathNameValue()
{
    return storedPathNameValue;
}
/*** end get value functions ***/



/***** private functions *****/



/*** typechecker functions ***/
bool pathNameValue::isValidPathname(std::string inputString)
{
    // found this solution here: https://stackoverflow.com/questions/18100097/portable-way-to-check-if-directory-exists-windows-linux-c
    bool conversionSuccess = true;

    struct stat info;
    if( stat( inputString.c_str(), &info ) != 0 )
    {
        //printf( "cannot access %s\n", inputString.c_str() );
        conversionSuccess = false;
    } else if( info.st_mode & S_IFDIR )  // S_ISDIR() doesn't exist on my windows
    {
        //printf( "%s is a directory\n", inputString.c_str() );
        conversionSuccess = true;
    } else
    {
        //printf( "%s is no directory\n", inputString.c_str() );
        conversionSuccess = false;
    }

    return conversionSuccess;
}
/*** end typechecker functions ***/
