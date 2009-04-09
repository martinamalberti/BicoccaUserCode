#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFJetCleaner.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <Math/VectorUtil.h>

#include <iostream>
#include <algorithm>


VBFJetCleaner::VBFJetCleaner (const edm::ParameterSet& iConfig) :
  m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag")) ,
  m_maxDeltaR (iConfig.getParameter<double> ("maxDeltaR")) ,
  m_minEleOJetEratio (iConfig.getParameter<double> ("minEleOJetEratio")),
  m_maxHEoverEmE (iConfig.getParameter<double> ("maxHEoverEmE"))
{
}  


// ----------------------------------------------------------------------------

  
VBFJetCleaner::~VBFJetCleaner ()
{
}


// ----------------------------------------------------------------------------

  
void 
  VBFJetCleaner::select (edm::Handle<VBFJetCleaner::collection> jetCollectionHandle, 
                       const edm::Event& iEvent,
                       const edm::EventSetup& iEs)
{
  m_selected.clear () ;

  //PG get the GSF electrons collection
  edm::Handle<electronCollection> GSFHandle; 
  iEvent.getByLabel (m_GSFInputTag,GSFHandle) ; 

  //PG loop over jets
  for (collection::const_iterator jetIt = jetCollectionHandle->begin () ;
       jetIt != jetCollectionHandle->end () ;
       ++jetIt)
    {
      double electronsEnergy = 0. ;
//      double electronsHoE = 0. ;
      int counter = 0 ;
      //PG loop over electroni
      for (electronCollection::const_iterator eleIt = GSFHandle->begin () ;
           eleIt != GSFHandle->end () ;
           ++eleIt)
        {
          //PG this is not an electron
          if (fabs (eleIt->hadronicOverEm ()) > m_maxHEoverEmE) continue ;          
          //PG FIXME NB should we use the SC direction, or the electron one?
          double deltaR = 
            ROOT::Math::VectorUtil::DeltaR (eleIt->momentum (), jetIt->momentum ()) ;
	  
//          if (deltaR < m_maxDeltaR)
//            {
//              ++counter ;
//              electronsEnergy += eleIt->caloEnergy () ;
//              electronsHoE += fabs (eleIt->hadronicOverEm ()) ;           
//            }          
          if (deltaR < m_maxDeltaR)
            {
              ++counter ;
              electronsEnergy += eleIt->caloEnergy () ;
            }          
        } //PG loop over electrons
      if (counter &&
          electronsEnergy / jetIt->energy () > m_minEleOJetEratio )
//           || electronsHoE / static_cast<double> (counter) > m_maxHEoverEmE))
        {
//          std::cerr << "PIETRO NOT selected" << std::endl ;
          continue ;
        } 
//      std::cerr << "PIETRO     selected" << std::endl ;
        m_selected.push_back (my_jet (jetCollectionHandle,jetIt - jetCollectionHandle->begin ())) ;
    } //PG loop over jets   
 }
