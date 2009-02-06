// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFEleHETIsolator.h"

#include <string.h>

VBFEleHETIsolator::VBFEleHETIsolator(const edm::ParameterSet& iConfig)
{
  eleTkIso_ = iConfig.getParameter<edm::InputTag>("eleTkIso");
  eleEcalIso_ = iConfig.getParameter<edm::InputTag>("eleEcalIso");
  eleHcalIso_ = iConfig.getParameter<edm::InputTag>("eleHcalIso");
  useCombinedCuts_ = iConfig.getParameter<bool>("useCombinedCuts");
  thresholdCombined_ = iConfig.getParameter<double>("thresholdCombined"); 
  thresholdTk_ = iConfig.getParameter<double>("thresholdTk"); 
  thresholdEcal_ = iConfig.getParameter<double>("thresholdEcal"); 
  thresholdHcal_ = iConfig.getParameter<double>("thresholdHcal"); 
  coefTk_ = iConfig.getParameter<double>("coefTk"); 
  coefEcal_ = iConfig.getParameter<double>("coefEcal"); 
  coefHcal_ = iConfig.getParameter<double>("coefHcal"); 
  electronIdCutsLabel_ = iConfig.getParameter<edm::InputTag>("electronIdCutsLabel");
}


VBFEleHETIsolator::~VBFEleHETIsolator()
{}


void
VBFEleHETIsolator::select (edm::Handle<reco::GsfElectronCollection> electrons,
                                      const edm::Event& iEvent,
                                        const edm::EventSetup& iEventSetup)
{

  using namespace edm;
  using namespace reco;
  selected_.clear();

  std::vector<edm::Handle<edm::ValueMap<double> > > eleIsoValueMap(3);
  
  if( iEvent.getByLabel( eleTkIso_ , eleIsoValueMap[0] )  &&
      iEvent.getByLabel( eleEcalIso_ , eleIsoValueMap[1] ) &&
      iEvent.getByLabel( eleHcalIso_ , eleIsoValueMap[2] ) ) {

    const edm::ValueMap<double> & eleIsoTk = * eleIsoValueMap[0] ;
    const edm::ValueMap<double> & eleIsoEcal = * eleIsoValueMap[1] ;
    const edm::ValueMap<double> & eleIsoHcal = * eleIsoValueMap[2] ;

    std::vector<edm::Handle<edm::ValueMap<float> > > eIDValueMap(1);
    iEvent.getByLabel( electronIdCutsLabel_ , eIDValueMap[0] ) ;
    const edm::ValueMap<float> & eIdmapCuts = * eIDValueMap[0] ;

    // Loop over electrons
    for (unsigned int i = 0; i < electrons->size(); i++) {  
      Ref<reco::GsfElectronCollection> electronRef(electrons,i);
      bool eleiso = false;

      if ( eIdmapCuts[electronRef] < 0.5 ) continue ; 
     
      if( useCombinedCuts_ ) {
        if( ( coefTk_*eleIsoTk[electronRef] + 
              coefEcal_*eleIsoEcal[electronRef] + 
              coefHcal_*eleIsoHcal[electronRef]  ) < thresholdCombined_ ) 
         eleiso = true;
      } 
      else {
         if( eleIsoTk[electronRef] < thresholdTk_ ) 
         if( eleIsoEcal[electronRef] < thresholdEcal_ ) 
         if( eleIsoHcal[electronRef] < thresholdHcal_ ) 
         eleiso = true;
      }
      
      if (eleiso==true)
         selected_.push_back (electronRef);
      
    }
  } else {
    LogWarning("VBFEleHETIsolator") << eleTkIso_ << " or " << eleEcalIso_ << " or " << eleHcalIso_ << " not available";
  }

}
