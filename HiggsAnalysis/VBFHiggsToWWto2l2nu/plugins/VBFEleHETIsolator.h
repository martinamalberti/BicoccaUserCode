#ifndef VBFEleHETIsolator_h 
#define VBFEleHETIsolator_h

#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"

class VBFEleHETIsolator  {
 public:
  explicit VBFEleHETIsolator(const edm::ParameterSet&);
  ~VBFEleHETIsolator();

  // Collections to be selected
  typedef reco::GsfElectronCollection collection;
  typedef std::vector<reco::GsfElectronRef> ::const_iterator const_iterator;

  //define iterators with above typedef
  const_iterator begin () const { return selected_.begin () ; }
  const_iterator end () const { return  selected_.end () ; }
 
  void select (edm::Handle<reco::GsfElectronCollection>,
               const edm::Event&, 
               const edm::EventSetup&) ;

  // ----------member data ---------------------------
 private:

  edm::InputTag eleTkIso_;
  edm::InputTag eleEcalIso_;
  edm::InputTag eleHcalIso_;
  bool useCombinedCuts_;
  double thresholdCombined_ ; 
  double thresholdTk_ ; 
  double thresholdEcal_ ; 
  double thresholdHcal_ ;
  double coefTk_ ; 
  double coefEcal_ ; 
  double coefHcal_ ;
  edm::InputTag electronIdCutsLabel_;

  std::vector<reco::GsfElectronRef> selected_ ;

};

#endif
