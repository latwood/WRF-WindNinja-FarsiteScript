#include "additionalWindNinjaOutputs_googleStorage.h"

/***** public functions *****/

/*** constructor functions ***/
additionalWindNinjaOutputs_googleStorage::additionalWindNinjaOutputs_googleStorage()
{

}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void additionalWindNinjaOutputs_googleStorage::reset()
{
    while(!stored_wrfFileNames.empty())
    {
        stored_wrfFileNames.pop_back();
    }
    while(!stored_write_wx_model_goog_output_values.empty())
    {
        stored_write_wx_model_goog_output_values.pop_back();
    }
    while(!stored_write_goog_output_values.empty())
    {
        stored_write_goog_output_values.pop_back();
    }
    while(!stored_goog_out_resolution_values.empty())
    {
        stored_goog_out_resolution_values.pop_back();
    }
    while(!stored_units_goog_out_resolution_values.empty())
    {
        stored_units_goog_out_resolution_values.pop_back();
    }
    while(!stored_goog_out_color_scheme_values.empty())
    {
        stored_goog_out_color_scheme_values.pop_back();
    }
    while(!stored_goog_out_vector_scaling_values.empty())
    {
        stored_goog_out_vector_scaling_values.pop_back();
    }
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool additionalWindNinjaOutputs_googleStorage::add_additionalWindNinjaOutputs_googleValueSet(std::string new_wrf_file_name, bool new_write_wx_model_goog_output,
                                                                                             bool new_write_goog_output, double new_goog_out_resolution,
                                                                                             std::string new_units_goog_out_resolution,
                                                                                             std::string new_goog_out_color_scheme, bool new_goog_out_vector_scaling)
{
    printf("adding additional_WindNinja_outputs_google \"%s %d %d %f %s %s %d\" (wrf_file_name write_wx_model_goog_output write_goog_output goog_out_resolution units_goog_out_resolution "
           "goog_out_color_scheme goog_out_vector_scaling) to additional_WindNinja_outputs_googleValues variable\n", new_wrf_file_name.c_str(), new_write_wx_model_goog_output,
           new_write_goog_output, new_goog_out_resolution, new_units_goog_out_resolution.c_str(), new_goog_out_color_scheme.c_str(), new_goog_out_vector_scaling);
    stored_wrfFileNames.push_back(new_wrf_file_name);
    boolValue new_boolValue1("");
    new_boolValue1.set_storedBoolValue(new_write_wx_model_goog_output);
    stored_write_wx_model_goog_output_values.push_back(new_boolValue1);
    boolValue new_boolValue2("");
    new_boolValue2.set_storedBoolValue(new_write_goog_output);
    stored_write_goog_output_values.push_back(new_boolValue2);
    doubleValue new_doubleValue("");
    new_doubleValue.set_storedDoubleValue(new_goog_out_resolution,"regular");
    stored_goog_out_resolution_values.push_back(new_doubleValue);
    stringValue new_stringValue1("");
    new_stringValue1.set_storedStringValue(new_units_goog_out_resolution,"units_goog_out_resolution");
    stored_units_goog_out_resolution_values.push_back(new_stringValue1);
    stringValue new_stringValue2("");
    new_stringValue2.set_storedStringValue(new_goog_out_color_scheme,"goog_out_color_scheme");
    stored_goog_out_color_scheme_values.push_back(new_stringValue2);
    boolValue new_boolValue3("");
    new_boolValue3.set_storedBoolValue(new_goog_out_vector_scaling);
    stored_goog_out_vector_scaling_values.push_back(new_boolValue3);

    // if it reaches here, it worked correctly
    return true;
}

bool additionalWindNinjaOutputs_googleStorage::add_additionalWindNinjaOutputs_googleValue_Line(std::string inputDataLine)
{
    bool success = true;

    // first parse through the line to separate all the values as strings
    bool isValue = true;
    std::vector<std::string> foundValues;
    size_t startValueSpot = 0;
    for(size_t charIdx = 0; charIdx < inputDataLine.length(); charIdx++)
    {
        std::string currentChr = inputDataLine.substr(charIdx,1);
        if(currentChr == " " && isValue == true)
        {
            foundValues.push_back(inputDataLine.substr(startValueSpot,charIdx-startValueSpot));
            isValue = false;
        }
        if(currentChr != " " && isValue == false)
        {
            startValueSpot = charIdx;
            isValue = true;
        }
        if(charIdx == inputDataLine.length()-1 && isValue == true)
        {
            foundValues.push_back(inputDataLine.substr(startValueSpot,charIdx-startValueSpot+1));
        }
    }

    /*printf("found values are:");
    for(size_t valIdx = 0; valIdx < foundValues.size(); valIdx++)
    {
        printf(" \"%s\"",foundValues[valIdx].c_str());
    }
    printf("\n\n");*/

    // found the right number of values?
    if(foundValues.size() != 7)
    {
        printf("not enough values found in additional_WindNinja_outputs_google!\n");
        success = false;
    } else  // attempt to store the values
    {
        stored_wrfFileNames.push_back(foundValues[0]);
        boolValue new_boolValue1("write_wx_model_goog_output");
        if(new_boolValue1.set_storedBoolValue_string(foundValues[1]) == false)
        {
            printf("couldn't set bool value \"%s\" to variable \"%s\"!\n",foundValues[1].c_str(),"write_wx_model_goog_output");
            success = false;
        } else
        {
            stored_write_wx_model_goog_output_values.push_back(new_boolValue1);
        }
        boolValue new_boolValue2("write_goog_output");
        if(new_boolValue2.set_storedBoolValue_string(foundValues[2]) == false)
        {
            printf("couldn't set bool value \"%s\" to variable \"%s\"!\n",foundValues[2].c_str(),"write_goog_output");
            success = false;
        } else
        {
            stored_write_goog_output_values.push_back(new_boolValue2);
        }
        doubleValue new_doubleValue("goog_out_resolution");
        if(new_doubleValue.set_storedDoubleValue_string(foundValues[3],"regular") == false)
        {
            printf("couldn't set double value \"%s\" to variable \"%s\"!\n",foundValues[3].c_str(),"goog_out_resolution");
            success = false;
        } else
        {
            stored_goog_out_resolution_values.push_back(new_doubleValue);
        }
        stringValue new_stringValue1("units_goog_out_resolution");
        if(new_stringValue1.set_storedStringValue(foundValues[4],"units_goog_out_resolution") == false)
        {
            printf("couldn't set string \"%s\" to variable \"%s\"!\n",foundValues[4].c_str(),"units_goog_out_resolution");
            success = false;
        } else
        {
            stored_units_goog_out_resolution_values.push_back(new_stringValue1);
        }
        stringValue new_stringValue2("goog_out_color_scheme");
        if(new_stringValue2.set_storedStringValue(foundValues[5],"goog_out_color_scheme") == false)
        {
            printf("couldn't set string \"%s\" to variable \"%s\"!\n",foundValues[5].c_str(),"goog_out_color_scheme");
            success = false;
        } else
        {
            stored_goog_out_color_scheme_values.push_back(new_stringValue2);
        }
        boolValue new_boolValue3("goog_out_vector_scaling_values");
        if(new_boolValue3.set_storedBoolValue_string(foundValues[6]) == false)
        {
            printf("couldn't set bool value \"%s\" to variable \"%s\"!\n",foundValues[6].c_str(),"goog_out_vector_scaling_values");
            success = false;
        } else
        {
            stored_goog_out_vector_scaling_values.push_back(new_boolValue3);
        }
    }

    return success;
}
/*** end set value functions ***/

/*** get value functions ***/
std::vector<std::string> additionalWindNinjaOutputs_googleStorage::get_stored_wrfFileNames()
{
    return stored_wrfFileNames;
}

std::vector<boolValue> additionalWindNinjaOutputs_googleStorage::get_stored_write_wx_model_goog_output_values()
{
    return stored_write_wx_model_goog_output_values;
}

std::vector<boolValue> additionalWindNinjaOutputs_googleStorage::get_stored_write_goog_output_values()
{
    return stored_write_goog_output_values;
}

std::vector<doubleValue> additionalWindNinjaOutputs_googleStorage::get_stored_goog_out_resolution_values()
{
    return stored_goog_out_resolution_values;
}

std::vector<stringValue> additionalWindNinjaOutputs_googleStorage::get_stored_units_goog_out_resolution_values()
{
    return stored_units_goog_out_resolution_values;
}

std::vector<stringValue> additionalWindNinjaOutputs_googleStorage::get_stored_goog_out_color_scheme_values()
{
    return stored_goog_out_color_scheme_values;
}

std::vector<boolValue> additionalWindNinjaOutputs_googleStorage::get_stored_goog_out_vector_scaling_values()
{
    return stored_goog_out_vector_scaling_values;
}
/*** end get value functions ***/



/***** private functions *****/



/*** reconstructor like functions ***/
/*** end reconstructor like functions ***/
