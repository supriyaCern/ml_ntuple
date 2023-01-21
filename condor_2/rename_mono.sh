
signame=Mu_Minus

sourcedir=/eos/cms/store/group/dpg_hgcal/comm_hgcal/geomval/ntuples/merged/Mono/${signame}
for PU in 00 100 200
do
    for pT in 25 50 100 150 200
    do
        for eta in 1.7 1.95 2.2 2.45 2.7
        do
	    printf -v zpadPU "%03d" $PU
	    printf -v zpadpT "%03d" $pT
	    pre_eta=`echo $eta | cut -d '.' -f 1`
	    #post_eta=`echo $eta | cut -d '.' -f 2`
	    post_eta1=`echo $eta | cut -d '.' -f 2`	    
	    if [ ${#post_eta1} -eq 1 ] ; then
		printf -v post_eta "%1d0" $post_eta1
	    else
		printf -v post_eta "%2d" $post_eta1
	    fi
	    #infile=$sourcedir/Raw_Hits_Mono_${signame}_PU_${PU}_Pt_${pT}_Eta_${pre_eta}pt${post_eta}.root
	    #outfile=$sourcedir/Raw_Hits_Mono_${signame}_PU_${zpadPU}_Pt_${zpadpT}_Eta_${pre_eta}pt${post_eta}.root
	    infile=$sourcedir/Raw_Hits_Mono_${signame}_PU_${zpadPU}_Pt_${zpadpT}_Eta_${pre_eta}pt${post_eta1}.root
	    outfile=$sourcedir/Raw_Hits_Mono_${signame}_PU_${zpadPU}_Pt_${zpadpT}_Eta_${pre_eta}pt${post_eta}.root
	    # infile=Raw_Hits_Mono_${signame}_PU_${PU}_Pt_${pT}_Eta_${pre_eta}pt${post_eta}.root
	    # outfile=Raw_Hits_Mono_${signame}_PU_${zpadPU}_Pt_${zpadpT}_Eta_${pre_eta}pt${post_eta}.root
	    #echo infile == $infile "||" outfile == $outfile
	    mv $infile $outfile
        done
    done
done
