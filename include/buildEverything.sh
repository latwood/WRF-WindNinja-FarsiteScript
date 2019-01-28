#!/bin/bash

### the basic idea of this script is that you set the versions and link names for downloading the dependencies, set the directory locations and other stuff as needed for other packages that need built, then you just run this script and it will download and build all 3rd library packages and extra applications used by this script. It will also create a config/cmake file with the paths, and a .cpp file needed by the code with paths, so that at the end of this script, an attempt to build the program is made using all these extra applications and libraries. If there are any problems found in any of the process, errors will be given so you can pinpoint what paths or variables set at the beginning of the script need changed. Warnings are given if certain steps aren't built in case they already exist, in which case you can decide to keep what you have or delete and retry certain steps, so if the process is interrupted, you can avoid rebuilding everything each time.
### in order to get the paths correct for the final script build, it turns out you don't need to edit the .bashrc file to add exports to the LD_LIBRARY_PATH for each new path, or even export the paths in such a way in the middle of the process. Instead a config/cmake file is written from scratch with the new paths as found when building everything, so if you need to edit the way that file is made, that stuff isn't specified at the beginning of the script like all the other variables.

nCores="8"  # this is the attempted number of cores when running any make process (except WindNinja which got mad when I tried using more than 4 cores during one of my builds). Could potentially change this value as a user to maybe get some speedup building gdal and netcdf libraries. Definitely important to somewhat get that annoying insert password trick to actually SOMEWHAT work.

# setup base directory variables
baseDir=$(pwd)
finalScriptDir=$baseDir"/.."   # this should be the case
finalBuildDir=$finalScriptDir"/build"
configCmakeFile=$finalScriptDir"/config/default.cmake"
extraLibsDir=$baseDir"/extraLibs"
extraAppsDir=$baseDir"/extraApps"
getWindNinjaPathFile=$baseDir"/../src/getWindNinjaPath.cpp"
WindNinjaPathLine=6   # this is the line in the file to be replaced with the new path
getFarsitePathFile=$baseDir"/../src/getFarsitePath.cpp"
farsitePathLine=6   # this is the line in the file to be replaced with the new path

# setup zlib variables
zlibVersion="1.2.11"
zlibSource="http://www.zlib.net"
zlibDirName="zlib-"$zlibVersion
zlibTarName=$zlibDirName".tar.gz"
zlibBuildDirName="build_"$zlibDirName
zlibLink=$zlibSource"/"$zlibTarName
zlibTarDir=$extraLibsDir"/"$zlibTarName
zlibTarDirName=$extraLibsDir"/"$zlibDirName # note this is the same as zlibDir, cause eventually it might not be the same
zlibDir=$extraLibsDir"/"$zlibDirName
zlibBuildDir=$zlibDir"/"$zlibBuildDirName
zlibCPPFLAGS=""
zlibLDFLAGS=""
zlibConfigure="./configure --prefix="$zlibBuildDir

# setup szlib variables
szlibVersion="2.1"
szlibSource="ftp://ftp.unidata.ucar.edu/pub/netcdf/netcdf-4"
szlibDirName="szip-"$szlibVersion
szlibTarName=$szlibDirName".tar.gz"
szlibBuildDirName="build_"$szlibDirName
szlibLink=$szlibSource"/"$szlibTarName
szlibTarDir=$extraLibsDir"/"$szlibTarName
szlibTarDirName=$extraLibsDir"/"$szlibDirName
szlibDir=$extraLibsDir"/"$szlibDirName
szlibBuildDir=$szlibDir"/"$szlibBuildDirName
szlibCPPFLAGS=""
szlibLDFLAGS=""
szlibConfigure="./configure --prefix="$szlibBuildDir

