// -*- C++ -*-
//
// Package:    NtupleMC
// Class:      NtupleMC
// 
/**\class NtupleMC NtupleMC.cc Analysis/NtupleMC/src/NtupleMC.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
 */
//
// Original Author:  Andrea Massironi
//         Created:  Fri Jan  5 17:34:31 CEST 2010
// $Id: NtupleMC.cc,v 1.7 2010/03/22 11:10:04 amassiro Exp $
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

#include "HiggsAnalysis/TTBarAnalysis/plugins/NtupleMC.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"


//---- utilities ----
#include "HiggsAnalysis/TTBarAnalysis/interface/MCDumperTTBar.h"


NtupleMC::NtupleMC(const edm::ParameterSet& iConfig)
{
 edm::Service<TFileService> fs ;
 outTree_  = fs->make <TTree>("SimpleTree","SimpleTree"); 
 
 MCtruthTag_ = iConfig.getParameter<edm::InputTag>("MCtruthTag");
 genJetTag_ = iConfig.getParameter<edm::InputTag>("genJetTag");

 verbosity_ = iConfig.getUntrackedParameter<bool>("verbosity","False");

 NtupleFactory_ = new NtupleFactory(outTree_);
}


// --------------------------------------------------------------------


NtupleMC::~NtupleMC()
{
 delete NtupleFactory_;
}


//
// member functions
//

///---- GenJet ----
void NtupleMC::fillGenJetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
   //std::cout << "SimpleNtple::fillGenJetInfo" << std::endl;
 
 edm::Handle<edm::View<reco::GenJet> > genJetHandle ;
 iEvent.getByLabel (genJetTag_,genJetHandle);
 for (edm::View<reco::GenJet>::const_iterator genJetIt = genJetHandle->begin (); genJetIt != genJetHandle->end (); ++genJetIt ) 
 { 
  NtupleFactory_->Fill4V("genJets",genJetIt->p4());
 }
}

///---- MC ----
void NtupleMC::fillMCInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
   //std::cout << "SimpleNtple::fillMCInfo" << std::endl;
 
 edm::Handle<reco::GenParticleCollection> genParticles;
 iEvent.getByLabel(MCtruthTag_, genParticles);
 
 MCDumperTTBar mcAnalysis(genParticles, 0 , verbosity_);
 bool isValid = mcAnalysis.isValid();
  
 if( isValid == true )
 {
  NtupleFactory_->Fill4V("mcT1",mcAnalysis.mcT1()->p4());
  NtupleFactory_->FillFloat("mcT1_charge",mcAnalysis.mcT1()->charge());
  
  NtupleFactory_->Fill4V("mcT2",mcAnalysis.mcT2()->p4());
  NtupleFactory_->FillFloat("mcT2_charge",mcAnalysis.mcT2()->charge());
  
  NtupleFactory_->Fill4V("mcB1",mcAnalysis.mcB1()->p4());
  NtupleFactory_->FillFloat("mcB1_charge",mcAnalysis.mcB1()->charge());
  
  NtupleFactory_->Fill4V("mcB2",mcAnalysis.mcB2()->p4());
  NtupleFactory_->FillFloat("mcB2_charge",mcAnalysis.mcB2()->charge());
  
  NtupleFactory_->Fill4V("mcV1",mcAnalysis.mcV1()->p4());
  NtupleFactory_->FillFloat("mcV1_charge",mcAnalysis.mcV1()->charge());
  NtupleFactory_->FillFloat("mcV1_pdgId",mcAnalysis.mcV1()->pdgId());
  
  NtupleFactory_->Fill4V("mcV2",mcAnalysis.mcV2()->p4());
  NtupleFactory_->FillFloat("mcV2_charge",mcAnalysis.mcV2()->charge());
  NtupleFactory_->FillFloat("mcV2_pdgId",mcAnalysis.mcV2()->pdgId());
     
  NtupleFactory_->Fill4V("mcF1_fromV1",mcAnalysis.mcF1_fromV1()->p4());
  NtupleFactory_->FillFloat("mcF1_fromV1_charge",mcAnalysis.mcF1_fromV1()->charge());
  NtupleFactory_->FillFloat("mcF1_fromV1_pdgId",mcAnalysis.mcF1_fromV1()->pdgId());

  NtupleFactory_->Fill4V("mcF2_fromV1",mcAnalysis.mcF2_fromV1()->p4());
  NtupleFactory_->FillFloat("mcF2_fromV1_charge",mcAnalysis.mcF2_fromV1()->charge());
  NtupleFactory_->FillFloat("mcF2_fromV1_pdgId",mcAnalysis.mcF2_fromV1()->pdgId());

  NtupleFactory_->Fill4V("mcF1_fromV2",mcAnalysis.mcF1_fromV2()->p4());
  NtupleFactory_->FillFloat("mcF1_fromV2_charge",mcAnalysis.mcF1_fromV2()->charge());
  NtupleFactory_->FillFloat("mcF1_fromV2_pdgId",mcAnalysis.mcF1_fromV2()->pdgId());

  NtupleFactory_->Fill4V("mcF2_fromV2",mcAnalysis.mcF2_fromV2()->p4());
  NtupleFactory_->FillFloat("mcF2_fromV2_charge",mcAnalysis.mcF2_fromV2()->charge());
  NtupleFactory_->FillFloat("mcF2_fromV2_pdgId",mcAnalysis.mcF2_fromV2()->pdgId());
        
  for (int iPartTau = 0; iPartTau<mcAnalysis.mcFX_fromV1_TauParticles()->size(); iPartTau++ ){
   NtupleFactory_->Fill4V("mcFX_fromV1_TauJet",mcAnalysis.mcFX_fromV1_TauParticles()->at(iPartTau)->p4());
   NtupleFactory_->FillFloat("mcFX_fromV1_TauJet_pfgId",mcAnalysis.mcFX_fromV1_TauParticles()->at(iPartTau)->pdgId());    
  }
  for (int iPartTau = 0; iPartTau<mcAnalysis.mcFX_fromV2_TauParticles()->size(); iPartTau++ ){
   NtupleFactory_->Fill4V("mcFX_fromV2_TauJet",mcAnalysis.mcFX_fromV2_TauParticles()->at(iPartTau)->p4());
   NtupleFactory_->FillFloat("mcFX_fromV2_TauJet_pfgId",mcAnalysis.mcFX_fromV2_TauParticles()->at(iPartTau)->pdgId());    
  }

 }
}




