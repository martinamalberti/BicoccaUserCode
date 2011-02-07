// -*- C++ -*-
//
// Package:    SimpleNtuple
// Class:      SimpleNtuple
// 
/**\class SimpleNtuple SimpleNtuple.cc Analysis/SimpleNtuple/src/SimpleNtuple.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
 */
//
// Original Author:  Andrea Massironi
//         Created:  Fri Jan  5 17:34:31 CEST 2010
// $Id: SimpleNtuple.cc,v 1.17 2011/02/01 16:57:16 deguio Exp $
//
//

#include "PhysicsTools/MiBiCommonPAT/plugins/SimpleNtuple.h"



#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"

#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"

#include "RecoVertex/PrimaryVertexProducer/interface/VertexHigherPtSquared.h"

#include "Math/Vector4D.h"
#include "Math/Vector3D.h"
///--------------
///---- ctor ----

SimpleNtuple::SimpleNtuple(const edm::ParameterSet& iConfig)
{
 //---- Out file ----
 edm::Service<TFileService> fs;
 outTree_ = fs -> make<TTree>("SimpleNtuple","SimpleNtuple"); 
 NtupleFactory_ = new NtupleFactory(outTree_);  
 
 
 //---- MC dumpers ----
 mcAnalysisHiggs_ = NULL;
 mcAnalysisTTBar_ = NULL;
 
 
 //---- Input tags ---- 
 TriggerEventTag_ = iConfig.getParameter<edm::InputTag>("TriggerEventTag");
 TriggerResultsTag_ = iConfig.getParameter<edm::InputTag>("TriggerResultsTag");
  
 PVTag_ = iConfig.getParameter<edm::InputTag>("PVTag");
 
 EleTag_      = iConfig.getParameter<edm::InputTag>("EleTag");
 EleID_names_ = iConfig.getParameter< std::vector<std::string> >("EleID_names");
 
 MuTag_ = iConfig.getParameter<edm::InputTag>("MuTag");

 PhotonTag_      = iConfig.getParameter<edm::InputTag>("PhotonTag");
 EBRechitTag_      = iConfig.getParameter<edm::InputTag>("EBRechitTag");
 EERechitTag_      = iConfig.getParameter<edm::InputTag>("EERechitTag");

 MetTag_   = iConfig.getParameter<edm::InputTag>("MetTag");
 TCMetTag_ = iConfig.getParameter<edm::InputTag>("TCMetTag");
 PFMetTag_ = iConfig.getParameter<edm::InputTag>("PFMetTag");
 
 JetTag_     = iConfig.getParameter<edm::InputTag>("JetTag");
 BTag_names_ = iConfig.getParameter< std::vector<std::string> >("BTag_names");
 
 MCtruthTag_ = iConfig.getParameter<edm::InputTag>("MCtruthTag");
 
 
 //---- flags ----
 dataFlag_      = iConfig.getUntrackedParameter<bool> ("dataFlag", true);
 saveHLT_       = iConfig.getUntrackedParameter<bool> ("saveHLT", true);
 saveBS_        = iConfig.getUntrackedParameter<bool> ("saveBS", true);
 savePV_        = iConfig.getUntrackedParameter<bool> ("savePV", true);
 saveEle_       = iConfig.getUntrackedParameter<bool> ("saveEle", true);
 saveMu_        = iConfig.getUntrackedParameter<bool> ("saveMu", true);
 savePhoton_    = iConfig.getUntrackedParameter<bool> ("savePhoton", true);
 saveMet_       = iConfig.getUntrackedParameter<bool> ("saveMet", true);
 saveJet_       = iConfig.getUntrackedParameter<bool> ("saveJet", true);
 saveHCALNoise_ = iConfig.getUntrackedParameter<bool> ("saveHCALNoise", true);
 saveMCPtHat_           = iConfig.getUntrackedParameter<bool> ("saveMCPtHat", false);
 saveMCTTBar_           = iConfig.getUntrackedParameter<bool> ("saveMCTTBar", false);
 saveMCHiggs_           = iConfig.getUntrackedParameter<bool> ("saveMCHiggs", false);
 saveMCHiggsWW_         = iConfig.getUntrackedParameter<bool> ("saveMCHiggsWW", false);
 saveMCHiggsGammaGamma_ = iConfig.getUntrackedParameter<bool> ("saveMCHiggsGammaGamma", false);
 saveMCZW_              = iConfig.getUntrackedParameter<bool> ("saveMCZW", false);

 verbosity_ = iConfig.getUntrackedParameter<bool>("verbosity", false);
 eventType_ = iConfig.getUntrackedParameter<int>("eventType", 1);
 
 
 
 
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
   NtupleFactory_->AddString("HLT_Names"); 
 }
 
 if(saveBS_)
 {
   NtupleFactory_ -> AddFloat("BS_x0"); 
   NtupleFactory_ -> AddFloat("BS_y0"); 
   NtupleFactory_ -> AddFloat("BS_z0"); 
   NtupleFactory_ -> AddFloat("BS_sigmaZ"); 
   NtupleFactory_ -> AddFloat("BS_dxdz"); 
   NtupleFactory_ -> AddFloat("BS_dydz"); 
   NtupleFactory_ -> AddFloat("BS_BeamWidthX"); 
   NtupleFactory_ -> AddFloat("BS_BeamWidthY"); 
 }

 if(savePV_)
 {
   NtupleFactory_ -> AddFloat("PV_normalizedChi2"); 
   NtupleFactory_ -> AddInt  ("PV_ndof"); 
   NtupleFactory_ -> AddInt  ("PV_nTracks"); 
   NtupleFactory_ -> AddFloat("PV_z"); 
   NtupleFactory_ -> AddFloat("PV_d0"); 
   NtupleFactory_ -> AddFloat("PV_SumPt"); 
 }
 
 if(saveEle_)
 {
   NtupleFactory_ -> Add4V   ("electrons");
   NtupleFactory_ -> AddFloat("electrons_charge"); 
   NtupleFactory_ -> AddFloat("electrons_dB");
   NtupleFactory_ -> AddFloat("electrons_edB");
   NtupleFactory_ -> AddFloat("electrons_dxy");
   NtupleFactory_ -> AddFloat("electrons_edxy");
   NtupleFactory_ -> AddFloat("electrons_dz");
   NtupleFactory_ -> AddFloat("electrons_edz");
   NtupleFactory_ -> AddFloat("electrons_dxy_PV");
   NtupleFactory_ -> AddFloat("electrons_dz_PV");
   
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
   NtupleFactory_ -> AddFloat("electrons_mva");
   NtupleFactory_ -> AddFloat("electrons_eSC");
   NtupleFactory_ -> AddFloat("electrons_pin");
   NtupleFactory_ -> AddFloat("electrons_pout");
   NtupleFactory_ -> AddFloat("electrons_pcalo");
   NtupleFactory_ -> AddFloat("electrons_eSCOverP");
   NtupleFactory_ -> AddFloat("electrons_eSeedOverP");
   NtupleFactory_ -> AddInt  ("electrons_classification");
   NtupleFactory_ -> AddFloat("electrons_fbrem");
   NtupleFactory_ -> AddFloat("electrons_hOverE");
   NtupleFactory_ -> AddFloat("electrons_deltaPhiIn");
   NtupleFactory_ -> AddFloat("electrons_deltaEtaIn");
   NtupleFactory_ -> AddFloat("electrons_sigmaIetaIeta");
   NtupleFactory_ -> AddFloat("electrons_e1x5");
   NtupleFactory_ -> AddFloat("electrons_e2x5Max");
   NtupleFactory_ -> AddFloat("electrons_e5x5");
   
   for( std::vector<std::string>::const_iterator iEleID = EleID_names_.begin(); iEleID != EleID_names_.end(); iEleID++ ) {
    NtupleFactory_->AddFloat(*iEleID);
   }
   
   NtupleFactory_->AddInt("electrons_mishits");
   NtupleFactory_->AddInt("electrons_nAmbiguousGsfTracks");
   
   NtupleFactory_->AddFloat("electrons_eES");
  }
 
 if(saveMu_)
 {
   NtupleFactory_ -> Add4V   ("muons");
   NtupleFactory_ -> AddFloat("muons_charge"); 
   NtupleFactory_ -> AddFloat("muons_dB");
   NtupleFactory_ -> AddFloat("muons_edB"); 
   NtupleFactory_ -> AddFloat("muons_dxy");
   NtupleFactory_ -> AddFloat("muons_edxy");
   NtupleFactory_ -> AddFloat("muons_dz");
   NtupleFactory_ -> AddFloat("muons_edz");
   NtupleFactory_ -> AddFloat("muons_dxy_PV");
   NtupleFactory_ -> AddFloat("muons_dz_PV");
   
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
   NtupleFactory_ -> AddInt  ("muons_goodMuon");
   NtupleFactory_ -> AddFloat("muons_normalizedChi2");
   NtupleFactory_ -> AddInt  ("muons_numberOfValidTrackerHits");
   NtupleFactory_ -> AddInt  ("muons_numberOfValidMuonHits");
 }
 
 if(savePhoton_)
 {
   NtupleFactory_ -> Add4V ("photons");
   NtupleFactory_ -> AddFloat ("photons_hcalIso");
   NtupleFactory_ -> AddInt  ("photons_isGap");
   NtupleFactory_ -> AddFloat("photons_e1x5");           
   NtupleFactory_ -> AddFloat("photons_e2x5");         
   NtupleFactory_ -> AddFloat("photons_e3x3");         
   NtupleFactory_ -> AddFloat("photons_e5x5");         
   NtupleFactory_ -> AddFloat("photons_maxEnergyXtal");
   NtupleFactory_ -> AddFloat("photons_sigmaEtaEta");  
   NtupleFactory_ -> AddFloat("photons_sigmaIetaIeta");
   NtupleFactory_ -> AddFloat("photons_r1x5");        
   NtupleFactory_ -> AddFloat("photons_r2x5");        
   NtupleFactory_ -> AddFloat("photons_r9");
   NtupleFactory_ -> AddFloat("photons_ecalIso");   
   NtupleFactory_ -> AddFloat("photons_hcalIso");   
   NtupleFactory_ -> AddFloat("photons_hadronicOverEm");   
   NtupleFactory_ -> AddFloat("photons_trkSumPtHollowConeDR04");   
   NtupleFactory_ -> AddInt("photons_hasPixelSeed");   
   NtupleFactory_ -> Add4V("photons_SC");   

   NtupleFactory_ ->AddTMatrix("photons_rechitTime");
   NtupleFactory_ ->AddTMatrix("photons_rechitE");

 }

 if(saveMet_)
 {
   NtupleFactory_->Add4V("Met");         
   NtupleFactory_->Add4V("TCMet");         
   NtupleFactory_->Add4V("PFMet");         
 }
 
 if(saveJet_)
 {
   NtupleFactory_->Add4V("jets");
   NtupleFactory_->AddFloat("jets_charge");   
   NtupleFactory_->AddFloat("jets_dzAvg");   
   NtupleFactory_->AddFloat("jets_dzAvgCut");   
   
   NtupleFactory_->AddFloat("jets_corrFactor_raw");   
   NtupleFactory_->AddFloat("jets_corrFactor_off");   
   NtupleFactory_->AddFloat("jets_corrFactor_rel");   
   NtupleFactory_->AddFloat("jets_corrFactor_abs");   
   NtupleFactory_->AddFloat("jets_corrFactor_res");   
   
   for( std::vector<std::string>::const_iterator iBTag = BTag_names_.begin(); iBTag != BTag_names_.end(); iBTag++ ) {
    NtupleFactory_->AddFloat(*iBTag);
   }
   
   NtupleFactory_->AddFloat("jets_etaetaMoment");
   NtupleFactory_->AddFloat("jets_phiphiMoment");   
   NtupleFactory_->AddFloat("jets_etaphiMoment");   
   
   NtupleFactory_->AddFloat("jets_fHPD");   
   NtupleFactory_->AddFloat("jets_fRBX");   
   NtupleFactory_->AddFloat("jets_n90Hits");   
   NtupleFactory_->AddFloat("jets_nHCALTowers");   
   NtupleFactory_->AddFloat("jets_nECALTowers");   
   
   NtupleFactory_->AddFloat("jets_towersArea");   
   NtupleFactory_->AddFloat("jets_emEnergyFraction");   
   
   NtupleFactory_->AddFloat("jets_chargedHadronEnergyFraction");
   NtupleFactory_->AddFloat("jets_neutralHadronEnergyFraction"); 
   NtupleFactory_->AddFloat("jets_chargedEmEnergyFraction"); 
   NtupleFactory_->AddFloat("jets_neutralEmEnergyFraction"); 
   NtupleFactory_->AddFloat("jets_photonEnergyFraction"); 
   NtupleFactory_->AddFloat("jets_muonEnergyFraction"); 
   NtupleFactory_->AddInt  ("jets_chargedMultiplicity"); 
   NtupleFactory_->AddInt  ("jets_neutralMultiplicity"); 
   NtupleFactory_->AddInt  ("jets_muonMultiplicity"); 
 }
 
 if(saveHCALNoise_)
 {
   NtupleFactory_ -> AddInt("HCAL_noise");
 }  
 
 ///==== Gen level ====  
 if(saveMCPtHat_)
 {
   NtupleFactory_->AddFloat("mc_ptHat");    
 }
 
 if(saveMCTTBar_)
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

 if(saveMCHiggs_)
 {
   NtupleFactory_->Add4V("mc_H");    
   NtupleFactory_->AddFloat("mc_H_charge");    
   
   if(saveMCHiggsWW_ || saveMCHiggsGammaGamma_)
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
   
   if(saveMCHiggsWW_)
   {
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
 }
 
 if(saveMCZW_)
   {
     NtupleFactory_->Add4V("mc_V");    
     NtupleFactory_->AddFloat("mc_V_charge");    
     NtupleFactory_->AddFloat("mcV_pdgId");    
     NtupleFactory_->Add3V("mc_V_vertex");
     
     NtupleFactory_->Add4V("mcQ1_tag");    
     NtupleFactory_->AddFloat("mcQ1_tag_charge");    
     NtupleFactory_->AddFloat("mcQ1_tag_pdgId");  
     
     NtupleFactory_->Add4V("mcQ2_tag");         
     NtupleFactory_->AddFloat("mcQ2_tag_charge");    
     NtupleFactory_->AddFloat("mcQ2_tag_pdgId");  
     
     NtupleFactory_->Add4V("mcF1_fromV");   
     NtupleFactory_->AddFloat("mcF1_fromV_charge");    
     NtupleFactory_->AddFloat("mcF1_fromV_pdgId");  
     
     NtupleFactory_->Add4V("mcF2_fromV");         
     NtupleFactory_->AddFloat("mcF2_fromV_charge");    
     NtupleFactory_->AddFloat("mcF2_fromV_pdgId");       
   }
 
}






