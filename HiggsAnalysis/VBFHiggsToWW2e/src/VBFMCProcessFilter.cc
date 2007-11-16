// $Id: VBFMCProcessFilter.cc,v 1.1 2007/11/15 17:28:15 tancini Exp $

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFMCProcessFilter.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include <iostream>


//! ctor
VBFMCProcessFilter::VBFMCProcessFilter(const edm::ParameterSet& iConfig) :
  label_ (iConfig.getUntrackedParameter ("moduleLabel",std::string ("source")))
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
    
 //PG loop over the generated particles
 for (HepMC::GenEvent::particle_const_iterator part = Evt->particles_begin(); 
       part != Evt->particles_end () ; 
       ++part) 
    {
        int processID = Evt->signal_process_id() ;
        if (processID == 123 || processID == 124)
             {
               return true ;
             }  
         }//PG loop over the generated particles

  return false ;

}
	

