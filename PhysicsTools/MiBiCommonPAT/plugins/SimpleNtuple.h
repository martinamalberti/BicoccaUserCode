#ifndef SimpleNtuple_h
#define SimpleNtuple_h

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

#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "DataFormats/Common/interface/AssociationMap.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/JetID.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETFwd.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METFwd.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "RecoVertex/PrimaryVertexProducer/interface/PrimaryVertexSorter.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "RecoEgamma/EgammaTools/interface/ConversionFinder.h"
#include "RecoEgamma/EgammaTools/interface/ConversionInfo.h"

#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"

#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"

#include "PhysicsTools/NtupleUtils/interface/NtupleFactory.h"

#include "PhysicsTools/MiBiCommonPAT/interface/MCDumperTTBar.h"
#include "PhysicsTools/MiBiCommonPAT/interface/MCDumperHiggs.h"

///==== PAT ====
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
///==== end PAT ====

#include "Math/PtEtaPhiE4D.h"
#include "Math/PtEtaPhiM4D.h"
#include "Math/LorentzVector.h"
#include "Math/Vector3D.h"
#include "Math/GenVector/VectorUtil.h"


//---------------------------
//---- class declaration ----
//---------------------------

class SimpleNtuple : public edm::EDAnalyzer {

public:
 typedef edm::AssociationMap<edm::OneToValue<std::vector<reco::GsfElectron>, float> > eleMap;
 typedef edm::AssociationMap<edm::OneToValue<std::vector<reco::Muon>, float> > muMap;

 public:
  explicit SimpleNtuple(const edm::ParameterSet&);
  ~SimpleNtuple();
  
 private:

  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  
  void fillHLTInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillPVInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ; 
  
  void fillMuInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillEleInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  
  void fillTrackInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillJetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillMetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;

  void fillMCHiggsInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillMCTTBarInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
 
  void fillMCPtHatInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ; 


  
  TTree* outTree_;
  NtupleFactory* NtupleFactory_;

  math::XYZPoint PVPoint_;    
  
  ///---- input tag ----
  edm::InputTag DCSTag_;
  edm::InputTag HLTTag_;
  edm::InputTag PVTag_;
  edm::InputTag TracksTag_;
  edm::InputTag EleTag_;
  edm::InputTag recHitCollection_EB_;
  edm::InputTag recHitCollection_EE_;

  edm::InputTag MuTag_;
  edm::InputTag MetTag_;
  edm::InputTag Type1MetTag_;
  edm::InputTag PFMetTag_;
  edm::InputTag TcMetTag_;
  edm::InputTag JetTag_;
  edm::InputTag JetTag_forID_;
  
  edm::InputTag MCtruthTag_;

  std::vector<std::string> eleID_names_;
  std::vector<std::string> BTag_names_;
  
  bool dataTag_;
  bool saveHLT_ ;
  bool savePV_ ;
  bool saveMu_ ;
  bool saveEle_ ;
  bool saveJet_ ;
  bool saveMet_ ;
  bool saveMCPtHat_ ;
  bool saveMCTTBar_; 
  bool saveMCHiggs_ ;
  bool saveMCHiggsWW_ ;
  bool saveMCHiggsGammaGamma_ ;
  
  int eventType_; //---- 0 = signal      1 = background 
  bool verbosity_; //---- true = loquacious     false = silence  
  int eventNaiveId_;
  
  MCDumperHiggs* mcAnalysisHiggs_;
  MCDumperTTBar* mcAnalysisTTBar_;

};





#endif

    
