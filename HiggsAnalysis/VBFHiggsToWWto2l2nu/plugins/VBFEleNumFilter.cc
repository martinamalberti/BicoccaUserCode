// $Id: VBFEleNumFilter.cc,v 1.2 2008/03/20 17:41:54 govoni Exp $

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFEleNumFilter.h"

#include <iostream>


//! ctor
VBFEleNumFilter::VBFEleNumFilter (const edm::ParameterSet& iConfig) :
  m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag")) ,
  m_useEleId (iConfig.getParameter<bool> ("useEleId")) ,
  m_eleIdInputTag (iConfig.getParameter<edm::InputTag> ("eleIDInputTag")) ,
  m_minLeptonsNum (iConfig.getParameter<int> ("minElectronsNum")) ,
  m_maxLeptonsNum (iConfig.getParameter<int> ("maxElectronsNum")) ,
  m_eleEtaMax (iConfig.getParameter<double> ("eleEtaMax")) , 
  m_elePtMin (iConfig.getParameter<double> ("elePtMin")) 
{}


// ------------------------------------------------------------------------------------


//! dtor
VBFEleNumFilter::~VBFEleNumFilter ()
{}


// ------------------------------------------------------------------------------------


//! filtering method
bool 
VBFEleNumFilter::filter (edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //PG get the GSF electrons collection
  edm::Handle<electronCollection> GSFHandle;
  iEvent.getByLabel (m_GSFInputTag,GSFHandle); 

  edm::Handle<reco::ElectronIDAssociationCollection> eleIdHandle ;
  //PG get the eleId, if required
  if (m_useEleId) iEvent.getByLabel (m_eleIdInputTag, eleIdHandle) ;

  if (GSFHandle->size () < m_minLeptonsNum) return false ;
  int leptonsNum = 0 ;
  
  //PG loop over the electrons collection
  for (electronCollection::const_iterator ele = GSFHandle->begin () ; 
       ele != GSFHandle->end () ; 
       ++ele) 
    {
      if (fabs (ele->eta ()) > m_eleEtaMax || ele->pt () < m_elePtMin) continue ;
      if (m_useEleId)
        {
          edm::RefToBase<reco::PixelMatchGsfElectron> eRef = GSFHandle->refAt (ele - GSFHandle->begin ()) ;
          reco::PixelMatchGsfElectronRef ref =
            eRef.castTo<reco::PixelMatchGsfElectronRef> () ;
          reco::ElectronIDAssociationCollection::const_iterator electronIDAssocItr = 
            eleIdHandle->find (ref) ;
          if (electronIDAssocItr != eleIdHandle->end () && 
              electronIDAssocItr->val->cutBasedDecision () != 1) continue ;        
//          std::cerr << "WARNING the electron ID option is disabled" << std::endl ;
        }
      ++leptonsNum ; 
    } //PG loop over the electrons collection
         
  if (leptonsNum < m_minLeptonsNum || leptonsNum >= m_maxLeptonsNum) return false ;
  return true ;
}
