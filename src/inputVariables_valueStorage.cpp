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
    for(size_t boolVarIdx = 0; boolVarIdx < boolValues.size(); boolVarIdx++)
    {
        boolValues[boolVarIdx].variableValue = defaultBoolValue;
    }
    for(size_t size_tVarIdx = 0; size_tVarIdx < size_tValues.size(); size_tVarIdx++)
    {
        size_tValues[size_tVarIdx].variableValue = defaultSize_tValue;
    }
    for(size_t intVarIdx = 0; intVarIdx < intValues.size(); intVarIdx++)
    {
        intValues[intVarIdx].variableValue = defaultIntValue;
    }
    for(size_t doubleVarIdx = 0; doubleVarIdx < doubleValues.size(); doubleVarIdx++)
    {
        doubleValues[doubleVarIdx].variableValue = defaultDoubleValue;
    }
    for(size_t stringVarIdx = 0; stringVarIdx < stringValues.size(); stringVarIdx++)
    {
        stringValues[stringVarIdx].variableValue = defaultStringValue;
    }
    for(size_t pathnameVarIdx = 0; pathnameVarIdx < pathnameValues.size(); pathnameVarIdx++)
    {
        pathnameValues[pathnameVarIdx].variableValue = defaultPathnameValue;
    }
    for(size_t filenameVarIdx = 0; filenameVarIdx < filenameValues.size(); filenameVarIdx++)
    {
        filenameValues[filenameVarIdx].variableValue = defaultFilenameValue;
    }
    for(size_t lat_long_pointVarIdx = 0; lat_long_pointVarIdx < lat_long_pointValues.size(); lat_long_pointVarIdx++)
    {
        lat_long_pointValues[lat_long_pointVarIdx].lat_coord = defaultLatValue;
        lat_long_pointValues[lat_long_pointVarIdx].long_coord = defaultLongValue;
    }
    for(size_t dateVarIdx = 0; dateVarIdx < dateValues.size(); dateVarIdx++)
    {
        dateValues[dateVarIdx].year = defaultYearValue;
        dateValues[dateVarIdx].month = defaultMonthValue;
        dateValues[dateVarIdx].day = defaultDayValue;
        dateValues[dateVarIdx].hour = defaultHourValue;
        dateValues[dateVarIdx].minute = defaultMinuteValue;
    }
    for(size_t hour_minVarIdx = 0; hour_minVarIdx < hour_minValues.size(); hour_minVarIdx++)
    {
        hour_minValues[hour_minVarIdx].hour = defaultHourValue;
        hour_minValues[hour_minVarIdx].minute = defaultMinuteValue;
    }

    // now all the count specific variable resetting
    while(!create_ignition_from_latlongs.empty())
    {
        create_ignition_from_latlongs.pop_back();
    }
    create_ignition_from_latlongs_count = 0;

    while(!polygon_ignit_shape_files.empty())
    {
        polygon_ignit_shape_files.pop_back();
    }
    polygon_ignit_shape_files_count = 0;

    while(!GeoMAC_fire_perimeter_files.empty())
    {
        GeoMAC_fire_perimeter_files.pop_back();
    }
    GeoMAC_fire_perimeter_files_count = 0;

    while(!farsite_output_fire_perimeter_files.empty())
    {
        farsite_output_fire_perimeter_files.pop_back();
    }
    farsite_output_fire_perimeter_files_count = 0;

    while(!wrf_files.empty())
    {
        wrf_files.pop_back();
    }
    wrf_files_count = 0;

    while(!additional_WindNinja_outputs_googleValues.empty())
    {
        additional_WindNinja_outputs_googleValues.pop_back();
    }
    additional_WindNinja_outputs_google_count = 0;

    while(!additional_WindNinja_outputs_shapefileValues.empty())
    {
        additional_WindNinja_outputs_shapefileValues.pop_back();
    }
    additional_WindNinja_outputs_shapefile_count = 0;

    while(!additional_WindNinja_outputs_pdfValues.empty())
    {
        additional_WindNinja_outputs_pdfValues.pop_back();
    }
    additional_WindNinja_outputs_pdf_count = 0;

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
bool inputVariables_valueStorage::set_inputVariableBoolValue(std::string varName, bool newBoolValue)
{
    bool success = true;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < boolValues.size(); varIdx++)
    {
        if(boolValues[varIdx].variableName == varName)
        {
            printf("setting bool value \"%d\" to variable \"%s\"\n",newBoolValue,varName.c_str());
            boolValues[varIdx].variableValue = newBoolValue;
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

bool inputVariables_valueStorage::set_inputVariableSize_tValue(std::string varName, size_t newSize_tValue)
{
    bool success = true;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < size_tValues.size(); varIdx++)
    {
        if(size_tValues[varIdx].variableName == varName)
        {
            printf("setting size_t value \"%zu\" to variable \"%s\"\n",newSize_tValue,varName.c_str());
            size_tValues[varIdx].variableValue = newSize_tValue;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not set size_t value \"%zu\" for variable name \"%s\", variableName \%s\" does not exist in size_t value storage\n!",newSize_tValue,varName.c_str(),varName.c_str());
        success = false;
    }

    return success;
}

bool inputVariables_valueStorage::set_inputVariableIntValue(std::string varName, int newIntValue)
{
    bool success = true;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < intValues.size(); varIdx++)
    {
        if(intValues[varIdx].variableName == varName)
        {
            printf("setting int value \"%d\" to variable \"%s\"\n",newIntValue,varName.c_str());
            intValues[varIdx].variableValue = newIntValue;
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

bool inputVariables_valueStorage::set_inputVariableDoubleValue(std::string varName, double newDoubleValue)
{
    bool success = true;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < doubleValues.size(); varIdx++)
    {
        if(doubleValues[varIdx].variableName == varName)
        {
            printf("setting double value \"%f\" to variable \"%s\"\n",newDoubleValue,varName.c_str());
            doubleValues[varIdx].variableValue = newDoubleValue;
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

bool inputVariables_valueStorage::set_inputVariableStringValue(std::string varName, std::string newStringValue)
{
    bool success = true;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < stringValues.size(); varIdx++)
    {
        if(stringValues[varIdx].variableName == varName)
        {
            printf("setting string value \"%s\" to variable \"%s\"\n",newStringValue.c_str(),varName.c_str());
            stringValues[varIdx].variableValue = newStringValue;
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

bool inputVariables_valueStorage::set_inputVariablePathnameValue(std::string varName, std::string newPathnameValue)
{
    bool success = true;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < pathnameValues.size(); varIdx++)
    {
        if(pathnameValues[varIdx].variableName == varName)
        {
            printf("setting pathname value \"%s\" to variable \"%s\"\n",newPathnameValue.c_str(),varName.c_str());
            pathnameValues[varIdx].variableValue = newPathnameValue;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not set pathname value \"%s\" for variable name \"%s\", variableName \%s\" does not exist in pathname value storage!\n",newPathnameValue.c_str(),varName.c_str(),varName.c_str());
        success = false;
    }

    return success;
}

bool inputVariables_valueStorage::set_inputVariableFilenameValue(std::string varName, std::string newFilenameValue)
{
    bool success = true;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < filenameValues.size(); varIdx++)
    {
        if(filenameValues[varIdx].variableName == varName)
        {
            printf("setting filename value \"%s\" to variable \"%s\"\n",newFilenameValue.c_str(),varName.c_str());
            filenameValues[varIdx].variableValue = newFilenameValue;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not set filename value \"%s\" for variable name \"%s\", variableName \%s\" does not exist in filename value storage!\n",newFilenameValue.c_str(),varName.c_str(),varName.c_str());
        success = false;
    }

    return success;
}


bool inputVariables_valueStorage::set_inputVariableLat_Long_PointValue(std::string varName, double newLat_CoordValue, double newLong_CoordValue)
{
    bool success = true;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < lat_long_pointValues.size(); varIdx++)
    {
        if(lat_long_pointValues[varIdx].variableName == varName)
        {
            printf("setting lat_long_point values \"%f %f\" (lat long) to variable \"%s\"\n",newLat_CoordValue,newLong_CoordValue,varName.c_str());
            lat_long_pointValues[varIdx].lat_coord = newLat_CoordValue;
            lat_long_pointValues[varIdx].long_coord = newLong_CoordValue;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not set lat_long_point value \"%f %f\" (lat long) for variable name \"%s\", variableName \%s\" does not exist in lat_long_point value storage!\n",newLat_CoordValue,newLong_CoordValue,varName.c_str(),varName.c_str());
        success = false;
    }

    return success;
}

bool inputVariables_valueStorage::set_inputVariableDateValue(std::string varName, int newYearValue, int newMonthValue, int newDayValue, int newHourValue, int newMinuteValue)
{
    bool success = true;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < dateValues.size(); varIdx++)
    {
        if(dateValues[varIdx].variableName == varName)
        {
            printf("setting date value \"%d %d %d %d:%d\" (month day year hour:minute) to variable \"%s\"\n",newMonthValue,newDayValue,newYearValue,newHourValue,newMinuteValue,varName.c_str());
            dateValues[varIdx].year = newYearValue;
            dateValues[varIdx].month = newMonthValue;
            dateValues[varIdx].day = newDayValue;
            dateValues[varIdx].hour = newHourValue;
            dateValues[varIdx].minute = newMinuteValue;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not set date value \"%d %d %d %d %d\" (year month day hour minute) for variable name \"%s\", variableName \%s\" does not exist in date value storage!\n",newYearValue,newMonthValue,newDayValue,newHourValue,newMinuteValue,varName.c_str(),varName.c_str());
        success = false;
    }

    return success;
}

bool inputVariables_valueStorage::set_inputVariableHour_MinValue(std::string varName, int newHourValue, int newMinuteValue)
{
    bool success = true;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < hour_minValues.size(); varIdx++)
    {
        if(hour_minValues[varIdx].variableName == varName)
        {
            printf("setting hour_min value \"%d:%d\" (hour:minute) to variable \"%s\"\n",newHourValue,newMinuteValue,varName.c_str());
            hour_minValues[varIdx].hour = newHourValue;
            hour_minValues[varIdx].minute = newMinuteValue;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not set hour_min value \"%d %d\" (hour minute) for variable name \"%s\", variableName \%s\" does not exist in hour_min value storage!\n",newHourValue,newMinuteValue,varName.c_str(),varName.c_str());
        success = false;
    }

    return success;
}

bool inputVariables_valueStorage::setTypeCount_CountVar(std::string varName,std::string loaderFunction,size_t newVarCountValue)
{
    bool success = true;

    if(loaderFunction == "load_create_ignition_from_latlongs")
    {
        create_ignition_from_latlongs_count = newVarCountValue;
    } else if(loaderFunction == "load_polygon_ignit_shape_files")
    {
        polygon_ignit_shape_files_count = newVarCountValue;
    } else if(loaderFunction == "load_GeoMAC_fire_perimeter_files")
    {
        GeoMAC_fire_perimeter_files_count = newVarCountValue;
    } else if(loaderFunction == "load_farsite_output_fire_perimeter_files")
    {
        farsite_output_fire_perimeter_files_count = newVarCountValue;
    } else if(loaderFunction == "load_wrf_files")
    {
        wrf_files_count = newVarCountValue;
    } else if(loaderFunction == "load_additional_WindNinja_outputs_google")
    {
        additional_WindNinja_outputs_google_count = newVarCountValue;
    } else if(loaderFunction == "load_additional_WindNinja_outputs_shapefile")
    {
        additional_WindNinja_outputs_shapefile_count = newVarCountValue;
    } else if(loaderFunction == "load_additional_WindNinja_outputs_pdf")
    {
        additional_WindNinja_outputs_pdf_count = newVarCountValue;
    } else
    {
        printf("loader function stuff for variable \"%s\" loader function \"%s\" has not been implemented in the code yet!\n",varName.c_str(),loaderFunction.c_str());
        success = false;
    }

    if(success == true)
    {
        printf("setting size_t value \"%zu\" as count for variable \"%s\"\n",newVarCountValue,varName.c_str());
    }

    return success;
}
/*** end set variable value functions ***/

/*** load function stuff for count type variables ***/
void inputVariables_valueStorage::add_create_ignition_from_latlongs_coord(std::string newVarName, double newLat_CoordValue, double newLong_CoordValue)
{
    printf("adding create_ignition_from_latlongs \"%f %f\" (lat long) to create_ignition_from_latlongs variable\n",newLat_CoordValue,newLong_CoordValue);
    create_ignition_from_latlongs.push_back({newVarName,newLat_CoordValue,newLong_CoordValue});
}

void inputVariables_valueStorage::add_polygon_ignit_shape_file(std::string new_polygon_ignit_shape_file)
{
    printf("adding polygon_ignit_shape_files \"%s\" to polygon_ignit_shape_files variable\n",new_polygon_ignit_shape_file.c_str());
    polygon_ignit_shape_files.push_back(new_polygon_ignit_shape_file);
}

void inputVariables_valueStorage::add_GeoMAC_fire_perimeter_file(std::string new_GeoMAC_fire_perimeter_file)
{
    printf("adding GeoMAC_fire_perimeter_files \"%s\" to GeoMAC_fire_perimeter_files variable\n",new_GeoMAC_fire_perimeter_file.c_str());
    GeoMAC_fire_perimeter_files.push_back(new_GeoMAC_fire_perimeter_file);
}

void inputVariables_valueStorage::add_farsite_output_fire_perimeter_file(std::string new_farsite_output_fire_perimeter_file)
{
    printf("adding farsite_output_fire_perimeter_files \"%s\" to farsite_output_fire_perimeter_files variable\n",new_farsite_output_fire_perimeter_file.c_str());
    farsite_output_fire_perimeter_files.push_back(new_farsite_output_fire_perimeter_file);
}

void inputVariables_valueStorage::add_wrf_file(std::string new_wrf_file)
{
    // might want to check to see if file is a legit file? Here or somewhere else? Format checking?
    // I guess the assumption right now is that the parser handles all the format checking and if it is a legit file, just calls storage functions from here
    printf("adding wrf_file \"%s\" to wrf_files variable\n",new_wrf_file.c_str());
    wrf_files.push_back(new_wrf_file);
}

void inputVariables_valueStorage::add_additional_WindNinja_outputs_googleValues(std::string new_wrf_file_name, bool new_write_wx_model_goog_output, bool new_write_goog_output,
                                                                                double new_goog_out_resolution, std::string new_units_goog_out_resolution,
                                                                                std::string new_goog_out_color_scheme, bool new_goog_out_vector_scaling)
{
    printf("adding additional_WindNinja_outputs_google \"%s %d %d %f %s %s %d\" (wrf_file_name write_wx_model_goog_output write_goog_output goog_out_resolution units_goog_out_resolution "
           "goog_out_color_scheme goog_out_vector_scaling) to additional_WindNinja_outputs_googleValues variable\n", new_wrf_file_name.c_str(), new_write_wx_model_goog_output,
           new_write_goog_output, new_goog_out_resolution, new_units_goog_out_resolution.c_str(), new_goog_out_color_scheme.c_str(), new_goog_out_vector_scaling);
    additional_WindNinja_outputs_googleValues.push_back({new_wrf_file_name, new_write_wx_model_goog_output, new_write_goog_output, new_goog_out_resolution,
                                                         new_units_goog_out_resolution, new_goog_out_color_scheme, new_goog_out_vector_scaling});
}

void inputVariables_valueStorage::add_additional_WindNinja_outputs_shapefileValues(std::string new_wrf_file_name, bool new_write_wx_model_shapefile_output, bool new_write_shapefile_output,
                                                                                   double new_shape_out_resolution, std::string new_units_shape_out_resolution)
{
    printf("adding additional_WindNinja_outputs_shapefile \"%s %d %d %f %s\" (wrf_file_name write_wx_model_shapefile_output write_shapefile_output shape_out_resolution "
           "units_shape_out_resolution) to additional_WindNinja_outputs_shapefileValues variable\n",new_wrf_file_name.c_str(), new_write_wx_model_shapefile_output,
           new_write_shapefile_output, new_shape_out_resolution, new_units_shape_out_resolution.c_str());
    additional_WindNinja_outputs_shapefileValues.push_back({new_wrf_file_name, new_write_wx_model_shapefile_output, new_write_shapefile_output, new_shape_out_resolution,
                                                            new_units_shape_out_resolution});
}

void inputVariables_valueStorage::add_additional_WindNinja_outputs_pdfValues(std::string new_wrf_file_name, bool new_write_pdf_output, double new_pdf_out_resolution,
                                                                             std::string new_units_pdf_out_resolution, double new_pdf_linewidth, std::string new_pdf_basemap,
                                                                             double new_pdf_height, double new_pdf_width, std::string new_pdf_size)
{
    printf("adding additional_WindNinja_outputs_pdf \"%s %d %f %s %f %s %f %f %s\" (wrf_file_name write_pdf_output pdf_out_resolution units_pdf_out_resolution pdf_linewidth "
           "pdf_basemap pdf_height pdf_width pdf_size) to additional_WindNinja_outputs_pdfValues variable\n",new_wrf_file_name.c_str(), new_write_pdf_output, new_pdf_out_resolution,
           new_units_pdf_out_resolution.c_str(), new_pdf_linewidth, new_pdf_basemap.c_str(), new_pdf_height, new_pdf_width, new_pdf_size.c_str());
    additional_WindNinja_outputs_pdfValues.push_back({new_wrf_file_name, new_write_pdf_output, new_pdf_out_resolution, new_units_pdf_out_resolution, new_pdf_linewidth,
                                                      new_pdf_basemap, new_pdf_height, new_pdf_width, new_pdf_size});
}
/*** end load function stuff for count type variables ***/

/*** get variable value functions ***/
bool inputVariables_valueStorage::get_inputVariableBoolValue(std::string varName)
{
    bool outputValue = defaultBoolValue;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < boolValues.size(); varIdx++)
    {
        if(boolValues[varIdx].variableName == varName)
        {
            outputValue = boolValues[varIdx].variableValue;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not get bool value for variable name \"%s\", variableName \%s\" does not exist in bool value storage!\n",varName.c_str(),varName.c_str());
        exit(1);
    }

    return outputValue;
}

size_t inputVariables_valueStorage::get_inputVariableSize_tValue(std::string varName)
{
    size_t outputValue = defaultSize_tValue;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < size_tValues.size(); varIdx++)
    {
        if(size_tValues[varIdx].variableName == varName)
        {
            outputValue = size_tValues[varIdx].variableValue;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not get size_t value for variable name \"%s\", variableName \%s\" does not exist in size_t value storage!\n",varName.c_str(),varName.c_str());
        exit(1);
    }

    return outputValue;
}

