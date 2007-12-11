#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFJetTagger.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Math/interface/Vector3D.h"
#include <Math/VectorUtil.h>

#include <iostream>
#include <algorithm>


VBFJetTagger::VBFJetTagger (const edm::ParameterSet& iConfig) :
  m_jetInputTag (iConfig.getParameter<edm::InputTag> ("jetInputTag")) ,
  m_tagJetsName (iConfig.getParameter<std::string> ("tagJetsName")) ,
  m_jetEtaMax (iConfig.getParameter<double> ("jetEtaMax")) ,
  m_jetPtMin (iConfig.getParameter<double> ("jetPtMin")) ,
  m_gatherConeSize (iConfig.getParameter<double> ("gatherConeSize")) 
{
  produces<reco::CaloJetCollection> (m_tagJetsName) ;
}  


// ----------------------------------------------------------------------------

  
VBFJetTagger::~VBFJetTagger ()
{
}


// ----------------------------------------------------------------------------

  
void 
VBFJetTagger::produce (edm::Event& iEvent, const edm::EventSetup& iEventSetup)
{

  edm::Handle<reco::CaloJetCollection> jetCollectionHandle ;
  iEvent.getByLabel (m_jetInputTag, jetCollectionHandle) ;

  //PG get the jet tags
  std::pair<VBFjetIt,VBFjetIt> tagJetCands = findTagJets (jetCollectionHandle->begin (),
                                                          jetCollectionHandle->end (),
                                                          m_jetPtMin, m_jetEtaMax) ;

//tagJetCands.first 
//tagJetCands.second

  //PG create the collection to be added to the event
  std::auto_ptr<reco::CaloJetCollection> tagJets (new reco::CaloJetCollection) ;
  
  //PG build the new jets
  reco::CaloJet firstTag = *tagJetCands.first ;
  reco::CaloJet secondTag = *tagJetCands.second ;
  
  math::XYZVector firstDir = firstTag.momentum () ;
  math::XYZVector secondDir = secondTag.momentum () ;
  //PG look for other jets in cones around the found ones and add them to the "leading"
  
  //PG loop over the jets collection
  for (VBFjetIt jetIt = jetCollectionHandle->begin () ; 
       jetIt != jetCollectionHandle->end () ; 
       ++jetIt) 
    {
      ROOT::Math::VectorUtil::DeltaR (firstDir,jetIt->momentum ()) ;
      /* 
        - trovare il modo di fare delta R in maniera efficiente, 
          ricordando le direzioni iniziali dei due getti
        - controllare la distanza da ciascun getto per aggiugnerlo al jet tag
          relativo
          - se il getto sta nei due coni, si accorpa al piu' vicino      
      */
    } //PG loop over the jets collection
  
  //PG fill the collection
    
  //PG insert the collection into the event
  iEvent.put (tagJets, m_tagJetsName) ;
}

