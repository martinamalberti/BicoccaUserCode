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
// $Id: SimpleNtple.cc,v 1.16 2010/01/14 15:05:06 govoni Exp $
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

#include "HiggsAnalysis/littleH/plugins/SimpleNtple.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"


//--- objects ----
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuonTrackLinks.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
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

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "RecoVertex/KinematicFitPrimitives/interface/ParticleMass.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicParticleFactoryFromTransientTrack.h"
#include "RecoVertex/KinematicFit/interface/KinematicParticleVertexFitter.h"
#include "RecoVertex/KinematicFit/interface/KinematicParticleFitter.h"

#include "DataFormats/Common/interface/ValueMap.h"

//---- utilities ----
#include "HiggsAnalysis/littleH/interface/MCDumper.h"

using namespace edm;

SimpleNtple::SimpleNtple(const ParameterSet& iConfig) :
  TracksTag_                (iConfig.getParameter<InputTag> ("TracksTag")),
  EleTag_                   (iConfig.getParameter<InputTag> ("EleTag")),
  MuTag_                    (iConfig.getParameter<InputTag> ("MuTag")),
  PrimaryVertexTag_         (iConfig.getParameter<InputTag> ("PrimaryVertexTag")),
  m_eleIDCut_LooseInputTag  (iConfig.getParameter<InputTag> ("eleIDCut_LooseInputTag")),
  m_eleIDCut_RLooseInputTag (iConfig.getParameter<InputTag> ("eleIDCut_RLooseInputTag")),
  m_eleIDCut_TightInputTag  (iConfig.getParameter<InputTag> ("eleIDCut_TightInputTag")),
  m_eleIDCut_RTightInputTag (iConfig.getParameter<InputTag> ("eleIDCut_RTightInputTag")),
  barrelClusterCollection_  (iConfig.getParameter<edm::InputTag> ("barrelClusterCollection")),
  endcapClusterCollection_  (iConfig.getParameter<edm::InputTag> ("endcapClusterCollection")),
  verbosity_                (iConfig.getUntrackedParameter<bool> ("verbosity","False")),
  eventType_                (iConfig.getUntrackedParameter<int> ("eventType",1))
 
{
 Service<TFileService> fs ;
 outTree_  = fs->make <TTree>("SimpleTree","SimpleTree"); 
 

 //  MCtruthTag_ = iConfig.getParameter<InputTag>("MCtruthTag");

 NtupleFactory_ = new NtupleFactory(outTree_);
}


// --------------------------------------------------------------------


SimpleNtple::~SimpleNtple()
{
 delete NtupleFactory_;
}


// --------------------------------------------------------------------


void 
SimpleNtple::fillVtxInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
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
SimpleNtple::fillMuInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
  Handle<MuonCollection> MuHandle;
  iEvent.getByLabel(MuTag_,MuHandle);
 
   //used to save all tracks BUT muons
  vector<int> theMuonTrkIndexes;
 
  MuonCollection theTrkMuons;
  MuonCollection theGlobalMuons;
 
  for (MuonCollection::const_iterator nmuon = MuHandle->begin(); nmuon != MuHandle->end(); ++nmuon) {
   if (nmuon->isGlobalMuon()) {
    theGlobalMuons.push_back(*nmuon);
    theMuonTrkIndexes.push_back(nmuon->innerTrack().index());
   }
   if (!(nmuon->isGlobalMuon()) && nmuon->isTrackerMuon()) {
    theTrkMuons.push_back(*nmuon);
    theMuonTrkIndexes.push_back(nmuon->innerTrack().index());
   }
  }
 
  int nMu = MuHandle->size();
 
  for(int i=0; i< nMu; i++){
   NtupleFactory_->Fill4V("muons", (*MuHandle)[i].p4()) ;
   
   NtupleFactory_->FillFloat("muons_charge",((*MuHandle)[i].charge()));
   NtupleFactory_->FillFloat("muons_tkIsoR03",((*MuHandle)[i].isolationR03()).sumPt);
   NtupleFactory_->FillFloat("muons_nTkIsoR03",((*MuHandle)[i].isolationR03()).nTracks);    
   NtupleFactory_->FillFloat("muons_emIsoR03",((*MuHandle)[i].isolationR03()).emEt);
   NtupleFactory_->FillFloat("muons_hadIsoR03",((*MuHandle)[i].isolationR03()).hadEt);
     
   NtupleFactory_->FillFloat("muons_tkIsoR05",((*MuHandle)[i].isolationR05()).sumPt);
   NtupleFactory_->FillFloat("muons_nTkIsoR05",((*MuHandle)[i].isolationR05()).nTracks);    
   NtupleFactory_->FillFloat("muons_emIsoR05",((*MuHandle)[i].isolationR05()).emEt);
   NtupleFactory_->FillFloat("muons_hadIsoR05",((*MuHandle)[i].isolationR05()).hadEt);
 
     //Get Global Muon Track
   TrackRef glbTrack = (*MuHandle)[i].globalTrack();
   
   NtupleFactory_->FillFloat("muons_track_d0", glbTrack->d0 ());
   NtupleFactory_->FillFloat("muons_track_dz", glbTrack->dz ());
   NtupleFactory_->FillFloat("muons_track_d0err", glbTrack->d0Error ());
   NtupleFactory_->FillFloat("muons_track_dzerr", glbTrack->dzError ());
  }
  return ;
}  


