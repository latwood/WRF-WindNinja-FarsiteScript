
#include <iostream>

#include "inputVariablesHandler.h"
#include "createIgnitions.h"
#include "wrfInterpretation.h"
#include "createFarsiteInputs.h"
#include "farsite.h"

#include <chrono>

using namespace std;

#define MAX_PATH 512

bool checkCommandLineInput(int argc, char*argv[], inputVariablesHandler *inputs)
{
    if(argc != 2)
    {
        printf("WRF-WindNinja-FarsiteScript expects one parameter!\n\n");
        inputs->explainInputReqs();
        exit(1);
    }
    FILE *cmd = fopen(argv[1], "rt");
    if(!cmd)
    {
        printf("Error, cannot open %s\n\n", argv[1]);
        inputs->explainInputReqs();
        exit(1);
    }
    fclose(cmd);
    return true;    // only other way to return is by exiting
}

bool doesFolderExist(std::string pathName)
{
    bool exists = true;

    struct stat info;
    if( stat( pathName.c_str(), &info ) != 0 )
    {
        //printf( "cannot access %s\n", inputString.c_str() );
        exists = false;
    } else if( info.st_mode & S_IFDIR )  // S_ISDIR() doesn't exist on my windows
    {
        //printf( "%s is a directory\n", inputString.c_str() );
        exists = true;
    } else
    {
        //printf( "%s is no directory\n", inputString.c_str() );
        exists = false;
    }

    return exists;
}

int main(int argc, char* argv[])
{
    printf("\nbeginning WRF-WindNinja-FarsiteScript\n\n");
    auto start = std::chrono::high_resolution_clock::now(); // start recording execution time

    inputVariablesHandler inputs;
    createIgnitions ignitions;
    wrfInterpretation wrfInterp;
    createFarsiteInputs farsiteInputs;
    farsite farsite;

    std::string inputFilePath;

    if(checkCommandLineInput(argc,argv,&inputs))
    {
        inputFilePath = argv[1];
        printf("using inputFilePath %s\n",inputFilePath.c_str());
    }

    if(!inputs.loadScriptInputs(inputFilePath))
    {
        printf("Error loading script inputs! Exiting program!\n");
        exit(1);
    }

    // create main output folders if needed. overwrite outputs will already be checked to get the right paths
    if(doesFolderExist(inputs.get_actualCreateInputs_path()) == false)
    {
            //force temp dir to DEM location (is this even necessary?)
        CPLSetConfigOption("CPL_TMPDIR", CPLGetDirname(inputs.get_actualCreateInputs_path().c_str()));
        CPLSetConfigOption("CPLTMPDIR", CPLGetDirname(inputs.get_actualCreateInputs_path().c_str()));
        CPLSetConfigOption("TEMP", CPLGetDirname(inputs.get_actualCreateInputs_path().c_str()));
            // now make the directory
        VSIMkdir( inputs.get_actualCreateInputs_path().c_str(), 0777 );
    }
    if(doesFolderExist(inputs.get_actualFinalOutput_path()) == false)
    {
            //force temp dir to DEM location (is this even necessary?)
        CPLSetConfigOption("CPL_TMPDIR", CPLGetDirname(inputs.get_actualFinalOutput_path().c_str()));
        CPLSetConfigOption("CPLTMPDIR", CPLGetDirname(inputs.get_actualFinalOutput_path().c_str()));
        CPLSetConfigOption("TEMP", CPLGetDirname(inputs.get_actualFinalOutput_path().c_str()));
            // now make the directory
        VSIMkdir( inputs.get_actualFinalOutput_path().c_str(), 0777 );
    }

    // probably need to load inputs into lcpManager and createIgnition classes before this
    if(inputs.get_inputVariableBoolValue("use_past_lcp") == true)
    {
        // one way process lcp first, then ignitions
    } else
    {
        // need to process something in ignitions area first, then process lcp (download it), then ignitions
    }

    if(!ignitions.createAllIgnitions(&inputs))
    {
        printf("Error creating ignitions! Exiting program!\n");
        exit(1);
    }

    // maybe move all this out to here, cause it is pretty repetitive. If could do something similar to this with other applications, fine, but turns out that doesn't occur
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

    auto finish = std::chrono::high_resolution_clock::now();  // finish recording execution time
    std::chrono::duration<double> elapsed = finish - start;
    printf("Elapsed time: %f seconds\n",elapsed.count());   // print out elapsed execution time

    printf("\nfinished WRF-WindNinja-FarsiteScript\n\n");
    return 0;
}