# setup curl variables
curlVersion="7.61.1" #7.61.0
curlSource="https://curl.haxx.se/download.html"
curlDirName="curl-"$curlVersion
curlTarName=$curlDirName".tar.gz"
curlBuildDirName="build_"$curlDirName
curlLink="https://curl.haxx.se/download/"$curlTarName
curlTarDir=$extraLibsDir"/"$curlTarName
curlTarDirName=$extraLibsDir"/"$curlDirName
curlDir=$extraLibsDir"/"$curlDirName
curlBuildDir=$curlDir"/"$curlBuildDirName
curlCPPFLAGS=""
curlLDFLAGS=""
curlConfigure="./configure --prefix="$curlBuildDir" --with-zlib="$zlibBuildDir

# setup hdf5 variables
## note there is something messy, I had to left click the download link before right clicking it would give me a download link that would work directly in a web browser, so this might not work without finding it online. I fixed it by switching the source website from https://www.hdfgroup.org/downloads/hdf5/source-code to the site for past releases, which is similar, but now somehow support? strange. Note that the current link still has embedded a part that will need editing separate from the version
hdf5Version="1.10.2"
hdf5Source="https://portal.hdfgroup.org/display/support/Download+HDF5"
hdf5DirName="hdf5-"$hdf5Version
hdf5TarName=$hdf5DirName".tar.gz"
hdf5BuildDirName="build_"$hdf5DirName
hdf5Link="https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.10/hdf5-"$hdf5Version"/src/hdf5-"$hdf5Version".tar.gz"
hdf5TarDir=$extraLibsDir"/"$hdf5TarName
hdf5TarDirName=$extraLibsDir"/"$hdf5DirName
hdf5Dir=$extraLibsDir"/"$hdf5DirName
hdf5BuildDir=$hdf5Dir"/"$hdf5BuildDirName
hdf5CPPFLAGS=""
hdf5LDFLAGS=""
hdf5Configure="./configure --prefix="$hdf5BuildDir" --enable-hl --with-szlib="$szlibBuildDir" --with-zlib="$zlibBuildDir

# setup netcdf-c variables
netcdf_cVersion="4.6.1"
netcdf_cSource="https://github.com/Unidata/netcdf-c/releases"
netcdf_cDirName="netcdf-c-"$netcdf_cVersion
netcdf_cTarName="v"$netcdf_cVersion".tar.gz"
netcdf_cBuildDirName="build_"$netcdf_cDirName
netcdf_cLink="https://github.com/Unidata/netcdf-c/archive/v"$netcdf_cVersion".tar.gz"
netcdf_cTarDir=$extraLibsDir"/"$netcdf_cTarName
netcdf_cTarDirName=$extraLibsDir"/"$netcdf_cDirName
netcdf_cDir=$extraLibsDir"/"$netcdf_cDirName
netcdf_cBuildDir=$netcdf_cDir"/"$netcdf_cBuildDirName
netcdf_cCPPFLAGS="-I"$zlibBuildDir"/include -I"$curlBuildDir"/include -I"$hdf5BuildDir"/include"
netcdf_cLDFLAGS="-L"$zlibBuildDir"/lib -L"$curlBuildDir"/lib -L"$hdf5BuildDir"/lib"
netcdf_cConfigure="./configure --prefix="$netcdf_cBuildDir

# setup netcdf-cxx variables
netcdf_cxxVersion="4.3.0"
netcdf_cxxSource="https://github.com/Unidata/netcdf-cxx4/releases"
netcdf_cxxDirName="netcdf-cxx-"$netcdf_cxxVersion
netcdf_cxxTarName="v"$netcdf_cxxVersion".tar.gz"
netcdf_cxxBuildDirName="build_"$netcdf_cxxDirName
netcdf_cxxLink="https://github.com/Unidata/netcdf-cxx4/archive/v"$netcdf_cxxVersion".tar.gz"
netcdf_cxxTarDir=$extraLibsDir"/"$netcdf_cxxTarName
netcdf_cxxTarDirName=$extraLibsDir"/netcdf-cxx4-"$netcdf_cxxVersion
netcdf_cxxDir=$extraLibsDir"/"$netcdf_cxxDirName
netcdf_cxxBuildDir=$netcdf_cxxDir"/"$netcdf_cxxBuildDirName
netcdf_cxxCPPFLAGS="-I"$hdf5BuildDir"/include -I"$netcdf_cBuildDir"/include"
netcdf_cxxLDFLAGS="-L"$hdf5BuildDir"/lib -L"$netcdf_cBuildDir"/lib"
netcdf_cxxConfigure="./configure --prefix="$netcdf_cxxBuildDir

