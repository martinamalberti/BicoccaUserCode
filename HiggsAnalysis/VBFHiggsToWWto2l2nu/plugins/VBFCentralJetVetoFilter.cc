// $Id: VBFCentralJetVetoFilter.cc,v 1.3 2008/03/28 08:17:28 govoni Exp $

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFCentralJetVetoFilter.h"
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidate.h>
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h>

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include <iostream>
#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include <Math/VectorUtil.h>

//! ctor
VBFCentralJetVetoFilter::VBFCentralJetVetoFilter (const edm::ParameterSet& iConfig) :
  m_jetInputTag (iConfig.getParameter<edm::InputTag> ("jetInputTag")) ,
  m_tagJetInputTag (iConfig.getParameter<edm::InputTag> ("tagJetInputTag")) ,
  m_jetDeltaEta (iConfig.getParameter<double> ("jetDeltaEta")) ,
  m_jetPtMin (iConfig.getParameter<double> ("jetPtMin")) ,
  m_maxJetsNum (iConfig.getParameter<int> ("maxJetsNum"))
{}


// ------------------------------------------------------------------------------------


//! dtor
VBFCentralJetVetoFilter::~VBFCentralJetVetoFilter ()
{}


// ------------------------------------------------------------------------------------


//! filtering method
bool 
VBFCentralJetVetoFilter::filter (edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //PG get the jet collection
  edm::Handle<reco::CaloJetCollection> jetCollectionHandle ;
  iEvent.getByLabel (m_jetInputTag, jetCollectionHandle) ;

  // Get the tag jets
  edm::Handle<reco::RecoChargedCandidateCollection> jetTagsHandle ;
  iEvent.getByLabel (m_tagJetInputTag, jetTagsHandle) ;

  //PG assume there are two tag jets
  LorentzVector firstTag = (*jetTagsHandle)[0].p4 () ;
  LorentzVector secondTag = (*jetTagsHandle)[1].p4 () ;

  double etaMax = firstTag.eta () ;
  double etaMin = secondTag.eta () ;
  if (etaMin > etaMax) 
    {
      double tempo = etaMin ;
      etaMin = etaMax ; 
      etaMax = tempo ;
    }

  etaMin -= m_jetDeltaEta ;
  etaMax += m_jetDeltaEta ;

  int counter = 0 ;

  typedef reco::CaloJetCollection::const_iterator VBFjetIt ;
  for (VBFjetIt jetIt = jetCollectionHandle->begin () ; 
       jetIt != jetCollectionHandle->end () ; 
       ++jetIt )
     {
       if (jetIt->eta () > etaMax ||
           jetIt->eta () < etaMin ||
           jetIt->et () < m_jetPtMin) continue ;
       ++counter ; 
     }

  if (counter > m_maxJetsNum) return false ;
  return true ;
}
