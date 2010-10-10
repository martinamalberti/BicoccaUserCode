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
// $Id: SimpleNtple.cc,v 1.22 2010/04/29 16:08:25 amassiro Exp $
//
//


// system include files
#include <memory>

// user include files
#include "HiggsAnalysis/VBFHiggsToVV/plugins/SimpleNtple.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/RefToBase.h"
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
 //---- Out file ----
 mcAnalysisVBF_ = NULL;
 mcAnalysisTTBar_ = NULL;
 edm::Service<TFileService> fs ;
 outTree_  = fs->make <TTree>("SimpleNtple","SimpleNtple"); 
 NtupleFactory_ = new NtupleFactory(outTree_);  

 //---- Input tags ---- 

 dataTag_   = iConfig.getUntrackedParameter<bool> ("dataTag_", false);

 HLTTag_    = iConfig.getParameter<edm::InputTag>("HLTTag");
 PVTag_     = iConfig.getParameter<edm::InputTag>("PVTag");
 TracksTag_ = iConfig.getParameter<edm::InputTag>("TracksTag");

 EleTag_ = iConfig.getParameter<edm::InputTag>("EleTag");
 Ele3DipSignificanceTag_ = iConfig.getParameter<edm::InputTag>("Ele3DipSignificanceTag");
 EleTipSignificanceTag_  = iConfig.getParameter<edm::InputTag>("EleTipSignificanceTag");
 EleLipSignificanceTag_  = iConfig.getParameter<edm::InputTag>("EleLipSignificanceTag");
 recHitCollection_EB_ = iConfig.getParameter<edm::InputTag>("recHitCollection_EB");
 recHitCollection_EE_ = iConfig.getParameter<edm::InputTag>("recHitCollection_EE");

 MuTag_  = iConfig.getParameter<edm::InputTag>("MuTag");
 Mu3DipSignificanceTag_ = iConfig.getParameter<edm::InputTag>("Mu3DipSignificanceTag");
 MuTipSignificanceTag_  = iConfig.getParameter<edm::InputTag>("MuTipSignificanceTag");
 MuLipSignificanceTag_  = iConfig.getParameter<edm::InputTag>("MuLipSignificanceTag");

 MetTag_ = iConfig.getParameter<edm::InputTag>("MetTag");
 Type1MetTag_ = iConfig.getParameter<edm::InputTag>("Type1MetTag");
 PFMetTag_    = iConfig.getParameter<edm::InputTag>("PFMetTag");
 TcMetTag_    = iConfig.getParameter<edm::InputTag>("TcMetTag");

 JetTag_ = iConfig.getParameter<edm::InputTag>("JetTag");
 JetTag_forID_ = iConfig.getParameter<edm::InputTag>("JetTag_forID");
 JetIDTag_     = iConfig.getParameter<edm::InputTag> ("JetIDTag");

 TrackCountingHighEffBJetTagsTag_ = iConfig.getParameter<edm::InputTag>("TrackCountingHighEffBJetTagsTag");
 TrackCountingHighPurBJetTagsTag_ = iConfig.getParameter<edm::InputTag>("TrackCountingHighPurBJetTagsTag");
 JetProbabilityBJetTagsTag_       = iConfig.getParameter<edm::InputTag>("JetProbabilityBJetTagsTag");
 JetBProbabilityBJetTagsTag_      = iConfig.getParameter<edm::InputTag>("JetBProbabilityBJetTagsTag");

 JetChargeTag_ = iConfig.getParameter<edm::InputTag> ("JetChargeTag");

 eleIDCut_LooseInputTag_ = iConfig.getParameter<edm::InputTag> ("eleIDCut_LooseInputTag");
 eleIDCut_RLooseInputTag_ = iConfig.getParameter<edm::InputTag> ("eleIDCut_RLooseInputTag");
 eleIDCut_TightInputTag_ = iConfig.getParameter<edm::InputTag> ("eleIDCut_TightInputTag");
 eleIDCut_RTightInputTag_ = iConfig.getParameter<edm::InputTag> ("eleIDCut_RTightInputTag");


 MCtruthTag_ = iConfig.getParameter<edm::InputTag>("MCtruthTag");
 genJetTag_ = iConfig.getParameter<edm::InputTag>("genJetTag");
 genMetTag_ = iConfig.getParameter<edm::InputTag>("genMetTag");

 //---- flags ----
 saveHLT_ =iConfig.getUntrackedParameter<bool> ("saveHLT", true);
 savePV_ =iConfig.getUntrackedParameter<bool> ("savePV", true);
 saveTrack_ = iConfig.getUntrackedParameter<bool> ("saveTrack", true);
 saveEle_ = iConfig.getUntrackedParameter<bool> ("saveEle", true);
 saveMu_ =iConfig.getUntrackedParameter<bool> ("saveMu", true);

 saveMet_ = iConfig.getUntrackedParameter<bool> ("saveMet", true);
  
 saveJet_ = iConfig.getUntrackedParameter<bool> ("saveJet", true);
 savePFJet_ = iConfig.getUntrackedParameter<bool> ("savePFJet", false);
 if(saveJet_ && savePFJet_) savePFJet_ = false;
 saveJetBTagging_ = iConfig.getUntrackedParameter<bool> ("saveJetBTagging", true);
  
 saveTTBar_ = iConfig.getUntrackedParameter<bool> ("saveTTBar", false);
 saveMCPtHat_ = iConfig.getUntrackedParameter<bool> ("saveMCPtHat", false);
 saveMCHiggs_ = iConfig.getUntrackedParameter<bool> ("saveMCHiggs", false);
 saveMCHiggsDecay_ = iConfig.getUntrackedParameter<bool> ("saveMCHiggsDecay", false);
 saveMCEle_ = iConfig.getUntrackedParameter<bool> ("saveMCEle", false);
 saveMCMu_ = iConfig.getUntrackedParameter<bool> ("saveMCMu", false);

 saveGenJet_ = iConfig.getUntrackedParameter<bool> ("saveGenJet", false);
 saveGenMet_ = iConfig.getUntrackedParameter<bool> ("saveGenMet", false);
  
 verbosity_ = iConfig.getUntrackedParameter<bool>("verbosity",false);
 eventType_ = iConfig.getUntrackedParameter<int>("eventType",1);
 



 //---- Add branches to ntuple ----  
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
    NtupleFactory_ -> AddString("HLT_Names"); 
 }
 
 if(savePV_)
 {
   NtupleFactory_ -> AddFloat("PV_normalizedChi2"); 
   NtupleFactory_ -> AddInt  ("PV_ndof"); 
   NtupleFactory_ -> AddInt  ("PV_nTracks"); 
   NtupleFactory_ -> AddFloat("PV_z"); 
   NtupleFactory_ -> AddFloat("PV_d0"); 
 }
 
 if(saveTrack_)
 {
   NtupleFactory_ -> Add3V("tracks_in");
   NtupleFactory_ -> Add3V("tracks_out");   
 }

 if(saveEle_)
  {
    NtupleFactory_->Add4V("electrons");
    NtupleFactory_->AddFloat("electrons_charge"); 
    NtupleFactory_ -> AddFloat("electrons_dxy");
    NtupleFactory_ -> AddFloat("electrons_dz");
    NtupleFactory_ -> AddFloat("electrons_3DipSignificance");    
    NtupleFactory_ -> AddFloat("electrons_tipSignificance");    
    NtupleFactory_ -> AddFloat("electrons_lipSignificance");

    NtupleFactory_ -> AddFloat("electrons_tkIsoR03"); 
    NtupleFactory_ -> AddFloat("electrons_tkIsoR04"); 
    NtupleFactory_ -> AddFloat("electrons_emIsoR03"); 
    NtupleFactory_ -> AddFloat("electrons_emIsoR04"); 
    NtupleFactory_ -> AddFloat("electrons_hadIsoR03_depth1"); 
    NtupleFactory_ -> AddFloat("electrons_hadIsoR03_depth2"); 
    NtupleFactory_ -> AddFloat("electrons_hadIsoR04_depth1"); 
    NtupleFactory_ -> AddFloat("electrons_hadIsoR04_depth2"); 

    NtupleFactory_ -> AddInt  ("electrons_isEB");
    NtupleFactory_ -> AddInt  ("electrons_ecalDrivenSeed");
    NtupleFactory_ -> AddInt  ("electrons_trackerDrivenSeed");
    NtupleFactory_ -> AddFloat("electrons_scE");
    NtupleFactory_ -> AddFloat("electrons_eSeed");
    NtupleFactory_ -> AddFloat("electrons_pin");
    NtupleFactory_ -> AddFloat("electrons_pout");
    NtupleFactory_ -> AddFloat("electrons_eOverP");
    NtupleFactory_ -> AddInt  ("electrons_classification");
    NtupleFactory_ -> AddFloat("electrons_fbrem");
    NtupleFactory_ -> AddFloat("electrons_hOverE");
    NtupleFactory_ -> AddFloat("electrons_deltaPhiIn");
    NtupleFactory_ -> AddFloat("electrons_deltaEtaIn");
    NtupleFactory_ -> AddFloat("electrons_sigmaIetaIeta");

    NtupleFactory_->AddFloat("electrons_IdLoose"); 
    NtupleFactory_->AddFloat("electrons_IdTight"); 
    NtupleFactory_->AddFloat("electrons_IdRobustLoose"); 
    NtupleFactory_->AddFloat("electrons_IdRobustTight"); 

    NtupleFactory_->AddFloat("electrons_track_d0");
    NtupleFactory_->AddFloat("electrons_track_dz");
    NtupleFactory_->AddFloat("electrons_track_d0err");
    NtupleFactory_->AddFloat("electrons_track_dzerr");

    NtupleFactory_->AddInt("electrons_mishits");
    NtupleFactory_->AddInt("electrons_nAmbiguousGsfTracks");
    NtupleFactory_->AddFloat("electrons_dist");
    NtupleFactory_->AddFloat("electrons_dcot");
    
    NtupleFactory_->AddInt("electrons_seedSeverityLevel");
    NtupleFactory_->AddInt("electrons_seedFlag");
    
    NtupleFactory_->AddFloat("electrons_ES");

   }

 if(saveMu_)
  {
    NtupleFactory_ -> Add4V("muons");
    NtupleFactory_ -> AddFloat("muons_charge"); 
    NtupleFactory_ -> AddFloat("muons_dxy");
    NtupleFactory_ -> AddFloat("muons_dz");
    NtupleFactory_ -> AddFloat("muons_3DipSignificance");    
    NtupleFactory_ -> AddFloat("muons_tipSignificance");    
    NtupleFactory_ -> AddFloat("muons_lipSignificance");    
    
    NtupleFactory_ -> AddFloat("muons_nTkIsoR03"); 
    NtupleFactory_ -> AddFloat("muons_nTkIsoR05"); 
    NtupleFactory_ -> AddFloat("muons_tkIsoR03"); 
    NtupleFactory_ -> AddFloat("muons_tkIsoR05"); 
    NtupleFactory_ -> AddFloat("muons_emIsoR03"); 
    NtupleFactory_ -> AddFloat("muons_emIsoR05"); 
    NtupleFactory_ -> AddFloat("muons_hadIsoR03"); 
    NtupleFactory_ -> AddFloat("muons_hadIsoR05"); 
    
    NtupleFactory_ -> AddInt  ("muons_tracker");
    NtupleFactory_ -> AddInt  ("muons_standalone");
    NtupleFactory_ -> AddInt  ("muons_global");
    NtupleFactory_ -> AddFloat("muons_normalizedChi2");
    NtupleFactory_ -> AddInt  ("muons_numberOfValidTrackerHits");
    NtupleFactory_ -> AddInt  ("muons_numberOfValidMuonHits");
  }

 if(saveMet_)
  {
   NtupleFactory_->Add4V("met");         
   NtupleFactory_->Add4V("type1Met");         
   NtupleFactory_->Add4V("TcMet");         
   NtupleFactory_->Add4V("PFMet");         
  }


 if(saveJet_ || savePFJet_)
  {
    NtupleFactory_->Add4V("jets");
    
    NtupleFactory_->AddFloat("jets_trackCountingHighEffBJetTags");   
    NtupleFactory_->AddFloat("jets_trackCountingHighPurBJetTags");   
    NtupleFactory_->AddFloat("jets_jetProbabilityBJetTags");   
    NtupleFactory_->AddFloat("jets_jetBProbabilityBJetTags");   
      
    NtupleFactory_->AddFloat("jets_charge");   

    NtupleFactory_->AddFloat("jets_etaetaMoment");
    NtupleFactory_->AddFloat("jets_phiphiMoment");   
    NtupleFactory_->AddFloat("jets_etaphiMoment");   
    NtupleFactory_->AddFloat("jets_jetArea");   
    
    
    if(saveJet_)
    {
      NtupleFactory_->AddFloat("jets_emEnergyFraction");   
      NtupleFactory_->AddFloat("jets_fHPD");   
      NtupleFactory_->AddFloat("jets_fRBX");   
      NtupleFactory_->AddFloat("jets_n90Hits");   
      NtupleFactory_->AddFloat("jets_nHCALTowers");   
      NtupleFactory_->AddFloat("jets_nECALTowers");   
    }
    
    
    if(savePFJet_)
    {
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
  

  ///==== Gen level ====  
  if(saveGenJet_)
  {
    NtupleFactory_->Add4V("genJets");         
  }    
   
  if(saveGenMet_)
  {
   NtupleFactory_->Add4V("genMet");                
  }
  
  if(saveTTBar_)
  {
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


  if(saveMCPtHat_)
  {
    NtupleFactory_->AddFloat("mc_ptHat");    
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




 
///---- HLT ----
void SimpleNtple::fillHLTInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //---- HLT information ----
  edm::Handle<edm::TriggerResults> triggerResultsHandle;
  iEvent.getByLabel(HLTTag_, triggerResultsHandle);
  const edm::TriggerNames& triggerNames = iEvent.triggerNames(*triggerResultsHandle);
  for(unsigned int iHLT = 0; iHLT < triggerResultsHandle->size(); ++iHLT)
  {
    //std::cout << "bit: " << std::fixed << setw(3)<< iHLT << "   name: " << triggerNames.triggerName(iHLT) << std::endl;
    if( triggerResultsHandle -> wasrun(iHLT) )
      NtupleFactory_ -> FillFloat("HLT_WasRun", 1);
    else
      NtupleFactory_ -> FillFloat("HLT_WasRun", 0);
    
    if( triggerResultsHandle -> accept(iHLT) )
      NtupleFactory_ -> FillFloat("HLT_Accept", 1);
    else
      NtupleFactory_ -> FillFloat("HLT_Accept", 0);
    
    if( triggerResultsHandle -> error(iHLT) )
      NtupleFactory_->FillFloat("HLT_Error", 1);
    else
      NtupleFactory_->FillFloat("HLT_Error", 0);
    
    NtupleFactory_ -> FillString("HLT_Names", triggerNames.triggerName(iHLT));
  }
}



///---- muons ----
void SimpleNtple::fillPFMuInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{

}


void SimpleNtple::fillMuInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //std::cout << "SimpleNtple::fillMuInfo" << std::endl;
 
 edm::Handle<reco::MuonCollection> MuHandle ;
 iEvent.getByLabel (MuTag_,MuHandle);
  
 edm::Handle<muMap> Mu3DipSignificanceHandle ;
 iEvent.getByLabel (Mu3DipSignificanceTag_,Mu3DipSignificanceHandle);
 edm::Handle<muMap> MuTipSignificanceHandle ;
 iEvent.getByLabel (MuTipSignificanceTag_,MuTipSignificanceHandle);
 edm::Handle<muMap> MuLipSignificanceHandle ;
 iEvent.getByLabel (MuLipSignificanceTag_,MuLipSignificanceHandle);
 
 
 for(unsigned int i=0; i<MuHandle->size(); i++){
  reco::MuonRef muRef(MuHandle, i);  
  
  NtupleFactory_->Fill4V("muons",muRef->p4());
  NtupleFactory_->FillInt("muons_goodMuon",muon::isGoodMuon((*MuHandle)[i], muon::GlobalMuonPromptTight));

  NtupleFactory_->FillFloat("muons_charge",(muRef->charge()));

  NtupleFactory_ -> FillFloat("muons_dxy",(muRef->globalTrack())->dxy(PVPoint_));
  NtupleFactory_ -> FillFloat("muons_dz",(muRef->globalTrack())->dz(PVPoint_));
 
  NtupleFactory_->FillFloat("muons_tkIsoR03",(muRef->isolationR03()).sumPt);
  NtupleFactory_->FillFloat("muons_nTkIsoR03",(muRef->isolationR03()).nTracks);    
  NtupleFactory_->FillFloat("muons_emIsoR03",(muRef->isolationR03()).emEt);
  NtupleFactory_->FillFloat("muons_hadIsoR03",(muRef->isolationR03()).hadEt);
    
  NtupleFactory_->FillFloat("muons_tkIsoR05",(muRef->isolationR05()).sumPt);
  NtupleFactory_->FillFloat("muons_nTkIsoR05",(muRef->isolationR05()).nTracks);    
  NtupleFactory_->FillFloat("muons_emIsoR05",(muRef->isolationR05()).emEt);
  NtupleFactory_->FillFloat("muons_hadIsoR05",(muRef->isolationR05()).hadEt);

  NtupleFactory_->FillFloat("muons_3DipSignificance",((*Mu3DipSignificanceHandle)[muRef]));
  NtupleFactory_->FillFloat("muons_tipSignificance",((*MuTipSignificanceHandle)[muRef]));
  NtupleFactory_->FillFloat("muons_lipSignificance",((*MuLipSignificanceHandle)[muRef]));

  
  NtupleFactory_ -> FillInt  ("muons_tracker",muRef->isTrackerMuon());
  NtupleFactory_ -> FillInt  ("muons_standalone",muRef->isStandAloneMuon());
  NtupleFactory_ -> FillInt  ("muons_global",muRef->isGlobalMuon());
  NtupleFactory_ -> FillFloat("muons_normalizedChi2",(muRef->globalTrack())->normalizedChi2());
  NtupleFactory_ -> FillInt  ("muons_numberOfValidTrackerHits",(muRef->globalTrack()->hitPattern()).numberOfValidTrackerHits());
  NtupleFactory_ -> FillInt  ("muons_numberOfValidMuonHits",(muRef->globalTrack()->hitPattern()).numberOfValidMuonHits());

 }
}






///---- electrons ----

void SimpleNtple::fillPFEleInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
}

void SimpleNtple::fillEleInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //std::cout << "SimpleNtple::fillEleInfo" << std::endl;
 
 edm::Handle<reco::GsfElectronCollection> EleHandle ;
 iEvent.getByLabel (EleTag_,EleHandle);
 
 //*********** CHANNEL STATUS
 edm::ESHandle<EcalChannelStatus> theChannelStatus;
 iESetup.get<EcalChannelStatusRcd>().get(theChannelStatus);

 //*********** EB REC HITS
 edm::Handle<EcalRecHitCollection> recHitsEB;
 iEvent.getByLabel( recHitCollection_EB_, recHitsEB );
 const EcalRecHitCollection* theBarrelEcalRecHits = recHitsEB.product () ;
 if ( ! recHitsEB.isValid() ) {
  std::cerr << "SimpleNtple::analyze --> recHitsEB not found" << std::endl; 
 }
  
 //*********** EE REC HITS
 edm::Handle<EcalRecHitCollection> recHitsEE;
 iEvent.getByLabel( recHitCollection_EE_, recHitsEE );
 const EcalRecHitCollection* theEndcapEcalRecHits = recHitsEE.product () ;
 if ( ! recHitsEE.isValid() ) {
  std::cerr << "SimpleNtple::analyze --> recHitsEE not found" << std::endl; 
 }

 //************* TRACKS
 edm::Handle<reco::TrackCollection> trackHandle;
 iEvent.getByLabel(TracksTag_, trackHandle);
 
 //************* BFIELD
 edm::Handle<DcsStatusCollection> dcsHandle;
 iEvent.getByLabel(DCSTag_, dcsHandle);
 double bField;
 
 if (dataTag_)
 {
   // scale factor = 3.801/18166.0 which are
   // average values taken over a stable two
   // week period
   float currentToBFieldScaleFactor = 2.09237036221512717e-04;
   float current = (*dcsHandle)[0].magnetCurrent();
   bField = current*currentToBFieldScaleFactor;
 }
 else
 {
   edm::ESHandle<MagneticField> magneticField;
   iESetup.get<IdealMagneticFieldRecord>().get(magneticField);
   bField = magneticField->inTesla(GlobalPoint(0.,0.,0.)).z();
 }



 

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

  reco::SuperClusterRef scRef = eleRef->superCluster();
  const edm::Ptr<reco::CaloCluster>& seedCluster = scRef->seed();

  NtupleFactory_ -> Fill4V   ("electrons",eleRef->p4());
  NtupleFactory_ -> FillFloat("electrons_charge",eleRef->charge());
  NtupleFactory_ -> FillFloat("electrons_dxy",(eleRef->gsfTrack())->dxy(PVPoint_));
  NtupleFactory_ -> FillFloat("electrons_dz",(eleRef->gsfTrack())->dz(PVPoint_));
 
  NtupleFactory_ -> FillFloat("electrons_tkIsoR03",eleRef->dr03TkSumPt());
  NtupleFactory_ -> FillFloat("electrons_tkIsoR04",eleRef->dr04TkSumPt());
  NtupleFactory_ -> FillFloat("electrons_emIsoR03",eleRef->dr03EcalRecHitSumEt());
  NtupleFactory_ -> FillFloat("electrons_emIsoR04",eleRef->dr04EcalRecHitSumEt());
  NtupleFactory_ -> FillFloat("electrons_hadIsoR03_depth1",eleRef->dr03HcalDepth1TowerSumEt());
  NtupleFactory_ -> FillFloat("electrons_hadIsoR03_depth2",eleRef->dr03HcalDepth2TowerSumEt());
  NtupleFactory_ -> FillFloat("electrons_hadIsoR04_depth1",eleRef->dr04HcalDepth1TowerSumEt());
  NtupleFactory_ -> FillFloat("electrons_hadIsoR04_depth2",eleRef->dr04HcalDepth2TowerSumEt());

  if(eleRef->isEB()) NtupleFactory_ -> FillInt("electrons_isEB", 1);
  else               NtupleFactory_ -> FillInt("electrons_isEB", 0);
  if(eleRef->ecalDrivenSeed()) NtupleFactory_ -> FillInt("electrons_ecalDrivenSeed", 1);
  else                         NtupleFactory_ -> FillInt("electrons_ecalDrivenSeed", 0);
  if(eleRef->trackerDrivenSeed()) NtupleFactory_ -> FillInt("electrons_trackerDrivenSeed", 1);
  else                            NtupleFactory_ -> FillInt("electrons_trackerDrivenSeed", 0);
  
  //ELE ID
  NtupleFactory_->FillFloat("electrons_IdLoose",(*(eleIdCutHandles[0]))[eleRef]) ;
  NtupleFactory_->FillFloat("electrons_IdRobustLoose",(*(eleIdCutHandles[1]))[eleRef]);
  NtupleFactory_->FillFloat("electrons_IdTight",(*(eleIdCutHandles[2]))[eleRef]);
  NtupleFactory_->FillFloat("electrons_IdRobustTight",(*(eleIdCutHandles[3]))[eleRef]);

  NtupleFactory_ -> FillFloat("electrons_scE",(eleRef->superCluster())->energy());
  NtupleFactory_ -> FillFloat("electrons_eSeed",eleRef->superCluster()->seed()->energy());
  NtupleFactory_ -> FillFloat("electrons_pin",eleRef->trackMomentumAtVtx().R());
  NtupleFactory_ -> FillFloat("electrons_pout",eleRef->trackMomentumOut().R());
  NtupleFactory_ -> FillFloat("electrons_eOverP",eleRef->eSuperClusterOverP());
  NtupleFactory_ -> FillInt  ("electrons_classification",eleRef->classification());
  NtupleFactory_ -> FillFloat("electrons_fbrem",eleRef->fbrem());
  NtupleFactory_ -> FillFloat("electrons_hOverE",eleRef->hadronicOverEm());
  NtupleFactory_ -> FillFloat("electrons_deltaPhiIn",eleRef->deltaPhiSuperClusterTrackAtVtx());
  NtupleFactory_ -> FillFloat("electrons_deltaEtaIn",eleRef->deltaEtaSuperClusterTrackAtVtx());
  NtupleFactory_ -> FillFloat("electrons_sigmaIetaIeta",eleRef->sigmaIetaIeta());

  NtupleFactory_->FillInt("electrons_mishits",eleRef->gsfTrack()->trackerExpectedHitsInner().numberOfHits());


  // conversion rejection variables
  ConversionFinder convFinder;
  ConversionInfo convInfo = convFinder.getConversionInfo((*EleHandle)[i], trackHandle, bField);
   
  NtupleFactory_->FillInt("electrons_nAmbiguousGsfTracks",eleRef->ambiguousGsfTracksSize());
  NtupleFactory_->FillFloat("electrons_dist", convInfo.dist());
  NtupleFactory_->FillFloat("electrons_dcot", convInfo.dcot());
   
   
   // spike removal variables
   int sev = -1;
   int flag = -1;
   
   if(eleRef->isEB())
   {
     std::pair<DetId, float> id = EcalClusterTools::getMaximum(seedCluster->hitsAndFractions(), theBarrelEcalRecHits);
     // severity level - SwissCross 
     sev = EcalSeverityLevelAlgo::severityLevel(id.first, *theBarrelEcalRecHits, *(theChannelStatus.product()));
     // flag - OutOfTime
     EcalRecHitCollection::const_iterator it = theBarrelEcalRecHits->find(id.first);
     if( it != theBarrelEcalRecHits->end() )
     {
       const EcalRecHit& rh = (*it);
       flag = rh.recoFlag();
     }
   }
 
   else
   {
     std::pair<DetId, float> id = EcalClusterTools::getMaximum(seedCluster->hitsAndFractions(), theEndcapEcalRecHits);
     // severity level - SwissCross 
     sev = EcalSeverityLevelAlgo::severityLevel(id.first, *theEndcapEcalRecHits, *(theChannelStatus.product()));
     // flag - OutOfTime
     EcalRecHitCollection::const_iterator it = theEndcapEcalRecHits->find(id.first);
     if( it != theEndcapEcalRecHits->end() )
     {
       const EcalRecHit& rh = (*it);
       flag = rh.recoFlag();
     }
   }

  NtupleFactory_->FillInt("electrons_seedSeverityLevel", sev);
  NtupleFactory_->FillInt("electrons_seedFlag", flag);
   
  // preshower variables 
  NtupleFactory_->FillFloat("electrons_ES",scRef->preshowerEnergy());

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



///---- primary vertex ----
void SimpleNtple::fillPVInfo(const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
  //std::cout << "SimpleNtple::fillPVInfo::begin" << std::endl;
  
  edm::Handle<reco::VertexCollection> vertexes;
  iEvent.getByLabel(PVTag_, vertexes);
  
  
  // select the primary vertex    
  reco::Vertex PV;
  bool PVfound = (vertexes -> size() != 0);
  
  if(PVfound)
  {
    // select the primary vertex as the one with higest sum of (pt)^2 of tracks    
    PrimaryVertexSorter PVSorter;
    std::vector<reco::Vertex> sortedVertices = PVSorter.sortedList( *(vertexes.product()) );
    PV = sortedVertices.front();
    
    NtupleFactory_->FillFloat("PV_normalizedChi2", PV.normalizedChi2());
    NtupleFactory_->FillInt("PV_ndof", PV.ndof());
    NtupleFactory_->FillInt("PV_nTracks", PV.tracksSize());
    NtupleFactory_->FillFloat("PV_z", PV.z());
    NtupleFactory_->FillFloat("PV_d0", PV.position().Rho());
  }
  
  else
  {
    //creating a dummy PV
    reco::Vertex::Point p(0,0,0);
    reco::Vertex::Error e;
    e(0,0) = 0.0015*0.0015;
    e(1,1) = 0.0015*0.0015;
    e(2,2) = 15.*15.;
    PV = reco::Vertex(p, e, 1, 1, 1);
  }
  
  math::XYZPoint PVPoint(PV.position().x(), PV.position().y(), PV.position().z());
  PVPoint_ = PVPoint;
  
  //std::cout << "SimpleNtple::fillPVInfo::end" << std::endl;
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
  //std::cout << "SimpleNtple::fillJetInfo::begin" << std::endl;
  
  edm::Handle<reco::CaloJetCollection> JetHandle ;
  iEvent.getByLabel(JetTag_,JetHandle);
  
  edm::Handle<edm::View<reco::Jet> > JetHandle_forID;  
  iEvent.getByLabel(JetTag_forID_,JetHandle_forID);
  
  edm::Handle<reco::JetIDValueMap> jetIDHandle;
  iEvent.getByLabel(JetIDTag_, jetIDHandle) ;
  
  edm::Handle<reco::JetFloatAssociation::Container> jetChargeHandle;
  iEvent.getByLabel(JetChargeTag_, jetChargeHandle) ;
  
  
  
  // loop on jets
  for(unsigned int i=0; i<JetHandle->size(); ++i) 
  { 
    reco::CaloJetRef jetRef(JetHandle, i);
    
    NtupleFactory_ -> Fill4V   ("jets",jetRef->p4());
    NtupleFactory_ -> FillFloat("jets_emEnergyFraction",jetRef->emEnergyFraction());
    NtupleFactory_ -> FillFloat("jets_etaetaMoment",jetRef->etaetaMoment());
    NtupleFactory_ -> FillFloat("jets_phiphiMoment",jetRef->phiphiMoment());
    NtupleFactory_ -> FillFloat("jets_etaphiMoment",jetRef->etaphiMoment());
    NtupleFactory_ -> FillFloat("jets_jetArea",jetRef->jetArea());
    
    
    //==== jet ID
    //==== jet ID calculated only on uncorrected jets ====
    //==== DR matching to get JetID variables ====
    float DRMin = 0.01;
   
    edm::View<reco::Jet>::const_iterator itSelJet = JetHandle_forID->end();
    for (edm::View<reco::Jet>::const_iterator itJet = JetHandle_forID->begin(); itJet < JetHandle_forID->end(); itJet++){
      float DRTemp = ROOT::Math::VectorUtil::DeltaR((*JetHandle)[i].p4(), itJet->p4());
      if(DRTemp < DRMin){
       itSelJet = itJet;
       DRMin = DRTemp;
      }
     }

   if (itSelJet != JetHandle_forID->end()){
    unsigned int idx = itSelJet - JetHandle_forID->begin();
    edm::RefToBase<reco::Jet> jetRef_temp = JetHandle_forID->refAt(idx);
    NtupleFactory_->FillFloat("jets_fHPD",((*jetIDHandle)[jetRef_temp]).fHPD);
    NtupleFactory_->FillFloat("jets_fRBX",((*jetIDHandle)[jetRef_temp]).fRBX);
    NtupleFactory_->FillFloat("jets_n90Hits",((*jetIDHandle)[jetRef_temp]).n90Hits);
    NtupleFactory_->FillFloat("jets_nHCALTowers",((*jetIDHandle)[jetRef_temp]).nHCALTowers);
    NtupleFactory_->FillFloat("jets_nECALTowers",((*jetIDHandle)[jetRef_temp]).nECALTowers);
   }
   else {
    NtupleFactory_->FillFloat("jets_fHPD",-999);
    NtupleFactory_->FillFloat("jets_fRBX",-999);
    NtupleFactory_->FillFloat("jets_n90Hits",-999);
    NtupleFactory_->FillFloat("jets_nHCALTowers",-999);
    NtupleFactory_->FillFloat("jets_nECALTowers",-999);
   }

    
    
    
    //==== jet charge

    DRMin = 0.01;
    int jMin = -1;
    
    for(unsigned int j=0; j<jetChargeHandle->size(); ++j)
    {
      reco::Jet jet_forCharge = *(jetChargeHandle->key(j));
      float DRTemp = ROOT::Math::VectorUtil::DeltaR(jetRef->p4(), jet_forCharge.p4());
      if(DRTemp < DRMin)
      {
        DRMin = DRTemp;
        jMin = j;
      }
    }
    
    if(jMin != -1)
    {
      NtupleFactory_ -> FillFloat("jets_charge",(jetChargeHandle->value(jMin)));
    }
    else {
      NtupleFactory_ -> FillFloat("jets_charge",-999);
    }
    
    
    
    //==== jet b tagging
    if(saveJetBTagging_)
      fillJetBTaggingInfo(iEvent, iESetup, jetRef->p4());
    
  } // loop on jets
  
  //std::cout << "SimpleNtple::fillJetInfo::end" << std::endl;
 
}






///---- PFJets ----
void SimpleNtple::fillPFJetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
//std::cout << "SimpleNtple::fillPFJetInfo::begin" << std::endl;
  
  edm::Handle<reco::PFJetCollection> JetHandle ;
  iEvent.getByLabel (JetTag_,JetHandle);
  
  edm::Handle<reco::JetFloatAssociation::Container> jetChargeHandle;
  iEvent.getByLabel(JetChargeTag_, jetChargeHandle) ;  
  
  
  
  // loop on jets
  for(unsigned int i=0; i<JetHandle->size(); ++i) 
  { 
    reco::PFJetRef jetRef(JetHandle, i);
    
    NtupleFactory_ -> Fill4V   ("jets",jetRef->p4());

    NtupleFactory_ -> FillFloat("jets_etaetaMoment",jetRef->etaetaMoment());
    NtupleFactory_ -> FillFloat("jets_phiphiMoment",jetRef->phiphiMoment());
    NtupleFactory_ -> FillFloat("jets_etaphiMoment",jetRef->etaphiMoment());
    NtupleFactory_ -> FillFloat("jets_jetArea",jetRef->jetArea());
    
    NtupleFactory_ -> FillFloat("jets_chargedHadronEnergy",jetRef->chargedHadronEnergy()); 
    NtupleFactory_ -> FillFloat("jets_chargedHadronEnergyFraction",jetRef->chargedHadronEnergyFraction()); 
    NtupleFactory_ -> FillFloat("jets_neutralHadronEnergy",jetRef->neutralHadronEnergy()); 
    NtupleFactory_ -> FillFloat("jets_neutralHadronEnergyFraction",jetRef->neutralHadronEnergyFraction()); 
    NtupleFactory_ -> FillFloat("jets_chargedEmEnergy",jetRef->chargedEmEnergy()); 
    NtupleFactory_ -> FillFloat("jets_chargedEmEnergyFraction",jetRef->chargedEmEnergyFraction()); 
    NtupleFactory_ -> FillFloat("jets_chargedMuEnergy",jetRef->chargedMuEnergy()); 
    NtupleFactory_ -> FillFloat("jets_chargedMuEnergyFraction",jetRef->chargedMuEnergyFraction()); 
    NtupleFactory_ -> FillFloat("jets_neutralEmEnergy",jetRef->neutralEmEnergy()); 
    NtupleFactory_ -> FillFloat("jets_neutralEmEnergyFraction",jetRef->neutralEmEnergyFraction()); 
    NtupleFactory_ -> FillInt  ("jets_chargedMultiplicity",jetRef->chargedMultiplicity()); 
    NtupleFactory_ -> FillInt  ("jets_neutralMultiplicity",jetRef->neutralMultiplicity()); 
    NtupleFactory_ -> FillInt  ("jets_muonMultiplicity",jetRef->muonMultiplicity()); 
    
    
    
    // jet charge
    float DRMin = 0.01;
    float jMin = -1;
    
    for(unsigned int j=0; j<jetChargeHandle->size(); ++j)
    {
      reco::Jet jet_forCharge = *(jetChargeHandle->key(j));
      
      float DRTemp = ROOT::Math::VectorUtil::DeltaR(jetRef->p4(), jet_forCharge.p4());
      if(DRTemp < DRMin)
      {
        DRMin = DRTemp;
        jMin = j;
      }
    }
    
    if(jMin != -1)
    {
      NtupleFactory_ -> FillFloat("jets_charge",(jetChargeHandle->value(jMin)));
    }
    
    // jet b-tagging
    if(saveJetBTagging_)
      fillJetBTaggingInfo(iEvent, iESetup, jetRef->p4());
    
  } // loop on jets
  
  //std::cout << "SimpleNtple::fillPFJetInfo::end" << std::endl;

}






