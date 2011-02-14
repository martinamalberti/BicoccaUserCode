#ifndef MCDumperPi0_h
#define MCDumperPi0_h

#include "PhysicsTools/NtupleUtils/interface/MCUtils.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include <vector>



class MCDumperPi0
{
 public:
  
  //!ctor
  MCDumperPi0(edm::Handle<reco::GenParticleCollection>& genParticles,
           const int& eventType,
           const bool& verbosity);
  
  //!dtor
  ~MCDumperPi0();
  
  
  void PrintEventInfo();
  void GetHDecayMode();
  void GetVDecayMode();
  
     
  bool isValid() { return isValid_p; };
  
  std::string mcVDecayMode() { return mcVDecayMode_p; };
  
  
  std::vector<const reco::Candidate*> GetMcE()  { return mcE_p;   };
  std::vector<const reco::Candidate*> GetMcMu() { return mcMu_p ; };
  
  
  const reco::Candidate* mcQ1_tag() { return mcQ1_tag_p; };
  const reco::Candidate* mcQ2_tag() { return mcQ2_tag_p; };

  const reco::Candidate* mcV() { return mcV_p; };

  const reco::Candidate* mcF1_fromV() { return mcF1_fromV_p; };
  const reco::Candidate* mcF2_fromV() { return mcF2_fromV_p; };
  
 private:
  
  void Analyze(edm::Handle<reco::GenParticleCollection>& hGenParticles);
  
  
  bool verbosity_p;
  int eventType_p;
  
  bool isValid_p;
  
  std::string mcVDecayMode_p;
  int         mcVDecayId_p;
  
  std::vector<const reco::Candidate*> mcCharged_p;
  std::vector<const reco::Candidate*> mcE_p;
  std::vector<const reco::Candidate*> mcMu_p;
  std::vector<const reco::Candidate*> mcGamma_p;
  std::vector<const reco::Candidate*> mcTau_p;
  std::vector<std::vector<const reco::Candidate*> > mcTauJ_p;
  

  const reco::Candidate* mcV_p;

  const reco::Candidate* mcQ1_tag_p;
  const reco::Candidate* mcQ2_tag_p;

  const reco::Candidate* mcF1_fromV_p;
  const reco::Candidate* mcF2_fromV_p;

};

#endif
