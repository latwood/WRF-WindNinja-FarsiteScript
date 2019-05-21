#!/bin/bash

### the basic idea of this script is that you set the versions via link names for downloading the dependencies, set the directory locations and other stuff as needed for other packages that need built, then you just run this script and it will download and build all 3rd library packages and extra applications used by this script. It will also update the config/cmake file with the paths, and a .cpp file needed by the code with paths, as well as any paths needed in the examples dir, so that at the end of this script, an attempt to build the program is made using all these extra applications and libraries and you can run any example right after this script is built without modifying paths. If there are any problems found in any of the process, errors will be given so you can pinpoint what paths or variables set at the beginning of the script need changed.
### in order to get the paths correct for the final script build, it turns out you don't need to edit the .bashrc file to add exports to the LD_LIBRARY_PATH for each new path, or even export the paths in such a way in the middle of the process. Instead a config/cmake file is written from scratch with the new paths as found when building everything, so if you need to edit the way that file is made, that stuff isn't specified at the beginning of the script like all the other variables. However, if trying to run WindNinja without running a final "sudo make install", the environment has to be changed through other means.

### there are a few dependencies downloaded and installed via "sudo apt install" at the very beginning, and when WindNinja runs. This means the user will have to supply a password and approve the installations cause the script will pause when it his these steps. This script used to have a bunch of nice password injection stuff to try to avoid putting in your password so many times, but this failed to work effectively, so it has been scrapped.

### actually, whether the script is being used right is placed first before the section you should set all the variables, so be aware of that.




######## first check script is being used right and this is the include directory, the include directory in the repo ########

# setup failing step bool
success=0  # 1 if failing, 0 if successful

# see if inputs are good
if [ $success == 0 ]; then
	if [ "$#" != 0 ]; then
		echo "" # want a nice clean line
		echo "!!! too many inputs! There should be no inputs !!!"
		success=1
	fi
fi

# certain directories need set/checked first
currentDir=$(pwd)
currentDirName=$(echo "${currentDir}" | sed 's/.*\///')		# find name of current directory (removes path so grabs last folder name in path)
currentDirParent=$(dirname $currentDir)		# find parent directory (pulls off last folder name from path)
currentDirParentName=$(echo "${currentDirParent}" | sed 's/.*\///')		# find name of parent directory (removes path so grabs last folder name in path)
if [ ${currentDirName} != "include" ] && [ ${currentDirParentName} != "WRF-WindNinja-FarsiteScript" ]; then
	echo "!!! buildEverything_Ubuntu.sh is not being run from the location of the executable !!! Please run this script from the /WRF-WindNinja-FarsiteScript/include/ directory !!!"
	success=1
fi




############   set required variables for everything being done in the script   ############

# it is assumed the user should already have installed git and already done "sudo apt update" and "sudo apt upgrade".
aptgetInstallString="gcc g++ cmake m4"	# this has everything that will be run at the begginning, finishing "sudo apt-get install ".

nCores="8"  # this is the attempted number of cores when running any make process (except WindNinja which got mad when I tried using more than 4 cores during one of my builds). Could potentially change this value as a user to maybe get some speedup building gdal and netcdf libraries. Definitely important to somewhat get that annoying insert password trick to actually SOMEWHAT work.



### overall script variables

includeDir=$currentDir  # this is expected to be "$pathToRepo/WRF-WindNinja-FarsiteScript/include"  # this is defining the location inside the overall script directory to place the third party libraries, packages, and applications
baseDir=$currentDirParent	# this is expected to be "$pathToRepo/WRF-WindNinja-FarsiteScript" 	# this is defining the location of the overall script directory on your system after running "git clone" on the overall script repo
outputDir=$(dirname $baseDir)		# find parent directory (pulls off last folder name from path), this is expected to be "$pathToRepo"
extraAppsDir=$includeDir"/extraApps"				# this is defining the location inside the overall script directory for placing the third party applications
extraLibsDir=$includeDir"/extraLibs"				# this is defining the location inside the overall script directory for placing the third party libraries and packages
finalBuildDir=$baseDir"/build"						# this is defining the location of the overall script build directory, for when running "make" on the overall script



### third party lib variables

zlibLink="http://www.zlib.net/zlib-1.2.11.tar.gz"
zlibTarFormat="-xzf"
zlibTarDir=$extraLibsDir"/zlib-1.2.11.tar.gz"
zlibTarDirName=$extraLibsDir"/zlib-1.2.11"
zlibDir=$extraLibsDir"/zlib-1.2.11"
zlibBuildDir=$zlibDir"/build_zlib-1.2.11"

zlibCPPFLAGS=""
zlibLDFLAGS=""
zlibConfigure="./configure --prefix="$zlibBuildDir
zlib_shouldMakeClean=0	# set to 1 if you want the unzipped folder deleted before running make again, which means a repeat of the unpacking process. Set this on whichever lib failed to build


szlibLink="ftp://ftp.unidata.ucar.edu/pub/netcdf/netcdf-4/szip-2.1.tar.gz"
szlibTarFormat="-xzf"
szlibTarDir=$extraLibsDir"/szip-2.1.tar.gz"
szlibTarDirName=$extraLibsDir"/szip-2.1"
szlibDir=$extraLibsDir"/szip-2.1"
szlibBuildDir=$szlibDir"/build_szip-2.1"

szlibCPPFLAGS=""
szlibLDFLAGS=""
szlibConfigure="./configure --prefix="$szlibBuildDir
szlib_shouldMakeClean=0	# set to 1 if you want the unzipped folder deleted before running make again, which means a repeat of the unpacking process. Set this on whichever lib failed to build


