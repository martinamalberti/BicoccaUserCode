#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFJetTagger.h"

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
  m_otherJetsName (iConfig.getParameter<std::string> ("otherJetsName")) ,
  m_jetEtaMax (iConfig.getParameter<double> ("jetEtaMax")) ,
  m_jetPtMin (iConfig.getParameter<double> ("jetPtMin")) ,
  m_jetDEtaMin (iConfig.getUntrackedParameter<double> ("jetDEtaMin")) ,
  m_jetMjjMin (iConfig.getUntrackedParameter<double> ("jetMjjMin")) ,
  m_gatherConeSize (iConfig.getParameter<double> ("gatherConeSize")) ,
  m_algoType (iConfig.getParameter<int> ("algoType")) 
{
//  produces<LorentzVectorCollection> (m_tagJetsName) ;
  produces<reco::RecoChargedCandidateCollection> (m_tagJetsName) ;
  produces<collection> (m_otherJetsName) ;
  //PG this has maybe to be changed into a RefVector in the future
}  


// ----------------------------------------------------------------------------

  
VBFJetTagger::~VBFJetTagger ()
{
}


// ----------------------------------------------------------------------------

  
void 
VBFJetTagger::produce (edm::Event& iEvent, const edm::EventSetup& iEventSetup)
{
  edm::Handle<collection> jetCollectionHandle ;
  iEvent.getByLabel (m_jetInputTag, jetCollectionHandle) ;

  if (jetCollectionHandle->size () == 0)
    {
      //VT create void collection
      std::auto_ptr<reco::RecoChargedCandidateCollection> tagJets (new reco::RecoChargedCandidateCollection) ;
      std::auto_ptr<collection> otherJets (new collection) ;

      //VT insert the collection into the event
      iEvent.put (tagJets, m_tagJetsName) ;
      iEvent.put (otherJets, m_otherJetsName) ;

      return;
    }  

  else if (jetCollectionHandle->size () == 1)
    {
      //VT create void collection                                                                                                                                     
      std::auto_ptr<reco::RecoChargedCandidateCollection> tagJets (new reco::RecoChargedCandidateCollection) ;
      std::auto_ptr<collection> otherJets (new collection) ;

      //VT insert the collection into the event
      otherJets -> push_back(*(jetCollectionHandle->begin()));
      iEvent.put (tagJets, m_tagJetsName) ;
      iEvent.put (otherJets, m_otherJetsName) ;

      return;
    }

  //PG get the jet tags
  std::pair<vbfhww2l::VBFjetIt,vbfhww2l::VBFjetIt> tagJetCands ;

  if (m_algoType == 0) tagJetCands = vbfhww2l::findTagJets (jetCollectionHandle->begin (), jetCollectionHandle->end (), m_jetPtMin, m_jetEtaMax, m_jetDEtaMin, m_jetMjjMin) ;

  else if (m_algoType == 1) 
    {
     tagJetCands = vbfhww2l::findMaxPtJetsPair (jetCollectionHandle->begin (), jetCollectionHandle->end (), m_jetPtMin, m_jetEtaMax, m_jetDEtaMin, m_jetMjjMin) ;
    }
    else if (m_algoType == 2) 
    {
     tagJetCands = vbfhww2l::findMaxNNJetsPair_PtOrdered (jetCollectionHandle->begin (), jetCollectionHandle->end (), m_jetPtMin, m_jetEtaMax, m_jetDEtaMin, m_jetMjjMin) ;
    }

  //PG build the new jets
  LorentzVector firstTag = tagJetCands.first->p4 () ;
  LorentzVector secondTag = tagJetCands.second->p4 () ;
  
  math::XYZVector firstDir = tagJetCands.first->momentum () ;
  math::XYZVector secondDir = tagJetCands.second->momentum () ;
  //PG look for other jets in cones around the found ones and add them to the "leading"
  
  std::auto_ptr<collection> otherJets 
      (new collection) ;

  //PG loop over the jets collection
  for (vbfhww2l::VBFjetIt jetIt = jetCollectionHandle->begin () ; 
       jetIt != jetCollectionHandle->end () ; 
       ++jetIt) 
    {
      if( (jetIt == tagJetCands.first) || (jetIt == tagJetCands.second) ){continue;}
      double firstDelta = ROOT::Math::VectorUtil::DeltaR (firstDir,jetIt->momentum ()) ;
      double secondDelta = ROOT::Math::VectorUtil::DeltaR (secondDir,jetIt->momentum ()) ;
      if (firstDelta < m_gatherConeSize) 
        {
          if (secondDelta < firstDelta) secondTag += jetIt->p4 () ;
          else firstTag += jetIt->p4 () ;
        }
      else if (secondDelta < m_gatherConeSize) 
	{
         secondTag += jetIt->p4 () ;
	}
      else otherJets->push_back (*jetIt) ;
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
  iEvent.put (otherJets, m_otherJetsName) ;

}

