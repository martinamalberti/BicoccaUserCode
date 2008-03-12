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
// Original Author:  Alessio Ghezzi
//         Created:  Tue Jun  5 19:34:31 CEST 2007
// $Id: SimpleNtple.cc,v 1.1 2008/03/12 15:32:42 ghezzi Exp $
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

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/SimpleNtple.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"

#include <DataFormats/RecoCandidate/interface/RecoChargedCandidate.h>
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h>

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"

#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

SimpleNtple::SimpleNtple(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
  edm::Service<TFileService> fs ;
  mytree_  = fs->make <TTree>("VBFSimpleTree","VBFSimpleTree"); 
   
  EleTag_= iConfig.getParameter<edm::InputTag>("EleTag");
  IsolEleTag_= iConfig.getParameter<edm::InputTag>("IsolEleTag");
  MuTag_= iConfig.getParameter<edm::InputTag>("MuTag");
  IsolMuTag_= iConfig.getParameter<edm::InputTag>("IsolMuTag");
  MetTag_= iConfig.getParameter<edm::InputTag>("MetTag");
  TagJetTag_= iConfig.getParameter<edm::InputTag>("TagJetTag");
  JetTag_= iConfig.getParameter<edm::InputTag>("JetTag");
}


SimpleNtple::~SimpleNtple()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
SimpleNtple::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  Init();
  FillEle (iEvent, iSetup);
  FillMu (iEvent, iSetup);
  FillMet (iEvent, iSetup);
  FillTagJet (iEvent, iSetup);
  FillJet (iEvent, iSetup);
  
  mytree_->Fill();
}

void SimpleNtple::FillEle(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  //  typedef edm::View<reco::PixelMatchGsfElectron> electronCollection
  // edm::Handle<electronCollection> EleHandle ;

  //typedef edm::View<reco::PixelMatchGsfElectron> electronCollection
  edm::Handle<edm::View<reco::PixelMatchGsfElectron> > EleHandle ;
  //  edm::Handle<reco::PixelMatchGsfElectronCollection> EleHandle ;

  iEvent.getByLabel (EleTag_,EleHandle) ;

  //Fix Me -> sort ?
  
  if(EleHandle->size() < 30 ){ nEle = EleHandle->size(); }
  else {nEle = 30;}
  for(int i=0; i< nEle; i++){
    EtEle[i]= (*EleHandle)[i].et();
    EtaEle[i]= (*EleHandle)[i].eta();
    PhiEle[i]= (*EleHandle)[i].phi();
    IsolEleSumPt[i]= (*EleHandle)[i].eta();
    IsolEleNTracks[i]= (*EleHandle)[i].eta();
    EleId[i]=0;
  }

  
}

void SimpleNtple::FillMu(const edm::Event& iEvent, const edm::EventSetup& iSetup){
 edm::Handle<reco::MuonCollection> MuHandle;
  iEvent.getByLabel (MuTag_,MuHandle);

  if(MuHandle->size() < 30 ){ nMu = MuHandle->size(); }
  else {nMu = 30;}
  for(int i=0; i< nMu; i++){
    EtMu[i]= (*MuHandle)[i].et();
    EtaMu[i]= (*MuHandle)[i].eta();
    PhiMu[i]= (*MuHandle)[i].phi();
    IsolMuSumPt[i]= (*MuHandle)[i].getIsolationR03().sumPt;
    IsolMuNTracks[i]= (*MuHandle)[i].getIsolationR03().nTracks;
  }
}

void SimpleNtple::FillMet(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  edm::Handle<reco::CaloMETCollection> metCollectionHandle;
  iEvent.getByLabel (MetTag_ , metCollectionHandle);
  const CaloMETCollection *calometcol = metCollectionHandle.product();
  const CaloMET *calomet = &(calometcol->front());
  Met = calomet->pt();
  MetX = calomet->py();
  MetY = calomet->px();

  
}

