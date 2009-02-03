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
// $Id: SimpleNtple.cc,v 1.12 2009/01/15 13:24:16 govoni Exp $
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

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/SimpleNtple.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"

#include <DataFormats/RecoCandidate/interface/RecoChargedCandidate.h>
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h>

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"

#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

SimpleNtple::SimpleNtple(const edm::ParameterSet& iConfig) :
  m_tkIsolationAlgo (
      iConfig.getParameter<double> ("coneRadius") ,
      iConfig.getParameter<double> ("vetoRadius") ,
      iConfig.getParameter<double> ("otherVetoRadius") ,
      iConfig.getParameter<double> ("ptMin") ,
      iConfig.getParameter<double> ("lipMax") ,
      iConfig.getUntrackedParameter<bool> ("useTkQuality",true)
    ) ,
  m_eleIDPTDRLooseInputTag (iConfig.getParameter<edm::InputTag> ("eleIDPTDRLooseInputTag")) ,
  m_eleIDPTDRMediumInputTag (iConfig.getParameter<edm::InputTag> ("eleIDPTDRMediumInputTag")) ,
  m_eleIDPTDRTightInputTag (iConfig.getParameter<edm::InputTag> ("eleIDPTDRTightInputTag"))
{
   //now do what ever initialization is needed
  edm::Service<TFileService> fs ;
  mytree_  = fs->make <TTree>("VBFSimpleTree","VBFSimpleTree"); 
   
  TracksTag_= iConfig.getParameter<edm::InputTag>("TracksTag");
  EleTag_= iConfig.getParameter<edm::InputTag>("EleTag");
  IsolEleTag_= iConfig.getParameter<edm::InputTag>("IsolEleTag");
  MuTag_= iConfig.getParameter<edm::InputTag>("MuTag");
  IsolMuTag_= iConfig.getParameter<edm::InputTag>("IsolMuTag");
  MetTag_= iConfig.getParameter<edm::InputTag>("MetTag");
  TagJetTag_= iConfig.getParameter<edm::InputTag>("TagJetTag");
  JetTag_= iConfig.getParameter<edm::InputTag>("JetTag");
}


// --------------------------------------------------------------------


SimpleNtple::~SimpleNtple()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

  delete m_tagJets ;
  delete m_otherJets ;
  delete m_electrons ;
  delete m_muons ;
  delete m_MET ;
  delete m_tracks ;
  
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
  FillTracks (iEvent, iSetup);
  
  mytree_->Fill();

  m_tagJets -> Clear () ;
  m_otherJets -> Clear () ;  
  m_electrons -> Clear ()  ;
  m_muons -> Clear ()  ;
  m_MET -> Clear ()  ;
  m_tracks -> Clear () ;
}


// --------------------------------------------------------------------


