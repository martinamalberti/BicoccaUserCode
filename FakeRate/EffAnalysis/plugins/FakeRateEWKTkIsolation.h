#ifndef FakeRateEWKTkIsolation_h
#define FakeRateEWKTkIsolation_h

/** \class FakeRateEWKTkIsolation
 *
 * Object selector perform electron track isolation selection
 *
 */  
 
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DataFormats/Common/interface/Handle.h" 
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"

class FakeRateEWKTkIsolation {

 public:
  
  FakeRateEWKTkIsolation(const edm::ParameterSet& conf);
  
  ~FakeRateEWKTkIsolation();
  
  typedef reco::PixelMatchGsfElectronRef electron ;
  typedef reco::PixelMatchGsfElectronCollection collection;
//  typedef std::vector<const reco::PixelMatchGsfElectron *> container;
  typedef std::vector<reco::PixelMatchGsfElectronRef> container;
  typedef container::const_iterator const_iterator;

  const_iterator begin() const { return selected_.begin(); }
  const_iterator end() const { return selected_.end(); }
 
  void select( edm::Handle<reco::PixelMatchGsfElectronCollection>, const edm::Event& );

 private:
 
  container selected_;
  
  std::string tracksLabel_;
  double radiusConeExt_;
  double radiusConeInt_;
  double pTMin_;
  double lip_;
  double cut_;
};
  
#endif
 


