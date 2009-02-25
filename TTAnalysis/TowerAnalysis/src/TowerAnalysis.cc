// -*- C++ -*-
//
// Package:    TowerAnalysis
// Class:      TowerAnalysis
// 
/**\class TowerAnalysis TowerAnalysis.cc TTAnalysis/TowerAnalysis/src/TowerAnalysis.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Leonardo Sala
//         Created:  Tue Feb 24 11:40:58 CET 2009
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include <map>
#include <string>
#include <vector>

#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

#include "DataFormats/EcalDetId/interface/EcalTrigTowerDetId.h"

#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>

//
// class decleration
//

class TowerAnalysis : public edm::EDAnalyzer {
   public:
      explicit TowerAnalysis(const edm::ParameterSet&);
      ~TowerAnalysis();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;


  edm::InputTag rhEB_;
  edm::InputTag rhEE_;

  edm::Service<TFileService> fs;
  std::map<std::string, TH1F*> histos;
  std::map<std::string, TH2F*> histos2d;


      // ----------member data ---------------------------
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
TowerAnalysis::TowerAnalysis(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
  rhEB_ = iConfig.getParameter<edm::InputTag>("ecalRH_EB");
  rhEE_ = iConfig.getParameter<edm::InputTag>("ecalRH_EE");

}


TowerAnalysis::~TowerAnalysis()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
TowerAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   // get the hit collection from the event:
   edm::Handle<EcalRecHitCollection> rhcHandle;
   iEvent.getByLabel( rhEB_, rhcHandle);
   if (!(rhcHandle.isValid()))
     {
       std::cout << "[Analysis] could not get a handle on the EcalRecHitCollection!" << std::endl;
       return;
     }

   const EcalRecHitCollection* hit_collection = rhcHandle.product();
   for(EcalRecHitCollection::const_iterator it = hit_collection->begin(); it != hit_collection->end(); ++it) {

     EBDetId singleHit = it->detid();

     histos2d["deadTowers_etaPhi"]->Fill(singleHit.iphi(), singleHit.ieta());

     //std::cout << it->energy() << std::endl;
   }

   edm::Handle<EcalRecHitCollection> rhcHandleEE;
   iEvent.getByLabel( rhEE_, rhcHandleEE);
   if (!(rhcHandleEE.isValid()))
     {
       std::cout << "[Analysis] could not get a handle on the EcalRecHitCollection!" << std::endl;
       return;
     }

   const EcalRecHitCollection* hit_collectionEE = rhcHandleEE.product();
   for(EcalRecHitCollection::const_iterator it = hit_collectionEE->begin(); it != hit_collectionEE->end(); ++it) {

     EEDetId singleHit = it->detid();

     if( singleHit.zside() >0)
       histos2d["deadTowersEE+_etaPhi"]->Fill(singleHit.ix(), singleHit.iy());
     else
       histos2d["deadTowersEE-_etaPhi"]->Fill(singleHit.ix(), singleHit.iy());


     //std::cout << it->energy() << std::endl;
   }


}


// ------------ method called once each job just before starting event loop  ------------
void 
TowerAnalysis::beginJob(const edm::EventSetup&)
{
 histos2d["deadTowers_etaPhi"] = fs->make<TH2F>("deadTowers_etaPhi","dead towers (i#phi,i#eta);i#phi;i#eta",365,0,365,200,-100,100);

 histos2d["deadTowersEE+_etaPhi"] = fs->make<TH2F>("deadTowersEE+_etaPhi","dead towers (i#phi,i#eta);i#phi;i#eta",100,0,100,100,0,100);
 histos2d["deadTowersEE-_etaPhi"] = fs->make<TH2F>("deadTowersEE-_etaPhi","dead towers (i#phi,i#eta);i#phi;i#eta",100,0,100,100,0,100);

}

// ------------ method called once each job just after ending the event loop  ------------
void 
TowerAnalysis::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(TowerAnalysis);
