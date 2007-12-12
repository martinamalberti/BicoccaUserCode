// $Id: VBFLeptonsNumFilter.cc,v 1.1 2007/12/12 16:03:52 govoni Exp $

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFLeptonsNumFilter.h"

#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"

#include <iostream>


//! ctor
VBFLeptonsNumFilter::VBFLeptonsNumFilter (const edm::ParameterSet& iConfig) :
  m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag")) ,
  m_muInputTag (iConfig.getParameter<edm::InputTag> ("muInputTag")) ,
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
  edm::Handle<reco::PixelMatchGsfElectronCollection> GSFHandle;
  iEvent.getByLabel (m_GSFInputTag,GSFHandle); 

  //VT get the Global muons collection
  edm::Handle<reco::MuonCollection> MuonHandle;
  iEvent.getByLabel (m_muInputTag,MuonHandle); 
    
  if ((MuonHandle->size () + GSFHandle->size ()) < m_minLeptonsNum) return false ;
  int leptonsNum = 0 ;
  
  //PG loop over the electrons collection
  for (reco::PixelMatchGsfElectronCollection::const_iterator ele = GSFHandle->begin () ; 
       ele != GSFHandle->end () ; 
       ++ele) 
    {
      if (fabs (ele->eta ()) > m_eleEtaMax || ele->pt () < m_elePtMin) continue ;
      ++leptonsNum ; 
    } //PG loop over the electrons collection
         
  //PG loop over the muons collection
  for (reco::MuonCollection::const_iterator mu = MuonHandle->begin (); 
       mu != MuonHandle->end (); 
       ++mu ) 
    {
      if (fabs (mu->eta ()) > m_muEtaMax || mu->pt () < m_muPtMin) continue ;
      ++leptonsNum ; 
    } //PG loop over the muons collection

  if (leptonsNum < m_minLeptonsNum || leptonsNum >= m_maxLeptonsNum) return false ;
  return true ;
}
