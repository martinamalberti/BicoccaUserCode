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
// $Id: SimpleNtple.cc,v 1.55 2010/02/08 08:18:08 dimatteo Exp $
//
//

#include "HiggsAnalysis/littleH/plugins/SimpleNtple.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/EventPrincipal.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DataFormats/Common/interface/AssociationMap.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "RecoTracker/Record/interface/TrackerRecoGeometryRecord.h"

#include "TrackingTools/GeomPropagators/interface/AnalyticalPropagator.h"
#include "TrackingTools/GeomPropagators/interface/Propagator.h"
#include "TrackingTools/GsfTools/interface/GsfPropagatorAdapter.h"
#include "TrackingTools/GsfTools/interface/MultiTrajectoryStateTransform.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/PatternTools/interface/TransverseImpactPointExtrapolator.h"

using namespace edm;

const float muMass_  = 0.105658;
const float eleMass_ = 0.000511;

SimpleNtple::SimpleNtple(const ParameterSet& iConfig) :
  gsfPSet                   (iConfig.getParameter<ParameterSet>("GsfParameters")),
  TracksTag_                (iConfig.getParameter<InputTag> ("TracksTag")),
  EleTag_                   (iConfig.getParameter<InputTag> ("EleTag")),
  MuTag_                    (iConfig.getParameter<InputTag> ("MuTag")),
  PrimaryVertexTag_         (iConfig.getParameter<InputTag> ("PrimaryVertexTag")),
  MCtruthTag_               (iConfig.getParameter<InputTag> ("MCtruthTag")),
  m_eleIDCut_LooseInputTag  (iConfig.getParameter<InputTag> ("eleIDCut_LooseInputTag")),
  m_eleIDCut_RLooseInputTag (iConfig.getParameter<InputTag> ("eleIDCut_RLooseInputTag")),
  m_eleIDCut_TightInputTag  (iConfig.getParameter<InputTag> ("eleIDCut_TightInputTag")),
  m_eleIDCut_RTightInputTag (iConfig.getParameter<InputTag> ("eleIDCut_RTightInputTag")),
  barrelClusterCollection_  (iConfig.getParameter<InputTag> ("barrelClusterCollection")),
  endcapClusterCollection_  (iConfig.getParameter<InputTag> ("endcapClusterCollection")),
  beamSpotTag_              (iConfig.getParameter<InputTag> ("beamSpotTag")),
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
  saveBeamSpot_             (iConfig.getUntrackedParameter<bool> ("saveBeamSpot", true)),
  saveOniaCand_             (iConfig.getUntrackedParameter<bool> ("saveOniaCand", true)),
  theStoreWSOnia            (iConfig.getUntrackedParameter<bool> ("storeWSOnia", true)), 
  theBeamSpotFlag           (iConfig.getUntrackedParameter<bool> ("beamSpotFlag", true)),
  theOniaMaxCat             (iConfig.getUntrackedParameter<int> ("oniaMaxCat", 1)),
  Chi2OniaVtxCut_           (iConfig.getUntrackedParameter<double> ("Chi2OniaVtxCut", 0.001)),
  OniaMassCut_              (iConfig.getUntrackedParameter<double> ("OniaMassCut", 3.2)),
  Onia3DipCut_              (iConfig.getUntrackedParameter<double> ("Onia3DipCut", 5.)),
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
    TVector3 myvect3 ((*privtxs)[i].position().x(),(*privtxs)[i].position().y(),(*privtxs)[i].position().z());
    NtupleFactory_->Fill3TV("priVtx_3vec",myvect3);
    NtupleFactory_->FillFloat("priVtx_xxE",(*privtxs)[i].covariance(0,0));
    NtupleFactory_->FillFloat("priVtx_yyE",(*privtxs)[i].covariance(1,1));
    NtupleFactory_->FillFloat("priVtx_zzE",(*privtxs)[i].covariance(2,2));
    NtupleFactory_->FillFloat("priVtx_yxE",(*privtxs)[i].covariance(1,0));
    NtupleFactory_->FillFloat("priVtx_zyE",(*privtxs)[i].covariance(2,1));
    NtupleFactory_->FillFloat("priVtx_zxE",(*privtxs)[i].covariance(2,0));
    NtupleFactory_->FillFloat("priVtx_chi2",(*privtxs)[i].chi2());
    NtupleFactory_->FillFloat("priVtx_ndof",(*privtxs)[i].ndof());
  }      
  return;
}  

// --------------------------------------------------------------------