curlLink="https://curl.haxx.se/download/curl-7.61.1.tar.gz"
curlTarFormat="-xzf"
curlTarDir=$extraLibsDir"/curl-7.61.1.tar.gz"
curlTarDirName=$extraLibsDir"/curl-7.61.1"
curlDir=$extraLibsDir"/curl-7.61.1"
curlBuildDir=$curlDir"/build_curl-7.61.1"

curlCPPFLAGS=""
curlLDFLAGS=""
curlConfigure="./configure --prefix="$curlBuildDir" --with-zlib="$zlibBuildDir
curl_shouldMakeClean=0	# set to 1 if you want the unzipped folder deleted before running make again, which means a repeat of the unpacking process. Set this on whichever lib failed to build


hdf5Link="https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.10/hdf5-1.10.2/src/hdf5-1.10.2.tar.gz"
hdf5TarFormat="-xzf"
hdf5TarDir=$extraLibsDir"/hdf5-1.10.2.tar.gz"
hdf5TarDirName=$extraLibsDir"/hdf5-1.10.2"
hdf5Dir=$extraLibsDir"/hdf5-1.10.2"
hdf5BuildDir=$hdf5Dir"/build_hdf5-1.10.2"

hdf5CPPFLAGS=""
hdf5LDFLAGS=""
hdf5Configure="./configure --prefix="$hdf5BuildDir" --enable-hl --with-szlib="$szlibBuildDir" --with-zlib="$zlibBuildDir
hdf5_shouldMakeClean=0	# set to 1 if you want the unzipped folder deleted before running make again, which means a repeat of the unpacking process. Set this on whichever lib failed to build


netcdf_cLink="https://github.com/Unidata/netcdf-c/archive/v4.6.1.tar.gz"
netcdf_cTarFormat="-xzf"
netcdf_cTarDir=$extraLibsDir"/v4.6.1.tar.gz"
netcdf_cTarDirName=$extraLibsDir"/netcdf-c-4.6.1"
netcdf_cDir=$extraLibsDir"/netcdf-c-4.6.1"
netcdf_cBuildDir=$netcdf_cDir"/build_netcdf-c-4.6.1"

netcdf_cCPPFLAGS="-I"$zlibBuildDir"/include -I"$curlBuildDir"/include -I"$hdf5BuildDir"/include"
netcdf_cLDFLAGS="-L"$zlibBuildDir"/lib -L"$curlBuildDir"/lib -L"$hdf5BuildDir"/lib"
netcdf_cConfigure="./configure --prefix="$netcdf_cBuildDir
netcdf_c_shouldMakeClean=0	# set to 1 if you want the unzipped folder deleted before running make again, which means a repeat of the unpacking process. Set this on whichever lib failed to build


netcdf_cxxLink="https://github.com/Unidata/netcdf-cxx4/archive/v4.3.0.tar.gz"
netcdf_cxxTarFormat="-xzf"
netcdf_cxxTarDir=$extraLibsDir"/v4.3.0.tar.gz"
netcdf_cxxTarDirName=$extraLibsDir"/netcdf-cxx4-4.3.0"
netcdf_cxxDir=$extraLibsDir"/netcdf-cxx-4.3.0"
netcdf_cxxBuildDir=$netcdf_cxxDir"/build_netcdf-cxx-4.3.0"

netcdf_cxxCPPFLAGS="-I"$hdf5BuildDir"/include -I"$netcdf_cBuildDir"/include"
netcdf_cxxLDFLAGS="-L"$hdf5BuildDir"/lib -L"$netcdf_cBuildDir"/lib"
netcdf_cxxConfigure="./configure --prefix="$netcdf_cxxBuildDir
netcdf_cxx_shouldMakeClean=0	# set to 1 if you want the unzipped folder deleted before running make again, which means a repeat of the unpacking process. Set this on whichever lib failed to build


jasperLink="https://launchpad.net/ubuntu/+archive/primary/+sourcefiles/jasper/1.900.1-14ubuntu3/jasper_1.900.1.orig.tar.gz"
jasperTarFormat="-xzf"
jasperTarDir=$extraLibsDir"/jasper_1.900.1.orig.tar.gz"
jasperTarDirName=$extraLibsDir"/jasper-1.900.1"
jasperDir=$extraLibsDir"/jasper-1.900.1"
jasperBuildDir=$jasperDir"/build_jasper-1.900.1"

jasperCPPFLAGS=""
jasperLDFLAGS=""
jasperConfigure="./configure --prefix="$jasperBuildDir" --enable-shared"
jasper_shouldMakeClean=0	# set to 1 if you want the unzipped folder deleted before running make again, which means a repeat of the unpacking process. Set this on whichever lib failed to build


popplerLink="http://poppler.freedesktop.org/poppler-0.23.4.tar.xz"
popplerTarFormat="-xvf"
popplerTarDir=$extraLibsDir"/poppler-0.23.4.tar.xz"
popplerTarDirName=$extraLibsDir"/poppler-0.23.4"
popplerDir=$extraLibsDir"/poppler-0.23.4"
popplerBuildDir=$popplerDir"/build_poppler-0.23.4"

popplerCPPFLAGS=""
popplerLDFLAGS=""
popplerConfigure="./configure --prefix="$popplerBuildDir" --enable-xpdf-headers"
poppler_shouldMakeClean=0	# set to 1 if you want the unzipped folder deleted before running make again, which means a repeat of the unpacking process. Set this on whichever lib failed to build


