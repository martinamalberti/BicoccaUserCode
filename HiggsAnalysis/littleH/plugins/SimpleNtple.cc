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
// $Id: SimpleNtple.cc,v 1.37 2010/01/21 15:04:11 dimatteo Exp $
//
//

#include "HiggsAnalysis/littleH/plugins/SimpleNtple.h"

using namespace edm;

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
  theOniaType               (iConfig.getUntrackedParameter<int> ("oniaType", 443)), 
  theOniaMaxCat             (iConfig.getUntrackedParameter<int> ("oniaMaxCat", 1)),
  Chi2OniaVtxCut_           (iConfig.getUntrackedParameter<double> ("Chi2OniaVtxCut", 0.01)), 
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
    theElectrons.push_back((*EleHandle)[i]);
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

  math::XYZVector myvect_XYZ (bs.x0(),bs.y0(),bs.z0());
  NtupleFactory_->Fill3V("beamSpot_3vec",myvect_XYZ);

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

  TVector3 vperp2;
  if ( theBeamSpotFlag ) 
  {
    vperp2.SetXYZ(bs.x0(), bs.y0(), 0);
  }
  else {
    if ( privtxs->begin() != privtxs->end() ) 
    {
      // select the primary vertex as the one
      // with higest sum of (pt)^2 of tracks
      PrimaryVertexSorter PVSorter;
      std::vector<reco::Vertex> sortedVertices = PVSorter.sortedList( *(privtxs.product()) );
      reco::Vertex PV = sortedVertices.front();
      vperp2.SetXYZ(PV.position().x(), PV.position().y(), 0);
      
//       privtx=privtxs->begin();
//       vperp2.SetXYZ(privtx->position().x(), privtx->position().y(), 0);
    }
    else 
    {
      vperp2.SetXYZ(0, 0, 0);
    }
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
  int m1=0;
  int m2g=0;
  int m2t=0;

  reco::GsfTrackRef ele1;
  reco::GsfTrackRef ele2;
  int e1=0;
  int e2=0;
  
  //Electrons
  for ( nele1 = theElectrons.begin(); nele1 != theElectrons.end(); nele1++) 
  {
    for( nele2 = nele1+1; nele2 != theElectrons.end()&&QQ_size<3000; nele2++ ) 
    {
      ele1 = nele1->gsfTrack();    
      ele2 = nele2->gsfTrack(); 
      e2++;
      fillOnia2EleEleTracks(ele1, e1, ele2, e1+e2, vperp2, 0);
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
      if (theOniaMaxCat >= 0) fillOnia2MuMuTracks(muon1, m1, muon2, m1+m2g, vperp2, 1);
    }
     
    for( tmuon2 = theTrkMuons.begin(); tmuon2 != theTrkMuons.end()&&QQ_size<3000; tmuon2++ ) 
    {
      muon1 = nmuon1->innerTrack();    
      muon2 = tmuon2->innerTrack(); 
      if (theOniaMaxCat >= 1) fillOnia2MuMuTracks(muon1, m1, muon2, m2t, vperp2, 2);
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
      if (theOniaMaxCat >= 2) fillOnia2MuMuTracks(muon1, m1, muon2, m2t+m1, vperp2, 3);
    }
    m1++;
  }
    
  NtupleFactory_->FillInt("QQ_size",QQ_size);
  return;
}


// --------------------------------------------------------------------

