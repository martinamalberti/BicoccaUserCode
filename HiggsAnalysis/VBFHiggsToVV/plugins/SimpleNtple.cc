// -*- C++ -*-
//
// Package:    SimpleNtple
// Class:      SimpleNtple
// 
/**\class SimpleNtple SimpleNtple.cc Analysis/SimpleNtple/src/SimpleNtple.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
 */
//
// Original Author:  Andrea Massironi
//         Created:  Fri Jan  5 17:34:31 CEST 2010
// $Id: SimpleNtple.cc,v 1.2 2010/01/13 16:23:37 amassiro Exp $
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

#include "HiggsAnalysis/VBFHiggsToVV/plugins/SimpleNtple.h"

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
#include "HiggsAnalysis/VBFHiggsToVV/interface/MCDumperVBF.h"


SimpleNtple::SimpleNtple(const edm::ParameterSet& iConfig)
{
 edm::Service<TFileService> fs ;
 outTree_  = fs->make <TTree>("SimpleTree","SimpleTree"); 
 
 TracksTag_ = iConfig.getParameter<edm::InputTag>("TracksTag");
 EleTag_ = iConfig.getParameter<edm::InputTag>("EleTag");
 MuTag_ = iConfig.getParameter<edm::InputTag>("MuTag");
 MetTag_ = iConfig.getParameter<edm::InputTag>("MetTag");
 JetTag_ = iConfig.getParameter<edm::InputTag>("JetTag");
 flag_JetBTag_ = iConfig.getUntrackedParameter<bool>("flag_JetBTag","False");
 if (flag_JetBTag_) JetBTag_ = iConfig.getUntrackedParameter<edm::InputTag>("JetBTag");
 correctedJetTag_ = iConfig.getParameter<edm::InputTag>("correctedJetTag");
 MCtruthTag_ = iConfig.getParameter<edm::InputTag>("MCtruthTag");
 genJetTag_ = iConfig.getParameter<edm::InputTag>("genJetTag");
 genMetTag_ = iConfig.getParameter<edm::InputTag>("genMetTag");

 eleIDCut_LooseInputTag_ = iConfig.getParameter<edm::InputTag> ("eleIDCut_LooseInputTag");
 eleIDCut_RLooseInputTag_ = iConfig.getParameter<edm::InputTag> ("eleIDCut_RLooseInputTag");
 eleIDCut_TightInputTag_ = iConfig.getParameter<edm::InputTag> ("eleIDCut_TightInputTag");
 eleIDCut_RTightInputTag_ = iConfig.getParameter<edm::InputTag> ("eleIDCut_RTightInputTag");
 
 //---- flags ----
 saveMu_ =iConfig.getUntrackedParameter<bool> ("saveMu", true);
 saveTrack_ = iConfig.getUntrackedParameter<bool> ("saveTrack", true);
 saveEle_ = iConfig.getUntrackedParameter<bool> ("saveEle", true);
 saveJet_ = iConfig.getUntrackedParameter<bool> ("saveJet", true);
 saveMet_ = iConfig.getUntrackedParameter<bool> ("saveMet", true);
 saveGenJet_ = iConfig.getUntrackedParameter<bool> ("saveGenJet", true);
 saveGenMet_ = iConfig.getUntrackedParameter<bool> ("saveGenMet", true);
 saveMC_ = iConfig.getUntrackedParameter<bool> ("saveMC", true);

 verbosity_ = iConfig.getUntrackedParameter<bool>("verbosity","False");
 eventType_ = iConfig.getUntrackedParameter<int>("eventType",1);
 
 NtupleFactory_ = new NtupleFactory(outTree_);
}


// --------------------------------------------------------------------


SimpleNtple::~SimpleNtple()
{
 delete NtupleFactory_;
}


//
// member functions
//

