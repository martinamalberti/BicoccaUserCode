#ifndef SimpleNtuple_h
#define SimpleNtuple_h

// cmssw include files
// PU MC information
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h" 

#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"                                                                                                                            
#include "CommonTools/UtilAlgos/interface/TFileService.h" 

#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "RecoVertex/PrimaryVertexProducer/interface/PrimaryVertexSorter.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

// pat include files
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

// user include files
#include "PhysicsTools/NtupleUtils/interface/NtupleFactory.h"
#include "PhysicsTools/MiBiCommonPAT/interface/MCDumperTTBar.h"
#include "PhysicsTools/MiBiCommonPAT/interface/MCDumperHiggs.h"
#include "PhysicsTools/MiBiCommonPAT/interface/MCDumperZW.h"

// root/c++ include files
#include "TTree.h"
#include <iostream>
#include <vector>






//---------------------------
//---- class declaration ----
//---------------------------

class SimpleNtuple : public edm::EDAnalyzer {
 
 public:
  
  //! ctor
  explicit SimpleNtuple(const edm::ParameterSet&);
  
  //! dtor
  ~SimpleNtuple();
  
  
  
 private:

  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  
  void fillHLTInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  
  void fillBSInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ; 
  void fillPVInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ; 
  
  void fillTauInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillMuInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillEleInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillPhotonInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  
  void fillMetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillJetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  
  void fillHCALNoiseInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  
  void fillMCPtHatInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ; 
  void fillMCHiggsInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillMCTTBarInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillMCZWInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;

  void fillMCPUInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  
  void fillProcessIdInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillPhotonsMotherInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;

  
  HLTConfigProvider hltConfig_;
  TTree* outTree_;
  NtupleFactory* NtupleFactory_;

  math::XYZPoint BSPoint_;
  math::XYZPoint PVPoint_;
    
  
  ///---- input tag ----
  edm::InputTag EBRechitTag_;
  edm::InputTag EERechitTag_;

  edm::InputTag TriggerEventTag_;
  edm::InputTag TriggerResultsTag_;
  
  edm::InputTag PVTag_;
  
  edm::InputTag EleTag_;
  edm::InputTag TracksTag_;
  edm::InputTag EBRecHitCollectionTag_;
  edm::InputTag EERecHitCollectionTag_;
  std::vector<std::string> EleID_names_;
  
  edm::InputTag PhotonTag_;

  edm::InputTag TauTag_;
  
  edm::InputTag MuTag_;
  
  edm::InputTag MetTag_;
  edm::InputTag TCMetTag_;
  edm::InputTag PFMetTag_;
  
  edm::InputTag JetTag_;
  std::vector<std::string> BTag_names_;
  
  edm::InputTag MCtruthTag_;
  
  edm::InputTag MCPileupTag_;

  
  ///---- save flags ----
  bool dataFlag_;
  bool saveHLT_ ;
  bool saveBS_ ;
  bool savePV_ ;
  bool saveTau_ ;
  bool saveMu_ ;
  bool saveEle_ ;
  bool savePhoton_ ;
  bool saveMet_ ;
  bool saveJet_ ;
  bool saveHCALNoise_ ;

  ///---- save MC Info ----
  bool saveMCPtHat_ ;
  bool saveMCTTBar_; 
  bool saveMCHiggs_ ;
  bool saveMCHiggsWW_ ;
  bool saveMCHiggsGammaGamma_ ;
  bool saveMCZW_ ;
  bool saveGenJet_;  
  bool saveMCPU_;
  bool saveProcessId_ ;
  bool savePhotonsMother_ ;

  int eventType_; //---- 0 = signal    1 = background 
  bool verbosity_; //---- true = loquacious    false = silence  
  int eventNaiveId_;
  
  MCDumperHiggs* mcAnalysisHiggs_;
  MCDumperTTBar* mcAnalysisTTBar_;
  MCDumperZW*    mcAnalysisZW_;

};

#endif

    
