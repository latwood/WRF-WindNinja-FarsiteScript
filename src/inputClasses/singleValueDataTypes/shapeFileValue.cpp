#include "shapeFileValue.h"

/***** public functions *****/

/*** constructor functions ***/
shapeFileValue::shapeFileValue(std::string newVariableName)
{
    variableName = newVariableName;
    storedShapeFileValue = defaultShapeFileValue;
}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
void shapeFileValue::reset()
{
    storedShapeFileValue = defaultShapeFileValue;
}
/*** end reconstructor like functions ***/

/*** set value functions ***/
bool shapeFileValue::set_storedShapeFileValue(std::string newFilenameValue)
{
    bool conversionSuccess = isValidShapeFilename(newFilenameValue,false);
    if(conversionSuccess == false)
    {
        printf("string value \"%s\" is not a valid shape filename!\n",newFilenameValue.c_str());
    } else
    {
        storedShapeFileValue = newFilenameValue;
    }

    return conversionSuccess;
}
/*** end set value functions ***/

/*** get value functions ***/
std::string shapeFileValue::get_variableName()
{
    return variableName;
}

std::string shapeFileValue::get_storedShapeFileValue()
{
    return storedShapeFileValue;
}
/*** end get value functions ***/



/***** private functions *****/



/*** typechecker functions ***/
// still need to finish editing this function
bool shapeFileValue::isValidShapeFilename(std::string inputString, bool suppressWarnings)
{
    bool conversionSuccess = true;

    //removeLeadingWhitespace(inputString);   // just in case
    //removeEndingWhitespace(inputString);    // just in case

    if(inputString.length() < 5)
    {
        if(suppressWarnings == false)
        {
            printf("shape filename not long enough to have .shp extension!\n");
        }
        conversionSuccess = false;
    }
    if(conversionSuccess == true)
    {
        if(inputString.substr(inputString.length()-4,inputString.length()) != ".shp")
        {
            if(suppressWarnings == false)
            {
                printf("shape filename does not have .shp extension!\n");
            }
            conversionSuccess = false;
        }
    }
    if(conversionSuccess == true)
    {
        GDALDatasetH  hDS;
        hDS = GDALOpenEx( inputString.c_str(), GDAL_OF_VECTOR, NULL, NULL, NULL );
        if(hDS == NULL)
        {
            if(suppressWarnings == false)
            {
                printf("could not open string \"%s\" as shape file!\n",inputString.c_str());
            }
            conversionSuccess = false;
        } else
        {
            /* tutorial stuff /

            // getting dataset information
            OGRLayerH hLayer;
            OGRFeatureH hFeature;
            OGRFeatureDefnH hFDefn;

            hLayer = GDALDatasetGetLayerByName( hDS, "point" );
            hFDefn = OGR_L_GetLayerDefn(hLayer);
            OGR_L_ResetReading(hLayer);
            while( (hFeature = OGR_L_GetNextFeature(hLayer)) != NULL )
            {
                int iField;
                OGRGeometryH hGeometry;
                for( iField = 0; iField < OGR_FD_GetFieldCount(hFDefn); iField++ )
                {
                    OGRFieldDefnH hFieldDefn = OGR_FD_GetFieldDefn( hFDefn, iField );
                    switch( OGR_Fld_GetType(hFieldDefn) )
                    {
                        case OFTInteger:
                            printf( "%d,", OGR_F_GetFieldAsInteger( hFeature, iField ) );
                            break;
                        case OFTInteger64:
                            printf( CPL_FRMT_GIB ",", OGR_F_GetFieldAsInteger64( hFeature, iField ) );
                            break;
                        case OFTReal:
                            printf( "%.3f,", OGR_F_GetFieldAsDouble( hFeature, iField) );
                            break;
                        case OFTString:
                            printf( "%s,", OGR_F_GetFieldAsString( hFeature, iField) );
                            break;
                        default:
                            printf( "%s,", OGR_F_GetFieldAsString( hFeature, iField) );
                            break;
                    }
                }
                hGeometry = OGR_F_GetGeometryRef(hFeature);
                if( hGeometry != NULL
                    && wkbFlatten(OGR_G_GetGeometryType(hGeometry)) == wkbPoint )
                {
                    printf( "%.3f,%3.f\n", OGR_G_GetX(hGeometry, 0), OGR_G_GetY(hGeometry, 0) );
                }
                else
                {
                    printf( "no point geometry\n" );
                }
                OGR_F_Destroy( hFeature );
            }
            GDALClose( hDS );

            / end tutorial stuff */
        }
        GDALClose( hDS );
    }
    // need to check for all other file types that go along with, or does gdal already deal with that type of problem?

    return conversionSuccess;
}
/*** end typechecker functions ***/
