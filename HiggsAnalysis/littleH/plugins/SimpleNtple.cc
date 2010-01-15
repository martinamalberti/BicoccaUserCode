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
// $Id: SimpleNtple.cc,v 1.26 2010/01/15 00:04:45 dimatteo Exp $
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

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"


//--- objects ----
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/MuonReco/interface/MuonTrackLinks.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/BTauReco/interface/JetTag.h"

#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETFwd.h"

#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"

#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "RecoVertex/KinematicFitPrimitives/interface/ParticleMass.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicParticleFactoryFromTransientTrack.h"
#include "RecoVertex/KinematicFit/interface/KinematicParticleVertexFitter.h"
#include "RecoVertex/KinematicFit/interface/KinematicParticleFitter.h"

#include "DataFormats/HLTReco/interface/TriggerEventWithRefs.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"
#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Framework/interface/TriggerNames.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "DataFormats/Common/interface/ValueMap.h"

//---- utilities ----
#include "HiggsAnalysis/littleH/interface/MCDumper.h"


#include "HiggsAnalysis/littleH/plugins/SimpleNtple.h"

using namespace edm;

SimpleNtple::SimpleNtple(const ParameterSet& iConfig) :
  TracksTag_                (iConfig.getParameter<InputTag> ("TracksTag")),
  EleTag_                   (iConfig.getParameter<InputTag> ("EleTag")),
  MuTag_                    (iConfig.getParameter<InputTag> ("MuTag")),
  PrimaryVertexTag_         (iConfig.getParameter<InputTag> ("PrimaryVertexTag")),
  MCtruthTag_               (iConfig.getParameter<InputTag> ("MCtruthTag")),
  m_eleIDCut_LooseInputTag  (iConfig.getParameter<InputTag> ("eleIDCut_LooseInputTag")),
  m_eleIDCut_RLooseInputTag (iConfig.getParameter<InputTag> ("eleIDCut_RLooseInputTag")),
  m_eleIDCut_TightInputTag  (iConfig.getParameter<InputTag> ("eleIDCut_TightInputTag")),
  m_eleIDCut_RTightInputTag (iConfig.getParameter<InputTag> ("eleIDCut_RTightInputTag")),
  barrelClusterCollection_  (iConfig.getParameter<edm::InputTag> ("barrelClusterCollection")),
  endcapClusterCollection_  (iConfig.getParameter<edm::InputTag> ("endcapClusterCollection")),
  thetriggerEventTag_       (iConfig.getParameter<string>   ("triggerEventTag")),
  theHLTriggerResults_      (iConfig.getParameter<string>   ("triggerResultsTag")),
  the8e29ProcName_          (iConfig.getParameter<string>   ("HLTprocessName8e29")),
  the1e31ProcName_          (iConfig.getParameter<string>   ("HLTprocessName1e31")),
  saveVtx_                  (iConfig.getUntrackedParameter<bool> ("saveVtx", true)),
  saveMu_                   (iConfig.getUntrackedParameter<bool> ("saveMu", true)),
  saveTracks_               (iConfig.getUntrackedParameter<bool> ("saveTracks", true)),
  saveEle_                  (iConfig.getUntrackedParameter<bool> ("saveEle", true)),
  saveMC_                   (iConfig.getUntrackedParameter<bool> ("saveMC", true)),
  saveSC_                   (iConfig.getUntrackedParameter<bool> ("saveSC", true)),
  saveTrigger_              (iConfig.getUntrackedParameter<bool> ("saveTrigger", true)),
  eventType_                (iConfig.getUntrackedParameter<int> ("eventType",1)),
  verbosity_                (iConfig.getUntrackedParameter<bool> ("verbosity","False"))
{
  std::string treeName = iConfig.getUntrackedParameter<std::string> ("treeName","SimpleTree") ;
  Service<TFileService> fs ;
  outTree_ = fs->make<TTree> (treeName.c_str (), treeName.c_str ()) ; 
 
  NtupleFactory_ = new NtupleFactory (outTree_) ;
}


// --------------------------------------------------------------------


SimpleNtple::~SimpleNtple()
{
 delete NtupleFactory_;
}


// --------------------------------------------------------------------