projLink="http://download.osgeo.org/proj/proj-4.8.0.tar.gz"
projTarFormat="xzf"
projTarDir=$extraLibsDir"/proj-4.8.0.tar.gz"
projTarDirName=$extraLibsDir"/proj-4.8.0"
projDir=$extraLibsDir"/proj-4.8.0"
projBuildDir=$projDir"/build_proj-4.8.0"

projCPPFLAGS=""
projLDFLAGS=""
projConfigure="./configure --prefix="$projBuildDir
proj_shouldMakeClean=0	# set to 1 if you want the unzipped folder deleted before running make again, which means a repeat of the unpacking process. Set this on whichever lib failed to build


gdalLink="http://download.osgeo.org/gdal/2.0.3/gdal-2.0.3.tar.gz"
gdalTarFormat="-xzf"
gdalTarDir=$extraLibsDir"/gdal-2.0.3.tar.gz"
gdalTarDirName=$extraLibsDir"/gdal-2.0.3"
gdalDir=$extraLibsDir"/gdal-2.0.3"
gdalBuildDir=$gdalDir"/build_gdal-2.0.3"

gdalCPPFLAGS=""
gdalLDFLAGS=""
gdalConfigure="./configure --prefix="$gdalBuildDir" --with-curl="$curlBuildDir" --with-jasper="$jasperBuildDir" --with-netcdf="$netcdf_cBuildDir" --with-hdf5="$hdf5BuildDir" --with-libz="$zlibBuildDir"  --with-poppler="$popplerBuildDir" --with-static-proj4="$projBuildDir
gdal_shouldMakeClean=0	# set to 1 if you want the unzipped folder deleted before running make again, which means a repeat of the unpacking process. Set this on whichever lib failed to build



### third party app variables

windNinjaDir=$extraAppsDir"/windninja"				# this is defining the location inside the overall script directory for placing the WindNinja application
windNinjaLink="https://github.com/firelab/windninja.git"	# this is the link to the WindNinja script Repo
windNinjaBuildDir=$windNinjaDir"/build_windninja"			# this is defining the location of the WindNinja build directory, for when running "make" on the WindNinja application

windNinjaBranch="3.5.2"								# this is for defining which version of WindNinja (which branch) to switch to by running "git checkout $branch", so defining which version of WindNinja to compile
windninjaScriptsDir=$windNinjaDir"/scripts"


farsiteDir=$extraAppsDir"/farsite"					# this is defining the location inside the overall script directory for placing the Farsite application
farsiteLink="https://github.com/firelab/farsite.git"	# this is the link to the Farsite script Repo
farsiteSrcDir=$farsiteDir"/src"						# this is the place where "make" needs run on Farsite to compile it



### third party app their dependency libary variables

WindNinja_popplerLink="http://poppler.freedesktop.org/poppler-0.23.4.tar.xz"
WindNinja_popplerTarFormat="-xvf"
WindNinja_popplerTarDir=$windninjaScriptsDir"/poppler-0.23.4.tar.xz"
WindNinja_popplerTarDirName=$windninjaScriptsDir"/poppler-0.23.4"
WindNinja_popplerDir=$windninjaScriptsDir"/poppler-0.23.4"
WindNinja_popplerBuildDir=$WindNinja_popplerDir"/build_poppler-0.23.4"

WindNinja_popplerCPPFLAGS=""
WindNinja_popplerLDFLAGS=""
WindNinja_popplerConfigure="./configure --prefix="$WindNinja_popplerBuildDir" --enable-xpdf-headers"
WindNinja_poppler_shouldMakeClean=0	# set to 1 if you want the unzipped folder deleted before running make again, which means a repeat of the unpacking process. Set this on whichever lib failed to build

WindNinja_projLink="http://download.osgeo.org/proj/proj-4.8.0.tar.gz"
WindNinja_projTarFormat="xvfz"
WindNinja_projTarDir=$windninjaScriptsDir"/proj-4.8.0.tar.gz"
WindNinja_projTarDirName=$windninjaScriptsDir"/proj-4.8.0"
WindNinja_projDir=$windninjaScriptsDir"/proj-4.8.0"
WindNinja_projBuildDir=$WindNinja_projDir"/build_proj-4.8.0"

WindNinja_projCPPFLAGS=""
WindNinja_projLDFLAGS=""
WindNinja_projConfigure="./configure --prefix="$WindNinja_projBuildDir
WindNinja_proj_shouldMakeClean=0	# set to 1 if you want the unzipped folder deleted before running make again, which means a repeat of the unpacking process. Set this on whichever lib failed to build

WindNinja_gdalLink="http://download.osgeo.org/gdal/2.0.3/gdal-2.0.3.tar.gz"
WindNinja_gdalTarFormat="-xvf"
WindNinja_gdalTarDir=$windninjaScriptsDir"/gdal-2.0.3.tar.gz"
WindNinja_gdalTarDirName=$windninjaScriptsDir"/gdal-2.0.3"
WindNinja_gdalDir=$windninjaScriptsDir"/gdal-2.0.3"
WindNinja_gdalBuildDir=$WindNinja_gdalDir"/build_gdal-2.0.3"

WindNinja_gdalCPPFLAGS=""
WindNinja_gdalLDFLAGS=""
WindNinja_gdalConfigure="./configure --prefix="$WindNinja_gdalBuildDir"  --with-poppler="$WindNinja_popplerBuildDir" --with-static-proj4="$WindNinja_projBuildDir
WindNinja_gdal_shouldMakeClean=0	# set to 1 if you want the unzipped folder deleted before running make again, which means a repeat of the unpacking process. Set this on whichever lib failed to build



### replacing paths in files variables

