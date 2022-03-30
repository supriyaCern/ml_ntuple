import ROOT as rt
import array as ar

inHE = rt.TFile.Open("/eos/user/p/psuryade/ml_ntuples/21_3/ml_ntuple_HE3.root", "READ")
outfile2 = rt.TFile.Open("/eos/user/p/psuryade/ml_ntuples/21_3/ml_ntupleHE1.root", "RECREATE")

nHit = ar.array('i', [0])
X_ = ar.array('f', 2000*[0.0])
Y_ = ar.array('f', 2000*[0.0])
E_ = ar.array('f', 2000*[0.0])
t_ = ar.array('f', 2000*[0.0])
adc_ = ar.array('H', 2000*[0])
thick_ = ar.array('H', 2000*[0])
intree = []
intreeB = []
outtree2 = []

for i in range(27, 34):
    intree.append(inHE.Get("Events/layer_" + str(i)))
    outtree2.append(rt.TTree("layer_" + str(i) , "hits in layer" + str(i)))
    outtree2[-1].Branch("nHit", nHit, "nHit/I")
    outtree2[-1].Branch("X", X_, "X[nHit]/F")
    outtree2[-1].Branch("Y", Y_, "Y[nHit]/F")
    outtree2[-1].Branch("SimHitE", E_, "SimHitE[nHit]/F")
    outtree2[-1].Branch("time", t_, "time[nHit]/F")
    outtree2[-1].Branch("ADC", adc_, "ADC[nHit]/s")
    outtree2[-1].Branch("Thick", thick_, "Thick[nHit]/s")

for j in range(1000):
    for i in range(27,34):
        intree[i-27].GetEntry(j)
        nHitsi = intree[i-27].nHit 
        nHit[0] = intree[i-27].nHit
        for ii in range(nHitsi):
            X_[ii] = intree[i-27].X[ii]
            Y_[ii] = intree[i-27].Y[ii]
            E_[ii] = intree[i-27].SimHitE[ii]
            t_[ii] = intree[i-27].time[ii]
            adc_[ii] = intree[i-27].ADC[ii]
            thick_[ii] = intree[i-27].Thick[ii]
        outtree2[i-27].Fill()
        #print(nHit[0])

tdir = outfile2.mkdir("Events")
#tdir = rt.TDirectory('Events', 'Events')
tdir.cd()
for i in range(27, 34):
    outtree2[i-34].Write()
#tdir.Write()
