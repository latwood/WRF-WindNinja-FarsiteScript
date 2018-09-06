#include "inputVariables_valueStorage.h"

/***** public functions *****/

/*** constructor functions ***/
inputVariables_valueStorage::inputVariables_valueStorage()
{
    bool success = true;

    inputVariableInfo = setupInputVariableInfo.get_inputVariableInfo();

    if(sortInputVariableInfo() == false)
    {
        printf("invalid input variable storage value options found during setup!\n");
        success = false;
    }

    if(success == false)
    {
        printf("exiting program\n");
        exit(1);
    }
}
/*** end constructor functions ***/

/*** reinit functions (like constructors) ***/
bool inputVariables_valueStorage::resetVariables()
{
    printf("\nreseting all variable values storage to default values\n\n");
    bool success = true;

    // reset values for all single count types to default values (keep the stuff with string names from setup)
    for(size_t varIdx = 0; varIdx < boolValues.size(); varIdx++)
    {
        boolValues[varIdx].reset();
    }
    for(size_t varIdx = 0; varIdx < size_tValues.size(); varIdx++)
    {
        size_tValues[varIdx].reset();
    }
    for(size_t varIdx = 0; varIdx < intValues.size(); varIdx++)
    {
        intValues[varIdx].reset();
    }
    for(size_t varIdx = 0; varIdx < doubleValues.size(); varIdx++)
    {
        doubleValues[varIdx].reset();
    }
    for(size_t varIdx = 0; varIdx < stringValues.size(); varIdx++)
    {
        stringValues[varIdx].reset();
    }
    for(size_t varIdx = 0; varIdx < pathNameValues.size(); varIdx++)
    {
        pathNameValues[varIdx].reset();
    }
    for(size_t varIdx = 0; varIdx < lcpFileValues.size(); varIdx++)
    {
        lcpFileValues[varIdx].reset();
    }
    for(size_t varIdx = 0; varIdx < shapeFileValues.size(); varIdx++)
    {
        shapeFileValues[varIdx].reset();
    }
    for(size_t varIdx = 0; varIdx < wrfFileValues.size(); varIdx++)
    {
        wrfFileValues[varIdx].reset();
    }
    for(size_t varIdx = 0; varIdx < lat_longValues.size(); varIdx++)
    {
        lat_longValues[varIdx].reset();
    }
    for(size_t varIdx = 0; varIdx < dateValues.size(); varIdx++)
    {
        dateValues[varIdx].reset();
    }
    for(size_t varIdx = 0; varIdx < hour_minValues.size(); varIdx++)
    {
        hour_minValues[varIdx].reset();
    }


    // now all the count specific variable resetting
    createIgnitionFromLatLongsCount = 0;
    stored_create_ignition_from_latlongs.reset();

    polygonIgnitShapeFileCount = 0;
    stored_polygon_ignit_shape_files.reset();

    GeoMACfirePerimeterFileCount = 0;
    stored_GeoMAC_fire_perimeter_files.reset();

    farsiteOutputFirePerimeterFileCount = 0;
    stored_farsite_output_fire_perimeter_files.reset();

    wrfFileCount = 0;
    stored_wrf_files.reset();

    additionalWindNinjaOutputs_googleCount = 0;
    stored_additional_WindNinja_Outputs_google.reset();

    additionalWindNinjaOutputs_shapefileCount = 0;
    stored_additional_WindNinja_Outputs_shapefile.reset();

    additionalWindNinjaOutputs_pdfCount = 0;
    stored_additional_WindNinja_Outputs_pdf.reset();


    if(setSpecializedDefaults() == false)
    {
        printf("problem setting up specialized default values!\n");
        success = false;
    }
    printf("\nfinished reseting all variable values storage to default values\n\n");

    return success;
}
/*** end reinit functions (like constructors) ***/

/*** get variable info functions ***/
std::vector<inputVariable_info> inputVariables_valueStorage::get_inputVariableInfo()
{
    return inputVariableInfo;
}

std::string inputVariables_valueStorage::get_maxVarNameColumnWhitespace()
{
    return setupInputVariableInfo.get_maxVarNameColumnWhitespace();
}
/*** end get variable info functions ***/

