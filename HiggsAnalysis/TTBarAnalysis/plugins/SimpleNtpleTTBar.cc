// -*- C++ -*-
//
// Package:    SimpleNtpleTTBar
// Class:      SimpleNtpleTTBar
// 
/**\class SimpleNtpleTTBar SimpleNtpleTTBar.cc Analysis/SimpleNtpleTTBar/src/SimpleNtpleTTBar.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
 */
//
// Original Author:  Andrea Massironi
//         Created:  Fri Jan  5 17:34:31 CEST 2010
// $Id: SimpleNtpleTTBar.cc,v 1.6 2010/01/27 10:39:48 amassiro Exp $
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

#include "HiggsAnalysis/TTBarAnalysis/plugins/SimpleNtpleTTBar.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"


//---- utilities ----
#include "HiggsAnalysis/TTBarAnalysis/interface/MCDumperTTBar.h"


SimpleNtpleTTBar::SimpleNtpleTTBar(const edm::ParameterSet& iConfig)
{
 edm::Service<TFileService> fs ;
 outTree_  = fs->make <TTree>("SimpleTree","SimpleTree"); 
 
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
 JetTag_ = iConfig.getParameter<edm::InputTag>("JetTag");
 MCtruthTag_ = iConfig.getParameter<edm::InputTag>("MCtruthTag");
 genJetTag_ = iConfig.getParameter<edm::InputTag>("genJetTag");
 genMetTag_ = iConfig.getParameter<edm::InputTag>("genMetTag");

 eleIDCut_LooseInputTag_ = iConfig.getParameter<edm::InputTag> ("eleIDCut_LooseInputTag");
 eleIDCut_RLooseInputTag_ = iConfig.getParameter<edm::InputTag> ("eleIDCut_RLooseInputTag");
 eleIDCut_TightInputTag_ = iConfig.getParameter<edm::InputTag> ("eleIDCut_TightInputTag");
 eleIDCut_RTightInputTag_ = iConfig.getParameter<edm::InputTag> ("eleIDCut_RTightInputTag");
 
 //---- ref check ----
 doEleRefCheck_ = iConfig.getUntrackedParameter<bool>("doEleRefCheck", false);
 if(doEleRefCheck_) EleRefTag_  = iConfig.getParameter<edm::InputTag>("EleRefTag");
 
 doMuRefCheck_ = iConfig.getUntrackedParameter<bool>("doMuRefCheck", false);
 if(doMuRefCheck_) MuRefTag_ = iConfig.getParameter<edm::InputTag>("MuRefTag");
 
 doJetRefCheck_ = iConfig.getUntrackedParameter<bool>("doJetRefCheck", false);
 if(doJetRefCheck_) JetRefTag_ = iConfig.getParameter<edm::InputTag>("JetRefTag");
 
 //---- flags ----
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
 saveMC_ = iConfig.getUntrackedParameter<bool> ("saveMC", true);

 verbosity_ = iConfig.getUntrackedParameter<bool>("verbosity","False");
 eventType_ = iConfig.getUntrackedParameter<int>("eventType",1);
 
 NtupleFactory_ = new NtupleFactory(outTree_);
}


// --------------------------------------------------------------------


SimpleNtpleTTBar::~SimpleNtpleTTBar()
{
 delete NtupleFactory_;
}


//
// member functions
//

///---- muons ----
void SimpleNtpleTTBar::fillMuInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
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
        
  NtupleFactory_->FillFloat("muons_track_d0", ((*MuHandle)[i].track())->d0 ());
  NtupleFactory_->FillFloat("muons_track_dz", ((*MuHandle)[i].track())->dz ());
  NtupleFactory_->FillFloat("muons_track_d0err",  ((*MuHandle)[i].track())->d0Error ());
  NtupleFactory_->FillFloat("muons_track_dzerr",  ((*MuHandle)[i].track())->dzError ());
  
 }
}

///---- electrons ----

void SimpleNtpleTTBar::fillEleInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
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
  NtupleFactory_->FillFloat("electrons_emIso",((*EleHandle)[i].dr03EcalRecHitSumEt()));
  NtupleFactory_->FillFloat("electrons_hadIso_1",((*EleHandle)[i].dr03HcalDepth1TowerSumEt()));
  NtupleFactory_->FillFloat("electrons_hadIso_2",((*EleHandle)[i].dr03HcalDepth2TowerSumEt()));
  //   if ((*EleHandle)[i].classification()== GsfElectron::GOLDEN
  
  //ELE ID
  NtupleFactory_->FillFloat("electrons_IdLoose",(*(eleIdCutHandles[0]))[eleRef]);
  NtupleFactory_->FillFloat("electrons_IdTight",(*(eleIdCutHandles[1]))[eleRef]);
  NtupleFactory_->FillFloat("electrons_IdRobustLoose",(*(eleIdCutHandles[2]))[eleRef]);
  NtupleFactory_->FillFloat("electrons_IdRobustTight",(*(eleIdCutHandles[3]))[eleRef]);
      
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

