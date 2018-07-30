#ifndef SCRIPTINPUTS_H
#define SCRIPTINPUTS_H

#include <iostream>

#include "scriptConfigVariables.h"

#define  input_Sep  ':'  /* used on the end of every switch                  */

/* define variable types */
#define input_WRF_FILENAMES 'WRF_FILENAMES'


/* define conflicting options */

/* define error messages */

class scriptInputs
{
public:
    scriptInputs();
    void explainInputReqs();
    bool loadScriptInputs();
    void get_variableByName(std::string theVariableName);

private:
    void reset();
    bool findVariableNames();    // probably need a file as input. Looks like we need a boolean for each input variable name for if it is found or no. Or I guess this is just returning, found variable or no
    bool findOptionConflicts();
    bool checkConflictingOptions();
    bool loadAllInputs();
    // now add bool functions for each necessary input loader function if needed.
    // turns out that findVariableCount function is pretty much what I had in mind with loadAllInputs
    // notice that isIntTypeCountVar can go away if one of the variableCountTypes is a count, so a count is an int, but a sign to load extra data if needed. Could add function to load inputs if is type count. Or heck, type count won't be used for anything, just use this on anything that is a single type, then this is used as well, but inside loading anything that has a type count
    bool findVariableCount(char variableCountType, bool isIntTypeCountVar); // if an integer, it is a count. If anything else, there is only one and it should be stored as a single value. Verify type and if filename verify it is a valid path (can it open?)
    // now should have a function for each type of variable that isn't a single type to be loaded

    scriptConfigVariables setupConfigVariables;
    std::vector<configVariable> configVariables;

};

#endif // SCRIPTINPUTS_H