/*** set variable value functions ***/
bool inputVariables_valueStorage::set_boolValue(std::string varName, bool newBoolValue)
{
    bool success = true;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < boolValues.size(); varIdx++)
    {
        if(boolValues[varIdx].get_variableName() == varName)
        {
            printf("setting bool value \"%d\" to variable \"%s\"\n",newBoolValue,varName.c_str());
            boolValues[varIdx].set_storedBoolValue(newBoolValue);
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not set bool value \"%d\" for variable name \"%s\", variableName \%s\" does not exist in bool value storage!\n",newBoolValue,varName.c_str(),varName.c_str());
        success = false;
    }

    return success;
}

bool inputVariables_valueStorage::set_size_tValue(std::string varName, size_t newSize_tValue)
{
    bool success = true;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < size_tValues.size(); varIdx++)
    {
        if(size_tValues[varIdx].get_variableName() == varName)
        {
            printf("setting size_t value \"%zu\" to variable \"%s\"\n",newSize_tValue,varName.c_str());
            size_tValues[varIdx].set_storedSize_tValue(newSize_tValue);
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not set size_t value \"%zu\" for variable name \"%s\", variableName \%s\" does not exist in size_t value storage!\n",newSize_tValue,varName.c_str(),varName.c_str());
        success = false;
    }

    return success;
}

bool inputVariables_valueStorage::set_intValue(std::string varName, int newIntValue)
{
    bool success = true;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < intValues.size(); varIdx++)
    {
        if(intValues[varIdx].get_variableName() == varName)
        {
            printf("setting int value \"%d\" to variable \"%s\"\n",newIntValue,varName.c_str());
            intValues[varIdx].set_storedIntValue(newIntValue);
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not set int value \"%d\" for variable name \"%s\", variableName \%s\" does not exist in int value storage!\n",newIntValue,varName.c_str(),varName.c_str());
        success = false;
    }

    return success;
}

bool inputVariables_valueStorage::set_doubleValue(std::string varName, double newDoubleValue, std::string doubleType)
{
    bool success = true;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < doubleValues.size(); varIdx++)
    {
        if(doubleValues[varIdx].get_variableName() == varName)
        {
            printf("setting double value \"%f\" to variable \"%s\"\n",newDoubleValue,varName.c_str());
            doubleValues[varIdx].set_storedDoubleValue(newDoubleValue,doubleType);
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not set double value \"%f\" for variable name \"%s\", variableName \%s\" does not exist in double value storage!\n",newDoubleValue,varName.c_str(),varName.c_str());
        success = false;
    }

    return success;
}

bool inputVariables_valueStorage::set_stringValue(std::string varName, std::string newStringValue)
{
    bool success = true;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < stringValues.size(); varIdx++)
    {
        if(stringValues[varIdx].get_variableName() == varName)
        {
            printf("setting string value \"%s\" to variable \"%s\"\n",newStringValue.c_str(),varName.c_str());
            stringValues[varIdx].set_storedStringValue(newStringValue);
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not set string value \"%s\" for variable name \"%s\", variableName \%s\" does not exist in string value storage!\n",newStringValue.c_str(),varName.c_str(),varName.c_str());
        success = false;
    }

    return success;
}

bool inputVariables_valueStorage::set_pathNameValue(std::string varName, std::string newPathNameValue)
{
    bool success = true;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < pathNameValues.size(); varIdx++)
    {
        if(pathNameValues[varIdx].get_variableName() == varName)
        {
            printf("setting pathName value \"%s\" to variable \"%s\"\n",newPathNameValue.c_str(),varName.c_str());
            pathNameValues[varIdx].set_storedPathNameValue(newPathNameValue);
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not set pathName value \"%s\" for variable name \"%s\", variableName \%s\" does not exist in pathName value storage!\n",newPathNameValue.c_str(),varName.c_str(),varName.c_str());
        success = false;
    }

    return success;
}

bool inputVariables_valueStorage::set_lcpFileValue(std::string varName, std::string newLcpFileValue)
{
    bool success = true;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < lcpFileValues.size(); varIdx++)
    {
        if(lcpFileValues[varIdx].get_variableName() == varName)
        {
            printf("setting lcpFile value \"%s\" to variable \"%s\"\n",newLcpFileValue.c_str(),varName.c_str());
            lcpFileValues[varIdx].set_storedLcpFileValue(newLcpFileValue);
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not set lcpFile value \"%s\" for variable name \"%s\", variableName \%s\" does not exist in lcpFile value storage!\n",newLcpFileValue.c_str(),varName.c_str(),varName.c_str());
        success = false;
    }

    return success;
}

