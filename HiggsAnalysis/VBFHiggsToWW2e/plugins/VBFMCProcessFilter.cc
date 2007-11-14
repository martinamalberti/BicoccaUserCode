// $Id: VBFMCProcessFilter.cc,v 1.3 2007/11/14 17:43:27 govoni Exp $

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFMCProcessFilter.h"#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"#include <iostream>//! ctor
VBFMCProcessFilter::VBFMCProcessFilter(const edm::ParameterSet& iConfig) :  label_ (iConfig.getUntrackedParameter ("moduleLabel",std::string ("source"))){}

// ------------------------------------------------------------------------------------

//! dtor
VBFMCProcessFilter::~VBFMCProcessFilter(){}

// ------------------------------------------------------------------------------------

//! filtering method
bool 
VBFMCProcessFilter::filter (edm::Event& iEvent, const edm::EventSetup& iSetup){
  Handle<HepMCProduct> evtMC;  event.getByLabel(label_,evtMC);

 //PG loop over the generated particles
 for (HepMC::GenEvent::particle_const_iterator part = Evt->particles_begin(); 
       part != Evt->particles_end () ; 
       ++part) 
    {
       if (abs((*part)->pdg_id()) == 25 && 
          (*part)->production_vertex ()->particles_in_size () == 2 ) //PG this is an higgs boson
         {
           int pdgID_1 = (*part)->production_vertex ()->particles_in_const_begin ()->pdg_id () ;
           int pdgID_2 = ((*part)->production_vertex ()->particles_in_const_begin ()+1)->pdg_id () ;
           if ( ( abs (pdgID_1) == 23 &&
                  abs (pdgID_2) == 23 ) ||
                ( abs (pdgID_1) == 24 &&
                  abs (pdgID_2) == 24 ) )
             {
               return true ;
             }  
         }
    } //PG loop over the generated particles  return false ;}	

