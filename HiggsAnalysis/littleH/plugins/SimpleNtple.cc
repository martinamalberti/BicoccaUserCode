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
// $Id: SimpleNtple.cc,v 1.73 2010/09/08 16:44:33 dimatteo Exp $
//
//

#include "HiggsAnalysis/littleH/plugins/SimpleNtple.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/EventPrincipal.h"
#include "FWCore/Utilities/interface/InputTag.h"

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
using namespace std;

const float muMass_  = 0.105658;
const float eleMass_ = 0.000511;

SimpleNtple::SimpleNtple(const ParameterSet& iConfig) :
Onia2MuMuTag_             (iConfig.getParameter<InputTag> ("Onia2MuMuTag")),
Onia2EleEleTag_           (iConfig.getParameter<InputTag> ("Onia2EleEleTag")),
TracksTag_                (iConfig.getParameter<InputTag> ("TracksTag")),
EleTag_                   (iConfig.getParameter<InputTag> ("EleTag")),
MuTag_                    (iConfig.getParameter<InputTag> ("MuTag")),
PrimaryVertexTag_         (iConfig.getParameter<InputTag> ("PrimaryVertexTag")),
MCtruthTag_               (iConfig.getParameter<InputTag> ("MCtruthTag")),
m_eleIDCut_LooseInputTag  (iConfig.getParameter<string> ("eleIDCut_LooseInputTag")),
m_eleIDCut_RLooseInputTag (iConfig.getParameter<string> ("eleIDCut_RLooseInputTag")),
m_eleIDCut_TightInputTag  (iConfig.getParameter<string> ("eleIDCut_TightInputTag")),
m_eleIDCut_RTightInputTag (iConfig.getParameter<string> ("eleIDCut_RTightInputTag")),
beamSpotTag_              (iConfig.getParameter<InputTag> ("beamSpotTag")),
saveEvt_                  (iConfig.getUntrackedParameter<bool> ("saveEvt", true)),
saveVtx_                  (iConfig.getUntrackedParameter<bool> ("saveVtx", true)),
saveMu_                   (iConfig.getUntrackedParameter<bool> ("saveMu", true)),
saveTracks_               (iConfig.getUntrackedParameter<bool> ("saveTracks", true)),
saveEle_                  (iConfig.getUntrackedParameter<bool> ("saveEle", true)),
saveMC_                   (iConfig.getUntrackedParameter<bool> ("saveMC", true)),
saveBeamSpot_             (iConfig.getUntrackedParameter<bool> ("saveBeamSpot", true)),
saveOniaCand_             (iConfig.getUntrackedParameter<bool> ("saveOniaCand", true)),
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
SimpleNtple::fillEvtInfo (const Event & iEvent, const EventSetup & iESetup) 
{
  int nEvent = iEvent.id().event() ;
  int nRun = iEvent.id().run() ;
  int nLumi = iEvent.id().luminosityBlock() ;
  NtupleFactory_->FillInt ("nEvent",nEvent) ;  
  NtupleFactory_->FillInt ("nRun",nRun) ;  
  NtupleFactory_->FillInt ("nLumi",nLumi) ;  

  return;
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
  Handle< View<pat::Muon> > MuHandle;
  iEvent.getByLabel(MuTag_,MuHandle);
  
  Handle< View<pat::CompositeCandidate> > Onia2MuMuHandle;
  iEvent.getByLabel(Onia2MuMuTag_,Onia2MuMuHandle);
  
  Reco_mu_glb_size = 0;
  
  //  const pat::Muon * muon1;
  //  const pat::Muon * muon2;
  
  //Save muon info only for those inside a dimuon candidate
  int theGlobalMuonsSize = 0;
  int theTrkMuonsSize = 0;
  for (View<pat::CompositeCandidate>::const_iterator ndimuon = Onia2MuMuHandle->begin(); ndimuon != Onia2MuMuHandle->end(); ++ndimuon) {
    
    const pat::Muon & muon1 = dynamic_cast<const pat::Muon &> ( * ndimuon -> daughter(0) );
    const pat::Muon & muon2 = dynamic_cast<const pat::Muon &> ( * ndimuon -> daughter(1) );
        
    int lepone_index = -1, leptwo_index = -1;
    
    int glbMuonCounter = 0;
    bool isglbone = false, isglbtwo = false;
    int theOniaCat = -1;
    
    
    //First Onia Candidate : don't cycle on theGlobalMuons collection (still empty)
    if ( theGlobalMuonsSize == 0
      && muon1 . isGlobalMuon() ) {
      isglbone = true;
    theGlobalMuons.push_back(*(muon1.clone()));
    theMuonTrkIndexes_.push_back(muon1 . innerTrack().index());
    //Onia->Lep Map
    lepone_index = glbMuonCounter;
    
    theGlobalMuonsSize++;
    glbMuonCounter++;   
    
    }
    
    if ( theGlobalMuonsSize == 0
      && muon2 . isGlobalMuon() ) {
      isglbone = true;
    theGlobalMuons.push_back(*(muon2.clone()));
    theMuonTrkIndexes_.push_back(muon2 . innerTrack().index());
    //Onia->Lep Map
    leptwo_index = glbMuonCounter;
    
    theGlobalMuonsSize++;
    glbMuonCounter++;   
    
    }
    
    int AddGlobalMuonSize = 0;
    
    for (pat::MuonCollection::const_iterator glbmuon = theGlobalMuons.begin();
    glbmuon != theGlobalMuons.end();
    glbmuon++) {
      
      //Saving this information is necessary to keep track of the iterator position
      //the iterator loses itself when you add an element to the particle collection (eg. theGlobalMuons)
      int theIterPos = glbmuon - theGlobalMuons.begin();
      
      if ( glbMuonCounter > theGlobalMuonsSize ) break; 
      
      //check that the muons of this Onia candidate are not already in the glbmuon collection
      if ( muon1 . eta() != glbmuon -> eta() 
	&& muon1 . phi() != glbmuon -> phi() 
	&& muon1 . isGlobalMuon() ) {
	isglbone = true;
      theGlobalMuons.push_back(*(muon1.clone()));
      theMuonTrkIndexes_.push_back(muon1 . innerTrack().index());
      //Onia->Lep Map
      lepone_index = glbMuonCounter;
      
      AddGlobalMuonSize++;
      glbMuonCounter++;   
      glbmuon = theGlobalMuons.begin() + theIterPos;
      }
      
      if ( muon1 . eta() != glbmuon -> eta() 
	&& muon1 . phi() != glbmuon -> phi() 
	&& muon2 . isGlobalMuon() ) {
	isglbtwo = true;
      theGlobalMuons.push_back(*(muon2.clone()));
      theMuonTrkIndexes_.push_back(muon1 . innerTrack().index());
      //Onia->Lep Map
      leptwo_index = glbMuonCounter;
      
      AddGlobalMuonSize++;
      glbMuonCounter++;   
      glbmuon = theGlobalMuons.begin() + theIterPos;
      
      }
      
    }
    theGlobalMuonsSize+= AddGlobalMuonSize;
        
    int trkMuonCounter = 0;
    bool istrkone = false, istrktwo = false;
    
    //First Onia Candidate : don't cycle on theTrkMuons collection (still empty)
    if ( theTrkMuonsSize == 0
      && muon1 . isTrackerMuon() ) {
      istrkone = true;
    theTrkMuons.push_back(*(muon1.clone()));
    theMuonTrkIndexes_.push_back(muon1 . innerTrack().index());
    //Onia->Lep Map
    lepone_index = trkMuonCounter;
    
    theTrkMuonsSize++;
    trkMuonCounter++;   
    
    }
    
    if ( theTrkMuonsSize == 0
      && muon2 . isTrackerMuon() ) {
      istrkone = true;
    theTrkMuons.push_back(*(muon2.clone()));
    theMuonTrkIndexes_.push_back(muon2 . innerTrack().index());
    //Onia->Lep Map
    leptwo_index = trkMuonCounter;
    
    theTrkMuonsSize++;
    trkMuonCounter++;   
    
    }
    int AddTrkMuonSize = 0;
    
    for (pat::MuonCollection::const_iterator trkMuon = theTrkMuons.begin();
    trkMuon != theTrkMuons.end(); 
    trkMuon++) {
      
      int theIterPos = trkMuon - theTrkMuons.begin();
      
      if ( trkMuonCounter >= theTrkMuonsSize ) break;
      
      //check that the muons of this Onia candidate are not already in the trkmuon collection
      if ( muon1 . eta() != trkMuon -> eta() 
	&& muon1 . phi() != trkMuon -> phi() 
	&& muon1 . isTrackerMuon() ) {
	
	istrkone = true;
      theTrkMuons.push_back(*(muon1.clone()));
      theMuonTrkIndexes_.push_back(muon1 . innerTrack().index());
      //Onia->Lep Map
      lepone_index = trkMuonCounter;
      
      AddTrkMuonSize++;
      trkMuonCounter++;
      trkMuon = theTrkMuons.begin() + theIterPos;
      
      }
      
      if ( muon2 . eta() != trkMuon -> eta() 
	&& muon2 . phi() != trkMuon -> phi() 
	&& muon2 . isTrackerMuon() ) {
	istrktwo = true;
      theTrkMuons.push_back(*(muon2.clone()));
      theMuonTrkIndexes_.push_back(muon2 . innerTrack().index());
      //Onia->Lep Map
      leptwo_index = trkMuonCounter;
      
      AddTrkMuonSize++;
      trkMuonCounter++;
      trkMuon = theTrkMuons.begin() + theIterPos;
      
      }
      
    }
    theTrkMuonsSize+=AddTrkMuonSize;
    
    if ( isglbone && isglbtwo ) theOniaCat = 1;
    if ( isglbone && istrktwo ) theOniaCat = 2;
    if ( istrkone && isglbtwo ) {
      theOniaCat = 2;
      //invert Onia->Lep map indeces so that leptwo will always point at a tracker muon
      int tmp_index = lepone_index;
      lepone_index = leptwo_index;
      leptwo_index = tmp_index;
    }
    if ( istrkone && istrktwo ) theOniaCat = 3;
    
    // Onia category:
    // 0 = EleEle 
    // 1 = goldenMuMu (2 global muons)
    // 2 = silverMuMu (1 global - 1 tracker muon)
    // 3 = bronzeMuMu (2 tracker muons)
    
    NtupleFactory_->FillInt("QQ_type",theOniaCat);
    NtupleFactory_->FillInt("QQ_lepone",lepone_index);
    NtupleFactory_->FillInt("QQ_leptwo",leptwo_index);
    
  }
    
  for (pat::MuonCollection::const_iterator glbmuon = theGlobalMuons.begin();
  glbmuon != theGlobalMuons.end() ; 
  glbmuon++) {
    reco::TrackRef glbTrack = glbmuon->globalTrack();
    reco::TrackRef innTrack = glbmuon->innerTrack();
    
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
  
  for (pat::MuonCollection::const_iterator trkmuon = theTrkMuons.begin();
  trkmuon != theTrkMuons.end(); 
  trkmuon++) {
    reco::TrackRef innTrack = trkmuon->innerTrack();
    
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
    const reco::MuonIsolation & muonIso = trkmuon->isolationR03() ; 
    
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
  Handle< View<pat::CompositeCandidate> > Onia2EleEleHandle;
  iEvent.getByLabel(Onia2EleEleTag_,Onia2EleEleHandle);
  
  //Save ele info only for those inside a diele candidate
  int theElectronsSize = 0;
  for (View<pat::CompositeCandidate>::const_iterator ndiele = Onia2EleEleHandle->begin(); ndiele != Onia2EleEleHandle->end(); ++ndiele) {
    
    const pat::Electron & ele1 = dynamic_cast<const pat::Electron &> ( * ndiele -> daughter(0) );
    const pat::Electron & ele2 = dynamic_cast<const pat::Electron &> ( * ndiele -> daughter(1) );
    
    int eleCounter = 0;
    
    //First Onia Candidate : don't cycle on theElectrons collection (still empty)
    if ( theElectronsSize == 0 ){
      theElectrons.push_back(*(ele1.clone()));
      //Onia->Lep Map
      NtupleFactory_->FillInt("QQ_lepone",eleCounter);
      
      theElectronsSize++;
      eleCounter++;   
      
    }
    
    if ( theElectronsSize == 0 ){
      theElectrons.push_back(*(ele2.clone()));
      //Onia->Lep Map
      NtupleFactory_->FillInt("QQ_leptwo",eleCounter);
      
      theElectronsSize++;
      eleCounter++;   
      
    }
    
    
    for (pat::ElectronCollection::const_iterator ele = theElectrons.begin();
    eleCounter < theElectronsSize ; 
    ele++) {
      
      int theIterPos = ele - theElectrons.begin();
      
      if ( eleCounter >= theElectronsSize ) break; 
      
      //check that the eles of this Onia candidate are not already in the glbele collection
      if ( ele1 . eta() != ele -> eta() 
	&& ele1 . phi() != ele -> phi() ) {
	
	theElectrons.push_back(*(ele1.clone()));
      //Onia->Lep Map
      NtupleFactory_->FillInt("QQ_lepone",eleCounter);
      theElectronsSize++;
      eleCounter++;
      ele = theElectrons.begin() + theIterPos;
      
      }
      
      if ( ele2 . eta() != ele -> eta() 
	&& ele2 . phi() != ele -> phi() ) {
	
	theElectrons.push_back(*(ele2.clone()));
      //Onia->Lep Map
      NtupleFactory_->FillInt("QQ_leptwo",eleCounter);
      theElectronsSize++;
      eleCounter++;
      ele = theElectrons.begin() + theIterPos;
      
      }
      
      
    }
  }
  
  for ( pat::ElectronCollection::const_iterator theEle = theElectrons.begin(); theEle != theElectrons.end(); theEle++) 
  {     
    
    TLorentzVector myvec4 ((*theEle).px(), (*theEle).py(), (*theEle).pz(), (*theEle).energy());      
    NtupleFactory_->Fill4TV("electrons",myvec4);
    NtupleFactory_->FillInt("electrons_charge",((*theEle).charge()));
    NtupleFactory_->FillFloat("electrons_tkIso", ((*theEle).dr03TkSumPt()));
    NtupleFactory_->FillFloat("electrons_emIso", ((*theEle).dr03EcalRecHitSumEt()));
    NtupleFactory_->FillFloat("electrons_hadIso",((*theEle).dr03HcalDepth1TowerSumEt()));
    
    // electron ID variable from mva (tracker driven electrons)
    NtupleFactory_->FillFloat("electrons_IdBDT",((*theEle).mva()));
    
    //ELE ID
    NtupleFactory_->FillFloat("electrons_IdLoose",((*theEle).electronID(m_eleIDCut_LooseInputTag)));
    NtupleFactory_->FillFloat("electrons_IdTight",((*theEle).electronID(m_eleIDCut_TightInputTag)));
    NtupleFactory_->FillFloat("electrons_IdRobustLoose",((*theEle).electronID(m_eleIDCut_RLooseInputTag)));
    NtupleFactory_->FillFloat("electrons_IdRobustTight",((*theEle).electronID(m_eleIDCut_RTightInputTag)));
    
    //Get Ele Track
    reco::GsfTrackRef eleTrack  = (*theEle).gsfTrack () ; 
    
    NtupleFactory_->FillFloat("electrons_track_d0", eleTrack->d0 ());
    NtupleFactory_->FillFloat("electrons_track_dz", eleTrack->dz ());
    NtupleFactory_->FillFloat("electrons_track_d0err", eleTrack->d0Error ());
    NtupleFactory_->FillFloat("electrons_track_dzerr", eleTrack->dzError ());
    
    int myWord = 0;
    if ((*theEle).trackerDrivenSeed ()) myWord += (int)pow(2.,0);
    if ((*theEle).ecalDrivenSeed ())    myWord += (int)pow(2.,1);
    
    NtupleFactory_->FillFloat("electrons_flag_mask", myWord);
    
  }  
  return ;
}


// --------------------------------------------------------------------


void 
SimpleNtple::fillMCInfo (const Event & iEvent, const EventSetup & iESetup) 
{
  Handle< View<pat::CompositeCandidate> > Onia2MuMuHandle;
  iEvent.getByLabel(Onia2MuMuTag_,Onia2MuMuHandle);
  
  Handle< View<pat::CompositeCandidate> > Onia2EleEleHandle;
  iEvent.getByLabel(Onia2EleEleTag_,Onia2EleEleHandle);
  
  ///Save Onia MC Info
  //dimu
  for (View<pat::CompositeCandidate>::const_iterator ndimu = Onia2MuMuHandle->begin(); ndimu != Onia2MuMuHandle->end(); ++ndimu) {
    
    
    NtupleFactory_->FillInt   ("MC_QQ_PDGID",ndimu -> userInt("momPDGId"));    
    NtupleFactory_->FillFloat ("MC_QQ_ctau",ndimu -> userFloat("ppdlTrue"));    
    
  }
  
  //diele
  for (View<pat::CompositeCandidate>::const_iterator ndiele = Onia2EleEleHandle->begin(); ndiele != Onia2EleEleHandle->end(); ++ndiele) {
    
    NtupleFactory_->FillInt   ("MC_QQ_PDGID",ndiele -> userInt("momPDGId"));    
    NtupleFactory_->FillFloat ("MC_QQ_ctau",ndiele -> userFloat("ppdlTrue"));    
    
  }
  
  ///Save Lepton MC Info
  //glb muons
  for (pat::MuonCollection::const_iterator glbmuon = theGlobalMuons.begin();
  glbmuon != theGlobalMuons.end();
  glbmuon++){
    if ( glbmuon->genLepton()!=0 ){
      const reco::GenParticle & genMuon = dynamic_cast<const reco::GenParticle &> ( * glbmuon -> genLepton() );
      TLorentzVector myvec4(genMuon . px(),genMuon . py(),genMuon . pz(),genMuon . energy());
      NtupleFactory_->FillInt ("isMCmatched", 1) ;
      NtupleFactory_->FillInt ("MC_pdgID", genMuon . pdgId()) ;
      NtupleFactory_->Fill4TV ("MC_particles4V", myvec4) ;
    }
    else {
      NtupleFactory_->FillInt ("isMCmatched", 0) ;
      NtupleFactory_->FillInt ("MC_pdgID", 0) ;
      TLorentzVector myvec4(0,0,0,0);
      NtupleFactory_->Fill4TV ("MC_particles4V", myvec4) ;
    }
  }
  
  //trk muons
  
  for (pat::MuonCollection::const_iterator trkmuon = theTrkMuons.begin();
  trkmuon != theTrkMuons.end();
  trkmuon++){
    
    if ( trkmuon->genLepton()!=0 ){
      const reco::GenParticle & genMuon = dynamic_cast<const reco::GenParticle &> ( * trkmuon -> genLepton() );
      TLorentzVector myvec4(genMuon . px(),genMuon . py(),genMuon . pz(),genMuon . energy());
      NtupleFactory_->FillInt ("isMCmatched", 1) ;
      NtupleFactory_->FillInt ("MC_pdgID", genMuon . pdgId()) ;
      NtupleFactory_->Fill4TV ("MC_particles4V", myvec4) ;
    }
    else {
      NtupleFactory_->FillInt ("isMCmatched", 0) ;
      NtupleFactory_->FillInt ("MC_pdgID", 0) ;
      TLorentzVector myvec4(0,0,0,0);
      NtupleFactory_->Fill4TV ("MC_particles4V", myvec4) ;
    }
    
  }
  
  
  for ( pat::ElectronCollection::const_iterator theEle = theElectrons.begin(); theEle != theElectrons.end(); theEle++) 
  {     
    if ( theEle->genLepton()!=0 ){
      const reco::GenParticle & genEle = dynamic_cast<const reco::GenParticle &> ( * theEle -> genLepton() );
      TLorentzVector myvec4(genEle . px(),genEle . py(),genEle . pz(),genEle . energy());
      NtupleFactory_->FillInt ("isMCmatched", 1) ;
      NtupleFactory_->FillInt ("MC_pdgID", genEle . pdgId()) ;
      NtupleFactory_->Fill4TV ("MC_particles4V", myvec4) ;
    }
    else {
      NtupleFactory_->FillInt ("isMCmatched", 0) ;
      NtupleFactory_->FillInt ("MC_pdgID", 0) ;
      TLorentzVector myvec4(0,0,0,0);
      NtupleFactory_->Fill4TV ("MC_particles4V", myvec4) ;
    }
    
  }
  
  return ;
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

/// =============== Onia candidate block ==============================
void 
SimpleNtple::fillOniaInfo(const Event &iEvent, const EventSetup & iESetup) 
{
  
  int QQ_size=0;
  
  Handle< View<pat::CompositeCandidate> > Onia2MuMuHandle;
  iEvent.getByLabel(Onia2MuMuTag_,Onia2MuMuHandle);
  
  Handle< View<pat::CompositeCandidate> > Onia2EleEleHandle;
  iEvent.getByLabel(Onia2EleEleTag_,Onia2EleEleHandle);
  
  //dimu
  for (View<pat::CompositeCandidate>::const_iterator ndimu = Onia2MuMuHandle->begin(); ndimu != Onia2MuMuHandle->end(); ++ndimu) {
    QQ_size++; //count OniaCand
    
    NtupleFactory_->FillInt   ("QQ_sign",ndimu -> charge()); 
    TLorentzVector my4mom (ndimu ->p4().Px(),ndimu ->p4().Py(),ndimu ->p4().Pz(),ndimu ->p4().E()) ;
    NtupleFactory_->Fill4TV ("QQ_4mom",my4mom);    
    //mancano dr e theta ecc...
    NtupleFactory_->FillFloat("QQ_normChi2",ndimu -> userFloat("vNChi2"));
    NtupleFactory_->FillFloat("QQ_probChi2",ndimu -> userFloat("vProb"));
    
    NtupleFactory_->FillFloat("QQ_ctau",ndimu -> userFloat("ppdlPV"));
    NtupleFactory_->FillFloat("QQ_ctauErr",ndimu -> userFloat("ppdlErrPV"));
    NtupleFactory_->FillFloat("QQ_cosAlpha",ndimu -> userFloat("cosAlpha"));
    
    NtupleFactory_->FillFloat("QQ_ctauBS",ndimu -> userFloat("ppdlBS"));
    NtupleFactory_->FillFloat("QQ_ctauErrBS",ndimu -> userFloat("ppdlErrBS"));
    NtupleFactory_->FillFloat("QQ_cosAlphaBS",ndimu -> userFloat("cosAlphaBS"));
    
  }
  
  //diele
  for (View<pat::CompositeCandidate>::const_iterator ndiele = Onia2EleEleHandle->begin(); ndiele != Onia2EleEleHandle->end(); ++ndiele) {
    
    QQ_size++; //count OniaCand
    
    NtupleFactory_->FillInt   ("QQ_sign",ndiele -> charge()); 
    TLorentzVector my4mom (ndiele ->p4().Px(),ndiele ->p4().Py(),ndiele ->p4().Pz(),ndiele ->p4().E()) ;
    NtupleFactory_->Fill4TV ("QQ_4mom",my4mom);    
    //mancano dr e theta ecc...
    NtupleFactory_->FillFloat("QQ_normChi2",ndiele -> userFloat("vNChi2"));
    NtupleFactory_->FillFloat("QQ_probChi2",ndiele -> userFloat("vProb"));
    
    NtupleFactory_->FillFloat("QQ_ctau",ndiele -> userFloat("ppdlPV"));
    NtupleFactory_->FillFloat("QQ_ctauErr",ndiele -> userFloat("ppdlErrPV"));
    NtupleFactory_->FillFloat("QQ_cosAlpha",ndiele -> userFloat("cosAlpha"));
    
    NtupleFactory_->FillFloat("QQ_ctauBS",ndiele -> userFloat("ppdlBS"));
    NtupleFactory_->FillFloat("QQ_ctauErrBS",ndiele -> userFloat("ppdlErrBS"));
    NtupleFactory_->FillFloat("QQ_cosAlphaBS",ndiele -> userFloat("cosAlphaBS"));
    
  }
  
  NtupleFactory_->FillInt("QQ_size",QQ_size);
  return;
}

// ------------ method called to for each event  ------------

void 
SimpleNtple::analyze(const Event& iEvent, const EventSetup& iSetup)
{
  
  if (saveEvt_)    fillEvtInfo (iEvent, iSetup) ;
  if (saveVtx_)    fillVtxInfo (iEvent, iSetup) ;
  if (saveMu_)     fillMuInfo (iEvent, iSetup) ;    //PG fillMuInfo should be called
  if (saveTracks_) fillTrackInfo (iEvent, iSetup) ; //PG before fillTrackInfo !! 
  if (saveEle_)    fillEleInfo (iEvent, iSetup) ;
  if (saveMC_)     fillMCInfo (iEvent, iSetup) ;
  if (saveBeamSpot_) fillBeamSpotInfo (iEvent, iSetup) ;
  if (saveOniaCand_) fillOniaInfo (iEvent, iSetup) ;
  // save the entry of the tree 
  NtupleFactory_->FillNtuple();
  
  // reset the NtupleFactory 
  NtupleFactory_->ClearNtuple();
  
  //Clear RECO Collections
  theGlobalMuons.clear();
  theTrkMuons.clear();
  theElectrons.clear();
  
  //Clear vectors
  theMuonTrkIndexes_.clear();
}


// ------------ method called once each job just before starting event loop  ------------
void 
SimpleNtple::beginJob()
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
  
  if (saveEvt_)
  {
    NtupleFactory_->AddInt ("nEvent") ;  
    NtupleFactory_->AddInt ("nRun") ;  
    NtupleFactory_->AddInt ("nLumi") ;  
  }

  //PG MC truth
  if (saveMC_)
  {
    NtupleFactory_->AddInt ("isMCmatched") ;
    NtupleFactory_->AddInt ("MC_pdgID") ;
    NtupleFactory_->Add4TV ("MC_particles4V") ;
    NtupleFactory_->AddInt ("MC_QQ_PDGID") ;
    NtupleFactory_->AddFloat ("MC_QQ_ctau") ;
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
    NtupleFactory_->Add4TV("QQ_4mom");
    
    NtupleFactory_->AddInt("QQ_lepone");
    NtupleFactory_->AddInt("QQ_leptwo");
    
    NtupleFactory_->AddFloat("QQ_ctau");
    NtupleFactory_->AddFloat("QQ_ctauErr");
    NtupleFactory_->AddFloat("QQ_cosAlpha");
    
    NtupleFactory_->AddFloat("QQ_ctauBS");
    NtupleFactory_->AddFloat("QQ_ctauErrBS");
    NtupleFactory_->AddFloat("QQ_cosAlphaBS");
    
    NtupleFactory_->AddFloat("QQ_normChi2");
    NtupleFactory_->AddFloat("QQ_probChi2");
    
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


/////////////////////////////////////////////////////////////////////
// Number of hits per muon/track
/////////////////////////////////////////////////////////////////////
std::vector<unsigned int> SimpleNtple::trackHits(const reco::Track& tr) 
{
  std::vector<unsigned int> theTrackHits;
  const reco::HitPattern& p = tr.hitPattern();
  
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

// ----------------------------------------------------------------

//define this as a plug-in
DEFINE_FWK_MODULE(SimpleNtple);

// ----------------------------------------------------------------

