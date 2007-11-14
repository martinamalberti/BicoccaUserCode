// -*- C++ -*-
//
// Package:    VBFHiggsToWW2e
// Class:      VBFHiggsToWW2e
// 
/**\class VBFHiggsToWW2e VBFHiggsToWW2e.cc HiggsAnalysis/VBFHiggsToWW2e/src/VBFHiggsToWW2e.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Pietro Govoni
//         Created:  Wed Nov 14 17:32:25 CET 2007
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

class VBFHiggsToWW2e : public edm::EDAnalyzer {
   public:
      explicit VBFHiggsToWW2e(const edm::ParameterSet&);
      ~VBFHiggsToWW2e();


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
VBFHiggsToWW2e::VBFHiggsToWW2e(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed

}


VBFHiggsToWW2e::~VBFHiggsToWW2e()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
VBFHiggsToWW2e::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
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
VBFHiggsToWW2e::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
VBFHiggsToWW2e::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(VBFHiggsToWW2e);
