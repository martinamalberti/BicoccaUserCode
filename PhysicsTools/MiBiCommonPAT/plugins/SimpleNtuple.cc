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
// $Id: SimpleNtple.cc,v 1.24 2010/10/11 18:01:35 amassiro Exp $
//
//


// system include files
#include <memory>

// user include files
#include "PhysicsTools/MiBiCommonPAT/plugins/SimpleNtuple.h"

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






SimpleNtuple::SimpleNtuple(const edm::ParameterSet& iConfig)
{
 //---- Out file ----
 mcAnalysisHiggs_ = NULL;
 mcAnalysisTTBar_ = NULL;
 edm::Service<TFileService> fs ;
 outTree_  = fs->make <TTree>("SimpleNtuple","SimpleNtuple"); 
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
 
 BTag_names_ = iConfig.getUntrackedParameter< std::vector<std::string> >("BTag_names");

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
  
 saveMCPtHat_ = iConfig.getUntrackedParameter<bool> ("saveMCPtHat", false);
 saveTTBar_ = iConfig.getUntrackedParameter<bool> ("saveTTBar",   false);
 saveMCHiggs_ = iConfig.getUntrackedParameter<bool> ("saveMCHiggs", false);
 saveMCHiggsWW_ = iConfig.getUntrackedParameter<bool> ("saveMCHiggsWW", false);
 saveMCHiggsGammaGamma_ = iConfig.getUntrackedParameter<bool> ("saveMCHiggsGammaGamma", false);
 saveMCEle_ = iConfig.getUntrackedParameter<bool> ("saveMCEle",   false);
 saveMCMu_ = iConfig.getUntrackedParameter<bool> ("saveMCMu",    false);

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
    
  for( std::vector<std::string>::const_iterator iBTag = BTag_names_.begin(); iBTag != BTag_names_.end(); iBTag++ ) {
   NtupleFactory_->AddFloat(*iBTag);
  }
      
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

  if(saveMCHiggsWW_)
  {
    NtupleFactory_->Add4V("mcQ1_tag");    
    NtupleFactory_->AddFloat("mcQ1_tag_charge");    
    NtupleFactory_->AddFloat("mcQ1_tag_pdgId");  
      
    NtupleFactory_->Add4V("mcQ2_tag");         
    NtupleFactory_->AddFloat("mcQ2_tag_charge");    
    NtupleFactory_->AddFloat("mcQ2_tag_pdgId");  
    
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
  }
  
  if(saveMCHiggsGammaGamma_)
  {
    NtupleFactory_->Add4V("mcQ1_tag");    
    NtupleFactory_->AddFloat("mcQ1_tag_charge");    
    NtupleFactory_->AddFloat("mcQ1_tag_pdgId");  
      
    NtupleFactory_->Add4V("mcQ2_tag");         
    NtupleFactory_->AddFloat("mcQ2_tag_charge");    
    NtupleFactory_->AddFloat("mcQ2_tag_pdgId");  
    
    NtupleFactory_->Add4V("mcV1");         
    NtupleFactory_->AddFloat("mcV1_charge");    
    NtupleFactory_->AddFloat("mcV1_pdgId");    
    
    NtupleFactory_->Add4V("mcV2");         
    NtupleFactory_->AddFloat("mcV2_charge");    
    NtupleFactory_->AddFloat("mcV2_pdgId");  
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


SimpleNtuple::~SimpleNtuple()
{
 NtupleFactory_->WriteNtuple();
 delete NtupleFactory_;
}




///-------------
///---- HLT ----
void SimpleNtuple::fillHLTInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
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


///---------------
///---- Muons ----

void SimpleNtuple::fillMuInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //std::cout << "SimpleNtuple::fillMuInfo" << std::endl;
 
 edm::Handle<edm::View<pat::Muon> > muHandle;
 iEvent.getByLabel(MuTag_,muHandle);
 edm::View<pat::Muon> muons = *muHandle;
 
 //edm::Handle<muMap> Mu3DipSignificanceHandle ;
 //iEvent.getByLabel (Mu3DipSignificanceTag_,Mu3DipSignificanceHandle);
 //edm::Handle<muMap> MuTipSignificanceHandle ;
 //iEvent.getByLabel (MuTipSignificanceTag_,MuTipSignificanceHandle);
 //edm::Handle<muMap> MuLipSignificanceHandle ;
 //iEvent.getByLabel (MuLipSignificanceTag_,MuLipSignificanceHandle);
 
 for ( unsigned int i=0; i<muons.size(); i++ ) {
    pat::Muon muon = muons.at(i);
      
  NtupleFactory_->Fill4V("muons",muon.p4());
  NtupleFactory_->FillInt("muons_goodMuon",muon::isGoodMuon(muon, muon::GlobalMuonPromptTight));

  NtupleFactory_->FillFloat("muons_charge",(muon.charge()));

  NtupleFactory_ -> FillFloat("muons_dxy",(muon.globalTrack())->dxy(PVPoint_));
  NtupleFactory_ -> FillFloat("muons_dz",(muon.globalTrack())->dz(PVPoint_));
 
  NtupleFactory_->FillFloat("muons_tkIsoR03",(muon.isolationR03()).sumPt);
  NtupleFactory_->FillFloat("muons_nTkIsoR03",(muon.isolationR03()).nTracks);    
  NtupleFactory_->FillFloat("muons_emIsoR03",(muon.isolationR03()).emEt);
  NtupleFactory_->FillFloat("muons_hadIsoR03",(muon.isolationR03()).hadEt);
    
  NtupleFactory_->FillFloat("muons_tkIsoR05",(muon.isolationR05()).sumPt);
  NtupleFactory_->FillFloat("muons_nTkIsoR05",(muon.isolationR05()).nTracks);    
  NtupleFactory_->FillFloat("muons_emIsoR05",(muon.isolationR05()).emEt);
  NtupleFactory_->FillFloat("muons_hadIsoR05",(muon.isolationR05()).hadEt);

//  NtupleFactory_->FillFloat("muons_3DipSignificance",((*Mu3DipSignificanceHandle)[muRef]));
//  NtupleFactory_->FillFloat("muons_tipSignificance",((*MuTipSignificanceHandle)[muRef]));
//  NtupleFactory_->FillFloat("muons_lipSignificance",((*MuLipSignificanceHandle)[muRef]));

  
  NtupleFactory_ -> FillInt  ("muons_tracker",muon.isTrackerMuon());
  NtupleFactory_ -> FillInt  ("muons_standalone",muon.isStandAloneMuon());
  NtupleFactory_ -> FillInt  ("muons_global",muon.isGlobalMuon());
  NtupleFactory_ -> FillFloat("muons_normalizedChi2",(muon.globalTrack())->normalizedChi2());
  NtupleFactory_ -> FillInt  ("muons_numberOfValidTrackerHits",(muon.globalTrack()->hitPattern()).numberOfValidTrackerHits());
  NtupleFactory_ -> FillInt  ("muons_numberOfValidMuonHits",(muon.globalTrack()->hitPattern()).numberOfValidMuonHits());

 }
}





///-------------------
///---- Electrons ----


void SimpleNtuple::fillEleInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //std::cout << "SimpleNtuple::fillEleInfo" << std::endl;
 
  edm::Handle<edm::View<pat::Electron> > eleHandle;
  iEvent.getByLabel(EleTag_,eleHandle);
  edm::View<pat::Electron> electrons = *eleHandle;

  //*********** CHANNEL STATUS
  edm::ESHandle<EcalChannelStatus> theChannelStatus;
  iESetup.get<EcalChannelStatusRcd>().get(theChannelStatus);

  //*********** EB REC HITS
  edm::Handle<EcalRecHitCollection> recHitsEB;
  iEvent.getByLabel( recHitCollection_EB_, recHitsEB );
  const EcalRecHitCollection* theBarrelEcalRecHits = recHitsEB.product () ;
  if ( ! recHitsEB.isValid() ) {
   std::cerr << "SimpleNtuple::analyze --> recHitsEB not found" << std::endl; 
  }
  
 //*********** EE REC HITS
  edm::Handle<EcalRecHitCollection> recHitsEE;
  iEvent.getByLabel( recHitCollection_EE_, recHitsEE );
  const EcalRecHitCollection* theEndcapEcalRecHits = recHitsEE.product () ;
  if ( ! recHitsEE.isValid() ) {
   std::cerr << "SimpleNtuple::analyze --> recHitsEE not found" << std::endl; 
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


  //edm::Handle<eleMap> Ele3DipSignificanceHandle ;
  //iEvent.getByLabel (Ele3DipSignificanceTag_,Ele3DipSignificanceHandle);
  //edm::Handle<eleMap> EleTipSignificanceHandle ;
  //iEvent.getByLabel (EleTipSignificanceTag_,EleTipSignificanceHandle);
  //edm::Handle<eleMap> EleLipSignificanceHandle ;
  //iEvent.getByLabel (EleLipSignificanceTag_,EleLipSignificanceHandle);
  
 std::vector<edm::Handle<edm::ValueMap<float> > > eleIdCutHandles(4) ;
 iEvent.getByLabel (eleIDCut_LooseInputTag_, eleIdCutHandles[0]) ;
 iEvent.getByLabel (eleIDCut_RLooseInputTag_, eleIdCutHandles[1]) ;
 iEvent.getByLabel (eleIDCut_TightInputTag_, eleIdCutHandles[2]) ;
 iEvent.getByLabel (eleIDCut_RTightInputTag_, eleIdCutHandles[3]) ;

 
 for ( unsigned int i=0; i<electrons.size(); ++i ) {
  pat::Electron electron = electrons.at(i);
  reco::SuperClusterRef scRef = electron.superCluster();

  const edm::Ptr<reco::CaloCluster>& seedCluster = scRef->seed();

  NtupleFactory_ -> Fill4V   ("electrons",electron.p4());
  NtupleFactory_ -> FillFloat("electrons_charge",electron.charge());
  NtupleFactory_ -> FillFloat("electrons_dxy",(electron.gsfTrack())->dxy(PVPoint_));
  NtupleFactory_ -> FillFloat("electrons_dz",(electron.gsfTrack())->dz(PVPoint_));
 
  NtupleFactory_ -> FillFloat("electrons_tkIsoR03",electron.dr03TkSumPt());
  NtupleFactory_ -> FillFloat("electrons_tkIsoR04",electron.dr04TkSumPt());
  NtupleFactory_ -> FillFloat("electrons_emIsoR03",electron.dr03EcalRecHitSumEt());
  NtupleFactory_ -> FillFloat("electrons_emIsoR04",electron.dr04EcalRecHitSumEt());
  NtupleFactory_ -> FillFloat("electrons_hadIsoR03_depth1",electron.dr03HcalDepth1TowerSumEt());
  NtupleFactory_ -> FillFloat("electrons_hadIsoR03_depth2",electron.dr03HcalDepth2TowerSumEt());
  NtupleFactory_ -> FillFloat("electrons_hadIsoR04_depth1",electron.dr04HcalDepth1TowerSumEt());
  NtupleFactory_ -> FillFloat("electrons_hadIsoR04_depth2",electron.dr04HcalDepth2TowerSumEt());

  if(electron.isEB()) NtupleFactory_ -> FillInt("electrons_isEB", 1);
  else               NtupleFactory_ -> FillInt("electrons_isEB", 0);
  if(electron.ecalDrivenSeed()) NtupleFactory_ -> FillInt("electrons_ecalDrivenSeed", 1);
  else                         NtupleFactory_ -> FillInt("electrons_ecalDrivenSeed", 0);
  if(electron.trackerDrivenSeed()) NtupleFactory_ -> FillInt("electrons_trackerDrivenSeed", 1);
  else                            NtupleFactory_ -> FillInt("electrons_trackerDrivenSeed", 0);
  
  //ELE ID
//  NtupleFactory_->FillFloat("electrons_IdLoose",(*(eleIdCutHandles[0]))[eleRef]) ;
//  NtupleFactory_->FillFloat("electrons_IdRobustLoose",(*(eleIdCutHandles[1]))[eleRef]);
//  NtupleFactory_->FillFloat("electrons_IdTight",(*(eleIdCutHandles[2]))[eleRef]);
//  NtupleFactory_->FillFloat("electrons_IdRobustTight",(*(eleIdCutHandles[3]))[eleRef]);

  NtupleFactory_ -> FillFloat("electrons_scE",scRef->energy());
  NtupleFactory_ -> FillFloat("electrons_eSeed",scRef->seed()->energy());
  NtupleFactory_ -> FillFloat("electrons_pin",electron.trackMomentumAtVtx().R());
  NtupleFactory_ -> FillFloat("electrons_pout",electron.trackMomentumOut().R());
  NtupleFactory_ -> FillFloat("electrons_eOverP",electron.eSuperClusterOverP());
  NtupleFactory_ -> FillInt  ("electrons_classification",electron.classification());
  NtupleFactory_ -> FillFloat("electrons_fbrem",electron.fbrem());
  NtupleFactory_ -> FillFloat("electrons_hOverE",electron.hadronicOverEm());
  NtupleFactory_ -> FillFloat("electrons_deltaPhiIn",electron.deltaPhiSuperClusterTrackAtVtx());
  NtupleFactory_ -> FillFloat("electrons_deltaEtaIn",electron.deltaEtaSuperClusterTrackAtVtx());
  NtupleFactory_ -> FillFloat("electrons_sigmaIetaIeta",electron.sigmaIetaIeta());

  NtupleFactory_->FillInt("electrons_mishits",electron.gsfTrack()->trackerExpectedHitsInner().numberOfHits());


  // conversion rejection variables
//  ConversionFinder convFinder;
//  ConversionInfo convInfo = convFinder.getConversionInfo((*EleHandle)[i], trackHandle, bField);
   
  NtupleFactory_->FillInt("electrons_nAmbiguousGsfTracks",electron.ambiguousGsfTracksSize());
//  NtupleFactory_->FillFloat("electrons_dist", convInfo.dist());
//  NtupleFactory_->FillFloat("electrons_dcot", convInfo.dcot());
   
   
   // spike removal variables
/*
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
   */

  // preshower variables 
  NtupleFactory_->FillFloat("electrons_ES",scRef->preshowerEnergy());

  //Get Ele Track
  reco::GsfTrackRef eleTrack  = electron.gsfTrack () ; 
      
  NtupleFactory_->FillFloat("electrons_track_d0", eleTrack->d0 ());
  NtupleFactory_->FillFloat("electrons_track_dz", eleTrack->dz ());
  NtupleFactory_->FillFloat("electrons_track_d0err", eleTrack->d0Error ());
  NtupleFactory_->FillFloat("electrons_track_dzerr", eleTrack->dzError ());
  
//  NtupleFactory_->FillFloat("electrons_3DipSignificance",((*Ele3DipSignificanceHandle)[eleRef]));
//  NtupleFactory_->FillFloat("electrons_tipSignificance",((*EleTipSignificanceHandle)[eleRef]));
//  NtupleFactory_->FillFloat("electrons_lipSignificance",((*EleLipSignificanceHandle)[eleRef]));
 }
}


///------------------------
///---- Primary Vertex ----

void SimpleNtuple::fillPVInfo(const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
  //std::cout << "SimpleNtuple::fillPVInfo::begin" << std::endl;
  
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
  
  //std::cout << "SimpleNtuple::fillPVInfo::end" << std::endl;
}




///----------------
///---- Tracks ----

void SimpleNtuple::fillTrackInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //std::cout << "SimpleNtuple::fillTrackInfo" << std::endl;
/* 
 edm::Handle<edm::View<reco::Track> > TracksHandle ;
 iEvent.getByLabel (TracksTag_, TracksHandle) ;
 for (edm::View<reco::Track>::const_iterator tkIt = TracksHandle->begin (); tkIt != TracksHandle->end (); ++tkIt ) 
 { 
  NtupleFactory_->Fill3V("tracks_in",(*tkIt).innerMomentum ());
  NtupleFactory_->Fill3V("tracks_out",(*tkIt).outerMomentum ());
 }
*/
}






