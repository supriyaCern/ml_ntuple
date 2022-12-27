#include <iostream>
#include <fstream>

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"

// essentials !!!
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "TH1.h"
#include "TTree.h"


class genInfoAna : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  //
  explicit genInfoAna(const edm::ParameterSet&);
  virtual ~genInfoAna() = default;  // no need to delete ROOT stuff
                                       // as it'll be deleted upon closing TFile

  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void beginJob() override;

private:
  const edm::EDGetTokenT<GenEventInfoProduct> tokenGenEvent_;
  const edm::EDGetTokenT<edm::HepMCProduct> tokenHepMC_;
  
  int n_ = 0;
  int id_[10000] = {0};
  float px_[10000] = {0};
  float py_[10000] = {0};
  float pz_[10000] = {0};
  float E_[10000] = {0};
  float eta_[10000] = {0};
  float phi_[10000] = {0};
  float pT_[10000] = {0};

  //TH1D* fid;
  TTree* genInfo;
  
};

genInfoAna::genInfoAna(const edm::ParameterSet& pset)
    : tokenGenEvent_(consumes<GenEventInfoProduct>(
          edm::InputTag(pset.getUntrackedParameter("moduleLabel", std::string("generator")), ""))),
      tokenHepMC_(consumes<edm::HepMCProduct>(
          edm::InputTag(pset.getUntrackedParameter("moduleLabel", std::string("generatorSmeared")), ""))){
  usesResource(TFileService::kSharedResource);
  // actually, pset is NOT in use - we keep it here just for illustratory putposes
}

void genInfoAna::beginJob() {
  edm::Service<TFileService> fs;
  //fid = fs->make<TH1D>("fid", "id", 100, -30, 30);
  genInfo = fs->make<TTree>("Gen", "Generator Info");
  genInfo->Branch("n_particle", &n_, "n_particle/I");
  genInfo->Branch("pdg_id", &id_, "pdg_id[n_particle]/I");
  genInfo->Branch("px", &px_, "px[n_particle]/F");
  genInfo->Branch("py", &py_, "py[n_particle]/F");
  genInfo->Branch("pz", &pz_, "pz[n_particle]/F");
  genInfo->Branch("E", &E_, "E[n_particle]/F");
  genInfo->Branch("eta", &eta_, "eta[n_particle]/F");
  genInfo->Branch("phi", &phi_, "phi[n_particle]/F");
  genInfo->Branch("pT", &pT_, "pT[n_particle]/F");
  return;
}


void genInfoAna::analyze(const edm::Event& e, const edm::EventSetup&) {
  

  // here's an example of accessing GenEventInfoProduct
  //const edm::Handle<GenEventInfoProduct>& GenInfoHandle = e.getHandle(tokenGenEvent_);

  //double qScale = GenInfoHandle->qScale();
  //double pthat = (GenInfoHandle->hasBinningValues() ? (GenInfoHandle->binningValues())[0] : 0.0);
  //std::cout << " qScale = " << qScale << " pthat = " << pthat << std::endl;
  //
  // this (commented out) code below just exemplifies how to access certain info
  //
  //double evt_weight1 = GenInfoHandle->weights()[0]; // this is "stanrd Py6 evt weight;
  // corresponds to PYINT1/VINT(97)
  //double evt_weight2 = GenInfoHandle->weights()[1]; // in case you run in CSA mode or otherwise
  // use PYEVWT routine, this will be weight
  // as returned by PYEVWT, i.e. PYINT1/VINT(99)
  //std::cout << " evt_weight1 = " << evt_weight1 << std::endl;
  //std::cout << " evt_weight2 = " << evt_weight2 << std::endl;
  //double weight = GenInfoHandle->weight();
  //std::cout << " as returned by the weight() method, integrated event weight = " << weight << std::endl;

  // here's an example of accessing particles in the event record (HepMCProduct)
  //
  
  n_ = 0;
  for (int ii = 0; ii<10000; ii++){
    id_[ii] = 0;
    px_[ii] = 0;
    py_[ii] = 0;
    pz_[ii] = 0;
    E_[ii] = 0;
    eta_[ii] = 0;
    phi_[ii] = 0;
    pT_[ii] = 0;
  }
  
  const edm::Handle<edm::HepMCProduct>& EvtHandle = e.getHandle(tokenHepMC_);

  const HepMC::GenEvent* Evt = EvtHandle->GetEvent();
  
  HepMC::GenEvent::particle_const_iterator part;
  
  for (part = Evt->particles_begin(); part != Evt->particles_end(); ++part) {
    //std::cout << "id " << (*part)->pdg_id() <<std::endl;
    //fid->Fill((*part)->pdg_id());
    if ((*part)->status() == 1 || (*part)->pdg_id() == 5 || (*part)->pdg_id() == -5){
      
      id_[n_] = (*part)->pdg_id();
      px_[n_] = (*part)->momentum().px();
      py_[n_] = (*part)->momentum().py();
      px_[n_] = (*part)->momentum().px();
      pz_[n_] = (*part)->momentum().pz();
      E_[n_] = (*part)->momentum().e();
      eta_[n_] = (*part)->momentum().eta();
      phi_[n_] = (*part)->momentum().phi();
      pT_[n_] = (*part)->momentum().perp();
      n_ += 1;
    }
  }
  genInfo->Fill();
  return;
}

typedef genInfoAna genInfoTest;
DEFINE_FWK_MODULE(genInfoTest);
