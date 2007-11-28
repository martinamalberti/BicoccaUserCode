#ifndef FakeRateHadIsolation_h
#define FakeRateHadIsolation_h

/** \class FakeRateHadIsolation
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
//#include "DataFormats/Common/interface/ESHandle.h" 
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"

class FakeRateHadIsolation {

 public:
  
  FakeRateHadIsolation(const edm::ParameterSet& conf);
  
  ~FakeRateHadIsolation();
  
  typedef reco::PixelMatchGsfElectronCollection collection;
  typedef reco::PixelMatchGsfElectronRef electron ;
//  typedef std::vector<const reco::PixelMatchGsfElectron *> container;
  typedef std::vector<reco::PixelMatchGsfElectronRef> container;
  typedef container::const_iterator const_iterator;

  const_iterator begin() const { return selected_.begin(); }
  const_iterator end() const { return selected_.end(); }
 
  void select( edm::Handle<reco::PixelMatchGsfElectronCollection>, const edm::Event&, const edm::EventSetup& );

 private:
 
  container selected_;
  
  std::string hcalrhitsLabel_;
  double radiusConeExt_;
  double radiusConeInt_;
  double eTMin_;
  double cut_;
  
  edm::ESHandle<CaloGeometry> theCaloGeom_;

};
  
#endif
 


