#!/bin/bash

## Export all environment variables in the qsub command's environment to the
## batch job.
#PBS -V

## Define a job name
#PBS -N compileComboScript

## Define compute options
#PBS -l nodes=1:dev:ppn=4
#PBS -l mem=3024mb
#PBS -l walltime=02:00:00
#PBS -q batch

## Define path for output & error logs
####PBS -k oe
#PBS -j oe
####PBS -e /fastscratch/latwood/qsubLogs/tryComboScript.e
####PBS -o /fastscratch/latwood/qsubLogs/tryComboScript.o

## Define path for reporting
#PBS -M pianotocador@gmail.com
#PBS -m abe

# -------- -------- -------- --------
# Actually do something
# -------- -------- -------- --------


##echo "" # add extra line
##echo "--------- environment ---------"
##env | grep PBS

##echo "" # add extra line
##echo "--------- where am i  ---------"
##echo WORKDIR: ${PBS_O_WORKDIR}
##echo HOMEDIR: ${PBS_O_HOME}

##echo "" # add extra line
##echo "--------- what i do   ---------"
##echo Test scheduler via test_script
##echo Running time on host `hostname`
##echo Time is `date`
##echo Directory is `pwd`





######## set required variables for everything being done in the script ########


nCores="8"  # this is the attempted number of cores when running any make process (except WindNinja which got mad when I tried using more than 4 cores during one of my builds). Could potentially change this value as a user to maybe get some speedup building gdal and netcdf libraries. Definitely important to somewhat get that annoying insert password trick to actually SOMEWHAT work.


outputDir="/home/latwood/src"	# this is where this script will place the overall script directory on Aeolus
echo "" # add extra line
echo "outputDir = \"${outputDir}\""
overallScriptOutputDir="/fastscratch/latwood"	# this is where this script will set the output location of the example script runs, by changing the example script input files
echo "" # add extra line
echo "overallScriptOutputDir = \"${overallScriptOutputDir}\""


compilerModuleString="gcc/5.5.0 python/2.7.15/gcc/5.5.0"		# this is a string with spaces containing what will be run with "module load". This used to hold a bunch of netcdf stuff and other packages, but cause of difficulty getting dependencies of dependencies to be happy, this now should just hold probably the compiler module to use for all compiling.




### overall script variables

comboScriptLink="https://github.com/latwood/WRF-WindNinja-FarsiteScript.git"	# this is the link to the overall script Repo

baseDir="${outputDir}/WRF-WindNinja-FarsiteScript"	# this is defining the location of the overall script directory on your system after running "git clone" on the overall script repo
includeDir="${baseDir}/include"						# this is defining the location inside the overall script directory to place the third party libraries, packages, and applications
extraAppsDir="${includeDir}/extraApps"				# this is defining the location inside the overall script directory for placing the third party applications
extraLibsDir="${includeDir}/extraLibs"				# this is defining the location inside the overall script directory for placing the third party libraries and packages
finalBuildDir="${baseDir}/build"						# this is defining the location of the overall script build directory, for when running "make" on the overall script
finalBuildExecutable="${baseDir}/bin/WRF-WindNinja-FarsiteScript"	# this is the expected executable that is built by make on the final build script
utilityScriptDir="${baseDir}/utility_scripts"			# this is the folder holding utility scripts like the text replacer used during the build process
finalScript_shouldMakeClean=0			# set to 1 if you want the unzipped folder deleted before running make again, which means a repeat of the unpacking process. Set this on whichever lib failed to build


### third party lib variables


hdf5Link="https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.10/hdf5-1.10.2/src/hdf5-1.10.2.tar.gz"
hdf5TarFormat="-xzf"
hdf5TarDir="${extraLibsDir}/hdf5-1.10.2.tar.gz"
hdf5TarDirName="${extraLibsDir}/hdf5-1.10.2"
hdf5Dir="${extraLibsDir}/hdf5-1.10.2"
hdf5BuildDir="${hdf5Dir}/build_hdf5-1.10.2"

hdf5CPPFLAGS=""
hdf5LDFLAGS=""
hdf5Configure="./configure --prefix=${hdf5BuildDir} --enable-hl"
hdf5_shouldMakeClean=0	# set to 1 if you want the unzipped folder deleted before running make again, which means a repeat of the unpacking process. Set this on whichever lib failed to build


netcdf_cLink="https://github.com/Unidata/netcdf-c/archive/v4.6.1.tar.gz"
netcdf_cTarFormat="-xzf"
netcdf_cTarDir="${extraLibsDir}/v4.6.1.tar.gz"
netcdf_cTarDirName="${extraLibsDir}/netcdf-c-4.6.1"
netcdf_cDir="${extraLibsDir}/netcdf-c-4.6.1"
netcdf_cBuildDir="${netcdf_cDir}/build_netcdf-c-4.6.1"