void SimpleNtple::FillEle(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  //  typedef edm::View<reco::PixelMatchGsfElectron> electronCollection
  // edm::Handle<electronCollection> EleHandle ;

  //typedef edm::View<reco::PixelMatchGsfElectron> electronCollection
  edm::Handle<edm::View<reco::PixelMatchGsfElectron> > EleHandle ;
  //  edm::Handle<reco::PixelMatchGsfElectronCollection> EleHandle ;

  iEvent.getByLabel (EleTag_,EleHandle) ;

  edm::Handle<edm::View<reco::Track> > TracksHandle ;
  //  edm::Handle<reco::PixelMatchGsfElectronCollection> EleHandle ;

  iEvent.getByLabel (TracksTag_,TracksHandle) ;

  //PG get the electron ID collections
  std::vector<edm::Handle<reco::ElectronIDAssociationCollection> > eleIdHandles ;
  for (int i=0 ; i<3 ; ++i)
    {
      edm::Handle<reco::ElectronIDAssociationCollection> dummy ;
      eleIdHandles.push_back (dummy) ;
    }
  iEvent.getByLabel (m_eleIDPTDRLooseInputTag, eleIdHandles[0]);
  iEvent.getByLabel (m_eleIDPTDRMediumInputTag, eleIdHandles[1]);
  iEvent.getByLabel (m_eleIDPTDRTightInputTag, eleIdHandles[2]);

  //Fix Me -> sort ?
  
  if(EleHandle->size() < 30 ){ nEle = EleHandle->size(); }
  else {nEle = 30;}
  for(int i=0; i< nEle; i++){
    reco::PixelMatchGsfElectronRef electronReference = 
      EleHandle->refAt (i).castTo<reco::PixelMatchGsfElectronRef> () ;
    IsolEleSumPt[i] = 
      m_tkIsolationAlgo.calcSumOfPt (EleHandle, TracksHandle, electronReference) ;
    IsolEleNTracks[i] = 
      m_tkIsolationAlgo.countNumOfTracks (EleHandle, TracksHandle, electronReference) ;

  
    int times[3] = {1,10,100} ;
    EleId[i]=0;
    reco::ElectronIDAssociationCollection::const_iterator electronIDAssocItr ;
    for (int j=0 ; j<3 ; ++j)
      {
        electronIDAssocItr = eleIdHandles[j]->find (electronReference) ;
        if (electronIDAssocItr != eleIdHandles[j]->end ())
          {
            const reco::ElectronIDRef & id = electronIDAssocItr->val ;
            bool cutBasedID = id->cutBasedDecision () ;
            EleId[i] += cutBasedID * times[j] ;
          }
      } //PG loop over the eleID
  }

  TClonesArray &electrons = *m_electrons;
  int counter = 0;
  for (int i=0; i< nEle; i++)
    {
      setMomentum (myvector, (*EleHandle)[i].p4());
      new (electrons[counter]) TLorentzVector (myvector);
      counter++;
    }


  
}


// --------------------------------------------------------------------


void SimpleNtple::FillMu(const edm::Event& iEvent, const edm::EventSetup& iSetup){
 edm::Handle<reco::MuonCollection> MuHandle;
  iEvent.getByLabel (MuTag_,MuHandle);

  if(MuHandle->size() < 30 ){ nMu = MuHandle->size(); }
  else {nMu = 30;}
//  for(int i=0; i< nMu; i++){
//    IsolMuSumPt[i]= (*MuHandle)[i].getIsolationR03().sumPt;
//    IsolMuNTracks[i]= (*MuHandle)[i].getIsolationR03().nTracks;
//  }

  TClonesArray &muons = *m_muons;
  int counter = 0;
  for (int i=0; i< nMu; i++)
    {
      setMomentum (myvector, (*MuHandle)[i].p4());
      new (muons[counter]) TLorentzVector (myvector);
      counter++;
    }



}


// --------------------------------------------------------------------


void SimpleNtple::FillMet(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  edm::Handle<reco::CaloMETCollection> metCollectionHandle;
  iEvent.getByLabel (MetTag_ , metCollectionHandle);
  const CaloMETCollection *calometcol = metCollectionHandle.product();
  const CaloMET *calomet = &(calometcol->front());

  TClonesArray &MET = *m_MET;
  setMomentum (myvector, calomet->p4());
  new (MET[0]) TLorentzVector (myvector);
  
}


// --------------------------------------------------------------------


void SimpleNtple::FillTagJet(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  //   edm::Handle<reco::RecoChargedCandidateCollection> jetTagsHandle ;
   edm::Handle<reco::RecoChargedCandidateCollection> jetTagsHandle;
  // edm::Handle<reco::CandidateCollection> jetTagsHandle ;
  iEvent.getByLabel (TagJetTag_, jetTagsHandle) ;
  if (jetTagsHandle->size () != 2) return ;
  math::XYZTLorentzVector sumLV = (*jetTagsHandle)[0].p4() + (*jetTagsHandle)[1].p4() ;
  MinvTags = sumLV.M();

  TClonesArray &jetTag = *m_tagJets;
  int counter = 0;
  for (RecoChargedCandidateCollection::const_iterator jet = jetTagsHandle->begin (); 
       jet != jetTagsHandle->end (); 
       ++jet )
    { 
      setMomentum (myvector, jet->p4());
      new (jetTag[counter]) TLorentzVector (myvector);
      counter++;
    }
}


// --------------------------------------------------------------------


