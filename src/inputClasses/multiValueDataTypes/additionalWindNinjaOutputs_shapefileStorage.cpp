#include "additionalWindNinjaOutputs_shapefileStorage.h"


/***** public functions *****/

/*** constructor functions ***/
additionalWindNinjaOutputs_shapefileStorage::additionalWindNinjaOutputs_shapefileStorage()
{

}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void additionalWindNinjaOutputs_shapefileStorage::reset()
{
    while(!stored_wrfFileNames.empty())
    {
        stored_wrfFileNames.pop_back();
    }
    while(!stored_write_wx_model_shapefile_output_values.empty())
    {
        stored_write_wx_model_shapefile_output_values.pop_back();
    }
    while(!stored_write_shapefile_output_values.empty())
    {
        stored_write_shapefile_output_values.pop_back();
    }
    while(!stored_shape_out_resolution_values.empty())
    {
        stored_shape_out_resolution_values.pop_back();
    }
    while(!stored_units_shape_out_resolution_values.empty())
    {
        stored_units_shape_out_resolution_values.pop_back();
    }
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool additionalWindNinjaOutputs_shapefileStorage::add_additionalWindNinjaOutputs_shapefileValueSet(std::string new_wrf_file_name, bool new_write_wx_model_shapefile_output,
                                                                                                   bool new_write_shapefile_output, double new_shape_out_resolution,
                                                                                                   std::string new_units_shape_out_resolution)
{
    printf("adding additional_WindNinja_outputs_shapefile \"%s %d %d %f %s\" (wrf_file_name write_wx_model_shapefile_output write_shapefile_output shape_out_resolution "
           "units_shape_out_resolution) to additional_WindNinja_outputs_shapefileValues variable\n",new_wrf_file_name.c_str(), new_write_wx_model_shapefile_output,
           new_write_shapefile_output, new_shape_out_resolution, new_units_shape_out_resolution.c_str());
    stored_wrfFileNames.push_back(new_wrf_file_name);
    boolValue new_boolValue1("");
    new_boolValue1.set_storedBoolValue(new_write_wx_model_shapefile_output);
    stored_write_wx_model_shapefile_output_values.push_back(new_boolValue1);
    boolValue new_boolValue2("");
    new_boolValue2.set_storedBoolValue(new_write_shapefile_output);
    stored_write_shapefile_output_values.push_back(new_boolValue2);
    doubleValue new_doubleValue("");
    new_doubleValue.set_storedDoubleValue(new_shape_out_resolution,"regular");
    stored_shape_out_resolution_values.push_back(new_doubleValue);
    stringValue new_stringValue("");
    new_stringValue.set_storedStringValue(new_units_shape_out_resolution,"units_shape_out_resolution");
    stored_units_shape_out_resolution_values.push_back(new_stringValue);

    // if it reaches here, it worked correctly
    return true;
}

bool additionalWindNinjaOutputs_shapefileStorage::add_additionalWindNinjaOutputs_shapeValue_Line(std::string inputDataLine)
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
    if(foundValues.size() != 5)
    {
        printf("not enough values found in additional_WindNinja_outputs_shapefile!\n");
        success = false;
    } else  // attempt to store the values
    {
        stored_wrfFileNames.push_back(foundValues[0]);
        boolValue new_boolValue1("");
        if(new_boolValue1.set_storedBoolValue_string(foundValues[1]) == false)
        {
            printf("couldn't set bool value \"%s\" to variable \"%s\"!\n",foundValues[1].c_str(),"write_wx_model_shapefile_output");
            success = false;
        } else
        {
            stored_write_wx_model_shapefile_output_values.push_back(new_boolValue1);
        }
        boolValue new_boolValue2("");
        if(new_boolValue2.set_storedBoolValue_string(foundValues[2]) == false)
        {
            printf("couldn't set bool value \"%s\" to variable \"%s\"!\n",foundValues[2].c_str(),"write_shapefile_output");
            success = false;
        } else
        {
            stored_write_shapefile_output_values.push_back(new_boolValue2);
        }
        doubleValue new_doubleValue("");
        if(new_doubleValue.set_storedDoubleValue_string(foundValues[3],"regular") == false)
        {
            printf("couldn't set string \"%s\" to variable \"%s\"!\n",foundValues[3].c_str(),"shape_out_resolution");
            success = false;
        } else
        {
            stored_shape_out_resolution_values.push_back(new_doubleValue);
        }
        stringValue new_stringValue("");
        if(new_stringValue.set_storedStringValue(foundValues[4],"units_shape_out_resolution") == false)
        {
            printf("couldn't set string \"%s\" to variable \"%s\"!\n",foundValues[4].c_str(),"units_shape_out_resolution");
            success = false;
        } else
        {
            stored_units_shape_out_resolution_values.push_back(new_stringValue);
        }
    }

    return success;
}
/*** end set value functions ***/

/*** get value functions ***/
std::vector<std::string> additionalWindNinjaOutputs_shapefileStorage::get_stored_wrfFileNames()
{
    return stored_wrfFileNames;
}

std::vector<boolValue> additionalWindNinjaOutputs_shapefileStorage::get_stored_write_wx_model_shapefile_output_values()
{
    return stored_write_wx_model_shapefile_output_values;
}

std::vector<boolValue> additionalWindNinjaOutputs_shapefileStorage::get_stored_write_shapefile_output_values()
{
    return stored_write_shapefile_output_values;
}

std::vector<doubleValue> additionalWindNinjaOutputs_shapefileStorage::get_stored_shape_out_resolution_values()
{
    return stored_shape_out_resolution_values;
}

std::vector<stringValue> additionalWindNinjaOutputs_shapefileStorage::get_stored_units_shape_out_resolution_values()
{
    return stored_units_shape_out_resolution_values;
}
/*** end get value functions ***/



/***** private functions *****/



/*** reconstructor like functions ***/
/*** end reconstructor like functions ***/