bool inputVariables_valueStorage::set_shapeFileValue(std::string varName, std::string newShapeFileValue)
{
    bool success = true;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < shapeFileValues.size(); varIdx++)
    {
        if(shapeFileValues[varIdx].get_variableName() == varName)
        {
            printf("setting shapeFile value \"%s\" to variable \"%s\"\n",newShapeFileValue.c_str(),varName.c_str());
            shapeFileValues[varIdx].set_storedShapeFileValue(newShapeFileValue);
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not set shapeFile value \"%s\" for variable name \"%s\", variableName \%s\" does not exist in shapeFile value storage!\n",newShapeFileValue.c_str(),varName.c_str(),varName.c_str());
        success = false;
    }

    return success;
}

bool inputVariables_valueStorage::set_wrfFileValue(std::string varName, std::string newWrfFileValue)
{
    bool success = true;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < wrfFileValues.size(); varIdx++)
    {
        if(wrfFileValues[varIdx].get_variableName() == varName)
        {
            printf("setting wrfFile value \"%s\" to variable \"%s\"\n",newWrfFileValue.c_str(),varName.c_str());
            wrfFileValues[varIdx].set_storedWrfFileName(newWrfFileValue);
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not set wrfFile value \"%s\" for variable name \"%s\", variableName \%s\" does not exist in wrfFile value storage!\n",newWrfFileValue.c_str(),varName.c_str(),varName.c_str());
        success = false;
    }

    return success;
}

bool inputVariables_valueStorage::set_lat_longValue(std::string varName, double newLatValue, double newLongValue)
{
    bool success = true;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < lat_longValues.size(); varIdx++)
    {
        if(lat_longValues[varIdx].get_variableName() == varName)
        {
            printf("setting lat_long value \"%f\" \"%f\" (\"lat\" \"long\") to variable \"%s\"\n",newLatValue,newLongValue,varName.c_str());
            lat_longValues[varIdx].set_storedLatLongValue(newLatValue,newLongValue);
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not set lat_long value \"%f\" \"%f\" (\"lat\" \"long\") for variable name \"%s\", variableName \%s\" does not exist in lat_long value storage!\n",newLatValue,newLongValue,varName.c_str(),varName.c_str());
        success = false;
    }

    return success;
}

bool inputVariables_valueStorage::set_dateValue(std::string varName, int newYearValue, int newMonthValue, int newDayValue, int newHourValue, int newMinuteValue)
{
    bool success = true;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < dateValues.size(); varIdx++)
    {
        if(dateValues[varIdx].get_variableName() == varName)
        {
            printf("setting date value \"%d %d %d %d:%d\" (month day year hour:min) to variable \"%s\"\n",newMonthValue,newDayValue,newYearValue,newHourValue,newMinuteValue,varName.c_str());
            dateValues[varIdx].set_storedDateValue(newYearValue,newMonthValue,newDayValue,newHourValue,newMinuteValue);
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not set date value \"%d %d %d %d:%d\" (month day year hour:min) for variable name \"%s\", variableName \%s\" does not exist in date value storage!\n",newMonthValue,newDayValue,newYearValue,newHourValue,newMinuteValue,varName.c_str(),varName.c_str());
        success = false;
    }

    return success;
}

bool inputVariables_valueStorage::set_hour_minValue(std::string varName, int newHourValue, int newMinuteValue)
{
    bool success = true;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < hour_minValues.size(); varIdx++)
    {
        if(hour_minValues[varIdx].get_variableName() == varName)
        {
            printf("setting hour_min value \"%d:%d\" (hour:minute) to variable \"%s\"\n",newHourValue,newMinuteValue,varName.c_str());
            hour_minValues[varIdx].set_storedHour_MinValue(newHourValue,newMinuteValue);
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not set hour_min value \"%d:%d\" (hour:minute) for variable name \"%s\", variableName \%s\" does not exist in hour_min value storage!\n",newHourValue,newMinuteValue,varName.c_str(),varName.c_str());
        success = false;
    }

    return success;
}
/*** end set variable value functions ***/

