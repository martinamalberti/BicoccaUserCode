#include "FakeRate/EffAnalysis/plugins/FakeRateEleId.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "RecoCaloTools/Selectors/interface/CaloConeSelector.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronIDAssociation.h"

#include <iostream>
#include <algorithm>

using namespace reco;
using namespace std;

FakeRateEleId::FakeRateEleId(const edm::ParameterSet& conf)
{
  electronIDAssocProducer_ = conf.getParameter<std::string>("electronIDAssocProducer");
}  
  
FakeRateEleId::~FakeRateEleId()
{
}

void FakeRateEleId::select(edm::Handle<reco::PixelMatchGsfElectronCollection> c, const edm::Event& e)
//void FakeRateEleId::select(edm::Handle<reco::ElectronIDCollection> c, const edm::Event& e)
{

  edm::Handle<reco::ElectronIDAssociationCollection> electronIDAssocHandle;
  e.getByLabel(electronIDAssocProducer_, electronIDAssocHandle);



//   std::cout << "[FakeRateEleId::select] nbr of initial electrons : " << c->size() << std::endl;
//   for( reco::PixelMatchGsfElectronCollection::const_iterator el = c->begin();  el != c->end(); el++ ) {
//   // for( reco::ElectronIDCollection::const_iterator el = c->begin();  el != c->end(); el++ ) {
//     std::cout << "[FakeRateEleId::select] new initial electron with pt = " << (el)->pt() << " and eta = " <<
//     (el)->eta() << std::endl;
//   } 
  

  selected_.clear();
  unsigned int i=0;
  for( reco::PixelMatchGsfElectronCollection::const_iterator el = c->begin(); 
       //  for( reco::ElectronIDCollection::const_iterator el = c->begin(); 
   el != c->end(); el++ ) {
    // apply some criterion
    //      edm::Ref<reco::PixelMatchGsfElectronCollection> electronRef(c,  el);
    edm::Ref<reco::PixelMatchGsfElectronCollection> electronRef(c,  i);
    reco::ElectronIDAssociationCollection::const_iterator electronIDAssocItr;
    electronIDAssocItr = electronIDAssocHandle->find( electronRef);
    const reco::ElectronIDRef& id = electronIDAssocItr->val;
    bool cutBasedID = id->cutBasedDecision();
//    if ( cutBasedID==1 ) selected_.push_back(& * el );
    if ( cutBasedID==1 ) selected_.push_back( electronRef );
    i++;
  }  
  
 //  std::cout << "[FakeRateEleId::select] nbr of selected electrons : " << selected_.size() << std::endl;
//   for( const_iterator el = begin();  el != end(); el++ ) {
//     std::cout << "[FakeRateEleId::select] new selected electron with pt = " << (*el)->pt() << " and eta = " <<
//     (*el)->eta() << std::endl;
//   } 
  
    
}