///---- JetBTagging ----
void SimpleNtple::fillJetBTaggingInfo(const edm::Event & iEvent, const edm::EventSetup & iESetup,
                                      const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >& v1)
{
//std::cout << "SimpleNtple::fillJetBTaggingInfo::begin" << std::endl;
  
  edm::Handle<reco::JetTagCollection> bTagHandle_trackCountingHighEff;
  iEvent.getByLabel(TrackCountingHighEffBJetTagsTag_, bTagHandle_trackCountingHighEff);  
  
  edm::Handle<reco::JetTagCollection> bTagHandle_trackCountingHighPur;
  iEvent.getByLabel(TrackCountingHighPurBJetTagsTag_, bTagHandle_trackCountingHighPur);  
  
  edm::Handle<reco::JetTagCollection> bTagHandle_jetProbability;
  iEvent.getByLabel(JetProbabilityBJetTagsTag_, bTagHandle_jetProbability);
  
  edm::Handle<reco::JetTagCollection> bTagHandle_jetBProbability;
  iEvent.getByLabel(JetBProbabilityBJetTagsTag_, bTagHandle_jetBProbability);
  
  // trackCountingHighEff
  float DRMin = 0.01;
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
    
  if(jMin != -1) NtupleFactory_->FillFloat("jets_trackCountingHighEffBJetTags", (*bTagHandle_trackCountingHighEff)[jMin].second);
  else           NtupleFactory_->FillFloat("jets_trackCountingHighEffBJetTags", -999999.);
  
  
  
  // trackCountingHighPur
  DRMin = 0.01;
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
  
  if(jMin != -1) NtupleFactory_->FillFloat("jets_trackCountingHighPurBJetTags", (*bTagHandle_trackCountingHighPur)[jMin].second);
  else           NtupleFactory_->FillFloat("jets_trackCountingHighPurBJetTags", -999999.);
  
  
  
  // jetProbability
  DRMin = 0.01;
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
  
  if(jMin != -1) NtupleFactory_->FillFloat("jets_jetProbabilityBJetTags", (*bTagHandle_jetProbability)[jMin].second);
  else           NtupleFactory_->FillFloat("jets_jetProbabilityBJetTags", -999999.);
  
  
  
  // jetBProbability
  DRMin = 0.01;
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
  
  if(jMin != -1) NtupleFactory_->FillFloat("jets_jetBProbabilityBJetTags", (*bTagHandle_jetBProbability)[jMin].second);
  else           NtupleFactory_->FillFloat("jets_jetBProbabilityBJetTags", -999999.);
  
  
  //std::cout << "SimpleNtple::fillJetBTaggingInfo::end" << std::endl;

    
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

void SimpleNtple::fillMCPtHatInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
  //std::cout << "SimpleNtple::fillPtHatInfo::begin" << std::endl; 
  
  edm::Handle<edm::HepMCProduct> HepMCEvt;
  iEvent.getByLabel("generator", HepMCEvt);
  float ptHat = HepMCEvt -> GetEvent() -> event_scale();
  
  NtupleFactory_->FillFloat("mc_ptHat", ptHat);
  
  //std::cout << "SimpleNtple::fillPtHatInfo::end" << std::endl; 
}



