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
// $Id: SimpleNtple.cc,v 1.21 2010/04/29 13:11:38 amassiro Exp $
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
#include "CommonTools/UtilAlgos/interface/TFileService.h"


//--- objects ----
#include "DataFormats/MuonReco/interface/MuonSelectors.h"

#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"

#include "DataFormats/Common/interface/ValueMap.h"






SimpleNtple::SimpleNtple(const edm::ParameterSet& iConfig)
{
 edm::Service<TFileService> fs ;
 outTree_  = fs->make <TTree>("SimpleNtple","SimpleNtple"); 
 
 HLTTag_ = iConfig.getParameter<edm::InputTag>("HLTTag");
 TracksTag_ = iConfig.getParameter<edm::InputTag>("TracksTag");
 Ele3DipSignificanceTag_ = iConfig.getParameter<edm::InputTag>("Ele3DipSignificanceTag");
 EleTipSignificanceTag_ = iConfig.getParameter<edm::InputTag>("EleTipSignificanceTag");
 EleLipSignificanceTag_ = iConfig.getParameter<edm::InputTag>("EleLipSignificanceTag");
 EleTag_ = iConfig.getParameter<edm::InputTag>("EleTag");
 MuTag_ = iConfig.getParameter<edm::InputTag>("MuTag");
 Mu3DipSignificanceTag_ = iConfig.getParameter<edm::InputTag>("Mu3DipSignificanceTag");
 MuTipSignificanceTag_ = iConfig.getParameter<edm::InputTag>("MuTipSignificanceTag");
 MuLipSignificanceTag_ = iConfig.getParameter<edm::InputTag>("MuLipSignificanceTag");
 MetTag_ = iConfig.getParameter<edm::InputTag>("MetTag");
 Type1MetTag_ = iConfig.getParameter<edm::InputTag>("Type1MetTag");
 PFMetTag_ = iConfig.getParameter<edm::InputTag>("PFMetTag");
 TcMetTag_ = iConfig.getParameter<edm::InputTag>("TcMetTag");
 JetTag_ = iConfig.getParameter<edm::InputTag>("JetTag");
 MCtruthTag_ = iConfig.getParameter<edm::InputTag>("MCtruthTag");
 genJetTag_ = iConfig.getParameter<edm::InputTag>("genJetTag");
 genMetTag_ = iConfig.getParameter<edm::InputTag>("genMetTag");

 eleIDCut_LooseInputTag_ = iConfig.getParameter<edm::InputTag> ("eleIDCut_LooseInputTag");
 eleIDCut_RLooseInputTag_ = iConfig.getParameter<edm::InputTag> ("eleIDCut_RLooseInputTag");
 eleIDCut_TightInputTag_ = iConfig.getParameter<edm::InputTag> ("eleIDCut_TightInputTag");
 eleIDCut_RTightInputTag_ = iConfig.getParameter<edm::InputTag> ("eleIDCut_RTightInputTag");
 
 jetIDTag_ = iConfig.getParameter<edm::InputTag> ("JetIDTag");

 //---- ref check ----
 doEleRefCheck_ = iConfig.getUntrackedParameter<bool>("doEleRefCheck", false);
 if(doEleRefCheck_) EleRefTag_  = iConfig.getParameter<edm::InputTag>("EleRefTag");
 
 doMuRefCheck_ = iConfig.getUntrackedParameter<bool>("doMuRefCheck", false);
 if(doMuRefCheck_) MuRefTag_ = iConfig.getParameter<edm::InputTag>("MuRefTag");
 
 doJetRefCheck_ = iConfig.getUntrackedParameter<bool>("doJetRefCheck", false);
 if(doJetRefCheck_) JetRefTag_ = iConfig.getParameter<edm::InputTag>("JetRefTag");
 
 //---- flags ----
 saveHLT_ =iConfig.getUntrackedParameter<bool> ("saveHLT", true);
 saveMu_ =iConfig.getUntrackedParameter<bool> ("saveMu", true);
 saveTrack_ = iConfig.getUntrackedParameter<bool> ("saveTrack", true);
 saveEle_ = iConfig.getUntrackedParameter<bool> ("saveEle", true);
 saveJet_ = iConfig.getUntrackedParameter<bool> ("saveJet", true);
 savePFJet_ = iConfig.getUntrackedParameter<bool> ("savePFJet", false);
 if(saveJet_ && savePFJet_) savePFJet_ = false;
 saveJetBTagging_ = iConfig.getUntrackedParameter<bool> ("saveJetBTagging", true);
 saveMet_ = iConfig.getUntrackedParameter<bool> ("saveMet", true);
 saveGenJet_ = iConfig.getUntrackedParameter<bool> ("saveGenJet", true);
 saveGenMet_ = iConfig.getUntrackedParameter<bool> ("saveGenMet", true);
 saveMCHiggs_ = iConfig.getUntrackedParameter<bool> ("saveMCHiggs", true);
 saveMCHiggsDecay_ = iConfig.getUntrackedParameter<bool> ("saveMCHiggsDecay", true);
 saveMCEle_ = iConfig.getUntrackedParameter<bool> ("saveMCEle", true);
 saveMCMu_ = iConfig.getUntrackedParameter<bool> ("saveMCMu", true);
 
 verbosity_ = iConfig.getUntrackedParameter<bool>("verbosity","False");
 eventType_ = iConfig.getUntrackedParameter<int>("eventType",1);

 mcAnalysis_ = NULL; 
 
 NtupleFactory_ = new NtupleFactory(outTree_);
 
 
 
  
 

 NtupleFactory_->AddInt("runId"); 
 NtupleFactory_->AddInt("lumiId"); 
 NtupleFactory_->AddInt("BXId"); 
 NtupleFactory_->AddInt("eventId"); 
 NtupleFactory_->AddInt("eventNaiveId"); 
 eventNaiveId_ = 0;
 
 if(saveHLT_)
 {
    NtupleFactory_->AddFloat("HLT_WasRun"); 
    NtupleFactory_->AddFloat("HLT_Accept"); 
    NtupleFactory_->AddFloat("HLT_Error"); 
//     const edm::TriggerNames & triggerNames = iEvent.triggerNames(*HLTR);
//     hlNames_=triggerNames.triggerNames();
//     const unsigned int n(hlNames_.size());
 }
 
 if(saveMu_)
  {
    NtupleFactory_->Add4V("muons");

    NtupleFactory_->AddInt("muons_global");
    NtupleFactory_->AddInt("muons_goodMuon");
    NtupleFactory_->AddFloat("muons_charge"); 
    NtupleFactory_->AddFloat("muons_tkIsoR03"); 
    NtupleFactory_->AddFloat("muons_nTkIsoR03"); 
    NtupleFactory_->AddFloat("muons_emIsoR03"); 
    NtupleFactory_->AddFloat("muons_hadIsoR03"); 
    NtupleFactory_->AddFloat("muons_tkIsoR05"); 
    NtupleFactory_->AddFloat("muons_nTkIsoR05"); 
    NtupleFactory_->AddFloat("muons_emIsoR05"); 
    NtupleFactory_->AddFloat("muons_hadIsoR05"); 
    NtupleFactory_->AddFloat("muons_3DipSignificance");    
    NtupleFactory_->AddFloat("muons_tipSignificance");    
    NtupleFactory_->AddFloat("muons_lipSignificance");    
  }
  
  if(saveEle_)
  {
    NtupleFactory_->Add4V("electrons");
    NtupleFactory_->AddFloat("electrons_charge"); 
    NtupleFactory_->AddFloat("electrons_tkIso"); 
    NtupleFactory_->AddFloat("electrons_emIso03"); 
    NtupleFactory_->AddFloat("electrons_emIso04"); 
    NtupleFactory_->AddFloat("electrons_hadIso03_1"); 
    NtupleFactory_->AddFloat("electrons_hadIso03_2"); 
    NtupleFactory_->AddFloat("electrons_hadIso04_1"); 
    NtupleFactory_->AddFloat("electrons_hadIso04_2"); 
    NtupleFactory_->AddFloat("electrons_IdLoose"); 
    NtupleFactory_->AddFloat("electrons_IdTight"); 
    NtupleFactory_->AddFloat("electrons_IdRobustLoose"); 
    NtupleFactory_->AddFloat("electrons_IdRobustTight"); 
    NtupleFactory_->AddFloat("electrons_track_d0");
    NtupleFactory_->AddFloat("electrons_track_dz");
    NtupleFactory_->AddFloat("electrons_track_d0err");
    NtupleFactory_->AddFloat("electrons_track_dzerr");
    NtupleFactory_->AddFloat("electrons_3DipSignificance");    
    NtupleFactory_->AddFloat("electrons_tipSignificance");    
    NtupleFactory_->AddFloat("electrons_lipSignificance");
    NtupleFactory_->AddFloat("electrons_scTheta");
    NtupleFactory_->AddFloat("electrons_scE");
    NtupleFactory_->AddFloat("electrons_eOverP");
    NtupleFactory_->AddFloat("electrons_eSeed");
    NtupleFactory_->AddFloat("electrons_fbrem");
    NtupleFactory_->AddFloat("electrons_sigmaIetaIeta");
    NtupleFactory_->AddFloat("electrons_pin");
    NtupleFactory_->AddFloat("electrons_pout");
    NtupleFactory_->AddFloat("electrons_hOverE");
    NtupleFactory_->AddFloat("electrons_deltaPhiIn");
    NtupleFactory_->AddFloat("electrons_deltaEtaIn");
    NtupleFactory_->AddInt("electrons_mishits");
  }
  
  if(saveTrack_)
  {
    NtupleFactory_->Add3V("tracks_in");
    NtupleFactory_->Add3V("tracks_out");   
  }
 
  if(saveJet_ || savePFJet_)
  {
    NtupleFactory_->Add4V("jets");
    
   
    NtupleFactory_->AddFloat("jets_trackCountingHighEffBJetTags");   
    NtupleFactory_->AddFloat("jets_trackCountingHighEffBJetTagsDR");   
    NtupleFactory_->AddFloat("jets_trackCountingHighPurBJetTags");   
    NtupleFactory_->AddFloat("jets_trackCountingHighPurBJetTagsDR");   
    NtupleFactory_->AddFloat("jets_simpleSecondaryVertexBJetTags");   
    NtupleFactory_->AddFloat("jets_simpleSecondaryVertexBJetTagsDR");   
    NtupleFactory_->AddFloat("jets_combinedSecondaryVertexBJetTags");   
    NtupleFactory_->AddFloat("jets_combinedSecondaryVertexBJetTagsDR");   
    NtupleFactory_->AddFloat("jets_combinedSecondaryVertexMVABJetTags");   
    NtupleFactory_->AddFloat("jets_combinedSecondaryVertexMVABJetTagsDR");   
    NtupleFactory_->AddFloat("jets_jetProbabilityBJetTags");   
    NtupleFactory_->AddFloat("jets_jetProbabilityBJetTagsDR");   
    NtupleFactory_->AddFloat("jets_jetBProbabilityBJetTags");   
    NtupleFactory_->AddFloat("jets_jetBProbabilityBJetTagsDR");   
    NtupleFactory_->AddFloat("jets_emEnergyFraction");   

    if(saveJet_)
    {
     NtupleFactory_->AddFloat("jets_etaetaMoment");   
     NtupleFactory_->AddFloat("jets_phiphiMoment");   
     NtupleFactory_->AddFloat("jets_etaphiMoment");   
     NtupleFactory_->AddFloat("jets_jetArea");   
     NtupleFactory_->AddFloat("jets_fHPD");   
     NtupleFactory_->AddFloat("jets_fRBX");   
     NtupleFactory_->AddFloat("jets_n90Hits");   
     NtupleFactory_->AddFloat("jets_nHCALTowers");   
     NtupleFactory_->AddFloat("jets_nECALTowers");   
    }
    if(savePFJet_){
     NtupleFactory_->AddFloat("jets_chargedHadronEnergy"); 
     NtupleFactory_->AddFloat("jets_chargedHadronEnergyFraction");
     NtupleFactory_->AddFloat("jets_neutralHadronEnergy"); 
     NtupleFactory_->AddFloat("jets_neutralHadronEnergyFraction"); 
     NtupleFactory_->AddFloat("jets_chargedEmEnergy"); 
     NtupleFactory_->AddFloat("jets_chargedEmEnergyFraction"); 
     NtupleFactory_->AddFloat("jets_chargedMuEnergy"); 
     NtupleFactory_->AddFloat("jets_chargedMuEnergyFraction"); 
     NtupleFactory_->AddFloat("jets_neutralEmEnergy"); 
     NtupleFactory_->AddFloat("jets_neutralEmEnergyFraction"); 
     NtupleFactory_->AddInt("jets_chargedMultiplicity"); 
     NtupleFactory_->AddInt("jets_neutralMultiplicity"); 
     NtupleFactory_->AddInt("jets_muonMultiplicity"); 
    }
    
  }
  
  if(saveGenJet_)
  {
    NtupleFactory_->Add4V("genJets");         
  }    
  
  if(saveMet_)
  {
   NtupleFactory_->Add4V("met");         
   NtupleFactory_->Add4V("type1Met");         
   NtupleFactory_->Add4V("TcMet");         
   NtupleFactory_->Add4V("PFMet");         
  }
  
  if(saveGenMet_)
  {
   NtupleFactory_->Add4V("genMet");                
  }
  
  if(saveMCHiggs_)
  {
    NtupleFactory_->Add4V("mc_H");    
    NtupleFactory_->AddFloat("mc_H_charge");    
  }

  if(saveMCHiggsDecay_)
  {
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
  
  if(saveMCEle_)
  {
    NtupleFactory_->Add4V("mc_ele");    
    NtupleFactory_->AddFloat("mc_ele_charge");    
  }

  if(saveMCMu_)
  {
    NtupleFactory_->Add4V("mc_mu");    
    NtupleFactory_->AddFloat("mc_mu_charge");    
  }
  

 
}

// --------------------------------------------------------------------


SimpleNtple::~SimpleNtple()
{
 NtupleFactory_->WriteNtuple();
 delete NtupleFactory_;
}


//
// member functions
//








 
///---- HLT ----
void SimpleNtple::fillHLTInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //---- HLT information ----
 edm::Handle<edm::TriggerResults> triggerResultsHandle;
 iEvent.getByLabel (HLTTag_,triggerResultsHandle);
//  iEvent.getByLabel ("TriggerResults",triggerResultsHandle);
//  std::cerr << "triggerResultsHandle->size() = " << triggerResultsHandle->size() << std::endl;
 for (int iHLT=0; iHLT<static_cast<int>(triggerResultsHandle->size()); iHLT++) {
//   for (int iHLT=0; iHLT<std::min(static_cast<int>(300),static_cast<int>(triggerResultsHandle->size())); iHLT++) {
  if (triggerResultsHandle->wasrun (iHLT)) {
   NtupleFactory_->FillFloat("HLT_WasRun",1);
  }
  else {
   NtupleFactory_->FillFloat("HLT_WasRun",0);
  }
  if (triggerResultsHandle->accept (iHLT)) {
   NtupleFactory_->FillFloat("HLT_Accept",1);
  }
  else {
   NtupleFactory_->FillFloat("HLT_Accept",0);
  }
  if (triggerResultsHandle->error (iHLT)) {
   NtupleFactory_->FillFloat("HLT_Error",1);
  }
  else {
   NtupleFactory_->FillFloat("HLT_Error",0);
  }
 }
}