void SimpleNtple::fillMuInfo (const Event & iEvent, const EventSetup & iESetup) 
{
  Handle<MuonCollection> MuHandle;
  iEvent.getByLabel(MuTag_,MuHandle);
 
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

    NtupleFactory_->Fill4TV("muons_glb_4mom", lorentzMomentumMu(*glbTrack)) ;
    NtupleFactory_->Fill4TV("muons_glb_track4mom", lorentzMomentumMu(*innTrack)) ;

    NtupleFactory_->FillInt("muons_glb_charge",(glbmuon->charge()));
    NtupleFactory_->FillFloat("muons_glb_tkIsoR03",(glbmuon->isolationR03()).sumPt);
    NtupleFactory_->FillFloat("muons_glb_nTkIsoR03",(glbmuon->isolationR03()).nTracks);    
    NtupleFactory_->FillFloat("muons_glb_emIsoR03",(glbmuon->isolationR03()).emEt);
    NtupleFactory_->FillFloat("muons_glb_hadIsoR03",(glbmuon->isolationR03()).hadEt);
     
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
    NtupleFactory_->FillFloat("muons_glb_normChi2", glbTrack->chi2()/glbTrack->ndof());

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

    NtupleFactory_->Fill4TV("muons_trk_4mom", lorentzMomentumMu(*innTrack)) ;

    NtupleFactory_->FillInt("muons_trk_charge",(trkmuon->charge()));
     
    NtupleFactory_->FillFloat("muons_trk_ptErr", innTrack->ptError ());
    NtupleFactory_->FillFloat("muons_trk_phiErr", innTrack->phiError ());
    NtupleFactory_->FillFloat("muons_trk_etaErr", innTrack->etaError ());
    NtupleFactory_->FillFloat("muons_trk_d0", innTrack->d0 ());
    NtupleFactory_->FillFloat("muons_trk_d0err", innTrack->d0Error ());
    NtupleFactory_->FillFloat("muons_trk_dz", innTrack->dz ());
    NtupleFactory_->FillFloat("muons_trk_dzerr", innTrack->dzError ());
    NtupleFactory_->FillFloat("muons_trk_normChi2", innTrack->chi2()/innTrack->ndof());

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

    NtupleFactory_->FillFloat("muons_trk_tkIsoR03", muonIso.sumPt);
    NtupleFactory_->FillFloat("muons_trk_nTkIsoR03",muonIso.nTracks);    
    NtupleFactory_->FillFloat("muons_trk_emIsoR03", muonIso.emEt);
    NtupleFactory_->FillFloat("muons_trk_hadIsoR03", muonIso.hadEt);

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
    
      TVector3 myvect3 ((*tkIt).innerMomentum().x(), (*tkIt).innerMomentum().y(), (*tkIt).innerMomentum().z());
      NtupleFactory_->Fill3TV("tracks_in",myvect3);  
      myvect3.SetXYZ ((*tkIt).innerMomentum().x(), (*tkIt).innerMomentum().y(), (*tkIt).innerMomentum().z());
      NtupleFactory_->Fill3TV("tracks_out",myvect3);
      
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
    theElectrons.push_back((*EleHandle)[i]);
      //Get Ele Ref
    Ref<View<reco::GsfElectron> > electronEdmRef(EleHandle,i);

    TLorentzVector myvec4 ((*EleHandle)[i].px(), (*EleHandle)[i].py(), (*EleHandle)[i].pz(), (*EleHandle)[i].energy());      
    NtupleFactory_->Fill4TV("electrons",myvec4);
    NtupleFactory_->FillInt("electrons_charge",((*EleHandle)[i].charge()));
    NtupleFactory_->FillFloat("electrons_tkIso", ((*EleHandle)[i].dr03TkSumPt()));
    NtupleFactory_->FillFloat("electrons_emIso", ((*EleHandle)[i].dr03EcalRecHitSumEt()));
    NtupleFactory_->FillFloat("electrons_hadIso",((*EleHandle)[i].dr03HcalDepth1TowerSumEt()));
       
      // electron ID variable from mva (tracker driven electrons)
    NtupleFactory_->FillFloat("electrons_IdBDT",((*EleHandle)[i].mva()));
      
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
    
    int myWord = 0;
    if ((*EleHandle)[i].isTrackerDriven ()) myWord += (int)pow(2.,0);
    if ((*EleHandle)[i].isEcalDriven ())    myWord += (int)pow(2.,1);

    NtupleFactory_->FillFloat("electrons_flag_mask", myWord);

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
    TVector3 dummy3V (iClus->x (), iClus->y (), iClus->z ()) ;
    NtupleFactory_->Fill3TV ("SC_position", dummy3V) ;
  } //PG loop on superclusters in the barrel

  Handle<reco::SuperClusterCollection> escHandle ;  
  iEvent.getByLabel (endcapClusterCollection_, escHandle) ;

  //PG loop on superclusters in the endcaps
  for (reco::SuperClusterCollection::const_iterator iClus = escHandle->begin () ; 
       iClus != escHandle->end () ; 
       ++iClus) 
  {

    NtupleFactory_->FillFloat ("SC_Energy", iClus->energy ()) ;
    TVector3 dummy3V (iClus->x (), iClus->y (), iClus->z ()) ;
    NtupleFactory_->Fill3TV ("SC_position", dummy3V) ;
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
    TLorentzVector myvec4(toBeSaved.at (iGen)->px(),toBeSaved.at (iGen)->py(),toBeSaved.at (iGen)->pz(),toBeSaved.at (iGen)->energy());
    NtupleFactory_->Fill4TV ("MC_particles4V", myvec4) ;
      
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
    SimpleNtple::fillTriggerInfo(const Event & iEvent, const EventSetup & iESetup) 
{
  using namespace trigger;
  Handle<TriggerResults> HLTR;
  iEvent.getByLabel(InputTag(theHLTriggerResults_,"",the8e29ProcName_), HLTR);
  if (HLTR.isValid()) 
  {
  
    NtupleFactory_->FillInt ("HLTGlobal_wasrun",(int) HLTR->wasrun()) ;  
    NtupleFactory_->FillInt ("HLTGlobal_Decision",(int) HLTR->accept()) ;  
    NtupleFactory_->FillInt ("HLTGlobal_error",(int) HLTR->error()) ;  
  
  //HLT
    int HLTBits_size=HLTR->size();
    for (int i=0; i<HLTBits_size && i<(int)Max_HLT_size; i++) 
    {   
      NtupleFactory_->FillInt ("HLTBits_wasrun",(int) HLTR->wasrun(hltBits[i])) ;
      NtupleFactory_->FillInt ("HLTBits_accept",(int) HLTR->accept(hltBits[i])) ;
      NtupleFactory_->FillInt ("HLTBits_error",(int) HLTR->error(hltBits[i])) ;
    }
  
  //Level-1
    int L1TBits_size=HLTR->size();
    for (int i=0; i<L1TBits_size && i<(int)Max_L1T_size; i++) 
    {   
      NtupleFactory_->FillInt ("L1TBits_wasrun",(int) HLTR->wasrun(l1tBits[i])) ;
      NtupleFactory_->FillInt ("L1TBits_accept",(int) HLTR->accept(l1tBits[i])) ;
      NtupleFactory_->FillInt ("L1TBits_error",(int) HLTR->error(l1tBits[i])) ;
    }
   
  }

  return;
}

// --------------------------------------------------------------------

void 
    SimpleNtple::fillBeamSpotInfo(const Event & iEvent, const EventSetup & iESetup) 
{

  Handle<reco::BeamSpot> recoBeamSpotHandle;
  iEvent.getByLabel(beamSpotTag_ ,recoBeamSpotHandle);
  reco::BeamSpot bs = *recoBeamSpotHandle; 

  TVector3 myvect3 (bs.x0(),bs.y0(),bs.z0());
  NtupleFactory_->Fill3TV("beamSpot_3vec",myvect3);

  NtupleFactory_->FillFloat("beamSpot_xxE",bs.covariance(0,0));
  NtupleFactory_->FillFloat("beamSpot_yyE",bs.covariance(1,1));
  NtupleFactory_->FillFloat("beamSpot_zzE",bs.covariance(2,2));
  NtupleFactory_->FillFloat("beamSpot_yxE",bs.covariance(1,0));
  NtupleFactory_->FillFloat("beamSpot_zyE",bs.covariance(2,1));
  NtupleFactory_->FillFloat("beamSpot_zxE",bs.covariance(2,0));
 
  return;
}

// --------------------------------------------------------------------

// Onia candidate block
void 
    SimpleNtple::findOniaCategories(const Event &iEvent, const EventSetup & iESetup) 
{
  
  QQ_size=0;

  Handle<reco::VertexCollection> privtxs;
  iEvent.getByLabel(PrimaryVertexTag_, privtxs);
  VertexCollection::const_iterator privtx;

  Handle<reco::BeamSpot> recoBeamSpotHandle;
  iEvent.getByLabel(beamSpotTag_ ,recoBeamSpotHandle);
  reco::BeamSpot bs = *recoBeamSpotHandle;

  reco::Vertex PV;
  if ( privtxs->begin() != privtxs->end() ) 
  {
    // select the primary vertex as the one
    // with higest sum of (pt)^2 of tracks
    PrimaryVertexSorter PVSorter;
    std::vector<reco::Vertex> sortedVertices = PVSorter.sortedList( *(privtxs.product()) );
    PV = sortedVertices.front();
  }
  else 
  {
    cout << " No PV found! " << endl ;
  }
  
  MuonCollection::const_iterator nmuon1;
  MuonCollection::const_iterator nmuon2;
  MuonCollection::const_iterator tmuon1;
  MuonCollection::const_iterator tmuon2;
  reco::GsfElectronCollection::const_iterator nele1 ;
  reco::GsfElectronCollection::const_iterator nele2 ;
  
  // int oniacato;
  reco::TrackRef muon1;
  reco::TrackRef muon2;
  int m1(0);
  int m2g(0);
  int m2t(0);

  reco::GsfTrackRef ele1;
  reco::GsfTrackRef ele2;
  int e1(0);
  int e2(0);
  
  //Electrons
  for ( nele1 = theElectrons.begin(); nele1 != theElectrons.end(); nele1++) 
  {
    for( nele2 = nele1+1; nele2 != theElectrons.end()&&QQ_size<3000; nele2++ ) 
    {
      ele1 = nele1->gsfTrack();    
      ele2 = nele2->gsfTrack(); 
      e2++;
      fillOnia2EleEleTracks(ele1, e1, ele2, e1+e2, PV);
    }
    e1++;
  }
  
  //Muons
  for ( nmuon1 = theGlobalMuons.begin(); nmuon1 != theGlobalMuons.end(); nmuon1++) 
  {
    for( nmuon2 = nmuon1+1; nmuon2 != theGlobalMuons.end()&&QQ_size<3000; nmuon2++ ) 
    {
      muon1 = nmuon1->innerTrack();    
      muon2 = nmuon2->innerTrack(); 
      m2g++;
      if (theOniaMaxCat >= 0) fillOnia2MuMuTracks(muon1, m1, muon2, m1+m2g, PV, 1);
    }
     
    for( tmuon2 = theTrkMuons.begin(); tmuon2 != theTrkMuons.end()&&QQ_size<3000; tmuon2++ ) 
    {
      muon1 = nmuon1->innerTrack();    
      muon2 = tmuon2->innerTrack(); 
      if (theOniaMaxCat >= 1) fillOnia2MuMuTracks(muon1, m1, muon2, m2t, PV, 2);
      m2t++;
    }
    m1++;
  }
  m1=0; m2t=0; 
  for ( tmuon1 = theTrkMuons.begin(); tmuon1 != theTrkMuons.end(); tmuon1++) 
  {
    for( tmuon2 = tmuon1+1; tmuon2 != theTrkMuons.end()&&QQ_size<3000; tmuon2++ ) 
    {
      muon1 = tmuon1->innerTrack();    
      muon2 = tmuon2->innerTrack(); 
      m2t++; 
      if (theOniaMaxCat >= 2) fillOnia2MuMuTracks(muon1, m1, muon2, m2t+m1, PV, 3);
    }
    m1++;
  }
    
  NtupleFactory_->FillInt("QQ_size",QQ_size);
  return;
}


// --------------------------------------------------------------------

//Fill Onia2MuMu category
void 
    SimpleNtple::fillOnia2MuMuTracks(TrackRef lep1, int l1, TrackRef lep2, int l2, reco::Vertex &PV, const int oniacato) 
{ 
  if (oniacato < 0) return;
  
  TransientTrack ttkp1   = (*theB).build(&(*lep1));
  TransientTrack ttkp2   = (*theB).build(&(*lep2));

  vector<TransientTrack> t_tks;
  t_tks.push_back(ttkp1);
  t_tks.push_back(ttkp2);

  KalmanVertexFitter kvf;
  TransientVertex tv = kvf.vertex(t_tks);
    
  if ( ! tv.isValid() ) return;
//   {
  GlobalPoint v = tv.position();
  GlobalError err = tv.positionError();
 
  GlobalPoint  PVposition(PV.position().x(), PV.position().y(), PV.position().z()) ;
  GlobalVector DistVector = v - PVposition; 
 
  TLorentzVector lp1=lorentzMomentumLep(*lep1, muMass_);
  TLorentzVector lp2=lorentzMomentumLep(*lep2, muMass_);
  TLorentzVector onia = lp1 + lp2;
   
  if ( TMath::Prob(tv.totalChiSquared(), (int)tv.degreesOfFreedom()) < Chi2OniaVtxCut_ ) return;//Loose Cut on Onia Vertex Chi2
  if ( onia.M() < OniaMassCut_ ) return;//Cut on Onia invariant mass
  if ( DistVector.mag() > Onia3DipCut_ ) return;//Cut on Onia 3Dip

  NtupleFactory_->FillInt("QQ_VtxIsVal",1);

  int QQ_sign = 0;
  if ( lep1->charge() == lep2->charge() ) 
  {
    if (theStoreWSOnia) {
      if (lep1->charge() == 1) {QQ_sign=1;}
      else {QQ_sign=-1;}
    }
    else return;
  }
  NtupleFactory_->FillInt("QQ_sign",QQ_sign);
    
  NtupleFactory_->FillInt("QQ_type",oniacato);
  NtupleFactory_->FillFloat("QQ_DeltaR",deltaR(lp1, lp2));
  NtupleFactory_->FillFloat("QQ_s",pow((lep1->d0()/lep1->d0Error()),2)+pow((lep2->d0()/lep2->d0Error()),2));
  TLorentzVector my4mom (onia.Px(),onia.Py(),onia.Pz(),onia.E()) ;
  NtupleFactory_->Fill4TV("QQ_4mom",my4mom);

  if ( lep1->charge() == 1 ) 
  {
    NtupleFactory_->FillInt("QQ_leppl",l1);
    NtupleFactory_->FillInt("QQ_lepmi",l2);
    NtupleFactory_->FillFloat("QQ_cosTheta",cos(GetTheta(lp1, lp2)));
  }
  else {
    NtupleFactory_->FillInt("QQ_leppl",l2);
    NtupleFactory_->FillInt("QQ_lepmi",l1);
    NtupleFactory_->FillFloat("QQ_cosTheta",cos(GetTheta(lp2, lp1)));
  }
  if (oniacato == 2 || lp1.Perp() > lp2.Perp()) 
  {   
    NtupleFactory_->FillInt("QQ_lephpt",l1);
    NtupleFactory_->FillInt("QQ_leplpt",l2);
  } else 
  {
    NtupleFactory_->FillInt("QQ_leplpt",l1);
    NtupleFactory_->FillInt("QQ_lephpt",l2);
  }
           
  TVector3 myvect3 (v.x(),v.y(),v.z());
  NtupleFactory_->Fill3TV("QQ_Vtx",myvect3);

  NtupleFactory_->FillFloat("QQ_VxxE",err.cxx());
  NtupleFactory_->FillFloat("QQ_VyyE",err.cyy());
  NtupleFactory_->FillFloat("QQ_VzzE",err.czz());
  NtupleFactory_->FillFloat("QQ_VyxE",err.cyx());
  NtupleFactory_->FillFloat("QQ_VzyE",err.czy());
  NtupleFactory_->FillFloat("QQ_VzxE",err.czx());
      
  NtupleFactory_->FillFloat("QQ_lxy",v.perp());
  NtupleFactory_->FillFloat("QQ_lxyErr",err.rerr(v));
  NtupleFactory_->FillFloat("QQ_normChi2",tv.normalisedChiSquared());
  NtupleFactory_->FillFloat("QQ_probChi2",TMath::Prob(tv.totalChiSquared(), (int)tv.degreesOfFreedom()));
    
  TVector3 pperp(onia.Px(), onia.Py(), 0);
  TVector3 vperp1(v.x(), v.y(), 0);
  TVector3 vperp2(PV.position().x(), PV.position().y(), 0. );
  TVector3 vperp = vperp1 - vperp2;
  double cosAlpha = vperp.Dot(pperp)/(vperp.Perp()*pperp.Perp());
  NtupleFactory_->FillFloat("QQ_cosAlpha",cosAlpha);
     
  //COMPUTE T.IP, L.IP and 3D IP
  
  AlgebraicSymMatrix33 PVerrMat;
  PVerrMat(0,0) = PV.covariance(0,0) ; 
  PVerrMat(1,1) = PV.covariance(1,1);
  PVerrMat(2,2) = PV.covariance(2,2);
  PVerrMat(0,1) = PVerrMat(1,0) = PV.covariance(1,0); 
  PVerrMat(0,2) = PVerrMat(2,0) = PV.covariance(2,0); 
  PVerrMat(1,2) = PVerrMat(2,1) = PV.covariance(2,1); 

  AlgebraicSymMatrix33 OniaVerrMat;
  OniaVerrMat(0,0) = err.cxx() ; 
  OniaVerrMat(1,1) = err.cyy();
  OniaVerrMat(2,2) = err.czz();
  OniaVerrMat(0,1) = OniaVerrMat(1,0) = err.cyx(); 
  OniaVerrMat(0,2) = OniaVerrMat(2,0) = err.czx(); 
  OniaVerrMat(1,2) = OniaVerrMat(2,1) = err.czy();
  
  // total error matrix
  AlgebraicSymMatrix33 totErrMat = PVerrMat + OniaVerrMat;
  
  AlgebraicVector3 mjacobian3dV;
  mjacobian3dV[0] = 1.;	
  mjacobian3dV[1] = 1.;
  mjacobian3dV[2] = 1.;
        
  AlgebraicVector2 mjacobianTranV;
  mjacobianTranV[0] = DistVector.x()/DistVector.perp();	
  mjacobianTranV[1] = DistVector.y()/DistVector.perp();
        
  AlgebraicVector1 mjacobianLongV;
  mjacobianLongV[0] = 1.;	
   
  //LIP,TIP,3DIP
  float QQ_Tip = DistVector.perp(); // positive by definition
  float QQ_Lip = DistVector.z(); // signed by definition
  float QQ_3Dip = DistVector.mag(); // positive by definition
  
  NtupleFactory_->FillFloat("QQ_Tip",QQ_Tip);
  NtupleFactory_->FillFloat("QQ_Lip",QQ_Lip);
  NtupleFactory_->FillFloat("QQ_3Dip",QQ_3Dip);
  	
  //LIP,TIP,3DIP errors
  float QQ_errTip = sqrt(ROOT::Math::Similarity(totErrMat.Sub<AlgebraicSymMatrix22>(0,0), mjacobianTranV));
  float QQ_errLip = sqrt(ROOT::Math::Similarity(totErrMat.Sub<AlgebraicSymMatrix11>(2,2), mjacobianLongV));
  float QQ_err3Dip = sqrt(ROOT::Math::Similarity(totErrMat, mjacobian3dV));

  NtupleFactory_->FillFloat("QQ_errTip",QQ_errTip);
  NtupleFactory_->FillFloat("QQ_errLip",QQ_errLip);
  NtupleFactory_->FillFloat("QQ_err3Dip",QQ_err3Dip);
  
  //LIP,TIP,3DIP significances

  NtupleFactory_->FillFloat("QQ_STip",QQ_Tip/QQ_errTip);
  NtupleFactory_->FillFloat("QQ_SLip",QQ_Lip/QQ_errLip);
  NtupleFactory_->FillFloat("QQ_S3Dip",QQ_3Dip/QQ_err3Dip);
  
  QQ_size++;
  return;
}

// --------------------------------------------------------------------

//Fill Onia2EleEle category
void SimpleNtple::fillOnia2EleEleTracks(GsfTrackRef lep1, int l1, GsfTrackRef lep2, int l2, reco::Vertex &PV) 
{
  TransientTrack ttkp1   = (*theB).build(&(*lep1));
  TransientTrack ttkp2   = (*theB).build(&(*lep2));

  vector<TransientTrack> t_tks;
  t_tks.push_back(ttkp1);
  t_tks.push_back(ttkp2);
  
  //   // Gaussian Sum Filter Algorithm 
  KalmanVertexFitter kvf;
  TransientVertex tv = kvf.vertex(t_tks);
//   AdaptiveGsfVertexFitter gsf(gsfPSet); 
//   GsfVertexFitter gsf(gsfPSet);
//   TransientVertex tv = gsf.vertex(t_tks);
    
  if ( ! tv.isValid() ) return;
  //   {
  GlobalPoint v = tv.position();
  GlobalError err = tv.positionError();
  
  GlobalPoint  PVposition(PV.position().x(), PV.position().y(), PV.position().z()) ;
  GlobalVector DistVector = v - PVposition; 
  
  TLorentzVector lp1=lorentzMomentumLep(*lep1, eleMass_);
  TLorentzVector lp2=lorentzMomentumLep(*lep2, eleMass_);
  TLorentzVector onia = lp1 + lp2;
    
  if ( TMath::Prob(tv.totalChiSquared(), (int)tv.degreesOfFreedom()) < Chi2OniaVtxCut_ ) return;//Loose Cut on Onia Vertex Chi2
  if ( onia.M() < OniaMassCut_ ) return;//Cut on Onia invariant mass
  if ( DistVector.mag() > Onia3DipCut_ ) return;//Cut on Onia 3Dip

  NtupleFactory_->FillInt("QQ_VtxIsVal",1);
     
  int QQ_sign = 0;
  if ( lep1->charge() == lep2->charge() ) 
    {
      if (theStoreWSOnia) {
	if (lep1->charge() == 1) {QQ_sign=1;}
	else {QQ_sign=-1;}
      }
      else return;
    }
  
  NtupleFactory_->FillInt("QQ_sign",QQ_sign);
  
  NtupleFactory_->FillInt("QQ_type",0);
  NtupleFactory_->FillFloat("QQ_DeltaR",deltaR(lp1, lp2));
  NtupleFactory_->FillFloat("QQ_s",pow((lep1->d0()/lep1->d0Error()),2)+pow((lep2->d0()/lep2->d0Error()),2));  
  TLorentzVector my4mom (onia.Px(),onia.Py(),onia.Pz(),onia.E()) ;
  NtupleFactory_->Fill4TV("QQ_4mom",my4mom);

  if ( lep1->charge() == 1 ) 
    {
      NtupleFactory_->FillInt("QQ_leppl",l1);
      NtupleFactory_->FillInt("QQ_lepmi",l2);
      NtupleFactory_->FillFloat("QQ_cosTheta",cos(GetTheta(lp1, lp2)));
    }
  else {
    NtupleFactory_->FillInt("QQ_leppl",l2);
    NtupleFactory_->FillInt("QQ_lepmi",l1);
    NtupleFactory_->FillFloat("QQ_cosTheta",cos(GetTheta(lp2, lp1)));
  }
  if (lp1.Perp() > lp2.Perp()) 
    {   
      NtupleFactory_->FillInt("QQ_lephpt",l1);
      NtupleFactory_->FillInt("QQ_leplpt",l2);
    } else 
      {
	NtupleFactory_->FillInt("QQ_leplpt",l1);
	NtupleFactory_->FillInt("QQ_lephpt",l2);
      }
      
  TVector3 myvect3 (v.x(),v.y(),v.z());
  NtupleFactory_->Fill3TV("QQ_Vtx",myvect3);

  NtupleFactory_->FillFloat("QQ_VxxE",err.cxx());
  NtupleFactory_->FillFloat("QQ_VyyE",err.cyy());
  NtupleFactory_->FillFloat("QQ_VzzE",err.czz());
  NtupleFactory_->FillFloat("QQ_VyxE",err.cyx());
  NtupleFactory_->FillFloat("QQ_VzyE",err.czy());
  NtupleFactory_->FillFloat("QQ_VzxE",err.czx());
      
  NtupleFactory_->FillFloat("QQ_lxy",v.perp());
  NtupleFactory_->FillFloat("QQ_lxyErr",err.rerr(v));
  NtupleFactory_->FillFloat("QQ_normChi2",tv.normalisedChiSquared());
  NtupleFactory_->FillFloat("QQ_probChi2",TMath::Prob(tv.totalChiSquared(), (int)tv.degreesOfFreedom()));
      
  TVector3 pperp(onia.Px(), onia.Py(), 0);
  TVector3 vperp1(v.x(), v.y(), 0);
  TVector3 vperp2(PV.position().x(), PV.position().y(), 0);
  TVector3 vperp = vperp1 - vperp2;
  double cosAlpha = vperp.Dot(pperp)/(vperp.Perp()*pperp.Perp());
  NtupleFactory_->FillFloat("QQ_cosAlpha",cosAlpha);
 
  //COMPUTE T.IP, L.IP and 3D IP
      
  AlgebraicSymMatrix33 PVerrMat;
  PVerrMat(0,0) = PV.covariance(0,0) ; 
  PVerrMat(1,1) = PV.covariance(1,1);
  PVerrMat(2,2) = PV.covariance(2,2);
  PVerrMat(0,1) = PVerrMat(1,0) = PV.covariance(1,0); 
  PVerrMat(0,2) = PVerrMat(2,0) = PV.covariance(2,0); 
  PVerrMat(1,2) = PVerrMat(2,1) = PV.covariance(2,1); 

  AlgebraicSymMatrix33 OniaVerrMat;
  OniaVerrMat(0,0) = err.cxx() ; 
  OniaVerrMat(1,1) = err.cyy();
  OniaVerrMat(2,2) = err.czz();
  OniaVerrMat(0,1) = OniaVerrMat(1,0) = err.cyx(); 
  OniaVerrMat(0,2) = OniaVerrMat(2,0) = err.czx(); 
  OniaVerrMat(1,2) = OniaVerrMat(2,1) = err.czy();
  
  // total error matrix
  AlgebraicSymMatrix33 totErrMat = PVerrMat + OniaVerrMat;
  
  AlgebraicVector3 mjacobian3dV;
  mjacobian3dV[0] = 1.;	
  mjacobian3dV[1] = 1.;
  mjacobian3dV[2] = 1.;
        
  AlgebraicVector2 mjacobianTranV;
  mjacobianTranV[0] = DistVector.x()/DistVector.perp();	
  mjacobianTranV[1] = DistVector.y()/DistVector.perp();
        
  AlgebraicVector1 mjacobianLongV;
  mjacobianLongV[0] = 1.;	
   
  //LIP,TIP,3DIP
  float QQ_Tip = DistVector.perp(); // positive by definition
  float QQ_Lip = DistVector.z(); // signed by definition
  float QQ_3Dip = DistVector.mag(); // positive by definition
  
  NtupleFactory_->FillFloat("QQ_Tip",QQ_Tip);
  NtupleFactory_->FillFloat("QQ_Lip",QQ_Lip);
  NtupleFactory_->FillFloat("QQ_3Dip",QQ_3Dip);
  	
  //LIP,TIP,3DIP errors
  float QQ_errTip = sqrt(ROOT::Math::Similarity(totErrMat.Sub<AlgebraicSymMatrix22>(0,0), mjacobianTranV));
  float QQ_errLip = sqrt(ROOT::Math::Similarity(totErrMat.Sub<AlgebraicSymMatrix11>(2,2), mjacobianLongV));
  float QQ_err3Dip = sqrt(ROOT::Math::Similarity(totErrMat, mjacobian3dV));

  NtupleFactory_->FillFloat("QQ_errTip",QQ_errTip);
  NtupleFactory_->FillFloat("QQ_errLip",QQ_errLip);
  NtupleFactory_->FillFloat("QQ_err3Dip",QQ_err3Dip);
  
  //LIP,TIP,3DIP significances

  NtupleFactory_->FillFloat("QQ_STip",QQ_Tip/QQ_errTip);
  NtupleFactory_->FillFloat("QQ_SLip",QQ_Lip/QQ_errLip);
  NtupleFactory_->FillFloat("QQ_S3Dip",QQ_3Dip/QQ_err3Dip);
    
  QQ_size++;
  return;
}


// ------------ method called to for each event  ------------


void 
    SimpleNtple::analyze(const Event& iEvent, const EventSetup& iSetup)
{
  // get TTRHBuilder 
  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);

  if (saveVtx_)    fillVtxInfo (iEvent, iSetup) ;
  if (saveMu_)     fillMuInfo (iEvent, iSetup) ;    //PG fillMuInfo should be called
  if (saveTracks_) fillTrackInfo (iEvent, iSetup) ; //PG before fillTrackInfo !! 
  if (saveEle_)    fillEleInfo (iEvent, iSetup) ;
  if (saveMC_)     fillMCInfo (iEvent, iSetup) ;
  if (saveSC_)     fillSCInfo (iEvent, iSetup) ;
  if (saveTrigger_) fillTriggerInfo (iEvent, iSetup) ;
  if (saveBeamSpot_) fillBeamSpotInfo (iEvent, iSetup) ;
  if (saveOniaCand_) findOniaCategories (iEvent, iSetup) ;
  // save the entry of the tree 
  NtupleFactory_->FillNtuple();
  
  //Clear RECO Collections
  theGlobalMuons.clear();
  theTrkMuons.clear();
  theElectrons.clear();
  
  //Clear vectors
  theMuonTrkIndexes_.clear();
}

    
// ------------ method called once each job just before starting event loop  ------------
void 
    SimpleNtple::beginJob(const EventSetup& iSetup)
{
  if (saveMu_)
  {
    NtupleFactory_->Add4TV("muons_glb_4mom");
    NtupleFactory_->Add4TV("muons_glb_track4mom");
    
    NtupleFactory_->AddInt("muons_glb_charge"); 
    NtupleFactory_->AddFloat("muons_glb_tkIsoR03"); 
    NtupleFactory_->AddFloat("muons_glb_nTkIsoR03"); 
    NtupleFactory_->AddFloat("muons_glb_emIsoR03"); 
    NtupleFactory_->AddFloat("muons_glb_hadIsoR03"); 
    
    NtupleFactory_->AddFloat("muons_glb_tkIsoR05"); 
    NtupleFactory_->AddFloat("muons_glb_nTkIsoR05"); 
    NtupleFactory_->AddFloat("muons_glb_emIsoR05"); 
    NtupleFactory_->AddFloat("muons_glb_hadIsoR05"); 
    
    NtupleFactory_->AddFloat("muons_glb_ptErr");
    NtupleFactory_->AddFloat("muons_glb_phiErr");
    NtupleFactory_->AddFloat("muons_glb_etaErr");
    NtupleFactory_->AddFloat("muons_glb_d0"); 
    NtupleFactory_->AddFloat("muons_glb_d0err"); 
    NtupleFactory_->AddFloat("muons_glb_dz"); 
    NtupleFactory_->AddFloat("muons_glb_dzerr"); 
    NtupleFactory_->AddFloat("muons_glb_normChi2");

    NtupleFactory_->AddFloat("muons_glb_nhitstrack");
    NtupleFactory_->AddFloat("muons_glb_nhitsDT");
    NtupleFactory_->AddFloat("muons_glb_nhitsCSC");
    NtupleFactory_->AddFloat("muons_glb_nhitsStrip");
    NtupleFactory_->AddFloat("muons_glb_nhitsPixB");
    NtupleFactory_->AddFloat("muons_glb_nhitsPixE");
    NtupleFactory_->AddFloat("muons_glb_nhitsPix1Hit");
    NtupleFactory_->AddFloat("muons_glb_nhitsPix1HitBE");

    NtupleFactory_->Add4TV("muons_trk_4mom");
    
    NtupleFactory_->AddInt("muons_trk_charge"); 
    NtupleFactory_->AddFloat("muons_trk_tkIsoR03"); 
    NtupleFactory_->AddFloat("muons_trk_nTkIsoR03"); 
    NtupleFactory_->AddFloat("muons_trk_emIsoR03"); 
    NtupleFactory_->AddFloat("muons_trk_hadIsoR03"); 
    
    NtupleFactory_->AddFloat("muons_trk_ptErr");
    NtupleFactory_->AddFloat("muons_trk_phiErr");
    NtupleFactory_->AddFloat("muons_trk_etaErr");
    NtupleFactory_->AddFloat("muons_trk_d0"); 
    NtupleFactory_->AddFloat("muons_trk_d0err"); 
    NtupleFactory_->AddFloat("muons_trk_dz"); 
    NtupleFactory_->AddFloat("muons_trk_dzerr"); 
    NtupleFactory_->AddFloat("muons_trk_normChi2");

    NtupleFactory_->AddFloat("muons_trk_nhitstrack");
    NtupleFactory_->AddFloat("muons_trk_nhitsStrip");
    NtupleFactory_->AddFloat("muons_trk_nhitsPixB");
    NtupleFactory_->AddFloat("muons_trk_nhitsPixE");
    NtupleFactory_->AddFloat("muons_trk_nhitsPix1Hit");
    NtupleFactory_->AddFloat("muons_trk_nhitsPix1HitBE");
    
    NtupleFactory_->AddFloat("muons_trk_PIDmask");
  }

  if (saveEle_)
  {
    NtupleFactory_->Add4TV("electrons");
    NtupleFactory_->AddInt("electrons_charge"); 
    NtupleFactory_->AddFloat("electrons_tkIso"); 
    NtupleFactory_->AddFloat("electrons_emIso"); 
    NtupleFactory_->AddFloat("electrons_hadIso");
    NtupleFactory_->AddFloat("electrons_IdBDT");
    NtupleFactory_->AddFloat("electrons_IdLoose"); 
    NtupleFactory_->AddFloat("electrons_IdTight"); 
    NtupleFactory_->AddFloat("electrons_IdRobustLoose"); 
    NtupleFactory_->AddFloat("electrons_IdRobustTight"); 
    NtupleFactory_->AddFloat("electrons_track_d0");
    NtupleFactory_->AddFloat("electrons_track_dz");
    NtupleFactory_->AddFloat("electrons_track_d0err");
    NtupleFactory_->AddFloat("electrons_track_dzerr");
    NtupleFactory_->AddFloat("electrons_flag_mask");
  }

  if (saveTracks_)
  {
    NtupleFactory_->Add3TV("tracks_in");
    NtupleFactory_->Add3TV("tracks_out");   
    NtupleFactory_->AddFloat("tracks_d0");
    NtupleFactory_->AddFloat("tracks_dz");   
    NtupleFactory_->AddFloat("tracks_d0err");
    NtupleFactory_->AddFloat("tracks_dzerr");  
  }

  if (saveVtx_)
  {
    NtupleFactory_->Add3TV("priVtx_3vec");
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
    NtupleFactory_->Add4TV ("MC_particles4V") ;
    NtupleFactory_->AddInt ("MC_iMother") ;
    NtupleFactory_->AddInt ("MC_iDau0") ;
    NtupleFactory_->AddInt ("MC_iDau1") ;
  }

  //PG supercluster information
  NtupleFactory_->AddFloat ("SC_Energy") ;
  NtupleFactory_->Add3TV ("SC_position") ;
  
  if (saveSC_)
  {
    NtupleFactory_->AddFloat ("SC_Energy") ;
    NtupleFactory_->Add3TV ("SC_position") ;
  }

  //Trigger Info
  if (saveTrigger_)
  {
   //Trigger Info
    string HLTbitNames[NHLTTRIGGERS] = {"HLT_Mu3", "HLT_Mu5", "HLT_Mu9", "HLT_DoubleMu0", "HLT_DoubleMu3",   "HLT_Ele10_LW_L1R", "HLT_DoubleEle5_SW_L1R", "HLT_DoublePhoton5_Jpsi_L1R", "HLT_DoublePhoton5_Upsilon_L1R", "HLT_DoublePhoton5_eeRes_L1R"};
    string L1TbitNames[NL1TTRIGGERS]  = {"HLT_L1MuOpen","HLT_L1Mu", "HLT_L1DoubleMuOpen", "HLT_L1SingleEG5", "HLT_L1SingleEG8", "HLT_L1DoubleEG5"};
  
    if (hltConfig.init(the8e29ProcName_)) 
    {
      // check if trigger name in config
      const unsigned int n(hltConfig.size());
   //HLT
      for (int ihlt = 0; ihlt < NHLTTRIGGERS; ihlt++) 
      {
        hltBits[ihlt] = 0;
        unsigned int triggerIndex( hltConfig.triggerIndex(HLTbitNames[ihlt]) );
        if (triggerIndex>=n) 
        {
          cout << "OniaToMuMu::beginRun: "
              << " TriggerName " << HLTbitNames[ihlt]
              << " not available in config!" << endl;
        } else {
          hltBits[ihlt] = triggerIndex;
        }
      }
   
   //Level-1
      for (int il1t = 0; il1t < NL1TTRIGGERS; il1t++) 
      {
        l1tBits[il1t] = 0;
        unsigned int triggerIndex( hltConfig.triggerIndex(L1TbitNames[il1t]) );
        if (triggerIndex>=n) 
        {
          cout << "OniaToMuMu::beginRun: "
              << " TriggerName " << L1TbitNames[il1t]
              << " not available in config!" << endl;
        } else {
          l1tBits[il1t] = triggerIndex;
        }
      }
   
    } else {
      cout << "OniaToMuMu::beginRun:"
          << " HLT config extraction failure with process name HLT" << endl;
    }

    NtupleFactory_->AddInt ("HLTGlobal_wasrun") ;  
    NtupleFactory_->AddInt ("HLTGlobal_Decision") ;  
    NtupleFactory_->AddInt ("HLTGlobal_error") ;
      
      //HLT
    NtupleFactory_->AddInt ("HLTBits_wasrun") ;
    NtupleFactory_->AddInt ("HLTBits_accept") ;
    NtupleFactory_->AddInt ("HLTBits_error") ;

      //L1
    NtupleFactory_->AddInt ("L1TBits_wasrun") ;
    NtupleFactory_->AddInt ("L1TBits_accept") ;
    NtupleFactory_->AddInt ("L1TBits_error") ;
       
  }

  //BeamSpot Info
  if (saveBeamSpot_)
  {
    NtupleFactory_->Add3TV("beamSpot_3vec");
    NtupleFactory_->AddFloat("beamSpot_xxE");
    NtupleFactory_->AddFloat("beamSpot_yyE");
    NtupleFactory_->AddFloat("beamSpot_zzE");
    NtupleFactory_->AddFloat("beamSpot_yxE");
    NtupleFactory_->AddFloat("beamSpot_zyE");
    NtupleFactory_->AddFloat("beamSpot_zxE");
  }

  //Onia Cand Info
  if (saveOniaCand_)
  {
    NtupleFactory_->AddInt("QQ_size");
    NtupleFactory_->AddInt("QQ_sign");
    NtupleFactory_->AddInt("QQ_type");
    
    NtupleFactory_->AddFloat("QQ_DeltaR");
    NtupleFactory_->AddFloat("QQ_s");
    NtupleFactory_->Add4TV("QQ_4mom");
    
    NtupleFactory_->AddInt("QQ_leppl");
    NtupleFactory_->AddInt("QQ_lepmi");
    
    NtupleFactory_->AddFloat("QQ_cosTheta");
    
    NtupleFactory_->AddInt("QQ_lephpt");
    NtupleFactory_->AddInt("QQ_leplpt");
    
    NtupleFactory_->AddInt("QQ_VtxIsVal");
    NtupleFactory_->Add3TV("QQ_Vtx");
    NtupleFactory_->AddFloat("QQ_VxxE");
    NtupleFactory_->AddFloat("QQ_VyyE");
    NtupleFactory_->AddFloat("QQ_VzzE");
    NtupleFactory_->AddFloat("QQ_VyxE");
    NtupleFactory_->AddFloat("QQ_VzyE");
    NtupleFactory_->AddFloat("QQ_VzxE");
    NtupleFactory_->AddFloat("QQ_lxy");
    NtupleFactory_->AddFloat("QQ_lxyErr");
    NtupleFactory_->AddFloat("QQ_normChi2");
    NtupleFactory_->AddFloat("QQ_probChi2");
    
    NtupleFactory_->AddFloat("QQ_cosAlpha");
    NtupleFactory_->AddFloat("QQ_ctau");
    
    NtupleFactory_->AddFloat("QQ_Tip");
    NtupleFactory_->AddFloat("QQ_Lip");
    NtupleFactory_->AddFloat("QQ_3Dip");
    NtupleFactory_->AddFloat("QQ_errTip");
    NtupleFactory_->AddFloat("QQ_errLip");
    NtupleFactory_->AddFloat("QQ_err3Dip");
    NtupleFactory_->AddFloat("QQ_STip");
    NtupleFactory_->AddFloat("QQ_SLip");
    NtupleFactory_->AddFloat("QQ_S3Dip");

  }
  
  return;
}
      
