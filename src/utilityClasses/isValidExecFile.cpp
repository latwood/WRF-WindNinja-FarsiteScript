
#include "isValidExecFile.h"

bool isValidExecutable(std::string executablePath)
{
    bool isValid = false;

    struct stat info;
    if( stat(executablePath.c_str(), &info) == 0 && info.st_mode & S_IXUSR)
    {
        isValid = true;
    }

    // alternative idea that I had in farsiteAPI instead of main
    /*
    // found this function here: https://stackoverflow.com/questions/5719694/having-a-path-to-file-how-to-check-if-it-is-executable
    // so it could use some extra stuff to test the owner and group and if you have Secure Linux then the security settings which stat does not give you
    struct stat  st;
    if (stat(file, &st) < 0)
        return false;
    if ((st.st_mode & S_IEXEC) != 0)
        return true;
    return false;
    */

    return isValid;
}
