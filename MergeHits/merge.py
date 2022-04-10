import ROOT as rt
import array as ar

inHE = rt.TFile.Open("/eos/user/p/psuryade/ml_ntuples/01_4/ml_ntuple_HE3.root", "READ")
inHEB = rt.TFile.Open("/eos/user/p/psuryade/ml_ntuples/01_4/ml_ntuple_HEB3.root", "READ")
outfile2 = rt.TFile.Open("/eos/user/p/psuryade/ml_ntuples/01_4/ml_ntupleHE2.root", "RECREATE")

print('open')
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

hADC_200_wi = inHE.Get('Events/hADC_200_wi').clone()
hE_200 = inHE.Get('Events/hE_200').clone()
hADC_200_wi.Add(inHEB.Get('Events/hADC_200_wi'))
hE_200.Add(inHE.Get('Events/hE_200'))

for i in range(34, 48):
    intree.append(inHE.Get("Events/layer_" + str(i)))
    intreeB.append(inHEB.Get("Events/layer_" + str(i)))
    outtree2.append(rt.TTree("layer_" + str(i) , "hits in layer" + str(i)))
    outtree2[-1].Branch("nHit", nHit, "nHit/I")
    outtree2[-1].Branch("X", X_, "X[nHit]/F")
    outtree2[-1].Branch("Y", Y_, "Y[nHit]/F")
    outtree2[-1].Branch("SimHitE", E_, "SimHitE[nHit]/F")
    outtree2[-1].Branch("time", t_, "time[nHit]/F")
    outtree2[-1].Branch("ADC", adc_, "ADC[nHit]/s")
    outtree2[-1].Branch("Thick", thick_, "Thick[nHit]/s")

n = intree[-1].GetEntriesFast()
for j in range(n):
    for i in range(34,48):
        intree[i-34].GetEntry(j)
        intreeB[i-34].GetEntry(j)
        nHitsi = intree[i-34].nHit 
        nHit[0] = intree[i-34].nHit + intreeB[i-34].nHit
        for ii in range(nHitsi):
            X_[ii] = intree[i-34].X[ii]
            Y_[ii] = intree[i-34].Y[ii]
            E_[ii] = intree[i-34].SimHitE[ii]
            t_[ii] = intree[i-34].time[ii]
            adc_[ii] = intree[i-34].ADC[ii]
            thick_[ii] = intree[i-34].Thick[ii]
        for ii in range(intreeB[i-34].nHit):
            X_[nHitsi + ii] = intreeB[i-34].X[ii]
            Y_[nHitsi + ii] = intreeB[i-34].Y[ii]
            E_[nHitsi + ii] = intreeB[i-34].SimHitE[ii]
            t_[nHitsi + ii] = intreeB[i-34].time[ii]
            adc_[nHitsi + ii] = intreeB[i-34].ADC[ii]
            thick_[nHitsi + ii] = intreeB[i-34].Thick[ii]
        outtree2[i-34].Fill()
        #print(nHit[0])

tdir = outfile2.mkdir("Events")
#tdir = rt.TDirectory('Events', 'Events')
tdir.cd()
hADC_200_wi.Write()
hE_200.Write()
for i in range(34, 48):
    outtree2[i-34].Write()
#tdir.Write()