int inputVariables_valueStorage::get_inputVariableIntValue(std::string varName)
{
    int outputValue = defaultIntValue;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < intValues.size(); varIdx++)
    {
        if(intValues[varIdx].variableName == varName)
        {
            outputValue = intValues[varIdx].variableValue;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not get int value for variable name \"%s\", variableName \%s\" does not exist in int value storage!\n",varName.c_str(),varName.c_str());
        exit(1);
    }

    return outputValue;
}

double inputVariables_valueStorage::get_inputVariableDoubleValue(std::string varName)
{
    double outputValue = defaultDoubleValue;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < doubleValues.size(); varIdx++)
    {
        if(doubleValues[varIdx].variableName == varName)
        {
            outputValue = doubleValues[varIdx].variableValue;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not get double value for variable name \"%s\", variableName \%s\" does not exist in double value storage!\n",varName.c_str(),varName.c_str());
        exit(1);
    }

    return outputValue;
}

std::string inputVariables_valueStorage::get_inputVariableStringValue(std::string varName)
{
    std::string outputValue = defaultStringValue;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < stringValues.size(); varIdx++)
    {
        if(stringValues[varIdx].variableName == varName)
        {
            outputValue = stringValues[varIdx].variableValue;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not get string value for variable name \"%s\", variableName \%s\" does not exist in string value storage!\n",varName.c_str(),varName.c_str());
        printf("possible values are:\n");
        for(size_t varIdx = 0; varIdx < stringValues.size(); varIdx++)
        {
            printf("\"%s\"\n",stringValues[varIdx].variableName.c_str());
        }
        exit(1);
    }

    return outputValue;
}

