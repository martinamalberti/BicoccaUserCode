#ifndef VBFJetTaggerAfterCutsAfterCuts_h
#define VBFJetTaggerAfterCutsAfterCuts_h

/** \class VBFJetTaggerAfterCutsAfterCuts
 *
 * Object selector perform electron track isolation selection
 * $Id: VBFJetTaggerAfterCuts.h,v 1.2 2008/03/19 15:36:57 govoni Exp $
 *
 */  
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
  
//#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/EventPrincipal.h" 
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DataFormats/Common/interface/Handle.h" 
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFUtils.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidate.h>
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h>
#include <Math/VectorUtil.h>

#include <string>

template <typename T>
class VBFJetTaggerAfterCuts: public edm::EDProducer
{

 public:

  //! ctor  
  VBFJetTaggerAfterCuts (const edm::ParameterSet& conf) ;  
  //! dtor
  virtual ~VBFJetTaggerAfterCuts () {} ;
  
  typedef math::XYZTLorentzVector LorentzVector ;
  typedef std::vector<LorentzVector> LorentzVectorCollection ;
  typedef reco::CaloJetCollection collection ;
  typedef reco::CaloJetRef jet ;
//  typedef std::vector<const reco::PixelMatchGsfElectron *> container;
  typedef std::vector<reco::CaloJetRef> container ;
  typedef container::const_iterator const_iterator ;

  //! begin job
  void beginJob ( const edm::EventSetup & ) {} ;
  //! actual produce method
  void produce (edm::Event &, const edm::EventSetup&) ;
  //! end job
  void endJob () {} ;

 private:
 
  //! input collection
  edm::InputTag m_jetInputTag ;
  //! output collection
  std::string m_tagJetsName ;
  //! residual jets collection
  std::string m_otherJetsName ;

  //! |eta| threshold below which look for jets
  double m_jetEtaMax ;
  //! jet pt minimum threshold above which look for jets
  double m_jetPtMin ;
  //! size of the cone inside which other jets are summed 
  //! to the leading tag ones
  double m_gatherConeSize ;


};


// ----------------------------------------------------------------------------

  
template <typename T>
VBFJetTaggerAfterCuts<T>::VBFJetTaggerAfterCuts (const edm::ParameterSet& iConfig) :
  m_jetInputTag (iConfig.getParameter<edm::InputTag> ("jetInputTag")) ,
  m_tagJetsName (iConfig.getParameter<std::string> ("tagJetsName")) ,
  m_otherJetsName (iConfig.getParameter<std::string> ("otherJetsName")) ,
  m_jetEtaMax (iConfig.getParameter<double> ("jetEtaMax")) ,
  m_jetPtMin (iConfig.getParameter<double> ("jetPtMin")) ,
  m_gatherConeSize (iConfig.getParameter<double> ("gatherConeSize"))
{
//  produces<LorentzVectorCollection> (m_tagJetsName) ;
  produces<reco::RecoChargedCandidateCollection> (m_tagJetsName) ;
  produces<reco::CaloJetCollection> (m_otherJetsName) ;
  //PG this has maybe to be changed into a RefVector in the future
}  


// ----------------------------------------------------------------------------

  
template <class T>
void 
VBFJetTaggerAfterCuts<T>::produce (edm::Event& iEvent, 
                                   const edm::EventSetup& iEventSetup)
{
  edm::Handle<reco::CaloJetCollection> jetCollectionHandle ;
  iEvent.getByLabel (m_jetInputTag, jetCollectionHandle) ;

  if (jetCollectionHandle->size () == 0)
    {
      //VT create void collection
      std::auto_ptr<reco::RecoChargedCandidateCollection> tagJets (new reco::RecoChargedCandidateCollection) ;
      std::auto_ptr<reco::CaloJetCollection> otherJets (new reco::CaloJetCollection) ;

      //VT insert the collection into the event
      iEvent.put (tagJets, m_tagJetsName) ;
      iEvent.put (otherJets, m_otherJetsName) ;

      return;
    }  

  else if (jetCollectionHandle->size () == 1)
    {
      //VT create void collection                                                                                                                                     
      std::auto_ptr<reco::RecoChargedCandidateCollection> tagJets (new reco::RecoChargedCandidateCollection) ;
      std::auto_ptr<reco::CaloJetCollection> otherJets (new reco::CaloJetCollection) ;

      //VT insert the collection into the event
      otherJets -> push_back (*(jetCollectionHandle->begin ())) ;
      iEvent.put (tagJets, m_tagJetsName) ;
      iEvent.put (otherJets, m_otherJetsName) ;

      return;
    }

  //PG get the jet tags
  std::pair<vbfhww2l::VBFjetIt,vbfhww2l::VBFjetIt> tagJetCands = 
     vbfhww2l::findTagJetsInSelected<T> (jetCollectionHandle->begin (), jetCollectionHandle->end ()) ; 
//                               ,m_jetPtMin, m_jetEtaMax) ;

  //PG build the new jets
  LorentzVector firstTag = tagJetCands.first->p4 () ;
  LorentzVector secondTag = tagJetCands.second->p4 () ;
  
  math::XYZVector firstDir = tagJetCands.first->momentum () ;
  math::XYZVector secondDir = tagJetCands.second->momentum () ;
  //PG look for other jets in cones around the found ones and add them to the "leading"
  
  std::auto_ptr<reco::CaloJetCollection> otherJets 
      (new reco::CaloJetCollection) ;

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


  
#endif
 


