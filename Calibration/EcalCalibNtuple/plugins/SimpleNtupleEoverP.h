#ifndef SimpleNtupleEoverP_h
#define SimpleNtupleEoverP_h

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
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
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

#include "PhysicsTools/MiBiCommonPAT/interface/MCDumperZW.h"

// Cluster correction functions
#include "Calibration/EcalCalibNtuple/interface/EnergyCorrectionFunctions.h"

using namespace cms ;
using namespace edm ;
using namespace std ;
using namespace reco;

class SimpleNtupleEoverP : public edm::EDAnalyzer {
 
 public:

  explicit SimpleNtupleEoverP (const edm::ParameterSet&) ;
  ~SimpleNtupleEoverP () ;

      
 private:  
  virtual void analyze (const edm::Event&, const edm::EventSetup&) ;

 protected:
   
  float params[42];
   
  bool myWselection (const edm::Event & iEvent, const edm::EventSetup & iSetup, const bool doTighterSel);
  bool myZselection (const edm::Event & iEvent, const edm::EventSetup & iSetup, const bool doTighterSel);
  void fillEleInfo (const edm::Event & iEvent, const edm::EventSetup & iSetup, const int iEle, const std::string eleName) ;
  
  // ----------member data ---------------------------
  
  
  ///---- input tag ----
  edm::InputTag rhoTag_;
  edm::InputTag DCSTag_;
  edm::InputTag PVTag_;
  edm::InputTag L1Tag_;
  edm::InputTag TriggerEventTag_;
  edm::InputTag TriggerResultsTag_;
  edm::InputTag recHitCollection_EB_;
  edm::InputTag recHitCollection_EE_;
  edm::InputTag EleTag_;
  edm::InputTag PhotonTag_;
  edm::InputTag MuTag_;
  edm::InputTag JetTag_;
  edm::InputTag PFMetTag_;
  edm::InputTag TCMetTag_;
  edm::InputTag MCPileupTag_;
  edm::InputTag MCtruthTag_;
  int eventType_;
  std::vector<std::string> eleId_names_;
  
  ///---- flags ----
  bool useTriggerEvent_ ;
  bool dataFlag_ ;
  bool saveL1_ ;
  bool saveHLT_ ;
  bool saveBS_ ;
  bool savePV_ ;
  bool saveEle_ ;
  bool savePho_ ;
  bool saveEleShape_ ;
  bool saveMu_ ;
  bool saveJet_ ;
  bool saveCALOMet_ ;
  bool saveTCMet_ ;
  bool savePFMet_ ;
  bool saveMCPU_;
  bool saveMCZW_ ;

  bool verbosity_; //---- true = loquacious     false = silence  
  bool doTighterSel_; //---- true = tighten the selection wrt to meridani's
  bool applyCorrections_;  //---- true = correct the recHit and SC energy IN the analyzer
  
  int eventNaiveId_;


  ///---- output ----
  TTree* outTree_;
  // event variables
  int eventId;
  int lumiId;
  int runId;
  int timeStampHigh;
  int PV_n;
  int isW;
  int isZ;
  
  // electron variables  
  float ele1_fEta;
  float ele1_fEtaCorr;
  float ele1_tkP;
  float ele1_e5x5;
  float ele1_e3x3;
  float ele1_scNxtal;
  float ele1_scE;
  float ele1_scEta;
  float ele1_scPhi;
  float ele1_scLaserCorr;
  float ele1_5x5LaserCorr;
  float ele1_3x3LaserCorr;
  float ele1_es;
  float ele1_seedE;
  float ele1_seedLaserAlpha;
  float ele1_seedLaserCorr;
  float ele1_seedICConstant;
  int ele1_seedIeta;
  int ele1_seedIphi;
  int ele1_seedIx;
  int ele1_seedIy;
  int ele1_seedZside;
  float ele1_EOverP;
  std::vector<float> ele1_recHit_E;
  std::vector<int> ele1_recHit_hashedIndex;
  std::vector<int> ele1_recHit_ietaORix;
  std::vector<int> ele1_recHit_iphiORiy;
  std::vector<int> ele1_recHit_zside;
  std::vector<float> ele1_recHit_laserCorrection;
  std::vector<float> ele1_recHit_ICConstant;

  float ele2_fEta;
  float ele2_fEtaCorr;
  float ele2_tkP;
  float ele2_e5x5;
  float ele2_e3x3;
  float ele2_scNxtal;
  float ele2_scE;
  float ele2_scEta;
  float ele2_scPhi;
  float ele2_scLaserCorr;
  float ele2_5x5LaserCorr;
  float ele2_3x3LaserCorr;
  float ele2_es;
  float ele2_seedE;
  float ele2_seedLaserAlpha;
  float ele2_seedLaserCorr;
  float ele2_seedICConstant;
  int ele2_seedIeta;
  int ele2_seedIphi;
  int ele2_seedIx;
  int ele2_seedIy;
  int ele2_seedZside;
  float ele2_EOverP;
  std::vector<float> ele2_recHit_E;
  std::vector<int> ele2_recHit_hashedIndex;
  std::vector<int> ele2_recHit_ietaORix;
  std::vector<int> ele2_recHit_iphiORiy;
  std::vector<int> ele2_recHit_zside;
  std::vector<float> ele2_recHit_laserCorrection;
  std::vector<float> ele2_recHit_ICConstant;

} ;

#endif
