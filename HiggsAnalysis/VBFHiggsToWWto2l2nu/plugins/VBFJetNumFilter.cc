// $Id: VBFJetNumFilter.cc,v 1.1 2008/04/02 13:58:29 tancini Exp $

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFJetNumFilter.h"

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
