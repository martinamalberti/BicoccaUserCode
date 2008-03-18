#ifndef FakeRateEleId_h
#define FakeRateEleId_h

/** \class FakeRateEleId
 *
 * Object selector to perform electron id
 *
 */  
 
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DataFormats/Common/interface/Handle.h" 
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"

class FakeRateEleId {

 public:
  
  FakeRateEleId(const edm::ParameterSet& conf);
  
  ~FakeRateEleId();
  
  typedef reco::PixelMatchGsfElectronRef electron ;
  typedef reco::PixelMatchGsfElectronCollection collection;
//  typedef std::vector<const reco::PixelMatchGsfElectron *> container;
  typedef std::vector<reco::PixelMatchGsfElectronRef> container;
  typedef container::const_iterator const_iterator;

  const_iterator begin() const { return selected_.begin(); }
  const_iterator end() const { return selected_.end(); }
 
  void select( edm::Handle<reco::PixelMatchGsfElectronCollection>, 
               const edm::Event&,
               const edm::EventSetup&) ;

 private:
 
  container selected_;
 std::string  electronIDAssocProducer_ ;

  
};
  
#endif
 


