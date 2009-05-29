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
// $Id: SimpleNtple.cc,v 1.8 2009/05/14 09:31:53 amassiro Exp $
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

#include "DataFormats/Common/interface/ValueMap.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/JetReco/interface/GenJet.h"

#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"

#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"

#include "DataFormats/BTauReco/interface/JetTag.h"


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
   
  //! isolation variable
 eleTkIsoTag_    = iConfig.getParameter<edm::InputTag>("eleTkIsoTag");
 eleEcalIsoTag_  = iConfig.getParameter<edm::InputTag>("eleEcalIsoTag");
 eleHcalIsoTag_  = iConfig.getParameter<edm::InputTag>("eleHcalIsoTag");

 doRefCheckTag_ = iConfig.getUntrackedParameter<bool>("doRefCheckTag",false);
 EleRefTag_  = iConfig.getUntrackedParameter<edm::InputTag>("EleRefTag");
  
 TracksTag_= iConfig.getParameter<edm::InputTag>("TracksTag");
 EleTag_= iConfig.getParameter<edm::InputTag>("EleTag");
 IsolEleTag_= iConfig.getParameter<edm::InputTag>("IsolEleTag");
 MuTag_= iConfig.getParameter<edm::InputTag>("MuTag");
 IsolMuTag_= iConfig.getParameter<edm::InputTag>("IsolMuTag");
 MetTag_= iConfig.getParameter<edm::InputTag>("MetTag");
 TagJetTag_= iConfig.getParameter<edm::InputTag>("TagJetTag");
 JetTag_= iConfig.getParameter<edm::InputTag>("JetTag");
 MCtruthTag_ = iConfig.getParameter<edm::InputTag> ("MCtruthTag");
 genJetTag_ = iConfig.getParameter<edm::InputTag> ("genJetTag");
 genMetTag_ = iConfig.getParameter<edm::InputTag> ("genMetTag");

 bool_JetTagSisCone5CaloJets_= iConfig.getUntrackedParameter<bool>("bool_JetTagSisCone5CaloJets",false);
 bool_JetTagIterativeCone5CaloJets_= iConfig.getUntrackedParameter<bool>("bool_JetTagIterativeCone5CaloJets",false);
 bool_JetTagSisCone5PFJets_= iConfig.getUntrackedParameter<bool>("bool_JetTagSisCone5PFJets",false);
 bool_JetTagIterativeCone5PFJets_= iConfig.getUntrackedParameter<bool>("bool_JetTagIterativeCone5PFJets",false);

  
 bool_JetTagSisCone5CaloJets_L2L3_= iConfig.getUntrackedParameter<bool>("bool_JetTagSisCone5CaloJets_L2L3",false);
 bool_JetTagIterativeCone5CaloJets_L2L3_= iConfig.getUntrackedParameter<bool>("bool_JetTagIterativeCone5CaloJets_L2L3",false);
 bool_JetTagSisCone5PFJets_L2L3_= iConfig.getUntrackedParameter<bool>("bool_JetTagSisCone5PFJets_L2L3",false);
 bool_JetTagIterativeCone5PFJets_L2L3_= iConfig.getUntrackedParameter<bool>("bool_JetTagIterativeCone5PFJets_L2L3",false);

  
 bool_JetTagSisCone5CaloJets_JPT_= iConfig.getUntrackedParameter<bool>("bool_JetTagSisCone5CaloJets_JPT",false);
 bool_JetTagIterativeCone5CaloJets_JPT_= iConfig.getUntrackedParameter<bool>("bool_JetTagIterativeCone5CaloJets_JPT",false);
 bool_JetTagSisCone5PFJets_JPT_= iConfig.getUntrackedParameter<bool>("bool_JetTagSisCone5PFJets_JPT",false);
 bool_JetTagIterativeCone5PFJets_JPT_= iConfig.getUntrackedParameter<bool>("bool_JetTagIterativeCone5PFJets_JPT",false);

  
  
  
  
 bool_JetTagIterativeCone5CaloJets_BTagging_= iConfig.getUntrackedParameter<bool>("bool_JetTagIterativeCone5CaloJets_BTagging",false);
  
 if (bool_JetTagSisCone5CaloJets_) JetTagSisCone5CaloJets_= iConfig.getParameter<edm::InputTag>("JetTagSisCone5CaloJets");
 if (bool_JetTagIterativeCone5CaloJets_) JetTagIterativeCone5CaloJets_= iConfig.getParameter<edm::InputTag>("JetTagIterativeCone5CaloJets");
 if (bool_JetTagSisCone5PFJets_) JetTagSisCone5PFJets_= iConfig.getParameter<edm::InputTag>("JetTagSisCone5PFJets");
 if (bool_JetTagIterativeCone5PFJets_) JetTagIterativeCone5PFJets_= iConfig.getParameter<edm::InputTag>("JetTagIterativeCone5PFJets");
 
  
 if (bool_JetTagSisCone5CaloJets_L2L3_) JetTagSisCone5CaloJets_L2L3_= iConfig.getParameter<edm::InputTag>("JetTagSisCone5CaloJets_L2L3");
 if (bool_JetTagIterativeCone5CaloJets_L2L3_) JetTagIterativeCone5CaloJets_L2L3_= iConfig.getParameter<edm::InputTag>("JetTagIterativeCone5CaloJets_L2L3");
 if (bool_JetTagSisCone5PFJets_L2L3_) JetTagSisCone5PFJets_L2L3_= iConfig.getParameter<edm::InputTag>("JetTagSisCone5PFJets_L2L3");
 if (bool_JetTagIterativeCone5PFJets_L2L3_) JetTagIterativeCone5PFJets_L2L3_= iConfig.getParameter<edm::InputTag>("JetTagIterativeCone5PFJets_L2L3");
 
  
 if (bool_JetTagSisCone5CaloJets_JPT_) JetTagSisCone5CaloJets_JPT_= iConfig.getParameter<edm::InputTag>("JetTagSisCone5CaloJets_JPT");
 if (bool_JetTagIterativeCone5CaloJets_JPT_) JetTagIterativeCone5CaloJets_JPT_= iConfig.getParameter<edm::InputTag>("JetTagIterativeCone5CaloJets_JPT");
 if (bool_JetTagSisCone5PFJets_JPT_) JetTagSisCone5PFJets_JPT_= iConfig.getParameter<edm::InputTag>("JetTagSisCone5PFJets_JPT");
 if (bool_JetTagIterativeCone5PFJets_JPT_) JetTagIterativeCone5PFJets_JPT_= iConfig.getParameter<edm::InputTag>("JetTagIterativeCone5PFJets_JPT");
 
  
    
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
 delete m_genParticles ;
 delete m_HiggsParticle ;
 delete m_genJets;
 delete m_genMet;
  
 delete m_otherJets_SisCone5CaloJets ;
 delete m_otherJets_IterativeCone5CaloJets ;
 delete m_otherJets_SisCone5PFJets ;
 delete m_otherJets_IterativeCone5PFJets ;

 delete m_otherJets_L2L3_SisCone5CaloJets ;
 delete m_otherJets_L2L3_IterativeCone5CaloJets ;
 delete m_otherJets_L2L3_SisCone5PFJets ;
 delete m_otherJets_L2L3_IterativeCone5PFJets ;

 delete m_otherJets_JPT_SisCone5CaloJets ;
 delete m_otherJets_JPT_IterativeCone5CaloJets ;
 delete m_otherJets_JPT_SisCone5PFJets ;
 delete m_otherJets_JPT_IterativeCone5PFJets ;

      

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
  SimpleNtple::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 
 Init();
 
 m_tagJets -> Clear () ;
 m_otherJets -> Clear () ;  
 m_electrons -> Clear ()  ;
 m_muons -> Clear ()  ;
 m_MET -> Clear ()  ;
 m_tracks -> Clear () ;
 m_genParticles -> Clear () ;
 m_HiggsParticle -> Clear () ;
 m_genJets -> Clear () ;
 m_genMet -> Clear () ;
  
 m_otherJets_SisCone5CaloJets -> Clear () ;
 m_otherJets_IterativeCone5CaloJets -> Clear () ;
 m_otherJets_SisCone5PFJets -> Clear () ;
 m_otherJets_IterativeCone5PFJets -> Clear () ;

 m_otherJets_L2L3_SisCone5CaloJets -> Clear () ;
 m_otherJets_L2L3_IterativeCone5CaloJets -> Clear () ;
 m_otherJets_L2L3_SisCone5PFJets -> Clear () ;
 m_otherJets_L2L3_IterativeCone5PFJets -> Clear () ;

 m_otherJets_JPT_SisCone5CaloJets -> Clear () ;
 m_otherJets_JPT_IterativeCone5CaloJets -> Clear () ;
 m_otherJets_JPT_SisCone5PFJets -> Clear () ;
 m_otherJets_JPT_IterativeCone5PFJets -> Clear () ;

    
 bTag_SisCone5CaloJets_ -> clear () ;
 bTag_IterativeCone5CaloJets_ -> clear () ;
 bTag_SisCone5PFJets_ -> clear () ;
 bTag_IterativeCone5PFJets_ -> clear () ;
  
 bTag_L2L3_SisCone5CaloJets_ -> clear () ;
 bTag_L2L3_IterativeCone5CaloJets_ -> clear () ;
 bTag_L2L3_SisCone5PFJets_ -> clear () ;
 bTag_L2L3_IterativeCone5PFJets_ -> clear () ;
  
 bTag_JPT_SisCone5CaloJets_ -> clear () ;
 bTag_JPT_IterativeCone5CaloJets_ -> clear () ;
 bTag_JPT_SisCone5PFJets_ -> clear () ;
 bTag_JPT_IterativeCone5PFJets_ -> clear () ;

 
  
 m_otherJets_SisCone5CaloJets_alpha -> clear () ;
 m_otherJets_IterativeCone5CaloJets_alpha -> clear () ;
 m_otherJets_SisCone5PFJets_alpha -> clear () ;
 m_otherJets_IterativeCone5PFJets_alpha -> clear () ;

 m_otherJets_L2L3_SisCone5CaloJets_alpha -> clear () ;
 m_otherJets_L2L3_IterativeCone5CaloJets_alpha -> clear () ;
 m_otherJets_L2L3_SisCone5PFJets_alpha -> clear () ;
 m_otherJets_L2L3_IterativeCone5PFJets_alpha -> clear () ;

 m_otherJets_JPT_SisCone5CaloJets_alpha -> clear () ;
 m_otherJets_JPT_IterativeCone5CaloJets_alpha -> clear () ;
 m_otherJets_JPT_SisCone5PFJets_alpha -> clear () ;
 m_otherJets_JPT_IterativeCone5PFJets_alpha -> clear () ;
 
 FillKindEvent (iEvent, iSetup);

 FillEle (iEvent, iSetup);
 FillMu (iEvent, iSetup);
 FillMet (iEvent, iSetup);
  //   FillTagJet (iEvent, iSetup); //---- AM --- not now!
 FillJet (iEvent, iSetup, 0);
 FillTracks (iEvent, iSetup);
 FillGenParticles (iEvent, iSetup); //---- AM --- to call after FillKindEvent
 FillGenJet (iEvent, iSetup);
 FillGenMet (iEvent, iSetup);
  
 if (bool_JetTagSisCone5CaloJets_)        FillJet(iEvent, iSetup, 1);
 if (bool_JetTagIterativeCone5CaloJets_)  FillJet(iEvent, iSetup, 2);
 if (bool_JetTagSisCone5PFJets_)          FillJet(iEvent, iSetup, 3);
 if (bool_JetTagIterativeCone5PFJets_)    FillJet(iEvent, iSetup, 4);

 if (bool_JetTagSisCone5CaloJets_L2L3_)    FillJet(iEvent, iSetup, 5);
 if (bool_JetTagIterativeCone5CaloJets_L2L3_)    FillJet(iEvent, iSetup, 6);
 if (bool_JetTagSisCone5PFJets_L2L3_)    FillJet(iEvent, iSetup, 7);
 if (bool_JetTagIterativeCone5PFJets_L2L3_)    FillJet(iEvent, iSetup, 8);
 
 if (bool_JetTagSisCone5CaloJets_JPT_)    FillJet(iEvent, iSetup, 9);
 if (bool_JetTagIterativeCone5CaloJets_JPT_)    FillJet(iEvent, iSetup, 10);
 if (bool_JetTagSisCone5PFJets_JPT_)    FillJet(iEvent, iSetup, 11);
 if (bool_JetTagIterativeCone5PFJets_JPT_)    FillJet(iEvent, iSetup, 12);

 if (bool_JetTagIterativeCone5CaloJets_BTagging_)    FillJet(iEvent, iSetup, 13);
  
 mytree_->Fill();
 
}