void 
SimpleNtple::fillVtxInfo (const Event & iEvent, const EventSetup & iESetup) 
{
  Handle<reco::VertexCollection> privtxs;
  iEvent.getByLabel(PrimaryVertexTag_, privtxs);
   
  int nPriVtx;
  if(privtxs->size() < 100 ){ nPriVtx = privtxs->size(); }
  else {nPriVtx = 100;}
 
  for(int i=0; i< nPriVtx; i++)
    {     
      math::XYZVector myvect_XYZ ((*privtxs)[i].position().x(),(*privtxs)[i].position().y(),(*privtxs)[i].position().z());
      NtupleFactory_->Fill3V("priVtx_3vec",myvect_XYZ);
      NtupleFactory_->FillFloat("priVtx_xxE",(*privtxs)[i].covariance(0,0));
      NtupleFactory_->FillFloat("priVtx_yyE",(*privtxs)[i].covariance(1,1));
      NtupleFactory_->FillFloat("priVtx_zzE",(*privtxs)[i].covariance(2,2));
      NtupleFactory_->FillFloat("priVtx_yxE",(*privtxs)[i].covariance(1,0));
      NtupleFactory_->FillFloat("priVtx_zyE",(*privtxs)[i].covariance(2,1));
      NtupleFactory_->FillFloat("priVtx_zxE",(*privtxs)[i].covariance(2,0));
      NtupleFactory_->FillFloat("priVtx_chi2",(*privtxs)[i].chi2());
      NtupleFactory_->FillFloat("priVtx_ndof",(*privtxs)[i].ndof());
    }      
  return ;
}  

// --------------------------------------------------------------------