netcdf_cCPPFLAGS="-I${hdf5BuildDir}/include"
netcdf_cLDFLAGS="-L${hdf5BuildDir}/lib"
netcdf_cConfigure="./configure --prefix=${netcdf_cBuildDir}"
netcdf_c_shouldMakeClean=0	# set to 1 if you want the unzipped folder deleted before running make again, which means a repeat of the unpacking process. Set this on whichever lib failed to build


popplerLink="https://poppler.freedesktop.org/poppler-0.23.4.tar.xz"
popplerTarFormat="-xvf"
popplerTarDir="${extraLibsDir}/poppler-0.23.4.tar.xz"
popplerTarDirName="${extraLibsDir}/poppler-0.23.4"
popplerDir="${extraLibsDir}/poppler-0.23.4"
popplerBuildDir="${popplerDir}/build_poppler-0.23.4"

popplerCPPFLAGS=""
popplerLDFLAGS=""
popplerConfigure="./configure --prefix=${popplerBuildDir} --enable-xpdf-headers"
poppler_shouldMakeClean=0	# set to 1 if you want the unzipped folder deleted before running make again, which means a repeat of the unpacking process. Set this on whichever lib failed to build


projLink="http://download.osgeo.org/proj/proj-4.8.0.tar.gz"
projTarFormat="xvfz"
projTarDir="${extraLibsDir}/proj-4.8.0.tar.gz"
projTarDirName="${extraLibsDir}/proj-4.8.0"
projDir="${extraLibsDir}/proj-4.8.0"
projBuildDir="${projDir}/build_proj-4.8.0"

projCPPFLAGS=""
projLDFLAGS=""
projConfigure="./configure --prefix=${projBuildDir}"
proj_shouldMakeClean=0	# set to 1 if you want the unzipped folder deleted before running make again, which means a repeat of the unpacking process. Set this on whichever lib failed to build


geosLink="http://download.osgeo.org/geos/geos-3.7.2.tar.bz2"
geosBzip2Format="-dk"
geosBzip2Dir="${extraLibsDir}/geos-3.7.2.tar.bz2"
geosTarFormat="-xvf"
geosTarDir="${extraLibsDir}/geos-3.7.2.tar"
geosTarDirName="${extraLibsDir}/geos-3.7.2"
geosDir="${extraLibsDir}/geos-3.7.2"
geosBuildDir="${geosDir}/build_geos-3.7.2"

geosCPPFLAGS=""
geosLDFLAGS=""
geosConfigure="./configure --prefix=${geosBuildDir}"
geos_shouldMakeClean=0	# set to 1 if you want the unzipped folder deleted before running make again, which means a repeat of the unpacking process. Set this on whichever lib failed to build


gdalLink="http://download.osgeo.org/gdal/2.0.3/gdal-2.0.3.tar.gz"
gdalTarFormat="-xzf"
gdalTarDir="${extraLibsDir}/gdal-2.0.3.tar.gz"
gdalTarDirName="${extraLibsDir}/gdal-2.0.3"
gdalDir="${extraLibsDir}/gdal-2.0.3"
gdalBuildDir="${gdalDir}/build_gdal-2.0.3"

gdalCPPFLAGS=""
gdalLDFLAGS=""
gdalConfigure="./configure --prefix=${gdalBuildDir} --with-netcdf=${netcdf_cBuildDir} --with-hdf5=${hdf5BuildDir} --with-static-proj4=${projBuildDir} --with-poppler=${popplerBuildDir} --with-geos=${geosBuildDir}/bin/geos-config"
gdal_shouldMakeClean=0	# set to 1 if you want the unzipped folder deleted before running make again, which means a repeat of the unpacking process. Set this on whichever lib failed to build

## boost is different from other 3rd party libs cause it doesn't use configure, but it is still similar
bzipLink="-O bzip2-1.0.6.tar.gz https://pilotfiber.dl.sourceforge.net/project/bzip2/bzip2-1.0.6.tar.gz --no-check-certificate"
bzipTarFormat="-xzf"
bzipTarDir="${extraLibsDir}/bzip2-1.0.6.tar.gz"
bzipTarDirName="${extraLibsDir}/bzip2-1.0.6"
bzipDir="${extraLibsDir}/bzip2-1.0.6"
bzipBuildDir="${bzipDir}/build_bzip2-1.0.6"

boostLink="-O boost_1_55_0.tar.gz https://sourceforge.net/projects/boost/files/boost/1.55.0/boost_1_55_0.tar.gz/download"
boostTarFormat="-zxvf"
boostTarDir="${extraLibsDir}/boost_1_55_0.tar.gz"
boostTarDirName="${extraLibsDir}/boost_1_55_0"
boostDir="${extraLibsDir}/boost_1_55_0"
boostBuildDir="${boostDir}/build_boost_1_55_0"
boost_shouldMakeClean=0	# set to 1 if you want the unzipped folder deleted before running make again, which means a repeat of the unpacking process. Set this on whichever lib failed to build
## instead of the regular build process, run bootstrap then b2 with an installation setup



