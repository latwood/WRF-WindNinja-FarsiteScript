#include "GeoMACfirePerimeterFileStorage.h"

/***** public functions *****/

/*** constructor functions ***/
GeoMACfirePerimeterFileStorage::GeoMACfirePerimeterFileStorage()
{

}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void GeoMACfirePerimeterFileStorage::reset()
{
    while(!storedGeoMACfirePerimeterFiles.empty())
    {
        storedGeoMACfirePerimeterFiles.pop_back();
    }
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool GeoMACfirePerimeterFileStorage::add_GeoMACfirePerimeterFile(std::string new_GeoMAC_fire_perimeter_file)
{
    printf("adding GeoMAC_fire_perimeter_files \"%s\" to GeoMAC_fire_perimeter_files variable\n",new_GeoMAC_fire_perimeter_file.c_str());
    shapeFileValue new_shapeFileValue("");
    new_shapeFileValue.set_storedShapeFileValue(new_GeoMAC_fire_perimeter_file);
    storedGeoMACfirePerimeterFiles.push_back(new_shapeFileValue);

    // if it reaches here, it worked correctly
    return true;
}

bool GeoMACfirePerimeterFileStorage::add_GeoMACfirePerimeterFile_Line(std::string inputDataLine)
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
    if(foundValues.size() != 1)
    {
        printf("not enough values found in GeoMACfirePerimeterFiles!\n");
        success = false;
    } else  // attempt to store the values
    {
        shapeFileValue new_shapeFileValue("");
        if(new_shapeFileValue.set_storedShapeFileValue(foundValues[0]) == false)
        {
            printf("couldn't set shape filename \"%s\" to variable \"%s\"!\n",foundValues[0].c_str(),"GeoMACfirePerimeterFiles");
        } else
        {
            storedGeoMACfirePerimeterFiles.push_back(new_shapeFileValue);
        }
    }

    return success;
}
/*** end set value functions ***/

/*** get value functions ***/
std::vector<shapeFileValue> GeoMACfirePerimeterFileStorage::get_storedGeoMACfirePerimeterFiles()
{
    return storedGeoMACfirePerimeterFiles;
}
/*** end get value functions ***/



/***** private functions *****/



/*** reconstructor like functions ***/
/*** end reconstructor like functions ***/