void 
SimpleNtple::fillMuInfo (const Event & iEvent, const EventSetup & iESetup) 
{
  Handle<MuonCollection> MuHandle;
  iEvent.getByLabel(MuTag_,MuHandle);
 
  MuonCollection theTrkMuons;
  MuonCollection theGlobalMuons;

  Reco_mu_glb_size = 0;

  for (MuonCollection::const_iterator nmuon = MuHandle->begin(); nmuon != MuHandle->end(); ++nmuon) {
    if (nmuon->isGlobalMuon()) {
      theGlobalMuons.push_back(*nmuon);
      theMuonTrkIndexes_.push_back(nmuon->innerTrack().index());
    }
    if (!(nmuon->isGlobalMuon()) && nmuon->isTrackerMuon()) {
      theTrkMuons.push_back(*nmuon);
      theMuonTrkIndexes_.push_back(nmuon->innerTrack().index());
    }
  }

  for (MuonCollection::const_iterator glbmuon = theGlobalMuons.begin();
       glbmuon != theGlobalMuons.end() ; 
       glbmuon++) {
    TrackRef glbTrack = glbmuon->globalTrack();
    TrackRef innTrack = glbmuon->innerTrack();

    NtupleFactory_->Fill4V("muons_glb_4mom", lorentzMomentum(*glbTrack)) ;
    NtupleFactory_->Fill4V("muons_glb_track4mom", lorentzMomentum(*innTrack)) ;

    NtupleFactory_->FillInt("muons_glb_charge",(glbmuon->charge()));
    NtupleFactory_->FillFloat("muons_tkIsoR03",(glbmuon->isolationR03()).sumPt);
    NtupleFactory_->FillFloat("muons_nTkIsoR03",(glbmuon->isolationR03()).nTracks);    
    NtupleFactory_->FillFloat("muons_emIsoR03",(glbmuon->isolationR03()).emEt);
    NtupleFactory_->FillFloat("muons_hadIsoR03",(glbmuon->isolationR03()).hadEt);
     
    NtupleFactory_->FillFloat("muons_glb_tkIsoR05",(glbmuon->isolationR05()).sumPt);
    NtupleFactory_->FillFloat("muons_glb_nTkIsoR05",(glbmuon->isolationR05()).nTracks);    
    NtupleFactory_->FillFloat("muons_glb_emIsoR05",(glbmuon->isolationR05()).emEt);
    NtupleFactory_->FillFloat("muons_glb_hadIsoR05",(glbmuon->isolationR05()).hadEt);

    NtupleFactory_->FillFloat("muons_glb_ptErr", glbTrack->ptError ());
    NtupleFactory_->FillFloat("muons_glb_phiErr", glbTrack->phiError ());
    NtupleFactory_->FillFloat("muons_glb_etaErr", glbTrack->etaError ());
    NtupleFactory_->FillFloat("muons_glb_d0", glbTrack->d0 ());
    NtupleFactory_->FillFloat("muons_glb_d0err", glbTrack->d0Error ());
    NtupleFactory_->FillFloat("muons_glb_dz", glbTrack->dz ());
    NtupleFactory_->FillFloat("muons_glb_dzerr", glbTrack->dzError ());
    NtupleFactory_->FillFloat("muons_glb_normchi", glbTrack->chi2()/glbTrack->ndof());

    vector<unsigned int> theHits = this->trackHits(*glbTrack);
    NtupleFactory_->FillFloat("muons_glb_nhitstrack", innTrack->numberOfValidHits());
    NtupleFactory_->FillFloat("muons_glb_nhitsDT", theHits.at(0));
    NtupleFactory_->FillFloat("muons_glb_nhitsCSC", theHits.at(1));
    NtupleFactory_->FillFloat("muons_glb_nhitsStrip", theHits.at(3));
    NtupleFactory_->FillFloat("muons_glb_nhitsPixB", theHits.at(4));
    NtupleFactory_->FillFloat("muons_glb_nhitsPixE", theHits.at(5));
    NtupleFactory_->FillFloat("muons_glb_nhitsPix1Hit", theHits.at(6));
    NtupleFactory_->FillFloat("muons_glb_nhitsPix1HitBE", theHits.at(7));

    Reco_mu_glb_size++;
  }

  Reco_mu_trk_size = 0;

  for (MuonCollection::const_iterator trkmuon = theTrkMuons.begin();
       trkmuon != theTrkMuons.end(); 
       trkmuon++) {
    TrackRef innTrack = trkmuon->innerTrack();

    NtupleFactory_->Fill4V("muons_trk_4mom", lorentzMomentum(*innTrack)) ;

    NtupleFactory_->FillInt("muons_trk_charge",(trkmuon->charge()));
     
    NtupleFactory_->FillFloat("muons_trk_ptErr", innTrack->ptError ());
    NtupleFactory_->FillFloat("muons_trk_phiErr", innTrack->phiError ());
    NtupleFactory_->FillFloat("muons_trk_etaErr", innTrack->etaError ());
    NtupleFactory_->FillFloat("muons_trk_d0", innTrack->d0 ());
    NtupleFactory_->FillFloat("muons_trk_d0err", innTrack->d0Error ());
    NtupleFactory_->FillFloat("muons_trk_dz", innTrack->dz ());
    NtupleFactory_->FillFloat("muons_trk_dzerr", innTrack->dzError ());
    NtupleFactory_->FillFloat("muons_trk_normchi", innTrack->chi2()/innTrack->ndof());

    vector<unsigned int> theHits = this->trackHits(*innTrack);
    NtupleFactory_->FillFloat("muons_trk_nhitstrack", innTrack->numberOfValidHits());
    NtupleFactory_->FillFloat("muons_trk_nhitsStrip", theHits.at(3));
    NtupleFactory_->FillFloat("muons_trk_nhitsPixB", theHits.at(4));
    NtupleFactory_->FillFloat("muons_trk_nhitsPixE", theHits.at(5));
    NtupleFactory_->FillFloat("muons_trk_nhitsPix1Hit", theHits.at(6));
    NtupleFactory_->FillFloat("muons_trk_nhitsPix1HitBE", theHits.at(7));

    // STANDARD SELECTORS
    int myWord = 0;
    if (muon::isGoodMuon(*trkmuon, muon::AllTrackerMuons))                   myWord += (int)pow(2.,0);
    if (muon::isGoodMuon(*trkmuon, muon::TrackerMuonArbitrated))             myWord += (int)pow(2.,1);
    if (muon::isGoodMuon(*trkmuon, muon::TMLastStationLoose))                myWord += (int)pow(2.,2);
    if (muon::isGoodMuon(*trkmuon, muon::TMLastStationTight))                myWord += (int)pow(2.,3);
    if (muon::isGoodMuon(*trkmuon, muon::TM2DCompatibilityLoose))            myWord += (int)pow(2.,4);
    if (muon::isGoodMuon(*trkmuon, muon::TM2DCompatibilityTight))            myWord += (int)pow(2.,5);
    if (muon::isGoodMuon(*trkmuon, muon::TMOneStationLoose))                 myWord += (int)pow(2.,6);
    if (muon::isGoodMuon(*trkmuon, muon::TMOneStationTight))                 myWord += (int)pow(2.,7);
    if (muon::isGoodMuon(*trkmuon, muon::TMLastStationOptimizedLowPtLoose))  myWord += (int)pow(2.,8);
    if (muon::isGoodMuon(*trkmuon, muon::TMLastStationOptimizedLowPtTight))  myWord += (int)pow(2.,9);

    NtupleFactory_->FillFloat("muons_trk_PIDmask", myWord);

    //isolation
    const MuonIsolation& muonIso = trkmuon->isolationR03() ; 

    NtupleFactory_->FillFloat("muons_trk_iso_sumPt", muonIso.sumPt);
    NtupleFactory_->FillFloat("muons_trk_iso_sumEcal", muonIso.emEt);
    NtupleFactory_->FillFloat("muons_trk_iso_sumHad", muonIso.hadEt);

    Reco_mu_trk_size++;
  }

  return;
}  


