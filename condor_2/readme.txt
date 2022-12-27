1: Create a CMSSW_12_4_2 environment 'cmsrel CMSSW_12_4_2'
2: Enter the src directory and setup cmsenv 'cd CMSSW_12_4_2/src; cmsenv'
3: Add Configuration/Generator using 'Git cms-addpkg Configuration/Generator'
4: Clone the git repository in the src folder of CMSSW environment 'git clone https://github.com/Pruthvi-ch/ml_ntuple/tree/master/'
5: Enter the condor submission directory condor_2 'cd ml_ntuple/condor_2'
6: Initiate voms proxy 'voms-proxy-init -voms cms -valid 192:00'
7: Copy the proxy file to condor_2 directory 'cp /tmp/x509up_u135619 ./'
8: Set the proxy location inside createJdlFiles_gen_mono.py
9: Set output directories inside the rungen_nn.sh files for each PU