// --------------------------------------------------------------------


void 
SimpleNtple::fillTrackInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
  Handle<View<reco::Track> > TracksHandle ;
  iEvent.getByLabel (TracksTag_, TracksHandle) ;
 
  int k(0);
  for (View<reco::Track>::const_iterator tkIt = TracksHandle->begin (); 
       tkIt != TracksHandle->end (); ++tkIt ) 
    { 
      k++;
    
      bool storeThisOther = true;
// FIXME questo e' da far funzionare in modo sensato
//      for (int j = 0; j<(int)theMuonTrkIndexes.size(); ++j) 
//        {
//          if (k == theMuonTrkIndexes.at(j)) 
//            {
//              storeThisOther = false;
//              break;
//            }
//        }
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
SimpleNtple::fillEleInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
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
SimpleNtple::fillSCInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
  edm::Handle<reco::SuperClusterCollection> bscHandle ;  
  iEvent.getByLabel (barrelClusterCollection_, bscHandle) ;
//  if (!(bscHandle.isValid())) {// do something? }

  //PG loop on superclusters in the barrel
  for (reco::SuperClusterCollection::const_iterator iClus = bscHandle->begin () ; 
       iClus != bscHandle->end () ; 
       ++iClus) 
    {

      NtupleFactory_->FillFloat ("SC_Energy", iClus->energy ()) ;
//      NtupleFactory_->Fill3V ("SC_position", iClus->position ()) ;
    } //PG loop on superclusters in the barrel

  edm::Handle<reco::SuperClusterCollection> escHandle ;  
  iEvent.getByLabel (endcapClusterCollection_, escHandle) ;

  //PG loop on superclusters in the endcaps
  for (reco::SuperClusterCollection::const_iterator iClus = escHandle->begin () ; 
       iClus != escHandle->end () ; 
       ++iClus) 
    {

      NtupleFactory_->FillFloat ("SC_Energy", iClus->energy ()) ;
//      NtupleFactory_->Fill3V ("SC_position", iClus->position ()) ;
    } //PG loop on superclusters in the endcaps

  return ;
}


// --------------------------------------------------------------------


void 
SimpleNtple::fillMCInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
  edm::Handle<reco::GenParticleCollection> genParticles;
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
  for (int iGen = 0; iGen < toBeSaved.size () ; ++iGen) 
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


// ------------ method called to for each event  ------------


void SimpleNtple::analyze(const Event& iEvent, const EventSetup& iSetup)
{

  
  fillMuInfo (iEvent, iSetup) ;
  fillTrackInfo (iEvent, iSetup) ;
  fillEleInfo (iEvent, iSetup) ;
  fillMCInfo (iEvent, iSetup) ;
  fillSCInfo (iEvent, iSetup) ;

  // save the entry of the tree 
  NtupleFactory_->FillNtuple();


}

    
// ------------ method called once each job just before starting event loop  ------------
void SimpleNtple::beginJob(const EventSetup& iSetup)
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
  
 NtupleFactory_->Add3V("tracks_in");
 NtupleFactory_->Add3V("tracks_out");   
 NtupleFactory_->AddFloat("tracks_d0");
 NtupleFactory_->AddFloat("tracks_dz");   
 NtupleFactory_->AddFloat("tracks_d0err");
 NtupleFactory_->AddFloat("tracks_dzerr");  
 
 NtupleFactory_->Add3V("priVtx_3vec");
 NtupleFactory_->AddFloat("priVtx_xxE");
 NtupleFactory_->AddFloat("priVtx_yyE");
 NtupleFactory_->AddFloat("priVtx_zzE");
 NtupleFactory_->AddFloat("priVtx_yxE");
 NtupleFactory_->AddFloat("priVtx_zyE");
 NtupleFactory_->AddFloat("priVtx_zxE");
 NtupleFactory_->AddFloat("priVtx_chi2");
 NtupleFactory_->AddFloat("priVtx_ndof");

  //PG MC truth
  NtupleFactory_->AddFloat ("MC_pdgID") ;
  NtupleFactory_->Add4V ("MC_particles4V") ;
  NtupleFactory_->AddInt ("MC_iMother") ;
  NtupleFactory_->AddInt ("MC_iDau0") ;
  NtupleFactory_->AddInt ("MC_iDau1") ;

  //PG supercluster information
  NtupleFactory_->AddFloat ("SC_Energy") ;
  NtupleFactory_->Add3V ("SC_position") ;

  return ;
}
   
   
// ------------ method called once each job just after ending the event loop  ------------
     
void SimpleNtple::endJob()
{
 NtupleFactory_->WriteNtuple();
 return;
}