// --------------------------------------------------------------------


void 
SimpleNtple::fillTrackInfo (const Event & iEvent, const EventSetup & iESetup) 
{
  Handle<View<reco::Track> > TracksHandle ;
  iEvent.getByLabel (TracksTag_, TracksHandle) ;
 
  int k(0);
  for (View<reco::Track>::const_iterator tkIt = TracksHandle->begin (); 
       tkIt != TracksHandle->end (); ++tkIt ) 
    { 
      k++;
    
      bool storeThisOther = true;
      for (int j = 0; j<(int)theMuonTrkIndexes_.size(); ++j) 
        {
          if (k == theMuonTrkIndexes_.at(j)) 
            {
              storeThisOther = false;
              break;
            }
        }
      if (!storeThisOther) continue;
    
      NtupleFactory_->Fill3V("tracks_in",(*tkIt).innerMomentum ());  
      NtupleFactory_->Fill3V("tracks_out",(*tkIt).outerMomentum ());
      
      NtupleFactory_->FillFloat("tracks_d0",(*tkIt).d0());
      NtupleFactory_->FillFloat("tracks_dz",(*tkIt).dz());
      NtupleFactory_->FillFloat("tracks_d0err",(*tkIt).d0Error());
      NtupleFactory_->FillFloat("tracks_dzerr",(*tkIt).dzError());
    }
  return ; 
}

// --------------------------------------------------------------------


void 
SimpleNtple::fillEleInfo (const Event & iEvent, const EventSetup & iESetup) 
{
  Handle<View<reco::GsfElectron> > EleHandle ;
  iEvent.getByLabel (EleTag_,EleHandle);
 
  int nEle(0);
 
  if(EleHandle->size() < 30 ){ nEle = EleHandle->size(); }
  else {nEle = 30;}
  
  //PG get the electron ID collections
  std::vector<Handle<ValueMap<float> > > eleIdCutHandles(4) ;
  iEvent.getByLabel (m_eleIDCut_LooseInputTag, eleIdCutHandles[0]) ;
  iEvent.getByLabel (m_eleIDCut_RLooseInputTag, eleIdCutHandles[1]) ;
  iEvent.getByLabel (m_eleIDCut_TightInputTag, eleIdCutHandles[2]) ;
  iEvent.getByLabel (m_eleIDCut_RTightInputTag, eleIdCutHandles[3]) ;
 
  for (int i=0; i< nEle; i++)
    {     
      //Get Ele Ref
      Ref<View<reco::GsfElectron> > electronEdmRef(EleHandle,i);
      
      NtupleFactory_->Fill4V("electrons",(*EleHandle)[i].p4());
      NtupleFactory_->FillFloat("electrons_charge",((*EleHandle)[i].charge()));
      NtupleFactory_->FillFloat("electrons_tkIso", ((*EleHandle)[i].dr03TkSumPt()));
      NtupleFactory_->FillFloat("electrons_emIso", ((*EleHandle)[i].dr03EcalRecHitSumEt()));
      NtupleFactory_->FillFloat("electrons_hadIso",((*EleHandle)[i].dr03HcalDepth1TowerSumEt()));   
      
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
    }  
  return ;
}


