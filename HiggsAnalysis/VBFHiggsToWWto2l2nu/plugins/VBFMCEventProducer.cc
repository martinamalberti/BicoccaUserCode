#define DEBUG    0 // 0=false
#define C_DEBUG  10 // currently debuging


#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFMCEventProducer.h"


using namespace std ;
using namespace edm ;
using namespace reco ;
using namespace vbfhww2l ;




VBFMCEventProducer::VBFMCEventProducer(const edm::ParameterSet& iConfig) :
  m_mcSourceInputTag (iConfig.getParameter<InputTag> ("mcSourceInputTag")),
  m_genParticlesInputTag (iConfig.getParameter<InputTag> ("genParticlesInputTag"))
{
  produces<VBFMCEvent>();
}

//------------------------------------------------------------

VBFMCEventProducer::~VBFMCEventProducer()
{
}

//------------------------------------------------------------


// ------------ method called to produce the data  ------------
void
VBFMCEventProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  Handle<HepMCProduct> evtMC;
  iEvent.getByLabel(m_mcSourceInputTag, evtMC);

  Handle<CandidateCollection> genParticles;
  iEvent.getByLabel(m_genParticlesInputTag, genParticles);
 
  const HepMC::GenEvent * mcEv = evtMC->GetEvent();

  
  std::auto_ptr<VBFMCEvent> mc_ev(new VBFMCEvent);
  
  if(mcEv->signal_process_id() == 123
     || mcEv->signal_process_id() == 124){

#if DEBUG
    fill_mc.Print(mcEv, genParticles, 25);
#endif

    fill_mc.FillMC(mcEv, mc_ev.get(), genParticles);
  }


  iEvent.put(mc_ev);
}

//------------------------------------------------------------

// ------------ method called once each job just before starting event loop  ------------
void 
VBFMCEventProducer::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
VBFMCEventProducer::endJob() {
}

