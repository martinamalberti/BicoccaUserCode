// $Id: VBFProcessFilter.cc,v 1.3 2007/11/17 16:14:24 tancini Exp $

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFProcessFilter.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include <iostream>


//! ctor
VBFProcessFilter::VBFProcessFilter (const edm::ParameterSet& iConfig) :
  m_jetInputTag (iConfig.getParameter<edm::InputTag> ("jetInputTag")) ,
  m_jetEtaMax (iConfig.getParameter<double> ("jetEtaMax")) ,
  m_jetPtMin (iConfig.getParameter<double> ("jetPtMin"))
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

  //PG first loop over jets
  for (reco::CaloJetCollection::const_iterator firstJet = jetCollectionHandle->begin (); 
       firstJet != jetCollectionHandle->end (); 
       ++firstJet ) 
    {
//      JetFlavour jetFlavour = m_jfi.identifyBasedOnPartons (*(jet)) ;
//      int myflav = jetFlavour.flavour () ;
//      m_recoJetFlavour-> push_back (myflav) ;
      //PG second loop over jets
      for (reco::CaloJetCollection::const_iterator secondjet = firstJet ; 
           secondjet != jetCollectionHandle->end (); 
           ++secondjet ) 
        {
    
    
        } //PG second loop over jets
    } //PG first loop over jets


  return false ;
}
	

