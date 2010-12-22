#ifndef MCDumper_h
#define MCDumper_h

#include "PhysicsTools/NtupleUtils/interface/MCUtils.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include <vector>



class MCDumper
{
 public:
  
  //!ctor
  MCDumper(edm::Handle<reco::GenParticleCollection>& genParticles,
           const bool& verbosity);
  
  //!dtor
  ~MCDumper();
  
  
  void PrintEventInfo();
     
  bool isValid() { return isValid_p; };
  
  std::vector<const reco::Candidate*> GetMcE()  { return mcE_p;   };
  std::vector<const reco::Candidate*> GetMcMu() { return mcMu_p ; };
  std::vector<const reco::Candidate*> GetMcTau() { return mcTau_p ; };
  std::vector<std::vector<const reco::Candidate*> > GetMcTauJ() { return mcTauJ_p ; };
  
  
  
 private:
  
  void Analyze(edm::Handle<reco::GenParticleCollection>& hGenParticles);
  
  bool verbosity_p;
  bool isValid_p;
  
  std::vector<const reco::Candidate*> mcCharged_p;
  std::vector<const reco::Candidate*> mcE_p;
  std::vector<const reco::Candidate*> mcMu_p;
  std::vector<const reco::Candidate*> mcTau_p;
  std::vector<std::vector<const reco::Candidate*> > mcTauJ_p;
};

#endif