///---- muons ----
void SimpleNtple::fillMuInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 edm::Handle<edm::View<reco::Muon> > MuHandle ;
 iEvent.getByLabel (MuTag_,MuHandle);
 int nMu;
 if(MuHandle->size() < 30 ){ nMu = MuHandle->size(); }
 else {nMu = 30;}
 for(int i=0; i< nMu; i++){
  NtupleFactory_->Fill4V("muons",(*MuHandle)[i].p4());
  NtupleFactory_->FillFloat("muons_charge",((*MuHandle)[i].charge()));
  NtupleFactory_->FillFloat("muons_tkIsoR03",((*MuHandle)[i].isolationR03()).sumPt);
  NtupleFactory_->FillFloat("muons_nTkIsoR03",((*MuHandle)[i].isolationR03()).nTracks);    
  NtupleFactory_->FillFloat("muons_emIsoR03",((*MuHandle)[i].isolationR03()).emEt);
  NtupleFactory_->FillFloat("muons_hadIsoR03",((*MuHandle)[i].isolationR03()).hadEt);
    
  NtupleFactory_->FillFloat("muons_tkIsoR05",((*MuHandle)[i].isolationR05()).sumPt);
  NtupleFactory_->FillFloat("muons_nTkIsoR05",((*MuHandle)[i].isolationR05()).nTracks);    
  NtupleFactory_->FillFloat("muons_emIsoR05",((*MuHandle)[i].isolationR05()).emEt);
  NtupleFactory_->FillFloat("muons_hadIsoR05",((*MuHandle)[i].isolationR05()).hadEt);
 }
}

///---- electrons ----

void SimpleNtple::fillEleInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 edm::Handle<edm::View<reco::GsfElectron> > EleHandle ;
 iEvent.getByLabel (EleTag_,EleHandle);
 int nEle;
 if(EleHandle->size() < 30 ){ nEle = EleHandle->size(); }
 else {nEle = 30;}
 
 std::vector<edm::Handle<edm::ValueMap<float> > > eleIdCutHandles(4) ;
 iEvent.getByLabel (eleIDCut_LooseInputTag_, eleIdCutHandles[0]) ;
 iEvent.getByLabel (eleIDCut_RLooseInputTag_, eleIdCutHandles[1]) ;
 iEvent.getByLabel (eleIDCut_TightInputTag_, eleIdCutHandles[2]) ;
 iEvent.getByLabel (eleIDCut_RTightInputTag_, eleIdCutHandles[3]) ;

 
 for(int i=0; i< nEle; i++){
  edm::Ref<edm::View<reco::GsfElectron> > electronEdmRef(EleHandle,i);
  
  NtupleFactory_->Fill4V("electrons",(*EleHandle)[i].p4());
  NtupleFactory_->FillFloat("electrons_charge",((*EleHandle)[i].charge()));
<<<<<<< SimpleNtple.cc
  NtupleFactory_->FillFloat("electrons_tkIso",((*EleHandle)[i].dr03TkSumPt()));
  NtupleFactory_->FillFloat("electrons_emIso",((*EleHandle)[i].dr03EcalRecHitSumEt()));
  NtupleFactory_->FillFloat("electrons_hadIso_1",((*EleHandle)[i].dr03HcalDepth1TowerSumEt()));
  NtupleFactory_->FillFloat("electrons_hadIso_2",((*EleHandle)[i].dr03HcalDepth2TowerSumEt()));
  //   if ((*EleHandle)[i].classification()== GsfElectron::GOLDEN
  
  //ELE ID
  NtupleFactory_->FillFloat("electrons_IdLoose",(*(eleIdCutHandles[0]))[electronEdmRef]);
  NtupleFactory_->FillFloat("electrons_IdTight",(*(eleIdCutHandles[1]))[electronEdmRef]);
  NtupleFactory_->FillFloat("electrons_IdRobustLoose",(*(eleIdCutHandles[2]))[electronEdmRef]);
  NtupleFactory_->FillFloat("electrons_IdRobustTight",(*(eleIdCutHandles[3]))[electronEdmRef]);
      
  //Get Ele Track
  reco::GsfTrackRef eleTrack  = (*EleHandle)[i].gsfTrack () ; 
      
  NtupleFactory_->FillFloat("electrons_track_d0", eleTrack->d0 ());
  NtupleFactory_->FillFloat("electrons_track_dz", eleTrack->dz ());
  NtupleFactory_->FillFloat("electrons_track_d0err", eleTrack->d0Error ());
  NtupleFactory_->FillFloat("electrons_track_dzerr", eleTrack->dzError ());
=======
  NtupleFactory_->FillFloat("electrons_tkIso",((*EleHandle)[i].dr03TkSumPt()));
  NtupleFactory_->FillFloat("electrons_emIso",((*EleHandle)[i].dr03EcalRecHitSumEt()));
  NtupleFactory_->FillFloat("electrons_hadIso",((*EleHandle)[i].dr03HcalDepth1TowerSumEt()));  NtupleFactory_->FillFloat("electrons_IdLoose",0);
  NtupleFactory_->FillFloat("electrons_IdTight",0);
  NtupleFactory_->FillFloat("electrons_IdRobustLoose",0);
  NtupleFactory_->FillFloat("electrons_IdRobustTight",0);  
>>>>>>> 1.2
 }
}

