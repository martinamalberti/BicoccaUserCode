#ifndef __VBFEvInfoProducer_H__
#define __VBFEvInfoProducer_H__

// system include files
#include <memory>
#include <iostream>


// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Framework/interface/TriggerNamesService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

//-------------------- VBF includes
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFObjects/classes.h"



  class VBFEvInfoProducer : public edm::EDProducer{

  public: 
    explicit VBFEvInfoProducer(const edm::ParameterSet&);
    ~VBFEvInfoProducer();

 private:
    virtual void beginJob(const edm::EventSetup&) ;
    virtual void produce(edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;
   

    edm::InputTag trigger ;
    edm::InputTag vertex_producer ;
    edm::InputTag primvertex_producer ;
    edm::InputTag kfactor ;
    std::string use_kFactor;
    edm::InputTag preselection ;



    

    
  }; 

#endif