// ------------ method called to for each event  ------------
void
  NtupleMC::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 ///---- fill genJets ---- 
 fillGenJetInfo (iEvent, iSetup);

 ///---- fill MCParticle ---- 
 fillMCInfo (iEvent, iSetup);
  
 ///---- save the entry of the tree ----
 NtupleFactory_->FillNtuple();

}

    
// ------------ method called once each job just before starting event loop  ------------
  
  void 
    NtupleMC::beginJob(const edm::EventSetup& iSetup)
{
  NtupleFactory_->Add4V("genJets");         
  NtupleFactory_->Add4V("mcT1");    
  NtupleFactory_->AddFloat("mcT1_charge");    
  NtupleFactory_->Add4V("mcT2");    
  NtupleFactory_->AddFloat("mcT2_charge");    

  NtupleFactory_->Add4V("mcB1");    
  NtupleFactory_->AddFloat("mcB1_charge");    
  NtupleFactory_->Add4V("mcB2");    
  NtupleFactory_->AddFloat("mcB2_charge");   
       
  NtupleFactory_->Add4V("mcV1");         
  NtupleFactory_->AddFloat("mcV1_charge");    
  NtupleFactory_->AddFloat("mcV1_pdgId");    
 
  NtupleFactory_->Add4V("mcV2");         
  NtupleFactory_->AddFloat("mcV2_charge");    
  NtupleFactory_->AddFloat("mcV2_pdgId");  
  
  NtupleFactory_->Add4V("mcF1_fromV1");   
  NtupleFactory_->AddFloat("mcF1_fromV1_charge");    
  NtupleFactory_->AddFloat("mcF1_fromV1_pdgId");  
       
  NtupleFactory_->Add4V("mcF2_fromV1");         
  NtupleFactory_->AddFloat("mcF2_fromV1_charge");    
  NtupleFactory_->AddFloat("mcF2_fromV1_pdgId");  
 
  NtupleFactory_->Add4V("mcF1_fromV2");         
  NtupleFactory_->AddFloat("mcF1_fromV2_charge");    
  NtupleFactory_->AddFloat("mcF1_fromV2_pdgId");  
 
  NtupleFactory_->Add4V("mcF2_fromV2");         
  NtupleFactory_->AddFloat("mcF2_fromV2_charge");    
  NtupleFactory_->AddFloat("mcF2_fromV2_pdgId");    
  
  NtupleFactory_->Add4V("mcFX_fromV1_TauJet");         
  NtupleFactory_->AddFloat("mcFX_fromV1_TauJet_pfgId");    
  NtupleFactory_->Add4V("mcFX_fromV2_TauJet");         
  NtupleFactory_->AddFloat("mcFX_fromV2_TauJet_pfgId");    
}
   
   
// ------------ method called once each job just after ending the event loop  ------------
     
  void 
    NtupleMC::endJob() 
{
   
 NtupleFactory_->WriteNtuple();

}