std::string inputVariables_valueStorage::get_inputVariablePathnameValue(std::string varName)
{
    std::string outputValue = defaultPathnameValue;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < pathnameValues.size(); varIdx++)
    {
        if(pathnameValues[varIdx].variableName == varName)
        {
            outputValue = pathnameValues[varIdx].variableValue;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not get pathname value for variable name \"%s\", variableName \%s\" does not exist in pathname value storage!\n",varName.c_str(),varName.c_str());
        exit(1);
    }

    return outputValue;
}

std::string inputVariables_valueStorage::get_inputVariableFilenameValue(std::string varName)
{
    std::string outputValue = defaultFilenameValue;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < filenameValues.size(); varIdx++)
    {
        if(filenameValues[varIdx].variableName == varName)
        {
            outputValue = filenameValues[varIdx].variableValue;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not get filename value for variable name \"%s\", variableName \%s\" does not exist in filename value storage!\n",varName.c_str(),varName.c_str());
        exit(1);
    }

    return outputValue;
}

dateValue inputVariables_valueStorage::get_inputVariableDateValue(std::string varName)
{
    dateValue outputValue = {0,defaultYearValue,defaultMonthValue,defaultDayValue,defaultHourValue,defaultMinuteValue};

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < dateValues.size(); varIdx++)
    {
        if(dateValues[varIdx].variableName == varName)
        {
            outputValue = dateValues[varIdx];
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not get date value for variable name \"%s\", variableName \%s\" does not exist in date value storage!\n",varName.c_str(),varName.c_str());
        exit(1);
    }

    return outputValue;
}

