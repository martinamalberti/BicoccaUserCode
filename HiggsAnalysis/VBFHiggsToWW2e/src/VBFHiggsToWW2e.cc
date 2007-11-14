#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFHiggsToWW2e.h"
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

