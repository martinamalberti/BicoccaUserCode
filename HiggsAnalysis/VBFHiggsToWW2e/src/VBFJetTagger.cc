#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFJetTagger.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <iostream>
#include <algorithm>


VBFJetTagger::VBFJetTagger (const edm::ParameterSet& iConfig) :
  m_jetEtaMax (iConfig.getParameter<double> ("jetEtaMax")) ,
  m_jetPtMin (iConfig.getParameter<double> ("jetPtMin")) 
{
}  


// ----------------------------------------------------------------------------

  
VBFJetTagger::~VBFJetTagger ()
{
}


// ----------------------------------------------------------------------------

  
void VBFJetTagger::select (edm::Handle<VBFJetTagger::collection> jetCollectionHandle, 
                           const edm::Event& iEvent)
{
  m_selected.clear () ;

  //PG get the jet tags
  std::pair<VBFjetIt,VBFjetIt> tagJets = findTagJets (jetCollectionHandle->begin (),
                                                      jetCollectionHandle->end (),
                                                      m_jetPtMin, m_jetEtaMax) ;

  m_selected.push_back (jet (jetCollectionHandle,tagJets.first - jetCollectionHandle->begin ())) ;
  m_selected.push_back (jet (jetCollectionHandle,tagJets.second - jetCollectionHandle->begin ())) ;
}

