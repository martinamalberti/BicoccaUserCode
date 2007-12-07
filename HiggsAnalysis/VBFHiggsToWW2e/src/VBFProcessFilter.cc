// $Id: VBFProcessFilter.cc,v 1.7 2007/12/07 16:17:27 govoni Exp $

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFProcessFilter.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include <iostream>


//! ctor
VBFProcessFilter::VBFProcessFilter (const edm::ParameterSet& iConfig) :
  m_jetInputTag (iConfig.getParameter<edm::InputTag> ("jetInputTag")) ,
  m_jetEtaMax (iConfig.getParameter<double> ("jetEtaMax")) ,
  m_jetPtMin (iConfig.getParameter<double> ("jetPtMin")) ,
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
  std::pair<VBFjetIt,VBFjetIt> tagJets = findTagJets (jetCollectionHandle->begin (),
                                                      jetCollectionHandle->end (),
                                                      m_jetPtMin, m_jetEtaMax) ;
  //PG tag jets not found
  if (tagJets.first == tagJets.second) return false ;

  //PG select the event on the basis of the jets kinematics
  if (m_checkOpposite && tagJets.first->eta () * tagJets.second->eta () > 0) return false ;
  double deltaEta = fabs (tagJets.first->eta () - tagJets.second->eta ()) ;
  if (deltaEta < m_tagJetsDeltaEtaMin || deltaEta > m_tagJetsDeltaEtaMax) return false ;
  double invMass = (tagJets.first->p4 () + tagJets.second->p4 ()).M () ;
  if (invMass < m_tagJetsInvMassMin || invMass > m_tagJetsInvMassMax) return false ;

  return true ;
}
