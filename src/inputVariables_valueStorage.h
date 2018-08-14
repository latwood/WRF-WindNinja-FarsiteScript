#ifndef INPUTVARIABLES_VALUESTORAGE_H
#define INPUTVARIABLES_VALUESTORAGE_H


#include "inputVariables_infoStorage.h"

struct lat_long_pointValue
{
    std::string variableName;
    double lat_coord;
    double long_coord;
};

struct dateValue // yeah, definitely going to either declare all types globally, or just avoid this weird problem of passing strange types between classes. Now I've got the type internal to the class and this external for all classes so I can pass the type on to others. Originally just did one line without defining this type, and was fine till I got to try an output function with this type, then compiler got really really angry. Ugh, declaring this section cleared many more errors, but heck now I've got issues with type conversions between two definitions. Has to be global or nothing, not two copies one global and one in class seems like.
{
    std::string variableName;
    int year;
    int month;
    int day;
    int hour;
    int minute;
};    // might have accidentally placed this into the global scope, might actually be better to have all these data member declarations done this way actually, as then I wouldn't need a second copy of the data storage declaration struct stuff when the values are desired

struct hour_minValue
{
    std::string variableName;
    int hour;
    int minute;
};

struct additional_WindNinja_outputs_googleValue
{
    std::string wrf_file_name;
    bool write_wx_model_goog_output;
    bool write_goog_output;
    double goog_out_resolution;
    std::string units_goog_out_resolution;
    std::string goog_out_color_scheme;
    bool goog_out_vector_scaling;
};

struct additional_WindNinja_outputs_shapefileValue
{
    std::string wrf_file_name;
    bool write_wx_model_shapefile_output;
    bool write_shapefile_output;
    double shape_out_resolution;
    std::string units_shape_out_resolution;
};

struct additional_WindNinja_outputs_pdfValue
{
    std::string wrf_file_name;
    bool write_pdf_output;
    double pdf_out_resolution;
    std::string units_pdf_out_resolution;
    double pdf_linewidth;
    std::string pdf_basemap;
    double pdf_height;
    double pdf_width;
    std::string pdf_size;
};

class inputVariables_valueStorage
{
public:

    // constructors
    inputVariables_valueStorage();

    // reinit functions (like constructors)
    bool resetVariables();     //kind of a reusable constructor/destructor

    // get variable info functions
    std::vector<inputVariable_info> get_inputVariableInfo();
    std::string get_maxVarNameColumnWhitespace();

    // set variable value functions
    bool set_inputVariableBoolValue(std::string varName, bool newBoolValue);
    bool set_inputVariableSize_tValue(std::string varName, size_t newSize_tValue);
    bool set_inputVariableIntValue(std::string varName, int newIntValue);
    bool set_inputVariableDoubleValue(std::string varName, double newDoubleValue);
    bool set_inputVariableStringValue(std::string varName, std::string newStringValue);
    bool set_inputVariablePathnameValue(std::string varName, std::string newPathnameValue);
    bool set_inputVariableFilenameValue(std::string varName, std::string newFilenameValue);
    bool set_inputVariableLat_Long_PointValue(std::string varName, double newLat_CoordValue, double newLong_CoordValue);
    bool set_inputVariableDateValue(std::string varName, int newYearValue, int newMonthValue, int newDayValue, int newHourValue, int newMinuteValue);    // something like this, though notice that sometimes year isn't included, so maybe have a -999 value to warn it isn't actually there
    bool set_inputVariableHour_MinValue(std::string varName, int newHourValue, int newMinuteValue);
    // bool set_inputVariableCountValues(std::string varName, needed data);     // I guess it will need a separate function for each required data, probably can skip varName, the loader function in the input parser class will have to know which variable it is setting it to based off of the varName, won't necessarily need it here
    bool setTypeCount_CountVar(std::string varName,std::string loaderFunction,size_t newVarCountValue);