# setup jasper variables
jasperVersion="1.900.1"
jasperSource="https://launchpad.net/ubuntu/+source/jasper"
jasperDirName="jasper-"$jasperVersion
jasperTarName="jasper_"$jasperVersion".orig.tar.gz"
jasperBuildDirName="build_"$jasperDirName
jasperLink="https://launchpad.net/ubuntu/+archive/primary/+sourcefiles/jasper/"$jasperVersion"-14ubuntu3/jasper_"$jasperVersion".orig.tar.gz"
jasperTarDir=$extraLibsDir"/"$jasperTarName
jasperTarDirName=$extraLibsDir"/jasper-"$jasperVersion
jasperDir=$extraLibsDir"/"$jasperDirName
jasperBuildDir=$jasperDir"/"$jasperBuildDirName
jasperCPPFLAGS=""
jasperLDFLAGS=""
jasperConfigure="./configure --prefix="$jasperBuildDir" --enable-shared"

# setup gdal variables
gdalVersion="2.0.3"
gdalSource="https://trac.osgeo.org/gdal/wiki/DownloadSource"
gdalDirName="gdal-"$gdalVersion
gdalTarName=$gdalDirName".tar.gz"
gdalBuildDirName="build_"$gdalDirName
gdalLink="http://download.osgeo.org/gdal/"$gdalVersion"/gdal-"$gdalVersion".tar.gz"
gdalTarDir=$extraLibsDir"/"$gdalTarName
gdalTarDirName=$extraLibsDir"/"$gdalDirName
gdalDir=$extraLibsDir"/"$gdalDirName
gdalBuildDir=$gdalDir"/"$gdalBuildDirName
gdalCPPFLAGS=""
gdalLDFLAGS=""
gdalConfigure="./configure --prefix="$gdalBuildDir" --with-curl="$curlBuildDir" --with-jasper="$jasperBuildDir" --with-netcdf="$netcdf_cBuildDir" --with-hdf5="$hdf5BuildDir" --with-libz="$zlibBuildDir

# setup WindNinja variables
windninjaDir=$extraAppsDir"/windninja"
windninjaSource="https://github.com/firelab/windninja"
windninjaLink="https://github.com/firelab/windninja.git"
windninjaScriptsDir=$windninjaDir"/scripts"
windninjaBuildDir=$windninjaDir"/build_windninja"
windninjaConfigure=""

# setup farsite variables
farsiteDir=$extraAppsDir"/farsite"
farsiteSource="https://github.com/firelab/farsite"
farsiteLink="https://github.com/firelab/farsite.git"
farsiteSrcDir=$farsiteDir"/src"

# replacing paths in files variables
varToReplace="\$scriptRoot"
#varToReplace="/home/atw09001/src/combinationScript-cmake"


