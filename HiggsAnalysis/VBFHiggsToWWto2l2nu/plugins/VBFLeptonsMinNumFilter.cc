#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFLeptonsMinNumFilter.h"

#include <iostream>


//! ctor
VBFLeptonsMinNumFilter::VBFLeptonsMinNumFilter (const edm::ParameterSet& iConfig) :
  m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag")) ,
  m_muInputTag (iConfig.getParameter<edm::InputTag> ("muInputTag")) ,
  m_elePtMin (iConfig.getParameter<double> ("elePtMin")) ,
  m_muPtMin (iConfig.getParameter<double> ("muPtMin")) ,
  m_minLeptonsNum (iConfig.getParameter<int> ("minLeptonsNum"))
{}


// ------------------------------------------------------------------------------------


//! dtor
  VBFLeptonsMinNumFilter::~VBFLeptonsMinNumFilter ()
  {}


// ------------------------------------------------------------------------------------


//! filtering method
  bool 
    VBFLeptonsMinNumFilter::filter (edm::Event& iEvent, const edm::EventSetup& iSetup)
  {
   //AM get the muons collection
   edm::Handle<reco::MuonCollection> MuonHandle;
   iEvent.getByLabel (m_muInputTag,MuonHandle); 
   int mu_counter = 0;
   for(reco::MuonCollection::const_iterator muonIt = MuonHandle->begin(); muonIt != MuonHandle->end(); muonIt++)
   {
    if (muonIt -> pt()> m_muPtMin) mu_counter++;
   }
   
    
   //AM get the GSF electrons collection
   edm::Handle<reco::PixelMatchGsfElectronCollection> GSFHandle;
   iEvent.getByLabel (m_GSFInputTag,GSFHandle); 
   int ele_counter = 0;
   for(reco::PixelMatchGsfElectronCollection::const_iterator electronIt = GSFHandle->begin(); electronIt != GSFHandle->end(); electronIt++)
   {
    if (electronIt -> pt()> m_elePtMin) ele_counter++;
   }
   
   if ((mu_counter + ele_counter) < m_minLeptonsNum) return false ;
   else return true;

      
//    if ((MuonHandle->size () + GSFHandle->size ()) < m_minLeptonsNum) return false ;
//    else return true;
  }


