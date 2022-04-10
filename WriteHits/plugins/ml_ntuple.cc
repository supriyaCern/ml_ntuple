// -*- C++ -*-
//
// Package:    Demo/CellHitSum
// Class:      CellHitSum
//
/**\class CellHitSum TrackAnalyzer.cc Track/TrackAnalyzer/plugins/TrackAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Indranil Das
//         Created:  Wed, 25 Aug 2021 06:18:11 GMT
//
//


// system include files
#include <memory>
#include <vector>
#include <fstream>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/CaloHit/interface/PCaloHit.h"

#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESTransientHandle.h"

#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/HGCalGeometry/interface/HGCalGeometry.h"
#include "DataFormats/ForwardDetId/interface/ForwardSubdetector.h"
#include "DataFormats/ForwardDetId/interface/HGCalDetId.h"
#include "DataFormats/ForwardDetId/interface/HGCScintillatorDetId.h"
#include "DataFormats/ForwardDetId/interface/HGCSiliconDetId.h"

#include "CoralBase/Exception.h"

#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/ESProducer.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"

#include "DataFormats/ForwardDetId/interface/HGCScintillatorDetId.h"
#include "DataFormats/ForwardDetId/interface/HGCSiliconDetId.h"
#include "DataFormats/ForwardDetId/interface/HGCalTriggerDetId.h"
#include "DataFormats/ForwardDetId/interface/HGCSiliconDetIdToModule.h"
#include "DataFormats/ForwardDetId/interface/HGCSiliconDetIdToROC.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/ForwardDetId/interface/HGCalDetId.h"
#include "DataFormats/HGCDigi/interface/HGCDigiCollections.h"

#include "RecoLocalCalo/HGCalRecAlgos/interface/RecHitTools.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/HGCalCommonData/interface/HGCalDDDConstants.h"

#include "CLHEP/Geometry/Point3D.h"
#include "CLHEP/Geometry/Transform3D.h"
#include "CLHEP/Geometry/Vector3D.h"
#include "CLHEP/Units/GlobalSystemOfUnits.h"
#include "CLHEP/Units/GlobalPhysicalConstants.h"

#include <TH1.h>
#include <TH2.h>
#include <TMath.h>
#include <TGraph.h>
#include <TTree.h>
#include <TBranch.h>
#include <TLorentzVector.h>
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.


using reco::TrackCollection;

class ml_ntuple : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
public:
  //Implemented following Validation/HGCalValidation/plugins/HGCalSimHitValidation.cc
  
  struct hitsinfo {
    hitsinfo() {
      x = y = z = phi = eta = trkpt = trketa = trkphi = 0.0;
      cell = cell2 = sector = sector2 = type = layer = pdg = charge = 0;
      hitid = nhits = 0;
      isMu = false;
    }
    double x, y, z, phi, eta, trkpt, trketa, trkphi;
    int cell, cell2, sector, sector2, type, layer, pdg, charge;
    unsigned int hitid, nhits;
    bool isMu ;
  };
  
  
  explicit ml_ntuple(const edm::ParameterSet&);
  ~ml_ntuple();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
  
  // ----------member data ---------------------------
  //edm::EDGetTokenT<TrackCollection> tracksToken_;  //used to select what tracks to read from configuration file
  edm::EDGetTokenT<edm::SimTrackContainer> tSimTrackContainer; 
  edm::EDGetTokenT<edm::PCaloHitContainer> tSimCaloHitContainer; 
  std::string name;
  edm::ESGetToken<HGCalGeometry, IdealGeometryRecord> geomToken_;
  edm::EDGetToken digiSource_;
  int n_layers;
  int lay_off;
  TTree **out_tree;
  int nHit_[47] = {0};
  float X_[47][20000] = {{0}};
  float Y_[47][20000] = {{0}};
  float E_[47][20000] = {{0}};
  float t_[47][20000] = {{0}};
  uint16_t adc_[47][20000] = {{0}};
  UShort_t thick_[47][20000] = {{0}};
  TH1D *hADC_200_wi;
  TH1D *hE_200;
  //double E_[100][100] = {{0}};
  
  edm::ESGetToken<CaloGeometry, CaloGeometryRecord> caloGeomToken_; 
  hgcal::RecHitTools rhtools_;
  //edm::ConsumesCollector iC;

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  edm::ESGetToken<SetupData, SetupRecord> setupToken_;
#endif

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
ml_ntuple::ml_ntuple(const edm::ParameterSet& iConfig)
  :
  //tracksToken_(consumes<TrackCollection>(iConfig.getUntrackedParameter<edm::InputTag>("simhits"))),
  tSimTrackContainer(consumes<edm::SimTrackContainer>(iConfig.getUntrackedParameter<edm::InputTag>("simtrack"))),
  tSimCaloHitContainer(consumes<edm::PCaloHitContainer>(iConfig.getUntrackedParameter<edm::InputTag>("simhits")))
  //name(iConfig.getParameter<std::string>("Detector")),
  //geomToken_(esConsumes<HGCalGeometry, IdealGeometryRecord>(edm::ESInputTag{"", name}))
{
  //now do what ever initialization is needed
  usesResource("TFileService");
  edm::Service<TFileService> fs;
  name = iConfig.getParameter<std::string>("Detector");
  if(name == "HGCalEESensitive") n_layers = 26;
  else if(name == "HGCalHESiliconSensitive") n_layers = 21;
  else n_layers = 14;

  if(name == "HGCalEESensitive") lay_off = 1;
  else if(name == "HGCalHESiliconSensitive") lay_off = 27;
  else lay_off = 34;
  
  //n_layers = (name == "HGCalEESensitive") ? 26 : 21;
  //lay_off = (name == "HGCalEESensitive") ? 1 : 27;
  out_tree = new TTree*[n_layers];
  hADC_200_wi = fs->make<TH1D>("hADC_200_wi", "ADC_200_wi", 100, 0.0, 200.0);
  hE_200 = fs->make<TH1D>("hE_200", "E_200", 100, 0.0, 200.0);
  for(int kk = 0; kk < n_layers; kk++){
    out_tree[kk] = fs->make<TTree>(Form("layer_%02d",(kk+lay_off)), Form("hits in layer %02d",(kk+1)));
    out_tree[kk]->Branch("nHit", &nHit_[kk], "nHit/I");
    out_tree[kk]->Branch("X", &X_[kk], "X[nHit]/F");
    out_tree[kk]->Branch("Y", &Y_[kk], "Y[nHit]/F");
    out_tree[kk]->Branch("SimHitE", &E_[kk], "SimHitE[nHit]/F");
    out_tree[kk]->Branch("time", &t_[kk], "time[nHit]/F");
    out_tree[kk]->Branch("ADC", &adc_[kk], "ADC[nHit]/s");
    out_tree[kk]->Branch("Thick", &thick_[kk], "Thick[nHit]/s");
    //for(int ii = 0;ii<100;ii++){
      //for(int jj = 0;jj<100;jj++){
        //out_tree->Branch(Form("eta_%02d_phi%02d",ii, jj), &E_[ii][jj]);
      //}
    //}
  }
  
  auto temp = iConfig.getUntrackedParameter<edm::InputTag>("digihits");
  if ((name == "HGCalEESensitive") || (name == "HGCalHESiliconSensitive") ||
      (name == "HGCalHEScintillatorSensitive") || (name == "HGCalHFNoseSensitive")) {
      digiSource_ = consumes<HGCalDigiCollection>(temp);
      //digiSource_=consumes<HGCalDigiCollection>(iconfig.getUntrackedParameter<edm::InputTag>("digihits"));
      //tSimCaloHitContainer=consumes<edm::PCaloHitContainer>(iconfig.getUntrackedParameter<edm::InputTag>("simhits"));
  } 
  //name = iConfig.getUntrackedParameter<string>("Detector", "");
  geomToken_ = esConsumes<HGCalGeometry, IdealGeometryRecord>(edm::ESInputTag{"", name});
  
  
  caloGeomToken_ = esConsumes<CaloGeometry, CaloGeometryRecord>();

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  setupDataToken_ = esConsumes<SetupData, SetupRecord>();
#endif
}


ml_ntuple::~ml_ntuple()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
ml_ntuple::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  

  const CaloGeometry &geomCalo = iSetup.getData(caloGeomToken_);
  rhtools_.setGeometry(geomCalo);
  
  const auto& geomR = iSetup.getData(geomToken_);
  const HGCalGeometry* geom = &geomR;
  
  Handle<PCaloHitContainer> simhit;
  Handle<HGCalDigiCollection> digicollection;
  iEvent.getByToken(tSimCaloHitContainer, simhit);
  iEvent.getByToken(digiSource_, digicollection);
  std::string type;
  
  //for(int ii = 0; ii < 100; ii++){
    //for(int jj = 0; jj < 100; jj++){
      //E_[ii][jj] = 0.0;
    //}
  //}
  for(int ii=0;ii<47;ii++){
    nHit_[ii] = 0;
    for(int jj=0;jj<20000;jj++){
      X_[ii][jj] = 0;
      Y_[ii][jj] = 0;
      E_[ii][jj] = 0;
      t_[ii][jj] = 0;
      adc_[ii][jj] = 0;
      thick_[ii][jj] = 0;
    }
  }

  //double z[47] = {0};
  TLorentzVector p4 = TLorentzVector();
  //double deta = (3.1 - 1.3)/101;
  //double dphi = (2 * 3.14159265359)/101;
  //double eta;
  //double phi;
  //bool new_hit1 = true;
  int kk;
  //std::cout<<eta<<"   "<<phi<<std::endl;
  if(name == "HGCalEESensitive" or name == "HGCalHESiliconSensitive" or name=="HGCalHEScintillatorSensitive"){
    for (const auto& it : *(digicollection.product())){
      DetId id = static_cast<DetId>(it.id());
      GlobalPoint global3 = geom->getPosition(id);
      if (global3.z() > 0){
	const HGCSample& hgcSample = it.sample(2);
	p4.SetXYZT(global3.x(), global3.y(), global3.z(), 0);
	kk = rhtools_.getLayerWithOffset(id) - lay_off;
	//if(nHit_[kk]!=0) nHit_[kk]++;
	//z[kk] = global3.z();
	X_[kk][nHit_[kk]] = global3.x();
	Y_[kk][nHit_[kk]] = global3.y();
	adc_[kk][nHit_[kk]] = hgcSample.data();
	//t_[kk][nHit_[kk]] = time;
	UShort_t thic = 0;
	if(name == "HGCalEESensitive" or name == "HGCalHESiliconSensitive"){
	  HGCSiliconDetId id2(it.id());
	  if (id2.type()==HGCSiliconDetId::HGCalFine) thic = 1;
	  else if(id2.type()==HGCSiliconDetId::HGCalCoarseThin) thic = 2;
	  else thic = 3;
	}
	else thic = 4;
	thick_[kk][nHit_[kk]] = thic;
	nHit_[kk]++;
	for(PCaloHitContainer::const_iterator itHit= simhit->begin(); itHit!= simhit->end(); ++itHit) {
	  //HGCSiliconDetId id1(itHit->id());
	  DetId id1 = static_cast<DetId>(itHit->id());
	  //GlobalPoint global2 = geom->getPosition(id);
	  if(id1 == id){
	    E_[kk][nHit_[kk]-1] += itHit->energy()*1.e6;
	    HepGeom::Point3D<float> gcoord = HepGeom::Point3D<float>(global3.x(), global3.y(), global3.z());
	    double tof = (gcoord.mag() * CLHEP::cm) / CLHEP::c_light;
	    double time = itHit->time() ;
	    time -= tof ;
	    t_[kk][nHit_[kk]-1] = time;
	  }
	}
	if((thic == 2) &&  (E_[kk][nHit_[kk]-1] > 0.01)){
	   hADC_200_wi->Fill(hgcSample.data());
	   hE_200->Fill(E_[kk][nHit_[kk]-1]);
	}
      }
    }
  }
      
      //for(int ii = 0; ii < 100; ii++){
        //for(int jj = 0; jj < 100; jj++){
          //if((eta>((ii*deta)+1.3)) & (eta<(((ii+1)*deta)+1.3))){
            //if((phi>((jj*dphi)-3.14159265359)) & (phi<(((jj+1)*dphi)-3.14159265359))){
              //if(rhtools_.getLayerWithOffset(id) == 1){
                //std::cout<<eta<<"   "<<ii<<"   "<<phi<<"   "<<jj<<std::endl;
                //E_[ii][jj] = E_[ii][jj] + (itHit->energy()*1.e6);
              //}
            //}
          //}
        //}
      //}
  
  
  //DetId::Detector det;
  

  // #ifdef THIS_IS_AN_EVENT_EXAMPLE
  //    Handle<ExampleData> pIn;
  //    iEvent.getByLabel("example",pIn);
  // #endif

  // #ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  //    ESHandle<SetupData> pSetup;
  //    iSetup.get<SetupRecord>().get(pSetup);
  // #endif

  // for (const auto& track : iEvent.get(tracksToken_)) {
  //   // do something with track parameters, e.g, plot the charge.
  //   int charge = track.charge();
  //   histo->Fill( charge );
  //   hPt->Fill(track.pt());
  // }
  
  for(int ii = 0; ii<n_layers; ii++){
    //std::cout<<z[ii]<<std::endl;
    //std::cout<<ii<<"  "<<nHit_[ii]<<std::endl;
    out_tree[ii]->Fill();
  }

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  // if the SetupData is always needed
  auto setup = iSetup.getData(setupToken_);
  // if need the ESHandle to check if the SetupData was there or not
  auto pSetup = iSetup.getHandle(setupToken_);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void
ml_ntuple::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
ml_ntuple::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ml_ntuple::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

  //Specify that only 'tracks' is allowed
  //To use, remove the default given above and uncomment below
  //ParameterSetDescription desc;
  //desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
  //descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ml_ntuple);
