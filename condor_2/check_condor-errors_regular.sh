
signal=muplus

basedir=/afs/cern.ch/work/i/idas/CMSSW/CMSSW_12_4_2/src/ml_ntuple/condor_2
for PU in 00 35 70 100 150 200
do
    echo ======================= Pileup $PU ================================
    execdir=$basedir/${signal}${PU}/log
    cd $execdir
    source $basedir/list_crash_jobs_full.sh
    cd $basedir
done

for PU in 70 100 150 200
do
    echo ======================= Pileup $PU ================================
    execdir=$basedir/${signal}${PU}_1/log
    cd $execdir
    source $basedir/list_crash_jobs_full.sh
    cd $basedir
done

