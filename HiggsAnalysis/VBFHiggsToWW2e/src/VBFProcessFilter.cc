// $Id: VBFProcessFilter.cc,v 1.5 2007/12/07 15:28:49 govoni Exp $

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFProcessFilter.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include <iostream>


//! ctor
VBFProcessFilter::VBFProcessFilter (const edm::ParameterSet& iConfig) :
  m_jetInputTag (iConfig.getParameter<edm::InputTag> ("jetInputTag")) ,
  m_jetEtaMax (iConfig.getParameter<double> ("jetEtaMax")) ,
  m_jetPtMin (iConfig.getParameter<double> ("jetPtMin")) ,
  m_tagJetsEtaMin (iConfig.getParameter<double> ("tagJetsEtaMin")) ,
  m_tagJetsEtaMax (iConfig.getParameter<double> ("tagJetsEtaMax")) ,
  m_tagJetsDeltaEtaMin (iConfig.getParameter<double> ("tagJetsDeltaEtaMin")) ,
  m_tagJetsDeltaEtaMax (iConfig.getParameter<double> ("tagJetsDeltaEtaMax")) ,
  m_tagJetsInvMassMin (iConfig.getParameter<double> ("tagJetsInvMassMin")) ,
  m_tagJetsInvMassMax (iConfig.getParameter<double> ("tagJetsInvMassMax")) ,
  m_checkOpposite (iConfig.getParameter<bool> ("checkOpposite")) 
{}


// ------------------------------------------------------------------------------------


//! dtor
VBFProcessFilter::~VBFProcessFilter ()
{}


// ------------------------------------------------------------------------------------


//! filtering method
bool 
VBFProcessFilter::filter (edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //PG get the jet collection
  edm::Handle<reco::CaloJetCollection> jetCollectionHandle ;
  iEvent.getByLabel (m_jetInputTag, jetCollectionHandle) ;

  //PG get the jet tags
  std::pair<jetIt,jetIt> tagJets = findTagJets (jetCollectionHandle->begin (),
                                                jetCollectionHandle->end ()) ;
  //PG tag jets not found
  if (tagJets.first == tagJets.second) return false ;

  //PG select the event on the basis of the jets kinematics
  if (m_checkOpposite && tagJets.first->eta () * tagJets.second->eta () > 0) return false ;
  double deltaEta = fabs (tagJets.first->eta () - tagJets.second->eta ()) ;
  if (deltaEta < m_tagJetsDeltaEtaMin || deltaEta > m_tagJetsDeltaEtaMax) return false ;
  if (tagJets.first->eta () < m_tagJetsEtaMin || tagJets.first->eta () > m_tagJetsEtaMax ||
      tagJets.second->eta () < m_tagJetsEtaMin || tagJets.second->eta () > m_tagJetsEtaMax) return false ;
  double invMass = (tagJets.first->p4 () + tagJets.second->p4 ()).M () ;
  if (invMass < m_tagJetsInvMassMin || invMass > m_tagJetsInvMassMax) return false ;

  return true ;
}


// ------------------------------------------------------------------------------------


std::pair<VBFProcessFilter::jetIt,VBFProcessFilter::jetIt>	
VBFProcessFilter::findTagJets (VBFProcessFilter::jetIt begin, VBFProcessFilter::jetIt end) 
{

  std::pair<jetIt,jetIt> tagJets (begin,begin) ;
  double maxInvMass = 0. ;

  //PG find the tagging jets

  //PG first loop over jets
  for (jetIt firstJet = begin ; 
       firstJet != end ; 
       ++firstJet ) 
    {
      if (firstJet->et () < m_jetPtMin || 
          fabs (firstJet->eta ()) > m_jetEtaMax) continue ;
      math::XYZTLorentzVector firstLV = firstJet->p4 () ;
      //PG second loop over jets
      for (jetIt secondJet = firstJet + 1 ; 
           secondJet != end ; 
           ++secondJet ) 
        {
          if (secondJet->et () < m_jetPtMin || 
              fabs (secondJet->eta ()) > m_jetEtaMax) continue ;
          math::XYZTLorentzVector sumLV = secondJet->p4 () + firstLV ;
          if (sumLV.M () > maxInvMass)
            {
              maxInvMass = sumLV.M () ;
              tagJets.first = firstJet ;
              tagJets.second = secondJet ;
            }
        } //PG second loop over jets
    } //PG first loop over jets

  return tagJets ;
}
