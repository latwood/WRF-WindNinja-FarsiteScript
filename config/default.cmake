# system specific settings for joinwrf
##set(ENV{CC} icc)
##set(ENV{CXX} icpc)
set(ENV{CC} cc)
set(ENV{CXX} g++)

#set(USER_CXX_FLAGS "--sysroot -restrict -std=c++11")
set(USER_CXX_FLAGS "-std=c++11")
#set(USER_CXX_FLAGS_RELEASE "-xHOST -O3")
set(USER_CXX_FLAGS_RELEASE "-O3")
set(USER_CXX_FLAGS_DEBUG "-debug -g -check=conversions,stack,uninit -fp-stack-check -fp-trap=common -fp-trap-all=common ")

#set(NETCDF_INCLUDE_DIR "/usr/local/netcdf/include/")
#set(NETCDF_LIB_C       "/usr/local/netcdf/lib/libnetcdf.dylib")
#set(NETCDF_LIB_CPP     "/usr/local/netcdf/lib/libnetcdf-cxx4.1.dylib")
#set(HDF5_LIB_1         "/usr/local/hdf5/lib/libhdf5.a")
#set(HDF5_LIB_2         "/usr/local/hdf5/lib/libhdf5_hl.a")
#set(SZIP_LIB           "/usr/local/szlib/lib/libsz.a")

set(NETCDF_C_INCLUDE_DIR "/home/atw09001/src/extraLibs/netcdf-c/build_netcdf-c-4.6.1/include/")
set(NETCDF_CXX_INCLUDE_DIR "/home/atw09001/src/extraLibs/netcdf-cxx/build_netcdf-cxx4-4.3.0/include/")
set(HDF5_INCLUDE_DIR   "/home/atw09001/src/extraLibs/hdf5/build_hdf5-1.10.2/include/")
set(SZIP_INCLUDE_DIR   "/home/atw09001/src/extraLibs/szlib/build_szip-2.1/include/")
set(ZLIB_INCLUDE_DIR   "/home/atw09001/src/extraLibs/zlib/build_zlib-1.2.11/include/")
set(CURL_INCLUDE_DIR   "/home/atw09001/src/extraLibs/curl/build_curl-7.61.0/include/curl/")
set(JASPER_INCLUDE_DIR "/home/atw09001/src/extraLibs/jasper/build_jasper-1.900.1/include/")
set(GDAL_INCLUDE_DIR   "/home/atw09001/src/extraLibs/gdal/build_gdal-2.3.1/include/")
set(NETCDF_LIB_C       "/home/atw09001/src/extraLibs/netcdf-c/build_netcdf-c-4.6.1/lib/libnetcdf.a")
set(NETCDF_LIB_CXX     "/home/atw09001/src/extraLibs/netcdf-cxx/build_netcdf-cxx4-4.3.0/lib/libnetcdf_c++4.so")
set(HDF5_LIB_1         "/home/atw09001/src/extraLibs/hdf5/build_hdf5-1.10.2/lib/libhdf5.so")
set(HDF5_LIB_2         "/home/atw09001/src/extraLibs/hdf5/build_hdf5-1.10.2/lib/libhdf5_hl.so")
set(SZIP_LIB           "/home/atw09001/src/extraLibs/szlib/build_szip-2.1/lib/libsz.a")
set(ZLIB_LIB           "/home/atw09001/src/extraLibs/zlib/build_zlib-1.2.11/lib/libz.a")
set(CURL_LIB           "/home/atw09001/src/extraLibs/curl/build_curl-7.61.0/lib/libcurl.so")
set(JASPER_LIB         "/home/atw09001/src/extraLibs/jasper/build_jasper-1.900.1/lib/libjasper.so")
set(GDAL_LIB           "/home/atw09001/src/extraLibs/gdal/build_gdal-2.3.1/lib/libgdal.so")