///--------------
///---- dtor ----

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
  edm::Handle<trigger::TriggerEvent> triggerEventHandle;
  iEvent.getByLabel(TriggerEventTag_, triggerEventHandle);
  const edm::Provenance* provenance = triggerEventHandle.provenance();
  //std::cout << "Trigger process name = " << provenance->processName() << std::endl;
  
  edm::Handle<edm::TriggerResults> triggerResultsHandle;
  iEvent.getByLabel(edm::InputTag(TriggerResultsTag_.label(), TriggerResultsTag_.instance(), provenance->processName()), triggerResultsHandle);
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






///-------------------
///---- Beam Spot ----

void SimpleNtuple::fillBSInfo(const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
  //std::cout << "SimpleNtuple::fillBSInfo::begin" << std::endl;
  
  edm::Handle<reco::BeamSpot> BSHandle;
  iEvent.getByType(BSHandle);
  
  
  // select the BS
  const reco::BeamSpot BS = *BSHandle;
  math::XYZPoint BSPoint(BS.position().x(), BS.position().y(), BS.position().z());
  BSPoint_ = BSPoint;
  
  
  NtupleFactory_ -> FillFloat("BS_x0", BS.x0());
  NtupleFactory_ -> FillFloat("BS_y0", BS.y0());
  NtupleFactory_ -> FillFloat("BS_z0", BS.z0());
  NtupleFactory_ -> FillFloat("BS_sigmaZ", BS.sigmaZ());
  NtupleFactory_ -> FillFloat("BS_dxdz", BS.dxdz());
  NtupleFactory_ -> FillFloat("BS_dydz", BS.dydz());
  NtupleFactory_ -> FillFloat("BS_BeamWidthX", BS.BeamWidthX());
  NtupleFactory_ -> FillFloat("BS_BeamWidthY", BS.BeamWidthY());
  
  //std::cout << "SimpleNtuple::fillPVInfo::end" << std::endl;
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
    VertexHigherPtSquared vertexTool;
    // sort the primary vertices according to sum of (pt)^2 of tracks (first one -> highest  sum of (pt)^2 )        
    PrimaryVertexSorter PVSorter;
    std::vector<reco::Vertex> sortedVertices = PVSorter.sortedList( *(vertexes.product()) );
    for( unsigned int u = 0 ; u < sortedVertices.size(); u++ ){
      PV = sortedVertices[u];
      
      NtupleFactory_ -> FillFloat("PV_normalizedChi2", PV.normalizedChi2());
      NtupleFactory_ -> FillInt  ("PV_ndof", PV.ndof());
      NtupleFactory_ -> FillInt  ("PV_nTracks", PV.tracksSize());
      NtupleFactory_ -> FillFloat("PV_z", PV.z());
      NtupleFactory_ -> FillFloat("PV_d0", PV.position().Rho());
      NtupleFactory_ -> FillFloat("PV_SumPt",vertexTool.sumPtSquared(PV));
    }
    PV = sortedVertices[0];
  }
  
  else
  {
    //creating a dummy PV
    reco::Vertex::Point p(BSPoint_.x(),BSPoint_.y(),BSPoint_.z());
    reco::Vertex::Error e;
    e(0,0) = 0.0015*0.0015;
    e(1,1) = 0.0015*0.0015;
    e(2,2) = 15.*15.;
    PV = reco::Vertex(p, e, 1, 1, 1);
    
    NtupleFactory_ -> FillFloat("PV_normalizedChi2", -1.);
    NtupleFactory_ -> FillInt  ("PV_ndof", -1);
    NtupleFactory_ -> FillInt  ("PV_nTracks", -1);
    NtupleFactory_ -> FillFloat("PV_z", -9999.);
    NtupleFactory_ -> FillFloat("PV_d0", -9999.);
    NtupleFactory_ -> FillFloat("PV_SumPt",-9999.);
  }
  
  math::XYZPoint PVPoint(PV.position().x(), PV.position().y(), PV.position().z());
  PVPoint_ = PVPoint;
  
  //std::cout << "SimpleNtuple::fillPVInfo::end" << std::endl;
}






