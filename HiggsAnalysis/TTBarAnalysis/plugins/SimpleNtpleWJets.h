// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TTree.h"
#include <TLorentzVector.h>
#include <TClonesArray.h>
#include "TParticle.h"

#include "PhysicsTools/NtupleUtils/interface/NtupleFactory.h"
#include "PhysicsTools/NtupleUtils/interface/MCDumper.h"

#include "Math/PtEtaPhiE4D.h"
#include "Math/PtEtaPhiM4D.h"
#include "Math/LorentzVector.h"
#include "Math/Vector3D.h"


//---------------------------
//---- class declaration ----
//---------------------------

class SimpleNtpleWJets : public edm::EDAnalyzer {
 public:
  explicit SimpleNtpleWJets(const edm::ParameterSet&);
  ~SimpleNtpleWJets();
  
 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
    
  void fillMCInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  
  TTree* outTree_;
  NtupleFactory* NtupleFactory_;
    
  
  ///---- input tag ----

  edm::InputTag MCtruthTag_;
  
  int eventType_; //---- 0 = signal      1 = background 
  bool verbosity_; //---- true = loquacious     false = silence  
    
};

