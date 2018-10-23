#include "wrfFileStorage.h"

/***** public functions *****/

/*** constructor functions ***/
wrfFileStorage::wrfFileStorage()
{

}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void wrfFileStorage::reset()
{
    while(!storedWrfFiles.empty())
    {
        storedWrfFiles.pop_back();
    }
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool wrfFileStorage::add_wrfFile(std::string new_wrf_file)
{
    // might want to check to see if file is a legit file? Here or somewhere else? Format checking?
    // I guess the assumption right now is that the parser handles all the format checking and if it is a legit file, just calls storage functions from here
    printf("adding wrf_file \"%s\" to wrf_files variable\n",new_wrf_file.c_str());
    wrfFileValue new_wrfFileValue("");
    new_wrfFileValue.set_storedWrfFileName(new_wrf_file);
    storedWrfFiles.push_back(new_wrfFileValue);

    // if it reaches here, it worked correctly
    return true;
}

bool wrfFileStorage::add_wrfFile_Line(std::string inputDataLine)
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
        printf("not enough values found in wrfFiles!\n");
        success = false;
    } else  // attempt to store the values
    {
        wrfFileValue new_wrfFileValue("");
        if(new_wrfFileValue.set_storedWrfFileName(foundValues[0]) == false)
        {
            printf("couldn't set wrf filename \"%s\" to variable \"%s\"!\n",foundValues[0].c_str(),"wrfFiles");
        } else
        {
            storedWrfFiles.push_back(new_wrfFileValue);
        }
    }

    return success;
}
/*** end set value functions ***/

/*** get value functions ***/
std::vector<wrfFileValue> wrfFileStorage::get_storedWrfFiles()
{
    return storedWrfFiles;
}
/*** end get value functions ***/



/***** private functions *****/



/*** reconstructor like functions ***/
/*** end reconstructor like functions ***/
