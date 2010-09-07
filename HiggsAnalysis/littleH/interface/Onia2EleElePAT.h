#ifndef HiggsAnalysis_littleH_Onia2EleElePAT_h
#define HiggsAnalysis_littleH_Onia2EleElePAT_h

// system include files
#include <memory>

// FW include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/Utils/interface/PtComparator.h"

// DataFormat includes
#include <DataFormats/PatCandidates/interface/CompositeCandidate.h>
#include <DataFormats/PatCandidates/interface/Electron.h>

#include <CommonTools/UtilAlgos/interface/StringCutObjectSelector.h>
//
// class decleration
//

class Onia2EleElePAT : public edm::EDProducer {
 public:
  explicit Onia2EleElePAT(const edm::ParameterSet&);
  ~Onia2EleElePAT();

 private:
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  bool isAbHadron(int pdgID);
  bool isAMixedbHadron(int pdgID, int momPdgID);
  std::pair<int, float> findJpsiMCInfo(reco::GenParticleRef genJpsi);

  // ----------member data ---------------------------
 private:
  edm::InputTag electrons_;
  edm::InputTag thebeamspot_;
  edm::InputTag thePVs_;
  StringCutObjectSelector<pat::Electron> higherPuritySelection_;
  StringCutObjectSelector<pat::Electron> lowerPuritySelection_; 
  StringCutObjectSelector<reco::Candidate, true> dieleSelection_;
  bool addCommonVertex_, addEleclessPrimaryVertex_;
  bool resolveAmbiguity_;
  bool addMCTruth_;
  GreaterByPt<pat::CompositeCandidate> pTComparator_;

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

#endif
