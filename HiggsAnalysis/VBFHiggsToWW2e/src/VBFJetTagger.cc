#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFJetTagger.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <iostream>
#include <algorithm>


VBFJetTagger::VBFJetTagger (const edm::ParameterSet& iConfig) :
  m_jetInputTag (iConfig.getParameter<edm::InputTag> ("jetInputTag")) ,
  m_jetEtaMax (iConfig.getParameter<double> ("jetEtaMax")) ,
  m_jetPtMin (iConfig.getParameter<double> ("jetPtMin")) 
{
}  


// ----------------------------------------------------------------------------

  
VBFJetTagger::~VBFJetTagger ()
{
}


// ----------------------------------------------------------------------------

  
void VBFJetTagger::select (edm::Handle<VBFJetTagger::collection> c, 
                           const edm::Event& iEvent)
{
  m_selected.clear () ;

  //PG get the jet collection
  edm::Handle<reco::CaloJetCollection> jetCollectionHandle ;
  iEvent.getByLabel (m_jetInputTag, jetCollectionHandle) ;

  //PG get the jet tags
  std::pair<VBFjetIt,VBFjetIt> tagJets = findTagJets (jetCollectionHandle->begin (),
                                                      jetCollectionHandle->end (),
                                                      m_jetPtMin, m_jetEtaMax) ;

  m_selected.push_back (jet (c,tagJets.first - jetCollectionHandle->begin ())) ;
  m_selected.push_back (jet (c,tagJets.second - jetCollectionHandle->begin ())) ;
}