//Fill Onia2MuMu category
void 
    SimpleNtple::fillOnia2MuMuTracks(TrackRef lep1, int l1, TrackRef lep2, int l2, TVector3 vperp2, int oniacato) 
{
 
  if (oniacato<0  ) return;
  
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
   
  if ( TMath::Prob(tv.totalChiSquared(), (int)tv.degreesOfFreedom()) < Chi2OniaVtxCut_ ) return;//Loose Cut on Onia Vertex Chi2

  NtupleFactory_->FillInt("QQ_VtxIsVal",1);

         // if ( lep1->charge() == lep2->charge() ) continue;
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
  
  math::XYZTLorentzVector lp1=lorentzMomentum(*lep1);
  math::XYZTLorentzVector lp2=lorentzMomentum(*lep2);
  math::XYZTLorentzVector onia = lp1 + lp2;
  
  NtupleFactory_->FillInt("QQ_type",oniacato);
  NtupleFactory_->FillFloat("QQ_DeltaR",deltaR(lp1, lp2));
  NtupleFactory_->FillFloat("QQ_s",pow((lep1->d0()/lep1->d0Error()),2)+pow((lep2->d0()/lep2->d0Error()),2));

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
  if (oniacato == 2 || lp1.pt() > lp2.pt()) 
  {   
    NtupleFactory_->FillInt("QQ_lephpt",l1);
    NtupleFactory_->FillInt("QQ_lephp",l2);
  } else 
  {
    NtupleFactory_->FillInt("QQ_lephp",l1);
    NtupleFactory_->FillInt("QQ_lephpt",l2);
  }
           
  math::XYZVector myvect_XYZ (v.x(),v.y(),v.z());
  NtupleFactory_->Fill3V("QQ_Vtx",myvect_XYZ);

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
  TVector3 vperp = vperp1 - vperp2;
  double cosAlpha = vperp.Dot(pperp)/(vperp.Perp()*pperp.Perp());
  double ctau = vperp.Perp()*cosAlpha*oniaMass/onia.pt();
  NtupleFactory_->FillFloat("QQ_cosAlpha",cosAlpha);
  NtupleFactory_->FillFloat("QQ_ctau",ctau);   
     
//   } else 
//   {
  //       
//     NtupleFactory_->FillInt("QQ_VtxIsVal",0);
  //     
//     math::XYZVector v(-1,-1,-1);
//     NtupleFactory_->Fill3V("QQ_Vtx",v);
  //     
//     NtupleFactory_->FillFloat("QQ_VxxE",-1);
//     NtupleFactory_->FillFloat("QQ_VyyE",-1);
//     NtupleFactory_->FillFloat("QQ_VzzE",-1);
//     NtupleFactory_->FillFloat("QQ_VyxE",-1);
//     NtupleFactory_->FillFloat("QQ_VzyE",-1);
//     NtupleFactory_->FillFloat("QQ_VzxE",-1);
  //           
//     NtupleFactory_->FillFloat("QQ_lxy",-1);
//     NtupleFactory_->FillFloat("QQ_lxyErr",-1);
//     NtupleFactory_->FillFloat("QQ_normChi2",-1);
//     NtupleFactory_->FillFloat("QQ_probChi2",-1);
  //           
//     NtupleFactory_->FillFloat("QQ_cosAlpha",-2);
//     NtupleFactory_->FillFloat("QQ_ctau",-100);
  //       
//   }
    
  QQ_size++;
  return;
}

// --------------------------------------------------------------------

