// $Id: VBFMCProcessFilter.cc,v 1.6 2007/11/15 16:22:11 tancini Exp $

#include "HiggsAnalysis/VBFHiggsToWW2e/plugins/VBFMCProcessFilter.h"

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
       if (abs((*part)->pdg_id()) == 25 && 
          (*part)->production_vertex ()->particles_in_size () == 2 ) //PG this is an higgs boson
         {
           HepMC::GenParticle* motherA = (*((*part)->production_vertex()->particles_in_const_begin())); 
           HepMC::GenParticle* motherB = (*((*part)->production_vertex()->particles_in_const_begin())+1);
             
           int pdgID_A =  motherA->pdg_id () ;
           int pdgID_B =  motherB->pdg_id () ;
  
           if ( ( abs (pdgID_A) == 23 &&
                  abs (pdgID_B) == 23 ) ||
                ( abs (pdgID_A) == 24 &&
                  abs (pdgID_B) == 24 ) )
             {
               return true ;
             }  
         }
    } //PG loop over the generated particles

  return false ;

}
	

