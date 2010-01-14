// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronIDAssociation.h"

#include "TTree.h"
#include <TLorentzVector.h>
#include <TClonesArray.h>
#include "TParticle.h"

#include "PhysicsTools/NtupleUtils/interface/NtupleFactory.h"

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
    
  void fillVtxInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillMuInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillTrackInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillEleInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillSCInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillMCInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
    
  TTree* outTree_;
  NtupleFactory* NtupleFactory_;
    
  
  ///---- input tags ----

  edm::InputTag TracksTag_;
  edm::InputTag EleTag_;
  edm::InputTag MuTag_;
  edm::InputTag PrimaryVertexTag_;        

  edm::InputTag MCtruthTag_;
  
  edm::InputTag m_eleIDCut_LooseInputTag ;
  edm::InputTag m_eleIDCut_RLooseInputTag ;
  edm::InputTag m_eleIDCut_TightInputTag ;
  edm::InputTag m_eleIDCut_RTightInputTag ;

  edm::InputTag barrelClusterCollection_;
  edm::InputTag endcapClusterCollection_;
  
//   string thetriggerEventLabel;
//   string theHLTriggerResults;     // HLT trigger results
//   string the8e29ProcName;
//   string the1e31ProcName;

  int eventType_; //---- 0 = signal      1 = background 
  bool verbosity_; //---- true = loquacious     false = silence  
    
};
