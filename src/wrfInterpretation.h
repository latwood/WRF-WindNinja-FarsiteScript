#ifndef WRFINTERPRETATION_H
#define WRFINTERPRETATION_H


#include "inputVariablesHandler.h"

class wrfInterpretation
{
public:
    wrfInterpretation();
    bool interpretWRFfiles(inputVariablesHandler *inputs);
private:
    bool runWindNinja();
    bool getWeatherInfo();
};

#endif // WRFINTERPRETATION_H