setupDownloadableLib()
{
  if [ "$#" != 13 ]; then
    echo "" # want a nice clean line
    echo "!!!Incorrect Number of parameters for setupDownloadableLib!!!"
    echo "need 13 parameters: \"extraLibsDir\" \"libDirName\" \"libTarDir\" \"libTarName\" \"libSource\" \"libLink\" \"libDir\" \"libTarDirName\" \"libBuildDir\" \"libCPPFLAGS\" \"libLDFLAGS\" \"libConfigure\" \"tryPasswordInjection\""
    echo "" # want a nice clean line
    return 1
  fi

  local extraLibsDir="${1}"
  local libDirName="${2}"
  local libTarDir="${3}"
  local libTarName="${4}"
  local libSource="${5}"
  local libLink="${6}"
  local libDir="${7}"
  local libTarDirName="${8}"
  local libBuildDir="${9}"
  local libCPPFLAGS="${10}"
  local libLDFLAGS="${11}"
  local libConfigure="${12}"
  local tryPasswordInjection="${13}"

  if [ $tryPasswordInjection == 1 ]; then
    sudo -v
  fi

  ## uncomment out if need to debug
#  echo "" # want a nice clean line
#  echo "extraLibsDir = "$extraLibsDir
#  echo "libDirName = "$libDirName
#  echo "libTarDir = "$libTarDir
#  echo "libTarName = "$libTarName
#  echo "libSource = "$libSource
#  echo "libLink = "$libLink
#  echo "libDir = "$libDir
#  echo "libTarDirName = "$libTarDirName
#  echo "libBuildDir = "$libBuildDir
#  echo "libCPPFLAGS = "$libCPPFLAGS
#  echo "libLDFLAGS = "$libLDFLAGS
#  echo "libConfigure = "$libConfigure
#  echo "" # want a nice clean line

  echo "" # want a nice clean line
  echo "checking for "$libDirName

  if [ ! -f "${libTarDir}" ]; then
    echo "downloading zlib from "$libSource
    cd $extraLibsDir
    wget ${libLink}
    if [ ! -f "${libTarDir}" ]; then
      echo "!!!"$libDirName" failed to download!!!"
      return 1
    fi
  else
    echo "!warning, "$libTarDir" already exists!"
  fi

  if [ ! -d "${libDir}" ]; then
    echo "unpacking "$libTarDir
    cd $extraLibsDir
    tar -xzf $libTarDir
    if [ ! -d "${libTarDirName}" ]; then
      echo "!!!failed to unpack "$libDir"!!!"
      return 1
    else
      if [ "${libTarDirName}" != "${libDir}" ]; then
        mv $libTarDirName $libDir
        if [ ! -d "${libDir}" ]; then
          echo "!!!failed to rename "$libTarDirName" to "$libDir"!!!"
          return 1
        fi
      fi
    fi
  else
    echo "!warning, "$libDir" already exists!"
  fi

  if [ ! -d "${libBuildDir}" ]; then
    echo "running configure "$libConfigure
    cd $libDir
    export CPPFLAGS=$libCPPFLAGS
    export LDFLAGS=$libLDFLAGS
    ${libConfigure}  # run the passed in configure command
    echo "building "$libDirName
    make -j$nCores && make install
    if [ ! -d "${libBuildDir}" ]; then
      echo "!!!failed to build "$libDirName"!!!"
      return 1
    fi
    cd $extraLibsDir
  else
    echo "!warning, "$libBuildDir" already exists!"
  fi

  echo "finished setup for "$libDirName
  echo "" # add a nice clean line

  return 0;
}





# setup failing step bool
success=0  # 1 if failing, 0 if successful


# see if inputs are good
if [ $success == 0 ]; then
  if [ "$#" != 1 ]; then
    echo "not enough inputs! Inputs must be \"tryPasswordInjection\""
    echo "  \"tryPasswordInjection\" is a boolean value of \"0\" or \"1\" where \"0\" means the script will not attempt to use sudo -v periodically to avoid password timeout (which can fail which means lots of excess password input), and \"1\" means the script will attempt to use sudo -v periodically to avoid password timeout, meaning if successful, should only need password once at the beginning of the script. Note that of late, this has only been successful on PC's with more than 2 processors"
    success=1
  fi
fi

# set inputs to variables
tryPasswordInjection="${1}"


if [ $success == 0 ]; then
  echo "" # want a nice clean line
  echo "running buildEverything.sh script"
  echo "" # want a nice clean line
fi


