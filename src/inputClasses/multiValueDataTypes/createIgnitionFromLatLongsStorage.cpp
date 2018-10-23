#include "createIgnitionFromLatLongsStorage.h"

/***** public functions *****/

/*** constructor functions ***/
createIgnitionFromLatLongsStorage::createIgnitionFromLatLongsStorage()
{

}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void createIgnitionFromLatLongsStorage::reset()
{
    while(!storedCreateIgnitionLatLongValues.empty())
    {
        storedCreateIgnitionLatLongValues.pop_back();
    }
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool createIgnitionFromLatLongsStorage::add_createIgnitionLatLongValue(double newLat_CoordValue, double newLong_CoordValue)
{
    printf("adding create_ignition_from_latlongs \"%f %f\" (lat long) to create_ignition_from_latlongs variable\n",newLat_CoordValue,newLong_CoordValue);
    lat_longValue new_lat_longValue("");
    new_lat_longValue.set_storedLatLongValue(newLat_CoordValue,newLong_CoordValue);
    storedCreateIgnitionLatLongValues.push_back(new_lat_longValue);

    // if it reaches here, it worked correctly
    return true;
}

bool createIgnitionFromLatLongsStorage::add_createIgnitionLatLongValue_Line(std::string inputDataLine)
{
    bool success = true;

    // first parse through the line to separate all the values as strings
    bool isValue = true;
    std::vector<std::string> foundValues;
    size_t startValueSpot = 0;
    for(size_t charIdx = 0; charIdx < inputDataLine.length(); charIdx++)
    {
        std::string currentChr = inputDataLine.substr(charIdx,1);
        if(currentChr == " " && isValue == true)
        {
            foundValues.push_back(inputDataLine.substr(startValueSpot,charIdx-startValueSpot));
            isValue = false;
        }
        if(currentChr != " " && isValue == false)
        {
            startValueSpot = charIdx;
            isValue = true;
        }
        if(charIdx == inputDataLine.length()-1 && isValue == true)
        {
            foundValues.push_back(inputDataLine.substr(startValueSpot,charIdx-startValueSpot+1));
        }
    }

    /*printf("found values are:");
    for(size_t valIdx = 0; valIdx < foundValues.size(); valIdx++)
    {
        printf(" \"%s\"",foundValues[valIdx].c_str());
    }
    printf("\n\n");*/

    // found the right number of values?
    if(foundValues.size() != 2)
    {
        printf("not enough values found in createIgnitionLatLongValues!\n");
        success = false;
    } else  // attempt to store the values
    {
        lat_longValue new_lat_longValue("");
        if(new_lat_longValue.set_storedLatLongValue_string(inputDataLine) == false)
        {
            printf("couldn't set lat_long_point value \"%s\" \"(lat long)\" to variable \"%s\"!\n",inputDataLine.c_str(),"createIgnitionLatLongValues");
            success = false;
        } else
        {
            storedCreateIgnitionLatLongValues.push_back(new_lat_longValue);
        }
    }

    return success;
}
/*** end set value functions ***/

/*** get value functions ***/
std::vector<lat_longValue> createIgnitionFromLatLongsStorage::get_storedCreateIgnitionLatLongValues()
{
    return storedCreateIgnitionLatLongValues;
}
/*** end get value functions ***/



/***** private functions *****/



/*** reconstructor like functions ***/
/*** end reconstructor like functions ***/