void SimpleNtpleTTBar::fillTrackInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
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

void SimpleNtpleTTBar::fillJetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
  //std::cout << "SimpleNtple::fillJetInfo" << std::endl;
 
 edm::Handle<reco::CaloJetCollection> JetHandle ;
 iEvent.getByLabel (JetTag_,JetHandle);

 edm::Handle<edm::RefVector<reco::CaloJetCollection> > JetRefHandle;
 if(doJetRefCheck_)
  iEvent.getByLabel(JetRefTag_, JetRefHandle);
 
 
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
   
  if(saveJetBTagging_)
   fillJetBTaggingInfo(iEvent, iESetup, (*JetHandle)[i].p4());
   
 }// loop on jets
 
}



///---- PFJets ----
void SimpleNtpleTTBar::fillPFJetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
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

  if(saveJetBTagging_)
   fillJetBTaggingInfo(iEvent, iESetup, (*JetHandle)[i].p4());
 }
}






///---- JetBTagging ----
void SimpleNtpleTTBar::fillJetBTaggingInfo(const edm::Event & iEvent, const edm::EventSetup & iESetup,
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

void SimpleNtpleTTBar::fillMetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
   //std::cout << "SimpleNtple::fillMetInfo" << std::endl;
 
 edm::Handle<reco::CaloMETCollection> MetHandle ;
 iEvent.getByLabel (MetTag_,MetHandle);
 
 edm::Handle<reco::CaloMETCollection> Type1MetHandle ;
 iEvent.getByLabel (Type1MetTag_,Type1MetHandle);
 
 edm::Handle<reco::PFMETCollection> PFMetHandle ;
 iEvent.getByLabel (PFMetTag_,PFMetHandle);
 
 
 
 const reco::CaloMET* met = &(MetHandle->front());
 NtupleFactory_->Fill4V("met",met->p4());
 
 const reco::MET* type1Met = &(Type1MetHandle->front());
 NtupleFactory_->Fill4V("type1Met",type1Met->p4());
 
 const reco::PFMET* PFMet = &(PFMetHandle->front());
 NtupleFactory_->Fill4V("PFMet",PFMet->p4());
}


///---- GenMet ----

void SimpleNtpleTTBar::fillGenMetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
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
void SimpleNtpleTTBar::fillGenJetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
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
void SimpleNtpleTTBar::fillMCInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
   //std::cout << "SimpleNtple::fillMCInfo" << std::endl;
 
 edm::Handle<reco::GenParticleCollection> genParticles;
 iEvent.getByLabel(MCtruthTag_, genParticles);
 
 MCDumperTTBar mcAnalysis(genParticles, eventType_, verbosity_);
 bool isValid = mcAnalysis.isValid();
  
 if( (eventType_ == 0) && (isValid == true) )
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
  SimpleNtpleTTBar::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
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
 if (saveMC_) fillMCInfo (iEvent, iSetup);
  
 ///---- save the entry of the tree ----
 NtupleFactory_->FillNtuple();

}

    
// ------------ method called once each job just before starting event loop  ------------
  
  void 
    SimpleNtpleTTBar::beginJob(const edm::EventSetup& iSetup)
{
 if(saveMu_)
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
  NtupleFactory_->AddFloat("muons_3DipSignificance");    
  NtupleFactory_->AddFloat("muons_tipSignificance");    
  NtupleFactory_->AddFloat("muons_lipSignificance");    
  NtupleFactory_->AddFloat("muons_track_d0");
  NtupleFactory_->AddFloat("muons_track_dz");
  NtupleFactory_->AddFloat("muons_track_d0err");
  NtupleFactory_->AddFloat("muons_track_dzerr");  
 }
  
 if(saveEle_)
 {
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
  NtupleFactory_->AddFloat("electrons_3DipSignificance");    
  NtupleFactory_->AddFloat("electrons_tipSignificance");    
  NtupleFactory_->AddFloat("electrons_lipSignificance");    
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
 }
  
 if(saveGenJet_)
 {
  NtupleFactory_->Add4V("genJets");         
 }    
  
 if(saveMet_)
 {
  NtupleFactory_->Add4V("met");         
  NtupleFactory_->Add4V("type1Met");         
  NtupleFactory_->Add4V("PFMet");         
 }
  
 if(saveGenMet_)
 {
  NtupleFactory_->Add4V("genMet");                
 }
  
 if(saveMC_)
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
}
   
   
// ------------ method called once each job just after ending the event loop  ------------
     
  void 
    SimpleNtpleTTBar::endJob() 
{
   
 NtupleFactory_->WriteNtuple();

}