### make sure the current working directory (baseDir) is the same location as the script executable so that everything gets placed in the right spots
if [ $success == 0 ]; then
  baseDirParent=$(dirname $baseDir)
  baseDirName=$(echo "${baseDir}" | sed 's/.*\///')
  baseDirParentName=$(echo "${baseDirParent}" | sed 's/.*\///')
  if [ ${baseDirName} != "include" ] && [ ${baseDirParentName} != "WRF-WindNinja-FarsiteScript" ]; then
    echo "!!!buildEverything.sh is not being run from the location of the executable!!!"
    success=1
  fi
fi


if [ $success == 0 ]; then
  if [ $tryPasswordInjection == 1 ]; then
    echo "running sudo -v so all future sudo commands shouldn't pause the process to ask for a password. Will run sudo -v at different points in the code to try to avoid password tracking timeout in bash, so if the script pauses to ask for passwords again, it means sudo -v wasn't run frequently enough to get bash to keep the password. Unfortunately the script is sometimes time consuming enough that the section for building WindNinja still asks for a password at least once even with password injection. If your CPU is slow enough (Windows subsystem of Ubuntu instead of VMWare), the number of passwords required when doing password injection can get really annoying."
    sudo -v
    echo "" # want a nice clean line
  fi
fi


### make sure extraLibs and extraApps directories exist, if not, make them
if [ $success == 0 ]; then
  if [ ! -d "${extraLibsDir}" ]; then
    echo "creating missing extraLibsDir "$extraLibsDir
    mkdir ${extraLibsDir}
    if [ ! -d "${extraLibsDir}" ]; then
      echo "!!!failed to create extraLibsDir!!!"
      success=1
    fi
  fi
fi

if [ $success == 0 ]; then
  if [ ! -d "${extraAppsDir}" ]; then
    echo "creating missing extraLibsDir "$extraAppsDir
    mkdir ${extraAppsDir}
    if [ ! -d "${extraAppsDir}" ]; then
      echo "!!!failed to create extraAppsDir!!!"
      success=1
    fi
  fi
fi


## now check installation and download all the needed libraries
if [ $success == 0 ]; then
  setupDownloadableLib "${extraLibsDir}" "${zlibDirName}" "${zlibTarDir}" "${zlibTarDirName}" "${zlibSource}" "${zlibLink}" "${zlibDir}" "${zlibTarDirName}" "${zlibBuildDir}" "${zlibCPPFLAGS}" "${zlibLDFLAGS}" "${zlibConfigure}" "{$tryPasswordInjection}"
  success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
  setupDownloadableLib "${extraLibsDir}" "${szlibDirName}" "${szlibTarDir}" "${szlibTarDirName}" "${szlibSource}" "${szlibLink}" "${szlibDir}" "${szlibTarDirName}" "${szlibBuildDir}" "${szlibCPPFLAGS}" "${szlibLDFLAGS}" "${szlibConfigure}" "{$tryPasswordInjection}"
  success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
  setupDownloadableLib "${extraLibsDir}" "${curlDirName}" "${curlTarDir}" "${curlTarDirName}" "${curlSource}" "${curlLink}" "${curlDir}" "${curlTarDirName}" "${curlBuildDir}" "${curlCPPFLAGS}" "${curlLDFLAGS}" "${curlConfigure}" "{$tryPasswordInjection}"
  success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
  setupDownloadableLib "${extraLibsDir}" "${hdf5DirName}" "${hdf5TarDir}" "${hdf5TarDirName}" "${hdf5Source}" "${hdf5Link}" "${hdf5Dir}" "${hdf5TarDirName}" "${hdf5BuildDir}" "${hdf5CPPFLAGS}" "${hdf5LDFLAGS}" "${hdf5Configure}" "{$tryPasswordInjection}"
  success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
  setupDownloadableLib "${extraLibsDir}" "${netcdf_cDirName}" "${netcdf_cTarDir}" "${netcdf_cTarDirName}" "${netcdf_cSource}" "${netcdf_cLink}" "${netcdf_cDir}" "${netcdf_cTarDirName}" "${netcdf_cBuildDir}" "${netcdf_cCPPFLAGS}" "${netcdf_cLDFLAGS}" "${netcdf_cConfigure}" "{$tryPasswordInjection}"
  success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
  setupDownloadableLib "${extraLibsDir}" "${netcdf_cxxDirName}" "${netcdf_cxxTarDir}" "${netcdf_cxxTarDirName}" "${netcdf_cxxSource}" "${netcdf_cxxLink}" "${netcdf_cxxDir}" "${netcdf_cxxTarDirName}" "${netcdf_cxxBuildDir}" "${netcdf_cxxCPPFLAGS}" "${netcdf_cxxLDFLAGS}" "${netcdf_cxxConfigure}" "{$tryPasswordInjection}"
  success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
  setupDownloadableLib "${extraLibsDir}" "${jasperDirName}" "${jasperTarDir}" "${jasperTarDirName}" "${jasperSource}" "${jasperLink}" "${jasperDir}" "${jasperTarDirName}" "${jasperBuildDir}" "${jasperCPPFLAGS}" "${jasperLDFLAGS}" "${jasperConfigure}" "{$tryPasswordInjection}"
  success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
  setupDownloadableLib "${extraLibsDir}" "${gdalDirName}" "${gdalTarDir}" "${gdalTarDirName}" "${gdalSource}" "${gdalLink}" "${gdalDir}" "${gdalTarDirName}" "${gdalBuildDir}" "${gdalCPPFLAGS}" "${gdalLDFLAGS}" "${gdalConfigure}" "{$tryPasswordInjection}"
  success=$? # result of last action, 0 if good, 1 if failed