// --------------------------------------------------------------------


void 
SimpleNtple::fillSCInfo (const Event & iEvent, const EventSetup & iESetup) 
{
  Handle<reco::SuperClusterCollection> bscHandle ;  
  iEvent.getByLabel (barrelClusterCollection_, bscHandle) ;
  //  if (!(bscHandle.isValid())) {// do something? }

  //PG loop on superclusters in the barrel
  for (reco::SuperClusterCollection::const_iterator iClus = bscHandle->begin () ; 
       iClus != bscHandle->end () ; 
       ++iClus) 
    {

      NtupleFactory_->FillFloat ("SC_Energy", iClus->energy ()) ;
      math::XYZVector dummy3V (iClus->x (), iClus->y (), iClus->z ()) ;
      NtupleFactory_->Fill3V ("SC_position", dummy3V) ;
    } //PG loop on superclusters in the barrel

  Handle<reco::SuperClusterCollection> escHandle ;  
  iEvent.getByLabel (endcapClusterCollection_, escHandle) ;

  //PG loop on superclusters in the endcaps
  for (reco::SuperClusterCollection::const_iterator iClus = escHandle->begin () ; 
       iClus != escHandle->end () ; 
       ++iClus) 
    {

      NtupleFactory_->FillFloat ("SC_Energy", iClus->energy ()) ;
      math::XYZVector dummy3V (iClus->x (), iClus->y (), iClus->z ()) ;
      NtupleFactory_->Fill3V ("SC_position", dummy3V) ;
    } //PG loop on superclusters in the endcaps

  return ;
}


// --------------------------------------------------------------------


void 
SimpleNtple::fillMCInfo (const Event & iEvent, const EventSetup & iESetup) 
{
  Handle<reco::GenParticleCollection> genParticles;
  iEvent.getByLabel(MCtruthTag_, genParticles);

  //  int eventType_ = 1; //---- 0 = signal      1 = background 
  //  bool verbosity_ = true; //---- true = loquacious     false = silence
  //  MCDumper mcAnalysis(genParticles, eventType_, verbosity_); //---- i "tau" mi fanno scrivere a schermo anche se NON è segnale
  //  bool isValid = mcAnalysis.isValid();

  vector<const Candidate *> toBeSaved ;
  //PG loop on gen particles
  for (size_t i = 0 ; i < genParticles->size () ; ++i) 
    {
      const int pid = abs (genParticles->at (i).pdgId ()) ;
      if ( (pid % 1000) / 10 == 55 || //PG bb mesons
	   (pid % 1000) / 10 == 44 || //PG cc mesons
	   pid == 11 ||               //PG electrons
	   pid == 13 ||               //PG muons
	   pid == 15                  //PG tau
	   )
	{
          toBeSaved.push_back (&genParticles->at (i)) ;
	}
    } //PG loop on gen particles 

  //PG loop on gen particles to be saved
  for (int iGen = 0; iGen < (int)toBeSaved.size () ; ++iGen) 
    {          
      NtupleFactory_->FillFloat ("MC_pdgID", toBeSaved.at (iGen)->pdgId ()) ;
      NtupleFactory_->Fill4V ("MC_particles4V", toBeSaved.at (iGen)->p4 ()) ;
      
      vector<const Candidate *>::const_iterator trovo =
        find (toBeSaved.begin (), toBeSaved.end (), toBeSaved.at (iGen)->mother ()) ;
      if (trovo != toBeSaved.end ()) 
	NtupleFactory_->FillInt ("MC_iMother", trovo - toBeSaved.begin ()) ;
      else 
	NtupleFactory_->FillInt ("MC_iMother", -1) ;

      int daughter[2] = {-1,-1} ;
      
      int daughters_num = toBeSaved.at (iGen)->numberOfDaughters () ;
      for (int j = 0 ; j < daughters_num ; ++j)
        {
          if(j > 1) continue ;
          vector<const Candidate *>::const_iterator trovo =
            find (toBeSaved.begin (), toBeSaved.end (), toBeSaved.at (iGen)->daughter (j)) ;
          if (trovo != toBeSaved.end ()) 
            {
              daughter[j] = trovo - toBeSaved.begin () ;
            }
        }
      NtupleFactory_->FillInt ("MC_iDau0", daughter[0]) ;
      NtupleFactory_->FillInt ("MC_iDau1", daughter[1]) ;
      
    } //PG loop on gen particles to be saved

  return ;
}