void SimpleNtple::FillJet(const edm::Event& iEvent, const edm::EventSetup& iSetup){

  edm::Handle<reco::CaloJetCollection> JetHandle ;
  iEvent.getByLabel (JetTag_, JetHandle) ;

  TClonesArray &jetOther = *m_otherJets;
  int counter = 0;
  for (CaloJetCollection::const_iterator jet2 = JetHandle->begin (); 
       jet2 != JetHandle->end (); 
       ++jet2 ) 
    { 
      setMomentum (myvector, jet2->p4());        
      new (jetOther[counter]) TLorentzVector (myvector);
      counter++;
     }

}


// --------------------------------------------------------------------


void 
SimpleNtple::FillTracks(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::Handle<trackCollection> TracksHandle ;
  iEvent.getByLabel (TracksTag_, TracksHandle) ;

  TClonesArray &tracks = *m_tracks;
  int counter = 0;
  for (trackCollection::const_iterator tkIt = TracksHandle->begin (); 
       tkIt != TracksHandle->end (); 
       ++tkIt ) 
    { 
      math::XYZVector mom = (*tkIt).innerMomentum () ; 
      myvector.SetPx (mom.x ()) ;
      myvector.SetPy (mom.y ()) ;
      myvector.SetPz (mom.z ()) ;
      new (tracks[counter]) TLorentzVector (myvector);
      counter++;
     }
}


// --------------------------------------------------------------------


void SimpleNtple::Init(){
  nEle = 0; 
  nMu = 0;
  for (int i=0;i<30;i++){
    IsolEleSumPt[i]=0;IsolEleNTracks[i]=0;EleId[i]=0;
    IsolMuSumPt[i]=0;IsolMuNTracks[i]=0;
  }

  MinvTags = -1;
}


// ------------ method called once each job just before starting event loop  ------------


void 
SimpleNtple::beginJob(const edm::EventSetup& iSetup)
{
  mytree_->Branch("nEle",&nEle,"nEle/I");
  mytree_->Branch("IsolEleSumPt",IsolEleSumPt,"IsolEleSumPt[30]/F");
  mytree_->Branch("IsolEleNTracks",IsolEleNTracks,"IsolEleNTracks[30]/F");
  mytree_->Branch("EleId",EleId,"EleId[30]/I");

  mytree_->Branch("nMu",&nMu,"nMu/I");
  mytree_->Branch("IsolMuSumPt",IsolMuSumPt,"IsolMuSumPt[30]/F");
  mytree_->Branch("IsolMuNTracks",IsolMuNTracks,"IsolMuNTracks[30]/F");

  mytree_->Branch("MinvTags",&MinvTags,"MinvTags/F");

  // vector with the 2 tag TLorentzVectors
  m_tagJets = new TClonesArray ("TLorentzVector");
  mytree_->Branch ("tagJets", "TClonesArray", &m_tagJets, 256000,0);

  // vector of the TLorentz Vectors of other jets
  m_otherJets = new TClonesArray ("TLorentzVector");
  mytree_->Branch ("otherJets", "TClonesArray", &m_otherJets, 256000,0);

  // vector of the TLorentz Vectors of other jets
  m_electrons = new TClonesArray ("TLorentzVector");
  mytree_->Branch ("electrons", "TClonesArray", &m_electrons, 256000,0);

  // vector of the TLorentz Vectors of other jets
  m_muons = new TClonesArray ("TLorentzVector");
  mytree_->Branch ("muons", "TClonesArray", &m_muons, 256000,0);

  // vector of the TLorentz Vectors of other jets
  m_MET = new TClonesArray ("TLorentzVector");
  mytree_->Branch ("MET", "TClonesArray", &m_MET, 256000,0);

  // vector of the TLorentz Vectors of other jets
  m_tracks = new TClonesArray ("TLorentzVector");
  mytree_->Branch ("tracks", "TClonesArray", &m_tracks, 256000,0);

}


// ------------ method called once each job just after ending the event loop  ------------


void 
SimpleNtple::endJob() {

}


// --------------------------------------------------------------------


void
SimpleNtple::setMomentum (TLorentzVector &myvector, const LorentzVector & mom)
{
    myvector.SetPx (mom.Px());
    myvector.SetPy (mom.Py());
    myvector.SetPz (mom.Pz());
    myvector.SetE (mom.E());
}

