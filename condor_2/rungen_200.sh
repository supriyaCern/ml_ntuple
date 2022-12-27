#!/bin/bash
#To be run on remote machine
#Take input arguments as an array
myArray=( "$@" )
#Array: Size=$#, an element=$1, all element = $@

printf "Start Running Histogramming at ";/bin/date
printf "Worker node hostname ";/bin/hostname
CMSVER=CMSSW_12_4_0_pre4

if [ -z ${_CONDOR_SCRATCH_DIR} ] ; then 
    echo "Running Interactively" ; 
else
    echo "Running In Batch"
    echo ${_CONDOR_SCRATCH_DIR}
    source /cvmfs/cms.cern.ch/cmsset_default.sh
    SCRAM_ARCH=slc7_amd64_gcc10
    scramv1 project CMSSW $CMSVER
    cd $CMSVER/src
    eval `scramv1 runtime -sh`
    #git cms-addpkg Configuration/Generator
    #cd ../..
    cp ../../generator.tar.gz .
fi

pwd
ls -la
tar --strip-components=0 -zxvf generator.tar.gz
cp ../../SingleElectronPt100_hgcal_cfi.py Configuration/Generator/python/SingleElectronPt100_hgcal_cfi.py

if [ -z ${_CONDOR_SCRATCH_DIR} ] ; then 
    echo "Running Interactively" ; 
else
    pwd
    ls -la
    scram b -j 4
fi
#Run for Base, Signal region

echo "All arguements: "$@
echo "Number of arguements: "$#
geom=$1
index=$2


cmsDriver.py SingleElectronPt100_hgcal_cfi -s GEN,SIM -n 240 --conditions auto:phase2_realistic_T21 --beamspot HGCALCloseBy --datatier GEN-SIM --eventcontent FEVTDEBUG --geometry Extended2026D86 --era Phase2C11I13M9 --relval 9000,100 --fileout file:step1_${index}.root  --customise_commands process.RandomNumberGeneratorService.generator.initialSeed="cms.untracked.uint32($RANDOM)" --no_exec  --nThreads 4
grep -n "process.RandomNumberGeneratorService.generator.initialSeed" SingleElectronPt100_hgcal_cfi_GEN_SIM.py
cmsRun SingleElectronPt100_hgcal_cfi_GEN_SIM.py
#cmsRun MinBias_14TeV_pythia8_TuneCUETP8M1_cfi_GEN_SIM.py

cmsDriver.py step2  -s DIGI:pdigi_valid,L1TrackTrigger,L1,DIGI2RAW,HLT:@fake2 --conditions auto:phase2_realistic_T21 --datatier GEN-SIM-DIGI-RAW -n 240 --eventcontent FEVTDEBUGHLT --geometry Extended2026D86 --era Phase2C17I13M9 --pileup AVE_200_BX_25ns --pileup_input root://eosuser.cern.ch//eos/user/p/psuryade/ml_ntuples/PU_14_1/step1_${index}.root --filein  file:step1_${index}.root  --fileout file:step2_${index}.root --nThreads 4



tag=${geom}_$index
#inloc=root://eosuser.cern.ch//eos/user/p/psuryade/ml_ntuples/p_60/
cmsRun ml_ntuple/WriteHits/python/genInfo_cfi.py inputFiles=file:step2_${index}.root outputFile=genInfo${tag}.root 
cmsRun ml_ntuple/WriteHits/python/ml_ntuple_cfi_EE.py inputFiles=file:step2_${index}.root outputFile=ml_ntuple_EE${tag}.root
cmsRun ml_ntuple/WriteHits/python/ml_ntuple_cfi_HE.py inputFiles=file:step2_${index}.root outputFile=ml_ntuple_HE${tag}.root
cmsRun ml_ntuple/WriteHits/python/ml_ntuple_cfi_HEB.py inputFiles=file:step2_${index}.root outputFile=ml_ntuple_HEB${tag}.root
python3 ./ml_ntuple/MergeHits/split.py --inHE ml_ntuple_HE${tag}.root --outHE1 ml_ntuple_HE1${tag}.root
python3 ./ml_ntuple/MergeHits/merge.py --inHE ml_ntuple_HE${tag}.root --inHEB ml_ntuple_HEB${tag}.root --outHE2 ml_ntuple_HE2${tag}.root
#rm ml_ntuplep_nn.root
hadd ml_ntuple${tag}.root genInfo${tag}.root ml_ntuple_EE${tag}.root ml_ntuple_HE1${tag}.root ml_ntuple_HE2${tag}.root


ls -ltr

pwd

printf "Simulation completed at ";/bin/date
#---------------------------------------------
#Copy the ouput root files
#---------------------------------------------
condorOutDir1=/eos/user/p/psuryade/ml_ntuples/${geom}
condorOutDir=/cms/store/user/psuryade/ml_ntuples/$geom
if [ -z ${_CONDOR_SCRATCH_DIR} ] ; then
    echo "Running Interactively" ;
else
    #xrdcp -f ${sample}_tree_*.root root://se01.indiacms.res.in:1094/${condorOutDir}/${year} 
    xrdcp -f ml_ntuple${tag}.root root://se01.indiacms.res.in/${condorOutDir}
    xrdcp -f step1_${index}.root root://se01.indiacms.res.in/${condorOutDir}
    echo "Cleanup"
    cd ../../
    rm -rf $CMSVER
    rm *.root
fi
printf "Done ";/bin/date
