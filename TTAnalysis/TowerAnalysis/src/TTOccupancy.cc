
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// Geometry
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloTopology/interface/EcalEndcapTopology.h"
#include "Geometry/CaloTopology/interface/EcalBarrelTopology.h"
#include "Geometry/CaloTopology/interface/EcalBarrelHardcodedTopology.h"

// Reconstruction Classes
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalDetId/interface/EcalTrigTowerDetId.h"

#include "Geometry/EcalMapping/interface/EcalElectronicsMapping.h"
#include "Geometry/EcalMapping/interface/EcalMappingRcd.h"

//TP
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "CalibCalorimetry/EcalTPGTools/interface/EcalTPGScale.h"

#include "TTAnalysis/EgammaClusterProducers/interface/EcalTTowerKiller.h"



#include <TH1F.h>
#include <fstream>
#include<string>
#include "TFile.h"
#include "TProfile.h"
#include "TProfile2D.h"

using namespace cms;
using namespace std;


class TTOccupancy : public edm::EDAnalyzer {

public:
  explicit TTOccupancy(const edm::ParameterSet&);
  ~TTOccupancy();
  
  
private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;


  TProfile2D* TPOccupancy_prof;
  TProfile2D* eb_TPOccupancy_prof;

  TProfile2D* universe_TPOccupancy_prof;

  TProfile2D* TPOccupancy_Energyprof;
  TProfile2D* universal_TPOccupancy_Energyprof;



  TProfile2D* recHitOccupancy_prof; 
  TProfile2D* eb_recHitOccupancy_prof; 

  TProfile2D* gio_recHitOccupancy_prof; 
  TProfile2D* gio_recHitOccupancy_Energyprof; 

  //with global eta phi coordinates
  TProfile2D* universe_recHitOccupancy_prof; 
  TProfile2D* universal_recHitOccupancy_Energyprof; 




  edm::InputTag recHitCollection;

  std::string OutputFile;
};


//
// constructors and destructor
//
TTOccupancy::TTOccupancy(const edm::ParameterSet& ps)
{
  recHitCollection = ps.getParameter<edm::InputTag>("recHitCollection");
  OutputFile = ps.getParameter<std::string>("OutputFile");

}


TTOccupancy::~TTOccupancy()
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}


// ------------ method called to produce the data  ------------
void TTOccupancy::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  //Trigger Primitives
  ESHandle<CaloGeometry> theGeometry;
  ESHandle<CaloSubdetectorGeometry> theEndcapGeometry_handle, theBarrelGeometry_handle;
  iSetup.get<CaloGeometryRecord>().get( theGeometry );
  iSetup.get<EcalEndcapGeometryRecord>().get("EcalEndcap",theEndcapGeometry_handle);
  iSetup.get<EcalBarrelGeometryRecord>().get("EcalBarrel",theBarrelGeometry_handle);
  const CaloSubdetectorGeometry * theBarrelGeometry_ , *theEndcapGeometry_;
   
  theBarrelGeometry_ = &(*theBarrelGeometry_handle);
  theEndcapGeometry_ = &(*theEndcapGeometry_handle);
   
  std::vector < std::pair< std::pair<int, int>, float > >TP_map_EB, TP_map_EE;
  std::vector < std::pair< std::pair<int, int>, float > >TT_map_EB, TT_map_EE;

  EcalTPGScale ecalScale;
  ecalScale.setEventSetup(iSetup) ;
   
  Handle<EcalTrigPrimDigiCollection> emulDigis;
  iEvent.getByLabel("simEcalTriggerPrimitiveDigis", emulDigis);
  for ( EcalTrigPrimDigiCollection::const_iterator tpdigiItr = emulDigis->begin();tpdigiItr != emulDigis->end(); ++tpdigiItr ) {
    EcalTriggerPrimitiveDigi data = (*tpdigiItr);
    EcalTrigTowerDetId idt = data.id();

    if(idt.subDet() == EcalBarrel){
      EBDetId id( idt.ieta(), idt.iphi() ) ;

      float theta =  theBarrelGeometry_->getGeometry(id)->getPosition().theta();
      float eta = theBarrelGeometry_->getGeometry(id)->getPosition().eta();
      float phi = theBarrelGeometry_->getGeometry(id)->getPosition().phi();


      int iFED = 600 + idt.iDCC();       
      //conversion from FED and iSM
      if(idt.iDCC() >18) iFED -= 9;
      else if( idt.iDCC() >= 1 && idt.iDCC() < 19) iFED += 27;

      int iTT =   idt.iTT();

      TPOccupancy_prof->Fill(idt.iphi(), idt.ieta(), iTT);
     

      //// from EBDetId
      int iFED_eb = 600 + id.ism();  
      //conversion from FED and iSM
      if(id.ism() >18) iFED_eb -= 9;
      else if( id.ism() >= 1 && id.ism() < 19) iFED_eb += 27;

      int iTT_eb =   id.tower().iTT();

      eb_TPOccupancy_prof->Fill(id.iphi(), id.ieta(), iTT_eb);

      universe_TPOccupancy_prof->Fill(phi, eta, iTT_eb);

      float  tpEt = ecalScale.getTPGInGeV(tpdigiItr->compressedEt(), tpdigiItr->id()) ;

      universal_TPOccupancy_Energyprof->Fill(phi, eta, abs(tpEt/sin(theta)));
      TPOccupancy_Energyprof->Fill(idt.iphi(), idt.ieta(), abs(tpEt/sin(theta)));

    }

  }

  // get the hit collection from the event:
  edm::Handle<EcalRecHitCollection> ec;
  iEvent.getByLabel(recHitCollection, ec);
  
  if (!( ec.isValid())) {
    std::cout << "could not get a handle on the EcalRecHitCollection! "<< recHitCollection << std::endl;
    return;
  }
     
  const EcalRecHitCollection* hit_collectionEB = ec.product();
     
  for(EcalRecHitCollection::const_iterator it = hit_collectionEB->begin(); it != hit_collectionEB->end(); ++it) {
    bool isBarrel = false;
    
    ESHandle< EcalElectronicsMapping > ecalmapping;
    iSetup.get< EcalMappingRcd >().get(ecalmapping);
    const EcalElectronicsMapping * theMapping_ = ecalmapping.product();
    
    if( it->detid().subdetId() == 1) isBarrel = true;
    
    EcalElectronicsId elecId = theMapping_->getElectronicsId( it->id() );
    int iFED = 600 + elecId.dccId();
    int iTT =   elecId.towerId();


    if(isBarrel){

    EBDetId cristallo(it->detid());
    float eta = theBarrelGeometry_->getGeometry(cristallo)->getPosition().eta();
    float phi = theBarrelGeometry_->getGeometry(cristallo)->getPosition().phi();

    recHitOccupancy_prof->Fill(cristallo.iphi(), cristallo.ieta(), iTT);

    //// TT from EB
    int iFED_eb = 600 + cristallo.ism();
    //conversion from FED and iSM 
    if(cristallo.ism() >18) iFED_eb -= 9;
    else if( cristallo.ism() >= 1 && cristallo.ism() < 19) iFED_eb += 27;

    int iTT_eb =   cristallo.tower().iTT();

    eb_recHitOccupancy_prof->Fill(cristallo.iphi(), cristallo.ieta(), iTT_eb);

    gio_recHitOccupancy_prof->Fill(cristallo.tower_iphi(), cristallo.tower_ieta(), iTT);
    
    gio_recHitOccupancy_Energyprof->Fill(cristallo.tower_iphi(), cristallo.tower_ieta(), it->energy());

    universal_recHitOccupancy_Energyprof->Fill(phi, eta, it->energy());
    
    universe_recHitOccupancy_prof->Fill(phi, eta, iTT_eb);
    

    }
  }
}