varToReplace="\$scriptRoot"							# this is the predefined placeholder text for the user paths in all the examples and src files, to be replaced with the user path found by this script so compiling can be done by this script in one step (instead of the user changing these paths manually)
###echo "varToReplace is "$varToReplace
###varToReplace="/home/atw09001/src/combinationScript-cmake"	# this is a possible alternative placeholder text, say you want to undo the change of the path back to the original placeholder for pushing changes to the examples




############# define functions used by all the other processes ##############
downloadAndUnpackLib()		### this function downloads and unzips a given 3rd party lib, then if the name is not what is wanted, renames the unpacked the directory to what is wanted
{
	if [ "$#" != 7 ]; then
		echo "" # want a nice clean line
		echo "!!!Incorrect Number of parameters for downloadAndUnpackLib!!!"
		echo "need 7 parameters: \"extraLibsDir\" \"libLink\" \"tarFormat\" \"libTarDir\" \"libTarDirName\" \"libDir\" \"libBuildDir\""
		echo "" # want a nice clean line
		return 1
	fi

	local extraLibsDir="${1}"
	local libLink="${2}"
	local tarFormat="${3}"
	local libTarDir="${4}"
	local libTarDirName="${5}"
	local libDir="${6}"
	local libBuildDir="${7}"
	
	
	### make some intermediate variables that used to be passed in, but technically come from these input variables
	###currentDir=$(pwd)
	###currentDirName=$(echo "${currentDir}" | sed 's/.*\///')		# find name of current directory (removes path so grabs last folder name in path)
	###currentDirParent=$(dirname $currentDir)		# find parent directory (pulls off last folder name from path)
	###currentDirParentName=$(echo "${currentDirParent}" | sed 's/.*\///')		# find name of parent directory (removes path so grabs last folder name in path)
	local libName=$(echo "${libDir}" | sed 's/.*\///')	# take the directory off of libDir
	

	echo "" # want a nice clean line
	echo "checking for "$libName

	if [ ! -f "${libTarDir}" ]; then
		echo "entering "${extraLibsDir}" directory"
		cd $extraLibsDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running cd command !!!"
			return 1
		fi
		echo "downloading "${libTarDir}" from "$libLink
		wget ${libLink}
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running wget command !!!"
			return 1
		fi
	else
		echo $libTarDir" already exists so skipping tar download"
	fi

	if [ ! -d "${libDir}" ]; then
		echo "entering "${extraLibsDir}" directory"
		cd $extraLibsDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running cd command !!!"
			return 1
		fi
		echo "unpacking "$libTarDir
		tar $tarFormat $libTarDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running tar command with tarFormat \"${tarFormat}\" !!!"
			return 1
		fi
		if [ "${libTarDirName}" != "${libDir}" ]; then
			echo "renaming tar directory "${libTarDirName}" to "${libDir}
			mv $libTarDirName $libDir
			success=$?
			if [ $success != 0 ]; then
				echo "!!! error running mv command !!!"
				return 1
			fi
		fi
	else
		echo $libDir" already exists so skipping tar unpacking"
	fi

	echo "finished setup for "$libName
	echo "" # add a nice clean line

	return 0;

}


buildLib()		### this function is to be run on libs after they've been downloaded and unpacked, basically just builds the lib
{
	if [ "$#" != 11 ]; then
		echo "" # want a nice clean line
		echo "!!!Incorrect Number of parameters for buildLib!!!"
		echo "need 10 parameters: \"extraLibsDir\" \"nCores\" \"libDir\" \"libBuildDir\" \"libCPPFLAGS\" \"libLDFLAGS\" \"libConfigure\" \"shouldMakeClean\" \"tarFormat\" \"libTarDir\" \"libTarDirName\""
		echo "" # want a nice clean line
		return 1
	fi

	local extraLibsDir="${1}"
	local nCores="${2}"
	local libDir="${3}"
	local libBuildDir="${4}"
	local libCPPFLAGS="${5}"
	local libLDFLAGS="${6}"
	local libConfigure="${7}"
	local shouldMakeClean="${8}"
	local tarFormat="${9}"
	local libTarDir="${10}"
	local libTarDirName="${11}"


	### make some intermediate variables that used to be passed in, but technically come from these input variables
	###currentDir=$(pwd)
	###currentDirParent=$(dirname $currentDir)		# find parent directory (pulls off last folder name from path)
	###currentDirName=$(echo "${currentDir}" | sed 's/.*\///')		# find name of current directory (removes path so grabs last folder name in path)
	###currentDirParentName=$(echo "${currentDirParent}" | sed 's/.*\///')		# find name of parent directory (removes path so grabs last folder name in path)
	local libName=$(echo "${libDir}" | sed 's/.*\///')	# take the directory off of libDir
	

	echo "" # want a nice clean line
	echo "checking if "$libName" needs to be built"



	if [[ $shouldMakeClean == 1 && -d "${libBuildDir}" ]]; then
		echo "shouldMakeClean is \"${shouldMakeClean}\" so deleting old ${libDir} directory for a clean build"
		rm -r ${libDir}
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running rm -r command !!!"
			return 1
		fi
		echo "running the download and unpacking function again, should ignore the downloading part"
		downloadAndUnpackLib "${extraLibsDir}" "null" "${tarFormat}" "${libTarDir}" "${libTarDirName}" "${libDir}" "${libBuildDir}"		### notice the link won't matter cause it will detect that the tar dir exists
		success=$? # result of last action, 0 if good, 1 if failed
		if [ $success != 0 ]; then
			echo "!!! error running downloadAndUnpackLib function !!!"
			return 1
		fi
	fi


	if [ ! -d "${libBuildDir}" ]; then
		echo "entering ${libDir} directory"		
		cd $libDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running cd command !!!"
			return 1
		fi
		echo "running configure "$libConfigure
		export CPPFLAGS=$libCPPFLAGS
		export LDFLAGS=$libLDFLAGS
		${libConfigure}  # run the passed in configure command
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running configure command !!!"
			return 1
		fi
		echo "building "$libDirName
		make -j$nCores
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running make command !!!"
			return 1
		fi
		echo "running make install (not sudo make install)"
		make install
		if [ $success != 0 ]; then
			echo "!!! error running make install command !!!"
			return 1
		fi
		echo "returning to ${extraLibsDir} directory"
		cd $extraLibsDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running cd command !!!"
			return 1
		fi
	else
		echo "${libBuildDir} already exists and shouldMakeClean is set to ${shouldMakeClean} so skipping build process"
	fi

	
	echo "finished building "$libName
	echo "" # add a nice clean line

	return 0;
}


