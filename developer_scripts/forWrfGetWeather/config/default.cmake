
set(ENV{CC} cc)
set(ENV{CXX} g++)

set(USER_CXX_FLAGS "-std=c++11")
set(USER_CXX_FLAGS_RELEASE "-O3")
set(USER_CXX_FLAGS_DEBUG "-debug -g -check=conversions,stack,uninit -fp-stack-check -fp-trap=common -fp-trap-all=common ")

set(NETCDF_C_INCLUDE_DIR "/home/atw09001/src/WRF-WindNinja-FarsiteScript/include/extraLibs/netcdf-c-4.6.1/build_netcdf-c-4.6.1/include/")
set(NETCDF_CXX_INCLUDE_DIR "/home/atw09001/src/WRF-WindNinja-FarsiteScript/include/extraLibs/netcdf-cxx-4.3.0/build_netcdf-cxx-4.3.0/include/")
set(HDF5_INCLUDE_DIR "/home/atw09001/src/WRF-WindNinja-FarsiteScript/include/extraLibs/hdf5-1.10.2/build_hdf5-1.10.2/include/")
set(SZIP_INCLUDE_DIR "/home/atw09001/src/WRF-WindNinja-FarsiteScript/include/extraLibs/szip-2.1/build_szip-2.1/include/")
set(ZLIB_INCLUDE_DIR "/home/atw09001/src/WRF-WindNinja-FarsiteScript/include/extraLibs/zlib-1.2.11/build_zlib-1.2.11/include/")
set(CURL_INCLUDE_DIR "/home/atw09001/src/WRF-WindNinja-FarsiteScript/include/extraLibs/curl-7.61.1/build_curl-7.61.1/include/")
set(JASPER_INCLUDE_DIR "/home/atw09001/src/WRF-WindNinja-FarsiteScript/include/extraLibs/jasper-1.900.1/build_jasper-1.900.1/include/")
set(GDAL_INCLUDE_DIR "/home/atw09001/src/WRF-WindNinja-FarsiteScript/include/extraLibs/gdal-2.0.3/build_gdal-2.0.3/include/")

set(NETCDF_LIB_C "/home/atw09001/src/WRF-WindNinja-FarsiteScript/include/extraLibs/netcdf-c-4.6.1/build_netcdf-c-4.6.1/lib/libnetcdf.a")
set(NETCDF_LIB_CXX "/home/atw09001/src/WRF-WindNinja-FarsiteScript/include/extraLibs/netcdf-cxx-4.3.0/build_netcdf-cxx-4.3.0/lib/libnetcdf_c++4.so")
set(HDF5_LIB_1 "/home/atw09001/src/WRF-WindNinja-FarsiteScript/include/extraLibs/hdf5-1.10.2/build_hdf5-1.10.2/lib/libhdf5.so")
set(HDF5_LIB_2 "/home/atw09001/src/WRF-WindNinja-FarsiteScript/include/extraLibs/hdf5-1.10.2/build_hdf5-1.10.2/lib/libhdf5_hl.so")
set(SZIP_LIB "/home/atw09001/src/WRF-WindNinja-FarsiteScript/include/extraLibs/szip-2.1/build_szip-2.1/lib/libsz.a")
set(ZLIB_LIB "/home/atw09001/src/WRF-WindNinja-FarsiteScript/include/extraLibs/zlib-1.2.11/build_zlib-1.2.11/lib/libz.a")
set(CURL_LIB "/home/atw09001/src/WRF-WindNinja-FarsiteScript/include/extraLibs/curl-7.61.1/build_curl-7.61.1/lib/libcurl.so")
set(JASPER_LIB "/home/atw09001/src/WRF-WindNinja-FarsiteScript/include/extraLibs/jasper-1.900.1/build_jasper-1.900.1/lib/libjasper.so")
set(GDAL_LIB "/home/atw09001/src/WRF-WindNinja-FarsiteScript/include/extraLibs/gdal-2.0.3/build_gdal-2.0.3/lib/libgdal.so")

set(LIBS ${NETCDF_LIB_CXX} ${NETCDF_LIB_C} ${HDF5_LIB_2} ${HDF5_LIB_1} ${SZIP_LIB} ${ZLIB_LIB} ${CURL_LIB} ${JASPER_LIB} ${GDAL_LIB} m z)
set(INCLUDE_DIRS ${NETCDF_C_INCLUDE_DIR} ${NETCDF_CXX_INCLUDE_DIR} ${HDF5_INCLUDE_DIR} ${SZIP_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR} ${CURL_INCLUDE_DIR} ${JASPER_INCLUDE_DIR} ${GDAL_INCLUDE_DIR})

add_definitions(-DRESTRICTKEYWORD=restrict)