///---- tracks ----

void SimpleNtple::fillTrackInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 edm::Handle<edm::View<reco::Track> > TracksHandle ;
 iEvent.getByLabel (TracksTag_, TracksHandle) ;
 for (edm::View<reco::Track>::const_iterator tkIt = TracksHandle->begin (); tkIt != TracksHandle->end (); ++tkIt ) 
 { 
  NtupleFactory_->Fill3V("tracks_in",(*tkIt).innerMomentum ());
  NtupleFactory_->Fill3V("tracks_out",(*tkIt).outerMomentum ());
 }
}

///---- Jets ----
void SimpleNtple::fillJetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 edm::Handle<edm::View<reco::CaloJet> > JetHandle ;
 iEvent.getByLabel (JetTag_,JetHandle);
 edm::Handle<reco::JetTagCollection> bTagHandle;
 if (flag_JetBTag_) iEvent.getByLabel(JetBTag_, bTagHandle);
 reco::JetTagCollection bTags;
 if (flag_JetBTag_) bTags = *(bTagHandle.product());
 int counter_jet = 0;
 for (edm::View<reco::CaloJet>::const_iterator jetIt = JetHandle->begin (); jetIt != JetHandle->end (); ++jetIt ) 
 { 
  NtupleFactory_->Fill4V("jets",jetIt->p4());
  if (flag_JetBTag_) NtupleFactory_->FillFloat("jets_btagging",bTags[counter_jet].second);
  counter_jet++;
 }
}

///---- MET ----
void SimpleNtple::fillMetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 edm::Handle<reco::CaloMETCollection> MetHandle ;
 iEvent.getByLabel (MetTag_,MetHandle);
 const reco::CaloMET *calomet = &(MetHandle->front());
 NtupleFactory_->Fill4V("met",calomet->p4());
}


///---- GenMet ----
void SimpleNtple::fillGenMetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 edm::Handle< reco::GenMETCollection > genMetHandle ;
 iEvent.getByLabel( genMetTag_, genMetHandle ) ;
 for (reco::GenMETCollection::const_iterator gMIt = genMetHandle->begin (); gMIt != genMetHandle->end (); ++gMIt ) 
 {
  math::XYZTLorentzVector myvect_XYZT(gMIt->px(),gMIt->py(),gMIt->pz(),gMIt->energy());
  NtupleFactory_->Fill4V("genMet",myvect_XYZT);
 }
}


///---- GenJet ----
void SimpleNtple::fillGenJetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 edm::Handle<edm::View<reco::GenJet> > genJetHandle ;
 iEvent.getByLabel (genJetTag_,genJetHandle);
 for (edm::View<reco::GenJet>::const_iterator genJetIt = genJetHandle->begin (); genJetIt != genJetHandle->end (); ++genJetIt ) 
 { 
  NtupleFactory_->Fill4V("genJets",genJetIt->p4());
 }
}


///---- MC ----
void SimpleNtple::fillMCInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 edm::Handle<reco::GenParticleCollection> genParticles;
 iEvent.getByLabel(MCtruthTag_, genParticles);

//  int eventType_ = 1; //---- 0 = signal      1 = background 
//  bool verbosity_ = true; //---- true = loquacious     false = silence
 MCDumperVBF mcAnalysis(genParticles, eventType_, verbosity_); //---- i "tau" mi fanno scrivere a schermo anche se NON è segnale
 bool isValid = mcAnalysis.isValid();
  
 if( (eventType_ == 0) && (isValid == true) )
 {
  NtupleFactory_->Fill4V("mc_H",mcAnalysis.mcH()->p4());
  NtupleFactory_->FillFloat("mc_H_charge",mcAnalysis.mcH()->charge());
  
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
    
  NtupleFactory_->Fill4V("mcQ1_tag",mcAnalysis.mcQ1_tag()->p4());
  NtupleFactory_->FillFloat("mcQ1_tag_charge",mcAnalysis.mcQ1_tag()->charge());
  NtupleFactory_->FillFloat("mcQ1_tag_pdgId",mcAnalysis.mcQ1_tag()->pdgId());

  NtupleFactory_->Fill4V("mcQ2_tag",mcAnalysis.mcQ2_tag()->p4());
  NtupleFactory_->FillFloat("mcQ2_tag_charge",mcAnalysis.mcQ2_tag()->charge());
  NtupleFactory_->FillFloat("mcQ2_tag_pdgId",mcAnalysis.mcQ2_tag()->pdgId());
      
 }
 
 
 
 
}
 








