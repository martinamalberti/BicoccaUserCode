#include <iostream>

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFEvInfoProducer.h"

using namespace std ;
using namespace edm ;
using namespace reco ;
using namespace vbfhww2l;


VBFEvInfoProducer::VBFEvInfoProducer(const edm::ParameterSet& iConfig){

  produces<VBFEventInfo>();

  trigger = iConfig.getParameter<InputTag>("TriggerInputTag");
  primvertex_producer = iConfig.getParameter<InputTag>("PrimVertexInputTag");
  vertex_producer = iConfig.getParameter<InputTag>("VertexInputTag");
  kfactor = iConfig.getParameter<InputTag>("kFactorInputTag");
  use_kFactor = iConfig.getParameter<string>("use_kFactor");
  preselection = iConfig.getParameter<InputTag>("Preselection");
  

}


VBFEvInfoProducer::~VBFEvInfoProducer()
{}



// ------------ method called to produce the data  ------------
void
VBFEvInfoProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    
  std::auto_ptr<VBFEventInfo> ev_i(new VBFEventInfo);
  
  ev_i->runId = iEvent.id().run();
  ev_i->eventId = iEvent.id().event();


  if(use_kFactor == "yes")
    {
      Handle<double> kfac;
      try{
	iEvent.getByLabel(kfactor, kfac);
	ev_i->kFac = *kfac;
      }
      catch(cms::Exception& e){
	cout << " module KFactorProducer hasn't been executed!!! " <<endl;
      }
    }
     
  Handle<bool>pres_ha;
    try{
      iEvent.getByLabel(preselection, pres_ha);
      ev_i->presel = *pres_ha;
    }
    catch(cms::Exception& e){
      cout << " module preselectionMarker hasn't been executed!!! " <<endl;
    }


  Handle<TriggerResults> trig_results;
  try{
    iEvent.getByLabel(trigger, trig_results);
    Service<service::TriggerNamesService> tns;
    vector<string> trigger_names;
    bool foundNames = tns->getTrigPaths(*trig_results, trigger_names);

    for(int i =0; i < trig_results->size(); i++){
      ev_i->SetTrigger(trigger_names.at(i), trig_results->at(i).accept());
    }  
  }
  catch(cms::Exception& e){ }

//  Handle<VertexCollection> primvert_ha;
//  try{
//    iEvent.getByLabel(primvertex_producer, primvert_ha);
//    for (VertexCollection::const_iterator primvert = primvert_ha->begin(); 
//	 primvert != primvert_ha->end(); 
//	 ++primvert){
//      ev_i-> primV = primvert ->position();
//    }
//  }
//  catch(cms::Exception& e){ }
  
//  Handle<VertexCollection> vert_ha;
//  try{
//    iEvent.getByLabel(vertex_producer, vert_ha);
//    ev_i-> nZvtx = vert_ha ->size();
//   }
//  catch(cms::Exception& e){ }

 
 
  iEvent.put(ev_i);

}

// ------------ method called once each job just before starting event loop  ------------
void 
VBFEvInfoProducer::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
VBFEvInfoProducer::endJob() {
}