///---- muons ----
void SimpleNtple::fillMuInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //std::cout << "SimpleNtple::fillMuInfo" << std::endl;
 
 edm::Handle<reco::MuonCollection> MuHandle ;
 iEvent.getByLabel (MuTag_,MuHandle);
 
 edm::Handle<edm::RefVector<reco::MuonCollection> > MuRefHandle;
 if(doMuRefCheck_)
   iEvent.getByLabel(MuRefTag_, MuRefHandle);
 
 
 edm::Handle<muMap> Mu3DipSignificanceHandle ;
 iEvent.getByLabel (Mu3DipSignificanceTag_,Mu3DipSignificanceHandle);
 edm::Handle<muMap> MuTipSignificanceHandle ;
 iEvent.getByLabel (MuTipSignificanceTag_,MuTipSignificanceHandle);
 edm::Handle<muMap> MuLipSignificanceHandle ;
 iEvent.getByLabel (MuLipSignificanceTag_,MuLipSignificanceHandle);
 
 
 for(unsigned int i=0; i<MuHandle->size(); i++){
  reco::MuonRef muRef(MuHandle, i);
  
  // do the reference check
  bool isMuRefCheckOk = true;
  if(doMuRefCheck_)
    if(find(MuRefHandle -> begin(), MuRefHandle -> end(), muRef) == MuRefHandle -> end())
      isMuRefCheckOk = false;

  if(!isMuRefCheckOk) continue;
  
  
  NtupleFactory_->Fill4V("muons",(*MuHandle)[i].p4());
  NtupleFactory_->FillInt("muons_global",((*MuHandle)[i].isGlobalMuon()));
  NtupleFactory_->FillInt("muons_goodMuon",muon::isGoodMuon((*MuHandle)[i], muon::GlobalMuonPromptTight));

  NtupleFactory_->FillFloat("muons_charge",((*MuHandle)[i].charge()));
  NtupleFactory_->FillFloat("muons_tkIsoR03",((*MuHandle)[i].isolationR03()).sumPt);
  NtupleFactory_->FillFloat("muons_nTkIsoR03",((*MuHandle)[i].isolationR03()).nTracks);    
  NtupleFactory_->FillFloat("muons_emIsoR03",((*MuHandle)[i].isolationR03()).emEt);
  NtupleFactory_->FillFloat("muons_hadIsoR03",((*MuHandle)[i].isolationR03()).hadEt);
    
  NtupleFactory_->FillFloat("muons_tkIsoR05",((*MuHandle)[i].isolationR05()).sumPt);
  NtupleFactory_->FillFloat("muons_nTkIsoR05",((*MuHandle)[i].isolationR05()).nTracks);    
  NtupleFactory_->FillFloat("muons_emIsoR05",((*MuHandle)[i].isolationR05()).emEt);
  NtupleFactory_->FillFloat("muons_hadIsoR05",((*MuHandle)[i].isolationR05()).hadEt);

  NtupleFactory_->FillFloat("muons_3DipSignificance",((*Mu3DipSignificanceHandle)[muRef]));
  NtupleFactory_->FillFloat("muons_tipSignificance",((*MuTipSignificanceHandle)[muRef]));
  NtupleFactory_->FillFloat("muons_lipSignificance",((*MuLipSignificanceHandle)[muRef]));
 }
}






