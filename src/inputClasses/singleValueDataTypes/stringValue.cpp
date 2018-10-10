#include "stringValue.h"

/***** public functions *****/

/*** constructor functions ***/
stringValue::stringValue(std::string newVariableName)
{
    variableName = newVariableName;
    storedStringValue = defaultStringValue;
}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void stringValue::reset()
{
    storedStringValue = defaultStringValue;
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool stringValue::set_storedStringValue(std::string newStringValue, std::string varName)
{
    bool conversionSuccess = isValidString(newStringValue,varName);
    if(conversionSuccess == false)
    {
        printf("string value \"%s\" is not a valid string!\n",newStringValue.c_str());
    } else
    {
        storedStringValue = newStringValue;
    }

    return conversionSuccess;
}
/*** end set value functions ***/

/*** get value functions ***/
std::string stringValue::get_variableName()
{
    return variableName;
}

std::string stringValue::get_storedStringValue()
{
    return storedStringValue;
}
/*** end get value functions ***/



/***** private functions *****/



/*** typechecker functions ***/
bool stringValue::isPureString(std::string inputString)
{
    bool conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    // separator characters bug me, but at this point I think that only spaces should be disallowed
    for(size_t charIdx = 0; charIdx < inputString.length(); charIdx++)
    {
        std::string currentChr = inputString.substr(charIdx,1);
        if(currentChr == " ")
        {
            conversionSuccess = false;
            break;
        }
    }

    return conversionSuccess;
}

bool stringValue::isPureFilenameString(std::string inputString)
{
    bool conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    // separator characters bug me, but at this point I think that only spaces should be disallowed
    for(size_t charIdx = 0; charIdx < inputString.length(); charIdx++)
    {
        std::string currentChr = inputString.substr(charIdx,1);
        if(currentChr == " " || currentChr == ":" || currentChr == "`" || currentChr == "~" || currentChr == "!" || currentChr == "@"
                || currentChr == "#" || currentChr == "$" || currentChr == "%" || currentChr == "^" || currentChr == "&" || currentChr == "*"
                || currentChr == "(" || currentChr == ")" || currentChr == "+" || currentChr == "=" || currentChr == "[" || currentChr == "]"
                || currentChr == "{" || currentChr == "}" || currentChr == "|" || currentChr == ";" || currentChr == "\'" || currentChr == "\""
                || currentChr == "," || currentChr == "<" || currentChr == ">" || currentChr == "?")    // probably an easier way to do this, but for now this works
        {
            conversionSuccess = false;
            break;
        }
    }

    return conversionSuccess;
}

bool stringValue::isValidString(std::string inputString, std::string varName)
{
    bool conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    // ugh, have to be insanely specific with string input

    if(varName == "run_base_name")
    {
        if(isPureFilenameString(inputString) == false)
        {
            conversionSuccess = false;
        }
    } else if(varName == "createIgnition_output_units")
    {
        if(inputString != "english" && inputString != "metric" && inputString != "input")
        {
            conversionSuccess = false;
        }
    } else if(varName == "wrfGetWeather_output_units")
    {
        if(inputString != "english" && inputString != "metric")
        {
            conversionSuccess = false;
        }
    } else if(varName == "WindNinja_required_output_units")
    {
        if(inputString != "english" && inputString != "metric")
        {
            conversionSuccess = false;
        }
    } else if(varName == "farsite_output_units")
    {
        if(inputString != "english" && inputString != "metric")
        {
            conversionSuccess = false;
        }
    } else if(varName == "lcp_download_buffer_units")
    {
        if(inputString != "kilometers" && inputString != "miles")
        {
            conversionSuccess = false;
        }
    } else if(varName == "WindNinja_mesh_choice")
    {
        if(inputString != "coarse" && inputString != "medium" && inputString != "fine" && inputString != "custom")
        {
            conversionSuccess = false;
        }
    } else if(varName == "WindNinja_mesh_res_units")
    {
        if(inputString != "ft" && inputString != "m")
        {
            conversionSuccess = false;
        }
    } else if(varName == "farsite_crown_fire_method")
    {
        if(inputString != "Finney" && inputString != "Reinhart")
        {
            conversionSuccess = false;
        }
    } else if(varName == "wrf_file_name")
    {
        if(isPureFilenameString(inputString) == false)
        {
            conversionSuccess = false;
        }
    } else if(varName == "units_goog_out_resolution")
    {
        if(inputString != "ft" && inputString != "m")
        {
            conversionSuccess = false;
        }
    } else if(varName == "goog_out_color_scheme")
    {
        if(inputString != "default" && inputString != "oranges" && inputString != "blues" && inputString != "greens"
                && inputString != "pinks" && inputString != "magic_beans" && inputString != "pink_to_green" && inputString != "ROPGW")
        {
            conversionSuccess = false;
        }
    } else if(varName == "units_shape_out_resolution")
    {
        if(inputString != "ft" && inputString != "m")
        {
            conversionSuccess = false;
        }
    } else if(varName == "units_pdf_out_resolution")
    {
        if(inputString != "ft" && inputString != "m")
        {
            conversionSuccess = false;
        }
    } else if(varName == "pdf_basemap")
    {
        if(inputString != "topofire" && inputString != "hillshade")   // not sure what other options are available
        {
            conversionSuccess = false;
        }
    } else if(varName == "pdf_size")
    {
        if(inputString != "letter" && inputString != "legal" && inputString != "tabloid")
        {
            conversionSuccess = false;
        }
    } else
    {
        printf("string verification for variable \"%s\" has not been implemented in the code yet!\n",varName.c_str());
        conversionSuccess = false;
    }

    if(conversionSuccess == false)
    {
        printf("count value \"%s\" is not valid for variable \"%s\"!\n",inputString.c_str(),varName.c_str());
    }

    return conversionSuccess;
}
/*** end typechecker functions ***/