double inputVariables_valueStorage::get_inputVariableLat_Long_PointValueLat_Coord(std::string varName)
{
    double outputValue = defaultLatValue;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < lat_long_pointValues.size(); varIdx++)
    {
        if(lat_long_pointValues[varIdx].variableName == varName)
        {
            outputValue = lat_long_pointValues[varIdx].lat_coord;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not get lat_long_point lat_coord value for variable name \"%s\", variableName \%s\" does not exist in lat_long_point value storage!\n",varName.c_str(),varName.c_str());
        exit(1);
    }

    return outputValue;
}

double inputVariables_valueStorage::get_inputVariableLat_Long_PointValueLong_Coord(std::string varName)
{
    double outputValue = defaultLongValue;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < lat_long_pointValues.size(); varIdx++)
    {
        if(lat_long_pointValues[varIdx].variableName == varName)
        {
            outputValue = lat_long_pointValues[varIdx].long_coord;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not get lat_long_point long_coord value for variable name \"%s\", variableName \%s\" does not exist in lat_long_point value storage!\n",varName.c_str(),varName.c_str());
        exit(1);
    }

    return outputValue;
}

int inputVariables_valueStorage::get_inputVariableDateValueYear(std::string varName)
{
    int outputValue = defaultYearValue;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < dateValues.size(); varIdx++)
    {
        if(dateValues[varIdx].variableName == varName)
        {
            outputValue = dateValues[varIdx].year;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not get date year value for variable name \"%s\", variableName \%s\" does not exist in date value storage!\n",varName.c_str(),varName.c_str());
        exit(1);
    }

    return outputValue;
}

int inputVariables_valueStorage::get_inputVariableDateValueMonth(std::string varName)
{
    int outputValue = defaultMonthValue;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < dateValues.size(); varIdx++)
    {
        if(dateValues[varIdx].variableName == varName)
        {
            outputValue = dateValues[varIdx].month;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not get date month value for variable name \"%s\", variableName \%s\" does not exist in date value storage!\n",varName.c_str(),varName.c_str());
        exit(1);
    }

    return outputValue;
}

int inputVariables_valueStorage::get_inputVariableDateValueDay(std::string varName)
{
    int outputValue = defaultDayValue;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < dateValues.size(); varIdx++)
    {
        if(dateValues[varIdx].variableName == varName)
        {
            outputValue = dateValues[varIdx].day;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not get date day value for variable name \"%s\", variableName \%s\" does not exist in date value storage!\n",varName.c_str(),varName.c_str());
        exit(1);
    }

    return outputValue;
}

int inputVariables_valueStorage::get_inputVariableDateValueHour(std::string varName)
{
    int outputValue = defaultHourValue;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < dateValues.size(); varIdx++)
    {
        if(dateValues[varIdx].variableName == varName)
        {
            outputValue = dateValues[varIdx].hour;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not get date hour value for variable name \"%s\", variableName \%s\" does not exist in date value storage!\n",varName.c_str(),varName.c_str());
        exit(1);
    }

    return outputValue;
}

int inputVariables_valueStorage::get_inputVariableDateValueMinute(std::string varName)
{
    int outputValue = defaultMinuteValue;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < dateValues.size(); varIdx++)
    {
        if(dateValues[varIdx].variableName == varName)
        {
            outputValue = dateValues[varIdx].minute;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not get date minute value for variable name \"%s\", variableName \%s\" does not exist in date value storage!\n",varName.c_str(),varName.c_str());
        exit(1);
    }

    return outputValue;
}

