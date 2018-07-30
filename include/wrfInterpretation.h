#ifndef WRFINTERPRETATION_H
#define WRFINTERPRETATION_H


#include "scriptInputs.h"

class wrfInterpretation
{
public:
    wrfInterpretation();
    bool interpretWRFfiles(scriptInputs *inputs);
private:
    bool runWindNinja();
    bool getWeatherInfo();
};

#endif // WRFINTERPRETATION_H
