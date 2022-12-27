for PU in 00 100 200
do
    for pT in 25 50 100 150 200
    do
        for eta in 1.7 1.95 2.2 2.45 2.7
        do
            python createJdlFiles_gen_mono.py -i ele --PU $PU --pt $pT --eta $eta 
            wait(20)
        done
    done
done
