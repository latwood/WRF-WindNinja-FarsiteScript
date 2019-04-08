#!/bin/bash

## Export all environment variables in the qsub command's environment to the
## batch job.
#PBS -V

## Define a job name
#PBS -N compileComboScript

## Define compute options
#PBS -l nodes=1:dev:ppn=4
#PBS -l mem=2048mb
#PBS -l walltime=00:15:00
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



nCores="8"  # this is the attempted number of cores when running any make process (except WindNinja which got mad when I tried using more than 4 cores during one of my builds). Could potentially change this value as a user to maybe get some speedup building gdal and netcdf libraries. Definitely important to somewhat get that annoying insert password trick to actually SOMEWHAT work.



outputDir="/home/latwood/src"
echo "" # add extra line
echo "outputDir = \""$outputDir"\""


comboScriptLink="https://github.com/latwood/WRF-WindNinja-FarsiteScript.git"

baseDir=$outputDir"/WRF-WindNinja-FarsiteScript"
finalBuildDir=$baseDir"/build"
includeDir=$baseDir"/include"
extraAppsDir=$includeDir"/extraApps"


windNinjaDir=$extraAppsDir"/windninja"
windNinjaLink="https://github.com/firelab/windninja.git"
windNinjaBuildDir=$windNinjaDir"/build_windninja"

windNinjaBranch="3.5"


farsiteDir=$extraAppsDir"/farsite"
farsiteLink="https://github.com/firelab/farsite.git"
farsiteSrcDir=$farsiteDir"/src"


### set boolean to stop process in the middle
success=0


### first create all the required source directories and build directories
if [ $success == 0 ]; then
	echo "" # add extra line
	if [ ! -d "${baseDir}" ]; then
		echo "Entering outputDir \""$outputDir"\""
		cd $outputDir
		echo "running git clone on WRF-WindNinja-FarsiteScript \""$comboScriptLink"\""
		git clone $comboScriptLink
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running git clone !!!"
			success=1
		fi
	else
		echo "WRF-WindNinja-FarsiteScript base dir \""$baseDir"\" already exists. Skipping repo cloning step for WRF-WindNinja-FarsiteScript"
	fi
fi

if [ $success == 0 ]; then
	echo "" # add extra line
	if [ ! -d "${extraAppsDir}" ]; then
		echo "creating extraAppsDir \""$extraAppsDir"\""
		mkdir $extraAppsDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running mkdir !!!"
			success=1
		fi
	else
		echo "extraAppsDir \""$extraAppsDir"\" already exists. Skipping extraAppsDir creation step"
	fi
fi

if [ $success == 0 ]; then
	echo "" # add extra line
	if [ ! -d "${windNinjaDir}" ]; then
		echo "entering \""$extraAppsDir"\""
		cd $extraAppsDir
		echo "running git clone on WindNinja \""$windNinjaLink"\""
		git clone $windNinjaLink
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running git clone !!!"
			success=1
		else
			echo "entering WindNinja directory"
			cd windninja
			echo "changing WindNinja branch to hopefully stable "$windNinjaBranch" branch"
			git checkout $windNinjaBranch
			success=$?
			if [ $success != 0 ]; then
				echo "!!! error running git checkout !!!"
				success=1
			fi
		fi
	else
		echo "windNinjaDir \""$windNinjaDir"\" already exists. Skipping repo cloning step for WindNinja"
	fi
fi

if [ $success == 0 ]; then
	echo "" # add extra line
	if [ ! -d "${farsiteDir}" ]; then
		echo "entering \""$extraAppsDir"\""
		cd $extraAppsDir
		echo "running git clone on Farsite \""$farsiteLink"\""
		git clone $farsiteLink
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running git clone !!!"
			success=1
		fi
	else
		echo "farsiteDir \""$farsiteDir"\" already exists. Skipping repo cloning step for farsite"
	fi
fi

if [ $success == 0 ]; then
	echo "" # add extra line
	if [ ! -d "${windNinjaBuildDir}" ]; then
		echo "creating windNinjaBuildDir \""$windNinjaBuildDir"\""
		mkdir $windNinjaBuildDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running mkdir !!!"
			success=1
		fi
	else
		echo "windNinjaBuildDir \""$windNinjaBuildDir"\" already exists. Skipping windNinjaBuildDir creation step"
	fi
fi

if [ $success == 0 ]; then
        echo "" # add extra line
        if [ ! -d "${finalBuildDir}" ]; then
                echo "creating finalBuildDir \""$finalBuildDir"\""
                mkdir $finalBuildDir
                success=$?
                if [ $success != 0 ]; then
                        echo "!!! error running mkdir !!!"
                        success=1
                fi
        else
                echo "finalBuildDir \""$finalBuildDir"\" already exists. Skipping finalBuildDir creation step"
        fi
fi


### now need to load the necessary modules
module load gcc/5.5.0 


### now attempt to build WindNinja

#if [ $success == 0 ]; then
#	echo "" # add extra line
#	if [ ! -f "${windNinjaBuildDir}/src/cli/WindNinja_cli" ]; then
#		echo "building WindNinja"
#		cd $windNinjaBuildDir
#		sudo ldconfig
#		cmake ..
#		success=$?
#		if [ $success != 0 ]; then
#			echo " !!! error running cmake !!!"
#			success=1
#		else
#			make -j4
#			success=$?
#			if [ $success != 0 ]; then
#				echo " !!! error running make !!!"
#				success=1
#			fi
#		fi
#	else
#		echo "WindNinja executable already exists so skipping WindNinja build process"
#	fi
#fi


### now attempt to build Farsite

if [ $success == 0 ]; then
	echo "" # add extra line
	if [ ! -f "${farsiteSrcDir}/TestFARSITE" ]; then
		echo "building Farsite"
		cd $farsiteSrcDir
		make -j$nCores
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running make !!!"
			success=1
		fi
	else
		echo "Farsite executable already exists so skipping Farsite build process"
	fi
fi


### now need to find and update all paths in the combo script repo to match the current base directory



### now attempt to build the final overall script

if [ $success == 0 ]; then
	echo "" # add extra line
	if [ ! -f "${baseDir}/bin/WRF-WindNinja-FarsiteScript" ]; then
		echo "building WRF-WindNinja-FarsiteScript"
		cd $finalBuildDir
		cmake $baseDir
		success=$?
		if [ $success != 0 ]; then
			echo "!!! error running cmake !!!"
			success=1
		else
			make -j$nCores
			success=$?
			if [ $success != 0 ]; then
				echo "!!! error running make !!!"
				success=1
			fi
		fi
	else
		echo "WRF-WindNinja-FarsiteScript executable already exists so skipping WRF-WindNinja-FarsiteScript build process"
	fi
fi


## now update local git ignore stuff so that example files that are tracked can still be ignored, so it is less easy to accidentally make changes to them




echo "" # add extra line
echo "--------- end of job  ---------"
echo ""

