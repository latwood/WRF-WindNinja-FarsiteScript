#include "polygonIgnitShapeFileStorage.h"

/***** public functions *****/

/*** constructor functions ***/
polygonIgnitShapeFileStorage::polygonIgnitShapeFileStorage()
{

}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void polygonIgnitShapeFileStorage::reset()
{
    while(!storedPolygonIgnitShapeFiles.empty())
    {
        storedPolygonIgnitShapeFiles.pop_back();
    }
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool polygonIgnitShapeFileStorage::add_polygonIgnitShapeFile(std::string new_polygon_ignit_shape_file)
{
    printf("adding polygon_ignit_shape_files \"%s\" to polygon_ignit_shape_files variable\n",new_polygon_ignit_shape_file.c_str());
    shapeFileValue new_shapeFileValue("");
    new_shapeFileValue.set_storedShapeFileValue(new_polygon_ignit_shape_file);
    storedPolygonIgnitShapeFiles.push_back(new_shapeFileValue);

    // if it reaches here, it worked correctly
    return true;
}

bool polygonIgnitShapeFileStorage::add_polygonIgnitShapeFile_Line(std::string inputDataLine)
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
        printf("not enough values found in polygonIgnitShapeFiles!\n");
        success = false;
    } else  // attempt to store the values
    {
        shapeFileValue new_shapeFileValue("");
        if(new_shapeFileValue.set_storedShapeFileValue(foundValues[0]) == false)
        {
            printf("couldn't set shape filename \"%s\" to variable \"%s\"!\n",foundValues[0].c_str(),"polygonIgnitShapeFiles");
        } else
        {
            storedPolygonIgnitShapeFiles.push_back(new_shapeFileValue);
        }
    }

    return success;
}
/*** end set value functions ***/

/*** get value functions ***/
std::vector<shapeFileValue> polygonIgnitShapeFileStorage::get_storedPolygonIgnitShapeFiles()
{
    return storedPolygonIgnitShapeFiles;
}
/*** end get value functions ***/



/***** private functions *****/



/*** reconstructor like functions ***/
/*** end reconstructor like functions ***/
