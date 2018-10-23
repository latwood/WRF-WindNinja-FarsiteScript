#include "additionalWindNinjaOutputs_pdfStorage.h"

/***** public functions *****/

/*** constructor functions ***/
additionalWindNinjaOutputs_pdfStorage::additionalWindNinjaOutputs_pdfStorage()
{

}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void additionalWindNinjaOutputs_pdfStorage::reset()
{
    while(!stored_wrfFileNames.empty())
    {
        stored_wrfFileNames.pop_back();
    }
    while(!stored_write_pdf_output_values.empty())
    {
        stored_write_pdf_output_values.pop_back();
    }
    while(!stored_pdf_out_resolution_values.empty())
    {
        stored_pdf_out_resolution_values.pop_back();
    }
    while(!stored_units_pdf_out_resolution_values.empty())
    {
        stored_units_pdf_out_resolution_values.pop_back();
    }
    while(!stored_pdf_linewidth_values.empty())
    {
        stored_pdf_linewidth_values.pop_back();
    }
    while(!stored_pdf_basemap_values.empty())
    {
        stored_pdf_basemap_values.pop_back();
    }
    while(!stored_pdf_height_values.empty())
    {
        stored_pdf_height_values.pop_back();
    }
    while(!stored_pdf_width_values.empty())
    {
        stored_pdf_width_values.pop_back();
    }
    while(!stored_pdf_size_values.empty())
    {
        stored_pdf_size_values.pop_back();
    }
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool additionalWindNinjaOutputs_pdfStorage::add_additionalWindNinjaOutputs_pdfValueSet(std::string new_wrf_file_name, bool new_write_pdf_output, double new_pdf_out_resolution,
                                                                             std::string new_units_pdf_out_resolution, double new_pdf_linewidth, std::string new_pdf_basemap,
                                                                             double new_pdf_height, double new_pdf_width, std::string new_pdf_size)
{
    printf("adding additional_WindNinja_outputs_pdf \"%s %d %f %s %f %s %f %f %s\" (wrf_file_name write_pdf_output pdf_out_resolution units_pdf_out_resolution pdf_linewidth "
           "pdf_basemap pdf_height pdf_width pdf_size) to additional_WindNinja_outputs_pdfValues variable\n",new_wrf_file_name.c_str(), new_write_pdf_output, new_pdf_out_resolution,
           new_units_pdf_out_resolution.c_str(), new_pdf_linewidth, new_pdf_basemap.c_str(), new_pdf_height, new_pdf_width, new_pdf_size.c_str());
    stored_wrfFileNames.push_back(new_wrf_file_name);
    boolValue new_boolValue("");
    new_boolValue.set_storedBoolValue(new_write_pdf_output);
    stored_write_pdf_output_values.push_back(new_boolValue);
    doubleValue new_doubleValue1("");
    new_doubleValue1.set_storedDoubleValue(new_pdf_out_resolution,"regular");
    stored_pdf_out_resolution_values.push_back(new_doubleValue1);
    stringValue new_stringValue1("");
    new_stringValue1.set_storedStringValue(new_units_pdf_out_resolution,"units_pdf_out_resolution");
    stored_units_pdf_out_resolution_values.push_back(new_stringValue1);
    doubleValue new_doubleValue2("");
    new_doubleValue2.set_storedDoubleValue(new_pdf_linewidth,"positive");
    stored_pdf_linewidth_values.push_back(new_doubleValue2);
    stringValue new_stringValue2("");
    new_stringValue2.set_storedStringValue(new_pdf_basemap,"pdf_basemap");
    stored_pdf_basemap_values.push_back(new_stringValue2);
    doubleValue new_doubleValue3("");
    new_doubleValue3.set_storedDoubleValue(new_pdf_height,"positive");
    stored_pdf_height_values.push_back(new_doubleValue3);
    doubleValue new_doubleValue4("");
    new_doubleValue4.set_storedDoubleValue(new_pdf_width,"positive");
    stored_pdf_width_values.push_back(new_doubleValue4);
    stringValue new_stringValue3("");
    new_stringValue3.set_storedStringValue(new_pdf_size,"pdf_size");
    stored_pdf_size_values.push_back(new_stringValue3);

    // if it reaches here, it worked correctly
    return true;
}

bool additionalWindNinjaOutputs_pdfStorage::add_additionalWindNinjaOutputs_pdfValue_Line(std::string inputDataLine)
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
    if(foundValues.size() != 9)
    {
        printf("not enough values found in additional_WindNinja_outputs_pdf!\n");
        success = false;
    } else  // attempt to store the values
    {
        stored_wrfFileNames.push_back(foundValues[0]);
        boolValue new_boolValue("");
        if(new_boolValue.set_storedBoolValue_string(foundValues[1]) == false)
        {
            printf("couldn't set bool value \"%s\" to variable \"%s\"!\n",foundValues[1].c_str(),"write_pdf_output");
            success = false;
        } else
        {
            stored_write_pdf_output_values.push_back(new_boolValue);
        }
        doubleValue new_doubleValue1("");
        if(new_doubleValue1.set_storedDoubleValue_string(foundValues[2],"regular") == false)
        {
            printf("couldn't set string \"%s\" to variable \"%s\"!\n",foundValues[2].c_str(),"pdf_out_resolution");
            success = false;
        } else
        {
            stored_pdf_out_resolution_values.push_back(new_doubleValue1);
        }
        stringValue new_stringValue1("");
        if(new_stringValue1.set_storedStringValue(foundValues[3],"units_pdf_out_resolution") == false)
        {
            printf("couldn't set string \"%s\" to variable \"%s\"!\n",foundValues[3].c_str(),"units_pdf_out_resolution");
            success = false;
        } else
        {
            stored_units_pdf_out_resolution_values.push_back(new_stringValue1);
        }
        doubleValue new_doubleValue2("");
        if(new_doubleValue2.set_storedDoubleValue_string(foundValues[4],"positive") == false)
        {
            printf("couldn't set double value \"%s\" to variable \"%s\"!\n",foundValues[4].c_str(),"pdf_linewidth");
            success = false;
        } else
        {
            stored_pdf_linewidth_values.push_back(new_doubleValue2);
        }
        stringValue new_stringValue2("");
        if(new_stringValue2.set_storedStringValue(foundValues[5],"pdf_basemap") == false)
        {
            printf("couldn't set string \"%s\" to variable \"%s\"!\n",foundValues[5].c_str(),"pdf_basemap");
            success = false;
        } else
        {
            stored_pdf_basemap_values.push_back(new_stringValue2);
        }
        doubleValue new_doubleValue3("");
        if(new_doubleValue3.set_storedDoubleValue_string(foundValues[6],"positive") == false)
        {
            printf("couldn't set double value \"%s\" to variable \"%s\"!\n",foundValues[6].c_str(),"pdf_height");
            success = false;
        } else
        {
            stored_pdf_height_values.push_back(new_doubleValue3);
        }
        doubleValue new_doubleValue4("");
        if(new_doubleValue4.set_storedDoubleValue_string(foundValues[7],"positive") == false)
        {
            printf("couldn't set double value \"%s\" to variable \"%s\"!\n",foundValues[7].c_str(),"pdf_width");
            success = false;
        } else
        {
            stored_pdf_width_values.push_back(new_doubleValue4);
        }
        stringValue new_stringValue3("");
        if(new_stringValue3.set_storedStringValue(foundValues[8],"pdf_size") == false)
        {
            printf("couldn't set string \"%s\" to variable \"%s\"!\n",foundValues[4].c_str(),"pdf_size");
            success = false;
        } else
        {
            stored_pdf_size_values.push_back(new_stringValue3);
        }
    }

    return success;
}
/*** end set value functions ***/