///---------------
///---- Muons ----

void SimpleNtuple::fillMuInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //std::cout << "SimpleNtuple::fillMuInfo" << std::endl;
 
 edm::Handle<edm::View<pat::Muon> > muHandle;
 iEvent.getByLabel(MuTag_,muHandle);
 edm::View<pat::Muon> muons = *muHandle;
 
 for ( unsigned int i=0; i<muons.size(); i++ ) {
  pat::Muon muon = muons.at(i);
  reco::TrackRef tkRef; 
  
  if(muon.isGlobalMuon())  
    tkRef = muon.globalTrack();
  else if(!muon.isGlobalMuon() && muon.isTrackerMuon())
    tkRef = muon.innerTrack();
  else if(!muon.isGlobalMuon() && muon.isStandAloneMuon())
    tkRef = muon.outerTrack();
  else
    continue;  
  
  NtupleFactory_ -> Fill4V   ("muons",muon.p4());
  NtupleFactory_ -> FillFloat("muons_charge",(muon.charge()));
  NtupleFactory_ -> FillFloat("muons_dB",muon.dB());
  NtupleFactory_ -> FillFloat("muons_edB",muon.edB());
  NtupleFactory_ -> FillFloat("muons_dxy",tkRef->dxy());
  NtupleFactory_ -> FillFloat("muons_edxy",tkRef->dxyError());
  NtupleFactory_ -> FillFloat("muons_dz",tkRef->dz());
  NtupleFactory_ -> FillFloat("muons_edz",tkRef->dzError());
  NtupleFactory_ -> FillFloat("muons_dxy_PV",tkRef->dxy(PVPoint_));
  NtupleFactory_ -> FillFloat("muons_dz_PV",tkRef->dz(PVPoint_));

  NtupleFactory_ -> FillFloat("muons_tkIsoR03",(muon.isolationR03()).sumPt);
  NtupleFactory_ -> FillFloat("muons_nTkIsoR03",(muon.isolationR03()).nTracks);    
  NtupleFactory_ -> FillFloat("muons_emIsoR03",(muon.isolationR03()).emEt);
  NtupleFactory_ -> FillFloat("muons_hadIsoR03",(muon.isolationR03()).hadEt);
  
  NtupleFactory_ -> FillFloat("muons_tkIsoR05",(muon.isolationR05()).sumPt);
  NtupleFactory_ -> FillFloat("muons_nTkIsoR05",(muon.isolationR05()).nTracks);    
  NtupleFactory_ -> FillFloat("muons_emIsoR05",(muon.isolationR05()).emEt);
  NtupleFactory_ -> FillFloat("muons_hadIsoR05",(muon.isolationR05()).hadEt);
  
  NtupleFactory_ -> FillInt  ("muons_tracker",muon.isTrackerMuon());
  NtupleFactory_ -> FillInt  ("muons_standalone",muon.isStandAloneMuon());
  NtupleFactory_ -> FillInt  ("muons_global",muon.isGlobalMuon());
  NtupleFactory_ -> FillInt  ("muons_goodMuon",muon::isGoodMuon(muon, muon::GlobalMuonPromptTight));
  NtupleFactory_ -> FillFloat("muons_normalizedChi2",tkRef->normalizedChi2());
  NtupleFactory_ -> FillInt  ("muons_numberOfValidTrackerHits",tkRef->hitPattern().numberOfValidTrackerHits());
  NtupleFactory_ -> FillInt  ("muons_numberOfValidMuonHits",tkRef->hitPattern().numberOfValidMuonHits());
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
 
 
 for ( unsigned int i=0; i<electrons.size(); ++i )
 {
  pat::Electron electron = electrons.at(i);
  reco::SuperClusterRef scRef = electron.superCluster();
  reco::GsfTrackRef tkRef = electron.gsfTrack (); 
  
  NtupleFactory_ -> Fill4V   ("electrons", electron.p4());
  NtupleFactory_ -> FillFloat("electrons_charge", electron.charge());
  NtupleFactory_ -> FillFloat("electrons_dB", electron.dB());
  NtupleFactory_ -> FillFloat("electrons_edB", electron.edB());
  NtupleFactory_ -> FillFloat("electrons_dxy", tkRef->dxy());
  NtupleFactory_ -> FillFloat("electrons_edxy", tkRef->dxyError());
  NtupleFactory_ -> FillFloat("electrons_dz", tkRef->dz());
  NtupleFactory_ -> FillFloat("electrons_edz", tkRef->dzError());
  NtupleFactory_ -> FillFloat("electrons_dxy_PV", tkRef->dxy(PVPoint_));
  NtupleFactory_ -> FillFloat("electrons_dz_PV", tkRef->dz(PVPoint_));
  
  NtupleFactory_ -> FillFloat("electrons_tkIsoR03",electron.dr03TkSumPt());
  NtupleFactory_ -> FillFloat("electrons_tkIsoR04",electron.dr04TkSumPt());
  NtupleFactory_ -> FillFloat("electrons_emIsoR03",electron.dr03EcalRecHitSumEt());
  NtupleFactory_ -> FillFloat("electrons_emIsoR04",electron.dr04EcalRecHitSumEt());
  NtupleFactory_ -> FillFloat("electrons_hadIsoR03_depth1",electron.dr03HcalDepth1TowerSumEt());
  NtupleFactory_ -> FillFloat("electrons_hadIsoR03_depth2",electron.dr03HcalDepth2TowerSumEt());
  NtupleFactory_ -> FillFloat("electrons_hadIsoR04_depth1",electron.dr04HcalDepth1TowerSumEt());
  NtupleFactory_ -> FillFloat("electrons_hadIsoR04_depth2",electron.dr04HcalDepth2TowerSumEt());
  
  if(electron.isEB()) NtupleFactory_ -> FillInt("electrons_isEB", 1);
  else                NtupleFactory_ -> FillInt("electrons_isEB", 0);
  if(electron.ecalDrivenSeed()) NtupleFactory_ -> FillInt("electrons_ecalDrivenSeed", 1);
  else                          NtupleFactory_ -> FillInt("electrons_ecalDrivenSeed", 0);
  if(electron.trackerDrivenSeed())
  { 
    NtupleFactory_ -> FillInt("electrons_trackerDrivenSeed", 1);
    NtupleFactory_ -> FillFloat("electrons_mva",electron.mva());
  }
  else
  {
    NtupleFactory_ -> FillInt("electrons_trackerDrivenSeed", 0);
    NtupleFactory_ -> FillFloat("electrons_mva",-9999.);
  }
  
  //ELE ID
  for( std::vector<std::string>::const_iterator iEleID = EleID_names_.begin(); iEleID != EleID_names_.end(); iEleID++ ) {
    NtupleFactory_ -> FillFloat(*iEleID,electron.electronID(*iEleID));
  }
  
  NtupleFactory_ -> FillFloat("electrons_eSC",scRef->energy());
  NtupleFactory_ -> FillFloat("electrons_pin",electron.trackMomentumAtVtx().R());
  NtupleFactory_ -> FillFloat("electrons_pout",electron.trackMomentumOut().R());
  NtupleFactory_ -> FillFloat("electrons_pcalo",electron.trackMomentumAtCalo().R());
  NtupleFactory_ -> FillFloat("electrons_eSCOverP",electron.eSuperClusterOverP());
  NtupleFactory_ -> FillFloat("electrons_eSeedOverP",electron.eSeedClusterOverP());
  NtupleFactory_ -> FillInt  ("electrons_classification",electron.classification());
  NtupleFactory_ -> FillFloat("electrons_fbrem",electron.fbrem());
  NtupleFactory_ -> FillFloat("electrons_hOverE",electron.hadronicOverEm());
  NtupleFactory_ -> FillFloat("electrons_deltaPhiIn",electron.deltaPhiSuperClusterTrackAtVtx());
  NtupleFactory_ -> FillFloat("electrons_deltaEtaIn",electron.deltaEtaSuperClusterTrackAtVtx());
  NtupleFactory_ -> FillFloat("electrons_sigmaIetaIeta",electron.sigmaIetaIeta());
  NtupleFactory_ -> FillFloat("electrons_e1x5",electron.e1x5());
  NtupleFactory_ -> FillFloat("electrons_e2x5Max",electron.e2x5Max());
  NtupleFactory_ -> FillFloat("electrons_e5x5",electron.e5x5());
  
  // conversion rejection variables
  NtupleFactory_ -> FillInt("electrons_mishits",electron.gsfTrack()->trackerExpectedHitsInner().numberOfHits());
  NtupleFactory_ -> FillInt("electrons_nAmbiguousGsfTracks",electron.ambiguousGsfTracksSize());

  // preshower variables 
  NtupleFactory_->FillFloat("electrons_eES",scRef->preshowerEnergy());
 }
 
}





