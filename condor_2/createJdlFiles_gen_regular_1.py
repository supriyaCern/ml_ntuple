import itertools
import os
import sys
import subprocess
import time

#IMPORT MODULES FROM OTHER DIR

import argparse as arg


parser = arg.ArgumentParser(description='Find Accuracy')
parser.add_argument('-i', '--particle', dest='particle', type=str, default='ele', help="particle")
parser.add_argument('--PU', dest='pu', type=str, default='00', help="PU")
args = parser.parse_args()

if args.particle == 'ele':
    particle = 'Electron'
    pdg_id = '11'
elif args.particle == 'pos':
    particle = 'Positron'
    pdg_id = '-11'
elif args.particle == 'jets':
    particle = 'Jet'
    pdg_id = '5'
else:
    print('Select correct particle from [ele, pos, jets]')
    exit()

if int(args.pu) not in [0, 35, 70, 100, 150, 200]:
    print('Select correct PU from [00, 35, 70, 100, 150, 200]')
    exit()

part = args.particle
PU = args.pu
#pTin = '100'
#eta_in = '2.2'

Geom_1 = [part + PU + '_1']
#D86 = ["Extended2026D83"]


if not os.path.exists(Geom_1[0] + "/log"):
    os.makedirs(Geom_1[0] + "/log")
condorLogDir = "log"
tarFile = Geom_1[0] + "/generator.tar.gz"
if os.path.exists(tarFile):
    os.system("rm %s"%tarFile)
#os.system("tar -zcvf %s ../../Configuration/Generator ../../ml_ntuple --exclude condor"%tarFile)
os.system('cp generator.tar.gz ' + tarFile)
os.system('cp rungen_' + PU + '_1.sh ' + Geom_1[0])
common_command = \
'Universe   = vanilla\n\
should_transfer_files = YES\n\
when_to_transfer_output = ON_EXIT\n\
Transfer_Input_Files = generator.tar.gz, rungen_' + PU + '_1.sh, SingleElectronPt100_hgcal_cfi.py\n\
x509userproxy = /afs/cern.ch/user/p/psuryade/private/validation/CMSSW_12_4_0_pre4/src/ml_ntuple/condor_2/x509up_u135619\n\
use_x509userproxy = true\n\
RequestCpus = 4\n\
+BenchmarkJob = True\n\
#+JobFlavour = "testmatch"\n\
+MaxRuntime = 259200\n\
Output = %s/log_$(cluster)_$(process).stdout\n\
Error  = %s/log_$(cluster)_$(process).stderr\n\
Log    = %s/log_$(cluster)_$(process).condor\n\n'%(condorLogDir, condorLogDir, condorLogDir)

gen_file = \
"import FWCore.ParameterSet.Config as cms\n\
generator = cms.EDFilter('Pythia8PtGun',\n\
                         PGunParameters = cms.PSet(\n\
        MaxPt = cms.double(250),\n\
        MinPt = cms.double(25),\n\
        ParticleID = cms.vint32(22),\n\
        AddAntiParticle = cms.bool(True),\n\
        MaxEta = cms.double(2.8),\n\
        MaxPhi = cms.double(3.14159265359),\n\
        MinEta = cms.double(1.6),\n\
        MinPhi = cms.double(-3.14159265359) ## in radians \n\
        ),\n\
                         Verbosity = cms.untracked.int32(0),\n\
                         psethack = cms.string('single el pt 100'),\n\
                         firstRun = cms.untracked.uint32(1),\n\
                         PythiaParameters = cms.PSet(parameterSets = cms.vstring())\n\
                         )"


#----------------------------------------
#Create jdl files
#----------------------------------------
geom_par = 1
sampleList = eval("Geom_%i"%(geom_par))
genName = 'SingleElectronPt100_hgcal_cfi.py'
genFile = open(Geom_1[0] + '/' + genName, 'w')
genFile.write(gen_file)
jdlName = 'submitJobs_%s.jdl'%(geom_par)
jdlFile = open(Geom_1[0] + '/%s'%jdlName,'w')
jdlFile.write('Executable =  rungen_' + PU + '_1.sh \n')
jdlFile.write(common_command)
jdlFile.write("X=$(step)\n")
for sample in sampleList:
    condorOutDir1="/eos/user/p/psuryade/ml_ntuples"
    condorOutDir="/cms/store/user/psuryade/ml_ntuples"
    os.system("xrdfs root://se01.indiacms.res.in/ mkdir -p %s/%s"%(condorOutDir, sample))
    #condorOutDir="/cms/store/user/idas/SimOut/DeltaPt"
    #os.system("xrdfs root://se01.indiacms.res.in/ mkdir -p %s/%s"%(condorOutDir, sample))
    run_command =  'Arguments  = %s $INT(X) \nQueue 5\n\n' %(sample)
    jdlFile.write(run_command)
    #print "condor_submit jdl/%s"%jdlFile
jdlFile.close() 
os.system('cd ' + Geom_1[0] + ';condor_submit submitJobs_1.jdl')

# subFile = open('tmpSub/condorSubmit.sh','w')
# for year in [2016,2017,2018]:
#     sampleList = eval("samples_%i"%year)
#     jdlName = 'submitJobs_%s.jdl'%(year)
#     jdlFile = open('tmpSub/%s'%jdlName,'w')
#     jdlFile.write('Executable =  rungen.sh \n')
#     jdlFile.write(common_command)
#     condorOutDir1="/eos/user/i/idas/SimOut/DeltaPt"
#     os.system("eos root://eosuser.cern.ch mkdir -p %s/%s"%(condorOutDir1, year))
#     condorOutDir="/cms/store/user/idas/SimOut/DeltaPt"
#     os.system("xrdfs root://se01.indiacms.res.in/ mkdir -p %s/%s"%(condorOutDir, year))
#     jdlFile.write("X=$(step)\n")
    
#     for sample in sampleList:
#         noflines = subprocess.Popen('wc -l ../input/eos/%i/%s_%i.txt | awk \'{print $1}\''%(year,sample,year),shell=True,stdout=subprocess.PIPE).communicate()[0].split('\n')[0]
#         nJob = int(noflines)
#         print "%s %s"%(sample,nJob)
#         if nJob==1:
#             run_command =  'Arguments  = %s %s input/eos/%i/%s_%i.txt 0 base \nQueue 1\n\n' %(year, sample, year, sample, year)
#         else:
#             run_command =  'Arguments  = %s %s input/eos/%i/%s_%i.txt $INT(X) base \nQueue %i\n\n' %(year, sample, year, sample, year, nJob)
#         jdlFile.write(run_command)
# 	#print "condor_submit jdl/%s"%jdlFile
#     subFile.write("condor_submit %s\n"%jdlName)
#     jdlFile.close() 
# subFile.close()
