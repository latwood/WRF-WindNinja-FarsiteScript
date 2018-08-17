#ifndef CREATEFARSITEINPUTS_H
#define CREATEFARSITEINPUTS_H


#include "inputVariablesHandler.h"
#include "createIgnitions.h"
#include "WindNinjaAPI.h"
#include "wrfGetWeather.h"


class createFarsiteInputs
{

public:

    // constructors
    createFarsiteInputs();

    // functions
    bool createAllFarsiteInputs(inputVariablesHandler *inputs, createIgnitions *ignitions, WindNinjaAPI *windAPI, wrfGetWeather *weatherAPI);

private:

};

#endif // CREATEFARSITEINPUTS_H