// --------------------------------------------------------------------

void SimpleNtple::FillKindEvent(const edm::Event& iEvent, const edm::EventSetup& iSetup){
 
 edm::Handle<edm::HepMCProduct> evtMC;
 
 iEvent.getByLabel("source", evtMC);
 
 if (evtMC.isValid ()) {
  const HepMC::GenEvent * mcEv = evtMC->GetEvent();
  IdEvent = mcEv->signal_process_id();
 }
 else IdEvent = -1;
 
}


// --------------------------------------------------------------------

void SimpleNtple::FillEle(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  
//  edm::Handle<reco::GsfElectronCollection> EleHandle ;
 edm::Handle<edm::View<reco::GsfElectron> > EleHandle ;

 iEvent.getByLabel (EleTag_,EleHandle) ;

 edm::Handle<edm::View<reco::Track> > TracksHandle ;

 iEvent.getByLabel (TracksTag_,TracksHandle) ;

  //PG get the electron ID collections
 std::vector<edm::Handle<edm::ValueMap<float> > > eleIdHandles(3) ;
 iEvent.getByLabel (m_eleIDPTDRLooseInputTag, eleIdHandles[0]);
 iEvent.getByLabel (m_eleIDPTDRMediumInputTag, eleIdHandles[1]);
 iEvent.getByLabel (m_eleIDPTDRTightInputTag, eleIdHandles[2]);

  
  //---- AM get electron Isolation collections ----
 std::vector<edm::Handle<edm::ValueMap<double> > > eleIsoValueMap(3);

 iEvent.getByLabel( eleTkIsoTag_ , eleIsoValueMap[0] );
 iEvent.getByLabel( eleEcalIsoTag_ , eleIsoValueMap[1] );
 iEvent.getByLabel( eleHcalIsoTag_ , eleIsoValueMap[2] );
  
 const edm::ValueMap<double> & eleIsoTk = * eleIsoValueMap[0] ;
 const edm::ValueMap<double> & eleIsoEcal = * eleIsoValueMap[1] ;
 const edm::ValueMap<double> & eleIsoHcal = * eleIsoValueMap[2] ;
  
  
 if(EleHandle->size() < 30 ){ nEle = EleHandle->size(); }
 else {nEle = 30;}
 
 
 edm::Handle<edm::RefVector<reco::GsfElectronCollection> > EleRefHandle ;
 if(doRefCheckTag_==true) iEvent.getByLabel(EleRefTag_,EleRefHandle);
 
 edm::Handle<reco::GsfElectronCollection> EleHandleNew ;
 iEvent.getByLabel (EleTag_,EleHandleNew) ;

 TClonesArray &electrons = *m_electrons;
 int counter = 0;
 bool selected;

 for(int i=0; i< nEle; i++){
   
  edm::Ref<reco::GsfElectronCollection> electronEdmRef(EleHandleNew,i);
  
  selected = true;
  if(doRefCheckTag_==true)
   if (std::find(EleRefHandle->begin(), EleRefHandle->end(),electronEdmRef) == EleRefHandle->end())
  {
   selected=false;
  }

  if (selected) {
   setMomentum (myvector, (*EleHandleNew)[i].p4());
   new (electrons[counter]) TLorentzVector (myvector);
    
     
    //---- Id and VBF Isolation ----
   
   reco::GsfElectronRef electronReference = EleHandle->refAt (i).castTo<reco::GsfElectronRef> () ;
   edm::Ref<edm::View<reco::GsfElectron> > electronRef(EleHandle,i);
      
   IsolEleSumPt_VBF[counter] = m_tkIsolationAlgo.calcSumOfPt (EleHandle, TracksHandle, electronReference) ;
   IsolEleNTracks_VBF[counter] = m_tkIsolationAlgo.countNumOfTracks (EleHandle, TracksHandle, electronReference) ;

   int times[3] = {1,10,100} ;
   EleId[i]=0;
   for (int j=0 ; j<3 ; ++j)
   {
    if ((*(eleIdHandles[j]))[electronRef] > 0) {
     EleId[counter] += times[j] ;
    }
   }
    
   //---- Isolation ----
   IsolEleECal[counter] = eleIsoEcal[electronRef];    
   IsolEleHCal[counter] = eleIsoHcal[electronRef];
   IsolEleTr[counter] = eleIsoTk[electronRef];
  
   //---- charge ----
   EleCharge[counter] = (*EleHandleNew)[i].charge();
   
  //---- only >= 3.1 ---- from https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideEgammaIsolation ----
//   IsolEleECal[counter] = (*EleHandle)[i].dr03EcalRecHitSumEt();
//   IsolEleHCal[counter] = (*EleHandle)[i].dr03HcalDepth1TowerSumEt();
//   IsolEleTr[counter] = (*EleHandle)[i].dr03TkSumPt();
 
   counter++;  
  }
  
 }

}


