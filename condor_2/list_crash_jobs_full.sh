outputfile=/tmp/$USER/fl_crash_et_error.txt
errorfile=/tmp/$USER/crash_et_error.txt
filteredoutputfile=/tmp/$USER/fl_crash_et_error_filtered.txt
resubmitfile=/tmp/$USER/resubmit_sec.txt
resubmitjdl=../resubmit.jdl
if [ -f $resubmitfile ]; then rm $resubmitfile ; fi

echo "This has to run from the log directory"
echo "Curretly we are in $PWD"

nofstderrs=`ls *.stderr | wc -l`
nofstdouts=`ls *.stdout | wc -l`

echo "The number of stderr files : $nofstderrs"
echo "The number of stdout files : $nofstdouts"

grep -i -E 'break|crash|segmentation|error' *.stderr | grep -v -E "TDecompLU" > $errorfile
grep -i -E 'break|crash|segmentation|error' *.stderr | grep -v -E "TDecompLU" | cut -f 1 -d '.' > $outputfile
noflines=`wc -l $outputfile | awk '{print $1}'`
echo "See the errors in  $errorfile"
echo "The list of files with error reports are saved in $outputfile with nof line $noflines"
if [ $noflines -gt 0 ]; then 
    # Avoid to set CMSSW to run the following macro. The default ROOT of lxplus is recommended due to its support for higher C++ standards compared to the ROOT of CMSSW
    #root -l -b -q ../../FilterOutDuplicates.C+\(\""$outputfile"\"\)
    root -l -b -q ../../FilterOutDuplicates.C+\(\""$outputfile"\"\) | grep "log" > $filteredoutputfile 
    noflines=`wc -l $filteredoutputfile | awk '{print $1}'`
    echo "The list of files after filtering out the duplicates is stored in  $filteredoutputfile with number of jobs $noflines"
#     while read line ; do file=$line.stdout ; arg=`grep "All\ arguements" $file | cut -f 2 -d ':'` ; echo -e "Arguments  =$arg\nQueue 1\n" >> $resubmitfile ; done < $filteredoutputfile
    submitjdl=`ls ../submit*.jdl | head -n 1`
    while read line 
    do 
	index=`echo $line | cut -f 3 -d '_'` 
	arg=`grep -i arguments $submitjdl | awk '{print $3}'` 
	echo -e "Arguments  = $arg $index\nQueue 1\n" >> $resubmitfile 
    done < $filteredoutputfile
    head -n 15 $submitjdl > $resubmitjdl  
    cat $resubmitfile >> $resubmitjdl
    echo Check and submit the jdl file $resubmitjdl for nof jobs `grep -i "Arguments" $resubmitjdl | wc -l`
fi