///---- electrons ----

void SimpleNtple::fillEleInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //std::cout << "SimpleNtple::fillEleInfo" << std::endl;
 
 edm::Handle<reco::GsfElectronCollection> EleHandle ;
 iEvent.getByLabel (EleTag_,EleHandle);
 
 edm::Handle<edm::RefVector<reco::GsfElectronCollection> > EleRefHandle;
 if(doEleRefCheck_)
   iEvent.getByLabel(EleRefTag_, EleRefHandle);

 edm::Handle<eleMap> Ele3DipSignificanceHandle ;
 iEvent.getByLabel (Ele3DipSignificanceTag_,Ele3DipSignificanceHandle);
 edm::Handle<eleMap> EleTipSignificanceHandle ;
 iEvent.getByLabel (EleTipSignificanceTag_,EleTipSignificanceHandle);
 edm::Handle<eleMap> EleLipSignificanceHandle ;
 iEvent.getByLabel (EleLipSignificanceTag_,EleLipSignificanceHandle);
  
 std::vector<edm::Handle<edm::ValueMap<float> > > eleIdCutHandles(4) ;
 iEvent.getByLabel (eleIDCut_LooseInputTag_, eleIdCutHandles[0]) ;
 iEvent.getByLabel (eleIDCut_RLooseInputTag_, eleIdCutHandles[1]) ;
 iEvent.getByLabel (eleIDCut_TightInputTag_, eleIdCutHandles[2]) ;
 iEvent.getByLabel (eleIDCut_RTightInputTag_, eleIdCutHandles[3]) ;

 
 for(unsigned int i=0; i<EleHandle->size(); i++){
  reco::GsfElectronRef eleRef(EleHandle, i);  

  // do the reference check 
  bool isEleRefCheckOk = true;
  if(doEleRefCheck_)
    if(find(EleRefHandle -> begin(), EleRefHandle -> end(), eleRef) == EleRefHandle -> end())
      isEleRefCheckOk = false;

  if(!isEleRefCheckOk) continue;

  NtupleFactory_->Fill4V("electrons",(*EleHandle)[i].p4());
  NtupleFactory_->FillFloat("electrons_charge",((*EleHandle)[i].charge()));
  NtupleFactory_->FillFloat("electrons_tkIso",((*EleHandle)[i].dr03TkSumPt()));
  NtupleFactory_->FillFloat("electrons_emIso03",((*EleHandle)[i].dr03EcalRecHitSumEt()));
  NtupleFactory_->FillFloat("electrons_emIso04",((*EleHandle)[i].dr04EcalRecHitSumEt()));
  NtupleFactory_->FillFloat("electrons_hadIso03_1",((*EleHandle)[i].dr03HcalDepth1TowerSumEt()));
  NtupleFactory_->FillFloat("electrons_hadIso03_2",((*EleHandle)[i].dr03HcalDepth2TowerSumEt()));
  NtupleFactory_->FillFloat("electrons_hadIso04_1",((*EleHandle)[i].dr04HcalDepth1TowerSumEt()));
  NtupleFactory_->FillFloat("electrons_hadIso04_2",((*EleHandle)[i].dr04HcalDepth2TowerSumEt()));
  //   if ((*EleHandle)[i].classification()== GsfElectron::GOLDEN
  
  //ELE ID
  NtupleFactory_->FillFloat("electrons_IdLoose",(*(eleIdCutHandles[0]))[eleRef]) ;
  NtupleFactory_->FillFloat("electrons_IdRobustLoose",(*(eleIdCutHandles[1]))[eleRef]);
  NtupleFactory_->FillFloat("electrons_IdTight",(*(eleIdCutHandles[2]))[eleRef]);
  NtupleFactory_->FillFloat("electrons_IdRobustTight",(*(eleIdCutHandles[3]))[eleRef]);

  NtupleFactory_->FillFloat("electrons_sigmaIetaIeta",(*EleHandle)[i].sigmaIetaIeta());
  NtupleFactory_->FillFloat("electrons_scTheta",(2*atan(exp(-(*EleHandle)[i].superCluster()->eta()))));
  NtupleFactory_->FillFloat("electrons_scE",(*EleHandle)[i].superCluster()->energy());
  NtupleFactory_->FillFloat("electrons_eOverP",(*EleHandle)[i].eSuperClusterOverP());
  NtupleFactory_->FillFloat("electrons_eSeed",(*EleHandle)[i].superCluster()->seed()->energy());
  NtupleFactory_->FillFloat("electrons_fbrem",(*EleHandle)[i].fbrem());
  NtupleFactory_->FillFloat("electrons_pin",(*EleHandle)[i].trackMomentumAtVtx().R());
  NtupleFactory_->FillFloat("electrons_pout",(*EleHandle)[i].trackMomentumOut().R());
  NtupleFactory_->FillFloat("electrons_hOverE",(*EleHandle)[i].hadronicOverEm());
  NtupleFactory_->FillFloat("electrons_deltaPhiIn",(*EleHandle)[i].deltaPhiSuperClusterTrackAtVtx());
  NtupleFactory_->FillFloat("electrons_deltaEtaIn",(*EleHandle)[i].deltaEtaSuperClusterTrackAtVtx());
  NtupleFactory_->FillInt("electrons_mishits",(*EleHandle)[i].gsfTrack()->trackerExpectedHitsInner().numberOfHits());

  //Get Ele Track
  reco::GsfTrackRef eleTrack  = (*EleHandle)[i].gsfTrack () ; 
      
  NtupleFactory_->FillFloat("electrons_track_d0", eleTrack->d0 ());
  NtupleFactory_->FillFloat("electrons_track_dz", eleTrack->dz ());
  NtupleFactory_->FillFloat("electrons_track_d0err", eleTrack->d0Error ());
  NtupleFactory_->FillFloat("electrons_track_dzerr", eleTrack->dzError ());
  
  NtupleFactory_->FillFloat("electrons_3DipSignificance",((*Ele3DipSignificanceHandle)[eleRef]));
  NtupleFactory_->FillFloat("electrons_tipSignificance",((*EleTipSignificanceHandle)[eleRef]));
  NtupleFactory_->FillFloat("electrons_lipSignificance",((*EleLipSignificanceHandle)[eleRef]));
 }
}