/*** load function stuff for count type variables ***/
bool inputVariables_valueStorage::setTypeCount_CountVar(std::string varName,size_t newVarCountValue)
{
    bool success = true;

    if(varName == "create_ignition_from_latlongs")
    {
        createIgnitionFromLatLongsCount = newVarCountValue;
    } else if(varName == "polygon_ignit_shape_files")
    {
        polygonIgnitShapeFileCount = newVarCountValue;
    } else if(varName == "GeoMAC_fire_perimeter_files")
    {
        GeoMACfirePerimeterFileCount = newVarCountValue;
    } else if(varName == "farsite_output_fire_perimeter_files")
    {
        farsiteOutputFirePerimeterFileCount = newVarCountValue;
    } else if(varName == "wrf_files")
    {
        wrfFileCount = newVarCountValue;
    } else if(varName == "additional_WindNinja_outputs_google")
    {
        additionalWindNinjaOutputs_googleCount = newVarCountValue;
    } else if(varName == "additional_WindNinja_outputs_shapefile")
    {
        additionalWindNinjaOutputs_pdfCount = newVarCountValue;
    } else if(varName == "additional_WindNinja_outputs_pdf")
    {
        additionalWindNinjaOutputs_shapefileCount = newVarCountValue;
    } else
    {
        printf("Can't set variable count, loader function stuff for variable \"%s\" has not been implemented in the code yet!\n",varName.c_str());
        success = false;
    }

    if(success == true)
    {
        printf("setting size_t value \"%zu\" as count for variable \"%s\"\n",newVarCountValue,varName.c_str());
    }

    return success;
}

bool inputVariables_valueStorage::add_createIgnitionLatLongValue(double newLat_CoordValue, double newLong_CoordValue)
{
    return stored_create_ignition_from_latlongs.add_createIgnitionLatLongValue(newLat_CoordValue,newLong_CoordValue);
}

bool inputVariables_valueStorage::add_polygonIgnitShapeFile(std::string new_polygon_ignit_shape_file)
{
    return stored_polygon_ignit_shape_files.add_polygonIgnitShapeFile(new_polygon_ignit_shape_file);
}

bool inputVariables_valueStorage::add_GeoMACfirePerimeterFile(std::string new_GeoMAC_fire_perimeter_file)
{
    return stored_GeoMAC_fire_perimeter_files.add_GeoMACfirePerimeterFile(new_GeoMAC_fire_perimeter_file);
}

bool inputVariables_valueStorage::add_farsiteOutputFirePerimeterFile(std::string new_farsite_output_fire_perimeter_file)
{
    return stored_farsite_output_fire_perimeter_files.add_farsiteOutputFirePerimeterFile(new_farsite_output_fire_perimeter_file);
}

bool inputVariables_valueStorage::add_wrfFile(std::string new_wrf_file)
{
    return stored_wrf_files.add_wrfFile(new_wrf_file);
}

bool inputVariables_valueStorage::add_additionalWindNinjaOutputs_googleValueSet(std::string new_wrf_file_name, bool new_write_wx_model_goog_output, bool new_write_goog_output, double new_goog_out_resolution,
                                                   std::string new_units_goog_out_resolution, std::string new_goog_out_color_scheme, bool new_goog_out_vector_scaling)
{
    return stored_additional_WindNinja_Outputs_google.add_additionalWindNinjaOutputs_googleValueSet(new_wrf_file_name,new_write_wx_model_goog_output,new_write_goog_output,
                                                                                             new_goog_out_resolution,new_units_goog_out_resolution,new_goog_out_color_scheme,
                                                                                             new_goog_out_vector_scaling);
}

bool inputVariables_valueStorage::add_additionalWindNinjaOutputs_shapefileValueSet(std::string new_wrf_file_name, bool new_write_wx_model_shapefile_output, bool new_write_shapefile_output,
                                                      double new_shape_out_resolution, std::string new_units_shape_out_resolution)
{
    return stored_additional_WindNinja_Outputs_shapefile.add_additionalWindNinjaOutputs_shapefileValueSet(new_wrf_file_name,new_write_wx_model_shapefile_output,
                                                                                                   new_write_shapefile_output,new_shape_out_resolution,new_units_shape_out_resolution);
}

