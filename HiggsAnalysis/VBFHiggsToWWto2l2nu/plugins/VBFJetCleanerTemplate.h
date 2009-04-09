#ifndef VBFJetCleanerTemplate_h
#define VBFJetCleanerTemplate_h

/** \class VBFJetCleanerTemplate
 *
 * Object selector perform electron track isolation selection
 *
 */  
 
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/EventPrincipal.h" 
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DataFormats/Common/interface/Handle.h" 
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFUtils.h"



template <class TCollection> class VBFJetCleanerTemplate {

 typedef edm::View<reco::PixelMatchGsfElectron> electronCollection ;

 public:
  
  VBFJetCleanerTemplate (const edm::ParameterSet& conf);
  
  ~VBFJetCleanerTemplate () ;
  
  typedef TCollection collection ;
  
  typedef edm::Ref<collection> jet;
 
  typedef edm::RefVector<collection> container ;
  
  typedef typename edm::RefVector<collection>::const_iterator const_iterator ;

  const_iterator begin () const { return m_selected.begin () ; }
  const_iterator end () const { return m_selected.end () ; }
 
  void select (edm::Handle<collection>, 
               const edm::Event&,
               const edm::EventSetup&) ;

 private:
 
  container m_selected ;

  edm::InputTag m_GSFInputTag ;
  double m_maxDeltaR ;
  double m_minEleOJetEratio ;
  double m_maxHEoverEmE ;


};
  


//---- I have to include the .cc since template class ----
// #include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFJetCleanerTemplate.cc.old"



// #include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFJetCleanerTemplate.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <Math/VectorUtil.h>

#include <iostream>
#include <algorithm>


template <class TCollection>
 VBFJetCleanerTemplate<TCollection>::VBFJetCleanerTemplate(const edm::ParameterSet& iConfig) :
  m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag")) ,
  m_maxDeltaR (iConfig.getParameter<double> ("maxDeltaR")) ,
  m_minEleOJetEratio (iConfig.getParameter<double> ("minEleOJetEratio")),
  m_maxHEoverEmE (iConfig.getParameter<double> ("maxHEoverEmE"))
{
}  


// ----------------------------------------------------------------------------

  
template <class TCollection> VBFJetCleanerTemplate<TCollection>::~VBFJetCleanerTemplate ()
{
}


// ----------------------------------------------------------------------------

  
template <class TCollection> void 
  VBFJetCleanerTemplate<TCollection>::select (edm::Handle<VBFJetCleanerTemplate<TCollection>::collection> jetCollectionHandle, 
                                              const edm::Event& iEvent,
                                              const edm::EventSetup& iEs)
{
 m_selected.clear () ;

  //PG get the GSF electrons collection
 edm::Handle<electronCollection> GSFHandle; 
 iEvent.getByLabel (m_GSFInputTag,GSFHandle) ; 

//  const_iterator jetIt; 
  //PG loop over jets
 for (typename collection::const_iterator jetIt = jetCollectionHandle->begin () ;
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
  m_selected.push_back (jet (jetCollectionHandle,jetIt - jetCollectionHandle->begin ())) ;
 } //PG loop over jets   
}










#endif
 


