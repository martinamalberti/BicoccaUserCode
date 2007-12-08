// $Id: VBFJetVetoFilter.cc,v 1.8 2007/12/07 17:46:35 govoni Exp $

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFJetVetoFilter.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include <iostream>


//! ctor
VBFJetVetoFilter::VBFJetVetoFilter (const edm::ParameterSet& iConfig) :
  m_jetInputTag (iConfig.getParameter<edm::InputTag> ("jetInputTag")) ,
  m_jetEtaMax (iConfig.getParameter<double> ("jetEtaMax")) ,
  m_jetPtMin (iConfig.getParameter<double> ("jetPtMin")) 
{}


// ------------------------------------------------------------------------------------


//! dtor
VBFJetVetoFilter::~VBFJetVetoFilter ()
{}


// ------------------------------------------------------------------------------------


//! filtering method
bool 
VBFJetVetoFilter::filter (edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //PG get the jet collection
  edm::Handle<reco::CaloJetCollection> jetCollectionHandle ;
  iEvent.getByLabel (m_jetInputTag, jetCollectionHandle) ;

  int counter = 0 ;

  typedef reco::CaloJetCollection::const_iterator VBFjetIt ;
  for (VBFjetIt jetIt = jetCollectionHandle->begin () ; 
       jetIt != jetCollectionHandle->end () ; 
       ++jetIt )
     {
       if (fabs (jetIt->eta () > m_jetEtaMax) || 
           jetIt->et () < m_jetPtMin) continue ;
       ++counter ; 
     }

  return counter ;
}
