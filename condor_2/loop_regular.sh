for PU in 00 35 70 100 150 200
do
    python createJdlFiles_gen_regular.py -i muplus --PU $PU 
    sleep 10
done

#The following set is prepared to compensate the limited statistics for higher pileup values
for PU in 70 100 150 200
do
    python createJdlFiles_gen_regular_1.py -i muplus --PU $PU 
    sleep 10
done