void SimpleNtple::fillMCHiggsInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //std::cout << "SimpleNtple::fillMCHiggsInfo" << std::endl; 
 
 NtupleFactory_->Fill4V("mc_H",mcAnalysisVBF_ -> mcH()->p4());
 NtupleFactory_->FillFloat("mc_H_charge",mcAnalysisVBF_ -> mcH()->charge());
 
}



void SimpleNtple::fillMCHiggsDecayInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //std::cout << "SimpleNtple::fillMCHiggsDecayInfo" << std::endl; 

 bool isValid = mcAnalysisVBF_ -> isValid();
  
 if( (eventType_ == 0) && (isValid == true) )
 {
  NtupleFactory_->Fill4V("mcV1",mcAnalysisVBF_ -> mcV1()->p4());
  NtupleFactory_->FillFloat("mcV1_charge",mcAnalysisVBF_ -> mcV1()->charge());
  NtupleFactory_->FillFloat("mcV1_pdgId",mcAnalysisVBF_ -> mcV1()->pdgId());
  
  NtupleFactory_->Fill4V("mcV2",mcAnalysisVBF_ -> mcV2()->p4());
  NtupleFactory_->FillFloat("mcV2_charge",mcAnalysisVBF_ -> mcV2()->charge());
  NtupleFactory_->FillFloat("mcV2_pdgId",mcAnalysisVBF_ -> mcV2()->pdgId());
     
  NtupleFactory_->Fill4V("mcF1_fromV1",mcAnalysisVBF_ -> mcF1_fromV1()->p4());
  NtupleFactory_->FillFloat("mcF1_fromV1_charge",mcAnalysisVBF_ -> mcF1_fromV1()->charge());
  NtupleFactory_->FillFloat("mcF1_fromV1_pdgId",mcAnalysisVBF_ -> mcF1_fromV1()->pdgId());

  NtupleFactory_->Fill4V("mcF2_fromV1",mcAnalysisVBF_ -> mcF2_fromV1()->p4());
  NtupleFactory_->FillFloat("mcF2_fromV1_charge",mcAnalysisVBF_ -> mcF2_fromV1()->charge());
  NtupleFactory_->FillFloat("mcF2_fromV1_pdgId",mcAnalysisVBF_ -> mcF2_fromV1()->pdgId());

  NtupleFactory_->Fill4V("mcF1_fromV2",mcAnalysisVBF_ -> mcF1_fromV2()->p4());
  NtupleFactory_->FillFloat("mcF1_fromV2_charge",mcAnalysisVBF_ -> mcF1_fromV2()->charge());
  NtupleFactory_->FillFloat("mcF1_fromV2_pdgId",mcAnalysisVBF_ -> mcF1_fromV2()->pdgId());

  NtupleFactory_->Fill4V("mcF2_fromV2",mcAnalysisVBF_ -> mcF2_fromV2()->p4());
  NtupleFactory_->FillFloat("mcF2_fromV2_charge",mcAnalysisVBF_ -> mcF2_fromV2()->charge());
  NtupleFactory_->FillFloat("mcF2_fromV2_pdgId",mcAnalysisVBF_ -> mcF2_fromV2()->pdgId());
    
  NtupleFactory_->Fill4V("mcQ1_tag",mcAnalysisVBF_ -> mcQ1_tag()->p4());
  NtupleFactory_->FillFloat("mcQ1_tag_charge",mcAnalysisVBF_ -> mcQ1_tag()->charge());
  NtupleFactory_->FillFloat("mcQ1_tag_pdgId",mcAnalysisVBF_ -> mcQ1_tag()->pdgId());

  NtupleFactory_->Fill4V("mcQ2_tag",mcAnalysisVBF_ -> mcQ2_tag()->p4());
  NtupleFactory_->FillFloat("mcQ2_tag_charge",mcAnalysisVBF_ -> mcQ2_tag()->charge());
  NtupleFactory_->FillFloat("mcQ2_tag_pdgId",mcAnalysisVBF_ -> mcQ2_tag()->pdgId());
      
 }
 
}
 


