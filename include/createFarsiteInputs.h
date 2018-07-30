#ifndef CREATEFARSITEINPUTS_H
#define CREATEFARSITEINPUTS_H


#include "scriptInputs.h"
#include "createIgnitions.h"
#include "wrfInterpretation.h"


class createFarsiteInputs
{
public:
    createFarsiteInputs();
    bool createAllFarsiteInputs(scriptInputs *inputs, createIgnitions *ignitions, wrfInterpretation *wrfInterp);

private:

};

#endif // CREATEFARSITEINPUTS_H