bool inputVariables_valueStorage::add_additionalWindNinjaOutputs_pdfValueSet(std::string new_wrf_file_name, bool new_write_pdf_output, double new_pdf_out_resolution, std::string new_units_pdf_out_resolution,
                                                    double new_pdf_linewidth, std::string new_pdf_basemap, double new_pdf_height, double new_pdf_width, std::string new_pdf_size)
{
    return stored_additional_WindNinja_Outputs_pdf.add_additionalWindNinjaOutputs_pdfValueSet(new_wrf_file_name,new_write_pdf_output,new_pdf_out_resolution,
                                                                                       new_units_pdf_out_resolution,new_pdf_linewidth,new_pdf_basemap,new_pdf_height,
                                                                                       new_pdf_width,new_pdf_size);
}
/*** end load function stuff for count type variables ***/

/*** get variable value functions ***/
boolValue inputVariables_valueStorage::get_boolValue(std::string varName)
{
    for(size_t varIdx = 0; varIdx < boolValues.size(); varIdx++)
    {
        if(boolValues[varIdx].get_variableName() == varName)
        {
            return boolValues[varIdx];
        }
    }

    // if it got here, the variable name doesn't exist in storage
    printf("could not get bool value for variable name \"%s\", variableName \%s\" does not exist in bool value storage!\n",varName.c_str(),varName.c_str());
    exit(1);
}

size_tValue inputVariables_valueStorage::get_size_tValue(std::string varName)
{
    for(size_t varIdx = 0; varIdx < size_tValues.size(); varIdx++)
    {
        if(size_tValues[varIdx].get_variableName() == varName)
        {
            return size_tValues[varIdx];
        }
    }

    // if it got here, the variable name doesn't exist in storage
    printf("could not get size_t value for variable name \"%s\", variableName \%s\" does not exist in size_t value storage!\n",varName.c_str(),varName.c_str());
    exit(1);
}

intValue inputVariables_valueStorage::get_intValue(std::string varName)
{
    for(size_t varIdx = 0; varIdx < intValues.size(); varIdx++)
    {
        if(intValues[varIdx].get_variableName() == varName)
        {
            return intValues[varIdx];
        }
    }

    // if it got here, the variable name doesn't exist in storage
    printf("could not get int value for variable name \"%s\", variableName \%s\" does not exist in int value storage!\n",varName.c_str(),varName.c_str());
    exit(1);
}

doubleValue inputVariables_valueStorage::get_doubleValue(std::string varName)
{
    for(size_t varIdx = 0; varIdx < doubleValues.size(); varIdx++)
    {
        if(doubleValues[varIdx].get_variableName() == varName)
        {
            return doubleValues[varIdx];
        }
    }

    // if it got here, the variable name doesn't exist in storage
    printf("could not get double value for variable name \"%s\", variableName \%s\" does not exist in double value storage!\n",varName.c_str(),varName.c_str());
    exit(1);
}

stringValue inputVariables_valueStorage::get_stringValue(std::string varName)
{
    for(size_t varIdx = 0; varIdx < stringValues.size(); varIdx++)
    {
        if(stringValues[varIdx].get_variableName() == varName)
        {
            return stringValues[varIdx];
        }
    }

    // if it got here, the variable name doesn't exist in storage
    printf("could not get string value for variable name \"%s\", variableName \%s\" does not exist in string value storage!\n",varName.c_str(),varName.c_str());
    exit(1);
}

pathNameValue inputVariables_valueStorage::get_pathNameValue(std::string varName)
{
    for(size_t varIdx = 0; varIdx < pathNameValues.size(); varIdx++)
    {
        if(pathNameValues[varIdx].get_variableName() == varName)
        {
            return pathNameValues[varIdx];
        }
    }

    // if it got here, the variable name doesn't exist in storage
    printf("could not get pathName value for variable name \"%s\", variableName \%s\" does not exist in pathName value storage!\n",varName.c_str(),varName.c_str());
    exit(1);
}

lcpFileValue inputVariables_valueStorage::get_lcpFileValue(std::string varName)
{
    for(size_t varIdx = 0; varIdx < lcpFileValues.size(); varIdx++)
    {
        if(lcpFileValues[varIdx].get_variableName() == varName)
        {
            return lcpFileValues[varIdx];
        }
    }

    // if it got here, the variable name doesn't exist in storage
    printf("could not get lcpFile value for variable name \"%s\", variableName \%s\" does not exist in lcpFile value storage!\n",varName.c_str(),varName.c_str());
    exit(1);
}