### third party app variables

windNinjaDir="${extraAppsDir}/windninja"				# this is defining the location inside the overall script directory for placing the WindNinja application
windNinjaLink="https://github.com/firelab/windninja.git"	# this is the link to the WindNinja script Repo
windNinjaBuildDir="${windNinjaDir}/build_windninja"			# this is defining the location of the WindNinja build directory, for when running "make" on the WindNinja application

windNinjaBranch="3.5.2"								# this is for defining which version of WindNinja (which branch) to switch to by running "git checkout $branch", so defining which version of WindNinja to compile
windNinjaScriptsDir="${windNinjaDir}/scripts"
windNinjaCmakeCommand="cmake .. -DGDAL_CONFIG=${gdalBuildDir}/bin/gdal-config -DGDAL_INCLUDE_DIR=${gdalBuildDir}/include -DGDAL_LIBRARY=${gdalBuildDir}/lib/libgdal.so -DNETCDF_INCLUDES=${netcdf_cBuildDir}/include -DNETCDF_LIBRARIES=${netcdf_cBuildDir}/lib/libnetcdf.so -DNETCDF_LIBRARIES_C=${netcdf_cBuildDir}/lib/libnetcdf.so -DBOOST_INCLUDEDIR=${boostBuildDir}/include -DNINJAFOAM=OFF -DNINJA_QTGUI=OFF"
windNinja_shouldMakeClean=0			# set to 1 if you want the unzipped folder deleted before running make again, which means a repeat of the unpacking process. Set this on whichever lib failed to build


farsiteDir="${extraAppsDir}/farsite"					# this is defining the location inside the overall script directory for placing the Farsite application
farsiteLink="https://github.com/firelab/farsite.git"	# this is the link to the Farsite script Repo
farsiteSrcDir="${farsiteDir}/src"						# this is the place where "make" needs run on Farsite to compile it
farsiteBuildExecutable="${farsiteSrcDir}/TestFARSITE"	# this is the executable created by running make on farsite
farsite_shouldMakeClean=0			# set to 1 if you want the unzipped folder deleted before running make again, which means a repeat of the unpacking process. Set this on whichever lib failed to build



### replacing paths in files variables

varToReplace="\$scriptRoot"							# this is the predefined placeholder text for the user paths in all the examples and src files, to be replaced with the user path found by this script so compiling can be done by this script in one step (instead of the user changing these paths manually)
###echo "varToReplace is "$varToReplace
###varToReplace="/home/atw09001/src/combinationScript-cmake"	# this is a possible alternative placeholder text, say you want to undo the change of the path back to the original placeholder for pushing changes to the examples


### set boolean to stop process in the middle
success=0




