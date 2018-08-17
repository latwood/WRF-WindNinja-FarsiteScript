#ifndef LCPMANAGER_H
#define LCPMANAGER_H

#include "inputVariablesHandler.h"

class lcpManager
{

public:

    // constructors
    lcpManager();

    // reconstructor like functions
    bool load_required_inputs(inputVariablesHandler *inputs, double newFirePerimMinLat, double newFirePerimMinLong, double newFirePerimMaxLat, double newFirePerimMaxLong);

    // functions
    bool downloadLcp();

private:

    // reconstructor like functions
    void reset();

    // downloader functions
    bool automaticLcpDownload();
    bool specifiedLcpDownload();

    // useful utility functions

    // data members needed from the inputs
    std::string actualLcpPath;
        // lcp download variables (WindNinja related)
    // going to do this one later after everything is finished. Not sure if use WindNinja api for lcp download or if setup separate lcp class
    bool automate_lcp_download;
    double fireperim_to_lcp_scalefactor;
    bool use_past_lcp;
    bool specify_lcp_download;
    double lcp_download_lat_point;
    double lcp_download_long_point;
    double lcp_download_northsouth_buffer;
    double lcp_download_westeast_buffer;
    std::string lcp_download_buffer_units;
    double lcp_download_north_lat_bound;
    double lcp_download_south_lat_bound;
    double lcp_download_east_long_bound;
    double lcp_download_west_long_bound;
        // createIgnition variables
    double firePerimMinLat;
    double firePerimMinLong;
    double firePerimMaxLat;
    double firePerimMaxLong;

    // data members created from inputs that are lcpManager specific

};

#endif // LCPMANAGER_H
