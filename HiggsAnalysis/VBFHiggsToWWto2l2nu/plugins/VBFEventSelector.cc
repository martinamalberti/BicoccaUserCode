// $Id: VBFEventSelector.cc,v 1.2 2009/02/16 14:52:51 amassiro Exp $

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFEventSelector.h"
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidate.h>
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h>

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include <iostream>
#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include <Math/VectorUtil.h>

//! ctor
VBFEventSelector::VBFEventSelector (const edm::ParameterSet& iConfig) :
  m_tagJetInputTag (iConfig.getParameter<edm::InputTag> ("tagJetInputTag")) ,
  m_jetDeltaEtaMin (iConfig.getParameter<double> ("jetDeltaEtaMin")) ,
  m_jetPtMin (iConfig.getParameter<double> ("jetPtMin")) ,
  m_MjjMin (iConfig.getParameter<double> ("MjjMin"))
{}


// ------------------------------------------------------------------------------------


//! dtor
VBFEventSelector::~VBFEventSelector ()
{}


// ------------------------------------------------------------------------------------


//! filtering method
bool 
VBFEventSelector::filter (edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // Get the tag jets
  edm::Handle<reco::RecoChargedCandidateCollection> jetTagsHandle ;
  iEvent.getByLabel (m_tagJetInputTag, jetTagsHandle) ;

  if (jetTagsHandle->size() != 2) {
   std::cerr << "*** Error: not enough jet tag ***" << std::endl;
   return false;
  }
  
  //AM if there are two tag jets
  LorentzVector firstTag = (*jetTagsHandle)[0].p4 () ;
  LorentzVector secondTag = (*jetTagsHandle)[1].p4 () ;

  
  ///---- pt min selector ----
  double Pt_1 = firstTag.pt () ;
  double Pt_2 = secondTag.pt () ;
  if (Pt_1<m_jetPtMin || Pt_2<m_jetPtMin) return false; 
  
  ///---- Mjj min selector ----
  LorentzVector sumTag = firstTag + secondTag;
  double Mjj = sumTag.mass();
  if (Mjj<m_MjjMin) return false; 
  
  ///---- Delta Eta min selector ----
  double Eta_1 = firstTag.eta () ;
  double Eta_2 = secondTag.eta () ;
  double DeltaEta = Eta_2 - Eta_1;
  if (DeltaEta<0) DeltaEta = -DeltaEta;
  if (DeltaEta<m_jetDeltaEtaMin) return false; 

  ///---- All selection successfull ----
  return true;
  
}