int inputVariables_valueStorage::get_inputVariableHour_MinValueHour(std::string varName)
{
    int outputValue = defaultHourValue;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < hour_minValues.size(); varIdx++)
    {
        if(hour_minValues[varIdx].variableName == varName)
        {
            outputValue = hour_minValues[varIdx].hour;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not get hour_min hour value for variable name \"%s\", variableName \%s\" does not exist in hour_min value storage!\n",varName.c_str(),varName.c_str());
        exit(1);
    }

    return outputValue;
}

int inputVariables_valueStorage::get_inputVariableHour_MinValueMinute(std::string varName)
{
    int outputValue = defaultMinuteValue;

    bool foundVarName = false;
    for(size_t varIdx = 0; varIdx < hour_minValues.size(); varIdx++)
    {
        if(hour_minValues[varIdx].variableName == varName)
        {
            outputValue = hour_minValues[varIdx].minute;
            foundVarName = true;
            break;
        }
    }

    if(foundVarName == false)
    {
        printf("could not get hour_min minute value for variable name \"%s\", variableName \%s\" does not exist in hour_min value storage!\n",varName.c_str(),varName.c_str());
        exit(1);
    }

    return outputValue;
}
/*** end get variable value functions for single values ***/

/*** get variable value functions for count values ***/
size_t inputVariables_valueStorage::getTypeCount_CountVar(std::string varName,std::string loaderFunction)
{
    size_t outputValue = defaultSize_tValue;

    bool foundVarName = true;
    if(loaderFunction == "load_create_ignition_from_latlongs")
    {
        outputValue = create_ignition_from_latlongs_count;
    } else if(loaderFunction == "load_polygon_ignit_shape_files")
    {
        outputValue = polygon_ignit_shape_files_count;
    } else if(loaderFunction == "load_GeoMAC_fire_perimeter_files")
    {
        outputValue = GeoMAC_fire_perimeter_files_count;
    } else if(loaderFunction == "load_farsite_output_fire_perimeter_files")
    {
        outputValue = farsite_output_fire_perimeter_files_count;
    } else if(loaderFunction == "load_wrf_files")
    {
        outputValue = wrf_files_count;
    } else if(loaderFunction == "load_additional_WindNinja_outputs_google")
    {
        outputValue = additional_WindNinja_outputs_google_count;
    } else if(loaderFunction == "load_additional_WindNinja_outputs_shapefile")
    {
        outputValue = additional_WindNinja_outputs_shapefile_count;
    } else if(loaderFunction == "load_additional_WindNinja_outputs_pdf")
    {
        outputValue = additional_WindNinja_outputs_pdf_count;
    } else
    {
        printf("loader function stuff for variable \"%s\" loader function \"%s\" has not been implemented in the code yet!\n",varName.c_str(),loaderFunction.c_str());
        foundVarName = false;
        exit(1);
    }

    return outputValue;
}

std::vector<lat_long_pointValue> inputVariables_valueStorage::get_create_ignition_from_latlongs()
{
    return create_ignition_from_latlongs;
}

std::vector<std::string> inputVariables_valueStorage::get_polygon_ignit_shape_files()
{
    return polygon_ignit_shape_files;
}

std::vector<std::string> inputVariables_valueStorage::get_GeoMAC_fire_perimeter_files()
{
    return GeoMAC_fire_perimeter_files;
}

std::vector<std::string> inputVariables_valueStorage::get_farsite_output_fire_perimeter_files()
{
    return farsite_output_fire_perimeter_files;
}

std::vector<std::string> inputVariables_valueStorage::get_wrf_files()
{
    return wrf_files;
}

/* additional_WindNinja_outputs_google get functions */
std::vector<std::string> inputVariables_valueStorage::get_additional_WindNinja_outputs_googleValues_wrf_file_names()
{
    std::vector<std::string> outputValues;
    for(size_t fileIdx = 0; fileIdx < additional_WindNinja_outputs_googleValues.size(); fileIdx++)
    {
        outputValues.push_back(additional_WindNinja_outputs_googleValues[fileIdx].wrf_file_name);
    }
    return outputValues;
}

bool inputVariables_valueStorage::get_write_wx_model_goog_output(std::string wrf_file_name)
{
    bool outputValue = defaultBoolValue;

    bool foundVar = false;
    for(size_t fileIdx = 0; fileIdx < additional_WindNinja_outputs_googleValues.size(); fileIdx++)
    {
        if(additional_WindNinja_outputs_googleValues[fileIdx].wrf_file_name == wrf_file_name)
        {
            outputValue = additional_WindNinja_outputs_googleValues[fileIdx].write_wx_model_goog_output;
            foundVar = true;
            break;
        }
    }

    if(foundVar == false)
    {
        printf("wrf_file_name \"%s\" does not have a stored write_wx_model_goog_output value!\n",wrf_file_name.c_str());
        exit(1);
    }
}

bool inputVariables_valueStorage::get_write_goog_output(std::string wrf_file_name)
{
    bool outputValue = defaultBoolValue;

    bool foundVar = false;
    for(size_t fileIdx = 0; fileIdx < additional_WindNinja_outputs_googleValues.size(); fileIdx++)
    {
        if(additional_WindNinja_outputs_googleValues[fileIdx].wrf_file_name == wrf_file_name)
        {
            outputValue = additional_WindNinja_outputs_googleValues[fileIdx].write_goog_output;
            foundVar = true;
            break;
        }
    }

    if(foundVar == false)
    {
        printf("wrf_file_name \"%s\" does not have a stored write_goog_output value!\n",wrf_file_name.c_str());
        exit(1);
    }
}

double inputVariables_valueStorage::get_goog_out_resolution(std::string wrf_file_name)
{
    double outputValue = defaultDoubleValue;

    bool foundVar = false;
    for(size_t fileIdx = 0; fileIdx < additional_WindNinja_outputs_googleValues.size(); fileIdx++)
    {
        if(additional_WindNinja_outputs_googleValues[fileIdx].wrf_file_name == wrf_file_name)
        {
            outputValue = additional_WindNinja_outputs_googleValues[fileIdx].goog_out_resolution;
            foundVar = true;
            break;
        }
    }

    if(foundVar == false)
    {
        printf("wrf_file_name \"%s\" does not have a stored goog_out_resolution value!\n",wrf_file_name.c_str());
        exit(1);
    }
}

