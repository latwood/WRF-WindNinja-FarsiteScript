#include "scriptInputs.h"

scriptInputs::scriptInputs()
{
    configVariables = setupConfigVariables.get_theVariables();

    /* define variable names (might be done as define statements) */
    /* define conflicting options */
    /* define error messages */
}

void scriptInputs::explainInputReqs()
{
    printf("WRF-WindNinja-FarsiteScript Usage:\n"
           "WRF-WindNinja-FarsiteScript [inputfile]\n"
           "Where:\n\t[commandfile] is the path to the input file.\n");
    printf("The input file can contain the following variables:\n");
    printf("\n%s input variables\n",configVariables[0].get_applicationUseName().c_str());
    for(size_t variableIdx = 0; variableIdx < configVariables.size(); variableIdx++)
    {
        if(variableIdx != 0 && configVariables[variableIdx].get_applicationUseName() != configVariables[variableIdx-1].get_applicationUseName())
        {
            printf("\n%s input variables\n",configVariables[variableIdx].get_applicationUseName().c_str());
        }
        // need to add a description overall vartype to detect when to split up sections between application type variables
        std::vector<unsigned int> currentLineBreaks = configVariables[variableIdx].get_variableDescriptionLineBreaks();
        printf("%s%s%s\n",configVariables[variableIdx].get_variableName().c_str(),configVariables[variableIdx].get_variableNameWhiteSpace().c_str(),configVariables[variableIdx].get_variableDescription().substr(0,currentLineBreaks[1]).c_str());
        for(size_t lineBreakIdx = 1; lineBreakIdx < currentLineBreaks.size()-1; lineBreakIdx++)
        {
            printf("%s%s\n",setupConfigVariables.get_maxVarNameColumnWhitespace().c_str(),configVariables[variableIdx].get_variableDescription().substr(currentLineBreaks[lineBreakIdx],currentLineBreaks[lineBreakIdx+1]-currentLineBreaks[lineBreakIdx]).c_str());
        }
        printf("\n");
    }
    printf("end description\n");
}

bool scriptInputs::loadScriptInputs()
{
    bool success = true;
    /* clean up all data that isn't just defined once for all uses */
    reset();

    // open file for use in everything


    //load all variable names
    if(findVariableNames() == false)
    {
        printf("some variable names not found!\n");
        success = false;
    }
    //check conflicting and stop and warn for all conflicts
    if(findOptionConflicts() == false)
    {
        printf("found conflicting options!\n");
        success = false;
    }

    //if no conflicting options, load each input, error if any cannot be loaded correctly
    if(success == true)
    {
        if(loadAllInputs() == false)
        {
            printf("problem loading inputs!\n");
            success = false;
        }
    }

    // now should close the file


    return success;
}

void scriptInputs::reset()
{
    for(size_t variableIdx = 0; variableIdx < configVariables.size(); variableIdx++)
    {
        configVariables[variableIdx].resetVariable();
    }
    // other cleanup stuff for another round, this function is basically a constructor/destructor for anytime this thing is reused :)
}

bool scriptInputs::findVariableNames()
{
    bool success = true;

    // need to adjust to have a file for input
    for(size_t variableIdx = 0; variableIdx < configVariables.size(); variableIdx++)
    {
        // search in file for each variable name, just the name not the count
        // set variable in class to resulting bool of findVariable();
    }

    return success;
}

bool scriptInputs::findOptionConflicts()
{
    bool success = true;

    // go through each variable, then through each conflicting option for each variable, find variable with name matching each conflicting option and verify bool for said variable for if it is found in input is not true
    for(size_t variableIdx = 0; variableIdx < configVariables.size(); variableIdx++)
    {

    }

    return success;
}

bool scriptInputs::loadAllInputs()
{
    bool success = true;

    for(size_t variableIdx = 0; variableIdx < configVariables.size(); variableIdx++)
    {
        // big if statement checking variableCountType and using the proper load function.
        //  If type is count, which comes last, then another big if statement using loader function names to determine which loader function to use.
        //   If it reaches the else, no loader function is used so give a warning to the user/programmer that it is an allowed/required input, but it is not really loaded or used anymore. So any format would go, just need to trick the input loader.
        // hm, I just realized, I need to add something to make sure if the type is not count, that the loader function is NA. Also, since variable values are not stored in configVariables, how to tell which variable to stuff the value into when the count shows up?
        //  I guess just add back in some kind of single value to the variables class, but if it has type count so it has a loader function (can't be NA), the values are stored separately, which is determined by the loader function directly phew.
        // Great :( now I have to add more checks and stuff to the variable classes to protect their use.
        // hm, with this in mind, it is like building a separate struct for all these single variables, just that the class manages the string names to the variable values. So whatever class uses this class, will need to be careful to use the option stuff to get the value? No, this class will have a function get_variableByName() that uses the input string to find a variable and grab the value from where it is needed accordingly.
        //  hm, but that function has to be of the type of variable that is getting sent out! So maybe it is getSingleVariableByName(), split up into different needed types of single values so it can verify that the desired value is of the type desired before it breaks.
        //  then for all the loaded function stuff, there can be separate functions get_ according to the specific data stored :)
    }

    return success;
}
