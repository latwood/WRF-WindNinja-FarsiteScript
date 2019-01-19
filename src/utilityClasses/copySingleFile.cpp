
#include "copySingleFile.h"

// found this function from: https://stackoverflow.com/questions/10195343/copy-a-file-in-a-sane-safe-and-efficient-way
bool copyFile(std::string inputFilename, std::string outputFilename)
{
    std::ifstream  src(inputFilename.c_str(), std::ios::binary);
    std::ofstream  dst(outputFilename.c_str(),   std::ios::binary);
    dst << src.rdbuf();

    // if it gets here, everything went well
    return true;
}
