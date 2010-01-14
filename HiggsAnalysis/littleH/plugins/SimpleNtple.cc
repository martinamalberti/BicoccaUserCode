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
// $Id: SimpleNtple.cc,v 1.9 2010/01/14 11:40:00 govoni Exp $
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

#include "DataFormats/Common/interface/ValueMap.h"

//---- utilities ----
#include "HiggsAnalysis/littleH/interface/MCDumper.h"

using namespace edm;

SimpleNtple::SimpleNtple(const ParameterSet& iConfig) 
{
 Service<TFileService> fs ;
 outTree_  = fs->make <TTree>("SimpleTree","SimpleTree"); 
 
 TracksTag_ = iConfig.getParameter<InputTag>("TracksTag");
 EleTag_ = iConfig.getParameter<InputTag>("EleTag");
 MuTag_ = iConfig.getParameter<InputTag>("MuTag");

 m_eleIDCut_LooseInputTag  = iConfig.getParameter<InputTag> ("eleIDCut_LooseInputTag");
 m_eleIDCut_RLooseInputTag = iConfig.getParameter<InputTag> ("eleIDCut_RLooseInputTag");
 m_eleIDCut_TightInputTag  = iConfig.getParameter<InputTag> ("eleIDCut_TightInputTag");
 m_eleIDCut_RTightInputTag = iConfig.getParameter<InputTag> ("eleIDCut_RTightInputTag");

 //  MCtruthTag_ = iConfig.getParameter<InputTag>("MCtruthTag");

 verbosity_ = iConfig.getUntrackedParameter<bool>("verbosity","False");
 eventType_ = iConfig.getUntrackedParameter<int>("eventType",1);
 
 NtupleFactory_ = new NtupleFactory(outTree_);
}


// --------------------------------------------------------------------


SimpleNtple::~SimpleNtple()
{
 delete NtupleFactory_;
}


//
// member functions
//

