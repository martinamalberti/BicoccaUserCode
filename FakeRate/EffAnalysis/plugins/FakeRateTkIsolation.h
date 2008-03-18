#ifndef FakeRateTkIsolation_h
#define FakeRateTkIsolation_h

/** \class FakeRateTkIsolation
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

class FakeRateTkIsolation {

 public:
  
  FakeRateTkIsolation(const edm::ParameterSet& conf);
  
  ~FakeRateTkIsolation();
  
  typedef reco::PixelMatchGsfElectronRef electron ;
  typedef reco::PixelMatchGsfElectronCollection collection;
//  typedef std::vector<const reco::PixelMatchGsfElectron *> container;
  typedef std::vector<reco::PixelMatchGsfElectronRef> container;
  typedef container::const_iterator const_iterator;

  const_iterator begin() const { return selected_.begin(); }
  const_iterator end() const { return selected_.end(); }
 
  void select( edm::Handle<reco::PixelMatchGsfElectronCollection>, 
              const edm::Event& ,
              const edm::EventSetup&);

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
 


