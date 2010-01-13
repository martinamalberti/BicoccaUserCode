// -*- C++ -*-
//
// Package:    TTBarAnalysis
// Class:      TTBarAnalysis
// 
/**\class TTBarAnalysis TTBarAnalysis.cc HiggsAnalysis/TTBarAnalysis/src/TTBarAnalysis.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Andrea Massironi
//         Created:  Wed Jan 13 08:44:58 CET 2010
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
//
// class decleration
//

class TTBarAnalysis : public edm::EDAnalyzer {
   public:
      explicit TTBarAnalysis(const edm::ParameterSet&);
      ~TTBarAnalysis();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
TTBarAnalysis::TTBarAnalysis(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

}


TTBarAnalysis::~TTBarAnalysis()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
TTBarAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;



#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
TTBarAnalysis::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TTBarAnalysis::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(TTBarAnalysis);