std::string inputVariables_valueStorage::get_units_goog_out_resolution(std::string wrf_file_name)
{
    std::string outputValue = defaultStringValue;

    bool foundVar = false;
    for(size_t fileIdx = 0; fileIdx < additional_WindNinja_outputs_googleValues.size(); fileIdx++)
    {
        if(additional_WindNinja_outputs_googleValues[fileIdx].wrf_file_name == wrf_file_name)
        {
            outputValue = additional_WindNinja_outputs_googleValues[fileIdx].units_goog_out_resolution;
            foundVar = true;
            break;
        }
    }

    if(foundVar == false)
    {
        printf("wrf_file_name \"%s\" does not have a stored units_goog_out_resolution value!\n",wrf_file_name.c_str());
        exit(1);
    }
}

std::string inputVariables_valueStorage::get_goog_out_color_scheme(std::string wrf_file_name)
{
    std::string outputValue = defaultStringValue;

    bool foundVar = false;
    for(size_t fileIdx = 0; fileIdx < additional_WindNinja_outputs_googleValues.size(); fileIdx++)
    {
        if(additional_WindNinja_outputs_googleValues[fileIdx].wrf_file_name == wrf_file_name)
        {
            outputValue = additional_WindNinja_outputs_googleValues[fileIdx].goog_out_color_scheme;
            foundVar = true;
            break;
        }
    }

    if(foundVar == false)
    {
        printf("wrf_file_name \"%s\" does not have a stored goog_out_color_scheme value!\n",wrf_file_name.c_str());
        exit(1);
    }
}

bool inputVariables_valueStorage::get_goog_out_vector_scaling(std::string wrf_file_name)
{
    bool outputValue = defaultBoolValue;

    bool foundVar = false;
    for(size_t fileIdx = 0; fileIdx < additional_WindNinja_outputs_googleValues.size(); fileIdx++)
    {
        if(additional_WindNinja_outputs_googleValues[fileIdx].wrf_file_name == wrf_file_name)
        {
            outputValue = additional_WindNinja_outputs_googleValues[fileIdx].goog_out_vector_scaling;
            foundVar = true;
            break;
        }
    }

    if(foundVar == false)
    {
        printf("wrf_file_name \"%s\" does not have a stored goog_out_vector_scaling value!\n",wrf_file_name.c_str());
        exit(1);
    }
}
/* end additional_WindNinja_outputs_google get functions */

/* additional_WindNinja_outputs_shapefile get functions */
std::vector<std::string> inputVariables_valueStorage::get_additional_WindNinja_outputs_shapefileValues_wrf_file_names()
{
    std::vector<std::string> outputValues;
    for(size_t fileIdx = 0; fileIdx < additional_WindNinja_outputs_shapefileValues.size(); fileIdx++)
    {
        outputValues.push_back(additional_WindNinja_outputs_shapefileValues[fileIdx].wrf_file_name);
    }
    return outputValues;
}

bool inputVariables_valueStorage::get_write_wx_model_shapefile_output(std::string wrf_file_name)
{
    bool outputValue = defaultBoolValue;

    bool foundVar = false;
    for(size_t fileIdx = 0; fileIdx < additional_WindNinja_outputs_shapefileValues.size(); fileIdx++)
    {
        if(additional_WindNinja_outputs_shapefileValues[fileIdx].wrf_file_name == wrf_file_name)
        {
            outputValue = additional_WindNinja_outputs_shapefileValues[fileIdx].write_wx_model_shapefile_output;
            foundVar = true;
            break;
        }
    }

    if(foundVar == false)
    {
        printf("wrf_file_name \"%s\" does not have a stored write_wx_model_shapefile_output value!\n",wrf_file_name.c_str());
        exit(1);
    }
}

bool inputVariables_valueStorage::get_write_shapefile_output(std::string wrf_file_name)
{
    bool outputValue = defaultBoolValue;

    bool foundVar = false;
    for(size_t fileIdx = 0; fileIdx < additional_WindNinja_outputs_shapefileValues.size(); fileIdx++)
    {
        if(additional_WindNinja_outputs_shapefileValues[fileIdx].wrf_file_name == wrf_file_name)
        {
            outputValue = additional_WindNinja_outputs_shapefileValues[fileIdx].write_shapefile_output;
            foundVar = true;
            break;
        }
    }

    if(foundVar == false)
    {
        printf("wrf_file_name \"%s\" does not have a stored write_shapefile_output value!\n",wrf_file_name.c_str());
        exit(1);
    }
}

double inputVariables_valueStorage::get_shape_out_resolution(std::string wrf_file_name)
{
    double outputValue = defaultDoubleValue;

    bool foundVar = false;
    for(size_t fileIdx = 0; fileIdx < additional_WindNinja_outputs_shapefileValues.size(); fileIdx++)
    {
        if(additional_WindNinja_outputs_shapefileValues[fileIdx].wrf_file_name == wrf_file_name)
        {
            outputValue = additional_WindNinja_outputs_shapefileValues[fileIdx].shape_out_resolution;
            foundVar = true;
            break;
        }
    }

    if(foundVar == false)
    {
        printf("wrf_file_name \"%s\" does not have a stored shape_out_resolution value!\n",wrf_file_name.c_str());
        exit(1);
    }
}

std::string inputVariables_valueStorage::get_units_shape_out_resolution(std::string wrf_file_name)
{
    std::string outputValue = defaultStringValue;

    bool foundVar = false;
    for(size_t fileIdx = 0; fileIdx < additional_WindNinja_outputs_shapefileValues.size(); fileIdx++)
    {
        if(additional_WindNinja_outputs_shapefileValues[fileIdx].wrf_file_name == wrf_file_name)
        {
            outputValue = additional_WindNinja_outputs_shapefileValues[fileIdx].units_shape_out_resolution;
            foundVar = true;
            break;
        }
    }

    if(foundVar == false)
    {
        printf("wrf_file_name \"%s\" does not have a stored units_shape_out_resolution value!\n",wrf_file_name.c_str());
        exit(1);
    }
}
/* end additional_WindNinja_outputs_shapefile get functions */

/* additional_WindNinja_outputs_pdf get functions */
std::vector<std::string> inputVariables_valueStorage::get_additional_WindNinja_outputs_pdfValues_wrf_file_names()
{
    std::vector<std::string> outputValues;
    for(size_t fileIdx = 0; fileIdx < additional_WindNinja_outputs_pdfValues.size(); fileIdx++)
    {
        outputValues.push_back(additional_WindNinja_outputs_pdfValues[fileIdx].wrf_file_name);
    }
    return outputValues;
}

