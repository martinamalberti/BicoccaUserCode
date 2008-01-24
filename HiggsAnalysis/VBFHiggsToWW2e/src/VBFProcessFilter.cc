// $Id: VBFProcessFilter.cc,v 1.8 2007/12/07 17:46:35 govoni Exp $

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFProcessFilter.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h"
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
  edm::Handle<reco::RecoChargedCandidateCollection> jetCollectionHandle ;
  iEvent.getByLabel (m_jetInputTag, jetCollectionHandle) ;

  //PG tag jets not found
  if (jetCollectionHandle->size () != 2) return false ;

  //PG select the event on the basis of the jets kinematics
  if (m_checkOpposite && 
      (*jetCollectionHandle)[0].eta () * (*jetCollectionHandle)[1].eta () > 0) 
    return false ;
  double deltaEta = fabs ((*jetCollectionHandle)[0].eta () - 
                          (*jetCollectionHandle)[1].eta ()) ;
  if (deltaEta < m_tagJetsDeltaEtaMin || 
      deltaEta > m_tagJetsDeltaEtaMax) return false ;
  double invMass = ((*jetCollectionHandle)[0].p4 () + 
                    (*jetCollectionHandle)[1].p4 ()).M () ;
  if (invMass < m_tagJetsInvMassMin || 
      invMass > m_tagJetsInvMassMax) return false ;

  return true ;
}
