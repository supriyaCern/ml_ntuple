Use WriteHits/python/ml_ntuple_cfi.py to generate ml_ntuple.root 
Copy the configuration file form GenConfig to Configuration/Generator/python/
cp ml_ntuple/GenConfig/SingleMuPt100_hgcal_cfi.py Configuration/Generator/python/SingleMuPt100_hgcal_cfi.py

Use following commands to generate, step1.root and step2.root files.

cmsDriver.py SingleMuPt100_hgcal_cfi -s GEN,SIM -n 10000 --conditions auto:phase2_realistic_T21 --beamspot HGCALCloseBy --datatier GEN-SIM --eventcontent FEVTDEBUG --geometry Extended2026D86 --era Phase2C11I13M9 --relval 9000,100 --fileout file:step1.root --customise_commands process.RandomNumberGeneratorService.generator.initialSeed=cms.untracked.uint32($RANDOM) --no_exec --nThreads 4

cmsDriver.py step2 -s DIGI:pdigi_valid,L1TrackTrigger,L1,DIGI2RAW,HLT:@fake2 --conditions auto:phase2_realistic_T21 --datatier GEN-SIM-DIGI-RAW -n 10000 --eventcontent FEVTDEBUGHLT --geometry Extended2026D86 --era Phase2C11I13M9 --no_exec --filein file:step1.root --fileout file:step2.root --nThreads 6

Run the generated python files to create step1.root and step2.root

Use the following command to generate the ml_ntuple.root 

cmsRun ml_ntuple/

# ml_ntuple
