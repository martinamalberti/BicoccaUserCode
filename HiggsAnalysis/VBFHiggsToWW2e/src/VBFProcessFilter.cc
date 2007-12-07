// $Id: VBFProcessFilter.cc,v 1.1 2007/12/07 14:10:19 govoni Exp $

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFProcessFilter.h"

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

  std::pair<jetIt,jetIt> tagJets = findTagJets (jetCollectionHandle->begin (),
                                                jetCollectionHandle->end ()) ;


  return false ;
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
//      JetFlavour jetFlavour = m_jfi.identifyBasedOnPartons (*(jet)) ;
//      int myflav = jetFlavour.flavour () ;
//      m_recoJetFlavour-> push_back (myflav) ;

      math::XYZTLorentzVector firstLV = firstJet->p4 () ;
      //PG second loop over jets
      for (jetIt secondJet = firstJet + 1 ; 
           secondJet != end ; 
           ++secondJet ) 
        {
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
