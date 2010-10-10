#ifndef _VBFHiggsToVVSimpleNtple
#define _VBFHiggsToVVSimpleNtple

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

#include "HiggsAnalysis/VBFHiggsToVV/interface/MCDumperTTBar.h"
#include "HiggsAnalysis/VBFHiggsToVV/interface/MCDumperVBF.h"

#include "Math/PtEtaPhiE4D.h"
#include "Math/PtEtaPhiM4D.h"
#include "Math/LorentzVector.h"
#include "Math/Vector3D.h"
#include "Math/GenVector/VectorUtil.h"


//---------------------------
//---- class declaration ----
//---------------------------

class SimpleNtple : public edm::EDAnalyzer {

public:
 typedef edm::AssociationMap<edm::OneToValue<std::vector<reco::GsfElectron>, float> > eleMap;
 typedef edm::AssociationMap<edm::OneToValue<std::vector<reco::Muon>, float> > muMap;

 public:
  explicit SimpleNtple(const edm::ParameterSet&);
  ~SimpleNtple();
  
 private:

  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  
  
  void fillHLTInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillPVInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ; 
  void fillMuInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillTrackInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillEleInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillJetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillJetBTaggingInfo(const edm::Event & iEvent, const edm::EventSetup & iESetup,
                           const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >& v1);
  void fillMetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;

  void fillPFEleInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ; 
  void fillPFMuInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillPFJetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;

  void fillGenJetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillGenMetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillMCHiggsInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillMCHiggsDecayInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillMCEleInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillMCMuInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillMCPtHatInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ; 
  void fillMCTTBarInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;

  
  TTree* outTree_;
  NtupleFactory* NtupleFactory_;

  math::XYZPoint PVPoint_;    
  
  ///---- input tag ----
  edm::InputTag DCSTag_;
  edm::InputTag HLTTag_;
  edm::InputTag PVTag_;
  edm::InputTag TracksTag_;
  edm::InputTag EleTag_;
  edm::InputTag Ele3DipSignificanceTag_;
  edm::InputTag EleTipSignificanceTag_;
  edm::InputTag EleLipSignificanceTag_;
  edm::InputTag recHitCollection_EB_;
  edm::InputTag recHitCollection_EE_;

  edm::InputTag MuTag_;
  edm::InputTag Mu3DipSignificanceTag_;
  edm::InputTag MuTipSignificanceTag_;
  edm::InputTag MuLipSignificanceTag_;
  edm::InputTag MetTag_;
  edm::InputTag Type1MetTag_;
  edm::InputTag PFMetTag_;
  edm::InputTag TcMetTag_;
  edm::InputTag JetTag_;
  edm::InputTag JetTag_forID_;
  edm::InputTag MCtruthTag_;
  edm::InputTag genJetTag_;
  edm::InputTag genMetTag_;
  
  edm::InputTag eleIDCut_LooseInputTag_ ;
  edm::InputTag eleIDCut_RLooseInputTag_ ;
  edm::InputTag eleIDCut_TightInputTag_ ;
  edm::InputTag eleIDCut_RTightInputTag_ ;
  
  edm::InputTag JetIDTag_ ;
  edm::InputTag TrackCountingHighEffBJetTagsTag_;
  edm::InputTag TrackCountingHighPurBJetTagsTag_;
  edm::InputTag JetProbabilityBJetTagsTag_;
  edm::InputTag JetBProbabilityBJetTagsTag_;
  edm::InputTag JetChargeTag_;
  
  bool dataTag_;
  bool saveHLT_ ;
  bool savePV_ ;
  bool saveMu_ ;
  bool saveTrack_ ;
  bool saveEle_ ;
  bool saveJet_ ;
  bool savePFJet_ ;
  bool saveJetBTagging_ ;
  bool saveMet_ ;
  bool saveGenJet_ ;
  bool saveGenMet_ ;
  bool saveMCPtHat_ ;
  bool saveMCHiggs_ ;
  bool saveMCHiggsDecay_ ;
  bool saveMCEle_ ;
  bool saveMCMu_ ;
  bool saveTTBar_; 

  int eventType_; //---- 0 = signal      1 = background 
  bool verbosity_; //---- true = loquacious     false = silence  
  int eventNaiveId_;
  
  MCDumperVBF* mcAnalysisVBF_;
  MCDumperTTBar* mcAnalysisTTBar_;

};





#endif

    
