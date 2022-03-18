//g++ ntuple_read.cc `root-config --cflags --libs` -lTMVA -lPhysics -o ntuple_read.out
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>

#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TLegend.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2.h"
#include "TH2D.h"
#include "TString.h"
#include "TRandom3.h"
#include "TLine.h"
#include "TPaveText.h"
#include "THStack.h"
#include "TROOT.h"
#include "TApplication.h"
#include "TStyle.h"
#include "TDirectory.h"
#include "TSystem.h"
#include "TMath.h"
#include "TLorentzVector.h"



int ntuple_read(){
  std::cout<< "start" << std::endl;
  TFile* infile = new TFile("./ml_ntuple2.root", "READ");
  // Z pisition fo all the layers
  double Z_[47] = {322.155,323.149,325.212,326.206,328.269,329.263,331.326,332.32,334.383,335.377,337.44,338.434,340.497,341.491,343.554,344.548,346.611,347.605,349.993,350.987,353.375,354.369,356.757,357.751,360.139,361.133,367.976,374.281,380.586,386.891,393.196,399.501,405.806,412.111,418.416,424.721,431.026,439.251,447.476,455.701,463.926,472.151,480.376,488.601,496.826,505.051,513.276};
  //Scaling of adc or energy
  double scale[] = {1.66, 1, 0.66};
  //image array unable to save as png, issue with menory, will create using python
  float img_[47][1][100][100];
  int n_layers = 47;
  int n = 1000; // number of events to analyze
  int dim1 = 200;
  int dim2 = 720;
  float eta_0(1.3), eta_n(3.1), phi_0(-1*TMath::Pi()), phi_n(TMath::Pi());
  double deta = (eta_n - eta_0)/(dim1);
  double dphi = (phi_n - phi_0)/(dim2);
  double eta;
  double phi;
  TTree **intree = new TTree*[n_layers];
  //Variables for adc, nHit, x, y, simhitE, .etc 
  int nHit_[47] = {0};
  float X_[47][2000] = {{0}};
  float Y_[47][2000] = {{0}};
  float E_[47][2000] = {{0}};
  float t_[47][2000] = {{0}};
  uint16_t adc_[47][2000] = {{0}};
  UShort_t thick_[47][2000] = {{0}};
  
  TH1F* nhit = new TH1F("nhit", "", 50, 0.0, 10.0);
  TH1F* Sim_E_120 = new TH1F("Sim_E_120", "SimHitE_120", 100, 0.0, 200.0);
  TH1F* Sim_E_200 = new TH1F("Sim_E_200", "SimHitE_200", 100, 0.0, 200.0);
  TH1F* Sim_E_300 = new TH1F("Sim_E_300", "SimHitE_300", 100, 0.0, 200.0);

  TH1F* adc_wi_120 = new TH1F("adc_wi_120", "adc_with_SimHit_120", 100, 0.0, 300.0);
  TH1F* adc_wi_200 = new TH1F("adc_wi_200", "adc_with_SimHit_200", 100, 0.0, 500.0);
  TH1F* adc_wi_300 = new TH1F("adc_wi_300", "adc_with_SimHit_300", 100, 0.0, 500.0);

  TH1F* adc_wo_120 = new TH1F("adc_wo_120", "adc_without_SimHit_120", 100, 0.0, 300.0);
  TH1F* adc_wo_200 = new TH1F("adc_wo_200", "adc_without_SimHit_200", 100, 0.0, 500.0);
  TH1F* adc_wo_300 = new TH1F("adc_wo_300", "adc_without_SimHit_300", 100, 0.0, 500.0);

  TH1F* adc_al_120 = new TH1F("adc_al_120", "adc_all_SimHit_120", 100, 0.0, 300.0);
  TH1F* adc_al_200 = new TH1F("adc_al_200", "adc_all_SimHit_200", 100, 0.0, 500.0);
  TH1F* adc_al_300 = new TH1F("adc_al_300", "adc_all_SimHit_300", 100, 0.0, 500.0);
  
  for(int i=0; i<n_layers; i++){
    intree[i] = (TTree*)(infile->Get(Form("Events/layer_%02d", i+1)));
    intree[i]->SetBranchAddress("nHit", &nHit_[i]);
    intree[i]->SetBranchAddress("X", &X_[i]);
    intree[i]->SetBranchAddress("Y", &Y_[i]);
    intree[i]->SetBranchAddress("SimHitE", &E_[i]);
    intree[i]->SetBranchAddress("time", &t_[i]);
    intree[i]->SetBranchAddress("ADC", &adc_[i]);
    intree[i]->SetBranchAddress("Thick", &thick_[i]);
  }
  //int nEvents = intree[i]->GetEntriesFast();
  for(int j=0; j<n; j++){
    for(int i=0; i<n_layers; i++){
      int n_ = 0;
      intree[i]->GetEntry(j);
      for(int k=0; k<nHit_[i]; k++){
	TLorentzVector p4 = TLorentzVector();
	p4.SetXYZT(X_[i][k], Y_[i][k], Z_[i], t_[i][k]);
	double eta = p4.Eta();
	double phi = p4.Phi();
	// This is code for creating image but here cannot save as png
	/*
	std::cout<<i << "   " << j << "   " << k << std::endl;
	for(int ii = 0; ii<dim; ii++){
	  for(int jj=0; jj<dim; jj++){
	    if((eta > eta_0 + ii*deta) && (eta < eta_0 + (ii+1)*deta)){
	      if((phi > phi_0 + jj*dphi) && (phi < phi_0 + (ii+1)*dphi)){
		img_[i][j][ii][jj] = adc_[i][k] * scale[thick_[i][k]-1] ;
	      }
	    }
	  }
	  }*/
	//filling energy of ith layer and kth hit for all events for 300 micron wafer 
	if(thick_[i][k]==3){
	  adc_al_300->Fill(adc_[i][k]);
	  if(E_[i][k]>3){
	    Sim_E_300->Fill(E_[i][k]);
	    adc_wi_300->Fill(adc_[i][k]);
	  } else{
	    adc_wo_300->Fill(adc_[i][k]);
	  }
	}

	if(thick_[i][k]==2){
	  adc_al_200->Fill(adc_[i][k]);
	  if(E_[i][k]>3){
	    Sim_E_200->Fill(E_[i][k]);
	    adc_wi_200->Fill(adc_[i][k]);
	  } else{
	    adc_wo_200->Fill(adc_[i][k]);
	  }
	}

	if(thick_[i][k]==1){
	  adc_al_120->Fill(adc_[i][k]);
	  if(E_[i][k]>3){
	    Sim_E_120->Fill(E_[i][k]);
	    adc_wi_120->Fill(adc_[i][k]);
	  } else{
	    adc_wo_120->Fill(adc_[i][k]);
	  }
	}
	
	if(E_[i][k]>3)
	  n_++;
      }

      nhit->Fill(n_);
      //std::cout<<X_[i][0]<<std::endl;
      
    }
  }

  // Plotting histograms
  TCanvas* c1 = new TCanvas("c1", "", 800, 800);
  c1->cd();
  nhit->GetXaxis()->SetTitle("Number of hits");
  nhit->GetYaxis()->SetTitle("Entries");
  nhit->Draw("HIST");
  c1->Draw();

  TCanvas* c2 = new TCanvas("c2", "", 800, 800);
  c2->cd();
  Sim_E_300->GetXaxis()->SetTitle("SimHit Energy 300 micron");
  Sim_E_300->GetYaxis()->SetTitle("Entries");
  Sim_E_300->Draw("HIST");
  c2->Draw();

  Sim_E_200->GetXaxis()->SetTitle("SimHit Energy 200 micron");
  Sim_E_200->GetYaxis()->SetTitle("Entries");
  Sim_E_120->GetXaxis()->SetTitle("SimHit Energy 120 micron");
  Sim_E_120->GetYaxis()->SetTitle("Entries");

  adc_wi_300->GetXaxis()->SetTitle("ADC 300 micron (with simhit)");
  adc_wi_300->GetYaxis()->SetTitle("Entries");
  adc_wi_200->GetXaxis()->SetTitle("ADC 200 micron (with simhit)");
  adc_wi_200->GetYaxis()->SetTitle("Entries");
  adc_wi_120->GetXaxis()->SetTitle("ADC 120 micron (with simhit)");
  adc_wi_120->GetYaxis()->SetTitle("Entries");

  adc_wo_300->GetXaxis()->SetTitle("ADC 300 micron (without simhit)");
  adc_wo_300->GetYaxis()->SetTitle("Entries");
  adc_wo_200->GetXaxis()->SetTitle("ADC 200 micron (without simhit)");
  adc_wo_200->GetYaxis()->SetTitle("Entries");
  adc_wo_120->GetXaxis()->SetTitle("ADC 120 micron (without simhit)");
  adc_wo_120->GetYaxis()->SetTitle("Entries");

  //infile->Close();
  TFile* outfile = new TFile("./ntuple_hist.root", "RECREATE");
  outfile->cd();
  nhit->Write();
  Sim_E_300->Write();
  adc_al_300->Write();
  adc_wi_300->Write();
  adc_wo_300->Write();
  Sim_E_200->Write();
  adc_al_200->Write();
  adc_wi_200->Write();
  adc_wo_200->Write();
  Sim_E_120->Write();
  adc_al_120->Write();
  adc_wi_120->Write();
  adc_wo_120->Write();
  outfile->Write();
  return 0;
}

int main(){
  ntuple_read();
}
  
