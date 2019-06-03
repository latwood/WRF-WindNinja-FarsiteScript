#!/bin/bash

## Export all environment variables in the qsub command's environment to the
## batch job.
#PBS -V

## Define a job name
#PBS -N overallScriptExample

## Define compute options
#PBS -l nodes=1:dev:ppn=4
#PBS -l mem=3024mb
#PBS -l walltime=00:50:00
#PBS -q batch

## Define path for output & error logs
####PBS -k oe
#PBS -j oe
####PBS -e /fastscratch/latwood/qsubLogs/runComboScriptExample.e
####PBS -o /fastscratch/latwood/qsubLogs/runComboScriptExample.o

## Define path for reporting
#PBS -M pianotocador@gmail.com
#PBS -m abe

# -------- -------- -------- --------
# Actually do something
# -------- -------- -------- --------

binDir="/home/latwood/src/WRF-WindNinja-FarsiteScript/bin"

compilerModuleString="gcc/5.5.0 python/2.7.15/gcc/5.5.0"



############## !!! now start doing stuff with the script !!! ################

echo "entering binDir ${binDir}"
cd $binDir
echo "current directory is "$(pwd)

echo "loading modules ${compilerModuleString}"
module load $compilerModuleString

echo "running WRF-WindNinja-FarsiteScript"
./WRF-WindNinja-FarsiteScript ../examples/cougarCreek-fire/firstRunCougarCreekFire.txt

echo "" # add extra line
echo "--------- end of job  ---------"
echo ""