// ------------ method called to for each event  ------------
void
  SimpleNtple::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 ///---- fill muons ----
 if (saveMu_) fillMuInfo (iEvent, iSetup);

 ///---- fill electrons ----
 if (saveEle_)  fillEleInfo (iEvent, iSetup);

 ///---- fill tracks ----
 if (saveTrack_) fillTrackInfo (iEvent, iSetup);
 
 ///---- fill jets ---- 
 if (saveJet_) fillJetInfo (iEvent, iSetup);

 ///---- fill met ---- 
 if (saveMet_) fillMetInfo (iEvent, iSetup);
 
 ///---- fill genJets ---- 
 if (saveGenJet_) fillGenJetInfo (iEvent, iSetup);

  ///---- fill genMet ---- 
 if (saveGenMet_)  fillGenMetInfo (iEvent, iSetup);
 
 ///---- fill MCParticle ---- 
 if (saveMC_) fillMCInfo (iEvent, iSetup);
  
 ///---- save the entry of the tree ----
 NtupleFactory_->FillNtuple();

}

    
// ------------ method called once each job just before starting event loop  ------------
  
  void 
    SimpleNtple::beginJob(const edm::EventSetup& iSetup)
{
   
 NtupleFactory_->Add4V("muons");
 NtupleFactory_->AddFloat("muons_charge"); 
 NtupleFactory_->AddFloat("muons_tkIsoR03"); 
 NtupleFactory_->AddFloat("muons_nTkIsoR03"); 
 NtupleFactory_->AddFloat("muons_emIsoR03"); 
 NtupleFactory_->AddFloat("muons_hadIsoR03"); 
 NtupleFactory_->AddFloat("muons_tkIsoR05"); 
 NtupleFactory_->AddFloat("muons_nTkIsoR05"); 
 NtupleFactory_->AddFloat("muons_emIsoR05"); 
 NtupleFactory_->AddFloat("muons_hadIsoR05"); 
   
 NtupleFactory_->Add4V("electrons");
 NtupleFactory_->AddFloat("electrons_charge"); 
 NtupleFactory_->AddFloat("electrons_tkIso"); 
 NtupleFactory_->AddFloat("electrons_emIso"); 
 NtupleFactory_->AddFloat("electrons_hadIso_1"); 
 NtupleFactory_->AddFloat("electrons_hadIso_2"); 
 NtupleFactory_->AddFloat("electrons_IdLoose"); 
 NtupleFactory_->AddFloat("electrons_IdTight"); 
 NtupleFactory_->AddFloat("electrons_IdRobustLoose"); 
 NtupleFactory_->AddFloat("electrons_IdRobustTight"); 
   
 NtupleFactory_->AddFloat("electrons_track_d0");
 NtupleFactory_->AddFloat("electrons_track_dz");
 NtupleFactory_->AddFloat("electrons_track_d0err");
 NtupleFactory_->AddFloat("electrons_track_dzerr");
 NtupleFactory_->Add3V("tracks_in");
 NtupleFactory_->Add3V("tracks_out");   
 NtupleFactory_->Add4V("jets");      
 NtupleFactory_->AddFloat("jets_btagging");   
   
 NtupleFactory_->Add4V("met");         
 NtupleFactory_->Add4V("genJets");         
 NtupleFactory_->Add4V("genMet");                

 NtupleFactory_->Add4V("mc_H");    
 NtupleFactory_->AddFloat("mc_H_charge");    
      
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
 
 NtupleFactory_->Add4V("mcQ1_tag");    
 NtupleFactory_->AddFloat("mcQ1_tag_charge");    
 NtupleFactory_->AddFloat("mcQ1_tag_pdgId");  
      
 NtupleFactory_->Add4V("mcQ2_tag");         
 NtupleFactory_->AddFloat("mcQ2_tag_charge");    
 NtupleFactory_->AddFloat("mcQ2_tag_pdgId");  
 
}
   
   
// ------------ method called once each job just after ending the event loop  ------------
     
  void 
    SimpleNtple::endJob() {
   
   NtupleFactory_->WriteNtuple();

    }

