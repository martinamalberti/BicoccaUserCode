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
#include "HiggsAnalysis/VBFHiggsToVV/interface/MCDumperVBF.h"

#include "Math/PtEtaPhiE4D.h"
#include "Math/PtEtaPhiM4D.h"
#include "Math/LorentzVector.h"
#include "Math/Vector3D.h"


//---------------------------
//---- class declaration ----
//---------------------------

class SimpleNtple : public edm::EDAnalyzer {
 public:
  explicit SimpleNtple(const edm::ParameterSet&);
  ~SimpleNtple();
  
 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  void fillMuInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillTrackInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillEleInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillJetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillMetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillGenJetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillGenMetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillMCInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  
  TTree* outTree_;
  NtupleFactory* NtupleFactory_;
    
  
  ///---- input tag ----

  edm::InputTag TracksTag_;
  edm::InputTag EleTag_;
  edm::InputTag MuTag_;
  edm::InputTag MetTag_;
  edm::InputTag JetTag_;
  bool flag_JetBTag_;
  edm::InputTag JetBTag_;
  edm::InputTag correctedJetTag_;
  edm::InputTag MCtruthTag_;
  edm::InputTag genJetTag_;
  edm::InputTag genMetTag_;
  
  edm::InputTag eleIDCut_LooseInputTag_ ;
  edm::InputTag eleIDCut_RLooseInputTag_ ;
  edm::InputTag eleIDCut_TightInputTag_ ;
  edm::InputTag eleIDCut_RTightInputTag_ ;

  bool saveMu_ ;
  bool saveTrack_ ;
  bool saveEle_ ;
  bool saveJet_ ;
  bool saveMet_ ;
  bool saveGenJet_ ;
  bool saveGenMet_ ;
  bool saveMC_ ;
 
  int eventType_; //---- 0 = signal      1 = background 
  bool verbosity_; //---- true = loquacious     false = silence  
    
};