// ------------ method called once each job just after ending the event loop  ------------
     
void 
    SimpleNtple::endJob()
{
  NtupleFactory_->WriteNtuple();
  return;
}

///////////////////////////////////////////////////////////////////////////////
// Returns Lorentz-vector of muon
//////////////////////////////////////////////////////////////////////////////
template <class T>
    TLorentzVector SimpleNtple::lorentzMomentumMu(const T& muon) const 
{
  const double preco = muon.p();
  const double ereco = sqrt(preco*preco + 0.011163613); 

  return TLorentzVector(muon.px(), muon.py(), muon.pz(), ereco);
}

///////////////////////////////////////////////////////////////////////////////
// Returns Lorentz-vector of muon or ele
//////////////////////////////////////////////////////////////////////////////
template <class T> 
    TLorentzVector SimpleNtple::lorentzMomentumLep(const T & lep, float mass) const
{
  const double preco = lep.p();
  const double ereco = sqrt(preco*preco + mass*mass); 

  return TLorentzVector(lep.px(), lep.py(), lep.pz(), ereco);
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

//////////////////////////////////////////////////////////////////////////
// calculates DeltaR
//////////////////////////////////////////////////////////////////////////
template <class T, class U>
    double SimpleNtple::deltaR(const T & t, const U & u) const 
{
  return sqrt(pow(t.Eta()-u.Eta(),2) +pow(PhiInRange(t.Phi()-u.Phi()),2));
}

////////////////////////////////////////////////////////////////////////
//calculates theta
////////////////////////////////////////////////////////////////////////
double SimpleNtple::GetTheta( TLorentzVector & a,  TLorentzVector & b ) const 
{
  TLorentzVector c = a+b;
  TVector3 bv=c.BoostVector();
  a.Boost(-bv);
  b.Boost(-bv);
  double theta = c.Vect().Angle(a.Vect());
  return theta;
}

//////////////////////////////////////////////////////////////////////////
// checks if phi is in range
//////////////////////////////////////////////////////////////////////////
double SimpleNtple::PhiInRange(const double& phi) const 
{
  double phiout = phi;

  if( phiout > 2*M_PI || phiout < -2*M_PI) {
    phiout = fmod( phiout, 2*M_PI);
  }
  if (phiout <= -M_PI) phiout += 2*M_PI;
  else if (phiout >  M_PI) phiout -= 2*M_PI;

  return phiout;
}

