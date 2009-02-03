#ifndef __VBFMuonProducer_H__
#define __VBFMuonProducer_H__


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
  
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
//#include "RecoMuon/MuonIdentification/interface/IdGlobalFunctions.h"
#include "DataFormats/TrackReco/interface/Track.h"


//-------------------- VBF includes
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFObjects/classes.h"

typedef vbfhww2l::VBFParticle     Particle_;


class VBFMuonProducer : public edm::EDProducer {

 public:
      explicit VBFMuonProducer(const edm::ParameterSet&);
      ~VBFMuonProducer();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;


      edm::InputTag g_muon_producer;
      edm::InputTag t_muon_producer;

      int n_muons;
};
  
#endif
 


