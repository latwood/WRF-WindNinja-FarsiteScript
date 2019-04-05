#!/bin/bash

## Export all environment variables in the qsub command's environment to the
## batch job.
#PBS -V

## Define a job name
#PBS -N tryComboScript

## Define compute options
#PBS -l nodes=1:dev:ppn=1
#PBS -l mem=1024mb
#PBS -l walltime=00:05:00
#PBS -q batch

## Define path for output & error logs
#PBS -k oe
####PBS -j oe
#PBS -e /fastscratch/latwood/qsubLogs/tryComboScript.e
#PBS -o /fastscratch/latwood/qsubLogs/tryComboScript.o

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



nCores="8"  # this is the attempted number of cores when running any make process (except WindNinja which got mad when I tried using more than 4 cores during one of my builds). Could potentially change this value as a user to maybe get some speedup building gdal and netcdf libraries. Definitely important to somewhat get that annoying insert password trick to actually SOMEWHAT work.



outputDir="/fastscratch/latwood/src"
echo "" # add extra line
echo "outputDir = \""$outputDir"\""


comboScriptLink="https://github.com/latwood/WRF-WindNinja-FarsiteScript.git"

baseDir=$outputDir"/WRF-WindNinja-FarsiteScript"
comboScriptBuildDir=$baseDir"/build"
includeDir=$baseDir"/include"
extraAppsDir=$includeDir"/extraApps"


windNinjaDir=$extraAppsDir"/windninja"
windNinjaLink="https://github.com/firelab/windninja.git"
windNinjaBuildDir=$windNinjaDir"/build_windninja"

windNinjaBranch="3.5"


farsiteDir=$extraAppsDir"/farsite"
farsiteLink="https://github.com/firelab/farsite.git"
farsiteSrcDir=$farsiteDir"/src"



### first create all the required source directories and build directories

echo "" # add extra line
if [ ! -d "${baseDir}" ]; then
	echo "Entering outputDir \""$outputDir"\""
	cd $outputDir
	echo "running git clone on WRF-WindNinja-FarsiteScript \""$comboScriptLink"\""
	git clone $comboScriptLink
else
	echo "WRF-WindNinja-FarsiteScript base dir \""$baseDir"\" already exists. Skipping repo cloning step for WRF-WindNinja-FarsiteScript"
fi

echo "" # add extra line
if [ ! -d "${extraAppsDir}" ]; then
	echo "creating extraAppsDir \""$extraAppsDir"\""
	mkdir $extraAppsDir
else
	echo "extraAppsDir \""$extraAppsDir"\" already exists. Skipping extraAppsDir creation step"
fi

echo "" # add extra line
if [ ! -d "${windNinjaDir}" ]; then
	echo "entering \""$extraAppsDir"\""
	cd $extraAppsDir
	echo "running git clone on WindNinja \""$windNinjaLink"\""
	git clone $WindNinjaLink
	echo "entering WindNinja directory"
	cd windninja
	echo "changing WindNinja branch to hopefully stable "$windNinjaBranch" branch"
	git checkout $windNinjaBranch
else
	echo "windNinjaDir \""$windNinjaDir"\" already exists. Skipping repo cloning step for WindNinja"
fi

echo "" # add extra line
if [ ! -d "${farsiteDir}" ]; then
	echo "entering \""$extraAppsDir"\""
	cd $extraAppsDir
	echo "running git clone on Farsite \""$farsiteLink"\""
	git clone $farsiteLink
else
	echo "farsiteDir \""$farsiteDir"\" already exists. Skipping repo cloning step for farsite"
fi

echo "" # add extra line
if [ ! -d "${windNinjaBuildDir}" ]; then
	echo "creating windNinjaBuildDir \""$windNinjaBuildDir"\""
	mkdir $windninjaBuildDir
else
	echo "windNinjaBuildDir \""$windNinjaBuildDir"\" already exists. Skipping windNinjaBuildDir creation step"
fi


### now need to load the necessary modules



### now attempt to build WindNinja

echo "" # add extra line
if [ ! -f "${windNinjaBuildDir}/src/cli/WindNinja_cli" ]; then
	echo "building WindNinja"
	cd $windNinjaBuildDir
	sudo ldconfig
	cmake ..
	make -j4
else
	echo "WindNinja executable already exists so skipping WindNinja build process"
fi


### now attempt to build Farsite

echo "" # add extra line
if [ ! -f "${farsiteSrcDir}/TestFARSITE" ]; then
	echo "building Farsite"
	cd $farsiteSrcDir
	make -j$nCores
else
	echo "Farsite executable already exists so skipping Farsite build process"
fi


### now need to find and update all paths in the combo script repo to match the current base directory



### now attempt to build the final overall script

echo "" # add extra line
if [ ! -f "${finalScriptDir}/bin/WRF-WindNinja-FarsiteScript" ]; then
	echo "building WRF-WindNinja-FarsiteScript"
	cd $finalBuildDir
	cmake $finalScriptDir
	make -j$nCores
else
	echo "WRF-WindNinja-FarsiteScript executable already exists so skipping WRF-WindNinja-FarsiteScript build process"
fi


## now update local git ignore stuff so that example files that are tracked can still be ignored, so it is less easy to accidentally make changes to them




echo "" # add extra line
echo "--------- end of job  ---------"
echo ""

