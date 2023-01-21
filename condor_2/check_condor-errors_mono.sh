
signal=jets

basedir=/afs/cern.ch/work/i/idas/CMSSW/CMSSW_12_4_2/src/ml_ntuple/condor_2

for PU in 00 100 200
do
    for pT in 25 50 100 150 200
    do
        for eta in 1.7 1.95 2.2 2.45 2.7
        do
	    echo ======================= Pileup $PU pT ${pT} eta ${eta} ================================
	    execdir=$basedir/${signal}_PU_${PU}_pT_${pT}_eta_${eta}/log
	    cd $execdir
	    source $basedir/list_crash_jobs_full.sh
	    cd $basedir
        done
    done
done