############# define functions used by all the other processes ##############
processMakeCleanRequests()		### this function takes as input each and every shouldMakeClean and the corresponding buildDirs and deletes all buildDirs after the first shouldMakeClean so you get a clean build from the first set to true shouldMakeClean that is found
{
	if [ "$#" != 22 ]; then
		echo "" # want a nice clean line
		echo "!!! Incorrect Number of parameters for processMakeCleanRequests() !!!"
		echo "need 22 parameters: \"finalScript_shouldMakeClean\" \"finalBuildDir\" \"finalBuildExecutable\"  \"windNinja_shouldMakeClean\" \"windNinjaBuildDir\"  \"farsite_shouldMakeClean\" \"farsiteBuildExecutable\"  \"boost_shouldMakeClean\" \"boostDir\" \"bzipDir\"  \"gdal_shouldMakeClean\" \"gdalDir\"  \"geos_shouldMakeClean\" \"geosDir\"  \"proj_shouldMakeClean\" \"projDir\"  \"poppler_shouldMakeClean\" \"popplerDir\"  \"netcdf_c_shouldMakeClean\" \"netcdf_cDir\"  \"hdf5_shouldMakeClean\" \"hdf5Dir\""
		echo "this also expects that the .configure prefix (the build directory) for third party libs is within the folder generated from unzipping"
		echo "" # want a nice clean line
		return 1
	fi

	local finalScript_shouldMakeClean="${1}"
	local finalBuildDir="${2}"
	local finalBuildExecutable="${3}"
	local windNinja_shouldMakeClean="${4}"
	local windNinjaBuildDir="${5}"
	local farsite_shouldMakeClean="${6}"
	local farsiteBuildExecutable="${7}"
	local boost_shouldMakeClean="${8}"
	local boostDir="${9}"
	local bzipDir="${10}"
	local gdal_shouldMakeClean="${11}"
	local gdalDir="${12}"
	local geos_shouldMakeClean="${13}"
	local geosDir="${14}"
	local proj_shouldMakeClean="${15}"
	local projDir="${16}"
	local poppler_shouldMakeClean="${17}"
	local popplerDir="${18}"
	local netcdf_c_shouldMakeClean="${19}"
	local netcdf_cDir="${20}"
	local hdf5_shouldMakeClean="${21}"
	local hdf5Dir="${22}"
	
	echo ""		## want some extra space
	echo "finding earliest shouldMakeClean variable and deleting all later build directories for a clean build from that point on"
	
	### first reset all shouldMakeClean depending on inputs
	if [ $hdf5_shouldMakeClean != 0 ]; then
		netcdf_c_shouldMakeClean=1
		gdal_shouldMakeClean=1
		windNinja_shouldMakeClean=1
		finalScript_shouldMakeClean=1
	fi
	if [ $netcdf_c_shouldMakeClean != 0 ]; then
		gdal_shouldMakeClean=1
		windNinja_shouldMakeClean=1
		finalScript_shouldMakeClean=1
	fi
	if [ $poppler_shouldMakeClean != 0 ]; then
		gdal_shouldMakeClean=1
		windNinja_shouldMakeClean=1
		finalScript_shouldMakeClean=1
	fi
	if [ $proj_shouldMakeClean != 0 ]; then
		gdal_shouldMakeClean=1
		windNinja_shouldMakeClean=1
		finalScript_shouldMakeClean=1
	fi
	if [ $geos_shouldMakeClean != 0 ]; then
		gdal_shouldMakeClean=1
		windNinja_shouldMakeClean=1
		finalScript_shouldMakeClean=1
	fi
	if [ $gdal_shouldMakeClean != 0 ]; then
		windNinja_shouldMakeClean=1
		finalScript_shouldMakeClean=1
	fi
	if [ $boost_shouldMakeClean != 0 ]; then
		windNinja_shouldMakeClean=1
		finalScript_shouldMakeClean=1
	fi
	if [ $farsite_shouldMakeClean != 0 ]; then
		finalScript_shouldMakeClean=1
	fi
	if [ $windNinja_shouldMakeClean != 0 ]; then
		finalScript_shouldMakeClean=1
	fi
	### no need for one for the $finalScript_shouldMakeClean


	## now go through each shouldMakeClean to decide which files and folders should be deleted and delete them
	### first set two arrays, one for the booleans and one for the folder names
	### don't include farsite since it doesn't have a folder to delete, also need to do exceptions for Bzip2 with boost and for finalScriptDir
	local shouldMakeClean_array=( ${finalScript_shouldMakeClean} ${windNinja_shouldMakeClean} ${boost_shouldMakeClean} ${gdal_shouldMakeClean} ${geos_shouldMakeClean} ${proj_shouldMakeClean}  ${poppler_shouldMakeClean} ${netcdf_c_shouldMakeClean} ${hdf5_shouldMakeClean} )
	local folderPaths_array=( ${finalBuildDir} ${windNinjaBuildDir} ${boostDir} ${gdalDir} ${geosDir} ${projDir} ${popplerDir} ${netcdf_cDir} ${hdf5Dir} )

	for idx in $(seq 0 $((${#shouldMakeClean_array[@]} - 1)))
	do
		###echo "folderPaths_array[${idx}] = ${folderPaths_array[idx]}"
		if [ ${shouldMakeClean_array[idx]} != 0 ]; then
			if [ -d "${folderPaths_array[idx]}" ]; then
				echo "deleting buildDir \"${folderPaths_array[idx]}\""
				rm -rf ${folderPaths_array[idx]}
				success=$?
				if [ $success != 0 ]; then
					echo "!!! error running rm -rf command !!!"
					return 1
				fi
			fi
		fi
	done

	## now do the remaining manual deletions
	if [ $finalScript_shouldMakeClean != 0 ]; then
		if [ -f "${finalBuildExecutable}" ]; then
			echo "deleting finalBuildExecutable \"${finalBuildExecutable}\""
			rm ${finalBuildExecutable}
			if [ $success != 0 ]; then
				echo "!!! error running rm command !!!"
				return 1
			fi
		fi
	fi
	
	if [ $farsite_shouldMakeClean != 0 ]; then
		if [ -f "${farsiteBuildExecutable}" ]; then
			echo "deleting farsiteBuildExecutable \"${farsiteBuildExecutable}\""
			rm ${farsiteBuildExecutable}
			if [ $success != 0 ]; then
				echo "!!! error running rm command !!!"
				return 1
			fi
		fi
	fi
	
	if [ $boost_shouldMakeClean != 0 ]; then
		if [ -d "${bzipDir}" ]; then
			echo "deleting bzipDir \"${bzipDir}\""
			rm -rf ${bzipDir}
			success=$?
			if [ $success != 0 ]; then
				echo "!!! error running rm -rf command !!!"
				return 1
			fi
		fi
	fi

	
	### if it gets to this point, it worked nicely
	return 0

}

downloadAndUnpackLib_tar()		### this function downloads and unzips a given 3rd party lib, then if the name is not what is wanted, renames the unpacked the directory to what is wanted
{
	if [ "$#" != 7 ]; then
		echo "" # want a nice clean line
		echo "!!! Incorrect Number of parameters for downloadAndUnpackLib_tar() !!!"
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
	echo "checking for ${libName}"

	if [ ! -f "${libTarDir}" ]; then
		echo "entering ${extraLibsDir} directory"
		cd $extraLibsDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running cd command !!!"
			return 1
		fi
		echo "downloading ${libTarDir} from ${libLink}"
		wget ${libLink}
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running wget command !!!"
			return 1
		fi
	else
		echo "${libTarDir} already exists so skipping tar download"
	fi

	if [ ! -d "${libDir}" ]; then
		echo "entering ${extraLibsDir} directory"
		cd $extraLibsDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running cd command !!!"
			return 1
		fi
		echo "unpacking ${libTarDir}"
		tar $tarFormat $libTarDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running tar command with tarFormat \"${tarFormat}\" !!!"
			return 1
		fi
		if [ "${libTarDirName}" != "${libDir}" ]; then
			echo "renaming tar directory ${libTarDirName} to ${libDir}"
			mv $libTarDirName $libDir
			success=$?
			if [ $success != 0 ]; then
				echo "!!! error running mv command !!!"
				return 1
			fi
		fi
	else
		echo "${libDir} already exists so skipping tar unpacking"
	fi

	echo "finished setup for ${libName}"
	echo "" # add a nice clean line

	return 0;

}


downloadAndUnpackLib_Bzip2()		### this function downloads and unzips a given 3rd party lib, then if the name is not what is wanted, renames the unpacked the directory to what is wanted
{
	if [ "$#" != 9 ]; then
		echo "" # want a nice clean line
		echo "!!! Incorrect Number of parameters for downloadAndUnpackLib_Bzip2() !!!"
		echo "need 9 parameters: \"extraLibsDir\" \"libLink\" \"Bzip2Format\" \"Bzip2Dir\" \"tarFormat\" \"libTarDir\" \"libTarDirName\" \"libDir\" \"libBuildDir\""
		echo "" # want a nice clean line
		return 1
	fi

	local extraLibsDir="${1}"
	local libLink="${2}"
	local Bzip2Format="${3}"
	local Bzip2Dir="${4}"
	local tarFormat="${5}"
	local libTarDir="${6}"
	local libTarDirName="${7}"
	local libDir="${8}"
	local libBuildDir="${9}"
	
	
	### make some intermediate variables that used to be passed in, but technically come from these input variables
	###currentDir=$(pwd)
	###currentDirName=$(echo "${currentDir}" | sed 's/.*\///')		# find name of current directory (removes path so grabs last folder name in path)
	###currentDirParent=$(dirname $currentDir)		# find parent directory (pulls off last folder name from path)
	###currentDirParentName=$(echo "${currentDirParent}" | sed 's/.*\///')		# find name of parent directory (removes path so grabs last folder name in path)
	local libName=$(echo "${libDir}" | sed 's/.*\///')	# take the directory off of libDir
	

	echo "" # want a nice clean line
	echo "checking for ${libName}"

	if [ ! -f "${Bzip2Dir}" ]; then
		echo "entering ${extraLibsDir} directory"
		cd $extraLibsDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running cd command !!!"
			return 1
		fi
		echo "downloading ${Bzip2Dir} from ${libLink}"
		wget ${libLink}
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running wget command !!!"
			return 1
		fi
	else
		echo "${Bzip2Dir} already exists so skipping tar download"
	fi


	if [ ! -f "${libTarDir}" ]; then
		echo "entering ${extraLibsDir} directory"
		cd $extraLibsDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running cd command !!!"
			return 1
		fi
		echo "unpacking ${Bzip2Dir}"
		bzip2 $Bzip2Format $Bzip2Dir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running Bzip2 command with Bzip2Format \"${Bzip2Format}\" !!!"
			return 1
		fi
	else
		echo "${libTarDir} already exists so skipping Bzip2 unpacking"
	fi
	

	if [ ! -d "${libDir}" ]; then
		echo "entering ${extraLibsDir} directory"
		cd $extraLibsDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running cd command !!!"
			return 1
		fi
		echo "unpacking ${libTarDir}"
		tar $tarFormat $libTarDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running tar command with tarFormat \"${tarFormat}\" !!!"
			return 1
		fi
		if [ "${libTarDirName}" != "${libDir}" ]; then
			echo "renaming tar directory ${libTarDirName} to ${libDir}"
			mv $libTarDirName $libDir
			success=$?
			if [ $success != 0 ]; then
				echo "!!! error running mv command !!!"
				return 1
			fi
		fi
	else
		echo "${libDir} already exists so skipping tar unpacking"
	fi

	echo "finished setup for ${libName}"
	echo "" # add a nice clean line

	return 0;

}


buildLib()		### this function is to be run on libs after they've been downloaded and unpacked, basically just builds the lib
{
	if [ "$#" != 7 ]; then
		echo "" # want a nice clean line
		echo "!!! Incorrect Number of parameters for buildLib() !!!"
		echo "need 7 parameters: \"extraLibsDir\" \"nCores\" \"libDir\" \"libBuildDir\" \"libCPPFLAGS\" \"libLDFLAGS\" \"libConfigure\""
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


	### make some intermediate variables that used to be passed in, but technically come from these input variables
	###currentDir=$(pwd)
	###currentDirParent=$(dirname $currentDir)		# find parent directory (pulls off last folder name from path)
	###currentDirName=$(echo "${currentDir}" | sed 's/.*\///')		# find name of current directory (removes path so grabs last folder name in path)
	###currentDirParentName=$(echo "${currentDirParent}" | sed 's/.*\///')		# find name of parent directory (removes path so grabs last folder name in path)
	local libName=$(echo "${libDir}" | sed 's/.*\///')	# take the directory off of libDir
	

	echo "" # want a nice clean line
	echo "checking if ${libName} needs to be built"


	if [ ! -d "${libBuildDir}" ]; then
		echo "entering ${libDir} directory"		
		cd $libDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running cd command !!!"
			return 1
		fi
		echo "running configure ${libConfigure}"
		export CPPFLAGS=$libCPPFLAGS
		export LDFLAGS=$libLDFLAGS
		${libConfigure}  # run the passed in configure command
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running configure command !!!"
			return 1
		fi
		echo "building ${libDirName}"
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
		echo "${libBuildDir} already exists so skipping build process"
	fi

	
	echo "finished building ${libName}"
	echo "" # add a nice clean line

	return 0;
}

buildBoost()		### boost is built differently so going to have its own function
{
	if [ "$#" != 6 ]; then
		echo "" # want a nice clean line
		echo "!!! Incorrect Number of parameters for buildLib() !!!"
		echo "need 6 parameters: \"extraLibsDir\" \"nCores\" \"bzipDir\" \"bzipBuildDir\" \"boostDir\" \"boostBuildDir\""
		echo "" # want a nice clean line
		return 1
	fi

	local extraLibsDir="${1}"
	local nCores="${2}"
	local bzipDir="${3}"
	local bzipBuildDir="${4}"
	local boostDir="${5}"
	local boostBuildDir="${6}"


	echo "" # want a nice clean line
	echo "checking if boost needs to be built"


	if [ ! -d "${bzipBuildDir}" ]; then
		echo "entering ${bzipDir} directory"
		cd $bzipDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running cd command !!!"
			return 1
		fi
		echo "running make command"
		make -f Makefile-libbz2_so
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running make command !!!"
			return 1
		fi
		echo "running make install with prefix command"
		make install PREFIX=$bzipBuildDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running make install command !!!"
			return 1
		fi
		echo "copying resulting libbz2.so.* libraries to bzip build dir"
		cp -ar libbz2.so.* $bzipBuildDir/lib
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running cp command !!!"
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
		echo "${bzipBuildDir} already exists so skipping build process"
	fi

	if [ ! -d "${boostBuildDir}" ]; then
		echo "entering ${boostDir} directory"		
		cd $boostDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running cd command !!!"
			return 1
		fi
		echo "running ./bootstrap.sh command"
		./bootstrap.sh
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running ./bootstrap.sh command !!!"
			return 1
		fi
		echo "running ./b2 install --prefix=${boostBuildDir} command"
		./b2 -j$nCores toolset=gcc cxxflags="-std=c++11" install --prefix=$boostBuildDir -sBZIP2_INCLUDE=$bzipBuildDir/include -sBZIP2_LIBPATH=$bzipBuildDir/lib
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running ./b2 install --prefix=${boostBuildDir} command !!!"
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
		echo "${boostBuildDir} already exists so skipping build process"
	fi

	
	echo "finished building boost"
	echo "" # add a nice clean line

	return 0;
}
############# end defining functions to be used by all the other processes ################



############## !!! now start doing stuff with the script !!! ################



######### first create all the required source directories and build directories ##########

### download via "git clone" the overall script directory
if [ $success == 0 ]; then
	echo "" # add extra line
	if [ ! -d "${baseDir}" ]; then
		echo "Entering outputDir \"${outputDir}\""
		cd $outputDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running cd command !!!"
			success=1
		else
			echo "running git clone on WRF-WindNinja-FarsiteScript \"${comboScriptLink}\""
			git clone $comboScriptLink
			success=$?
			if [ $success != 0 ]; then
				echo "!!! error running git clone command !!!"
				success=1
			fi
		fi
	else
		echo "WRF-WindNinja-FarsiteScript base dir \"${baseDir}\" already exists. Skipping repo cloning step for WRF-WindNinja-FarsiteScript"
	fi
fi

##### now process which stuff needs redone so creating source and build directories can go into a clean spot for remakes ######
if [ $success == 0 ]; then
	processMakeCleanRequests "${finalScript_shouldMakeClean}" "${finalBuildDir}" "${finalBuildExecutable}" "${windNinja_shouldMakeClean}" "${windNinjaBuildDir}" "${farsite_shouldMakeClean}" "${farsiteBuildExecutable}" "${boost_shouldMakeClean}" "${boostDir}" "${bzipDir}" "${gdal_shouldMakeClean}" "${gdalDir}" "${geos_shouldMakeClean}" "${geosDir}" "${proj_shouldMakeClean}" "${projDir}" "${poppler_shouldMakeClean}" "${popplerDir}" "${netcdf_c_shouldMakeClean}" "${netcdf_cDir}" "${hdf5_shouldMakeClean}" "${hdf5Dir}"
	success=$? 	# result of last action, 0 if good, 1 if failed
fi

### create the final build directory for later use
if [ $success == 0 ]; then
    echo "" # add extra line
    if [ ! -d "${finalBuildDir}" ]; then
        echo "creating finalBuildDir \"${finalBuildDir}\""
        mkdir $finalBuildDir
        success=$?
        if [ $success != 0 ]; then
            echo "!!! error running mkdir command !!!"
            success=1
        fi
    else
        echo "finalBuildDir \"${finalBuildDir}\" already exists. Skipping finalBuildDir creation step"
    fi
fi

### create the extra applications directory for later use
if [ $success == 0 ]; then
	echo "" # add extra line
	if [ ! -d "${extraAppsDir}" ]; then
		echo "creating extraAppsDir \"${extraAppsDir}\""
		mkdir $extraAppsDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running mkdir command !!!"
			success=1
		fi
	else
		echo "extraAppsDir \"${extraAppsDir}\" already exists. Skipping extraAppsDir creation step"
	fi
fi

### create the extra libs directory for later use
if [ $success == 0 ]; then
	echo "" # add extra line
	if [ ! -d "${extraLibsDir}" ]; then
		echo "creating extraLibsDir \"${extraLibsDir}\""
		mkdir $extraLibsDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running mkdir command !!!"
			success=1
		fi
	else
		echo "extraLibsDir \"${extraLibsDir}\" already exists. Skipping extraLibsDir creation step"
	fi
fi

### download via "git clone" the WindNinja script directory
if [ $success == 0 ]; then
	echo "" # add extra line
	if [ ! -d "${windNinjaDir}" ]; then
		echo "entering \"${extraAppsDir}\""
		cd $extraAppsDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running cd command !!!"
			success=1
		else
			echo "running git clone on WindNinja \"${windNinjaLink}\""
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
					echo "changing WindNinja branch to hopefully stable ${windNinjaBranch} branch"
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
		echo "windNinjaDir \"${windNinjaDir}\" already exists. Skipping repo cloning step for WindNinja"
	fi
fi

### create the WindNinja build directory for later use
if [ $success == 0 ]; then
	echo "" # add extra line
	if [ ! -d "${windNinjaBuildDir}" ]; then
		echo "creating windNinjaBuildDir \"${windNinjaBuildDir}\""
		mkdir $windNinjaBuildDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running mkdir command !!!"
			success=1
		fi
	else
		echo "windNinjaBuildDir \"${windNinjaBuildDir}\" already exists. Skipping windNinjaBuildDir creation step"
	fi
fi

### download via "git clone" the Farsite script directory
if [ $success == 0 ]; then
	echo "" # add extra line
	if [ ! -d "${farsiteDir}" ]; then
		echo "entering \"${extraAppsDir}\""
		cd $extraAppsDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running cd command !!!"
			success=1
		else
			echo "running git clone on Farsite \"${farsiteLink}\""
			git clone $farsiteLink
			success=$?
			if [ $success != 0 ]; then
				echo "!!! error running git clone command !!!"
				success=1
			fi
		fi
	else
		echo "farsiteDir \"${farsiteDir}\" already exists. Skipping repo cloning step for farsite"
	fi
fi


######## download via "wget" and unpack with "tar" each of the third party packages required for the overall script ###########

if [ $success == 0 ]; then
	downloadAndUnpackLib_tar "${extraLibsDir}" "${hdf5Link}" "${hdf5TarFormat}" "${hdf5TarDir}" "${hdf5TarDirName}" "${hdf5Dir}" "${hdf5BuildDir}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	downloadAndUnpackLib_tar "${extraLibsDir}" "${netcdf_cLink}" "${netcdf_cTarFormat}" "${netcdf_cTarDir}" "${netcdf_cTarDirName}" "${netcdf_cDir}" "${netcdf_cBuildDir}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	downloadAndUnpackLib_tar "${extraLibsDir}" "${popplerLink}" "${popplerTarFormat}" "${popplerTarDir}" "${popplerTarDirName}" "${popplerDir}" "${popplerBuildDir}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	downloadAndUnpackLib_tar "${extraLibsDir}" "${projLink}" "${projTarFormat}" "${projTarDir}" "${projTarDirName}" "${projDir}" "${projBuildDir}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	downloadAndUnpackLib_Bzip2 "${extraLibsDir}" "${geosLink}" "${geosBzip2Format}" "${geosBzip2Dir}" "${geosTarFormat}" "${geosTarDir}" "${geosTarDirName}" "${geosDir}" "${geosBuildDir}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	downloadAndUnpackLib_tar "${extraLibsDir}" "${gdalLink}" "${gdalTarFormat}" "${gdalTarDir}" "${gdalTarDirName}" "${gdalDir}" "${gdalBuildDir}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	downloadAndUnpackLib_tar "${extraLibsDir}" "${bzipLink}" "${bzipTarFormat}" "${bzipTarDir}" "${bzipTarDirName}" "${bzipDir}" "${bzipBuildDir}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	downloadAndUnpackLib_tar "${extraLibsDir}" "${boostLink}" "${boostTarFormat}" "${boostTarDir}" "${boostTarDirName}" "${boostDir}" "${boostBuildDir}"
	success=$? # result of last action, 0 if good, 1 if failed
fi



##### It is expected that the examples get copied to a spot with faster read and write access (fastscratch probably), so the $scriptRoot needs updated in the examples separate than other directories, and also first of all other directories
### call the utility script
if [ $success == 0 ]; then
	${utilityScriptDir}/keywordReplacer.sh replaceKeyword "${baseDir}/examples" "${varToReplace}" "${overallScriptOutputDir}"
	success=$?
	if [ $success != 0 ]; then
		echo "!!! could not execute keywordReplacer.sh replaceKeyword function !!!"
		success=1
	fi
fi


############### now need to find and update all paths in the combo script repo to match the current base directory ##############
### call the utility script
if [ $success == 0 ]; then
	aboveBaseDir=$(dirname $baseDir)
	${utilityScriptDir}/keywordReplacer.sh replaceKeyword "${baseDir}/src" "${varToReplace}" "${aboveBaseDir}"
	success=$?
	if [ $success != 0 ]; then
		echo "!!! could not execute keywordReplacer.sh replaceKeyword function !!!"
		success=1
	else
		${utilityScriptDir}/keywordReplacer.sh replaceKeyword "${baseDir}/config" "${varToReplace}" "${aboveBaseDir}"
		success=$?
		if [ $success != 0 ]; then
			echo "!!! could not execute keywordReplacer.sh replaceKeyword function !!!"
			success=1
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
		echo "running \"git update-index --assume-unchanged\" on files changed by replacing \"${varToReplace}\""
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



########### now need to load the necessary modules for compiling (going to manually compile other dependencies from source) ############
if [ $success == 0 ]; then
	echo "" # add extra line
	echo "loading modules required for compiling: ${compilerModuleString}"
	module load $compilerModuleString
	success=$?
	if [ $success != 0 ]; then
		echo "!!! error running module load !!!"
		success=1
	fi
fi


########## now need to compile all the third party libraries and packages for the overall script ###########

if [ $success == 0 ]; then
	buildLib "${extraLibsDir}" "${nCores}" "${hdf5Dir}" "${hdf5BuildDir}" "${hdf5CPPFLAGS}" "${hdf5LDFLAGS}" "${hdf5Configure}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	buildLib "${extraLibsDir}" "${nCores}" "${netcdf_cDir}" "${netcdf_cBuildDir}" "${netcdf_cCPPFLAGS}" "${netcdf_cLDFLAGS}" "${netcdf_cConfigure}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	buildLib "${extraLibsDir}" "${nCores}" "${popplerDir}" "${popplerBuildDir}" "${popplerCPPFLAGS}" "${popplerLDFLAGS}" "${popplerConfigure}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	buildLib "${extraLibsDir}" "${nCores}" "${projDir}" "${projBuildDir}" "${projCPPFLAGS}" "${projLDFLAGS}" "${projConfigure}"
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
	buildLib "${extraLibsDir}" "${nCores}" "${geosDir}" "${geosBuildDir}" "${geosCPPFLAGS}" "${geosLDFLAGS}" "${geosConfigure}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

if [ $success == 0 ]; then
	buildLib "${extraLibsDir}" "${nCores}" "${gdalDir}" "${gdalBuildDir}" "${gdalCPPFLAGS}" "${gdalLDFLAGS}" "${gdalConfigure}"
	success=$? # result of last action, 0 if good, 1 if failed
fi

## boost is built differently
if [ $success == 0 ]; then
	buildBoost "${extraLibsDir}" "${nCores}" "${bzipDir}" "${bzipBuildDir}" "${boostDir}" "${boostBuildDir}"
	success=$? # result of last action, 0 if good, 1 if failed
fi




########## now need to compile each of the required applications/scripts ############

### now attempt to build Farsite
if [ $success == 0 ]; then
	echo "" # add extra line
	if [ ! -f "${farsiteBuildExecutable}" ]; then
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
			${windNinjaCmakeCommand}
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
	if [ ! -f "${finalBuildExecutable}" ]; then
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


echo "" # add extra line
echo "--------- end of job  ---------"
echo ""