############# end defining functions to be used by all the other processes ################



############## !!! now start doing stuff with the script !!! ################



if [ $success == 0 ]; then
	echo "" # want a nice clean line
	echo "running buildEverything_Ubuntu.sh script"
	echo "" # want a nice clean line
fi


# it is assumed the user should already have installed git and already done "sudo apt update" and "sudo apt upgrade". Apparently apt is bad for scripts, apt-get is good for scripts.
if [ $success == 0 ]; then
	echo "before doing anything else, \"sudo apt-get install\" will be run so you will need to supply your password and accept installations of the following packages: \""$aptgetInstallString
	echo ""  # want a nice clean line
	echo "this script assumes you have already installed git and run \"sudo apt update\" and \"sudo apt upgrade\". Expect the script to ask for password again when the script reaches the WindNinja installation step, as WindNinja runs \"sudo apt install\" for some more packages."
	echo ""  # want a nice clean line
fi


if [ $success == 0 ]; then
	echo "running \"sudo apt-get install "$aptgetInstallString"\""
	sudo apt-get install $aptgetInstallString
	success=$?
	if [ $success != 0 ]; then
		echo "!!! error running sudo apt-get install command !!!"
		success=1
	fi
fi


######### first create all the required source directories and build directories ##########

### create the final build directory for later use
if [ $success == 0 ]; then
    echo "" # add extra line
    if [ ! -d "${finalBuildDir}" ]; then
        echo "creating finalBuildDir \""$finalBuildDir"\""
        mkdir $finalBuildDir
        success=$?
        if [ $success != 0 ]; then
            echo "!!! error running mkdir command !!!"
            success=1
        fi
    else
        echo "finalBuildDir \""$finalBuildDir"\" already exists. Skipping finalBuildDir creation step"
    fi
fi

### create the extra applications directory for later use
if [ $success == 0 ]; then
	echo "" # add extra line
	if [ ! -d "${extraAppsDir}" ]; then
		echo "creating extraAppsDir \""$extraAppsDir"\""
		mkdir $extraAppsDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running mkdir command !!!"
			success=1
		fi
	else
		echo "extraAppsDir \""$extraAppsDir"\" already exists. Skipping extraAppsDir creation step"
	fi
fi

### create the extra libs directory for later use
if [ $success == 0 ]; then
	echo "" # add extra line
	if [ ! -d "${extraLibsDir}" ]; then
		echo "creating extraLibsDir \""$extraLibsDir"\""
		mkdir $extraLibsDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running mkdir command !!!"
			success=1
		fi
	else
		echo "extraLibsDir \""$extraLibsDir"\" already exists. Skipping extraLibsDir creation step"
	fi
fi

### download via "git clone" the WindNinja script directory
if [ $success == 0 ]; then
	echo "" # add extra line
	if [ ! -d "${windNinjaDir}" ]; then
		echo "entering \""$extraAppsDir"\""
		cd $extraAppsDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running cd command !!!"
			success=1
		else
			echo "running git clone on WindNinja \""$windNinjaLink"\""
			git clone $windNinjaLink
			success=$?
			if [ $success != 0 ]; then
				echo "!!! error running git clone command !!!"
				success=1
			else
				echo "entering WindNinja directory"
				cd windninja
				success=$?
				if [ $success != 0 ]; then
					echo "!!! error running cd command !!!"
					success=1
				else
					echo "changing WindNinja branch to hopefully stable "$windNinjaBranch" branch"
					git checkout $windNinjaBranch
					success=$?
					if [ $success != 0 ]; then
						echo "!!! error running git checkout command !!!"
						success=1
					fi
				fi
			fi
		fi
	else
		echo "windNinjaDir \""$windNinjaDir"\" already exists. Skipping repo cloning step for WindNinja"
	fi
fi

### create the WindNinja build directory for later use
if [ $success == 0 ]; then
	echo "" # add extra line
	if [ ! -d "${windNinjaBuildDir}" ]; then
		echo "creating windNinjaBuildDir \""$windNinjaBuildDir"\""
		mkdir $windNinjaBuildDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running mkdir command !!!"
			success=1
		fi
	else
		echo "windNinjaBuildDir \""$windNinjaBuildDir"\" already exists. Skipping windNinjaBuildDir creation step"
	fi
fi

### download via "git clone" the Farsite script directory
if [ $success == 0 ]; then
	echo "" # add extra line
	if [ ! -d "${farsiteDir}" ]; then
		echo "entering \""$extraAppsDir"\""
		cd $extraAppsDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running cd command !!!"
			success=1
		else
			echo "running git clone on Farsite \""$farsiteLink"\""
			git clone $farsiteLink
			success=$?
			if [ $success != 0 ]; then
				echo "!!! error running git clone command !!!"
				success=1
			fi
		fi
	else
		echo "farsiteDir \""$farsiteDir"\" already exists. Skipping repo cloning step for farsite"
	fi
