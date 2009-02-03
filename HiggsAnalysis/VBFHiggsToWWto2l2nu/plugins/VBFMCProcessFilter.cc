// $Id: VBFMCProcessFilter.cc,v 1.1 2008/03/17 17:01:17 govoni Exp $

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFMCProcessFilter.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include <iostream>


//! ctor
VBFMCProcessFilter::VBFMCProcessFilter(const edm::ParameterSet& iConfig) :
  label_ (iConfig.getUntrackedParameter<std::string> ("moduleLabel",std::string ("source")))
{}


// ------------------------------------------------------------------------------------


//! dtor
VBFMCProcessFilter::~VBFMCProcessFilter()
{}


// ------------------------------------------------------------------------------------


//! filtering method
bool 
VBFMCProcessFilter::filter (edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::Handle<edm::HepMCProduct> evtMC;
  iEvent.getByLabel(label_,evtMC);

  const HepMC::GenEvent * Evt = evtMC->GetEvent();

  int processID = Evt->signal_process_id() ;
  if (processID == 123 || processID == 124) return true ;
  //if (processID == 102) return true ;
  return false ;

}
	