// --------------------------------------------------------------------

void 
SimpleNtple::fillTriggerInfo(const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
 using namespace trigger;
 Handle<TriggerResults> HLTR;
 iEvent.getByLabel(InputTag(theHLTriggerResults_,"",the8e29ProcName_), HLTR);
 if (HLTR.isValid()) {
  
  NtupleFactory_->FillInt ("HLTGlobal_wasrun",(int) HLTR->wasrun()) ;  
  NtupleFactory_->FillInt ("HLTGlobal_Decision",(int) HLTR->accept()) ;  
  NtupleFactory_->FillInt ("HLTGlobal_error",(int) HLTR->error()) ;  
 
//   HLTGlobal_wasrun=HLTR->wasrun();
//   HLTGlobal_Decision=HLTR->accept();
//   HLTGlobal_error=HLTR->error();
    
 
  int HLTBits_size=HLTR->size();
  for (int i=0; i<HLTBits_size && i<(int)Max_trig_size; i++) {
//    HLTBits_accept[i]=(int) HLTR->accept(hltBits[i]);
//    HLTBits_error[i] =(int) HLTR->error(hltBits[i]);
   NtupleFactory_->FillInt ("HLTBits_wasrun",(int) HLTR->wasrun(hltBits[i])) ;
   NtupleFactory_->FillInt ("HLTBits_accept",(int) HLTR->accept(hltBits[i])) ;
   NtupleFactory_->FillInt ("HLTBits_error",(int) HLTR->error(hltBits[i])) ;
  }
   
  Handle<TriggerEvent> trgEvent;
  bool hltF = true;
  try {
   iEvent.getByLabel(InputTag(thetriggerEventTag_,"",the8e29ProcName_), trgEvent);
  }
  catch (const cms::Exception& e) {
   hltF = false;
   cout<<"Error!! No TriggerEvent with label " << thetriggerEventTag_ << endl;
  }
  if ( hltF ) {
   const TriggerObjectCollection& TOC(trgEvent->getObjects());
 
   for ( int lvl = 1; lvl<2; lvl++ ) { 
    for ( int ipath = 0; ipath < HLTBits_size; ipath++) {
     const InputTag trigName = hltModules[lvl][ipath];
     size_type index = trgEvent->filterIndex(trigName);
     if ( index < trgEvent->sizeFilters() ) {
      const Keys& KEYS( trgEvent->filterKeys(index) );
      int muonsize = KEYS.size();
      int minNMuons = 1; if ( ipath>=3 ) minNMuons = 2;
      if (  muonsize < minNMuons && HLTR->accept(hltBits[ipath]) ) {  
       cout<<"Error!! Not enough HLT muons for "<<trigName.label()<<", but decision = "<<HLTR->accept(hltBits[ipath])<<endl;
      }
      for ( int hltm = 0; hltm < muonsize; hltm++ ) {
       size_type hltf = KEYS[hltm];
       const TriggerObject& TO(TOC[hltf]);
//        TLorentzVector a = lorentzTriObj(TO);

       if ( lvl==1 ) {
        if ( ipath==0 ) {
         NtupleFactory_->Fill4V ("HLT1Mu3_L3_4mom", TO.particle().p4());
         NtupleFactory_->FillInt("HLT1Mu3_L3_id", TO.id()) ;
        }
        if ( ipath==1 ) {
         NtupleFactory_->Fill4V ("HLT1Mu5_L3_4mom", TO.particle().p4());
         NtupleFactory_->FillInt("HLT1Mu5_L3_id", TO.id()) ;
        }
        if ( ipath==2 ) {
         NtupleFactory_->Fill4V ("HLT1Mu9_L3_4mom", TO.particle().p4());
         NtupleFactory_->FillInt("HLT1Mu9_L3_id", TO.id()) ;
        }
        if ( ipath==3 ) {
         NtupleFactory_->Fill4V ("HLT2Mu0_L3_4mom", TO.particle().p4());
         NtupleFactory_->FillInt("HLT2Mu0_L3_id", TO.id()) ;
        }
        if ( ipath==4 ) {
         NtupleFactory_->Fill4V ("HLT2Mu3_L3_4mom", TO.particle().p4());
         NtupleFactory_->FillInt("HLT2Mu3_L3_id", TO.id()) ;
        }
       }
      } 
     } 
    }
   }
  }
 }

 return;
}