fi

######## download via "wget" and unpack with "tar" each of the third party packages required for the overall script ###########

if [ $success == 0 ]; then
	downloadAndUnpackLib "${extraLibsDir}" "${zlibLink}" "${zlibTarFormat}" "${zlibTarDir}" "${zlibTarDirName}" "${zlibDir}" "${zlibBuildDir}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	downloadAndUnpackLib "${extraLibsDir}" "${szlibLink}" "${szlibTarFormat}" "${szlibTarDir}" "${szlibTarDirName}" "${szlibDir}" "${szlibBuildDir}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	downloadAndUnpackLib "${extraLibsDir}" "${curlLink}" "${curlTarFormat}" "${curlTarDir}" "${curlTarDirName}" "${curlDir}" "${curlBuildDir}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	downloadAndUnpackLib "${extraLibsDir}" "${hdf5Link}" "${hdf5TarFormat}" "${hdf5TarDir}" "${hdf5TarDirName}" "${hdf5Dir}" "${hdf5BuildDir}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	downloadAndUnpackLib "${extraLibsDir}" "${netcdf_cLink}" "${netcdf_cTarFormat}" "${netcdf_cTarDir}" "${netcdf_cTarDirName}" "${netcdf_cDir}" "${netcdf_cBuildDir}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	downloadAndUnpackLib "${extraLibsDir}" "${netcdf_cxxLink}" "${netcdf_cxxTarFormat}" "${netcdf_cxxTarDir}" "${netcdf_cxxTarDirName}" "${netcdf_cxxDir}" "${netcdf_cxxBuildDir}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	downloadAndUnpackLib "${extraLibsDir}" "${jasperLink}" "${jasperTarFormat}" "${jasperTarDir}" "${jasperTarDirName}" "${jasperDir}" "${jasperBuildDir}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	downloadAndUnpackLib "${extraLibsDir}" "${popplerLink}" "${popplerTarFormat}" "${popplerTarDir}" "${popplerTarDirName}" "${popplerDir}" "${popplerBuildDir}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	downloadAndUnpackLib "${extraLibsDir}" "${projLink}" "${projTarFormat}" "${projTarDir}" "${projTarDirName}" "${projDir}" "${projBuildDir}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	downloadAndUnpackLib "${extraLibsDir}" "${gdalLink}" "${gdalTarFormat}" "${gdalTarDir}" "${gdalTarDirName}" "${gdalDir}" "${gdalBuildDir}"
	success=$? # result of last action, 0 if good, 1 if failed
fi



######## download and install via "sudo apt install" as well as download via "wget" and unpack with "tar" each of the third party packages required specifically for WindNinja, all by running WindNinja's "build_deps.sh" script ############

if [ $success == 9999 ]; then
	echo "entering windninja scripts dir ${windninjaScriptsDir}"
	cd $windninjaScriptsDir
	success=$?
	if [ $success != 0 ]; then
		echo "!!! error running cd command !!!"
		success=1
	else
		echo "building WindNinja 3rd party libs"
		./build_deps.sh
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running WindNinja's build_deps.sh script !!!"
			success=1
		fi
	fi
fi


if [ $success == 0 ]; then
	echo "running sudo apt install for WindNinja required 3rd party stuff"
	sudo apt-get install libfontconfig1-dev libcurl4-gnutls-dev libnetcdf-dev qt4-dev-tools libqtwebkit-dev libboost-program-options-dev libboost-date-time-dev libgeos-dev libboost-test-dev
	success=$?
	if [ $success != 0 ]; then
		echo "!!! error running sudo apt install for WindNinja 3rd party lib binary dependencies !!!"
		success=1
	fi
fi

if [ $success == 0 ]; then
	downloadAndUnpackLib "${windninjaScriptsDir}" "${WindNinja_popplerLink}" "${WindNinja_popplerTarFormat}" "${WindNinja_popplerTarDir}" "${WindNinja_popplerTarDirName}" "${WindNinja_popplerDir}" "${WindNinja_popplerBuildDir}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	downloadAndUnpackLib "${windninjaScriptsDir}" "${WindNinja_projLink}" "${WindNinja_projTarFormat}" "${WindNinja_projTarDir}" "${WindNinja_projTarDirName}" "${WindNinja_projDir}" "${WindNinja_projBuildDir}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	downloadAndUnpackLib "${windninjaScriptsDir}" "${WindNinja_gdalLink}" "${WindNinja_gdalTarFormat}" "${WindNinja_gdalTarDir}" "${WindNinja_gdalTarDirName}" "${WindNinja_gdalDir}" "${WindNinja_gdalBuildDir}"
	success=$? # result of last action, 0 if good, 1 if failed
fi



############### now need to find and update all paths in the combo script repo to match the current base directory ##############
### this should eventually be moved into a separate utility script, named something like "keywordReplacer", which will then need to be called here.
if [ $success == 0 ]; then
	echo "entering baseDir "$baseDir
	cd $baseDir
	success=$?
	if [ $success != 0 ]; then
		echo "!!! could not execute cd command !!!"
		success=1
	else
		aboveBaseDir=$(dirname $baseDir)
		echo "aboveBaseDir = "$aboveBaseDir
		echo "" # want a nice clean line
		echo "finding all files with \""$varToReplace"\" in them to replace with the above current base directory \""$aboveBaseDir"\""
		preppedVarToReplace=$(sed 's/\//\\\//g' <<<"$varToReplace")
		preppedAboveBaseDir=$(sed 's/\//\\\//g' <<<"$aboveBaseDir")
		#preppedAboveBaseDir="\/home/latwood/src"
		#preppedAboveBaseDir="\$scriptRoot"
		grep -rl $preppedVarToReplace $baseDir --exclude-dir=.git --exclude-dir=include --exclude=readme | xargs sed -i 's/'$preppedVarToReplace'/'$preppedAboveBaseDir'/g'
		success=$?
		if [ $success != 0 ]; then
			if [ $success == 123 ]; then
				echo "!warning, no stuff to replace!"
				success=0
			else
				echo "!!! failed during replace path text process !!!"
				success=1
			fi
		else
			echo "finished replacing text"
		fi
	fi
