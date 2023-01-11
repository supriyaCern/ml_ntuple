
signal=muplus
signame=Mu_Plus

resourcedir=/eos/cms/store/group/dpg_hgcal/comm_hgcal/geomval/ntuples
outdir=$resourcedir/merged

for PU in 00 35
do
    printf -v zpadPU "%03d" $PU
    indir=$resourcedir/${signal}${PU}
    outfile=$outdir/Raw_Hits_Regular_${signame}_PU_${zpadPU}.root
    unset array
    declare -a array
    nof_file=0
    for ifile in `find $indir -name "ml_ntuple${signal}${PU}_*.root"`
    do
	array[$nof_file]=$ifile
	nof_file=$[$nof_file+1]
    done
    echo ======================= Pileup $PU ================================
    echo Number of files are ${nof_file} inside directory  $indir
    echo files are ${array[@]}
    echo Output file name $outfile
    if [ -f $outfile ] ; then
    	rm $outfile
    fi
    hadd $outfile ${array[@]}
done

for PU in 70 100 150 200
do
    printf -v zpadPU "%03d" $PU
    indir=$resourcedir/${signal}${PU}
    indir1=$resourcedir/${signal}${PU}_1
    outfile=$outdir/Raw_Hits_Regular_${signame}_PU_${zpadPU}.root
    unset array
    declare -a array
    nof_file=0
    for ifile in `find $indir -name "ml_ntuple${signal}${PU}_*.root"`
    do
	array[$nof_file]=$ifile
	nof_file=$[$nof_file+1]
    done
    for ifile in `find $indir1 -name "ml_ntuple${signal}${PU}_*.root"`
    do
	array[$nof_file]=$ifile
	nof_file=$[$nof_file+1]
    done
    echo ======================= Pileup $PU ================================
    echo Number of files are ${nof_file} inside directory  $indir
    echo files are ${array[@]}
    echo Output file name $outfile
    if [ -f $outfile ] ; then
    	rm $outfile
    fi
    hadd $outfile ${array[@]}
done