///---- Jets ----

void SimpleNtuple::fillJetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
  //std::cout << "SimpleNtuple::fillJetInfo::begin" << std::endl;
 
  edm::Handle<edm::View<pat::Jet> > jetHandle;
  iEvent.getByLabel(JetTag_,jetHandle);
  edm::View<pat::Jet> jets = *jetHandle;
 
  // loop on jets
  for ( unsigned int i=0; i<jets.size(); ++i ) {
  pat::Jet jet = jets.at(i);

  NtupleFactory_ -> Fill4V   ("jets",jet.p4());
  NtupleFactory_ -> FillFloat("jets_emEnergyFraction",jet.emEnergyFraction());
  NtupleFactory_ -> FillFloat("jets_etaetaMoment",jet.etaetaMoment());
  NtupleFactory_ -> FillFloat("jets_phiphiMoment",jet.phiphiMoment());
  NtupleFactory_ -> FillFloat("jets_etaphiMoment",jet.etaphiMoment());
  NtupleFactory_ -> FillFloat("jets_jetArea",jet.jetArea());
    
    
  NtupleFactory_->FillFloat("jets_fHPD",jet.jetID().fHPD);
  NtupleFactory_->FillFloat("jets_fRBX",jet.jetID().fRBX);
  NtupleFactory_->FillFloat("jets_n90Hits",jet.jetID().n90Hits);
  NtupleFactory_->FillFloat("jets_nHCALTowers",jet.jetID().nHCALTowers);
  NtupleFactory_->FillFloat("jets_nECALTowers",jet.jetID().nECALTowers);
 
  NtupleFactory_ -> FillFloat("jets_charge",jet.charge());
 

  NtupleFactory_ -> FillFloat("jets_chargedHadronEnergy",jet.chargedHadronEnergy()); 
  NtupleFactory_ -> FillFloat("jets_chargedHadronEnergyFraction",jet.chargedHadronEnergyFraction()); 
  NtupleFactory_ -> FillFloat("jets_neutralHadronEnergy",jet.neutralHadronEnergy()); 
  NtupleFactory_ -> FillFloat("jets_neutralHadronEnergyFraction",jet.neutralHadronEnergyFraction()); 
  NtupleFactory_ -> FillFloat("jets_chargedEmEnergy",jet.chargedEmEnergy()); 
  NtupleFactory_ -> FillFloat("jets_chargedEmEnergyFraction",jet.chargedEmEnergyFraction()); 
  NtupleFactory_ -> FillFloat("jets_chargedMuEnergy",jet.chargedMuEnergy()); 
  NtupleFactory_ -> FillFloat("jets_chargedMuEnergyFraction",jet.chargedMuEnergyFraction()); 
  NtupleFactory_ -> FillFloat("jets_neutralEmEnergy",jet.neutralEmEnergy()); 
  NtupleFactory_ -> FillFloat("jets_neutralEmEnergyFraction",jet.neutralEmEnergyFraction()); 
  NtupleFactory_ -> FillInt  ("jets_chargedMultiplicity",jet.chargedMultiplicity()); 
  NtupleFactory_ -> FillInt  ("jets_neutralMultiplicity",jet.neutralMultiplicity()); 
  NtupleFactory_ -> FillInt  ("jets_muonMultiplicity",jet.muonMultiplicity()); 
   
    
 //==== jet b tagging
  for( std::vector<std::string>::const_iterator iBTag = BTag_names_.begin(); iBTag != BTag_names_.end(); iBTag++ ) {
    NtupleFactory_ -> FillFloat(*iBTag,jet.bDiscriminator(*iBTag));
   }
  } // loop on jets
  
  //std::cout << "SimpleNtuple::fillJetInfo::end" << std::endl;
 
}