shapeFileValue inputVariables_valueStorage::get_shapeFileValue(std::string varName)
{
    for(size_t varIdx = 0; varIdx < shapeFileValues.size(); varIdx++)
    {
        if(shapeFileValues[varIdx].get_variableName() == varName)
        {
            return shapeFileValues[varIdx];
        }
    }

    // if it got here, the variable name doesn't exist in storage
    printf("could not get shapeFile value for variable name \"%s\", variableName \%s\" does not exist in shapeFile value storage!\n",varName.c_str(),varName.c_str());
    exit(1);
}

wrfFileValue inputVariables_valueStorage::get_wrfFileValue(std::string varName)
{
    for(size_t varIdx = 0; varIdx < wrfFileValues.size(); varIdx++)
    {
        if(wrfFileValues[varIdx].get_variableName() == varName)
        {
            return wrfFileValues[varIdx];
        }
    }

    // if it got here, the variable name doesn't exist in storage
    printf("could not get wrfFile value for variable name \"%s\", variableName \%s\" does not exist in wrfFile value storage!\n",varName.c_str(),varName.c_str());
    exit(1);
}

lat_longValue inputVariables_valueStorage::get_lat_longValue(std::string varName)
{
    for(size_t varIdx = 0; varIdx < lat_longValues.size(); varIdx++)
    {
        if(lat_longValues[varIdx].get_variableName() == varName)
        {
            return lat_longValues[varIdx];
        }
    }

    // if it got here, the variable name doesn't exist in storage
    printf("could not get lat_long value for variable name \"%s\", variableName \%s\" does not exist in lat_long value storage!\n",varName.c_str(),varName.c_str());
    exit(1);
}

dateValue inputVariables_valueStorage::get_dateValue(std::string varName)
{
    for(size_t varIdx = 0; varIdx < dateValues.size(); varIdx++)
    {
        if(dateValues[varIdx].get_variableName() == varName)
        {
            return dateValues[varIdx];
        }
    }

    // if it got here, the variable name doesn't exist in storage
    printf("could not get date value for variable name \"%s\", variableName \%s\" does not exist in date value storage!\n",varName.c_str(),varName.c_str());
    exit(1);
}

hour_minValue inputVariables_valueStorage::get_hour_minValue(std::string varName)
{
    for(size_t varIdx = 0; varIdx < hour_minValues.size(); varIdx++)
    {
        if(hour_minValues[varIdx].get_variableName() == varName)
        {
            return hour_minValues[varIdx];
        }
    }

    // if it got here, the variable name doesn't exist in storage
    printf("could not get hour_min value for variable name \"%s\", variableName \%s\" does not exist in hour_min value storage!\n",varName.c_str(),varName.c_str());
    exit(1);
}
/*** end get variable value functions for single values ***/

/*** get variable value functions for count values ***/
size_t inputVariables_valueStorage::getTypeCount_CountVar(std::string varName)
{
    //size_t outputValue = defaultSize_tValue;
    size_t outputValue = 0;

    bool foundVarName = true;
    if(varName == "create_ignition_from_latlongs")
    {
        outputValue = createIgnitionFromLatLongsCount;
    } else if(varName == "polygon_ignit_shape_files")
    {
        outputValue = polygonIgnitShapeFileCount;
    } else if(varName == "GeoMAC_fire_perimeter_files")
    {
        outputValue = GeoMACfirePerimeterFileCount;
    } else if(varName == "farsite_output_fire_perimeter_files")
    {
        outputValue = farsiteOutputFirePerimeterFileCount;
    } else if(varName == "wrf_files")
    {
        outputValue = wrfFileCount;
    } else if(varName == "additional_WindNinja_outputs_google")
    {
        outputValue = additionalWindNinjaOutputs_googleCount;
    } else if(varName == "additional_WindNinja_outputs_shapefile")
    {
        outputValue = additionalWindNinjaOutputs_shapefileCount;
    } else if(varName == "additional_WindNinja_outputs_pdf")
    {
        outputValue = additionalWindNinjaOutputs_pdfCount;
    } else
    {
        printf("can't get variable count, loader function stuff for variable \"%s\" has not been implemented in the code yet!\n",varName.c_str());
        foundVarName = false;
        exit(1);
    }

    return outputValue;
}

createIgnitionFromLatLongsStorage inputVariables_valueStorage::get_createIgnitionFromLatLongsStorage()
{
    return stored_create_ignition_from_latlongs;
}

polygonIgnitShapeFileStorage inputVariables_valueStorage::get_polygonIgnitShapeFileStorage()
{
    return stored_polygon_ignit_shape_files;
}