///-----------------
///---- Photons ----

void SimpleNtuple::fillPhotonInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //std::cout << "SimpleNtuple::fillPhotonInfo" << std::endl;
 
 edm::Handle<edm::View<pat::Photon> > photonHandle;
 iEvent.getByLabel(PhotonTag_,photonHandle);
 edm::View<pat::Photon> photons = *photonHandle;
 
 
 for ( unsigned int i=0; i<photons.size(); ++i )
 {
  pat::Photon photon = photons.at(i);
  
  NtupleFactory_ -> Fill4V   ("photons", photon.p4());
  NtupleFactory_ -> FillFloat("photons_hcalIso", photon.hcalIso());
  
  NtupleFactory_ -> FillInt  ("photons_isGap",photon.isEBEtaGap() || photon.isEBPhiGap() || photon.isEERingGap() || photon.isEEDeeGap() || photon.isEBEEGap() );
  NtupleFactory_ -> FillFloat("photons_e1x5",photon.e1x5());           
  NtupleFactory_ -> FillFloat("photons_e2x5",photon.e2x5());         
  NtupleFactory_ -> FillFloat("photons_e3x3",photon.e3x3());         
  NtupleFactory_ -> FillFloat("photons_e5x5",photon.e5x5());         
  NtupleFactory_ -> FillFloat("photons_maxEnergyXtal",photon.maxEnergyXtal());
  NtupleFactory_ -> FillFloat("photons_sigmaEtaEta",photon.sigmaEtaEta());  
  NtupleFactory_ -> FillFloat("photons_sigmaIetaIeta",photon.sigmaIetaIeta());
  NtupleFactory_ -> FillFloat("photons_r1x5",photon.r1x5());        
  NtupleFactory_ -> FillFloat("photons_r2x5",photon.r2x5());        
  NtupleFactory_ -> FillFloat("photons_r9",photon.r9());   
  NtupleFactory_ -> FillFloat("photons_ecalIso",photon.ecalIso());   
  NtupleFactory_ -> FillFloat("photons_hcalIso",photon.hcalIso());   
  NtupleFactory_ -> FillFloat("photons_hadronicOverEm",photon.hadronicOverEm());   
  NtupleFactory_ -> FillFloat("photons_trkSumPtHollowConeDR04",photon.trkSumPtHollowConeDR04());   
  NtupleFactory_ -> FillInt  ("photons_hasPixelSeed",photon.hasPixelSeed());   


  //superCluster Info
  reco::SuperClusterRef phoSC = photon.superCluster();
  
  double pos = sqrt(phoSC->x()*phoSC->x() + phoSC->y()*phoSC->y() + phoSC->z()*phoSC->z());
  double ratio = phoSC->energy() / pos;
  ROOT::Math::XYZTVector phoVec(phoSC->x()*ratio, phoSC->y()*ratio, phoSC->z()*ratio, phoSC->energy());
  NtupleFactory_ -> Fill4V("photons_SC", phoVec);


  //recHit time and energy
  TMatrix rechitTime(3,3);
  TMatrix rechitE(3,3);
  for (int i=0;i<3;i++){
    for (int j=0; j< 3; j++){
      rechitTime[i][j]=999;
      rechitE[i][j]=-999;
    }
  }

   // calo topology
  edm::ESHandle<CaloTopology> pTopology;
  iESetup.get<CaloTopologyRecord>().get(pTopology);
  const CaloTopology *topology = pTopology.product();
 // Ecal barrel RecHits 
  edm::Handle<EcalRecHitCollection> pBarrelEcalRecHits ;
  iEvent.getByLabel (EBRechitTag_, pBarrelEcalRecHits) ;
  const EcalRecHitCollection* theBarrelEcalRecHits = pBarrelEcalRecHits.product () ;

  edm::Handle<EcalRecHitCollection> pEndcapEcalRecHits ;
  iEvent.getByLabel (EERechitTag_, pEndcapEcalRecHits) ;
  const EcalRecHitCollection* theEndcapEcalRecHits = pEndcapEcalRecHits.product () ;


  if( photon.isEB() ){
    EBDetId ebid = (EcalClusterTools::getMaximum( photon.superCluster()->hitsAndFractions(), theBarrelEcalRecHits )).first;
    for(int xx = 0; xx < 3; ++xx)
      for(int yy = 0; yy < 3; ++yy)
	{
	  std::vector<DetId> vector =  EcalClusterTools::matrixDetId(topology, ebid, xx-1, xx-1, yy-1, yy-1);
	  if(vector.size() == 0) continue;
	  EcalRecHitCollection::const_iterator iterator = theBarrelEcalRecHits->find (vector.at(0)) ;
	  if(iterator == theBarrelEcalRecHits->end()) continue;
	  rechitE[xx][yy]  = iterator -> energy();
	  rechitTime[xx][yy]  = iterator -> time();
	}
    
  }
  else if (  photon.isEE() ){
    EEDetId ebid = (EcalClusterTools::getMaximum( photon.superCluster()->hitsAndFractions(), theEndcapEcalRecHits )).first;
    for(int xx = 0; xx < 3; ++xx)
      for(int yy = 0; yy < 3; ++yy)
	{
	  std::vector<DetId> vector =  EcalClusterTools::matrixDetId(topology, ebid, xx-1, xx-1, yy-1, yy-1);
	  if(vector.size() == 0) continue;
	  EcalRecHitCollection::const_iterator iterator = theEndcapEcalRecHits->find (vector.at(0)) ;
	  if(iterator == theEndcapEcalRecHits->end()) continue;
	  rechitE[xx][yy]  = iterator -> energy();
	  rechitTime[xx][yy]  = iterator -> time();
	}
  }
  

  NtupleFactory_ -> FillTMatrix("photons_rechitTime",rechitTime);
  NtupleFactory_ -> FillTMatrix("photons_rechitE",rechitE);

 }
 
}