fi


## now check for WindNinja and download if needed
if [ $success == 0 ]; then
  if [ $tryPasswordInjection == 1 ]; then
    sudo -v
  fi
  echo "" # want a nice clean line
  echo "checking for WindNinja"
  if [ ! -d "${windninjaDir}" ]; then
    echo "downloading WindNinja from "$windninjaSource
    cd $extraAppsDir
    git clone $windninjaLink
    if [ ! -d "${windninjaDir}" ]; then
      echo "!!!failed to download WindNinja!!!"
      success=1
    fi
  else
    echo "!warning, "$windninjaDir" already exists!"
  fi
fi

if [ $success == 0 ]; then
  if [ ! -d "${windninjaBuildDir}" ]; then
    echo "building WindNinja 3rd party libs"
    cd $windninjaScriptsDir
    if [ $tryPasswordInjection == 1 ]; then
      sudo -v
    fi
    ./build_deps.sh
    if [ $tryPasswordInjection == 1 ]; then
      sudo -v
    fi
    cd $windninjaDir
  else
    echo "!warning, "$windninjaBuildDir" already exists so build_deps.sh for WindNinja should already have been done. Not running build_deps.sh for WindNinja!"
  fi
fi

if [ $success == 0 ]; then
  if [ ! -d "${windninjaBuildDir}" ]; then
    echo "creating WindNinja build dir "$windninjaBuildDir
    cd $windninjaDir
    mkdir $windninjaBuildDir
    cd $windninjaBuildDir
    if [ ! -d "${windninjaBuildDir}" ]; then
      echo "!!!failed to make WindNinja build dir!!!"
      success=1
    fi
  else
    echo "!warning, "$windninjaBuildDir" already exists!"
  fi
fi

if [ $success == 0 ]; then
  if [ ! -f "${windninjaBuildDir}/src/cli/WindNinja_cli" ]; then
    echo "building WindNinja"
    cd $windninjaBuildDir
    cmake ..
    make -j4
    success=$? # result of last action, 0 if good, 1 if failed
    sudo ldconfig
    cd $extraAppsDir
    if [ $success == 0 ]; then
      echo "finished building WindNinja"
    else
      echo "!!!failed to build WindNinja!!!"
    fi
  else
    echo "!warning, WindNinja executable "${windninjaBuildDir}"/src/cli/WindNinja_cli already exits. Not running cmake or make on WindNinja!"
  fi
fi