// ------------ method called to for each event  ------------
void SimpleNtple::analyze(const Event& iEvent, const EventSetup& iSetup)
{
  ///---- fill muons ----
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

  ///---- fill electrons ----
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

  for(int i=0; i< nEle; i++){
  
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

  ///---- fill tracks ----
  Handle<View<reco::Track> > TracksHandle ;
  iEvent.getByLabel (TracksTag_, TracksHandle) ;

  int k(0);
  for (View<reco::Track>::const_iterator tkIt = TracksHandle->begin (); tkIt != TracksHandle->end (); ++tkIt ) 
    { 
      k++;

      bool storeThisOther = true;
      for (int j = 0; j<(int)theMuonTrkIndexes.size(); ++j) {
	if (k == theMuonTrkIndexes.at(j)) {
	  storeThisOther = false;
	  break;
	}
      }
      if(!storeThisOther) continue;

      NtupleFactory_->Fill3V("tracks_in",(*tkIt).innerMomentum ());  
      NtupleFactory_->Fill3V("tracks_out",(*tkIt).outerMomentum ());
  
      NtupleFactory_->FillFloat("tracks_d0",(*tkIt).d0());
      NtupleFactory_->FillFloat("tracks_dz",(*tkIt).dz());
      NtupleFactory_->FillFloat("tracks_d0err",(*tkIt).d0Error());
      NtupleFactory_->FillFloat("tracks_dzerr",(*tkIt).dzError());
    }
 
  ///---- fill MCParticles ---- 
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
      NtupleFactory_->FillFloat ("MCpdgID", toBeSaved.at (iGen)->pdgId ()) ;
      NtupleFactory_->Fill4V ("MCparticles4V", toBeSaved.at(iGen)->p4 ()) ;
    } //PG loop on gen particles to be saved

  
      ///---- fill MCParticle ---- 
      //  Handle<reco::GenParticleCollection> genParticles;
      //  iEvent.getByLabel(MCtruthTag_, genParticles);
      // 
      // //  int eventType_ = 1; //---- 0 = signal      1 = background 
      // //  bool verbosity_ = true; //---- true = loquacious     false = silence
      //  MCDumper mcAnalysis(genParticles, eventType_, verbosity_); //---- i "tau" mi fanno scrivere a schermo anche se NON è segnale
      //  bool isValid = mcAnalysis.isValid();
      //   
      //  if( (eventType_ == 0) && (isValid == true) )
      // {
      //   math::XYZTLorentzVector* myvect_XYZT_mcH = new math::XYZTLorentzVector(mcAnalysis.mcH()->p4().Px(),mcAnalysis.mcH()->p4().Py(),mcAnalysis.mcH()->p4().Pz(),mcAnalysis.mcH()->p4().E());
      //   NtupleFactory_->Fill4V("mc_H",myvect_XYZT_mcH);
      //   NtupleFactory_->FillFloat("mc_H_charge",mcAnalysis.mcH()->charge());
      //   
      //   math::XYZTLorentzVector* myvect_XYZT_mcV1 = new math::XYZTLorentzVector(mcAnalysis.mcV1()->p4().Px(),mcAnalysis.mcV1()->p4().Py(),mcAnalysis.mcV1()->p4().Pz(),mcAnalysis.mcV1()->p4().E());
      //   NtupleFactory_->Fill4V("mcV1",myvect_XYZT_mcV1);
      //   NtupleFactory_->FillFloat("mcV1_charge",mcAnalysis.mcV1()->charge());
      //   NtupleFactory_->FillFloat("mcV1_pdgId",mcAnalysis.mcV1()->pdgId());
      //   
      //   math::XYZTLorentzVector* myvect_XYZT_mcV2 = new math::XYZTLorentzVector(mcAnalysis.mcV2()->p4().Px(),mcAnalysis.mcV2()->p4().Py(),mcAnalysis.mcV2()->p4().Pz(),mcAnalysis.mcV2()->p4().E());
      //   NtupleFactory_->Fill4V("mcV2",myvect_XYZT_mcV2);
      //   NtupleFactory_->FillFloat("mcV2_charge",mcAnalysis.mcV2()->charge());
      //   NtupleFactory_->FillFloat("mcV2_pdgId",mcAnalysis.mcV2()->pdgId());
      //      
      //   math::XYZTLorentzVector* myvect_XYZT_mcF1_fromV1 = new math::XYZTLorentzVector(mcAnalysis.mcF1_fromV1()->p4().Px(),mcAnalysis.mcF1_fromV1()->p4().Py(),mcAnalysis.mcF1_fromV1()->p4().Pz(),mcAnalysis.mcF1_fromV1()->p4().E());
      //   NtupleFactory_->Fill4V("mcF1_fromV1",myvect_XYZT_mcF1_fromV1);
      //   NtupleFactory_->FillFloat("mcF1_fromV1_charge",mcAnalysis.mcF1_fromV1()->charge());
      //   NtupleFactory_->FillFloat("mcF1_fromV1_pdgId",mcAnalysis.mcF1_fromV1()->pdgId());
      // 
      //   math::XYZTLorentzVector* myvect_XYZT_mcF2_fromV1 = new math::XYZTLorentzVector(mcAnalysis.mcF2_fromV1()->p4().Px(),mcAnalysis.mcF2_fromV1()->p4().Py(),mcAnalysis.mcF2_fromV1()->p4().Pz(),mcAnalysis.mcF2_fromV1()->p4().E());
      //   NtupleFactory_->Fill4V("mcF2_fromV1",myvect_XYZT_mcF2_fromV1);
      //   NtupleFactory_->FillFloat("mcF2_fromV1_charge",mcAnalysis.mcF2_fromV1()->charge());
      //   NtupleFactory_->FillFloat("mcF2_fromV1_pdgId",mcAnalysis.mcF2_fromV1()->pdgId());
      // 
      //   math::XYZTLorentzVector* myvect_XYZT_mcF1_fromV2 = new math::XYZTLorentzVector(mcAnalysis.mcF1_fromV2()->p4().Px(),mcAnalysis.mcF1_fromV2()->p4().Py(),mcAnalysis.mcF1_fromV2()->p4().Pz(),mcAnalysis.mcF1_fromV2()->p4().E());
      //   NtupleFactory_->Fill4V("mcF1_fromV2",myvect_XYZT_mcF1_fromV2);
      //   NtupleFactory_->FillFloat("mcF1_fromV2_charge",mcAnalysis.mcF1_fromV2()->charge());
      //   NtupleFactory_->FillFloat("mcF1_fromV2_pdgId",mcAnalysis.mcF1_fromV2()->pdgId());
      // 
      //   math::XYZTLorentzVector* myvect_XYZT_mcF2_fromV2 = new math::XYZTLorentzVector(mcAnalysis.mcF2_fromV2()->p4().Px(),mcAnalysis.mcF2_fromV2()->p4().Py(),mcAnalysis.mcF2_fromV2()->p4().Pz(),mcAnalysis.mcF2_fromV2()->p4().E());
      //   NtupleFactory_->Fill4V("mcF2_fromV2",myvect_XYZT_mcF2_fromV2);
      //   NtupleFactory_->FillFloat("mcF2_fromV2_charge",mcAnalysis.mcF2_fromV2()->charge());
      //   NtupleFactory_->FillFloat("mcF2_fromV2_pdgId",mcAnalysis.mcF2_fromV2()->pdgId());
      //     
      //   math::XYZTLorentzVector* myvect_XYZT_mcQ1_tag = new math::XYZTLorentzVector(mcAnalysis.mcQ1_tag()->p4().Px(),mcAnalysis.mcQ1_tag()->p4().Py(),mcAnalysis.mcQ1_tag()->p4().Pz(),mcAnalysis.mcQ1_tag()->p4().E());
      //   NtupleFactory_->Fill4V("mcQ1_tag",myvect_XYZT_mcQ1_tag);
      //   NtupleFactory_->FillFloat("mcQ1_tag_charge",mcAnalysis.mcQ1_tag()->charge());
      //   NtupleFactory_->FillFloat("mcQ1_tag_pdgId",mcAnalysis.mcQ1_tag()->pdgId());
      // 
      //   math::XYZTLorentzVector* myvect_XYZT_mcQ2_tag = new math::XYZTLorentzVector(mcAnalysis.mcQ2_tag()->p4().Px(),mcAnalysis.mcQ2_tag()->p4().Py(),mcAnalysis.mcQ2_tag()->p4().Pz(),mcAnalysis.mcQ2_tag()->p4().E());
      //   NtupleFactory_->Fill4V("mcQ2_tag",myvect_XYZT_mcQ2_tag);
      //   NtupleFactory_->FillFloat("mcQ2_tag_charge",mcAnalysis.mcQ2_tag()->charge());
      //   NtupleFactory_->FillFloat("mcQ2_tag_pdgId",mcAnalysis.mcQ2_tag()->pdgId());
      //       
      // }
  
      ///---- save the entry of the tree ----
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

 return;
}
   
   
// ------------ method called once each job just after ending the event loop  ------------
     
void SimpleNtple::endJob()
{
  NtupleFactory_->WriteNtuple();
  return;
}