///--------------
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
  NtupleFactory_ -> FillFloat("jets_charge",jet.charge());
  
  
  // jet energy corrections
  bool isUncorrectedLevelFound = false;
  bool isL1OffsetLevelFound = false;
  bool isL2RelativeLevelFound = false;
  bool isL3AbsoluteLevelFound = false;
  bool isL2L3ResidualLevelFound = false;
  
  std::vector<std::string> jecLevels = jet.availableJECLevels();
  for(unsigned int kk = 0; kk < jecLevels.size(); ++kk)
  {
    if(jecLevels.at(kk) == "Uncorrected")
    {
      NtupleFactory_ -> FillFloat("jets_corrFactor_raw",jet.jecFactor("Uncorrected"));
      isUncorrectedLevelFound = true;
    }
    
    if(jecLevels.at(kk) == "L1Offset")
    {
      NtupleFactory_ -> FillFloat("jets_corrFactor_off",jet.jecFactor("L1Offset"));
      isL1OffsetLevelFound = true;
    }
    
    if(jecLevels.at(kk) == "L2Relative")
    {
      NtupleFactory_ -> FillFloat("jets_corrFactor_rel",jet.jecFactor("L2Relative"));
      isL2RelativeLevelFound = true;
    }
    
    if(jecLevels.at(kk) == "L3Absolute")
    {
      NtupleFactory_ -> FillFloat("jets_corrFactor_abs",jet.jecFactor("L3Absolute"));
      isL3AbsoluteLevelFound = true;
    }
    
    if(jecLevels.at(kk) == "L2L3Residual")
    {
      NtupleFactory_ -> FillFloat("jets_corrFactor_res",jet.jecFactor("L2L3Residual"));
      isL2L3ResidualLevelFound = true;
    }
  }
  
  if(isUncorrectedLevelFound == false)  NtupleFactory_ -> FillFloat("jets_corrFactor_raw", -1.);
  if(isL1OffsetLevelFound == false)     NtupleFactory_ -> FillFloat("jets_corrFactor_off", -1.);
  if(isL2RelativeLevelFound == false)   NtupleFactory_ -> FillFloat("jets_corrFactor_rel", -1.);
  if(isL3AbsoluteLevelFound == false)   NtupleFactory_ -> FillFloat("jets_corrFactor_abs", -1.);
  if(isL2L3ResidualLevelFound == false) NtupleFactory_ -> FillFloat("jets_corrFactor_res", -1.);
  
  
  
  //==== jet b tagging
  for( std::vector<std::string>::const_iterator iBTag = BTag_names_.begin(); iBTag != BTag_names_.end(); iBTag++ )
    NtupleFactory_ -> FillFloat(*iBTag,jet.bDiscriminator(*iBTag));
  
  NtupleFactory_ -> FillFloat("jets_etaetaMoment",jet.etaetaMoment());
  NtupleFactory_ -> FillFloat("jets_phiphiMoment",jet.phiphiMoment());
  NtupleFactory_ -> FillFloat("jets_etaphiMoment",jet.etaphiMoment());
  
  NtupleFactory_->FillFloat("jets_fHPD",jet.jetID().fHPD);
  NtupleFactory_->FillFloat("jets_fRBX",jet.jetID().fRBX);
  NtupleFactory_->FillFloat("jets_n90Hits",jet.jetID().n90Hits);
  NtupleFactory_->FillFloat("jets_nHCALTowers",jet.jetID().nHCALTowers);
  NtupleFactory_->FillFloat("jets_nECALTowers",jet.jetID().nECALTowers);
  
  if (jet.isCaloJet()){
   NtupleFactory_ -> FillFloat("jets_emEnergyFraction",jet.emEnergyFraction());
   NtupleFactory_ -> FillFloat("jets_towersArea",jet.towersArea());
  }

  if (jet.isPFJet()){
   NtupleFactory_ -> FillFloat("jets_chargedHadronEnergyFraction",jet.chargedHadronEnergyFraction()); 
   NtupleFactory_ -> FillFloat("jets_neutralHadronEnergyFraction",jet.neutralHadronEnergyFraction()); 
   NtupleFactory_ -> FillFloat("jets_chargedEmEnergyFraction",jet.chargedEmEnergyFraction()); 
   NtupleFactory_ -> FillFloat("jets_neutralEmEnergyFraction",jet.neutralEmEnergyFraction()); 
   NtupleFactory_ -> FillFloat("jets_photonEnergyFraction",jet.photonEnergyFraction()); 
   NtupleFactory_ -> FillFloat("jets_muonEnergyFraction",jet.muonEnergyFraction()); 
   NtupleFactory_ -> FillInt  ("jets_chargedMultiplicity",jet.chargedMultiplicity()); 
   NtupleFactory_ -> FillInt  ("jets_neutralMultiplicity",jet.neutralMultiplicity()); 
   NtupleFactory_ -> FillInt  ("jets_muonMultiplicity",jet.muonMultiplicity()); 
   
   // loop on charged constituents to get avg z
   std::vector<reco::PFCandidatePtr> jetConstituents =  jet.getPFConstituents();
   float dzAvg = 0.;
   int nChargedConstituents = 0;
   for(unsigned int jj = 0; jj < jetConstituents.size(); ++jj)
   {
     if( jetConstituents.at(jj)->trackRef().isNonnull() )
     {
       dzAvg += jetConstituents.at(jj)->trackRef()->dz();
       ++nChargedConstituents;
     }
   }
   
   if(nChargedConstituents > 0) dzAvg /= nChargedConstituents;
   else dzAvg = -9999.;
   
   NtupleFactory_ -> FillFloat("jets_dzAvg", dzAvg);
   
   
   if( dzAvg > -9999.)
   {
     float dzAvgCut = 0.;
     int nChargedConstituentsCut = 0;
     for(unsigned int jj = 0; jj < jetConstituents.size(); ++jj)
     {
       if( jetConstituents.at(jj)->trackRef().isNonnull() )
       {
         if( fabs(jetConstituents.at(jj)->trackRef()->dz() - dzAvg) > 0.5 ) continue;
         
         dzAvgCut += jetConstituents.at(jj)->trackRef()->dz();
         ++nChargedConstituentsCut;
       }
     }
     
     if(nChargedConstituentsCut > 0) dzAvgCut /= nChargedConstituentsCut;
     else dzAvgCut = -9999.;
     
     NtupleFactory_ -> FillFloat("jets_dzAvgCut", dzAvgCut);
   }


   } 
   
  } // loop on jets
  
  //std::cout << "SimpleNtuple::fillJetInfo::end" << std::endl;
}






