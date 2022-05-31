#include <iostream>

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"

// essentials !!!
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ServiceRegistry/interface/Service.h" 
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"

class genInfo : public edm::EDAnalyzer
{

   public:
   
      //
      explicit genInfo( const edm::ParameterSet& ) ;
      virtual ~genInfo() {} // no need to delete ROOT stuff
                                   // as it'll be deleted upon closing TFile
      
      virtual void analyze( const edm::Event&, const edm::EventSetup& ) override;
      virtual void beginJob() override;

   private:
   
     TH1F*       h_id ;
     TH1F*       h_px ;
     TH1F*       h_py ;
     
}; 

using namespace edm;
using namespace std;

genInfo::genInfo( const ParameterSet& pset )
  : h_id(0), h_px(0), h_py(0)
{
// actually, pset is NOT in use - we keep it here just for illustratory putposes
}

void genInfo::beginJob()
{
  
  Service<TFileService> fs;
  h_id = fs->make<TH1D>(  "h_id", "", 100,  -30, 30 ) ;
  h_px = fs->make<TH1D>(  "h_px", "", 100, 50., 210. ) ;
  h_py = fs->make<TH1D>(  "h_py", "", 100, 50., 210. ) ;    
    
  return ;
  
}

void genInfo::analyze( const Event& e, const EventSetup& )
{
  int n_vert = 0;
  // here's an example of accessing GenEventInfoProduct
  Handle< GenEventInfoProduct > GenInfoHandle;
  e.getByLabel( "generator", GenInfoHandle );
  double qScale = GenInfoHandle->qScale();
  double pthat = ( GenInfoHandle->hasBinningValues() ? 
                  (GenInfoHandle->binningValues())[0] : 0.0);
  cout << " qScale = " << qScale << " pthat = " << pthat << endl;
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
  Handle< HepMCProduct > EvtHandle ;
  
  // find initial (unsmeared, unfiltered,...) HepMCProduct
  //
  e.getByLabel( "generator", EvtHandle ) ;
  
  const HepMC::GenEvent* Evt = EvtHandle->GetEvent() ;
  
  // this a pointer - and not an array/vector/... 
  // because this example explicitely assumes
  // that there one and only Higgs in the record
  //
  //HepMC::GenVertex* HiggsDecVtx = 0 ;
  
  // find the 1st vertex with outgoing Higgs 
  // and get Higgs decay vertex from there;
  //
  // in principal, one can look for the vertex 
  // with incoming Higgs as well...
  //
  for ( HepMC::GenEvent::vertex_const_iterator
          vit=Evt->vertices_begin(); vit!=Evt->vertices_end(); vit++ )
  {
    n_vert += 1;
      for ( HepMC::GenVertex::particles_out_const_iterator
              pout=(*vit)->particles_out_const_begin();
            pout!=(*vit)->particles_out_const_end(); pout++ )
      {
	cout << "n_vert " << n_vert << " id " << (*pout)->pdg_id();  
      }
  }   
  return ;
   
}

//typedef genInfo genInfo;
DEFINE_FWK_MODULE(genInfo);