// ------------ method called to for each event  ------------


void SimpleNtple::analyze(const Event& iEvent, const EventSetup& iSetup)
{
  if (saveVtx_)    fillVtxInfo (iEvent, iSetup) ;
  if (saveMu_)     fillMuInfo (iEvent, iSetup) ;    //PG fillMuInfo should be called
  if (saveTracks_) fillTrackInfo (iEvent, iSetup) ; //PG before fillTrackInfo !! 
  if (saveEle_)    fillEleInfo (iEvent, iSetup) ;
  if (saveMC_)     fillMCInfo (iEvent, iSetup) ;
  if (saveSC_)     fillSCInfo (iEvent, iSetup) ;
  if (saveTrigger_) fillTriggerInfo (iEvent, iSetup) ;
  // save the entry of the tree 
  NtupleFactory_->FillNtuple();

}

    
// ------------ method called once each job just before starting event loop  ------------
void SimpleNtple::beginJob(const EventSetup& iSetup)
{
  if (saveMu_)
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
      NtupleFactory_->AddFloat("muons_track_d0"); 
      NtupleFactory_->AddFloat("muons_track_dz"); 
      NtupleFactory_->AddFloat("muons_track_d0err"); 
      NtupleFactory_->AddFloat("muons_track_dzerr"); 
    }

  if (saveEle_)
    {
      NtupleFactory_->Add4V("electrons");
      NtupleFactory_->AddFloat("electrons_charge"); 
      NtupleFactory_->AddFloat("electrons_tkIso"); 
      NtupleFactory_->AddFloat("electrons_emIso"); 
      NtupleFactory_->AddFloat("electrons_hadIso"); 
      NtupleFactory_->AddFloat("electrons_IdLoose"); 
      NtupleFactory_->AddFloat("electrons_IdTight"); 
      NtupleFactory_->AddFloat("electrons_IdRobustLoose"); 
      NtupleFactory_->AddFloat("electrons_IdRobustTight"); 
      NtupleFactory_->AddFloat("electrons_track_d0");
      NtupleFactory_->AddFloat("electrons_track_dz");
      NtupleFactory_->AddFloat("electrons_track_d0err");
      NtupleFactory_->AddFloat("electrons_track_dzerr");
    }

  if (saveTracks_)
    {
      NtupleFactory_->Add3V("tracks_in");
      NtupleFactory_->Add3V("tracks_out");   
      NtupleFactory_->AddFloat("tracks_d0");
      NtupleFactory_->AddFloat("tracks_dz");   
      NtupleFactory_->AddFloat("tracks_d0err");
      NtupleFactory_->AddFloat("tracks_dzerr");  
    }

  if (saveVtx_)
    {
      NtupleFactory_->Add3V("priVtx_3vec");
      NtupleFactory_->AddFloat("priVtx_xxE");
      NtupleFactory_->AddFloat("priVtx_yyE");
      NtupleFactory_->AddFloat("priVtx_zzE");
      NtupleFactory_->AddFloat("priVtx_yxE");
      NtupleFactory_->AddFloat("priVtx_zyE");
      NtupleFactory_->AddFloat("priVtx_zxE");
      NtupleFactory_->AddFloat("priVtx_chi2");
      NtupleFactory_->AddFloat("priVtx_ndof");
    }

  //PG MC truth
  if (saveMC_)
    {
      NtupleFactory_->AddFloat ("MC_pdgID") ;
      NtupleFactory_->Add4V ("MC_particles4V") ;
      NtupleFactory_->AddInt ("MC_iMother") ;
      NtupleFactory_->AddInt ("MC_iDau0") ;
      NtupleFactory_->AddInt ("MC_iDau1") ;
    }

  //PG supercluster information
  if (saveSC_)
    {
      NtupleFactory_->AddFloat ("SC_Energy") ;
      NtupleFactory_->Add3V ("SC_position") ;
    }

  //Trigger Info
  if (saveTrigger_)
    {
      NtupleFactory_->AddInt ("HLTBits_wasrun") ;
      NtupleFactory_->AddInt ("HLTBits_accept") ;
      NtupleFactory_->AddInt ("HLTBits_error") ;
      NtupleFactory_->AddInt ("HLTGlobal_wasrun") ;  
      NtupleFactory_->AddInt ("HLTGlobal_Decision") ;  
      NtupleFactory_->AddInt ("HLTGlobal_error") ;  
      NtupleFactory_->Add4V ("HLT1Mu3_L3_4mom");
      NtupleFactory_->AddInt("HLT1Mu3_L3_id") ;
      NtupleFactory_->Add4V ("HLT1Mu5_L3_4mom");
      NtupleFactory_->AddInt("HLT1Mu5_L3_id") ;
      NtupleFactory_->Add4V ("HLT1Mu9_L3_4mom");
      NtupleFactory_->AddInt("HLT1Mu9_L3_id") ;
      NtupleFactory_->Add4V ("HLT2Mu0_L3_4mom");
      NtupleFactory_->AddInt("HLT2Mu0_L3_id") ;
      NtupleFactory_->Add4V ("HLT2Mu3_L3_4mom");
      NtupleFactory_->AddInt("HLT2Mu3_L3_id") ;
    }

  return ;
}
   
   
// ------------ method called once each job just after ending the event loop  ------------
     
