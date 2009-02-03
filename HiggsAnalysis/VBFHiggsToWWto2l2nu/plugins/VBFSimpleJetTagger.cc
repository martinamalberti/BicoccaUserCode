#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFSimpleJetTagger.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <iostream>
#include <algorithm>


VBFSimpleJetTagger::VBFSimpleJetTagger (const edm::ParameterSet& iConfig) :
  m_jetEtaMax (iConfig.getParameter<double> ("jetEtaMax")) ,
  m_jetPtMin (iConfig.getParameter<double> ("jetPtMin")) 
{
}  


// ----------------------------------------------------------------------------

  
VBFSimpleJetTagger::~VBFSimpleJetTagger ()
{
}


// ----------------------------------------------------------------------------

  
void VBFSimpleJetTagger::select (edm::Handle<VBFSimpleJetTagger::collection> jetCollectionHandle, 
                                 const edm::Event& iEvent,
                                 const edm::EventSetup& iEs)
{
  m_selected.clear () ;

  //PG get the jet tags
  std::pair<vbfhww2l::VBFjetIt,vbfhww2l::VBFjetIt> tagJets = 
               vbfhww2l::findTagJets (jetCollectionHandle->begin (),
                                      jetCollectionHandle->end (),
                                      m_jetPtMin, m_jetEtaMax) ;

  m_selected.push_back (jet (jetCollectionHandle,tagJets.first - jetCollectionHandle->begin ())) ;
  m_selected.push_back (jet (jetCollectionHandle,tagJets.second - jetCollectionHandle->begin ())) ;
}