///--------------------
///---- HCAL Noise ----
void SimpleNtuple::fillHCALNoiseInfo(const edm::Event & iEvent, const edm::EventSetup & iESetup)
{
  //std::cout << "SimpleNtuple::fillHCALNoiseInfo::begin" << std::endl;
  
  edm::Handle<bool> HBHENoiseFilterResultHandle;
  iEvent.getByLabel("HBHENoiseFilterResultProducer", "HBHENoiseFilterResult", HBHENoiseFilterResultHandle);

  NtupleFactory_ -> FillInt("HCAL_noise", *(HBHENoiseFilterResultHandle.product()));

  //std::cout << "SimpleNtuple::fillHCALNoiseInfo::end" << std::endl;
}






///-------------
///---- MET ----

void SimpleNtuple::fillMetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //std::cout << "SimpleNtuple::fillMetInfo" << std::endl;
 
  edm::Handle<edm::View<pat::MET> > MetHandle;
  iEvent.getByLabel(MetTag_,MetHandle);
  edm::View<pat::MET> Met = *MetHandle;
  
  edm::Handle<edm::View<pat::MET> > TCMetHandle;
  iEvent.getByLabel(TCMetTag_,TCMetHandle);
  edm::View<pat::MET> TCMet = *TCMetHandle;

  edm::Handle<edm::View<pat::MET> > PFMetHandle;
  iEvent.getByLabel(PFMetTag_,PFMetHandle);
  edm::View<pat::MET> PFMet = *PFMetHandle;
 
  NtupleFactory_->Fill4V("Met", Met.at(0).p4());
  NtupleFactory_->Fill4V("TCMet", TCMet.at(0).p4());
  NtupleFactory_->Fill4V("PFMet", PFMet.at(0).p4());
}






