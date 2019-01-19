
#include "doesFilenameExist.h"

bool doesFileExist(std::string fileName)
{
    // found this here: https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
    // hopefully it works for files as well as directories as this is the same thing used to check a file path. More interesting hopefully this works for .prj files!
    struct stat buffer;
    return (stat (fileName.c_str(), &buffer) == 0);
}