///---- tracks ----

void SimpleNtple::fillTrackInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //std::cout << "SimpleNtple::fillTrackInfo" << std::endl;
 
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
  //std::cout << "SimpleNtple::fillJetInfo" << std::endl;
   edm::Handle<edm::View<reco::Jet> > JetHandle_forID;
//  edm::Handle<reco::CaloJetCollection> JetHandle_forID ;
 iEvent.getByLabel ("ak5CaloJets",JetHandle_forID);
 
 edm::Handle<reco::CaloJetCollection> JetHandle ;
 iEvent.getByLabel (JetTag_,JetHandle);

 edm::Handle<edm::RefVector<reco::CaloJetCollection> > JetRefHandle;
 if(doJetRefCheck_)
   iEvent.getByLabel(JetRefTag_, JetRefHandle);
 
 edm::Handle<reco::JetIDValueMap> jetIDHandle;
 iEvent.getByLabel (jetIDTag_, jetIDHandle) ;

  
 for(unsigned int i=0; i<JetHandle->size(); ++i) 
 { 
   reco::CaloJetRef jetRef(JetHandle, i);
   
   // do the reference check
   bool isJetRefCheckOk = true;
   if(doJetRefCheck_)
     if(find(JetRefHandle -> begin(), JetRefHandle -> end(), jetRef) == JetRefHandle -> end())
       isJetRefCheckOk = false;

   if(!isJetRefCheckOk) continue;
   
   NtupleFactory_->Fill4V("jets",(*JetHandle)[i].p4());
   NtupleFactory_->FillFloat("jets_emEnergyFraction",(*JetHandle)[i].emEnergyFraction());
   NtupleFactory_->FillFloat("jets_etaetaMoment",(*JetHandle)[i].etaetaMoment());
   NtupleFactory_->FillFloat("jets_phiphiMoment",(*JetHandle)[i].phiphiMoment());
   NtupleFactory_->FillFloat("jets_etaphiMoment",(*JetHandle)[i].etaphiMoment());
   NtupleFactory_->FillFloat("jets_jetArea",(*JetHandle)[i].jetArea());
  
   
   float DRMin = 999999;
   int found = 0;
   
   edm::View<reco::Jet>::const_iterator itSelJet = JetHandle_forID->end();
//    reco::CaloJetCollection::const_iterator itSelJet = JetHandle_forID->end();
   
   for (edm::View<reco::Jet>::const_iterator itJet = JetHandle_forID->begin(); itJet < JetHandle_forID->end(); itJet++){
   
//    for (reco::CaloJetCollection::const_iterator itJet = JetHandle_forID->begin(); itJet < JetHandle_forID->end(); itJet++){
     float DRTemp = ROOT::Math::VectorUtil::DeltaR((*JetHandle)[i].p4(), itJet->p4());
      if(DRTemp < DRMin){
       itSelJet = itJet;
       DRMin = DRTemp;
      }
   }
   
   if (itSelJet != JetHandle_forID->end()){
    unsigned int idx = itSelJet - JetHandle_forID->begin();
    edm::RefToBase<reco::Jet> jetRef = JetHandle_forID->refAt(idx);
    NtupleFactory_->FillFloat("jets_fHPD",((*jetIDHandle)[jetRef]).fHPD);
    NtupleFactory_->FillFloat("jets_fRBX",((*jetIDHandle)[jetRef]).fRBX);
    NtupleFactory_->FillFloat("jets_n90Hits",((*jetIDHandle)[jetRef]).n90Hits);
    NtupleFactory_->FillFloat("jets_nHCALTowers",((*jetIDHandle)[jetRef]).nHCALTowers);
    NtupleFactory_->FillFloat("jets_nECALTowers",((*jetIDHandle)[jetRef]).nECALTowers);
   }
   else {
    NtupleFactory_->FillFloat("jets_fHPD",-999);
    NtupleFactory_->FillFloat("jets_fRBX",-999);
    NtupleFactory_->FillFloat("jets_n90Hits",-999);
    NtupleFactory_->FillFloat("jets_nHCALTowers",-999);
    NtupleFactory_->FillFloat("jets_nECALTowers",-999);
   }
   
   
   if(saveJetBTagging_)
     fillJetBTaggingInfo(iEvent, iESetup, (*JetHandle)[i].p4());
   
  }// loop on jets
 
}






