#include "lcpDownloader.h"

/***** public functions *****/

/*** constructor functions ***/
lcpDownloader::lcpDownloader()
{

}
/*** end constructor functions ***/

/*** reconstructor like functions ***/
bool lcpDownloader::load_required_inputs(inputVariablesHandler *inputs, double newFirePerimMinLat, double newFirePerimMinLong, double newFirePerimMaxLat, double newFirePerimMaxLong)
{
    // first reset all values to default values with reconstructor
    reset();

    // next load all the needed inputs from the input file
    actualLcpPath = inputs->get_actualLcpPath();
        // lcp download variables (WindNinja related)
    // going to do this one later after everything is finished. Not sure if use WindNinja api for lcp download or if setup separate lcp class
    automate_lcp_download = inputs->get_inputVariableBoolValue("automate_lcp_download");
    fireperim_to_lcp_scalefactor = inputs->get_inputVariableDoubleValue("fireperim_to_lcp_scalefactor");
    use_past_lcp = inputs->get_inputVariableBoolValue("use_past_lcp");
    specify_lcp_download = inputs->get_inputVariableBoolValue("specify_lcp_download");
    lcp_download_lat_point = inputs->get_inputVariableLat_Long_PointValueLat_Coord("lcp_download_lat_long_point");
    lcp_download_long_point = inputs->get_inputVariableLat_Long_PointValueLong_Coord("lcp_download_lat_long_point");
    lcp_download_northsouth_buffer = inputs->get_inputVariableDoubleValue("lcp_download_northsouth_buffer");
    lcp_download_westeast_buffer = inputs->get_inputVariableDoubleValue("lcp_download_westeast_buffer");
    lcp_download_buffer_units = inputs->get_inputVariableStringValue("lcp_download_buffer_units");
    lcp_download_north_lat_bound = inputs->get_inputVariableDoubleValue("lcp_download_north_lat_bound");
    lcp_download_south_lat_bound = inputs->get_inputVariableDoubleValue("lcp_download_south_lat_bound");
    lcp_download_east_long_bound = inputs->get_inputVariableDoubleValue("lcp_download_east_long_bound");
    lcp_download_west_long_bound = inputs->get_inputVariableDoubleValue("lcp_download_west_long_bound");
        // createIgnition variables
    firePerimMinLat = newFirePerimMinLat;
    firePerimMinLong = newFirePerimMinLong;
    firePerimMaxLat = newFirePerimMaxLat;
    firePerimMaxLong = newFirePerimMaxLong;

    // if it gets here, everything went well
    return true;
}
/*** end reconstructor like functions ***/

/*** functions ***/
bool lcpDownloader::downloadLcp()
{
    bool success = true;

    // this next set of stuff could even just be done using the lcp class as a single function downloadLcp() that does this switch to the different types inside itself
    if(automate_lcp_download == true)
    {
        // now use fire perimeter stuff for lcp download
        if(automaticLcpDownload() == false)
        {
            printf("failed to perform automatic lcp download!\n");
            success = false;
        }
    } else if(specify_lcp_download == true)
    {
        if(specifiedLcpDownload() == false)
        {
            printf("failed to download lcp file using specified download variables!\n");
            success = false;
        }
    } else
    {
        printf("lcpManager::downloadLcp function called, but without method for downloading lcp file!\n");
        success = false;
    }

    return success;
}
/*** end functions ***/



/***** private functions *****/



/*** reconstructor like functions ***/
void lcpDownloader::reset()
{
    // data members needed from the inputs
    actualLcpPath = "";
        // lcp download variables (WindNinja related)
    // going to do this one later after everything is finished. Not sure if use WindNinja api for lcp download or if setup separate lcp class
    automate_lcp_download = false;
    fireperim_to_lcp_scalefactor = 1.0;
    use_past_lcp = false;
    specify_lcp_download = false;
    lcp_download_lat_point = 0.0;
    lcp_download_long_point = 0.0;
    lcp_download_northsouth_buffer = 0.0;
    lcp_download_westeast_buffer = 0.0;
    lcp_download_buffer_units = "";
    lcp_download_north_lat_bound = 0.0;
    lcp_download_south_lat_bound = 0.0;
    lcp_download_east_long_bound = 0.0;
    lcp_download_west_long_bound = 0.0;
        // createIgnition variables
    firePerimMinLat = 0.0;
    firePerimMinLong = 0.0;
    firePerimMaxLat = 0.0;
    firePerimMaxLong = 0.0;
}
/*** end reconstructor like functions ***/

/*** downloader functions ***/
bool lcpDownloader::automaticLcpDownload()
{
    // if it gets here, everything went well
    return true;
}

bool lcpDownloader::specifiedLcpDownload()
{
    // if it gets here, everything went well
    return true;
}
/*** end downloader functions ***/

/*** utility functions ***/
/*** end utility functions ***/
