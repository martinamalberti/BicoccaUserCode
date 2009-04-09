#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFEventsCounter.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"


#include <Math/VectorUtil.h>

      VBFEventsCounter::VBFEventsCounter (const edm::ParameterSet& iConfig)
      {
      }


// --------------------------------------------------------------------


      VBFEventsCounter::~VBFEventsCounter ()
      {
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
      }


// --------------------------------------------------------------------


      void
        VBFEventsCounter::analyze (const edm::Event& iEvent, const edm::EventSetup& iSetup)
      {
       numEvents++;
      }
// --------------------------------------------------------------------


      void 
        VBFEventsCounter::beginJob (const edm::EventSetup&)
      {
       numEvents = 0;

       edm::Service<TFileService> fs ;

       m_ntuple = fs->make <TNtuple> ("ntuple","number of events","numEvents");

      }


// --------------------------------------------------------------------


      void 
        VBFEventsCounter::endJob () 
      {
       m_ntuple -> Fill (numEvents) ;
      }