fi


############ now update local git ignore stuff so that example files that are tracked can still be ignored, so it is less easy to accidentally make changes to them ############
### this should eventually be moved into a separate utility script, named something like "ignoreModifiedButUnchangingGitFiles" or "ignoreTrackedGitFiles", which will then need to be called here.
if [ $success == 0 ]; then
	echo "" # want a nice clean line
	echo "entering ${baseDir} directory"
	cd $baseDir
	success=$?
	if [ $success != 0 ]; then
		echo "!!! error running cd command !!!"
		success=1
	else
		echo "running \"git update-index --assume-unchanged\" on files changed by replacing \""$varToReplace\"
		git update-index --assume-unchanged src/getFarsitePath.cpp src/getWindNinjaPath.cpp config/default.cmake
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running git update-index --assume-unchanged command on src/getFarsitePath.cpp src/getWindNinjaPath.cpp config/default.cmake files !!!"
			success=1
		fi
		if [ $success == 0 ]; then
			examplesDir=$baseDir"/examples"
			echo "running \"git update-index --assume-unchanged\" on examples dir ${examplesDir} files"
			git ls-files -- $examplesDir | xargs -l git update-index --assume-unchanged
			success=$?
			if [ $success != 0 ]; then
				echo "!!! error running git update-index --assume-unchanged command on examples dir !!!"
				success=1
			fi
		fi
	fi
	if [ $success == 0 ]; then
		echo "successfully updated local git ignore stuff. If you need to git add a specific ignored file, run the command \"git update-index --no-assume-unchanged\" on the file you want to add"
	fi
fi




########## now need to compile all the third party libraries and packages for the overall script ###########

if [ $success == 0 ]; then
	buildLib "${extraLibsDir}" "${nCores}" "${zlibDir}" "${zlibBuildDir}" "${zlibCPPFLAGS}" "${zlibLDFLAGS}" "${zlibConfigure}" "${zlib_shouldMakeClean}" "${zlibTarFormat}" "${zlibTarDir}" "${zlibTarDirName}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	buildLib "${extraLibsDir}" "${nCores}" "${szlibDir}" "${szlibBuildDir}" "${szlibCPPFLAGS}" "${szlibLDFLAGS}" "${szlibConfigure}" "${szlib_shouldMakeClean}" "${szlibTarFormat}" "${szlibTarDir}" "${szlibTarDirName}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	buildLib "${extraLibsDir}" "${nCores}" "${curlDir}" "${curlBuildDir}" "${curlCPPFLAGS}" "${curlLDFLAGS}" "${curlConfigure}" "${curl_shouldMakeClean}" "${curlTarFormat}" "${curlTarDir}" "${curlTarDirName}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	buildLib "${extraLibsDir}" "${nCores}" "${hdf5Dir}" "${hdf5BuildDir}" "${hdf5CPPFLAGS}" "${hdf5LDFLAGS}" "${hdf5Configure}" "${hdf5_shouldMakeClean}" "${hdf5TarFormat}" "${hdf5TarDir}" "${hdf5TarDirName}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	buildLib "${extraLibsDir}" "${nCores}" "${netcdf_cDir}" "${netcdf_cBuildDir}" "${netcdf_cCPPFLAGS}" "${netcdf_cLDFLAGS}" "${netcdf_cConfigure}" "${netcdf_c_shouldMakeClean}" "${netcdf_cTarFormat}" "${netcdf_cTarDir}" "${netcdf_cTarDirName}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	buildLib "${extraLibsDir}" "${nCores}" "${netcdf_cxxDir}" "${netcdf_cxxBuildDir}" "${netcdf_cxxCPPFLAGS}" "${netcdf_cxxLDFLAGS}" "${netcdf_cxxConfigure}" "${netcdf_cxx_shouldMakeClean}" "${netcdf_cxxTarFormat}" "${netcdf_cxxTarDir}" "${netcdf_cxxTarDirName}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	buildLib "${extraLibsDir}" "${nCores}" "${jasperDir}" "${jasperBuildDir}" "${jasperCPPFLAGS}" "${jasperLDFLAGS}" "${jasperConfigure}" "${jasper_shouldMakeClean}" "${jasperTarFormat}" "${jasperTarDir}" "${jasperTarDirName}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	buildLib "${extraLibsDir}" "${nCores}" "${popplerDir}" "${popplerBuildDir}" "${popplerCPPFLAGS}" "${popplerLDFLAGS}" "${popplerConfigure}" "${poppler_shouldMakeClean}" "${popplerTarFormat}" "${popplerTarDir}" "${popplerTarDirName}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	buildLib "${extraLibsDir}" "${nCores}" "${projDir}" "${projBuildDir}" "${projCPPFLAGS}" "${projLDFLAGS}" "${projConfigure}" "${proj_shouldMakeClean}" "${projTarFormat}" "${projTarDir}" "${projTarDirName}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	echo "running cp on proj header file"
	cp ${projBuildDir}/include/proj_api.h ${projBuildDir}/lib
	success=$?
	if [ $success != 0 ]; then
		echo "!!! error running cp command !!!"
		success=1
	fi
