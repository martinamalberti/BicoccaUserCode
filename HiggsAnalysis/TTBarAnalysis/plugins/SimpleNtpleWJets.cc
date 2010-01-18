// -*- C++ -*-
//
// Package:    SimpleNtpleWJets
// Class:      SimpleNtpleWJets
// 
/**\class SimpleNtpleWJets SimpleNtpleWJets.cc Analysis/SimpleNtpleWJets/src/SimpleNtpleWJets.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
 */
//
// Original Author:  Andrea Massironi
//         Created:  Fri Jan  5 17:34:31 CEST 2010
// $Id: SimpleNtpleWJets.cc,v 1.4 2010/01/16 18:40:17 amassiro Exp $
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

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/RefToBase.h"

#include "HiggsAnalysis/TTBarAnalysis/plugins/SimpleNtpleWJets.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"


//--- objects ----
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/BTauReco/interface/JetTag.h"

#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETFwd.h"

#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"

#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"

#include "DataFormats/Common/interface/ValueMap.h"


//---- utilities ----
#include "HiggsAnalysis/TTBarAnalysis/interface/MCDumperWJets.h"


SimpleNtpleWJets::SimpleNtpleWJets(const edm::ParameterSet& iConfig)
{
 edm::Service<TFileService> fs ;
 outTree_  = fs->make <TTree>("SimpleTree","SimpleTree"); 
 
 MCtruthTag_ = iConfig.getParameter<edm::InputTag>("MCtruthTag");
 
 verbosity_ = iConfig.getUntrackedParameter<bool>("verbosity","False");
 eventType_ = iConfig.getUntrackedParameter<int>("eventType",1);
 
 NtupleFactory_ = new NtupleFactory(outTree_);
}


// --------------------------------------------------------------------


SimpleNtpleWJets::~SimpleNtpleWJets()
{
 delete NtupleFactory_;
}


//
// member functions
//

///---- MC ----
void SimpleNtpleWJets::fillMCInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 edm::Handle<reco::GenParticleCollection> genParticles;
 iEvent.getByLabel(MCtruthTag_, genParticles);

 MCDumperWJets mcAnalysis(genParticles, eventType_, verbosity_);
 bool isValid = mcAnalysis.isValid();
  
 if( (eventType_ == 0) && (isValid == true) )
 {  
  NtupleFactory_->Fill4V("mcV",mcAnalysis.mcV()->p4());
  NtupleFactory_->FillFloat("mcV_charge",mcAnalysis.mcV()->charge());
  NtupleFactory_->FillFloat("mcV_pdgId",mcAnalysis.mcV()->pdgId());
     
  NtupleFactory_->Fill4V("mcF1_fromV",mcAnalysis.mcF1_fromV()->p4());
  NtupleFactory_->FillFloat("mcF1_fromV_charge",mcAnalysis.mcF1_fromV()->charge());
  NtupleFactory_->FillFloat("mcF1_fromV_pdgId",mcAnalysis.mcF1_fromV()->pdgId());

  NtupleFactory_->Fill4V("mcF2_fromV",mcAnalysis.mcF2_fromV()->p4());
  NtupleFactory_->FillFloat("mcF2_fromV_charge",mcAnalysis.mcF2_fromV()->charge());
  NtupleFactory_->FillFloat("mcF2_fromV_pdgId",mcAnalysis.mcF2_fromV()->pdgId());
      
 }
}




// ------------ method called to for each event  ------------
void
  SimpleNtpleWJets::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  ///---- fill MCParticle ---- 
 fillMCInfo (iEvent, iSetup);
  
 ///---- save the entry of the tree ----
 NtupleFactory_->FillNtuple();

}

    
// ------------ method called once each job just before starting event loop  ------------
  
  void 
    SimpleNtpleWJets::beginJob(const edm::EventSetup& iSetup)
{

 NtupleFactory_->Add4V("mcV");         
 NtupleFactory_->AddFloat("mcV_charge");    
 NtupleFactory_->AddFloat("mcV_pdgId");    
 
 NtupleFactory_->Add4V("mcF1_fromV");   
 NtupleFactory_->AddFloat("mcF1_fromV_charge");    
 NtupleFactory_->AddFloat("mcF1_fromV_pdgId");  
       
 NtupleFactory_->Add4V("mcF2_fromV");         
 NtupleFactory_->AddFloat("mcF2_fromV_charge");    
 NtupleFactory_->AddFloat("mcF2_fromV_pdgId");  
 
}
   
   
// ------------ method called once each job just after ending the event loop  ------------
     
  void 
    SimpleNtpleWJets::endJob() {
   
   NtupleFactory_->WriteNtuple();

    }

