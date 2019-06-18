# set cmake version and config file location
cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/config)

# install to the run directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Get system settings file
set(SYST default)
include(${SYST} OPTIONAL RESULT_VARIABLE SYSTINC)

# Set the default build type to RELEASE.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RELEASE CACHE STRING
    "Choose the type of build, options are: None Debug Release." FORCE)
else()
  string(TOUPPER ${CMAKE_BUILD_TYPE} TEMP)
  set(CMAKE_BUILD_TYPE ${TEMP} CACHE STRING
    "Choose the type of build, options are: None Debug Release." FORCE)
endif()

# Start the project only after the system specific settings are loaded.
project(WRF-WindNinja-FarsiteScript)

# Only set the compiler flags when the cache is created
# to enable editing of the flags in the CMakeCache.txt file.
if(NOT HASCACHE)
  set(CMAKE_CXX_FLAGS ${USER_CXX_FLAGS} CACHE STRING
    "Flags used by the CXX-compiler during all build types." FORCE)
  set(CMAKE_CXX_FLAGS_DEBUG ${USER_CXX_FLAGS_DEBUG} CACHE STRING
    "Flags used by the CXX-compiler during debug builds." FORCE)
  set(CMAKE_CXX_FLAGS_RELEASE ${USER_CXX_FLAGS_RELEASE} CACHE STRING
    "Flags used by the CXX-compiler during release builds." FORCE)

  message(STATUS "Build Type: " ${CMAKE_BUILD_TYPE})
  set(HASCACHE TRUE CACHE BOOL "CMakeCache.txt created." FORCE)
  # Make sure that ccmake only contains build type.
  mark_as_advanced(HASCACHE)
  mark_as_advanced(CMAKE_INSTALL_PREFIX)
endif()

# Print the C++ and CUDA compiler flags to the screen.
if(CMAKE_BUILD_TYPE STREQUAL "RELEASE")
  message(STATUS "Compiler flags: " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_RELEASE})
else()
  message(STATUS "Compiler flags: " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_DEBUG})
endif()

#file(GLOB_RECURSE SOURCES "src/*")
set(SOURCES "src/main.cpp" "src/lcpDownloader.cpp" "src/createIgnitions.cpp" "src/WindNinjaAPI.cpp" "src/wrfGetWeather.cpp" "src/farsiteAPI.cpp"

        "src/getWindNinjaPath.cpp" "src/getFarsitePath.cpp"

        "src/utilityClasses/calcTime.cpp" "src/utilityClasses/doesFolderExist.cpp" "src/utilityClasses/createFolder.cpp"
        "src/utilityClasses/isValidExecFile.cpp" "src/utilityClasses/doesFilenameExist.cpp" "src/utilityClasses/copySingleFile.cpp"
        "src/utilityClasses/findFileBaseNameStuff.cpp"

        "src/inputClasses/inputVariablesHandler.cpp" "src/inputClasses/singleInputVariable.cpp" "src/inputClasses/inputVariablesInitializer.cpp"
        "src/inputClasses/inputParser.cpp"

        "src/inputClasses/singleValueDataTypes/boolValue.cpp" "src/inputClasses/singleValueDataTypes/size_tValue.cpp"
	"src/inputClasses/singleValueDataTypes/intValue.cpp" "src/inputClasses/singleValueDataTypes/doubleValue.cpp"
        "src/inputClasses/singleValueDataTypes/stringValue.cpp" "src/inputClasses/singleValueDataTypes/pathNameValue.cpp"
	"src/inputClasses/singleValueDataTypes/lcpFileValue.cpp" "src/inputClasses/singleValueDataTypes/shapeFileValue.cpp"
        "src/inputClasses/singleValueDataTypes/wrfFileValue.cpp" "src/inputClasses/singleValueDataTypes/lat_longValue.cpp"
        "src/inputClasses/singleValueDataTypes/dateValue.cpp" "src/inputClasses/singleValueDataTypes/hour_minValue.cpp"

        "src/inputClasses/multiValueDataTypes/createIgnitionFromLatLongsStorage.cpp" "src/inputClasses/multiValueDataTypes/polygonIgnitShapeFileStorage.cpp"
	"src/inputClasses/multiValueDataTypes/GeoMACfirePerimeterFileStorage.cpp" "src/inputClasses/multiValueDataTypes/farsiteOutputFirePerimeterFileStorage.cpp"
	"src/inputClasses/multiValueDataTypes/wrfFileStorage.cpp" "src/inputClasses/multiValueDataTypes/additionalWindNinjaOutputs_googleStorage.cpp"
        "src/inputClasses/multiValueDataTypes/additionalWindNinjaOutputs_shapefileStorage.cpp" "src/inputClasses/multiValueDataTypes/additionalWindNinjaOutputs_pdfStorage.cpp")
include_directories("include" SYSTEM ${INCLUDE_DIRS})
link_libraries(${LIBS})
#add_executable(utahlsm ${SOURCES})
add_executable(${PROJECT_NAME} ${SOURCES})

