#ifndef __VBFMetProducer_H__
#define __VBFMetPRoducer_H__


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
  
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"

#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
//#include "RecoMuon/MuonIdentification/interface/IdGlobalFunctions.h"
#include "DataFormats/TrackReco/interface/Track.h"


//-------------------- VBF includes
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFObjects/classes.h"

typedef vbfhww2l::VBFParticle     Particle_;


class VBFMetProducer : public edm::EDProducer {

 public:
      explicit VBFMetProducer(const edm::ParameterSet&);
      ~VBFMetProducer();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      edm::InputTag met_producer;
     
      int n_muons;
};
  
#endif
 


