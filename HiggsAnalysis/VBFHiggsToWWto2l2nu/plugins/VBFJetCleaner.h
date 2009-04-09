#ifndef VBFJetCleaner_h
#define VBFJetCleaner_h

/** \class VBFJetCleaner
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
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFUtils.h"

#include "DataFormats/Common/interface/RefProd.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefVector.h"


class VBFJetCleaner {

 typedef edm::View<reco::PixelMatchGsfElectron> electronCollection ;
 
 public:
  
  VBFJetCleaner (const edm::ParameterSet& conf);
  
  ~VBFJetCleaner () ;
  
//   typedef reco::CaloJetCollection collection ;
//   typedef reco::CaloJetRef jet ;
//   typedef std::vector<jet> container ;
//   typedef container::const_iterator const_iterator ;

  //---- funziona con CaloJet ----
//   typedef reco::Jet kindOfJet ;
//   typedef std::vector<kindOfJet> collection ;
//   typedef edm::Ref<std::vector<kindOfJet> > my_jet ;
//   typedef std::vector<my_jet> my_container ;
//   typedef my_container::const_iterator my_const_iterator ;

  typedef reco::CaloJet kindOfJet ;
  typedef std::vector<kindOfJet> collection ;
  typedef edm::Ref<std::vector<kindOfJet> > my_jet ;
  typedef std::vector<my_jet> my_container ;
  typedef my_container::const_iterator my_const_iterator ;

  
//   typedef reco::CaloJet kindOfJet ;
//   typedef edm::View<kindOfJet> collection ;
//   typedef edm::Ref<collection> my_jet ;
// //   typedef std::vector<jet> container ;
//   typedef edm::RefVector<collection> my_container ;
//   typedef my_container::const_iterator my_const_iterator ;


  
// // // //   typedef reco::PFJet kindOfJet ;
// // // //   typedef std::vector<kindOfJet> collection ;
// // // //   typedef edm::Ref<collection> my_jet ;
// // // // //   typedef std::vector<jet> container ;
// // // //   typedef edm::RefVector<collection> my_container ;
// // // //   typedef my_container::const_iterator my_const_iterator ;
  
   
  
// // //   typedef reco::PFJetCollection collection ;
// // //   typedef reco::PFJetRef my_jet ;
// // //   typedef reco::PFJetRefVector my_container ;
// // //   typedef my_container::const_iterator my_const_iterator ;

  
// // //   typedef reco::CaloJetCollection collection ;
// // //   typedef reco::CaloJetRef my_jet ;
// // //   typedef reco::CaloJetRefVector my_container ;
// // //   typedef my_container::const_iterator my_const_iterator ;

  
  
  
  
  
  
// //   typedef reco::CaloJet kindOfJet ;
// //   typedef std::vector<kindOfJet> collection ;
// //   typedef edm::Ref<collection> my_jet ;
// // //   typedef std::vector<jet> my_container ;
// //   typedef edm::RefVector<collection> my_container ;
// //   typedef my_container::const_iterator my_const_iterator ;

  
  
  
  
  
  
  /*
  
  
//   typedef edm::View<reco::Jet> CaloJetCollection;
//   typedef std::vector<reco::CaloJet> CaloJetCollection;

  typedef std::vector<reco::CaloJet> collection ;
//   typedef edm::View< reco::Jet > collection ;
    
//   typedef reco::CaloJetRef jet ;
  typedef reco::CaloJetRef jet;
 
//   typedef std::vector<reco::CaloJetRef> container ;
  typedef std::vector<reco::CaloJet> container ;
//   typedef edm::RefVector<collection> container ;
  
  typedef container::const_iterator const_iterator ;

  */
  
  
  
  my_const_iterator begin () const { return m_selected.begin () ; }
  my_const_iterator end () const { return m_selected.end () ; }
 
  void select (edm::Handle<collection>, 
               const edm::Event&,
               const edm::EventSetup&) ;

  
  
 private:
 
  my_container m_selected ;

  edm::InputTag m_GSFInputTag ;
  double m_maxDeltaR ;
  double m_minEleOJetEratio ;
  double m_maxHEoverEmE ;


};
  
#endif
 


