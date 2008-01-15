#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFJetTagger.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidate.h>
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h>
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
//  produces<LorentzVectorCollection> (m_tagJetsName) ;
  produces<reco::RecoChargedCandidateCollection> (m_tagJetsName) ;
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

  //PG build the new jets
  LorentzVector firstTag = tagJetCands.first->p4 () ;
  LorentzVector secondTag = tagJetCands.second->p4 () ;
  
  math::XYZVector firstDir = tagJetCands.first->momentum () ;
  math::XYZVector secondDir = tagJetCands.first->momentum () ;
  //PG look for other jets in cones around the found ones and add them to the "leading"
  
  //PG loop over the jets collection
  for (VBFjetIt jetIt = jetCollectionHandle->begin () ; 
       jetIt != jetCollectionHandle->end () ; 
       ++jetIt) 
    {
      double firstDelta = ROOT::Math::VectorUtil::DeltaR (firstDir,jetIt->momentum ()) ;
      double secondDelta = ROOT::Math::VectorUtil::DeltaR (secondDir,jetIt->momentum ()) ;
      if (firstDelta < m_gatherConeSize) 
        {
          if (secondDelta < firstDelta) secondTag += jetIt->p4 () ;
          else firstTag += jetIt->p4 () ;
        }
      else secondTag += jetIt->p4 () ;
      /* 
        - controllare la distanza da ciascun getto per aggiugnerlo al jet tag
          relativo
          - se il getto sta nei due coni, si accorpa al piu' vicino      
      */
    } //PG loop over the jets collection
  
  //PG create and fill the collection to be added to the event
//  std::auto_ptr<LorentzVectorCollection> tagJets (new LorentzVectorCollection) ;
//  tagJets->push_back (firstTag) ;  
//  tagJets->push_back (secondTag) ;  

  std::auto_ptr<reco::RecoChargedCandidateCollection> tagJets 
      (new reco::RecoChargedCandidateCollection) ;

  reco::RecoChargedCandidate firstTagCandidate (99,firstTag) ;
  tagJets->push_back (firstTagCandidate) ;  
  reco::RecoChargedCandidate secondTagCandidate (99,secondTag) ;
  tagJets->push_back (secondTagCandidate) ;  
  
  //PG insert the collection into the event
  iEvent.put (tagJets, m_tagJetsName) ;
}