GeoMACfirePerimeterFileStorage inputVariables_valueStorage::get_GeoMACfirePerimeterFileStorage()
{
    return stored_GeoMAC_fire_perimeter_files;
}

farsiteOutputFirePerimeterFileStorage inputVariables_valueStorage::get_farsiteOutputFirePerimeterFileStorage()
{
    return stored_farsite_output_fire_perimeter_files;
}

wrfFileStorage inputVariables_valueStorage::get_wrfFileStorage()
{
    return stored_wrf_files;
}

additionalWindNinjaOutputs_googleStorage inputVariables_valueStorage::get_additionalWindNinjaOutputs_googleStorage()
{
    return stored_additional_WindNinja_Outputs_google;
}

additionalWindNinjaOutputs_shapefileStorage inputVariables_valueStorage::get_additionalWindNinjaOutputs_shapefileStorage()
{
    return stored_additional_WindNinja_Outputs_shapefile;
}

additionalWindNinjaOutputs_pdfStorage inputVariables_valueStorage::get_additionalWindNinjaOutputs_pdfStorage()
{
    return stored_additional_WindNinja_Outputs_pdf;
}
/*** end get variable value functions for count values ***/



/***** private functions *****/



/*** setup functions ***/
bool inputVariables_valueStorage::sortInputVariableInfo()
{
    bool success = true;

    for(size_t varIdx = 0; varIdx < inputVariableInfo.size(); varIdx++)
    {
        if(inputVariableInfo[varIdx].get_variableCountType() == "bool")
        {
            boolValues.push_back(boolValue(inputVariableInfo[varIdx].get_variableName()));
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "size_t")
        {
            size_tValues.push_back(size_tValue(inputVariableInfo[varIdx].get_variableName()));
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "int")
        {
            intValues.push_back(intValue(inputVariableInfo[varIdx].get_variableName()));
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "double")
        {
            doubleValues.push_back(doubleValue(inputVariableInfo[varIdx].get_variableName()));
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "positive double")   // value is still a double, input function will make sure it is positive before allowing storage
        {
            doubleValues.push_back(doubleValue(inputVariableInfo[varIdx].get_variableName()));
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "signless percent")  // value is still a double, input function will make sure it is signless before allowing storage
        {
            doubleValues.push_back(doubleValue(inputVariableInfo[varIdx].get_variableName()));
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "string")
        {
            stringValues.push_back(stringValue(inputVariableInfo[varIdx].get_variableName()));
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "pathname")
        {
            pathNameValues.push_back(pathNameValue(inputVariableInfo[varIdx].get_variableName()));
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "lcp filename")  // value is still a filename, input function will make sure it is a valid lcp filename before allowing storage
        {
            lcpFileValues.push_back(lcpFileValue(inputVariableInfo[varIdx].get_variableName()));
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "shape filename")    // value is still a filename, input function will make sure it is a valid shapefile filename before allowing storage
        {
            shapeFileValues.push_back(shapeFileValue(inputVariableInfo[varIdx].get_variableName()));
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "wrf filename")    // value is still a filename, input function will make sure it is a valid shapefile filename before allowing storage
        {
            wrfFileValues.push_back(wrfFileValue(inputVariableInfo[varIdx].get_variableName()));
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "lat_coord")
        {
            doubleValues.push_back(doubleValue(inputVariableInfo[varIdx].get_variableName()));  // value is still a double, input function will make sure it is between -90 and 90 before allowing storage
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "long_coord")
        {
            doubleValues.push_back(doubleValue(inputVariableInfo[varIdx].get_variableName()));  // value is still a double, input function will make sure it is between -180 and 180 before allowing storage
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "lat_long_point")
        {
            lat_longValues.push_back(lat_longValue(inputVariableInfo[varIdx].get_variableName()));
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "date")
        {
            dateValues.push_back(dateValue(inputVariableInfo[varIdx].get_variableName()));
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "hour_min")
        {
            hour_minValues.push_back(hour_minValue(inputVariableInfo[varIdx].get_variableName()));
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "count")
        {
            createIgnitionFromLatLongsCount = 0;
            polygonIgnitShapeFileCount = 0;
            GeoMACfirePerimeterFileCount = 0;
            farsiteOutputFirePerimeterFileCount = 0;
            wrfFileCount = 0;
            additionalWindNinjaOutputs_googleCount = 0;
            additionalWindNinjaOutputs_shapefileCount = 0;
            additionalWindNinjaOutputs_pdfCount = 0;
        } else
        {
            printf("count type \"%s\" for variable \"%s\" has not implemented in code yet!\n",inputVariableInfo[varIdx].get_variableCountType().c_str(),inputVariableInfo[varIdx].get_variableName().c_str());
            success = false;
        }
    }

    return success;
}

