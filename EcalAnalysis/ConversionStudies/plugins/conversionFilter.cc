#include "EcalAnalysis/ConversionStudies/plugins/conversionFilter.h"






//! ctor
conversionFilter::conversionFilter(const edm::ParameterSet& iConfig)
{}

// ----------------------------------------------------------------






//! dtor
conversionFilter::~conversionFilter()
{}

// ----------------------------------------------------------------






void conversionFilter::beginJob() 
{}

// ----------------------------------------------------------------






void conversionFilter::endJob() 
{}

// ----------------------------------------------------------------






//! check the conversionsCollection size
bool conversionFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
//   int run = iEvent.id().run();
//   if (run != 124009 &&  run != 124020 && run != 124022 && run != 124023 && run != 124024) 
//     return false;

  edm::Handle<reco::ConversionCollection> pConversionTkOnly;
  iEvent.getByLabel("trackerOnlyConversions",pConversionTkOnly);

  if(pConversionTkOnly->size() > 0)
    return true;
    
  else
    return false;  
}