///------------
///---- MC ----

void SimpleNtuple::fillMCPtHatInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
  //std::cout << "SimpleNtuple::fillPtHatInfo::begin" << std::endl; 
  
  edm::Handle< GenEventInfoProduct > GenInfoHandle;
  iEvent.getByLabel( "generator", GenInfoHandle );
  float ptHat = ( GenInfoHandle->hasBinningValues() ? (GenInfoHandle->binningValues())[0] : 0.0);
  
  NtupleFactory_->FillFloat("mc_ptHat", ptHat);
  
  //std::cout << "SimpleNtuple::fillPtHatInfo::end" << std::endl; 
}


void SimpleNtuple::fillMCHiggsInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //std::cout << "SimpleNtuple::fillMCHiggsDecayInfo" << std::endl; 

 bool isValid = mcAnalysisHiggs_ -> isValid();
  
 if( (eventType_ == 0) && (isValid == true) )
 {
   NtupleFactory_->Fill4V("mc_H",mcAnalysisHiggs_ -> mcH()->p4());
   NtupleFactory_->FillFloat("mc_H_charge",mcAnalysisHiggs_ -> mcH()->charge());
   
   if(saveMCHiggsWW_ || saveMCHiggsGammaGamma_)
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
   }
   
   if(saveMCHiggsWW_)
   {
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

}


