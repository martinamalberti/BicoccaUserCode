#define DEBUG    0 // 0=false
#define C_DEBUG  10 // currently debuging

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFMetProducer.h"


using namespace std;
using namespace edm;
using namespace reco;
using namespace vbfhww2l;



VBFMetProducer::VBFMetProducer(const edm::ParameterSet& iConfig){

  produces<VBFParticleCollection>(""); 
// produces<VBFParticleCollection>("");

  met_producer = iConfig.getParameter<InputTag>("metProducer");
//  met_cut_pt = iConfig.getParameter<double>("met_cut_pt");
}


VBFMetProducer::~VBFMetProducer(){


   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
VBFMetProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){
    
  //met without/with corrections--------------------------------
  Handle<CaloMETCollection> metHa;
  iEvent.getByLabel(met_producer, metHa);
  //CaloMETCollection::const_iterator met = metHa->begin();

 
  auto_ptr<VBFParticleCollection> Met(new VBFParticleCollection);
  Particle_* met = new Particle_();
  met->v4 = metHa->begin()->p4();

  Met->push_back(*met);
  iEvent.put(Met, "");



  //met from Jets-----------------------------------------------------
  

}




// ------------ method called once each job just before starting event loop  ------------
void VBFMetProducer::beginJob(const edm::EventSetup&){
}

// ------------ method called once each job just after ending the event loop  ------------
void VBFMetProducer::endJob(){
}

