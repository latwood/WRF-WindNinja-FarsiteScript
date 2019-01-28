
#include <iostream>

#include "inputClasses/inputVariablesHandler.h"
#include "lcpDownloader.h"
#include "createIgnitions.h"
#include "WindNinjaAPI.h"
#include "wrfGetWeather.h"
#include "farsiteAPI.h"

#include "getWindNinjaPath.h"
#include "getFarsitePath.h"

#include "utilityClasses/calcTime.h"
#include "utilityClasses/createFolder.h"
#include "utilityClasses/isValidExecFile.h"

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
        printf("Error, cannot open \"%s\"\n\n", argv[1]);
        inputs->explainInputReqs();
        exit(1);
    }
    fclose(cmd);
    return true;    // only other way to return is by exiting
}

int main(int argc, char* argv[])
{
    printf("\n\nbeginning WRF-WindNinja-FarsiteScript\n\n");
    calcTime timers;
    timers.startNewTime("WRF-WindNinja-FarsiteScript total runTime"); // start recording execution time

    inputVariablesHandler inputs;
    lcpDownloader createLcp;
    createIgnitions ignitions;
    WindNinjaAPI windAPI;
    wrfGetWeather weatherAPI;
    farsiteAPI fireAPI;

    std::string inputFilePath;

    printf("\nchecking WRF-WindNinja-FarsiteScript command line inputs\n");
    if(checkCommandLineInput(argc,argv,&inputs) == true)
    {
        inputFilePath = argv[1];
        printf("using inputFilePath %s\n",inputFilePath.c_str());
    }

    printf("\nchecking WindNinja path\n");
    std::string WindNinjaPath = getWindNinjaPath();
    if(isValidExecutable(WindNinjaPath) == false)
    {
        printf("Invalid WindNinja path %s! buildEverything.sh should have set this up at compile time! Exiting Program!\n",WindNinjaPath.c_str());
        exit(1);
    }

    printf("\nchecking farsite path\n");
    std::string farsitePath = getFarsitePath();
    if(isValidExecutable(farsitePath) == false)
    {
        printf("Invalid farsite path %s! buildEverything.sh should have set this up at compile time! Exiting Program!\n",farsitePath.c_str());
        exit(1);
    }

    timers.startNewTime("loadScriptInputs total runTime");
    printf("\nloading WRF-WinNinja-FarsiteScript inputs into input class\n");
    if(inputs.loadScriptInputs(inputFilePath) == false)
    {
        printf("Error loading script inputs! Exiting program!\n");
        exit(1);
    }
    timers.getAndPrint_time("loadScriptInputs total runTime");   // print out elapsed execution time

    // okay now create the necessary overall folders to be used by all the other classes, as they add more folders to these folders

    printf("\ncreating main output folders\n");
    // create main output folders if needed. overwrite outputs will already be checked to get the right paths
    if(createFolder(inputs.get_actualCreateInputs_path()) == false)
    {
        printf("failed to create createInputs folder!\n");
        exit(1);
    }
    if(createFolder(inputs.get_actualFinalOutput_path()) == false)
    {
        printf("failed to create finalOutput folder!\n");
        exit(1);
    }

    timers.startNewTime("createIgnitions and lcpDownload runTime");
    printf("\nloading inputs into createIgnition class\n");
    // okay do what is needed for lcp downloading stuff, which possibly includes some ignition stuff, so load inputs first
    if(ignitions.load_required_inputs(&inputs) == false)
    {
        printf("failed to load createIgnition inputs!\n");
        exit(1);
    }

    printf("\nfinding largest fire perimeter from ignition files\n");
    // turns out that we need the fire perimeter lat long positions no matter what anyways, so let's just get that done first
    if(ignitions.findLargestFirePerimeter() == false)
    {
        printf("failed to find largest fire perimeter for automatic lcp download!\n");
        exit(1);
    }

    printf("\ndetermining whether lcp file needs downloaded\n");
    // probably need to load inputs into lcpManager and createIgnition classes before this
    if(inputs.get_boolValue("use_past_lcp").get_storedBoolValue() != true)
    {
        printf("\nloading inputs into lcpDownloader class\n");
        // okay need to download lcp so first load inputs
        if(createLcp.load_required_inputs(&inputs, ignitions.get_firePerimMinLat(), ignitions.get_firePerimMinLong(), ignitions.get_firePerimMaxLat(), ignitions.get_firePerimMaxLong()) == false)
        {
            printf("failed to load inputs into lcpDownloader class!\n");
            exit(1);
        }

        printf("\nrunning lcpDownloader!\n");
        // now attempt to download lcp. lcpDownloader class determines whether to use a specified boundary or an automated process with the largest fire perimeters
        if(createLcp.downloadLcp() == false)
        {
            printf("failed to download lcp file!\n");
            exit(1);
        }
    }

    printf("\nrunning createIgnitions!\n");
    if(ignitions.createAllIgnitions() == false)
    {
        printf("Error creating ignitions! Exiting program!\n");
        exit(1);
    }
    timers.getAndPrint_time("createIgnitions and lcpDownload runTime");

    timers.startNewTime("WindNinja total runTime");
    printf("\nloading inputs into WindNinjaAPI\n");
    // load inputs into WindNinjaApi class
    if(windAPI.load_required_inputs(&inputs, WindNinjaPath) == false)
    {
        printf("failed to load inputs into WindNinjaAPI class!\n");
        exit(1);
    }

    printf("\ncreating WindNinja cfg files\n");
    // now create WindNinja cfg files
    if(windAPI.create_WindNinja_cfg_files() == false)
    {
        printf("failed to create all WindNinja cfg files!\n");
        exit(1);
    }

    printf("\nrunning WindNinja!\n");
    // now run WindNinja
    if(windAPI.run_WindNinja() == false)
    {
        printf("problems running WindNinja!\n");
        exit(1);
    }

    printf("\nfinding WindNinja final run files\n");
    if(windAPI.findFinalRunFiles() == false)
    {
        printf("problems finding WindNinja output run files for processing!\n");
        exit(1);
    }

    printf("\nfinding wrf file times from WindNinja logs\n");
    if(windAPI.findWrfTimes() == false)
    {
        printf("problems finding wrf file times from WindNinja logs!\n");
        exit(1);
    }
    timers.getAndPrint_time("WindNinja total runTime");

    timers.startNewTime("wrfGetWeather runTime");
    printf("\nloading inputs into wrfGetWeather class\n");
    // first load in the inputs
    if(weatherAPI.load_required_inputs(&inputs) == false)
    {
        printf("failed to load inputs into wrfGetWeather class!\n");
        exit(1);
    }

    printf("\nrunning wrfGetWeather!\n");
    // now run getWeather to get weather data from all netcdf files
    if(weatherAPI.getWeather() == false)
    {
        printf("problems running wrfGetWeather!\n");
        exit(1);
    }
    timers.getAndPrint_time("wrfGetWeather runTime");

    timers.startNewTime("farsite total runTime");
    printf("\nloading inputs into farsiteAPI class\n");
    if(fireAPI.load_required_inputs(&inputs,&ignitions,&windAPI,&weatherAPI, farsitePath) == false)
    {
        printf("Error creating farsite inputs! Exiting program!\n");
        exit(1);
    }

    printf("\ncreating farsite input files\n");
    // okay finished getting everything ready, now start farsite stuff
    if(fireAPI.createAllFarsiteInputs() == false)
    {
        printf("Error creating farsite inputs! Exiting program!\n");
        exit(1);
    }

    printf("\nrunning farsite!\n");
    if(fireAPI.runFarsite() == false) // maybe replace with load inputs? Or just go with it?
    {
        printf("Error running Farsite! Exiting program!\n");
        exit(1);
    }
    timers.getAndPrint_time("farsite total runTime");

    timers.startNewTime("processFarsiteResults runTime");
    printf("\nprocessing output farsite files for additional results\n");
    if(fireAPI.createAdditionalFarsiteResults() == false)
    {
        printf("Error generating extra farsite inputs! Exiting program!\n");
        exit(1);
    }
    timers.getAndPrint_time("processFarsiteResults runTime");

    printf("\n\nfinished WRF-WindNinja-FarsiteScript\n");

    timers.getAndPrint_time("WRF-WindNinja-FarsiteScript total runTime");   // print out elapsed execution time

    return 0;
}
