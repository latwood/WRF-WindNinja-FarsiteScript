
#include "isValidExecFile.h"

bool isValidExecutable(std::string executablePath)
{
    bool isValid = false;

    struct stat info;
    if( stat(executablePath.c_str(), &info) == 0 && info.st_mode & S_IXUSR)
    {
        isValid = true;
    }

    return isValid;
}