///---- PFJets ----
void SimpleNtple::fillPFJetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //std::cout << "SimpleNtple::fillPFJetInfo" << std::endl;
 
 edm::Handle<reco::PFJetCollection> JetHandle ;
 iEvent.getByLabel (JetTag_,JetHandle);
 
 edm::Handle<edm::RefVector<reco::PFJetCollection> > JetRefHandle;
 if(doJetRefCheck_)
   iEvent.getByLabel(JetRefTag_, JetRefHandle);
 
 
 for(unsigned int i=0; i<JetHandle->size(); ++i) 
 { 
   reco::PFJetRef jetRef(JetHandle, i);
   
   // do the reference check
   bool isJetRefCheckOk = true;
   if(doJetRefCheck_)
     if(find(JetRefHandle -> begin(), JetRefHandle -> end(), jetRef) == JetRefHandle -> end())
       isJetRefCheckOk = false;

   if(!isJetRefCheckOk) continue;
   
   NtupleFactory_->Fill4V("jets",(*JetHandle)[i].p4());
   NtupleFactory_->FillFloat("jets_emEnergyFraction",(*JetHandle)[i].neutralHadronEnergyFraction()); 
   
   NtupleFactory_->FillFloat("jets_chargedHadronEnergy",(*JetHandle)[i].chargedHadronEnergy()); 
   NtupleFactory_->FillFloat("jets_chargedHadronEnergyFraction",(*JetHandle)[i].chargedHadronEnergyFraction()); 
   NtupleFactory_->FillFloat("jets_neutralHadronEnergy",(*JetHandle)[i].neutralHadronEnergy()); 
   NtupleFactory_->FillFloat("jets_neutralHadronEnergyFraction",(*JetHandle)[i].neutralHadronEnergyFraction()); 
     
   NtupleFactory_->FillFloat("jets_chargedEmEnergy",(*JetHandle)[i].chargedEmEnergy()); 
   NtupleFactory_->FillFloat("jets_chargedEmEnergyFraction",(*JetHandle)[i].chargedEmEnergyFraction()); 
   NtupleFactory_->FillFloat("jets_chargedMuEnergy",(*JetHandle)[i].chargedMuEnergy()); 
   NtupleFactory_->FillFloat("jets_chargedMuEnergyFraction",(*JetHandle)[i].chargedMuEnergyFraction()); 
   NtupleFactory_->FillFloat("jets_neutralEmEnergy",(*JetHandle)[i].neutralEmEnergy()); 
   NtupleFactory_->FillFloat("jets_neutralEmEnergyFraction",(*JetHandle)[i].neutralEmEnergyFraction()); 
   
   NtupleFactory_->FillInt("jets_chargedMultiplicity",(*JetHandle)[i].chargedMultiplicity()); 
   NtupleFactory_->FillInt("jets_neutralMultiplicity",(*JetHandle)[i].neutralMultiplicity()); 
   NtupleFactory_->FillInt("jets_muonMultiplicity",(*JetHandle)[i].muonMultiplicity()); 
 
   if(saveJetBTagging_)
     fillJetBTaggingInfo(iEvent, iESetup, (*JetHandle)[i].p4());
  }
}