#set(LIBS ${NETCDF_LIB_CPP} ${NETCDF_LIB_C} ${HDF5_LIB_2} ${HDF5_LIB_1} ${SZIP_LIB})
#set(LIBS ${NETCDF_LIB_CPP} ${NETCDF_LIB_C} ${HDF5_LIB_2} ${HDF5_LIB_1} ${SZIP_LIB} ${CURL_LIB} m z)
#set(LIBS ${NETCDF_LIB_CPP} ${NETCDF_LIB_C} ${HDF5_LIB_2} ${HDF5_LIB_1} ${SZIP_LIB} ${ZLIB_LIB} ${CURL_LIB} m z)
set(LIBS ${NETCDF_LIB_CXX} ${NETCDF_LIB_C} ${HDF5_LIB_2} ${HDF5_LIB_1} ${SZIP_LIB} ${ZLIB_LIB} ${CURL_LIB} ${JASPER_LIB} ${GDAL_LIB} m z)
#set(INCLUDE_DIRS ${NETCDF_INCLUDE_DIR} ${HDF5_INCLUDE_DIR} ${GDAL_INCLUDE_DIR})
#set(INCLUDE_DIRS ${NETCDF_INCLUDE_DIR} ${GDAL_INCLUDE_DIR})
set(INCLUDE_DIRS ${NETCDF_C_INCLUDE_DIR} ${NETCDF_CXX_INCLUDE_DIR} ${HDF5_INCLUDE_DIR} ${SZIP_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR} ${CURL_INCLUDE_DIR} ${JASPER_INCLUDE_DIR} ${GDAL_INCLUDE_DIR})

add_definitions(-DRESTRICTKEYWORD=restrict)


### acquired hdf5 from https://www.hdfgroup.org/downloads/ latest download, zlib from http://www.zlib.net/ following the zlib.net location to get tar.gz version, curl from https://curl.haxx.se/download.html latest tar.gz download, ideas for setting up netcdf from https://www.youtube.com/watch?v=psK7wdBo_SU which gave the link to szlib which was ftp://ftp.unidata.ucar.edu/pub/netcdf/ under netcdf4 to get to ftp://ftp.unidata.ucar.edu/pub/netcdf/netcdf-4/, got netcdf-c and netcdf-cxx from https://www.unidata.ucar.edu/downloads/netcdf/index.jsp choosing the latest stable releases as tar.gz. I used https://www.unidata.ucar.edu/software/netcdf/docs/getting_and_building_netcdf.html to help figure out how to do the build process, with references to https://trac.osgeo.org/gdal/wiki/BuildHints especially under the external library issues netcdf and hdf to understand more problems that can occur. Then I built in the order zlib, szlib, curl, hdf5, netcdf-c, netcdf-cxx. In each case I opened the readme just in case there were more hints, ran ./configure --help to see what the defaults for -fpic (--with-pic), --enable-shared, --enable-static tags were at a minimum to make sure it was to create the static AND shared libraries AND made it position independent code. I also looked in this same spot to see what other libraries might be needed just in case and to see how to add in szlib, zlib, and curl to hdf5 and how to do the same to netcdf. Note that netcdf did NOT have options like --with-zlib or even --with-hdf5 which is probably why it is confusing to build. Because of the gdal reference, I realized that hdf5 needs the -hl tag as well and that fixed a ton of problems. Didn't need to use --disable-netcdf to stop the namespace issues with hdf5 as explained in gdal this time, must be related to the -hl tag. Because there were no --with-hdf5 tags, it was confusing to do the netcdf, but the user docs shows what commands to use. I think my past problems were also that I tried to throw in zlib again in that step in the past, you only need hdf5 in the CPPFLAGS and LDFLAGS for netcdf. Might find all this wasn't enough when I try to make gdal.
### so here is a list of the configure stuff that worked. I also ran make check between make and make install for each of these. No sudo make install.
## -- in zlib location --
# ./configure --prefix=$HOME/src/extraLibs/zlib/build_zlib-1.2.11
## -- in szlib location --
# ./configure --prefix=$HOME/src/extraLibs/szlib/build_szip-2.1
## -- in curl location --
# ./configure --prefix=$HOME/src/extraLibs/curl/build_curl-7.61.0 --with-zlib=/home/latwood/src/extraLibs/zlib/build_zlib-1.2.11
## -- in hdf5 location --
# ./configure --with-szlib=$HOME/src/extraLibs/szlib/build_szip-2.1 --with-zlib=$HOME/src/extraLibs/zlib/build_zlib-1.2.11 --prefix=$HOME/src/extraLibs/hdf5/build_hdf5-1.10.2 --enable-hl
## -- in netcdf-c location --
# CPPFLAGS=-I$HOME/src/extraLibs/hdf5/build_hdf5-1.10.2/include LDFLAGS=-L$HOME/src/extraLibs/hdf5/build_hdf5-1.10.2/lib ./configure --prefix=$HOME/src/extraLibs/netcdf-c/build_netcdf-c-4.6.1
## -- in netcdf-cxx location --
# CPPFLAGS=-I$HOME/src/extraLibs/hdf5/build_hdf5-1.10.2/include LDFLAGS=-L$HOME/src/extraLibs/hdf5/build_hdf5-1.10.2/lib ./configure --prefix=$HOME/src/extraLibs/netcdf-cxx/build_netcdf-cxx4-4.3.0
### note you may need to change $HOME for /home/username/ or something similar. In the end I didn't actually use curl, except as a separate include.

