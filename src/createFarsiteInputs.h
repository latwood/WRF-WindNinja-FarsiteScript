#ifndef CREATEFARSITEINPUTS_H
#define CREATEFARSITEINPUTS_H


#include "inputVariablesHandler.h"
#include "createIgnitions.h"
#include "wrfInterpretation.h"


class createFarsiteInputs
{
public:
    createFarsiteInputs();
    bool createAllFarsiteInputs(inputVariablesHandler *inputs, createIgnitions *ignitions, wrfInterpretation *wrfInterp);

private:

};

#endif // CREATEFARSITEINPUTS_H