///---- MET ----

void SimpleNtuple::fillMetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //std::cout << "SimpleNtuple::fillMetInfo" << std::endl;
 
  edm::Handle<edm::View<pat::MET> > MetHandle;
  iEvent.getByLabel(MetTag_,MetHandle);
  edm::View<pat::MET> mets = *MetHandle;
  
 edm::Handle<reco::CaloMETCollection> Type1MetHandle ;
 iEvent.getByLabel (Type1MetTag_,Type1MetHandle);
 
 edm::Handle<reco::METCollection> TcMetHandle ;
 iEvent.getByLabel (TcMetTag_,TcMetHandle);
 
 edm::Handle<reco::PFMETCollection> PFMetHandle ;
 iEvent.getByLabel (PFMetTag_,PFMetHandle);
 
 NtupleFactory_->Fill4V("met",mets.at(0).p4());
 
 const reco::MET* type1Met = &(Type1MetHandle->front());
 NtupleFactory_->Fill4V("type1Met",type1Met->p4());
 
 const reco::MET* TcMet = &(TcMetHandle->front());
 NtupleFactory_->Fill4V("TcMet",TcMet->p4());
 
 const reco::PFMET* PFMet = &(PFMetHandle->front());
 NtupleFactory_->Fill4V("PFMet",PFMet->p4());
}