///---- JetBTagging ----
void SimpleNtple::fillJetBTaggingInfo(const edm::Event & iEvent, const edm::EventSetup & iESetup,
                                      const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >& v1)
{
  //std::cout << "SimpleNtple::fillJetBTaggingInfo" << std::endl;
  
  edm::Handle<reco::JetTagCollection> bTagHandle_trackCountingHighEff;
  iEvent.getByLabel("trackCountingHighEffBJetTags", bTagHandle_trackCountingHighEff);  

  edm::Handle<reco::JetTagCollection> bTagHandle_trackCountingHighPur;
  iEvent.getByLabel("trackCountingHighPurBJetTags", bTagHandle_trackCountingHighPur);  

  edm::Handle<reco::JetTagCollection> bTagHandle_simpleSecondaryVertex;
  iEvent.getByLabel("simpleSecondaryVertexBJetTags", bTagHandle_simpleSecondaryVertex);
  
  edm::Handle<reco::JetTagCollection> bTagHandle_combinedSecondaryVertex;
  iEvent.getByLabel("combinedSecondaryVertexBJetTags", bTagHandle_combinedSecondaryVertex);
  
  edm::Handle<reco::JetTagCollection> bTagHandle_combinedSecondaryVertexMVA;
  iEvent.getByLabel("combinedSecondaryVertexMVABJetTags", bTagHandle_combinedSecondaryVertexMVA);  
  
  edm::Handle<reco::JetTagCollection> bTagHandle_jetProbability;
  iEvent.getByLabel("jetProbabilityBJetTags", bTagHandle_jetProbability);
  
  edm::Handle<reco::JetTagCollection> bTagHandle_jetBProbability;
  iEvent.getByLabel("jetBProbabilityBJetTags", bTagHandle_jetBProbability);
  
  
  
    // trackCountingHighEff
    float DRMin = 999999;
    int jMin = -1;
    
    for(unsigned int j = 0; j < bTagHandle_trackCountingHighEff->size(); ++j)
    {
      float DRTemp = ROOT::Math::VectorUtil::DeltaR(v1, (*bTagHandle_trackCountingHighEff)[j].first->p4());
      if(DRTemp < DRMin)
      {
        DRMin = DRTemp;
        jMin = j;
      }
    }
    
    if(jMin != -1)
    {
      NtupleFactory_->FillFloat("jets_trackCountingHighEffBJetTags", (*bTagHandle_trackCountingHighEff)[jMin].second);
      NtupleFactory_->FillFloat("jets_trackCountingHighEffBJetTagsDR", DRMin);
    }
    else
    {
      NtupleFactory_->FillFloat("jets_trackCountingHighEffBJetTags", -999999.);
      NtupleFactory_->FillFloat("jets_trackCountingHighEffBJetTagsDR", -1.);
    }
    
    
    
    // trackCountingHighPur
    DRMin = 999999;
    jMin = -1;
    
    for(unsigned int j = 0; j < bTagHandle_trackCountingHighPur->size(); ++j)
    {
      float DRTemp = ROOT::Math::VectorUtil::DeltaR(v1, (*bTagHandle_trackCountingHighPur)[j].first->p4());
      if(DRTemp < DRMin)
      {
        DRMin = DRTemp;
        jMin = j;
      }
    }
    
    if(jMin != -1)
    {
      NtupleFactory_->FillFloat("jets_trackCountingHighPurBJetTags", (*bTagHandle_trackCountingHighPur)[jMin].second);
      NtupleFactory_->FillFloat("jets_trackCountingHighPurBJetTagsDR", DRMin);
    }
    else
    {
      NtupleFactory_->FillFloat("jets_trackCountingHighPurBJetTags", -999999.);
      NtupleFactory_->FillFloat("jets_trackCountingHighPurBJetTagsDR", -1.);
    }
    
    
    
    // simpleSecondaryVertex
    DRMin = 999999;
    jMin = -1;
    
    for(unsigned int j = 0; j < bTagHandle_simpleSecondaryVertex->size(); ++j)
    {
      float DRTemp = ROOT::Math::VectorUtil::DeltaR(v1, (*bTagHandle_simpleSecondaryVertex)[j].first->p4());
      if(DRTemp < DRMin)
      {
        DRMin = DRTemp;
        jMin = j;
      }
    }
    
    if(jMin != -1)
    {
      NtupleFactory_->FillFloat("jets_simpleSecondaryVertexBJetTags", (*bTagHandle_simpleSecondaryVertex)[jMin].second);
      NtupleFactory_->FillFloat("jets_simpleSecondaryVertexBJetTagsDR", DRMin);
    }
    else
    {
      NtupleFactory_->FillFloat("jets_simpleSecondaryVertexBJetTags", -999999.);
      NtupleFactory_->FillFloat("jets_simpleSecondaryVertexBJetTagsDR", -1.);
    }
    
    
    
    // combinedSecondaryVertex
    DRMin = 999999;
    jMin = -1;
    
    for(unsigned int j = 0; j < bTagHandle_combinedSecondaryVertex->size(); ++j)
    {
      float DRTemp = ROOT::Math::VectorUtil::DeltaR(v1, (*bTagHandle_combinedSecondaryVertex)[j].first->p4());
      if(DRTemp < DRMin)
      {
        DRMin = DRTemp;
        jMin = j;
      }
    }
    
    if(jMin != -1)
    {
      NtupleFactory_->FillFloat("jets_combinedSecondaryVertexBJetTags", (*bTagHandle_combinedSecondaryVertex)[jMin].second);
      NtupleFactory_->FillFloat("jets_combinedSecondaryVertexBJetTagsDR", DRMin);
    }
    else
    {
      NtupleFactory_->FillFloat("jets_combinedSecondaryVertexBJetTags", -999999.);
      NtupleFactory_->FillFloat("jets_combinedSecondaryVertexBJetTagsDR", -1.);
    }
    
    
    
    // combinedSecondaryVertexMVA
    DRMin = 999999;
    jMin = -1;
    
    for(unsigned int j = 0; j < bTagHandle_combinedSecondaryVertexMVA->size(); ++j)
    {
      float DRTemp = ROOT::Math::VectorUtil::DeltaR(v1, (*bTagHandle_combinedSecondaryVertexMVA)[j].first->p4());
      if(DRTemp < DRMin)
      {
        DRMin = DRTemp;
        jMin = j;
      }
    }
    
    if(jMin != -1)
    {
      NtupleFactory_->FillFloat("jets_combinedSecondaryVertexMVABJetTags", (*bTagHandle_combinedSecondaryVertexMVA)[jMin].second);
      NtupleFactory_->FillFloat("jets_combinedSecondaryVertexMVABJetTagsDR", DRMin);
    }
    else
    {
      NtupleFactory_->FillFloat("jets_combinedSecondaryVertexMVABJetTags", -999999.);
      NtupleFactory_->FillFloat("jets_combinedSecondaryVertexMVABJetTagsDR", -1.);
    }
    
    
    
    // jetProbability
    DRMin = 999999;
    jMin = -1;
    
    for(unsigned int j = 0; j < bTagHandle_jetProbability->size(); ++j)
    {
      float DRTemp = ROOT::Math::VectorUtil::DeltaR(v1, (*bTagHandle_jetProbability)[j].first->p4());
      if(DRTemp < DRMin)
      {
        DRMin = DRTemp;
        jMin = j;
      }
    }
    
    if(jMin != -1)
    {
      NtupleFactory_->FillFloat("jets_jetProbabilityBJetTags", (*bTagHandle_jetProbability)[jMin].second);
      NtupleFactory_->FillFloat("jets_jetProbabilityBJetTagsDR", DRMin);
    }
    else
    {
      NtupleFactory_->FillFloat("jets_jetProbabilityBJetTags", -999999.);
      NtupleFactory_->FillFloat("jets_jetProbabilityBJetTagsDR", -1.);
    }
    
    
    
    // jetBProbability
    DRMin = 999999;
    jMin = -1;
    
    for(unsigned int j = 0; j < bTagHandle_jetBProbability->size(); ++j)
    {
      float DRTemp = ROOT::Math::VectorUtil::DeltaR(v1, (*bTagHandle_jetBProbability)[j].first->p4());
      if(DRTemp < DRMin)
      {
        DRMin = DRTemp;
        jMin = j;
      }
    }
    
    if(jMin != -1)
    {
      NtupleFactory_->FillFloat("jets_jetBProbabilityBJetTags", (*bTagHandle_jetBProbability)[jMin].second);
      NtupleFactory_->FillFloat("jets_jetBProbabilityBJetTagsDR", DRMin);
    }
    else
    {
      NtupleFactory_->FillFloat("jets_jetBProbabilityBJetTags", -999999.);
      NtupleFactory_->FillFloat("jets_jetBProbabilityBJetTagsDR", -1.);
    }
    
}  