// --------------------------------------------------------------------


void SimpleNtple::FillMu(const edm::Event& iEvent, const edm::EventSetup& iSetup){
 edm::Handle<reco::MuonCollection> MuHandle;
 iEvent.getByLabel (MuTag_,MuHandle);

 if(MuHandle->size() < 30 ){ nMu = MuHandle->size(); }
 else {nMu = 30;}
 for(int i=0; i< nMu; i++){
  IsolMuTr[i]     = (*MuHandle)[i].isolationR03().sumPt;   
  IsolMuECal[i]   = (*MuHandle)[i].isolationR03().emEt;   
  IsolMuHCal[i]   = (*MuHandle)[i].isolationR03().hadEt; 

  IsolMuNTracks[i]= (*MuHandle)[i].isolationR03().nTracks;
 }

 TClonesArray &muons = *m_muons;
 int counter = 0;
 for (int i=0; i< nMu; i++)
 {
    //---- charge ----
  MuCharge[counter] = (*MuHandle)[i].charge();
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


void SimpleNtple::FillJet(const edm::Event& iEvent, const edm::EventSetup& iSetup, int kind_algo = 0){

 if (kind_algo==0){
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

 if (kind_algo==1){
  edm::Handle<reco::CaloJetCollection> JetHandle ;
  iEvent.getByLabel (JetTagSisCone5CaloJets_, JetHandle) ;
  TClonesArray &jetOther = *m_otherJets_SisCone5CaloJets;
  int counter = 0;
  for (CaloJetCollection::const_iterator jet2 = JetHandle->begin (); 
       jet2 != JetHandle->end (); 
       ++jet2 ) 
  { 
   m_otherJets_SisCone5CaloJets_alpha->push_back(jet2->emEnergyFraction());
   setMomentum (myvector, jet2->p4());        
   new (jetOther[counter]) TLorentzVector (myvector);
   counter++;
  }
 }

 if (kind_algo==2){
  edm::Handle<reco::CaloJetCollection> JetHandle ;
  iEvent.getByLabel (JetTagIterativeCone5CaloJets_, JetHandle) ;
  TClonesArray &jetOther = *m_otherJets_IterativeCone5CaloJets;
  int counter = 0;
  for (CaloJetCollection::const_iterator jet2 = JetHandle->begin (); 
       jet2 != JetHandle->end (); 
       ++jet2 ) 
  { 
   m_otherJets_IterativeCone5CaloJets_alpha->push_back(jet2->emEnergyFraction());
   setMomentum (myvector, jet2->p4());        
   new (jetOther[counter]) TLorentzVector (myvector);
   counter++;
  }
 }

 if (kind_algo==3){
  edm::Handle<reco::PFJetCollection> JetHandle ;
  iEvent.getByLabel (JetTagSisCone5PFJets_, JetHandle) ;
  TClonesArray &jetOther = *m_otherJets_SisCone5PFJets;
  int counter = 0;
  for (PFJetCollection::const_iterator jet2 = JetHandle->begin (); 
       jet2 != JetHandle->end (); 
       ++jet2 ) 
  { 
   m_otherJets_SisCone5PFJets_alpha->push_back((jet2->chargedEmEnergy()+jet2->neutralEmEnergy())/( jet2->energy()));
   setMomentum (myvector, jet2->p4());        
   new (jetOther[counter]) TLorentzVector (myvector);
   counter++;
  }
 }

 if (kind_algo==4){
  edm::Handle<reco::PFJetCollection> JetHandle ;
  iEvent.getByLabel (JetTagIterativeCone5PFJets_, JetHandle) ;
  TClonesArray &jetOther = *m_otherJets_IterativeCone5PFJets;
  int counter = 0;
  for (PFJetCollection::const_iterator jet2 = JetHandle->begin (); 
       jet2 != JetHandle->end (); 
       ++jet2 ) 
  { 
   m_otherJets_IterativeCone5PFJets_alpha->push_back((jet2->chargedEmEnergy()+jet2->neutralEmEnergy())/( jet2->energy()));
   setMomentum (myvector, jet2->p4());        
   new (jetOther[counter]) TLorentzVector (myvector);
   counter++;
  }
 }
 
 
 if (kind_algo==5){
  edm::Handle<reco::CaloJetCollection> JetHandle ;
  iEvent.getByLabel (JetTagSisCone5CaloJets_L2L3_, JetHandle) ;
  TClonesArray &jetOther = *m_otherJets_L2L3_SisCone5CaloJets;
  int counter = 0;
  for (CaloJetCollection::const_iterator jet2 = JetHandle->begin (); 
       jet2 != JetHandle->end (); 
       ++jet2 ) 
  { 
   m_otherJets_L2L3_SisCone5CaloJets_alpha->push_back(jet2->emEnergyFraction());
   setMomentum (myvector, jet2->p4());        
   new (jetOther[counter]) TLorentzVector (myvector);
   counter++;
  }
 }

 if (kind_algo==6){
  edm::Handle<reco::CaloJetCollection> JetHandle ;
  iEvent.getByLabel (JetTagIterativeCone5CaloJets_L2L3_, JetHandle) ;
  TClonesArray &jetOther = *m_otherJets_L2L3_IterativeCone5CaloJets;
  int counter = 0;
  for (CaloJetCollection::const_iterator jet2 = JetHandle->begin (); 
       jet2 != JetHandle->end (); 
       ++jet2 ) 
  { 
   m_otherJets_L2L3_IterativeCone5CaloJets_alpha->push_back(jet2->emEnergyFraction());
   setMomentum (myvector, jet2->p4());        
   new (jetOther[counter]) TLorentzVector (myvector);
   counter++;
  }
 }

 if (kind_algo==7){
  edm::Handle<reco::PFJetCollection> JetHandle ;
  iEvent.getByLabel (JetTagSisCone5PFJets_L2L3_, JetHandle) ;
  TClonesArray &jetOther = *m_otherJets_L2L3_SisCone5PFJets;
  int counter = 0;
  for (PFJetCollection::const_iterator jet2 = JetHandle->begin (); 
       jet2 != JetHandle->end (); 
       ++jet2 ) 
  { 
   m_otherJets_L2L3_SisCone5PFJets_alpha->push_back((jet2->chargedEmEnergy()+jet2->neutralEmEnergy())/( jet2->energy()));
   setMomentum (myvector, jet2->p4());        
   new (jetOther[counter]) TLorentzVector (myvector);
   counter++;
  }
 }

 if (kind_algo==8){
  edm::Handle<reco::PFJetCollection> JetHandle ;
  iEvent.getByLabel (JetTagIterativeCone5PFJets_L2L3_, JetHandle) ;
  TClonesArray &jetOther = *m_otherJets_L2L3_IterativeCone5PFJets;
  int counter = 0;
  for (PFJetCollection::const_iterator jet2 = JetHandle->begin (); 
       jet2 != JetHandle->end (); 
       ++jet2 ) 
  { 
   m_otherJets_L2L3_IterativeCone5PFJets_alpha->push_back((jet2->chargedEmEnergy()+jet2->neutralEmEnergy())/( jet2->energy()));
   setMomentum (myvector, jet2->p4());        
   new (jetOther[counter]) TLorentzVector (myvector);
   counter++;
  }
 }
 
 
 
 
 
 if (kind_algo==9){
  edm::Handle<reco::CaloJetCollection> JetHandle ;
  iEvent.getByLabel (JetTagSisCone5CaloJets_JPT_, JetHandle) ;
  TClonesArray &jetOther = *m_otherJets_JPT_SisCone5CaloJets;
  int counter = 0;
  for (CaloJetCollection::const_iterator jet2 = JetHandle->begin (); 
       jet2 != JetHandle->end (); 
       ++jet2 ) 
  { 
   m_otherJets_JPT_SisCone5CaloJets_alpha->push_back(jet2->emEnergyFraction());
   setMomentum (myvector, jet2->p4());        
   new (jetOther[counter]) TLorentzVector (myvector);
   counter++;
  }
 }

 if (kind_algo==10){
  edm::Handle<reco::CaloJetCollection> JetHandle ;
  iEvent.getByLabel (JetTagIterativeCone5CaloJets_JPT_, JetHandle) ;
  TClonesArray &jetOther = *m_otherJets_JPT_IterativeCone5CaloJets;
  int counter = 0;
  for (CaloJetCollection::const_iterator jet2 = JetHandle->begin (); 
       jet2 != JetHandle->end (); 
       ++jet2 ) 
  { 
   m_otherJets_JPT_IterativeCone5CaloJets_alpha->push_back(jet2->emEnergyFraction());
   setMomentum (myvector, jet2->p4());        
   new (jetOther[counter]) TLorentzVector (myvector);
   counter++;
  }
 }

 if (kind_algo==11){
  edm::Handle<reco::PFJetCollection> JetHandle ;
  iEvent.getByLabel (JetTagSisCone5PFJets_JPT_, JetHandle) ;
  TClonesArray &jetOther = *m_otherJets_JPT_SisCone5PFJets;
  int counter = 0;
  for (PFJetCollection::const_iterator jet2 = JetHandle->begin (); 
       jet2 != JetHandle->end (); 
       ++jet2 ) 
  { 
   m_otherJets_JPT_SisCone5PFJets_alpha->push_back((jet2->chargedEmEnergy()+jet2->neutralEmEnergy())/( jet2->energy()));
   setMomentum (myvector, jet2->p4());        
   new (jetOther[counter]) TLorentzVector (myvector);
   counter++;
  }
 }

 if (kind_algo==12){
  edm::Handle<reco::PFJetCollection> JetHandle ;
  iEvent.getByLabel (JetTagIterativeCone5PFJets_JPT_, JetHandle) ;
  TClonesArray &jetOther = *m_otherJets_JPT_IterativeCone5PFJets;
  int counter = 0;
  for (PFJetCollection::const_iterator jet2 = JetHandle->begin (); 
       jet2 != JetHandle->end (); 
       ++jet2 ) 
  { 
   m_otherJets_JPT_IterativeCone5PFJets_alpha->push_back((jet2->chargedEmEnergy()+jet2->neutralEmEnergy())/( jet2->energy()));
   setMomentum (myvector, jet2->p4());        
   new (jetOther[counter]) TLorentzVector (myvector);
   counter++;
  }
 }
 
 
 
 
 if (kind_algo==13){
  //---- b Tagging ----
  
  edm::Handle<reco::JetTagCollection> bTagHandle;
  iEvent.getByLabel("newJetBProbabilityBJetTags", bTagHandle);
  const reco::JetTagCollection & bTags = *(bTagHandle.product());

   //---- Loop over jets and study b tag info.
//   TClonesArray &jetOther = *m_otherJets_IterativeCone5CaloJets_Btag;
  int counter = 0;
  for (int i = 0; i != bTags.size(); ++i) {
   cout<<" Jet "<< i 
     <<" has b tag discriminator = "<<bTags[i].second
     << " and jet Pt = "<<bTags[i].first->pt()<<endl;
   
//    bTag_ -> push_back(bTags[i].second);
   
//    setMomentum (myvector, bTags[i].first->p4());        
//    new (jetOther[counter]) TLorentzVector (myvector);
//    counter++;
  }   
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


void 
  SimpleNtple::FillGenParticles(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 edm::Handle<reco::GenParticleCollection> genParticlesHandle; 
 iEvent.getByLabel (MCtruthTag_,genParticlesHandle);
 
 TClonesArray &genParticles = *m_genParticles;
 TClonesArray &HiggsParticle = *m_HiggsParticle;
 
 int counter = 0;
 int counter_Higgs = 0;
   
 if (IdEvent==123 || IdEvent==124){//---- only if VBF
  for (reco::GenParticleCollection::const_iterator genIt = genParticlesHandle->begin (); 
       genIt != genParticlesHandle->end (); 
       ++genIt ) 
  {
   
   Int_t pdg = genIt->pdgId();
   Int_t status = genIt->status();
   Int_t mother1 = 0;
   if (genIt->numberOfMothers()>0) mother1 = genIt->mother(0)->pdgId();
   Int_t mother2 = 0;
   if (genIt->numberOfMothers()>1) mother2 = genIt->mother(1)->pdgId();
   Int_t daughter1 = 0;
   if (genIt->numberOfDaughters()>0) daughter1 = genIt->daughter(0)->pdgId();
   Int_t daughter2 = 0;
   if (genIt->numberOfDaughters()>1) daughter2 = genIt->daughter(1)->pdgId();
   Double_t px = genIt->px();
   Double_t py = genIt->py();
   Double_t pz = genIt->pz();
   Double_t etot = genIt->energy();
   Double_t vx = 0;
   Double_t vy = 0;
   Double_t vz = 0;
   Double_t time = 0;
  
   new (genParticles[counter]) TParticle (pdg, status, mother1, mother2, daughter1, daughter2, px, py, pz, etot, vx, vy, vz, time);
 
   if (counter == 6) { //---- q1
//     q1 = new TParticle (pdg, status, mother1, mother2, daughter1, daughter2, px, py, pz, etot, vx, vy, vz, time);
    new (HiggsParticle[counter_Higgs]) TParticle (pdg, status, mother1, mother2, daughter1, daughter2, px, py, pz, etot, vx, vy, vz, time);
    counter_Higgs++;
   }
   if (counter == 7) { //---- q2
//     q2 = new TParticle (pdg, status, mother1, mother2, daughter1, daughter2, px, py, pz, etot, vx, vy, vz, time);
    new (HiggsParticle[counter_Higgs]) TParticle (pdg, status, mother1, mother2, daughter1, daughter2, px, py, pz, etot, vx, vy, vz, time);
    counter_Higgs++;
   }
 
   counter++;
  }
 }
 
 
  //---- Fill Higgs ----
 if (IdEvent==123 || IdEvent==124){//---- only if VBF
  for (reco::GenParticleCollection::const_iterator genIt = genParticlesHandle->begin (); 
       genIt != genParticlesHandle->end (); 
       ++genIt ) 
  { //---- loop over GenParticles ----
   Int_t pdg = genIt->pdgId();
   Int_t status = genIt->status();
   Int_t mother1 = 0;
   if (genIt->numberOfMothers()>0) mother1 = genIt->mother(0)->pdgId();
   Int_t mother2 = 0;
   if (genIt->numberOfMothers()>1) mother2 = genIt->mother(1)->pdgId();
   Int_t daughter1 = 0;
   if (genIt->numberOfDaughters()>0) daughter1 = genIt->daughter(0)->pdgId();
   Int_t daughter2 = 0;
   if (genIt->numberOfDaughters()>1) daughter2 = genIt->daughter(1)->pdgId();
   Double_t px = genIt->px();
   Double_t py = genIt->py();
   Double_t pz = genIt->pz();
   Double_t etot = genIt->energy();
   Double_t vx = 0;
   Double_t vy = 0;
   Double_t vz = 0;
   Double_t time = 0;
   
   if (pdg == 25 && status==3) { //---- Higgs ----
//     Higgs = new TParticle (pdg, status, mother1, mother2, daughter1, daughter2, px, py, pz, etot, vx, vy, vz, time);
    new (HiggsParticle[counter_Higgs]) TParticle (pdg, status, mother1, mother2, daughter1, daughter2, px, py, pz, etot, vx, vy, vz, time);
    counter_Higgs++;
    
    bool bool_V1 = false; 
    int numDaughters = genIt->numberOfDaughters();
    for (int ii=0; ii<numDaughters; ii++){ //---- loop over daughters of Higgs ----
     const reco::Candidate * daughter = genIt->daughter(ii);
     
     Int_t pdg_ii = daughter->pdgId();
     Int_t status_ii = daughter->status();
     Int_t mother1_ii = 0;
     if (daughter->numberOfMothers()>0) mother1_ii = daughter->mother(0)->pdgId();
     Int_t mother2_ii = 0;
     if (daughter->numberOfMothers()>1) mother2_ii = daughter->mother(1)->pdgId();
     Int_t daughter1_ii = 0;
     if (daughter->numberOfDaughters()>0) daughter1_ii = daughter->daughter(0)->pdgId();
     Int_t daughter2_ii = 0;
     if (daughter->numberOfDaughters()>1) daughter2_ii = daughter->daughter(1)->pdgId();
     Double_t px_ii = daughter->px();
     Double_t py_ii = daughter->py();
     Double_t pz_ii = daughter->pz();
     Double_t etot_ii = daughter->energy();
     Double_t vx_ii = 0;
     Double_t vy_ii = 0;
     Double_t vz_ii = 0;
     Double_t time_ii = 0;

     if ((abs(pdg_ii) == 24 || pdg_ii == 23 ) && status_ii==3 && mother1_ii == 25) { //---- W or Z ----
      if (!bool_V1) { //---- V1 ----
//        V1 = new TParticle (pdg_ii, status_ii, mother1_ii, mother2_ii, daughter1_ii, daughter2_ii, px_ii, py_ii, pz_ii, etot_ii, vx_ii, vy_ii, vz_ii, time_ii);
       new (HiggsParticle[counter_Higgs]) TParticle (pdg_ii, status_ii, mother1_ii, mother2_ii, daughter1_ii, daughter2_ii, px_ii, py_ii, pz_ii, etot_ii, vx_ii, vy_ii, vz_ii, time_ii);
       counter_Higgs++;
       
       bool_V1 = true;
       
       int numDaughters_V1 = daughter->numberOfDaughters();
       bool bool_f1 = false;
       for (int jj=0; jj<numDaughters_V1; jj++){   
        
        const reco::Candidate * daughter_daughter = daughter->daughter(jj);
     
        Int_t pdg_jj = daughter_daughter->pdgId();
        Int_t status_jj = daughter_daughter->status();
        Int_t mother1_jj = 0;
        if (daughter_daughter->numberOfMothers()>0) mother1_jj = daughter_daughter->mother(0)->pdgId();
        Int_t mother2_jj = 0;
        if (daughter_daughter->numberOfMothers()>1) mother2_jj = daughter_daughter->mother(1)->pdgId();
        Int_t daughter1_jj = 0;
        if (daughter_daughter->numberOfDaughters()>0) daughter1_jj = daughter_daughter->daughter(0)->pdgId();
        Int_t daughter2_jj = 0;
        if (daughter_daughter->numberOfDaughters()>1) daughter2_jj = daughter_daughter->daughter(1)->pdgId();
        Double_t px_jj = daughter_daughter->px();
        Double_t py_jj = daughter_daughter->py();
        Double_t pz_jj = daughter_daughter->pz();
        Double_t etot_jj = daughter_daughter->energy();
        Double_t vx_jj = 0;
        Double_t vy_jj = 0;
        Double_t vz_jj = 0;
        Double_t time_jj = 0;
        
        if (status_jj==3) {
         if (!bool_f1) {
//           f1V1 = new TParticle (pdg_jj, status_jj, mother1_jj, mother2_jj, daughter1_jj, daughter2_jj, px_jj, py_jj, pz_jj, etot_jj, vx_jj, vy_jj, vz_jj, time_jj);
          new (HiggsParticle[counter_Higgs]) TParticle (pdg_jj, status_jj, mother1_jj, mother2_jj, daughter1_jj, daughter2_jj, px_jj, py_jj, pz_jj, etot_jj, vx_jj, vy_jj, vz_jj, time_jj);
          counter_Higgs++;
          bool_f1 = true;
         }
         else {
//           f2V1 = new TParticle (pdg_jj, status_jj, mother1_jj, mother2_jj, daughter1_jj, daughter2_jj, px_jj, py_jj, pz_jj, etot_jj, vx_jj, vy_jj, vz_jj, time_jj);
          new (HiggsParticle[counter_Higgs]) TParticle (pdg_jj, status_jj, mother1_jj, mother2_jj, daughter1_jj, daughter2_jj, px_jj, py_jj, pz_jj, etot_jj, vx_jj, vy_jj, vz_jj, time_jj);
          counter_Higgs++;
         }
        }
       }
      }
      
      else { //---- V2 ----
//        V2 = new TParticle (pdg_ii, status_ii, mother1_ii, mother2_ii, daughter1_ii, daughter2_ii, px_ii, py_ii, pz_ii, etot_ii, vx_ii, vy_ii, vz_ii, time_ii);
       new (HiggsParticle[counter_Higgs]) TParticle (pdg_ii, status_ii, mother1_ii, mother2_ii, daughter1_ii, daughter2_ii, px_ii, py_ii, pz_ii, etot_ii, vx_ii, vy_ii, vz_ii, time_ii);
       counter_Higgs++;

       
       int numDaughters_V2 = daughter->numberOfDaughters();
       bool bool_f1 = false;
       for (int jj=0; jj<numDaughters_V2; jj++){   
        
        const reco::Candidate * daughter_daughter = daughter->daughter(jj);
     
        Int_t pdg_jj = daughter_daughter->pdgId();
        Int_t status_jj = daughter_daughter->status();
        Int_t mother1_jj = 0;
        if (daughter_daughter->numberOfMothers()>0) mother1_jj = daughter_daughter->mother(0)->pdgId();
        Int_t mother2_jj = 0;
        if (daughter_daughter->numberOfMothers()>1) mother2_jj = daughter_daughter->mother(1)->pdgId();
        Int_t daughter1_jj = 0;
        if (daughter_daughter->numberOfDaughters()>0) daughter1_jj = daughter_daughter->daughter(0)->pdgId();
        Int_t daughter2_jj = 0;
        if (daughter_daughter->numberOfDaughters()>1) daughter2_jj = daughter_daughter->daughter(1)->pdgId();
        Double_t px_jj = daughter_daughter->px();
        Double_t py_jj = daughter_daughter->py();
        Double_t pz_jj = daughter_daughter->pz();
        Double_t etot_jj = daughter_daughter->energy();
        Double_t vx_jj = 0;
        Double_t vy_jj = 0;
        Double_t vz_jj = 0;
        Double_t time_jj = 0;
        
        if (status_jj==3) {
         if (!bool_f1) {
//           f1V2 = new TParticle (pdg_jj, status_jj, mother1_jj, mother2_jj, daughter1_jj, daughter2_jj, px_jj, py_jj, pz_jj, etot_jj, vx_jj, vy_jj, vz_jj, time_jj);
          new (HiggsParticle[counter_Higgs]) TParticle (pdg_jj, status_jj, mother1_jj, mother2_jj, daughter1_jj, daughter2_jj, px_jj, py_jj, pz_jj, etot_jj, vx_jj, vy_jj, vz_jj, time_jj);
          counter_Higgs++;
         
          bool_f1 = true;
         }
         else {
//           f2V2 = new TParticle (pdg_jj, status_jj, mother1_jj, mother2_jj, daughter1_jj, daughter2_jj, px_jj, py_jj, pz_jj, etot_jj, vx_jj, vy_jj, vz_jj, time_jj);
          new (HiggsParticle[counter_Higgs]) TParticle (pdg_jj, status_jj, mother1_jj, mother2_jj, daughter1_jj, daughter2_jj, px_jj, py_jj, pz_jj, etot_jj, vx_jj, vy_jj, vz_jj, time_jj);
          counter_Higgs++;
         
         }
        }
       }
      }
        
     }//---- end W or Z ----
    } //---- end loop over daughters of Higgs ----
   } //---- end Higgs ----
  } //---- end loop over GenParticles ----
 }//---- end only if VBF

}




// --------------------------------------------------------------------


void 
  SimpleNtple::FillGenJet(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 edm::Handle< reco::GenJetCollection > genJetsHandle ;
 iEvent.getByLabel( genJetTag_, genJetsHandle ) ;

 TClonesArray &genJets = *m_genJets;
 int counter = 0;
 for (reco::GenJetCollection::const_iterator gJIt = genJetsHandle->begin (); 
      gJIt != genJetsHandle->end (); 
      ++gJIt ) 
 { 
  
  myvector.SetPx ((*gJIt).px ()) ;
  myvector.SetPy ((*gJIt).py ()) ;
  myvector.SetPz ((*gJIt).pz ()) ;
  myvector.SetE ((*gJIt).energy ()) ;
//   myvector.SetE ((*gJIt).emEnergy () + (*gJIt).hadEnergy ()) ;
  new (genJets[counter]) TLorentzVector (myvector);
  counter++;
 }
}



// --------------------------------------------------------------------


void 
  SimpleNtple::FillGenMet(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 edm::Handle< reco::GenMETCollection > genMetHandle ;
 iEvent.getByLabel( genMetTag_, genMetHandle ) ;

 TClonesArray &genMets = *m_genMet;
 int counter = 0;
 for (reco::GenMETCollection::const_iterator gMIt = genMetHandle->begin (); 
      gMIt != genMetHandle->end (); 
      ++gMIt ) 
 { 
  myvector.SetPx ((*gMIt).px ()) ;
  myvector.SetPy ((*gMIt).py ()) ;
  myvector.SetPz ((*gMIt).pz ()) ;
  myvector.SetE ((*gMIt).energy ()) ;
//   myvector.SetE ((*gMIt).emEnergy () + (*gMIt).hadEnergy ()) ;
  new (genMets[counter]) TLorentzVector (myvector);
  counter++;
 }
}

// --------------------------------------------------------------------


void SimpleNtple::Init(){
 nEle = 0; 
 nMu = 0;
 for (int i=0;i<30;i++){
  IsolEleSumPt_VBF[i]=0;
  IsolEleNTracks_VBF[i]=0;
  IsolEleECal[i]=0;
  IsolEleHCal[i]=0;
  IsolEleTr[i]=0;
  EleId[i]=0;
  EleCharge[i]=0;
  
  IsolMuECal[i]=0;
  IsolMuHCal[i]=0;
  IsolMuTr[i]=0;
  IsolMuSumPt[i]=0;
  IsolMuNTracks[i]=0;
  MuCharge[i]=0;
 }

 MinvTags = -1;
}


// ------------ method called once each job just before starting event loop  ------------


void 
  SimpleNtple::beginJob(const edm::EventSetup& iSetup)
{
   
 bTag_SisCone5CaloJets_ = new std::vector<float>;
 bTag_IterativeCone5CaloJets_ = new std::vector<float>;
 bTag_SisCone5PFJets_ = new std::vector<float>;
 bTag_IterativeCone5PFJets_ = new std::vector<float>;
  
 bTag_L2L3_SisCone5CaloJets_ = new std::vector<float>;
 bTag_L2L3_IterativeCone5CaloJets_ = new std::vector<float>;
 bTag_L2L3_SisCone5PFJets_ = new std::vector<float>;
 bTag_L2L3_IterativeCone5PFJets_ = new std::vector<float>;
  
 bTag_JPT_SisCone5CaloJets_ = new std::vector<float>;
 bTag_JPT_IterativeCone5CaloJets_ = new std::vector<float>;
 bTag_JPT_SisCone5PFJets_ = new std::vector<float>;
 bTag_JPT_IterativeCone5PFJets_ = new std::vector<float>;
 
 
 m_otherJets_SisCone5CaloJets_alpha = new std::vector<float> ;
 m_otherJets_IterativeCone5CaloJets_alpha = new std::vector<float> ;
 m_otherJets_SisCone5PFJets_alpha = new std::vector<float> ;
 m_otherJets_IterativeCone5PFJets_alpha = new std::vector<float> ;

 m_otherJets_L2L3_SisCone5CaloJets_alpha = new std::vector<float> ;
 m_otherJets_L2L3_IterativeCone5CaloJets_alpha = new std::vector<float> ;
 m_otherJets_L2L3_SisCone5PFJets_alpha = new std::vector<float> ;
 m_otherJets_L2L3_IterativeCone5PFJets_alpha = new std::vector<float> ;

 m_otherJets_JPT_SisCone5CaloJets_alpha = new std::vector<float> ;
 m_otherJets_JPT_IterativeCone5CaloJets_alpha = new std::vector<float> ;
 m_otherJets_JPT_SisCone5PFJets_alpha = new std::vector<float> ;
 m_otherJets_JPT_IterativeCone5PFJets_alpha = new std::vector<float> ;
 
 

 mytree_->Branch("m_otherJets_SisCone5CaloJets_alpha","std::vector<float>",&m_otherJets_SisCone5CaloJets_alpha);
 mytree_->Branch("m_otherJets_IterativeCone5CaloJets_alpha","std::vector<float>",&m_otherJets_IterativeCone5CaloJets_alpha);
 mytree_->Branch("m_otherJets_SisCone5PFJets_alpha","std::vector<float>",&m_otherJets_SisCone5PFJets_alpha);
 mytree_->Branch("m_otherJets_IterativeCone5PFJets_alpha","std::vector<float>",&m_otherJets_IterativeCone5PFJets_alpha);
 
 mytree_->Branch("m_otherJets_L2L3_SisCone5CaloJets_alpha","std::vector<float>",&m_otherJets_L2L3_SisCone5CaloJets_alpha);
 mytree_->Branch("m_otherJets_L2L3_IterativeCone5CaloJets_alpha","std::vector<float>",&m_otherJets_L2L3_IterativeCone5CaloJets_alpha);
 mytree_->Branch("m_otherJets_L2L3_SisCone5PFJets_alpha","std::vector<float>",&m_otherJets_L2L3_SisCone5PFJets_alpha);
 mytree_->Branch("m_otherJets_L2L3_IterativeCone5PFJets_alpha","std::vector<float>",&m_otherJets_L2L3_IterativeCone5PFJets_alpha);
 
 mytree_->Branch("m_otherJets_JPT_SisCone5CaloJets_alpha","std::vector<float>",&m_otherJets_JPT_SisCone5CaloJets_alpha);
 mytree_->Branch("m_otherJets_JPT_IterativeCone5CaloJets_alpha","std::vector<float>",&m_otherJets_JPT_IterativeCone5CaloJets_alpha);
 mytree_->Branch("m_otherJets_JPT_SisCone5PFJets_alpha","std::vector<float>",&m_otherJets_JPT_SisCone5PFJets_alpha);
 mytree_->Branch("m_otherJets_JPT_IterativeCone5PFJets_alpha","std::vector<float>",&m_otherJets_JPT_IterativeCone5PFJets_alpha);
 
 
 
 
 
 mytree_->Branch("bTag_SisCone5CaloJets_","std::vector<float>",&bTag_SisCone5CaloJets_);
 mytree_->Branch("bTag_IterativeCone5CaloJets_","std::vector<float>",&bTag_IterativeCone5CaloJets_);
 mytree_->Branch("bTag_SisCone5PFJets_","std::vector<float>",&bTag_SisCone5PFJets_);
 mytree_->Branch("bTag_IterativeCone5PFJets_","std::vector<float>",&bTag_IterativeCone5PFJets_);
 
 mytree_->Branch("bTag_L2L3_SisCone5CaloJets_","std::vector<float>",&bTag_L2L3_SisCone5CaloJets_);
 mytree_->Branch("bTag_L2L3_IterativeCone5CaloJets_","std::vector<float>",&bTag_L2L3_IterativeCone5CaloJets_);
 mytree_->Branch("bTag_L2L3_SisCone5PFJets_","std::vector<float>",&bTag_L2L3_SisCone5PFJets_);
 mytree_->Branch("bTag_L2L3_IterativeCone5PFJets_","std::vector<float>",&bTag_L2L3_IterativeCone5PFJets_);
 
 mytree_->Branch("bTag_JPT_SisCone5CaloJets_","std::vector<float>",&bTag_JPT_SisCone5CaloJets_);
 mytree_->Branch("bTag_JPT_IterativeCone5CaloJets_","std::vector<float>",&bTag_JPT_IterativeCone5CaloJets_);
 mytree_->Branch("bTag_JPT_SisCone5PFJets_","std::vector<float>",&bTag_JPT_SisCone5PFJets_);
 mytree_->Branch("bTag_JPT_IterativeCone5PFJets_","std::vector<float>",&bTag_JPT_IterativeCone5PFJets_);
  
 
 
 mytree_->Branch("IdEvent",&IdEvent,"IdEvent/I");
 
 mytree_->Branch("nEle",&nEle,"nEle/I");
 mytree_->Branch("IsolEleSumPt_VBF",IsolEleSumPt_VBF,"IsolEleSumPt_VBF[30]/F");
 mytree_->Branch("IsolEleNTracks_VBF",IsolEleNTracks_VBF,"IsolEleNTracks_VBF[30]/F");
 mytree_->Branch("IsolEleECal",IsolEleECal,"IsolEleECal[30]/F");
 mytree_->Branch("IsolEleHCal",IsolEleHCal,"IsolEleHCal[30]/F");
 mytree_->Branch("IsolEleTr",IsolEleTr,"IsolEleTr[30]/F");
 mytree_->Branch("EleId",EleId,"EleId[30]/I");
 mytree_->Branch("EleCharge",EleCharge,"EleCharge[30]/I");

 mytree_->Branch("nMu",&nMu,"nMu/I");
 mytree_->Branch("IsolMuECal",IsolMuECal,"IsolMuECal[30]/F");
 mytree_->Branch("IsolMuHCal",IsolMuHCal,"IsolMuHCal[30]/F");
 mytree_->Branch("IsolMuTr",IsolMuTr,"IsolMuTr[30]/F");
//   mytree_->Branch("IsolMuSumPt",IsolMuSumPt,"IsolMuSumPt[30]/F");
 mytree_->Branch("IsolMuNTracks",IsolMuNTracks,"IsolMuNTracks[30]/F");
 mytree_->Branch("MuCharge",MuCharge,"MuCharge[30]/I");
 
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

  // vector of the TLorentz Vectors of other genParticles
 m_genParticles = new TClonesArray ("TParticle");
 mytree_->Branch ("genParticles", "TClonesArray", &m_genParticles, 256000,0);

  // vector of the TLorentz Vectors of other genJets
 m_genJets = new TClonesArray ("TLorentzVector");
 mytree_->Branch ("genJets", "TClonesArray", &m_genJets, 256000,0);

  // vector of the TLorentz Vectors of other genMet
 m_genMet = new TClonesArray ("TLorentzVector");
 mytree_->Branch ("genMet", "TClonesArray", &m_genMet, 256000,0);

  
  // vector of the TLorentz Vectors of other jets
 m_otherJets_SisCone5CaloJets = new TClonesArray ("TLorentzVector");
 mytree_->Branch ("otherJets_SisCone5CaloJets", "TClonesArray", &m_otherJets_SisCone5CaloJets, 256000,0);

  // vector of the TLorentz Vectors of other jets
 m_otherJets_IterativeCone5CaloJets = new TClonesArray ("TLorentzVector");
 mytree_->Branch ("otherJets_IterativeCone5CaloJets", "TClonesArray", &m_otherJets_IterativeCone5CaloJets, 256000,0);

  // vector of the TLorentz Vectors of other jets
 m_otherJets_SisCone5PFJets = new TClonesArray ("TLorentzVector");
 mytree_->Branch ("otherJets_SisCone5PFJets", "TClonesArray", &m_otherJets_SisCone5PFJets, 256000,0);

  // vector of the TLorentz Vectors of other jets
 m_otherJets_IterativeCone5PFJets = new TClonesArray ("TLorentzVector");
 mytree_->Branch ("otherJets_IterativeCone5PFJets", "TClonesArray", &m_otherJets_IterativeCone5PFJets, 256000,0);

  
  
  
   // vector of the TLorentz Vectors of other jets
 m_otherJets_L2L3_SisCone5CaloJets = new TClonesArray ("TLorentzVector");
 mytree_->Branch ("otherJets_L2L3_SisCone5CaloJets", "TClonesArray", &m_otherJets_L2L3_SisCone5CaloJets, 256000,0);

  // vector of the TLorentz Vectors of other jets
 m_otherJets_L2L3_IterativeCone5CaloJets = new TClonesArray ("TLorentzVector");
 mytree_->Branch ("otherJets_L2L3_IterativeCone5CaloJets", "TClonesArray", &m_otherJets_L2L3_IterativeCone5CaloJets, 256000,0);

  // vector of the TLorentz Vectors of other jets
 m_otherJets_L2L3_SisCone5PFJets = new TClonesArray ("TLorentzVector");
 mytree_->Branch ("otherJets_L2L3_SisCone5PFJets", "TClonesArray", &m_otherJets_L2L3_SisCone5PFJets, 256000,0);

  // vector of the TLorentz Vectors of other jets
 m_otherJets_L2L3_IterativeCone5PFJets = new TClonesArray ("TLorentzVector");
 mytree_->Branch ("otherJets_L2L3_IterativeCone5PFJets", "TClonesArray", &m_otherJets_L2L3_IterativeCone5PFJets, 256000,0);

  
  
  
  
   // vector of the TLorentz Vectors of other jets
 m_otherJets_JPT_SisCone5CaloJets = new TClonesArray ("TLorentzVector");
 mytree_->Branch ("otherJets_JPT_SisCone5CaloJets", "TClonesArray", &m_otherJets_JPT_SisCone5CaloJets, 256000,0);

  // vector of the TLorentz Vectors of other jets
 m_otherJets_JPT_IterativeCone5CaloJets = new TClonesArray ("TLorentzVector");
 mytree_->Branch ("otherJets_JPT_IterativeCone5CaloJets", "TClonesArray", &m_otherJets_JPT_IterativeCone5CaloJets, 256000,0);

  // vector of the TLorentz Vectors of other jets
 m_otherJets_JPT_SisCone5PFJets = new TClonesArray ("TLorentzVector");
 mytree_->Branch ("otherJets_JPT_SisCone5PFJets", "TClonesArray", &m_otherJets_JPT_SisCone5PFJets, 256000,0);

  // vector of the TLorentz Vectors of other jets
 m_otherJets_JPT_IterativeCone5PFJets = new TClonesArray ("TLorentzVector");
 mytree_->Branch ("otherJets_JPT_IterativeCone5PFJets", "TClonesArray", &m_otherJets_JPT_IterativeCone5PFJets, 256000,0);

 //! MC information

 m_HiggsParticle = new TClonesArray ("TParticle");
 mytree_->Branch ("HiggsParticle", "TClonesArray", &m_HiggsParticle, 256000,0);
 
}


// ------------ method called once each job just after ending the event loop  ------------


void 
  SimpleNtple::endJob() {

 delete bTag_SisCone5CaloJets_;
 delete bTag_IterativeCone5CaloJets_;
 delete bTag_SisCone5PFJets_;
 delete bTag_IterativeCone5PFJets_;
  
 delete bTag_L2L3_SisCone5CaloJets_;
 delete bTag_L2L3_IterativeCone5CaloJets_;
 delete bTag_L2L3_SisCone5PFJets_;
 delete bTag_L2L3_IterativeCone5PFJets_;
  
 delete bTag_JPT_SisCone5CaloJets_;
 delete bTag_JPT_IterativeCone5CaloJets_;
 delete bTag_JPT_SisCone5PFJets_;
 delete bTag_JPT_IterativeCone5PFJets_; 
 
 delete m_otherJets_SisCone5CaloJets_alpha ;
 delete m_otherJets_IterativeCone5CaloJets_alpha ;
 delete m_otherJets_SisCone5PFJets_alpha ;
 delete m_otherJets_IterativeCone5PFJets_alpha ;
  
 delete m_otherJets_L2L3_SisCone5CaloJets_alpha ;
 delete m_otherJets_L2L3_IterativeCone5CaloJets_alpha ;
 delete m_otherJets_L2L3_SisCone5PFJets_alpha ;
 delete m_otherJets_L2L3_IterativeCone5PFJets_alpha ;
  
 delete m_otherJets_JPT_SisCone5CaloJets_alpha ;
 delete m_otherJets_JPT_IterativeCone5CaloJets_alpha ;
 delete m_otherJets_JPT_SisCone5PFJets_alpha ;
 delete m_otherJets_JPT_IterativeCone5PFJets_alpha ; 
 
 
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