    // load function stuff for count type variables
    void add_create_ignition_from_latlongs_coord(std::string newVarName, double newLat_CoordValue, double newLong_CoordValue);
    void add_polygon_ignit_shape_file(std::string new_polygon_ignit_shape_file);
    void add_GeoMAC_fire_perimeter_file(std::string new_GeoMAC_fire_perimeter_file);
    void add_farsite_output_fire_perimeter_file(std::string new_farsite_output_fire_perimeter_file);
    void add_wrf_file(std::string new_wrf_file);
    void add_additional_WindNinja_outputs_googleValues(std::string new_wrf_file_name, bool new_write_wx_model_goog_output, bool new_write_goog_output, double new_goog_out_resolution,
                                                       std::string new_units_goog_out_resolution, std::string new_goog_out_color_scheme, bool new_goog_out_vector_scaling);
    void add_additional_WindNinja_outputs_shapefileValues(std::string new_wrf_file_name, bool new_write_wx_model_shapefile_output, bool new_write_shapefile_output,
                                                          double new_shape_out_resolution, std::string new_units_shape_out_resolution);
    void add_additional_WindNinja_outputs_pdfValues(std::string new_wrf_file_name, bool new_write_pdf_output, double new_pdf_out_resolution, std::string new_units_pdf_out_resolution,
                                                    double new_pdf_linewidth, std::string new_pdf_basemap, double new_pdf_height, double new_pdf_width, std::string new_pdf_size);

    // get variable value functions for single values
    bool get_inputVariableBoolValue(std::string varName);
    size_t get_inputVariableSize_tValue(std::string varName);
    int get_inputVariableIntValue(std::string varName);
    double get_inputVariableDoubleValue(std::string varName);
    std::string get_inputVariableStringValue(std::string varName);
    std::string get_inputVariablePathnameValue(std::string varName);
    std::string get_inputVariableFilenameValue(std::string varName);
    double get_inputVariableLat_Long_PointValueLat_Coord(std::string varName);
    double get_inputVariableLat_Long_PointValueLong_Coord(std::string varName);
    dateValue get_inputVariableDateValue(std::string varName);
    int get_inputVariableDateValueYear(std::string varName);
    int get_inputVariableDateValueMonth(std::string varName);
    int get_inputVariableDateValueDay(std::string varName);
    int get_inputVariableDateValueHour(std::string varName);
    int get_inputVariableDateValueMinute(std::string varName);
    int get_inputVariableHour_MinValueHour(std::string varName);
    int get_inputVariableHour_MinValueMinute(std::string varName);

    // get variable value functions for count values
    // datatype get_inputVariableCountValues(std::string varName); // definitely don't think this kind of generic type idea will work for counts, they are way too complex! Need separate getter for each type of count variable, maybe even multiple getters for said variables if the function that uses them doesn't want to define something to unravel all the values out
    size_t getTypeCount_CountVar(std::string varName,std::string loaderFunction);
    std::vector<lat_long_pointValue> get_create_ignition_from_latlongs();
    std::vector<std::string> get_polygon_ignit_shape_files();
    std::vector<std::string> get_GeoMAC_fire_perimeter_files();
    std::vector<std::string> get_farsite_output_fire_perimeter_files();
    std::vector<std::string> get_wrf_files();
    // additional_WindNinja_outputs_google get functions
    std::vector<std::string> get_additional_WindNinja_outputs_googleValues_wrf_file_names();
    bool get_write_wx_model_goog_output(std::string wrf_file_name);
    bool get_write_goog_output(std::string wrf_file_name);
    double get_goog_out_resolution(std::string wrf_file_name);
    std::string get_units_goog_out_resolution(std::string wrf_file_name);
    std::string get_goog_out_color_scheme(std::string wrf_file_name);
    bool get_goog_out_vector_scaling(std::string wrf_file_name);
    // additional_WindNinja_outputs_shapefile get functions
    std::vector<std::string> get_additional_WindNinja_outputs_shapefileValues_wrf_file_names();
    bool get_write_wx_model_shapefile_output(std::string wrf_file_name);
    bool get_write_shapefile_output(std::string wrf_file_name);
    double get_shape_out_resolution(std::string wrf_file_name);
    std::string get_units_shape_out_resolution(std::string wrf_file_name);
    // additional_WindNinja_outputs_pdf get functions
    std::vector<std::string> get_additional_WindNinja_outputs_pdfValues_wrf_file_names();
    bool get_write_pdf_output(std::string wrf_file_name);
    double get_pdf_out_resolution(std::string wrf_file_name);
    std::string get_units_pdf_out_resolution(std::string wrf_file_name);
    double get_pdf_linewidth(std::string wrf_file_name);
    std::string get_pdf_basemap(std::string wrf_file_name);
    double get_pdf_height(std::string wrf_file_name);
    double get_pdf_width(std::string wrf_file_name);
    std::string get_pdf_size(std::string wrf_file_name);

private:

    // setup functions
    bool sortInputVariableInfo();
    bool setSpecializedDefaults();

    // class data members
    inputVariables_infoStorage setupInputVariableInfo;
    std::vector<inputVariable_info> inputVariableInfo;

    // boolean variable value struct and vector of this struct
    struct boolValue
    {
        std::string variableName;
        bool variableValue;
    };
    bool defaultBoolValue = false;
    std::vector<boolValue> boolValues;

    // size_t variable value struct and vector of this struct
    struct size_tValue
    {
        std::string variableName;
        size_t variableValue;
    };
    size_t defaultSize_tValue = 0;
    std::vector<size_tValue> size_tValues;

    // int variable value struct and vector of this struct
    struct intValue
    {
        std::string variableName;
        int variableValue;
    };
    int defaultIntValue = 0;
    std::vector<intValue> intValues;

    // double variable value struct and vector of this struct
    struct doubleValue
    {
        std::string variableName;
        double variableValue;
    };
    double defaultDoubleValue = 0.0;
    std::vector<doubleValue> doubleValues;

    // string variable value struct and vector of this struct
    struct stringValue
    {
        std::string variableName;
        std::string variableValue;
    };
    std::string defaultStringValue = "";
    std::vector<stringValue> stringValues;

    // pathname variable value struct and vector of this struct
    struct pathnameValue
    {
        std::string variableName;
        std::string variableValue;
    };
    std::string defaultPathnameValue;
    std::vector<pathnameValue> pathnameValues;

    // filename variable value struct and vector of this struct
    struct filenameValue
    {
        std::string variableName;
        std::string variableValue;
    };
    std::string defaultFilenameValue = "";
    std::vector<filenameValue> filenameValues;

    // lat_long_point value struct and vector of this struct
    double defaultLatValue = 0.0;
    double defaultLongValue = 0.0;
    std::vector<lat_long_pointValue> lat_long_pointValues;

    // date variable value struct and vector of this struct
    // commented out this type because while it is great when used locally, trying to use it for get functions causes tons of obnoxious compiler errors
    // basically it says type is not completely defined and splits between the global and class type. If you copy this define twice,
    // it then complains about type conversion between the two. So I guess you have either in class or global, not both, of a given type.
    // So not easy to setup special self declared pass types between classes, unless type is defined globally.
    // So get more inventive, split this type getter function into multiple getter functions for each part of the type!!!
    /*struct dateValue
    {
        std::string variableName;
        int year;
        int month;
        int day;
        int hour;
        int minute;
    };*/
    int defaultYearValue = 0;
    int defaultMonthValue = 0;
    int defaultDayValue = 0;
    int defaultHourValue = 0;
    int defaultMinuteValue = 0;
    std::vector<dateValue> dateValues;

    // hour_min variable value struct and vector of this struct
    std::vector<hour_minValue> hour_minValues;


    // variable value of type count data members, all kinds of data types from vectors to structs
    // I tried to keep variable value of type count data member names to be the same as the variable info variable names
    size_t create_ignition_from_latlongs_count;
    std::vector<lat_long_pointValue> create_ignition_from_latlongs;

    size_t polygon_ignit_shape_files_count;
    std::vector<std::string> polygon_ignit_shape_files;

    size_t GeoMAC_fire_perimeter_files_count;
    std::vector<std::string> GeoMAC_fire_perimeter_files;

    size_t farsite_output_fire_perimeter_files_count;
    std::vector<std::string> farsite_output_fire_perimeter_files;

    size_t wrf_files_count;
    std::vector<std::string> wrf_files;

    size_t additional_WindNinja_outputs_google_count;
    std::vector<additional_WindNinja_outputs_googleValue> additional_WindNinja_outputs_googleValues;

    size_t additional_WindNinja_outputs_shapefile_count;
    std::vector<additional_WindNinja_outputs_shapefileValue> additional_WindNinja_outputs_shapefileValues;

    size_t additional_WindNinja_outputs_pdf_count;
    std::vector<additional_WindNinja_outputs_pdfValue> additional_WindNinja_outputs_pdfValues;

};

#endif // INPUTVARIABLES_VALUESTORAGE_H
