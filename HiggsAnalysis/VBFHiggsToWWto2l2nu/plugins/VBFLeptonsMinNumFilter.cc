#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFLeptonsMinNumFilter.h"

#include <iostream>


//! ctor
VBFLeptonsMinNumFilter::VBFLeptonsMinNumFilter (const edm::ParameterSet& iConfig) :
  m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag")) ,
  m_muInputTag (iConfig.getParameter<edm::InputTag> ("muInputTag")) ,
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
  //PG get the GSF electrons collection
   edm::Handle<electronCollection> GSFHandle;
   iEvent.getByLabel (m_GSFInputTag,GSFHandle); 

  //VT get the Global muons collection
   edm::Handle<muonCollection> MuonHandle;
   iEvent.getByLabel (m_muInputTag,MuonHandle); 

   if ((MuonHandle->size () + GSFHandle->size ()) < m_minLeptonsNum) return false ;
   else return true;
  }


