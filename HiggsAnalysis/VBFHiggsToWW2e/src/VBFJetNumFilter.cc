// $Id: VBFLeptonsNumFilter.cc,v 1.6 2008/02/12 15:17:35 tancini Exp $

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFJetNumFilter.h"

#include <iostream>


//! ctor
VBFJetNumFilter::VBFJetNumFilter (const edm::ParameterSet& iConfig) :
  m_jetInputTag (iConfig.getParameter<edm::InputTag> ("jetInputTag")) ,
  m_minJetNum (iConfig.getParameter<int> ("minJetNum")) ,
  m_maxJetNum (iConfig.getParameter<int> ("maxJetNum"))
{}


// ------------------------------------------------------------------------------------


//! dtor
VBFJetNumFilter::~VBFJetNumFilter ()
{}


// ------------------------------------------------------------------------------------


//! filtering method
bool 
VBFJetNumFilter::filter (edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //VT get the GSF electrons collection
  edm::Handle<reco::CaloJetCollection> jetCollectionHandle ;
  iEvent.getByLabel (m_jetInputTag, jetCollectionHandle) ;

  if ((jetCollectionHandle->size () < m_minJetNum) || (jetCollectionHandle->size () > m_maxJetNum)) return false ;

  return true ;
}
