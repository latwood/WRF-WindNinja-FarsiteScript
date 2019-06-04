
set(ENV{CC} cc)
set(ENV{CXX} g++)

set(USER_CXX_FLAGS "-std=c++11")
set(USER_CXX_FLAGS_RELEASE "-O3")
set(USER_CXX_FLAGS_DEBUG "-debug -g -check=conversions,stack,uninit -fp-stack-check -fp-trap=common -fp-trap-all=common ")

set(NETCDF_C_INCLUDE_DIR "$scriptRoot/WRF-WindNinja-FarsiteScript/include/extraLibs/netcdf-c-4.6.1/build_netcdf-c-4.6.1/include/")
set(HDF5_INCLUDE_DIR "$scriptRoot/WRF-WindNinja-FarsiteScript/include/extraLibs/hdf5-1.10.2/build_hdf5-1.10.2/include/")
set(GDAL_INCLUDE_DIR "$scriptRoot/WRF-WindNinja-FarsiteScript/include/extraLibs/gdal-2.0.3/build_gdal-2.0.3/include/")
set(BOOST_INCLUDE_DIR "$scriptRoot/WRF-WindNinja-FarsiteScript/include/extraLibs/boost_1_55_0/build_boost_1_55_0/include/")

set(NETCDF_LIB_C "$scriptRoot/WRF-WindNinja-FarsiteScript/include/extraLibs/netcdf-c-4.6.1/build_netcdf-c-4.6.1/lib/libnetcdf.a")
set(HDF5_LIB_1 "$scriptRoot/WRF-WindNinja-FarsiteScript/include/extraLibs/hdf5-1.10.2/build_hdf5-1.10.2/lib/libhdf5.so")
set(HDF5_LIB_2 "$scriptRoot/WRF-WindNinja-FarsiteScript/include/extraLibs/hdf5-1.10.2/build_hdf5-1.10.2/lib/libhdf5_hl.so")
set(GDAL_LIB "$scriptRoot/WRF-WindNinja-FarsiteScript/include/extraLibs/gdal-2.0.3/build_gdal-2.0.3/lib/libgdal.so")

set(LIBS ${NETCDF_LIB_C} ${HDF5_LIB_2} ${HDF5_LIB_1} ${GDAL_LIB} m z curl)
set(INCLUDE_DIRS ${NETCDF_C_INCLUDE_DIR} ${HDF5_INCLUDE_DIR} ${GDAL_INCLUDE_DIR} ${BOOST_INCLUDE_DIR})

add_definitions(-DRESTRICTKEYWORD=restrict)