bool inputVariables_valueStorage::get_write_pdf_output(std::string wrf_file_name)
{
    bool outputValue = defaultBoolValue;

    bool foundVar = false;
    for(size_t fileIdx = 0; fileIdx < additional_WindNinja_outputs_pdfValues.size(); fileIdx++)
    {
        if(additional_WindNinja_outputs_pdfValues[fileIdx].wrf_file_name == wrf_file_name)
        {
            outputValue = additional_WindNinja_outputs_pdfValues[fileIdx].write_pdf_output;
            foundVar = true;
            break;
        }
    }

    if(foundVar == false)
    {
        printf("wrf_file_name \"%s\" does not have a stored write_pdf_output value!\n",wrf_file_name.c_str());
        exit(1);
    }
}

double inputVariables_valueStorage::get_pdf_out_resolution(std::string wrf_file_name)
{
    double outputValue = defaultDoubleValue;

    bool foundVar = false;
    for(size_t fileIdx = 0; fileIdx < additional_WindNinja_outputs_pdfValues.size(); fileIdx++)
    {
        if(additional_WindNinja_outputs_pdfValues[fileIdx].wrf_file_name == wrf_file_name)
        {
            outputValue = additional_WindNinja_outputs_pdfValues[fileIdx].pdf_out_resolution;
            foundVar = true;
            break;
        }
    }

    if(foundVar == false)
    {
        printf("wrf_file_name \"%s\" does not have a stored pdf_out_resolution value!\n",wrf_file_name.c_str());
        exit(1);
    }
}

std::string inputVariables_valueStorage::get_units_pdf_out_resolution(std::string wrf_file_name)
{
    std::string outputValue = defaultStringValue;

    bool foundVar = false;
    for(size_t fileIdx = 0; fileIdx < additional_WindNinja_outputs_pdfValues.size(); fileIdx++)
    {
        if(additional_WindNinja_outputs_pdfValues[fileIdx].wrf_file_name == wrf_file_name)
        {
            outputValue = additional_WindNinja_outputs_pdfValues[fileIdx].units_pdf_out_resolution;
            foundVar = true;
            break;
        }
    }

    if(foundVar == false)
    {
        printf("wrf_file_name \"%s\" does not have a stored units_pdf_out_resolution value!\n",wrf_file_name.c_str());
        exit(1);
    }
}

double inputVariables_valueStorage::get_pdf_linewidth(std::string wrf_file_name)
{
    double outputValue = defaultDoubleValue;

    bool foundVar = false;
    for(size_t fileIdx = 0; fileIdx < additional_WindNinja_outputs_pdfValues.size(); fileIdx++)
    {
        if(additional_WindNinja_outputs_pdfValues[fileIdx].wrf_file_name == wrf_file_name)
        {
            outputValue = additional_WindNinja_outputs_pdfValues[fileIdx].pdf_linewidth;
            foundVar = true;
            break;
        }
    }

    if(foundVar == false)
    {
        printf("wrf_file_name \"%s\" does not have a stored pdf_linewidth value!\n",wrf_file_name.c_str());
        exit(1);
    }
}

std::string inputVariables_valueStorage::get_pdf_basemap(std::string wrf_file_name)
{
    std::string outputValue = defaultStringValue;

    bool foundVar = false;
    for(size_t fileIdx = 0; fileIdx < additional_WindNinja_outputs_pdfValues.size(); fileIdx++)
    {
        if(additional_WindNinja_outputs_pdfValues[fileIdx].wrf_file_name == wrf_file_name)
        {
            outputValue = additional_WindNinja_outputs_pdfValues[fileIdx].pdf_basemap;
            foundVar = true;
            break;
        }
    }

    if(foundVar == false)
    {
        printf("wrf_file_name \"%s\" does not have a stored pdf_basemap value!\n",wrf_file_name.c_str());
        exit(1);
    }
}

double inputVariables_valueStorage::get_pdf_height(std::string wrf_file_name)
{
    double outputValue = defaultDoubleValue;

    bool foundVar = false;
    for(size_t fileIdx = 0; fileIdx < additional_WindNinja_outputs_pdfValues.size(); fileIdx++)
    {
        if(additional_WindNinja_outputs_pdfValues[fileIdx].wrf_file_name == wrf_file_name)
        {
            outputValue = additional_WindNinja_outputs_pdfValues[fileIdx].pdf_height;
            foundVar = true;
            break;
        }
    }

    if(foundVar == false)
    {
        printf("wrf_file_name \"%s\" does not have a stored pdf_height value!\n",wrf_file_name.c_str());
        exit(1);
    }
}

double inputVariables_valueStorage::get_pdf_width(std::string wrf_file_name)
{
    double outputValue = defaultDoubleValue;

    bool foundVar = false;
    for(size_t fileIdx = 0; fileIdx < additional_WindNinja_outputs_pdfValues.size(); fileIdx++)
    {
        if(additional_WindNinja_outputs_pdfValues[fileIdx].wrf_file_name == wrf_file_name)
        {
            outputValue = additional_WindNinja_outputs_pdfValues[fileIdx].pdf_width;
            foundVar = true;
            break;
        }
    }

    if(foundVar == false)
    {
        printf("wrf_file_name \"%s\" does not have a stored pdf_width value!\n",wrf_file_name.c_str());
        exit(1);
    }
}