void SimpleNtple::fillMCEleInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //std::cout << "SimpleNtple::fillMCEleInfo" << std::endl; 
 
  for(unsigned int eleIt = 0; eleIt < (mcAnalysisVBF_ -> GetMcE()).size(); ++eleIt)
  {
    NtupleFactory_->Fill4V("mc_ele", (mcAnalysisVBF_ -> GetMcE()).at(eleIt)->p4());
    NtupleFactory_->FillFloat("mc_ele_charge", (mcAnalysisVBF_ -> GetMcE()).at(eleIt)->charge());
  }
}



void SimpleNtple::fillMCMuInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //std::cout << "SimpleNtple::fillMCMuInfo" << std::endl; 
 
 for(unsigned int muIt = 0; muIt < (mcAnalysisVBF_ -> GetMcMu()).size(); ++muIt)
 {
   NtupleFactory_->Fill4V("mc_mu", (mcAnalysisVBF_ -> GetMcMu()).at(muIt)->p4());
   NtupleFactory_->FillFloat("mc_mu_charge", (mcAnalysisVBF_ -> GetMcMu()).at(muIt)->charge());
 }
}


void SimpleNtple::fillMCTTBarInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
   //std::cout << "SimpleNtple::fillMCTTBarInfo" << std::endl;
 
 edm::Handle<reco::GenParticleCollection> genParticles;
 iEvent.getByLabel(MCtruthTag_, genParticles);
 
 bool isValid = mcAnalysisTTBar_->isValid();
  
 if( (eventType_ == 0) && (isValid == true) )
 {
  NtupleFactory_->Fill4V("mcT1",mcAnalysisTTBar_->mcT1()->p4());
  NtupleFactory_->FillFloat("mcT1_charge",mcAnalysisTTBar_->mcT1()->charge());
  
  NtupleFactory_->Fill4V("mcT2",mcAnalysisTTBar_->mcT2()->p4());
  NtupleFactory_->FillFloat("mcT2_charge",mcAnalysisTTBar_->mcT2()->charge());
  
  NtupleFactory_->Fill4V("mcB1",mcAnalysisTTBar_->mcB1()->p4());
  NtupleFactory_->FillFloat("mcB1_charge",mcAnalysisTTBar_->mcB1()->charge());
  
  NtupleFactory_->Fill4V("mcB2",mcAnalysisTTBar_->mcB2()->p4());
  NtupleFactory_->FillFloat("mcB2_charge",mcAnalysisTTBar_->mcB2()->charge());
  
  NtupleFactory_->Fill4V("mcV1",mcAnalysisTTBar_->mcV1()->p4());
  NtupleFactory_->FillFloat("mcV1_charge",mcAnalysisTTBar_->mcV1()->charge());
  NtupleFactory_->FillFloat("mcV1_pdgId",mcAnalysisTTBar_->mcV1()->pdgId());
  
  NtupleFactory_->Fill4V("mcV2",mcAnalysisTTBar_->mcV2()->p4());
  NtupleFactory_->FillFloat("mcV2_charge",mcAnalysisTTBar_->mcV2()->charge());
  NtupleFactory_->FillFloat("mcV2_pdgId",mcAnalysisTTBar_->mcV2()->pdgId());
     
  NtupleFactory_->Fill4V("mcF1_fromV1",mcAnalysisTTBar_->mcF1_fromV1()->p4());
  NtupleFactory_->FillFloat("mcF1_fromV1_charge",mcAnalysisTTBar_->mcF1_fromV1()->charge());
  NtupleFactory_->FillFloat("mcF1_fromV1_pdgId",mcAnalysisTTBar_->mcF1_fromV1()->pdgId());

  NtupleFactory_->Fill4V("mcF2_fromV1",mcAnalysisTTBar_->mcF2_fromV1()->p4());
  NtupleFactory_->FillFloat("mcF2_fromV1_charge",mcAnalysisTTBar_->mcF2_fromV1()->charge());
  NtupleFactory_->FillFloat("mcF2_fromV1_pdgId",mcAnalysisTTBar_->mcF2_fromV1()->pdgId());

  NtupleFactory_->Fill4V("mcF1_fromV2",mcAnalysisTTBar_->mcF1_fromV2()->p4());
  NtupleFactory_->FillFloat("mcF1_fromV2_charge",mcAnalysisTTBar_->mcF1_fromV2()->charge());
  NtupleFactory_->FillFloat("mcF1_fromV2_pdgId",mcAnalysisTTBar_->mcF1_fromV2()->pdgId());

  NtupleFactory_->Fill4V("mcF2_fromV2",mcAnalysisTTBar_->mcF2_fromV2()->p4());
  NtupleFactory_->FillFloat("mcF2_fromV2_charge",mcAnalysisTTBar_->mcF2_fromV2()->charge());
  NtupleFactory_->FillFloat("mcF2_fromV2_pdgId",mcAnalysisTTBar_->mcF2_fromV2()->pdgId());
        
  for (int iPartTau = 0; iPartTau < (int) mcAnalysisTTBar_->mcFX_fromV1_TauParticles()->size(); iPartTau++ ){
   NtupleFactory_->Fill4V("mcFX_fromV1_TauJet",mcAnalysisTTBar_->mcFX_fromV1_TauParticles()->at(iPartTau)->p4());
   NtupleFactory_->FillFloat("mcFX_fromV1_TauJet_pfgId",mcAnalysisTTBar_->mcFX_fromV1_TauParticles()->at(iPartTau)->pdgId());    
  }
  for (int iPartTau = 0; iPartTau < (int) mcAnalysisTTBar_->mcFX_fromV2_TauParticles()->size(); iPartTau++ ){
   NtupleFactory_->Fill4V("mcFX_fromV2_TauJet",mcAnalysisTTBar_->mcFX_fromV2_TauParticles()->at(iPartTau)->p4());
   NtupleFactory_->FillFloat("mcFX_fromV2_TauJet_pfgId",mcAnalysisTTBar_->mcFX_fromV2_TauParticles()->at(iPartTau)->pdgId());    
  }

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
  
   mcAnalysisVBF_ = new MCDumperVBF(genParticles, eventType_, verbosity_);
 }
  
 if(saveTTBar_)
 {
  mcAnalysisTTBar_ = new MCDumperTTBar(genParticles, eventType_, verbosity_);
 }

 ///---- fill HLT ----
 if (saveHLT_) fillHLTInfo (iEvent, iSetup);
  
 ///---- fill PV ----
 if(savePV_) fillPVInfo (iEvent, iSetup);

 ///---- fill muons ----
 if (saveMu_) fillMuInfo (iEvent, iSetup);

 ///---- fill electrons ----
 if (saveEle_)  fillEleInfo (iEvent, iSetup);

 ///---- fill tracks ----
 if (saveTrack_) fillTrackInfo (iEvent, iSetup);
 
 ///---- fill jets ---- 
 if (saveJet_) fillJetInfo (iEvent, iSetup);
 
 ///---- fill PF jets ---- 
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

 ///---- fill MCPtHat ---- 
 if(saveMCPtHat_) fillMCPtHatInfo (iEvent, iSetup);


 ///---- save the entry of the tree ----
 NtupleFactory_->FillNtuple();

}






///===================================
DEFINE_FWK_MODULE(SimpleNtple) ;


