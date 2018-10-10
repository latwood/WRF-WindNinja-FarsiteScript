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