std::string inputVariables_valueStorage::get_pdf_size(std::string wrf_file_name)
{
    std::string outputValue = defaultStringValue;

    bool foundVar = false;
    for(size_t fileIdx = 0; fileIdx < additional_WindNinja_outputs_pdfValues.size(); fileIdx++)
    {
        if(additional_WindNinja_outputs_pdfValues[fileIdx].wrf_file_name == wrf_file_name)
        {
            outputValue = additional_WindNinja_outputs_pdfValues[fileIdx].pdf_size;
            foundVar = true;
            break;
        }
    }

    if(foundVar == false)
    {
        printf("wrf_file_name \"%s\" does not have a pdf_size value!\n",wrf_file_name.c_str());
        exit(1);
    }
}
/* end additional_WindNinja_outputs_pdf get functions */

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
            boolValues.push_back({inputVariableInfo[varIdx].get_variableName(),defaultBoolValue});
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "size_t")
        {
            size_tValues.push_back({inputVariableInfo[varIdx].get_variableName(),defaultSize_tValue});
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "int")
        {
            intValues.push_back({inputVariableInfo[varIdx].get_variableName(),defaultIntValue});
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "double")
        {
            doubleValues.push_back({inputVariableInfo[varIdx].get_variableName(),defaultDoubleValue});
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "positive double")   // value is still a double, input function will make sure it is positive before allowing storage
        {
            doubleValues.push_back({inputVariableInfo[varIdx].get_variableName(),defaultDoubleValue});
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "signless percent")  // value is still a double, input function will make sure it is signless before allowing storage
        {
            doubleValues.push_back({inputVariableInfo[varIdx].get_variableName(),defaultDoubleValue});
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "string")
        {
            stringValues.push_back({inputVariableInfo[varIdx].get_variableName(),defaultStringValue});
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "pathname")
        {
            pathnameValues.push_back({inputVariableInfo[varIdx].get_variableName(),defaultPathnameValue});
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "lcp filename")  // value is still a filename, input function will make sure it is a valid lcp filename before allowing storage
        {
            filenameValues.push_back({inputVariableInfo[varIdx].get_variableName(),defaultFilenameValue});
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "shape filename")    // value is still a filename, input function will make sure it is a valid shapefile filename before allowing storage
        {
            filenameValues.push_back({inputVariableInfo[varIdx].get_variableName(),defaultFilenameValue});
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "lat_coord")
        {
            doubleValues.push_back({inputVariableInfo[varIdx].get_variableName(),defaultDoubleValue});  // value is still a double, input function will make sure it is between -90 and 90 before allowing storage
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "long_coord")
        {
            doubleValues.push_back({inputVariableInfo[varIdx].get_variableName(),defaultDoubleValue});  // value is still a double, input function will make sure it is between -180 and 180 before allowing storage
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "lat_long_point")
        {
            lat_long_pointValues.push_back({inputVariableInfo[varIdx].get_variableName(),defaultLatValue,defaultLongValue});
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "date")
        {
            dateValues.push_back({inputVariableInfo[varIdx].get_variableName(),defaultYearValue,defaultMonthValue,defaultDayValue,defaultHourValue,defaultMinuteValue});
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "hour_min")
        {
            hour_minValues.push_back({inputVariableInfo[varIdx].get_variableName(),defaultHourValue,defaultMinuteValue});
        } else if(inputVariableInfo[varIdx].get_variableCountType() == "count")
        {
            create_ignition_from_latlongs_count = 0;
            polygon_ignit_shape_files_count = 0;
            GeoMAC_fire_perimeter_files_count = 0;
            farsite_output_fire_perimeter_files_count = 0;
            wrf_files_count = 0;
            additional_WindNinja_outputs_google_count = 0;
            additional_WindNinja_outputs_shapefile_count = 0;
            additional_WindNinja_outputs_pdf_count = 0;
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
    if(set_inputVariableBoolValue("overwrite_past_outputs",false) == false)
    {
        success = false;
    }
    if(set_inputVariableStringValue("createIgnition_output_units","input") == false)
    {
        success = false;
    }
    if(set_inputVariableStringValue("wrfGetWeather_output_units","metric") == false)
    {
        success = false;
    }
    if(set_inputVariableStringValue("WindNinja_required_output_units","metric") == false)
    {
        success = false;
    }
    if(set_inputVariableStringValue("farsite_output_units","english") == false)
    {
        success = false;
    }
    if(set_inputVariableBoolValue("use_native_timezone",false) == false)
    {
        success = false;
    }

        // lcp download variables (WindNinja related)
    if(set_inputVariableBoolValue("automate_lcp_download",true) == false)
    {
        success = false;
    }
    if(set_inputVariableDoubleValue("fireperim_to_lcp_scalefactor",1.0) == false)
    {
        success = false;
    }
    if(set_inputVariableBoolValue("use_past_lcp",false) == false)
    {
        success = false;
    }
    if(set_inputVariableBoolValue("specify_lcp_download",false) == false)
    {
        success = false;
    }

        // createIgnition variables
    if(set_inputVariableDoubleValue("fire_perimeter_widening_factor",1.0) == false)
    {
        success = false;
    }

        // wrfGetWeather and WindNinja variables
    if(set_inputVariableBoolValue("extend_wrf_data",false) == false)
    {
        success = false;
    }

        // WindNinja only variables
    if(set_inputVariableSize_tValue("WindNinja_number_of_threads",8) == false)
    {
        success = false;
    }
    if(set_inputVariableStringValue("WindNinja_mesh_choice","fine") == false)
    {
        success = false;
    }
    if(set_inputVariableStringValue("WindNinja_mesh_res_units","m") == false)
    {
        success = false;
    }
    if(set_inputVariableBoolValue("diurnal_winds",false) == false)
    {
        success = false;
    }
    if(set_inputVariableBoolValue("non_neutral_stability",false) == false)
    {
        success = false;
    }

        // wrfGetWeather only variables
    if(set_inputVariableBoolValue("use_weather_from_ignition_center",false) == false)
    {
        success = false;
    }
    if(set_inputVariableBoolValue("use_weather_from_full_ignition_area",false) == false)
    {
        success = false;
    }
    if(set_inputVariableBoolValue("use_weather_from_wrf_center",true) == false)
    {
        success = false;
    }

        // createFarsiteInput variables
    if(set_inputVariableDoubleValue("farsite_spot_probability",0.05) == false)   // need to be careful that it is a legit signless percent
    {
        success = false;
    }
    if(set_inputVariableSize_tValue("farsite_spot_ignition_delay",0) == false)
    {
        success = false;
    }
    if(set_inputVariableSize_tValue("farsite_spotting_seed",1000) == false)
    {
        success = false;
    }
    if(set_inputVariableHour_MinValue("farsite_earliest_burn_hour",8,00) == false)
    {
        success = false;
    }
    if(set_inputVariableHour_MinValue("farsite_latest_burn_hour",19,59) == false)  // need to be careful that this is later than "farsite_earliest_burn_hour"
    {
        success = false;
    }
    if(set_inputVariableDoubleValue("farsite_foliar_moisture_content",70.0) == false)   // need to be careful that it is a legit signless percent
    {
        success = false;
    }
    if(set_inputVariableStringValue("farsite_crown_fire_method","Finney") == false)   // need to be careful that it is a legit signless percent
    {
        success = false;
    }

        // debugging related variables
    if(set_inputVariableBoolValue("report_largest_ignition_bounds",false) == false)
    {
        success = false;
    }
    if(set_inputVariableBoolValue("report_found_inputs",false) == false)
    {
        success = false;
    }

        // optional WindNinja output settings, may not even add these options
    // thank goodness, none here

    return success;
}
/*** end setup functions ***/
