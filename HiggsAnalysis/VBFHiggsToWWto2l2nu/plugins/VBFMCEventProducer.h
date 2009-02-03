#ifndef __VBFMCEventProducer_H__
#define __VBFMCEVentProducer_H__


// system include files
#include <memory>
#include <iostream>


// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CompositeRefCandidate.h"


//-------------------- VBF includes
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFFillMCEvent.h"


class VBFMCEventProducer : public edm::EDProducer {
   public:
      explicit VBFMCEventProducer(const edm::ParameterSet&);
      ~VBFMCEventProducer();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      // ----------member data ---------------------------
      edm::InputTag m_mcSourceInputTag ;
      edm::InputTag m_genParticlesInputTag ;

      MCEvent_*    mc_ev;
      VBFFillMCEvent fill_mc;
};


#endif
