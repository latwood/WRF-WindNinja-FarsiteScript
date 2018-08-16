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
set(SOURCES "src/main.cpp" "src/inputVariablesHandler.cpp" "src/inputVariable_info.cpp" "src/inputVariables_infoStorage.cpp"
    "src/inputVariables_valueStorage.cpp" "src/lcpManager.cpp" "src/createIgnitions.cpp" "src/wrfGetWeather.cpp" "src/wrfInterpretation.cpp" "src/WindNinjaAPI.cpp"
    "src/createFarsiteInputs.cpp" "src/farsite.cpp" "src/inputParser.cpp")
include_directories("include" SYSTEM ${INCLUDE_DIRS})
link_libraries(${LIBS})
#add_executable(utahlsm ${SOURCES})
add_executable(${PROJECT_NAME} ${SOURCES})

find_package(Doxygen)
if(DOXYGEN_FOUND)
  configure_file(config/doxygen.conf.in ${CMAKE_CURRENT_BINARY_DIR}/doxygen.conf)
  add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxygen.conf
    COMMENT "Generating API documentation with Doxygen." VERBATIM
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/doxygen.conf)
  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc DESTINATION ${CMAKE_SOURCE_DIR})
else(DOXYGEN)
  message(STATUS "WARNING: Doxygen not found - Reference manual will not be created.")
endif()