### might be an alternative way to do all this that is similar to building a bash script like the following: https://gist.github.com/perrette/cd815d03830b53e24c82. This script doesn't quite work, probably is because of the missing -hl stuff in hdf5 or maybe something to do with older versions, but the idea is to make something similar. WindNinja does something similar in their runscript.sh file to get all the extra dependencies: https://github.com/firelab/windninja/blob/master/scripts/build_deps.sh


## there were some interesting issues trying to use all these libraries. -xHost had to go because this is not a macOS computer. Not sure if -restrict really needed to go, but --sysroot makes the compiler/linker complain that it can't find wierd os references, probably because these libraries are built separate from the normal OS versions. I had to switch back and forth with the static .a libraries and the dynamic .so libraries to get rid of strange errors where it complained it couldn't find different needed parts during the linking process (not compile). This was just the combination that worked. The use of the libraries is different when using straight make, cmake does something strange to the makefiles. Means that during the process sometimes have to restart the entire make or cmake process, and delete any .user IDE files to clean the process and start again to get rid of an error after trying a fix. Not sure how much trouble these issues will be in making my own netcdf file/application process, but this helps see a ton of what has already been going on.


### for running WindNinja with archived NAM files, Natalie wanted me to use gdal version 1.9.1 with a command basically like ./configure --prefix=/home/natalie/src/gdal/build/ --with-netcdf --with-jasper. I had issues trying to get this to work when I did it, finally half-hazardly got it to work, but still struggled. The jasper source I found was here https://launchpad.net/ubuntu/+source/jasper. Another probably better source as gdal points to this as the best source is http://www.ece.uvic.ca/~frodo/jasper/. Turns out the first jasper source is easier to work with, though notice it doesn't build shared libraries without --enable-shared
## -- in jasper location --
# ./configure --prefix=$HOME/src/extraLibs/jasper/build_jasper-1.900.1 --enable-shared

## -- in gdal location --
# ./configure --prefix=$HOME/src/extraLibs/gdal/build_gdal-1.9.1 --with-curl=$HOME/src/extraLibs/curl/build_curl-7.61.0 --with-jasper=$HOME/src/extraLibs/jasper/build_jasper-1.900.1 --with-netcdf=$HOME/src/extraLibs/netcdf-c/build_netcdf-c-4.6.1 --with-hdf5=$HOME/src/extraLibs/hdf5/build_hdf5-1.10.2 --with-libz=$HOME/src/extraLibs/zlib/build_zlib-1.2.11 CXX=g++-4.8 CC=gcc-4.8
### during the make process, you have to be careful cause it fails the make process with the current compiler. Ubuntu 14.04 had less problems somehow. So you have to do a few edits of the files to get rid of old problems. For example, changed #define CPLIsNan(x) isnan(x) to #define CPLIsNan(x) __builtin_isnan(x) in gdal/gdal-1.9.1/port/cpl_port.h as is explained in https://trac.osgeo.org/gdal/ticket/6489#no1 and is shown by https://trac.osgeo.org/gdal/changeset/34134. Actually that fix didn't work, doing a cast to float in the call that used that function did the job.
### Used info from this website to install an older gcc https://askubuntu.com/questions/923337/installing-an-older-gcc-version3-4-3-on-ubuntu-14-04-currently-4-8-installed, and this website to add the flags https://stackoverflow.com/questions/3698441/how-to-use-an-older-version-of-gcc-in-linux. Yeah this is apparently a bunch of bugs not related to the versions of the compiling stuff. Guess I got to ask Natalie some stuff, and see if newer gdal can open the archive stuff.
# ./configure --prefix=$HOME/src/extraLibs/gdal/build_gdal-2.3.1 --with-curl=$HOME/src/extraLibs/curl/build_curl-7.61.0 --with-jasper=$HOME/src/extraLibs/jasper/build_jasper-1.900.1 --with-netcdf=$HOME/src/extraLibs/netcdf-c/build_netcdf-c-4.6.1 --with-hdf5=$HOME/src/extraLibs/hdf5/build_hdf5-1.10.2 --with-libz=$HOME/src/extraLibs/zlib/build_zlib-1.2.11
## this second newer version built without problems and no changes to files. My guess is the other one had some issues with later versions of gcc.