// ------------ method called once each job just before starting event loop  ------------
void TTOccupancy::beginJob(const edm::EventSetup&)
{

  TPOccupancy_prof = new TProfile2D("TPOccupancy_prof", "TPOccupancy_prof", 360, 1, 361, 172, -85, 86);
  eb_TPOccupancy_prof = new TProfile2D("eb_TPOccupancy_prof", "eb_TPOccupancy_prof", 360, 1, 361, 172, -85, 86);

  universe_TPOccupancy_prof = new TProfile2D("universe_TPOccupancy_prof", "universe_TPOccupancy_prof", 360, -3.14159, 3.14159, 172, -1.5, 1.5);

  TPOccupancy_Energyprof = new TProfile2D("TPOccupancy_Energyprof", "TPOccupancy_Energyprof", 360, 1, 361, 172, -85, 86);
  universal_TPOccupancy_Energyprof = new TProfile2D("universal_TPOccupancy_Energyprof","universal_TPOccupancy_Energyprof",360,-3.14159,3.14159,172,-1.5,1.5);



  recHitOccupancy_prof = new TProfile2D("recHitOccupancy_prof", "recHitOccupancy_prof", 360, 1, 361, 172, -85, 86);
  eb_recHitOccupancy_prof = new TProfile2D("eb_recHitOccupancy_prof", "eb_recHitOccupancy_prof", 360, 1, 361, 172, -85, 86);

  gio_recHitOccupancy_prof = new TProfile2D("gio_recHitOccupancy_prof", "gio_recHitOccupancy_prof", 360, 1, 361, 172, -85, 86);

  gio_recHitOccupancy_Energyprof = new TProfile2D("gio_recHitOccupancy_Energyprof","gio_recHitOccupancy_Energyprof",360, 1, 361, 172, -85, 86);

  universe_recHitOccupancy_prof = new TProfile2D("universe_recHitOccupancy_prof","universe_recHitOccupancy_prof",360,-3.14159,3.14159,172,-1.5,1.5);
  universal_recHitOccupancy_Energyprof = new TProfile2D("universal_recHitOccupancy_Energyprof","universal_recHitOccupancy_Energyprof",360,-3.14159,3.14159,172,-1.5,1.5);

}



// ------------ method called once each job just after ending the event loop  ------------
void TTOccupancy::endJob() {

  TFile outputFile(OutputFile.c_str(), "recreate");
  outputFile.cd();


  TPOccupancy_prof->Write();
  eb_TPOccupancy_prof->Write();

  universe_TPOccupancy_prof->Write();

  TPOccupancy_Energyprof->Write();
  universal_TPOccupancy_Energyprof->Write();


  recHitOccupancy_prof->Write();
  eb_recHitOccupancy_prof->Write();

  gio_recHitOccupancy_prof->Write();

  gio_recHitOccupancy_Energyprof->Write();

  universe_recHitOccupancy_prof->Write();
  universal_recHitOccupancy_Energyprof->Write();

  outputFile.Close();
}

////define this as a plug-in
DEFINE_FWK_MODULE(TTOccupancy);
