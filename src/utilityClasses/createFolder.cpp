
#include "createFolder.h"

bool createFolder(std::string folderName)
{
    bool folderExists = doesDirExist(folderName);
    if(folderExists == false)
    {
        //force temp dir to DEM location (is this even necessary?)
        CPLSetConfigOption("CPL_TMPDIR", CPLGetDirname(folderName.c_str()));
        CPLSetConfigOption("CPLTMPDIR", CPLGetDirname(folderName.c_str()));
        CPLSetConfigOption("TEMP", CPLGetDirname(folderName.c_str()));
        // now make the directory
        VSIMkdir( folderName.c_str(), 0777 );

        // make sure it worked, return bool whether it did or not
        folderExists = doesDirExist(folderName);
    }

    return folderExists;
}