bool inputVariables_valueStorage::setSpecializedDefaults()
{
    bool success = true;

    // basically if they specify the variable, it has to be of a specific type of value. If they do not specify it, the default value needs given here in whatever way possible
    // so this should run slow because it runs the inputVariables_valueStorage::findVariableIdx() function multiple times

        // application specific variables
    if(set_boolValue("overwrite_past_outputs",false) == false)
    {
        success = false;
    }
    if(set_stringValue("createIgnition_output_units","input") == false)
    {
        success = false;
    }
    if(set_stringValue("wrfGetWeather_output_units","metric") == false)
    {
        success = false;
    }
    if(set_stringValue("WindNinja_required_output_units","metric") == false)
    {
        success = false;
    }
    if(set_stringValue("farsite_output_units","english") == false)
    {
        success = false;
    }
    if(set_boolValue("use_native_timezone",false) == false)
    {
        success = false;
    }

        // lcp download variables (WindNinja related)
    if(set_boolValue("automate_lcp_download",true) == false)
    {
        success = false;
    }
    if(set_doubleValue("fireperim_to_lcp_scalefactor",1.0,"regular") == false)
    {
        success = false;
    }
    if(set_boolValue("use_past_lcp",false) == false)
    {
        success = false;
    }
    if(set_boolValue("specify_lcp_download",false) == false)
    {
        success = false;
    }

        // createIgnition variables
    if(set_doubleValue("fire_perimeter_widening_factor",1.0,"regular") == false)
    {
        success = false;
    }

        // wrfGetWeather and WindNinja variables
    if(set_boolValue("extend_wrf_data",false) == false)
    {
        success = false;
    }

        // WindNinja only variables
    if(set_size_tValue("WindNinja_number_of_threads",8) == false)
    {
        success = false;
    }
    if(set_stringValue("WindNinja_mesh_choice","fine") == false)
    {
        success = false;
    }
    if(set_stringValue("WindNinja_mesh_res_units","m") == false)
    {
        success = false;
    }
    if(set_boolValue("diurnal_winds",false) == false)
    {
        success = false;
    }
    if(set_boolValue("non_neutral_stability",false) == false)
    {
        success = false;
    }

        // wrfGetWeather only variables
    if(set_boolValue("use_weather_from_ignition_center",false) == false)
    {
        success = false;
    }
    if(set_boolValue("use_weather_from_full_ignition_area",false) == false)
    {
        success = false;
    }
    if(set_boolValue("use_weather_from_wrf_center",true) == false)
    {
        success = false;
    }

        // farsiteAPI variables
    if(set_doubleValue("farsite_spot_probability",0.05,"signless percent") == false)   // need to be careful that it is a legit signless percent
    {
        success = false;
    }
    if(set_size_tValue("farsite_spot_ignition_delay",0) == false)
    {
        success = false;
    }
    if(set_size_tValue("farsite_spotting_seed",1000) == false)
    {
        success = false;
    }
    if(set_hour_minValue("farsite_earliest_burn_time",8,00) == false)
    {
        success = false;
    }
    if(set_hour_minValue("farsite_latest_burn_time",19,59) == false)  // need to be careful that this is later than "farsite_earliest_burn_hour"
    {
        success = false;
    }
    if(set_doubleValue("farsite_foliar_moisture_content",70.0,"signless percent") == false)   // need to be careful that it is a legit signless percent
    {
        success = false;
    }
    if(set_stringValue("farsite_crown_fire_method","Finney") == false)   // need to be careful that it is a legit signless percent
    {
        success = false;
    }

        // debugging related variables
    if(set_boolValue("report_largest_ignition_bounds",false) == false)
    {
        success = false;
    }
    if(set_boolValue("report_found_inputs",false) == false)
    {
        success = false;
    }

        // optional WindNinja output settings, may not even add these options
    // thank goodness, none here

    return success;
}
/*** end setup functions ***/
