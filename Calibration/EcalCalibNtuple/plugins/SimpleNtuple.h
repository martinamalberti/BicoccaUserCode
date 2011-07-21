#ifndef SimpleNtuple_h
#define SimpleNtuple_h

// system include files
#include <memory>
#include <vector>
#include <map>
#include <set>
#include "TFile.h"
#include "TTree.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "Geometry/CaloGeometry/interface/TruncatedPyramid.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"
#include "CondFormats/EcalObjects/interface/EcalIntercalibConstants.h"
#include "CondFormats/DataRecord/interface/EcalIntercalibConstantsRcd.h"
#include "CondFormats/EcalObjects/interface/EcalLaserAlphas.h"
#include "CondFormats/DataRecord/interface/EcalLaserAlphasRcd.h"
#include "CondFormats/EcalObjects/interface/EcalADCToGeVConstant.h"
#include "CondFormats/DataRecord/interface/EcalADCToGeVConstantRcd.h"
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbService.h"
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbRecord.h"

#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/Math/interface/Point3D.h"
#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "RecoVertex/PrimaryVertexProducer/interface/PrimaryVertexSorter.h"
#include "RecoVertex/PrimaryVertexProducer/interface/VertexHigherPtSquared.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterFunctionBaseClass.h" 
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterFunctionFactory.h" 
#include "RecoEcal/EgammaCoreTools/interface/EcalTools.h"
#include "RecoEcal/EgammaCoreTools/interface/PositionCalc.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Flags.h"
#include "DataFormats/PatCandidates/interface/Lepton.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

// PU MC information
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h" 

#include "PhysicsTools/NtupleUtils/interface/NtupleFactory.h"

using namespace cms ;
using namespace edm ;
using namespace std ;
using namespace reco;



class SimpleNtuple : public edm::EDAnalyzer {
 
  typedef ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<float> > XYZVectorF; 

 public:

  explicit SimpleNtuple (const edm::ParameterSet&) ;
  ~SimpleNtuple () ;

      
 private:  
  virtual void analyze (const edm::Event&, const edm::EventSetup&) ;

 protected:

  void fillL1Info (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillHLTInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillBSInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillPVInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillEleInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillMuInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillCALOMetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillTCMetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillPFMetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillJetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillPFJetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillMCPUInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  

  // ----------member data ---------------------------
  EcalClusterFunctionBaseClass* EcalClusterCrackCorrection;
  EcalClusterFunctionBaseClass* EcalClusterLocalContCorrection;
  PositionCalc                  positionCalculator;
  

  HLTConfigProvider hltConfig_;
  
  math::XYZPoint BSPoint_;
  math::XYZPoint PVPoint_;
  
  
  ///---- input tag ----
  edm::InputTag DCSTag_;
  edm::InputTag PVTag_;
  edm::InputTag L1Tag_;
  edm::InputTag TriggerEventTag_;
  edm::InputTag TriggerResultsTag_;
  edm::InputTag recHitCollection_EB_;
  edm::InputTag recHitCollection_EE_;
  edm::InputTag EleTag_;
  edm::InputTag MuTag_;
  edm::InputTag JetTag_;
  edm::InputTag CALOMetTag_;
  edm::InputTag TCMetTag_;
  edm::InputTag PFMetTag_;
  edm::InputTag MCPileupTag_;

  std::vector<std::string> eleId_names_;
  
  ///---- flags ----
  bool useTriggerEvent_ ;
  bool dataFlag_ ;
  bool saveL1_ ;
  bool saveHLT_ ;
  bool saveBS_ ;
  bool savePV_ ;
  bool saveEle_ ;
  bool saveEleShape_ ;
  bool saveMu_ ;
  bool saveJet_ ;
  bool saveCALOMet_ ;
  bool saveTCMet_ ;
  bool savePFMet_ ;
  bool saveMCPU_;

  bool verbosity_; //---- true = loquacious     false = silence  
  
  int eventNaiveId_;


  ///---- output ----
  TTree* outTreeNameHLT;
  TTree* outTreeNameEleId;
  TTree* outTree_;
  NtupleFactory* NtupleFactory_;
  
} ;

#endif