///---- MET ----

void SimpleNtple::fillMetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //std::cout << "SimpleNtple::fillMetInfo" << std::endl;
 
 edm::Handle<reco::CaloMETCollection> MetHandle ;
 iEvent.getByLabel (MetTag_,MetHandle);
 
 edm::Handle<reco::CaloMETCollection> Type1MetHandle ;
 iEvent.getByLabel (Type1MetTag_,Type1MetHandle);
 
 edm::Handle<reco::METCollection> TcMetHandle ;
 iEvent.getByLabel (TcMetTag_,TcMetHandle);
 
 edm::Handle<reco::PFMETCollection> PFMetHandle ;
 iEvent.getByLabel (PFMetTag_,PFMetHandle);
 
 
 
 const reco::CaloMET* met = &(MetHandle->front());
 NtupleFactory_->Fill4V("met",met->p4());
 
 const reco::MET* type1Met = &(Type1MetHandle->front());
 NtupleFactory_->Fill4V("type1Met",type1Met->p4());
 
 const reco::MET* TcMet = &(TcMetHandle->front());
 NtupleFactory_->Fill4V("TcMet",TcMet->p4());
 
 const reco::PFMET* PFMet = &(PFMetHandle->front());
 NtupleFactory_->Fill4V("PFMet",PFMet->p4());
}






///---- GenMet ----

void SimpleNtple::fillGenMetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //std::cout << "SimpleNtple::fillGenMetInfo" << std::endl;
 
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
 //std::cout << "SimpleNtple::fillGenJetInfo" << std::endl;
 
 edm::Handle<edm::View<reco::GenJet> > genJetHandle ;
 iEvent.getByLabel (genJetTag_,genJetHandle);
 for (edm::View<reco::GenJet>::const_iterator genJetIt = genJetHandle->begin (); genJetIt != genJetHandle->end (); ++genJetIt ) 
 { 
  NtupleFactory_->Fill4V("genJets",genJetIt->p4());
 }
}






///---- MC ----

void SimpleNtple::fillMCHiggsInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //std::cout << "SimpleNtple::fillMCHiggsInfo" << std::endl; 
 
 NtupleFactory_->Fill4V("mc_H",mcAnalysis_ -> mcH()->p4());
 NtupleFactory_->FillFloat("mc_H_charge",mcAnalysis_ -> mcH()->charge());
 
}



