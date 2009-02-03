// $Id: VBFLeptonsNumFilter.cc,v 1.1 2008/03/17 17:01:17 govoni Exp $

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFLeptonsNumFilter.h"

#include <iostream>


//! ctor
VBFLeptonsNumFilter::VBFLeptonsNumFilter (const edm::ParameterSet& iConfig) :
  m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag")) ,
  m_muInputTag (iConfig.getParameter<edm::InputTag> ("muInputTag")) ,
  m_useEleId (iConfig.getParameter<bool> ("useEleId")) ,
  m_eleIdInputTag (iConfig.getParameter<edm::InputTag> ("eleIDInputTag")) ,
  m_minLeptonsNum (iConfig.getParameter<int> ("minLeptonsNum")) ,
  m_maxLeptonsNum (iConfig.getParameter<int> ("maxLeptonsNum")) ,
  m_eleEtaMax (iConfig.getParameter<double> ("eleEtaMax")) , 
  m_elePtMin (iConfig.getParameter<double> ("elePtMin")) ,
  m_muEtaMax (iConfig.getParameter<double> ("muEtaMax")) ,
  m_muPtMin (iConfig.getParameter<double> ("muPtMin"))  
{}


// ------------------------------------------------------------------------------------


//! dtor
VBFLeptonsNumFilter::~VBFLeptonsNumFilter ()
{}


// ------------------------------------------------------------------------------------


//! filtering method
bool 
VBFLeptonsNumFilter::filter (edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //PG get the GSF electrons collection
  edm::Handle<electronCollection> GSFHandle;
  iEvent.getByLabel (m_GSFInputTag,GSFHandle); 

  //VT get the Global muons collection
  edm::Handle<muonCollection> MuonHandle;
  iEvent.getByLabel (m_muInputTag,MuonHandle); 

  edm::Handle<reco::ElectronIDAssociationCollection> eleIdHandle ;
  //PG get the eleId, if required
  if (m_useEleId) iEvent.getByLabel (m_eleIdInputTag, eleIdHandle) ;

  if ((MuonHandle->size () + GSFHandle->size ()) < m_minLeptonsNum) return false ;
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
         
  //PG loop over the muons collection
  for (muonCollection::const_iterator mu = MuonHandle->begin (); 
       mu != MuonHandle->end (); 
       ++mu ) 
    {
      if (fabs (mu->eta ()) > m_muEtaMax || mu->pt () < m_muPtMin) continue ;
      ++leptonsNum ; 
    } //PG loop over the muons collection

  if (leptonsNum < m_minLeptonsNum || leptonsNum >= m_maxLeptonsNum) return false ;
  return true ;
}