## now check for farsite and download if needed
if [ $success == 0 ]; then
  if [ $tryPasswordInjection == 1 ]; then
    sudo -v
  fi
  echo "" # want a nice clean line
  echo "checking for farsite"
  if [ ! -d "${farsiteDir}" ]; then
    echo "downloading farsite from "$farsiteSource
    cd $extraAppsDir
    git clone $farsiteLink
    if [ ! -d "${farsiteDir}" ]; then
      echo "!!!failed to download farsite!!!"
      success=1
    fi
  else
    echo "!warning, "$farsiteDir" already exists!"
  fi
fi

if [ $success == 0 ]; then
  if [ ! -f "${farsiteSrcDir}/TestFARSITE" ]; then
    echo "building farsite"
    cd $farsiteSrcDir
    make -j$nCores
    success=$? # result of last action, 0 if good, 1 if failed
    cd $extraAppsDir
    if [ $success == 0 ]; then
      echo "finished building farsite"
    else
      echo "!!!failed to build farsite!!!"
    fi
  else
    echo "!warning, farsite executable "${farsiteSrcDir}"/TestFARSITE already exits. Not running make on farsite!"
  fi
fi


## now find and update all paths in the repo that to match the current base directory
if [ $success == 0 ]; then
  aboveBaseDir=$(dirname $baseDirParent)
  echo "" # want a nice clean line
  echo "finding all files with \""$varToReplace"\" in them to replace with the above current base directory \""$aboveBaseDir"\""
  preppedVarToReplace=$(sed 's/\//\\\//g' <<<"$varToReplace")
  preppedAboveBaseDir=$(sed 's/\//\\\//g' <<<"$aboveBaseDir")
  #preppedAboveBaseDir="\$scriptRoot"
  grep -rl $preppedVarToReplace $finalScriptDir --exclude-dir=.git --exclude-dir=include | xargs sed -i 's/'$preppedVarToReplace'/'$preppedAboveBaseDir'/g'
  success=$?
  if [ $success != 0 ]; then
    if [ $success == 123 ]; then
      echo "!warning, no stuff to replace!"
      success=0
    else
      echo "!!!failed during replace path text process!!!"
      success=1
    fi
  else
    echo "finished replacing text"
  fi
fi


## now attempt final build
if [ $success == 0 ]; then
  echo "" # want a nice clean line
  echo "trying to do final build"
  if [ ! -d "${finalBuildDir}" ]; then
    echo "creating final script build directory "$finalBuildDir
    cd $baseDir
    mkdir $finalBuildDir
    if [ ! -d "${finalBuildDir}" ]; then
      echo "!!!failed to create "$finalBuildDir"!!!"
      $success=1
    fi
  else
    echo "!warning, "$finalBuildDir" already exists!"
  fi
fi

if [ $success == 0 ]; then
  if [ ! -f "${finalScriptDir}/bin/WRF-WindNinja-FarsiteScript" ]; then
    echo "building final script"
    cd $finalBuildDir
    cmake $finalScriptDir
    make -j$nCores
    success=$? # result of last action, 0 if good, 1 if failed
    cd $baseDir
    if [ $success == 0 ]; then
      echo "finished building final script"
    else
      echo "!!!failed to build final script!!!"
    fi
  else
    echo "!warning, WRF-WindNinja-FarsiteScript executable "${finalScriptDir}"/bin/WRF-WindNinja-FarsiteScript already exits. Not running make on WRF-WindNinja-FarsiteScript!"
  fi
fi


echo "" # want a nice clean line
echo "returning to original directory"
cd $baseDir

if [ $success == 1 ]; then
  echo "" # want a fresh clean line
  echo "!!! failed at some point during the build process!!!"
else
  echo "" # want a fresh clean line
  echo "should have finished downloading and building all dependencies up to this point."
  echo "you can check to make sure what library paths were grabbed during the make process for your script by running the command \"ldd ~/script_executable\"."
fi


echo "" # want a fresh clean line
echo "finished build_depsh.sh script"
echo "" # want a fresh clean line