void SimpleNtple::endJob()
{
  NtupleFactory_->WriteNtuple();
  return;
}

///////////////////////////////////////////////////////////////////////////////
// Returns Lorentz-vector of muon
//////////////////////////////////////////////////////////////////////////////
template <class T>
math::XYZTLorentzVector SimpleNtple::lorentzMomentum(const T& muon) const 
{
  const double preco = muon.p();
  const double ereco = sqrt(preco*preco + 0.011163613);

  return math::XYZTLorentzVector(muon.px(), muon.py(), muon.pz(), ereco);
}

/////////////////////////////////////////////////////////////////////
// Number of hits per muon/track
/////////////////////////////////////////////////////////////////////
vector<unsigned int> SimpleNtple::trackHits(const Track& tr) 
{
  vector<unsigned int> theTrackHits;
  const HitPattern& p = tr.hitPattern();

  theTrackHits.push_back(p.numberOfValidMuonDTHits()); 
  theTrackHits.push_back(p.numberOfValidMuonCSCHits());
  theTrackHits.push_back(p.numberOfValidMuonRPCHits());
  theTrackHits.push_back(p.numberOfValidStripHits()); 
  theTrackHits.push_back(p.numberOfValidPixelBarrelHits());
  theTrackHits.push_back(p.numberOfValidPixelEndcapHits());

  // do not loop over the hits of the track
  const uint32_t firsthit = p.getHitPattern(0);
    
  // if the hit is valid and in pixel barrel... etc. etc.
  if (p.validHitFilter(firsthit) && p.pixelBarrelHitFilter(firsthit)) {
    theTrackHits.push_back(p.getLayer(firsthit));
    theTrackHits.push_back(1);
  } else if (p.validHitFilter(firsthit) && p.pixelEndcapHitFilter(firsthit)) {
    theTrackHits.push_back(p.getLayer(firsthit));
    theTrackHits.push_back(2);
  } else {
    theTrackHits.push_back(p.getLayer(firsthit));
    theTrackHits.push_back(0);
  }

  return theTrackHits;
}
