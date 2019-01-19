#ifndef ISVALIDEXECFILE_H
#define ISVALIDEXECFILE_H

#include <iostream>
#include <sys/stat.h>

// this is just a temporary fix.
// Almost need to try the script with simple
// inputs as a test run to see for sure if it is a
// valid application executable
bool isValidExecutable(std::string executablePath);

#endif // ISVALIDEXECFILE
