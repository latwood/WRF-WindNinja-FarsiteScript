#!/bin/bash

# set failing step bool variable
success=0  # 1 if failing, 0 if successful

if [ $success == 0 ]; then
  if [ "$#" != 3 ]; then
    echo "not enough inputs! Inputs must be \"inDirectory\" \"outDirectory\" \"debug\""
    echo "  \"inDirectory\" represents a folder full of whatever plus wrf files to be trimmed using \"ncks -v Times,T2,V10,U10,QCLOUD,Q2,RAINC,RAINNC wrffilename wrffilename\""
    echo "  \"outDirectory\" represents a folder in which to put the output trimmed wrf files"
    echo "  \"debug\" is a boolean value of \"0\" or \"1\" where \"0\" means the script will have normal output, and \"1\" means the script will have a bunch of extra output used for major debugging"
    success=1
  fi
fi

inDirectory="${1}"
outDirectory="${2}"
debug="${3}"

if [ $success == 0 ]; then
  if [ ! -d "${inDirectory}" ]; then
    echo "!!!"$inDirectory" does not exist!!!"
    success=1
  fi
fi

if [ $success == 0 ]; then
  if [ ! -d "${outDirectory}" ]; then
    echo "!!!"$outDirectory" does not exist!!!"
    success=1
  fi
fi

if [ $success == 0 ]; then
  compareIn=${inDirectory}
  compareOut=${outDirectory}
  if [ ${compareIn: -1} == "/" ]; then
    compareIn=${inDirectory:: -1}
  fi
  if [ ${compareOut: -1} == "/" ]; then
    compareOut=${outDirectory:: -1}
  fi
  if [ ${compareIn} == ${compareOut} ]; then
    echo "!!!inDirectory and outDirectory must not be equal!!!"
    success=1
  fi
fi


# not needed, but originally I thought it would be needed
baseDir=$(pwd)


if [ $success == 0 ]; then
  foundFiles=${compareIn}"/*"
  declare -i fileCount=1
  declare -i processedWrfFiles=0
  for fileIdx in $foundFiles; do

    if [ $debug == 1 ]; then
      echo ""  # want a nice extra space
      echo "fileCount = "$fileCount
    fi
    
    currentInFile=$fileIdx
    if [ $debug == 1 ]; then
      echo "currentInFile with path = "$currentInFile
    fi

    currentFile=$(echo "${currentInFile}" | sed 's/.*\///')
    if [ $debug == 1 ]; then
      echo "currentInFile no path = "$currentFile
    fi
    
    currentOutFile=${compareOut}"/"${currentFile}
    if [ $debug == 1 ]; then
      echo "currentOutFile = "$currentOutFile
    fi

    fileCount+=1

    # use ncdump looking at headers to see if it is a netcdf file
    #  suppress output by sending result to the null device
    ncdump ${currentInFile} -h > /dev/null 2>&1
    validFile_success=$?

    trim_success=0
    if [ $success == 0 ] && [ $validFile_success == 0 ]; then
      # file is a legit netcdf file, attempt trimmer function
      ncks -v Times,T2,V10,U10,QCLOUD,Q2,RAINC,RAINNC $currentInFile $currentOutFile
      trim_success=$?
    fi

    if [ $trim_success == 0 ]; then
      if [ $validFile_success == 0 ]; then
        processedWrfFiles+=1
      fi
    else
      echo "!!!found netcdf file that failed to be trimmed!!!"
      echo "probably not the right kinds of data or format in said wrf file"
      success=1
      break;
    fi

    if [ $debug == 1 ]; then
      echo "processedWrfFiles = "$processedWrfFiles
      echo ""  # want an extra space
    fi

  done
  if [ ${processedWrfFiles} == 0 ]; then
    echo "!!! no wrf files found in "$inDirectory" directory !!!"
    success=1
  fi
fi