///---- MC ----

void SimpleNtuple::fillMCPtHatInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
  //std::cout << "SimpleNtuple::fillPtHatInfo::begin" << std::endl; 
  
  edm::Handle<edm::HepMCProduct> HepMCEvt;
  iEvent.getByLabel("generator", HepMCEvt);
  float ptHat = HepMCEvt -> GetEvent() -> event_scale();
  
  NtupleFactory_->FillFloat("mc_ptHat", ptHat);
  
  //std::cout << "SimpleNtuple::fillPtHatInfo::end" << std::endl; 
}


void SimpleNtuple::fillMCHiggsInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //std::cout << "SimpleNtuple::fillMCHiggsDecayInfo" << std::endl; 

 bool isValid = mcAnalysisHiggs_ -> isValid();
  
 if( (eventType_ == 0) && (isValid == true) )
 {
  NtupleFactory_->Fill4V("mcQ1_tag",mcAnalysisHiggs_ -> mcQ1_tag()->p4());
  NtupleFactory_->FillFloat("mcQ1_tag_charge",mcAnalysisHiggs_ -> mcQ1_tag()->charge());
  NtupleFactory_->FillFloat("mcQ1_tag_pdgId",mcAnalysisHiggs_ -> mcQ1_tag()->pdgId());

  NtupleFactory_->Fill4V("mcQ2_tag",mcAnalysisHiggs_ -> mcQ2_tag()->p4());
  NtupleFactory_->FillFloat("mcQ2_tag_charge",mcAnalysisHiggs_ -> mcQ2_tag()->charge());
  NtupleFactory_->FillFloat("mcQ2_tag_pdgId",mcAnalysisHiggs_ -> mcQ2_tag()->pdgId());

  NtupleFactory_->Fill4V("mcV1",mcAnalysisHiggs_ -> mcV1()->p4());
  NtupleFactory_->FillFloat("mcV1_charge",mcAnalysisHiggs_ -> mcV1()->charge());
  NtupleFactory_->FillFloat("mcV1_pdgId",mcAnalysisHiggs_ -> mcV1()->pdgId());
  
  NtupleFactory_->Fill4V("mcV2",mcAnalysisHiggs_ -> mcV2()->p4());
  NtupleFactory_->FillFloat("mcV2_charge",mcAnalysisHiggs_ -> mcV2()->charge());
  NtupleFactory_->FillFloat("mcV2_pdgId",mcAnalysisHiggs_ -> mcV2()->pdgId());
     
  NtupleFactory_->Fill4V("mcF1_fromV1",mcAnalysisHiggs_ -> mcF1_fromV1()->p4());
  NtupleFactory_->FillFloat("mcF1_fromV1_charge",mcAnalysisHiggs_ -> mcF1_fromV1()->charge());
  NtupleFactory_->FillFloat("mcF1_fromV1_pdgId",mcAnalysisHiggs_ -> mcF1_fromV1()->pdgId());

  NtupleFactory_->Fill4V("mcF2_fromV1",mcAnalysisHiggs_ -> mcF2_fromV1()->p4());
  NtupleFactory_->FillFloat("mcF2_fromV1_charge",mcAnalysisHiggs_ -> mcF2_fromV1()->charge());
  NtupleFactory_->FillFloat("mcF2_fromV1_pdgId",mcAnalysisHiggs_ -> mcF2_fromV1()->pdgId());

  NtupleFactory_->Fill4V("mcF1_fromV2",mcAnalysisHiggs_ -> mcF1_fromV2()->p4());
  NtupleFactory_->FillFloat("mcF1_fromV2_charge",mcAnalysisHiggs_ -> mcF1_fromV2()->charge());
  NtupleFactory_->FillFloat("mcF1_fromV2_pdgId",mcAnalysisHiggs_ -> mcF1_fromV2()->pdgId());

  NtupleFactory_->Fill4V("mcF2_fromV2",mcAnalysisHiggs_ -> mcF2_fromV2()->p4());
  NtupleFactory_->FillFloat("mcF2_fromV2_charge",mcAnalysisHiggs_ -> mcF2_fromV2()->charge());
  NtupleFactory_->FillFloat("mcF2_fromV2_pdgId",mcAnalysisHiggs_ -> mcF2_fromV2()->pdgId());
 }
 
}
 