fi

if [ $success == 0 ]; then
	buildLib "${extraLibsDir}" "${nCores}" "${gdalDir}" "${gdalBuildDir}" "${gdalCPPFLAGS}" "${gdalLDFLAGS}" "${gdalConfigure}" "${gdal_shouldMakeClean}" "${gdalTarFormat}" "${gdalTarDir}" "${gdalTarDirName}"
	success=$? # result of last action, 0 if good, 1 if failed
fi




############# now attempt to build third party libraries for third party applications   #########################

if [ $success == 0 ]; then
	buildLib "${windninjaScriptsDir}" "${nCores}" "${WindNinja_popplerDir}" "${WindNinja_popplerBuildDir}" "${WindNinja_popplerCPPFLAGS}" "${WindNinja_popplerLDFLAGS}" "${WindNinja_popplerConfigure}" "${WindNinja_poppler_shouldMakeClean}" "${WindNinja_popplerTarFormat}" "${WindNinja_popplerTarDir}" "${WindNinja_popplerTarDirName}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	buildLib "${windninjaScriptsDir}" "${nCores}" "${WindNinja_projDir}" "${WindNinja_projBuildDir}" "${WindNinja_projCPPFLAGS}" "${WindNinja_projLDFLAGS}" "${WindNinja_projConfigure}" "${WindNinja_proj_shouldMakeClean}" "${WindNinja_projTarFormat}" "${WindNinja_projTarDir}" "${WindNinja_projTarDirName}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	echo "running cp on proj header file"
	cp ${WindNinja_projBuildDir}/include/proj_api.h ${WindNinja_projBuildDir}/lib
	success=$?
	if [ $success != 0 ]; then
		echo "!!! error running cp command !!!"
		success=1
	fi
fi

if [ $success == 0 ]; then
	buildLib "${windninjaScriptsDir}" "${nCores}" "${WindNinja_gdalDir}" "${WindNinja_gdalBuildDir}" "${WindNinja_gdalCPPFLAGS}" "${WindNinja_gdalLDFLAGS}" "${WindNinja_gdalConfigure}" "${WindNinja_gdal_shouldMakeClean}" "${WindNinja_gdalTarFormat}" "${WindNinja_gdalTarDir}" "${WindNinja_gdalTarDirName}"
	success=$? # result of last action, 0 if good, 1 if failed
fi




########## now need to compile each of the required applications/scripts ############

### now attempt to build Farsite
if [ $success == 0 ]; then
	echo "" # add extra line
	if [ ! -f "${farsiteSrcDir}/TestFARSITE" ]; then
		echo "entering farsite directory ${farsiteSrcDir}"
		cd $farsiteSrcDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running cd command !!!"
			success=1
		else
			echo "building Farsite"
			make -j$nCores
			success=$?
			if [ $success != 0 ]; then
				echo "!!! error running make command !!!"
				success=1
			fi
		fi
	else
		echo "Farsite executable already exists so skipping Farsite build process"
	fi
fi

### now attempt to build WindNinja
if [ $success == 0 ]; then
	echo "" # add extra line
	if [ ! -f "${windNinjaBuildDir}/src/cli/WindNinja_cli" ]; then
		echo "entering WindNinja Build Dir ${windNinjaBuildDir}"
		cd $windNinjaBuildDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running cd command !!!"
			success=1
		else
			echo "building WindNinja"
			cmake .. -DGDAL_CONFIG=$WindNinja_gdalBuildDir/bin/gdal-config -DGDAL_INCLUDE_DIR=$WindNinja_gdalBuildDir/include -DGDAL_LIBRARY=$WindNinja_gdalBuildDir/lib/libgdal.so
			success=$?
			if [ $success != 0 ]; then
				echo " !!! error running cmake command !!!"
				success=1
			else
				make -j4
				success=$?
				if [ $success != 0 ]; then
					echo " !!! error running make command !!!"
					success=1
				fi
			fi
		fi
	else
		echo "WindNinja executable already exists so skipping WindNinja build process"
	fi
fi

### now attempt to build the final overall script
if [ $success == 0 ]; then
	echo "" # add extra line
	if [ ! -f "${baseDir}/bin/WRF-WindNinja-FarsiteScript" ]; then
		echo "entering finalBuildDir ${finalBuildDir}"
		cd $finalBuildDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running cd command !!!"
			success=1
		else
			echo "building WRF-WindNinja-FarsiteScript"
			cmake $baseDir
			success=$?
			if [ $success != 0 ]; then
				echo "!!! error running cmake command !!!"
				success=1
			else
				make -j$nCores
				success=$?
				if [ $success != 0 ]; then
					echo "!!! error running make command !!!"
					success=1
				fi
			fi
		fi
	else
		echo "WRF-WindNinja-FarsiteScript executable already exists so skipping WRF-WindNinja-FarsiteScript build process"
	fi
fi


############ build process has completed! ############



echo "" # want a nice clean line
echo "returning to original directory"
cd $includeDir

if [ $success == 1 ]; then
  echo "" # want a fresh clean line
  echo "!!! failed at some point during the build process !!!"
else
  echo "" # want a fresh clean line
  echo "should have finished downloading and building all dependencies up to this point."
  echo "you can check to make sure what library paths were grabbed during the make process for your script by running the command \"ldd ~/script_executable\"."
fi


echo "" # want a fresh clean line
echo "finished buildEverything_Ubuntu.sh script"
echo "" # want a fresh clean line