void SimpleNtple::FillTagJet(const edm::Event& iEvent, const edm::EventSetup& iSetup){
   edm::Handle<reco::RecoChargedCandidateCollection> jetTagsHandle ;
  // edm::Handle<reco::CandidateCollection> jetTagsHandle ;
  iEvent.getByLabel (TagJetTag_, jetTagsHandle) ;
  if (jetTagsHandle->size () != 2) return ;
  for(int i=0; i< 2; i++){
    EtTagJet[i]= (*jetTagsHandle)[i].p4().pt ();
    EtaTagJet[i]= (*jetTagsHandle)[i].p4().eta ();
    PhiTagJet[i]= (*jetTagsHandle)[i].p4().phi ();
  }
   math::XYZTLorentzVector sumLV = (*jetTagsHandle)[0].p4() + (*jetTagsHandle)[1].p4() ;
   MinvTags = sumLV.M();
}

void SimpleNtple::FillJet(const edm::Event& iEvent, const edm::EventSetup& iSetup){

  edm::Handle<reco::CaloJetCollection> JetHandle ;
  iEvent.getByLabel (JetTag_, JetHandle) ;

  if(JetHandle->size() < 50 ){ nJet = JetHandle->size(); }
  else {nJet = 50;}
  for(int i=0; i< nJet; i++){
    EtJet[i]= (*JetHandle)[i].et();
    EtaJet[i]= (*JetHandle)[i].eta();
    PhiJet[i]= (*JetHandle)[i].phi();
  }


}

void SimpleNtple::Init(){
  nEle = 0; 
  nMu = 0;
  for (int i=0;i<30;i++){
    EtEle[i]=0;EtaEle[i]=0;PhiEle[i]=0;IsolEleSumPt[i]=0;IsolEleNTracks[i]=0;EleId[i]=0;
    EtMu[i]=0;EtaMu[i]=0;PhiMu[i]=0;IsolMuSumPt[i]=0;IsolMuNTracks[i]=0;
  }

  MetX=0;MetY=0;Met=-1;

  MinvTags = -1;
  for (int i=0;i<2;i++){
    EtTagJet[i]=0;EtaTagJet[i]=0;PhiTagJet[i]=0;
  }

  nJet = 0;
  for (int i=0;i<50;i++){
    EtJet[i]=0;EtaJet[i]=0;PhiJet[i]=0;
  }

}

// ------------ method called once each job just before starting event loop  ------------
void 
SimpleNtple::beginJob(const edm::EventSetup& iSetup)
{
  mytree_->Branch("nEle",&nEle,"nEle/I");
  mytree_->Branch("EtEle",EtEle,"EtEle[30]/F");
  mytree_->Branch("EtaEle",EtaEle,"EtaEle[30]/F");
  mytree_->Branch("PhiEle",PhiEle,"PhiEle[30]/F");
  mytree_->Branch("IsolEleSumPt",IsolEleSumPt,"IsolEleSumPt[30]/F");
  mytree_->Branch("IsolEleNTracks",IsolEleNTracks,"IsolEleNTracks[30]/F");

  mytree_->Branch("nMu",&nMu,"nMu/I");
  mytree_->Branch("EtMu",EtMu,"EtMu[30]/F");
  mytree_->Branch("EtaMu",EtaMu,"EtaMu[30]/F");
  mytree_->Branch("PhiMu",PhiMu,"PhiMu[30]/F");
  mytree_->Branch("IsolMuSumPt",IsolMuSumPt,"IsolMuSumPt[30]/F");
  mytree_->Branch("IsolMuNTracks",IsolMuNTracks,"IsolMuNTracks[30]/F");

  mytree_->Branch("MetX",&MetX,"MetX/F");
  mytree_->Branch("MetY",&MetY,"MetY/F");
  mytree_->Branch("Met",&Met,"Met/F");

  mytree_->Branch("EtTagJet",EtTagJet,"EtTagJet[2]/F");
  mytree_->Branch("EtaTagJet",EtaTagJet,"EtaTagJet[2]/F");
  mytree_->Branch("PhiTagJet",PhiTagJet,"PhiTagJet[2]/F");
  mytree_->Branch("MinvTags",&MinvTags,"MinvTags/F");

  mytree_->Branch("nJet",&nJet,"nJet/I");
  mytree_->Branch("EtJet",EtJet,"EtJet[50]/F");
  mytree_->Branch("EtaJet",EtaJet,"EtaJet[50]/F");
  mytree_->Branch("PhiJet",PhiJet,"PhiJet[50]/F");



}

// ------------ method called once each job just after ending the event loop  ------------
void 
SimpleNtple::endJob() {

}

