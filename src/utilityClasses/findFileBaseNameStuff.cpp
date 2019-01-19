
#include "findFileBaseNameStuff.h"

std::string findFileBaseNameAndExtension(std::string filePath)
{
    std::string fileName = "";

    for(size_t charIdx = filePath.length()-1; charIdx > 0; charIdx--)
    {
        std::string currentChr = filePath.substr(charIdx,1);
        if(currentChr == "/")
        {
            fileName = filePath.substr(charIdx+1,filePath.length());
            break;
        }
        if(charIdx == 1)    // was no extra path on it
        {
            fileName = filePath;
        }
    }

    return fileName;
}

std::string findBaseName(std::string filePath)
{
    // first pull off the path part
    std::string fileBaseName = findFileBaseNameAndExtension(filePath);

    // now pull of the extension
    for(size_t charIdx = fileBaseName.length()-1; charIdx > 0; charIdx--)
    {
        std::string currentChr = fileBaseName.substr(charIdx,1);
        if(currentChr == ".")
        {
            fileBaseName = fileBaseName.substr(0,charIdx);
            break;
        }
    }

    return fileBaseName;
}

std::string findBaseNameWithPath(std::string filePath)
{
    // first pull off the path part
    std::string fileBaseName = "";

    // now pull of the extension
    for(size_t charIdx = filePath.length()-1; charIdx > 0; charIdx--)
    {
        std::string currentChr = filePath.substr(charIdx,1);
        if(currentChr == ".")
        {
            fileBaseName = filePath.substr(0,charIdx);
            break;
        }
        if(charIdx == 1)    // was no extension on it
        {
            fileBaseName = filePath;
        }
    }

    return fileBaseName;
}
