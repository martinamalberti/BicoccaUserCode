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
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFUtils.h"

class VBFJetCleaner {

 public:
  
  VBFJetCleaner (const edm::ParameterSet& conf);
  
  ~VBFJetCleaner () ;
  
  typedef reco::CaloJetCollection collection ;
  typedef reco::CaloJetRef jet ;
//  typedef std::vector<const reco::PixelMatchGsfElectron *> container;
  typedef std::vector<reco::CaloJetRef> container ;
  typedef container::const_iterator const_iterator ;

  const_iterator begin () const { return m_selected.begin () ; }
  const_iterator end () const { return m_selected.end () ; }
 
  void select (edm::Handle<collection>, 
               const edm::Event&) ;

 private:
 
  container m_selected ;

  edm::InputTag m_GSFInputTag ;
  double m_maxDeltaR ;
  double m_minEleOJetEratio ;
  double m_maxHEoverEmE ;


};
  
#endif
 