//Fill Onia2EleEle category
void 
    SimpleNtple::fillOnia2EleEleTracks(GsfTrackRef lep1, int l1, GsfTrackRef lep2, int l2, TVector3 vperp2, int oniacato) 
{
 
  if (oniacato<0  ) return;
  
  TransientTrack ttkp1   = (*theB).build(&(*lep1));
  TransientTrack ttkp2   = (*theB).build(&(*lep2));

  vector<TransientTrack> t_tks;
  t_tks.push_back(ttkp1);
  t_tks.push_back(ttkp2);
  
  KalmanVertexFitter kvf;
  TransientVertex tv = kvf.vertex(t_tks);

//  // Gaussian Sum Filter Algorithm 
//   GsfVertexFitter gsf(gsfPSet);
//   TransientVertex tv = gsf.vertex(t_tks);
    
  if ( ! tv.isValid() ) return;
//   {
  GlobalPoint v = tv.position();
  GlobalError err = tv.positionError();
    
  if ( TMath::Prob(tv.totalChiSquared(), (int)tv.degreesOfFreedom()) < Chi2OniaVtxCut_ ) return;//Loose Cut on Onia Vertex Chi2
    
  NtupleFactory_->FillInt("QQ_VtxIsVal",1);
     
  // if ( lep1->charge() == lep2->charge() ) continue;
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

  math::XYZTLorentzVector lp1=lorentzMomentum(*lep1);
  math::XYZTLorentzVector lp2=lorentzMomentum(*lep2);
  math::XYZTLorentzVector onia = lp1 + lp2;
  
  NtupleFactory_->FillInt("QQ_type",oniacato);
  NtupleFactory_->FillFloat("QQ_DeltaR",deltaR(lp1, lp2));
  NtupleFactory_->FillFloat("QQ_s",pow((lep1->d0()/lep1->d0Error()),2)+pow((lep2->d0()/lep2->d0Error()),2));

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
  if (oniacato == 2 || lp1.pt() > lp2.pt()) 
  {   
    NtupleFactory_->FillInt("QQ_lephpt",l1);
    NtupleFactory_->FillInt("QQ_lephp",l2);
  } else 
  {
    NtupleFactory_->FillInt("QQ_lephp",l1);
    NtupleFactory_->FillInt("QQ_lephpt",l2);
  }
      
  math::XYZVector myvect_XYZ (v.x(),v.y(),v.z());
  NtupleFactory_->Fill3V("QQ_Vtx",myvect_XYZ);

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
  TVector3 vperp = vperp1 - vperp2;
  double cosAlpha = vperp.Dot(pperp)/(vperp.Perp()*pperp.Perp());
  double ctau = vperp.Perp()*cosAlpha*oniaMass/onia.pt();
  NtupleFactory_->FillFloat("QQ_cosAlpha",cosAlpha);
  NtupleFactory_->FillFloat("QQ_ctau",ctau);
//   } else 
//   {
  //       
//     NtupleFactory_->FillInt("QQ_VtxIsVal",0);
  //     
//     math::XYZVector v(-1,-1,-1);
//     NtupleFactory_->Fill3V("QQ_Vtx",v);
  //     
//     NtupleFactory_->FillFloat("QQ_VxxE",-1);
//     NtupleFactory_->FillFloat("QQ_VyyE",-1);
//     NtupleFactory_->FillFloat("QQ_VzzE",-1);
//     NtupleFactory_->FillFloat("QQ_VyxE",-1);
//     NtupleFactory_->FillFloat("QQ_VzyE",-1);
//     NtupleFactory_->FillFloat("QQ_VzxE",-1);
  //           
//     NtupleFactory_->FillFloat("QQ_lxy",-1);
//     NtupleFactory_->FillFloat("QQ_lxyErr",-1);
//     NtupleFactory_->FillFloat("QQ_normChi2",-1);
//     NtupleFactory_->FillFloat("QQ_probChi2",-1);
  //           
//     NtupleFactory_->FillFloat("QQ_cosAlpha",-2);
//     NtupleFactory_->FillFloat("QQ_ctau",-100);
  //       
//   }
    
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
  if ( theOniaType==443 )  {
    oniaMass=3.09688;
    branch_ratio = 0.0593;
  }
  else if ( theOniaType==553 ) {
    oniaMass=9.46030;
    branch_ratio = 0.0248;
  }
  else if ( theOniaType==100443 ) {
    oniaMass=3.68600;
    branch_ratio = 0.0083;
  }
  else if ( theOniaType==100553 ) {
    oniaMass=10.0233;
    branch_ratio = 0.014;
  }
  else if ( theOniaType==200553 ) {
    oniaMass=10.3552;
    branch_ratio = 0.0218;
  }
  else {
    cout<<"Please set the correct onia type: 443 (Jpsi), 100443(psi'), 553(Upsilon1S), 100553(Upsilon2S), 200553(Upsilon3S),"<<endl;
  }
  
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
  NtupleFactory_->AddFloat ("SC_Energy") ;
  NtupleFactory_->Add3V ("SC_position") ;
  
  if (saveSC_)
  {
    NtupleFactory_->AddFloat ("SC_Energy") ;
    NtupleFactory_->Add3V ("SC_position") ;
  }

  //Trigger Info
  if (saveTrigger_)
  {
   //Trigger Info
    string HLTbitNames[NHLTTRIGGERS] = {"HLT_Mu3", "HLT_Mu5", "HLT_Mu9", "HLT_DoubleMu0", "HLT_DoubleMu3",   "HLT_Ele10_LW_L1R", "HLT_DoubleEle5_SW_L1R"};
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
    NtupleFactory_->Add3V("beamSpot_3vec");
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
    
    NtupleFactory_->AddInt("QQ_leppl");
    NtupleFactory_->AddInt("QQ_lepmi");
    
    NtupleFactory_->AddFloat("QQ_cosTheta");
    
    NtupleFactory_->AddInt("QQ_lephpt");
    NtupleFactory_->AddInt("QQ_lephp");
    
    NtupleFactory_->AddInt("QQ_VtxIsVal");
    NtupleFactory_->Add3V("QQ_Vtx");
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
double SimpleNtple::GetTheta( math::XYZTLorentzVector & a,  math::XYZTLorentzVector & b) const 
{
  TLorentzVector aL, bL ; 
  aL.SetXYZT(a.px(),a.py(),a.pz(),a.energy());
  bL.SetXYZT(b.px(),b.py(),b.pz(),b.energy());
  
  TLorentzVector cL = aL+bL;
  TVector3 bv=cL.BoostVector();
  aL.Boost(-bv);
  bL.Boost(-bv);
  double theta = cL.Vect().Angle(aL.Vect());
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