/*** get value functions ***/
std::vector<std::string> additionalWindNinjaOutputs_pdfStorage::get_stored_wrfFileNames()
{
    return stored_wrfFileNames;
}

std::vector<boolValue> additionalWindNinjaOutputs_pdfStorage::get_stored_write_pdf_output_values()
{
    return stored_write_pdf_output_values;
}

std::vector<doubleValue> additionalWindNinjaOutputs_pdfStorage::get_stored_pdf_out_resolution_values()
{
    return stored_pdf_out_resolution_values;
}

std::vector<stringValue> additionalWindNinjaOutputs_pdfStorage::get_stored_units_pdf_out_resolution_values()
{
    return stored_units_pdf_out_resolution_values;
}

std::vector<doubleValue> additionalWindNinjaOutputs_pdfStorage::get_stored_pdf_linewidth_values()
{
    return stored_pdf_linewidth_values;
}

std::vector<stringValue> additionalWindNinjaOutputs_pdfStorage::get_stored_pdf_basemap_values()
{
    return stored_pdf_basemap_values;
}

std::vector<doubleValue> additionalWindNinjaOutputs_pdfStorage::get_stored_pdf_height_values()
{
    return stored_pdf_height_values;
}

std::vector<doubleValue> additionalWindNinjaOutputs_pdfStorage::get_stored_pdf_width_values()
{
    return stored_pdf_width_values;
}

std::vector<stringValue> additionalWindNinjaOutputs_pdfStorage::get_stored_pdf_size_values()
{
    return stored_pdf_size_values;
}
/*** end get value functions ***/



/***** private functions *****/



/*** reconstructor like functions ***/
/*** end reconstructor like functions ***/
