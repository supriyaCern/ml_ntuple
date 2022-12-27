import ROOT as rt
import array as ar
import argparse

parser = argparse.ArgumentParser(description='Process some integers.')
parser.add_argument('--inHE', dest='inHE', type=str,
                    help='infile HE')
parser.add_argument('--inHEB', dest='inHEB', type=str,
                    help='infile HEB')
parser.add_argument('--outHE2', dest='outHE2', type=str,
                    help='outfile HE2')

args = parser.parse_args()

print(args.inHE)
inHE = rt.TFile.Open(args.inHE, 'READ')
inHEB = rt.TFile.Open(args.inHEB, 'READ')
outfile2 = rt.TFile.Open(args.outHE2, 'RECREATE')
#inHE = rt.TFile.Open("/eos/user/p/psuryade/ml_ntuples/08_6/ml_ntuple_HEp_nn.root", "READ")
#inHEB = rt.TFile.Open("/eos/user/p/psuryade/ml_ntuples/08_6/ml_ntuple_HEBp_nn.root", "READ")
#outfile2 = rt.TFile.Open("/eos/user/p/psuryade/ml_ntuples/08_6/ml_ntupleHE2p_nn.root", "RECREATE")

print('open')
nHit = ar.array('i', [0])
X_ = ar.array('f', 10000*[0.0])
Y_ = ar.array('f', 10000*[0.0])
E_ = ar.array('f', 10000*[0.0])
t_ = ar.array('f', 10000*[0.0])
adc_ = ar.array('H', 10000*[0])
thick_ = ar.array('H', 10000*[0])
mode_ = ar.array('H', 10000*[0])
zside_ = ar.array('h', 10000*[0])
intree = []
intreeB = []
outtree2 = []

hADC_200_wi = inHE.Get('Events/hADC_200_wi').Clone()
hE_200 = inHE.Get('Events/hE_200').Clone()
hADC_200_wi.Add(inHEB.Get('Events/hADC_200_wi'))
hE_200.Add(inHEB.Get('Events/hE_200'))

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
    outtree2[-1].Branch("ADC_mode", mode_, "ADC_mode[nHit]/s")
    outtree2[-1].Branch("z_side", zside_, "z_side[nHit]/S")

n = intree[-1].GetEntriesFast()
for j in range(n):
    print(j)
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
            mode_[ii] = intree[i-34].ADC_mode[ii]
            zside_[ii] = intree[i-34].z_side[ii]
        for ii in range(intreeB[i-34].nHit):
            #print(i, nHitsi, ii, intreeB[i-34].nHit)
            X_[nHitsi + ii] = intreeB[i-34].X[ii]
            Y_[nHitsi + ii] = intreeB[i-34].Y[ii]
            E_[nHitsi + ii] = intreeB[i-34].SimHitE[ii]
            t_[nHitsi + ii] = intreeB[i-34].time[ii]
            adc_[nHitsi + ii] = intreeB[i-34].ADC[ii]
            thick_[nHitsi + ii] = intreeB[i-34].Thick[ii]
            mode_[nHitsi + ii] = intreeB[i-34].ADC_mode[ii]
            zside_[nHitsi + ii] = intreeB[i-34].z_side[ii]
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
