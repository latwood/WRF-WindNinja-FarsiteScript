
#include <iostream>
#include <string.h>

#include "inputVariablesHandler.h"
#include "createIgnitions.h"
#include "wrfInterpretation.h"
#include "createFarsiteInputs.h"
#include "farsite.h"

using namespace std;

#define MAX_PATH 512

bool checkCommandLineInput(int argc, char*argv[], inputVariablesHandler *inputs)
{
    if(argc < 2)
    {
        printf("WRF-WindNinja-FarsiteScript expects one parameter!\n");
       inputs->explainInputReqs();
        exit(1);
    }
    FILE *cmd = fopen(argv[1], "rt");
    if(!cmd)
    {
        printf("Error, cannot open %s\n", argv[1]);
        inputs->explainInputReqs();
        exit(1);
    }
    fclose(cmd);
    return true;    // only other way to return is by exiting
}

int main(int argc, char* argv[])
{
    printf("beginning WRF-WindNinja-FarsiteScript\n");

    inputVariablesHandler inputs;
    createIgnitions ignitions;
    wrfInterpretation wrfInterp;
    createFarsiteInputs farsiteInputs;
    farsite farsite;

    char inputFilePath[MAX_PATH];

    if(checkCommandLineInput(argc,argv,&inputs))
    {
        strcpy(argv[1], inputFilePath);
        printf("using inputFilePath %s\n",inputFilePath);
    }

    if(!inputs.loadScriptInputs(inputFilePath))
    {
        printf("Error loading script inputs! Exiting program!\n");
        exit(1);
    }

    if(!ignitions.createAllIgnitions(&inputs))
    {
        printf("Error creating ignitions! Exiting program!\n");
        exit(1);
    }

    if(!wrfInterp.interpretWRFfiles(&inputs))
    {
        printf("Error running windninja or extracting weather information from wrf files! Exiting program!\n");
        exit(1);
    }

    if(!farsiteInputs.createAllFarsiteInputs(&inputs,&ignitions,&wrfInterp))
    {
        printf("Error creating farsite inputs! Exiting program!\n");
        exit(1);
    }

    if(!farsite.runFarsite(&farsiteInputs)) // maybe replace with load inputs? Or just go with it?
    {
        printf("Error running Farsite! Exiting program!\n");
        exit(1);
    }

    if(!farsite.createAdditionalFarsiteResults())
    {
        printf("Error generating extra farsite inputs! Exiting program!\n");
        exit(1);
    }

    printf("finished WRF-WindNinja-FarsiteScript\n");
    return 0;
}
