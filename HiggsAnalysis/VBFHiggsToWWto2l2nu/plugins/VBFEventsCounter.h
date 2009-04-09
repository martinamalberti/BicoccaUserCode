#ifndef VBFEventsCounter_h
#define VBFEventsCounter_h
// -*- C++ -*-
//
// Package:    VBFEventsCounter
// Class:      VBFEventsCounter
// 
/* 

 Description: number of events counter

*/
// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <TNtuple.h>


class VBFEventsCounter : public edm::EDAnalyzer 
{

   public:
      //! ctor
    explicit VBFEventsCounter (const edm::ParameterSet&) ;
      //! dtor
      ~VBFEventsCounter();
      //! the filtering method
      virtual void beginJob (const edm::EventSetup&);
      virtual void analyze (const edm::Event&, const edm::EventSetup&);
      virtual void endJob ();
      
   private:

    TNtuple *m_ntuple;
    int numEvents;
};

#endif


