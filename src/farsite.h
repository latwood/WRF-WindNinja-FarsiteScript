#ifndef FARSITE_H
#define FARSITE_H


#include "createFarsiteInputs.h"


class farsite
{

public:

    // constructors
    farsite();

    // functions
    bool runFarsite(createFarsiteInputs *farsiteInputs);
    bool createAdditionalFarsiteResults();

private:


};

#endif // FARSITE_H
