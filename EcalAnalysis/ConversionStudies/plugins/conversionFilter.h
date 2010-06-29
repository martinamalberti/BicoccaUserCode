#ifndef conversionFilter_h
#define conversionFilter_h

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"


#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"



class conversionFilter : public edm::EDFilter
{
  
 public:
  
  //! ctor
  explicit conversionFilter (const edm::ParameterSet&);
  
  //! dtor 
  ~conversionFilter();
  
  
  
 private:
  
  void beginJob();
  void endJob();
  
  //! the actual filter method 
  bool filter(edm::Event&, const edm::EventSetup&);
  
  
  
 private:
  
};

#endif