void SimpleNtple::fillMCHiggsDecayInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //std::cout << "SimpleNtple::fillMCHiggsDecayInfo" << std::endl; 

 bool isValid = mcAnalysis_ -> isValid();
  
 if( (eventType_ == 0) && (isValid == true) )
 {
  NtupleFactory_->Fill4V("mcV1",mcAnalysis_ -> mcV1()->p4());
  NtupleFactory_->FillFloat("mcV1_charge",mcAnalysis_ -> mcV1()->charge());
  NtupleFactory_->FillFloat("mcV1_pdgId",mcAnalysis_ -> mcV1()->pdgId());
  
  NtupleFactory_->Fill4V("mcV2",mcAnalysis_ -> mcV2()->p4());
  NtupleFactory_->FillFloat("mcV2_charge",mcAnalysis_ -> mcV2()->charge());
  NtupleFactory_->FillFloat("mcV2_pdgId",mcAnalysis_ -> mcV2()->pdgId());
     
  NtupleFactory_->Fill4V("mcF1_fromV1",mcAnalysis_ -> mcF1_fromV1()->p4());
  NtupleFactory_->FillFloat("mcF1_fromV1_charge",mcAnalysis_ -> mcF1_fromV1()->charge());
  NtupleFactory_->FillFloat("mcF1_fromV1_pdgId",mcAnalysis_ -> mcF1_fromV1()->pdgId());

  NtupleFactory_->Fill4V("mcF2_fromV1",mcAnalysis_ -> mcF2_fromV1()->p4());
  NtupleFactory_->FillFloat("mcF2_fromV1_charge",mcAnalysis_ -> mcF2_fromV1()->charge());
  NtupleFactory_->FillFloat("mcF2_fromV1_pdgId",mcAnalysis_ -> mcF2_fromV1()->pdgId());

  NtupleFactory_->Fill4V("mcF1_fromV2",mcAnalysis_ -> mcF1_fromV2()->p4());
  NtupleFactory_->FillFloat("mcF1_fromV2_charge",mcAnalysis_ -> mcF1_fromV2()->charge());
  NtupleFactory_->FillFloat("mcF1_fromV2_pdgId",mcAnalysis_ -> mcF1_fromV2()->pdgId());

  NtupleFactory_->Fill4V("mcF2_fromV2",mcAnalysis_ -> mcF2_fromV2()->p4());
  NtupleFactory_->FillFloat("mcF2_fromV2_charge",mcAnalysis_ -> mcF2_fromV2()->charge());
  NtupleFactory_->FillFloat("mcF2_fromV2_pdgId",mcAnalysis_ -> mcF2_fromV2()->pdgId());
    
  NtupleFactory_->Fill4V("mcQ1_tag",mcAnalysis_ -> mcQ1_tag()->p4());
  NtupleFactory_->FillFloat("mcQ1_tag_charge",mcAnalysis_ -> mcQ1_tag()->charge());
  NtupleFactory_->FillFloat("mcQ1_tag_pdgId",mcAnalysis_ -> mcQ1_tag()->pdgId());

  NtupleFactory_->Fill4V("mcQ2_tag",mcAnalysis_ -> mcQ2_tag()->p4());
  NtupleFactory_->FillFloat("mcQ2_tag_charge",mcAnalysis_ -> mcQ2_tag()->charge());
  NtupleFactory_->FillFloat("mcQ2_tag_pdgId",mcAnalysis_ -> mcQ2_tag()->pdgId());
      
 }
 
}
 


void SimpleNtple::fillMCEleInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //std::cout << "SimpleNtple::fillMCEleInfo" << std::endl; 
 
  for(unsigned int eleIt = 0; eleIt < (mcAnalysis_ -> GetMcE()).size(); ++eleIt)
  {
    NtupleFactory_->Fill4V("mc_ele", (mcAnalysis_ -> GetMcE()).at(eleIt)->p4());
    NtupleFactory_->FillFloat("mc_ele_charge", (mcAnalysis_ -> GetMcE()).at(eleIt)->charge());
  }
}



void SimpleNtple::fillMCMuInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //std::cout << "SimpleNtple::fillMCMuInfo" << std::endl; 
 
 for(unsigned int muIt = 0; muIt < (mcAnalysis_ -> GetMcMu()).size(); ++muIt)
 {
   NtupleFactory_->Fill4V("mc_mu", (mcAnalysis_ -> GetMcMu()).at(muIt)->p4());
   NtupleFactory_->FillFloat("mc_mu_charge", (mcAnalysis_ -> GetMcMu()).at(muIt)->charge());
 }
}











// ------------ method called to for each event  ------------
void SimpleNtple::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  ++eventNaiveId_;
  
  NtupleFactory_->FillInt("runId", iEvent.id().run());
  NtupleFactory_->FillInt("lumiId", iEvent.luminosityBlock());
  NtupleFactory_->FillInt("BXId", iEvent.bunchCrossing());
  NtupleFactory_->FillInt("eventId", iEvent.id().event());
  NtupleFactory_->FillInt("eventNaiveId", eventNaiveId_);





 edm::Handle<reco::GenParticleCollection> genParticles;

 if(saveMCHiggs_ || saveMCHiggsDecay_ || saveMCEle_ || saveMCMu_)
 {
   iEvent.getByLabel(MCtruthTag_, genParticles);
  
   mcAnalysis_ = new MCDumperVBF(genParticles, eventType_, verbosity_);
 }
  
 ///---- fill HLT ----
 if (saveHLT_) fillHLTInfo (iEvent, iSetup);
  
 ///---- fill muons ----
 if (saveMu_) fillMuInfo (iEvent, iSetup);

 ///---- fill electrons ----
 if (saveEle_)  fillEleInfo (iEvent, iSetup);

 ///---- fill tracks ----
 if (saveTrack_) fillTrackInfo (iEvent, iSetup);
 
 ///---- fill jets ---- 
 if (saveJet_) fillJetInfo (iEvent, iSetup);
 
 ///---- fill jets ---- 
 if (savePFJet_) fillPFJetInfo (iEvent, iSetup);

 ///---- fill genJets ---- 
 if (saveGenJet_) fillGenJetInfo (iEvent, iSetup);

 ///---- fill met ---- 
 if (saveMet_) fillMetInfo (iEvent, iSetup);
 
 ///---- fill genMet ---- 
 if (saveGenMet_)  fillGenMetInfo (iEvent, iSetup);
 
 ///---- fill MCParticle ---- 
 if (saveMCHiggs_) fillMCHiggsInfo (iEvent, iSetup);

 ///---- fill MCParticle ---- 
 if (saveMCHiggsDecay_) fillMCHiggsDecayInfo (iEvent, iSetup);

 ///---- fill MCParticle ---- 
 if (saveMCEle_) fillMCEleInfo (iEvent, iSetup);

 ///---- fill MCParticle ---- 
 if (saveMCMu_) fillMCMuInfo (iEvent, iSetup);

 ///---- save the entry of the tree ----
 NtupleFactory_->FillNtuple();

}
