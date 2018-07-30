#ifndef FARSITE_H
#define FARSITE_H


#include "createFarsiteInputs.h"


class farsite
{
public:
    farsite();
    bool runFarsite(createFarsiteInputs *farsiteInputs);
    bool createAdditionalFarsiteResults();
};

#endif // FARSITE_H