void SimpleNtuple::fillMCZWInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //std::cout << "SimpleNtuple::fillMCZWDecayInfo" << std::endl; 

 bool isValid = mcAnalysisZW_ -> isValid();
  
 if( (eventType_ == 0) && (isValid == true) )
 {

   NtupleFactory_->Fill4V("mc_V",mcAnalysisZW_ -> mcV()->p4());
   NtupleFactory_->FillFloat("mc_V_charge",mcAnalysisZW_ -> mcV()->charge());
   NtupleFactory_->FillFloat("mcV_pdgId",mcAnalysisZW_ -> mcV()->pdgId());

   math::XYZPoint p(mcAnalysisZW_ -> mcV()->vertex());
   ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<double>,ROOT::Math::DefaultCoordinateSystemTag> vertex(p.x(), p.y(), p.z());
   NtupleFactory_->Fill3V("mc_V_vertex", vertex);   
   
   NtupleFactory_->Fill4V("mcQ1_tag",mcAnalysisZW_ -> mcQ1_tag()->p4());
   NtupleFactory_->FillFloat("mcQ1_tag_charge",mcAnalysisZW_ -> mcQ1_tag()->charge());
   NtupleFactory_->FillFloat("mcQ1_tag_pdgId",mcAnalysisZW_ -> mcQ1_tag()->pdgId());
   
   NtupleFactory_->Fill4V("mcQ2_tag",mcAnalysisZW_ -> mcQ2_tag()->p4());
   NtupleFactory_->FillFloat("mcQ2_tag_charge",mcAnalysisZW_ -> mcQ2_tag()->charge());
   NtupleFactory_->FillFloat("mcQ2_tag_pdgId",mcAnalysisZW_ -> mcQ2_tag()->pdgId());
   
   NtupleFactory_->Fill4V("mcF1_fromV",mcAnalysisZW_ -> mcF1_fromV()->p4());
   NtupleFactory_->FillFloat("mcF1_fromV_charge",mcAnalysisZW_ -> mcF1_fromV()->charge());
   NtupleFactory_->FillFloat("mcF1_fromV_pdgId",mcAnalysisZW_ -> mcF1_fromV()->pdgId());
   
   NtupleFactory_->Fill4V("mcF2_fromV",mcAnalysisZW_ -> mcF2_fromV()->p4());
   NtupleFactory_->FillFloat("mcF2_fromV_charge",mcAnalysisZW_ -> mcF2_fromV()->charge());
   NtupleFactory_->FillFloat("mcF2_fromV_pdgId",mcAnalysisZW_ -> mcF2_fromV()->pdgId());
 } 
 
} 


void SimpleNtuple::fillMCTTBarInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 //std::cout << "SimpleNtuple::fillMCTTBarInfo" << std::endl;
 
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
void SimpleNtuple::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 ++eventNaiveId_;
 
 NtupleFactory_->FillInt("runId", iEvent.id().run());
 NtupleFactory_->FillInt("lumiId", iEvent.luminosityBlock());
 NtupleFactory_->FillInt("BXId", iEvent.bunchCrossing());
 NtupleFactory_->FillInt("eventId", iEvent.id().event());
 NtupleFactory_->FillInt("eventNaiveId", eventNaiveId_);
 
 
 edm::Handle<reco::GenParticleCollection> genParticles;
 
 if(saveMCTTBar_)
 {
  iEvent.getByLabel(MCtruthTag_, genParticles);
  mcAnalysisTTBar_ = new MCDumperTTBar(genParticles, eventType_, verbosity_);
 }
 
 if(saveMCHiggs_)
   {
     iEvent.getByLabel(MCtruthTag_, genParticles);
     mcAnalysisHiggs_ = new MCDumperHiggs(genParticles, eventType_, verbosity_);
   }
 
 if(saveMCZW_)
   {
     iEvent.getByLabel(MCtruthTag_, genParticles);
     mcAnalysisZW_ = new MCDumperZW(genParticles, eventType_, verbosity_);
   }
 
 ///---- fill HLT ----
 if (saveHLT_) fillHLTInfo (iEvent, iSetup);
  
 ///---- fill PV ----
 if(saveBS_) fillBSInfo (iEvent, iSetup);
 
 ///---- fill PV ----
 if(savePV_) fillPVInfo (iEvent, iSetup);
 
 ///---- fill muons ----
 if (saveMu_) fillMuInfo (iEvent, iSetup);

 ///---- fill electrons ----
 if (saveEle_)  fillEleInfo (iEvent, iSetup);
 
 ///---- fill photons ----
 if(savePhoton_)  fillPhotonInfo (iEvent, iSetup);
 
 ///---- fill met ---- 
 if (saveMet_) fillMetInfo (iEvent, iSetup);
 
 ///---- fill jets ---- 
 if (saveJet_) fillJetInfo (iEvent, iSetup);
 
 ///---- fill HCAL noise ---- 
 if(saveHCALNoise_) fillHCALNoiseInfo (iEvent, iSetup);
  
 ///---- fill MCPtHat ---- 
 if(saveMCPtHat_) fillMCPtHatInfo (iEvent, iSetup);

 ///---- fill MCParticle ---- 
 if (saveMCTTBar_) fillMCTTBarInfo (iEvent, iSetup);
 if (saveMCHiggs_) fillMCHiggsInfo (iEvent, iSetup);
 if (saveMCZW_) fillMCZWInfo (iEvent, iSetup);

 ///---- save the entry of the tree ----
 NtupleFactory_->FillNtuple();

}






///===================================
DEFINE_FWK_MODULE(SimpleNtuple) ;