void SimpleNtuple::fillMCTTBarInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
   //std::cout << "SimpleNtuple::fillMCTTBarInfo" << std::endl;
 
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
void SimpleNtuple::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  ++eventNaiveId_;
  
  NtupleFactory_->FillInt("runId", iEvent.id().run());
  NtupleFactory_->FillInt("lumiId", iEvent.luminosityBlock());
  NtupleFactory_->FillInt("BXId", iEvent.bunchCrossing());
  NtupleFactory_->FillInt("eventId", iEvent.id().event());
  NtupleFactory_->FillInt("eventNaiveId", eventNaiveId_);
  
  
  
  
  
 edm::Handle<reco::GenParticleCollection> genParticles;

 if(saveMCHiggs_ || saveMCHiggsWW_ || saveMCHiggsGammaGamma_ || saveMCEle_ || saveMCMu_)
 {
   iEvent.getByLabel(MCtruthTag_, genParticles);
   mcAnalysisHiggs_ = new MCDumperHiggs(genParticles, eventType_, verbosity_);
 }
  
 if(saveTTBar_)
 {
   iEvent.getByLabel(MCtruthTag_, genParticles);
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
 
 ///---- fill met ---- 
 if (saveMet_) fillMetInfo (iEvent, iSetup);
 
 ///---- fill MCParticle ---- 
 if (saveMCHiggs_) fillMCHiggsInfo (iEvent, iSetup);
 if (saveTTBar_) fillMCTTBarInfo (iEvent, iSetup);

 ///---- fill MCPtHat ---- 
 if(saveMCPtHat_) fillMCPtHatInfo (iEvent, iSetup);

 ///---- save the entry of the tree ----
 NtupleFactory_->FillNtuple();

}






///===================================
DEFINE_FWK_MODULE(SimpleNtuple) ;


