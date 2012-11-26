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
#include "DataFormats/EcalDigi/interface/EBSrFlag.h"
#include "DataFormats/EcalDigi/interface/EESrFlag.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalDetId/interface/EcalDetIdCollections.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalDetId/interface/EcalTrigTowerDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterFunctionBaseClass.h" 
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterFunctionFactory.h" 
#include "RecoEcal/EgammaCoreTools/interface/EcalTools.h"
#include "RecoEcal/EgammaCoreTools/interface/PositionCalc.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackExtra.h"
#include "DataFormats/GsfTrackReco/interface/GsfTangent.h"
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
#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"
#include "EGamma/EGammaAnalysisTools/interface/ElectronEffectiveArea.h"
// PU MC information
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h" 

#include "PhysicsTools/NtupleUtils/interface/NtupleFactory.h"

#include "PhysicsTools/MiBiCommonPAT/interface/MCDumperZW.h"
#include "PhysicsTools/NtupleUtils/interface/treeReader.h"
// Cluster PU cleaning
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterPUCleaningTools.h"
// Cluster correction functions
#include "Calibration/EcalCalibNtuple/interface/EnergyCorrectionFunctions.h"
// Cluster correction functions - regression
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "RecoEgamma/EgammaTools/interface/EGEnergyCorrector.h"

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
   
  bool myWselection (const edm::Event & iEvent, const edm::EventSetup & iSetup);
  bool myZselection (const edm::Event & iEvent, const edm::EventSetup & iSetup);
  void fillEleInfo (const edm::Event & iEvent, const edm::EventSetup & iSetup, const int iEle, const std::string eleName) ;
  void fillPVInfo (const edm::Event & iEvent, const edm::EventSetup & iSetup) ;
  void fillRhoInfo(const edm::Event & iEvent, const edm::EventSetup & iSetup) ;
  void fillMetInfo(const edm::Event & iEvent, const edm::EventSetup & iSetup) ;
  void fillDoubleEleInfo(const edm::Event & iEvent, const edm::EventSetup & iSetup) ;
  void fillMCPUInfo (const edm::Event & iEvent, const edm::EventSetup & iSetup) ;
  void fillMCInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup);
  void fillMatrixRecHit(reco::SuperClusterRef scRef, int type, const EcalRecHitCollection* theRecHitCollection, const CaloTopology* topology);

  bool  TightEle   (const edm::Event & iEvent, const edm::EventSetup & iSetup, const int &iEle) ;
  bool  MediumEle  (const edm::Event & iEvent, const edm::EventSetup & iSetup, const int &iEle) ;
  bool  LooseEle   (const edm::Event & iEvent, const edm::EventSetup & iSetup,const int &iEle) ;
  double deltaPhi(const double& phi1, const double& phi2);
  // ----------member data ---------------------------
  
  EGEnergyCorrector             ecorr_;  
  EGEnergyCorrector             ecorrPho_;  
  EcalClusterFunctionBaseClass* EcalClusterCrackCorrection;
  EcalClusterFunctionBaseClass* EcalClusterLocalContCorrection;
 
  
  ///---- input tag ----
  edm::InputTag PVTag_;
  edm::InputTag rhoTag_;
  edm::InputTag recHitCollection_EB_;
  edm::InputTag recHitCollection_EE_;
  edm::InputTag SRFlagCollection_EB_;
  edm::InputTag SRFlagCollection_EE_;
  edm::InputTag conversionsInputTag_;
  edm::InputTag EleTag_;
  edm::InputTag PFMetTag_;
  edm::InputTag MCtruthTag_;
  edm::InputTag MCPileupTag_;

  int eventType_;
  std::vector<std::string> eleId_names_;

  std::string jsonFileName_;
  std::map<int, std::vector<std::pair<int, int> > > jsonMap_;  
  std::string dataRun_;

  bool jsonFlag_;
  bool saveMCPU_;
  bool verbosity_; //---- true = loquacious     false = silence  
  bool applyCorrections_;  //---- true = correct the recHit and SC energy IN the analyzer
  bool doWZSelection_;
  bool dataFlag_;
  bool saveRecHitMatrix_;
  bool saveFbrem_;
  bool saveMCInfo_;

  int eventNaiveId_;
  std::map<float,int> eleIts_;
 
  math::XYZPoint PVPoint_;

  ///---- output ----
  TTree* outTree_;
  MCDumperZW*    mcAnalysisZW_;

  // event variables
  long int bxId;
  long int eventId;
  int lumiId;
  int runId;
  int timeStampHigh;
  int isW;
  int isZ;

  // PU variables

  float rho;
  
  // PV variables
  int PV_n;
  float PV_z;
  float PV_d0;
  float PV_SumPt;
  
  
  
  // ele1 variables  
  ROOT::Math::XYZTVector ele1;
  float ele1_charge;
  float ele1_p;
  float ele1_pt;
  float ele1_eta;
  float ele1_phi;
  int ele1_isTrackerDriven;

  float ele1_sigmaIetaIeta;
  float ele1_DphiIn;
  float ele1_DetaIn;
  float ele1_HOverE;
  float ele1_tkIso;
  float ele1_emIso;
  float ele1_hadIso;

  float ele1_scERaw;
  float ele1_scEtRaw;
  float ele1_scEt;
  float ele1_scLocalEta;
  float ele1_scLocalPhi;
  float ele1_scEtaWidth;
  float ele1_scPhiWidth;
  float ele1_scCrackCorr;
  float ele1_scLocalContCorr;
  float ele1_scE;
  float ele1_scEta;
  float ele1_scPhi;
  float ele1_scLaserCorr;
  float ele1_scE_regression;
  float ele1_scEerr_regression;
  float ele1_scE_regression_PhotonTuned ;
  float ele1_scEerr_regression_PhotonTuned ;

  float ele1_scERaw_PUcleaned;
  float ele1_scEtaWidth_PUcleaned;
  float ele1_scPhiWidth_PUcleaned;
  float ele1_fCorrection_PUcleaned;

  float ele1_fEta;
  float ele1_fEtaCorr;
  float ele1_tkP;
  float ele1_tkPt;
  float ele1_fbrem;
  
  float ele1_dxy_PV ;
  float ele1_dz_PV ;
  float ele1_EcalEnergy ;
  float ele1_sigmaP ;

  float ele1_ooemoop ;
  float ele1_effAreaForIso;

  float ele1_eSeedBC;
  float ele1_e5x5;
  float ele1_e3x3;
  float ele1_scNxtal;
  int   ele1_bcN;
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
  std::vector<int> ele1_recHit_flag;
  std::vector<int> ele1_recHit_hashedIndex;
  std::vector<int> ele1_recHit_ietaORix;
  std::vector<int> ele1_recHit_iphiORiy;
  std::vector<int> ele1_recHit_zside;
  std::vector<float> ele1_recHit_laserCorrection;
  std::vector<float> ele1_recHit_Alpha;
  std::vector<float> ele1_recHit_ICConstant;
  int   ele1_nRecHits;

  std::vector<float> ele1_recHitMatrix_E;
  std::vector<int> ele1_recHitMatrix_flag;
  std::vector<int> ele1_recHitMatrix_hashedIndex;
  std::vector<int> ele1_recHitMatrix_ietaORix;
  std::vector<int> ele1_recHitMatrix_iphiORiy;
  std::vector<int> ele1_recHitMatrix_zside;
  std::vector<float> ele1_recHitMatrix_laserCorrection;
  std::vector<float> ele1_recHitMatrix_ICConstant;
 
  int ele1_isEB;
  int ele1_isEBEEGap;
  int ele1_isEBEtaGap;
  int ele1_isEBPhiGap;
  int ele1_isEEDeeGap;
  int ele1_isEERingGap;

  // Regression V3 variables
  float ele1_eRegrInput_rawE;
  float ele1_eRegrInput_r9;
  float ele1_eRegrInput_eta;
  float ele1_eRegrInput_phi;
  float ele1_eRegrInput_r25;
  float ele1_eRegrInput_hoe;
  float ele1_eRegrInput_etaW;
  float ele1_eRegrInput_phiW;
  float ele1_eRegrInput_rho;
  float ele1_eRegrInput_Deta_bC_sC;
  float ele1_eRegrInput_Dphi_bC_sC;
  float ele1_eRegrInput_bCE_Over_sCE;
  float ele1_eRegrInput_e3x3_Over_bCE;
  float ele1_eRegrInput_e5x5_Over_bCE;
  float ele1_eRegrInput_sigietaieta_bC1;
  float ele1_eRegrInput_sigiphiiphi_bC1;
  float ele1_eRegrInput_sigietaiphi_bC1;
  float ele1_eRegrInput_bEMax_Over_bCE;
  float ele1_eRegrInput_bE2nd_Over_bCE;
  float ele1_eRegrInput_bEtop_Over_bCE;
  float ele1_eRegrInput_bEbot_Over_bCE;

  float ele1_eRegrInput_bEleft_Over_bCE;
  float ele1_eRegrInput_bEright_Over_bCE;
  float ele1_eRegrInput_be2x5max_Over_bCE;
  float ele1_eRegrInput_be2x5top_Over_bCE;
  float ele1_eRegrInput_be2x5bottom_Over_bCE;
  float ele1_eRegrInput_be2x5left_Over_bCE;
  float ele1_eRegrInput_be2x5right_Over_bCE;

  float ele1_eRegrInput_seedbC_eta;
  float ele1_eRegrInput_seedbC_phi;
  float ele1_eRegrInput_seedbC_eta_p5;
  float ele1_eRegrInput_seedbC_phi_p2;
  float ele1_eRegrInput_seedbC_bieta;
  float ele1_eRegrInput_seedbC_phi_p20;
  float ele1_eRegrInput_seedbC_etacry;
  float ele1_eRegrInput_seedbC_phicry;

 
  float ele1_eRegrInput_ESoSC;
  float ele1_eRegrInput_nPV;
  float ele1_eRegrInput_SCsize;
    
  float ele1_inner_p;
  float ele1_inner_x;
  float ele1_inner_y;
  float ele1_inner_z;
  float ele1_outer_p;
  float ele1_outer_x;
  float ele1_outer_y;
  float ele1_outer_z;
  int ele1_tangent_n;
  std::vector<float> ele1_tangent_p;
  std::vector<float> ele1_tangent_x;
  std::vector<float> ele1_tangent_y;
  std::vector<float> ele1_tangent_z;
  std::vector<float> ele1_tangent_dP;
  std::vector<float> ele1_tangent_dPerr;

  
 // ele2 variables  
  ROOT::Math::XYZTVector ele2;
  float ele2_charge;
  float ele2_p;
  float ele2_pt;
  float ele2_eta;
  float ele2_phi;
  int ele2_isTrackerDriven;


  float ele2_sigmaIetaIeta;
  float ele2_DphiIn;
  float ele2_DetaIn;
  float ele2_HOverE;
  float ele2_tkIso;
  float ele2_emIso;
  float ele2_hadIso;

  float ele2_scERaw;
  float ele2_scEtRaw;
  float ele2_scEt;
  float ele2_scLocalEta;
  float ele2_scLocalPhi;
  float ele2_scEtaWidth;
  float ele2_scPhiWidth;
  float ele2_scCrackCorr;
  float ele2_scLocalContCorr;
  float ele2_scE;
  float ele2_scEta;
  float ele2_scPhi;
  float ele2_scLaserCorr;
  float ele2_scE_regression;
  float ele2_scEerr_regression;
  float ele2_scE_regression_PhotonTuned ;
  float ele2_scEerr_regression_PhotonTuned ;
  float ele2_scERaw_PUcleaned;
  float ele2_scEtaWidth_PUcleaned;
  float ele2_scPhiWidth_PUcleaned;
  float ele2_fCorrection_PUcleaned;

  float ele2_fEta;
  float ele2_fEtaCorr;
  float ele2_tkP;
  float ele2_tkPt;
  float ele2_fbrem;

  float ele2_dxy_PV ;
  float ele2_dz_PV ;
  float ele2_sigmaP ;
  float ele2_EcalEnergy ;
  float ele2_effAreaForIso ;
  float ele2_ooemoop ;

  float ele2_eSeedBC;
  float ele2_e5x5;
  float ele2_e3x3;
  float ele2_scNxtal;
  int   ele2_bcN;
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
  std::vector<int> ele2_recHit_flag;
  std::vector<int> ele2_recHit_hashedIndex;
  std::vector<int> ele2_recHit_ietaORix;
  std::vector<int> ele2_recHit_iphiORiy;
  std::vector<int> ele2_recHit_zside;
  std::vector<float> ele2_recHit_laserCorrection;
  std::vector<float> ele2_recHit_Alpha;
  std::vector<float> ele2_recHit_ICConstant;
  int   ele2_nRecHits;

  std::vector<float> ele2_recHitMatrix_E;
  std::vector<int> ele2_recHitMatrix_flag;
  std::vector<int> ele2_recHitMatrix_hashedIndex;
  std::vector<int> ele2_recHitMatrix_ietaORix;
  std::vector<int> ele2_recHitMatrix_iphiORiy;
  std::vector<int> ele2_recHitMatrix_zside;
  std::vector<float> ele2_recHitMatrix_laserCorrection;
  std::vector<float> ele2_recHitMatrix_ICConstant;

 
  int ele2_isEB;
  int ele2_isEBEEGap;
  int ele2_isEBEtaGap;
  int ele2_isEBPhiGap;
  int ele2_isEEDeeGap;
  int ele2_isEERingGap;


 // Regression V3 variables
  float ele2_eRegrInput_rawE;
  float ele2_eRegrInput_r9;
  float ele2_eRegrInput_eta;
  float ele2_eRegrInput_phi;
  float ele2_eRegrInput_r25;
  float ele2_eRegrInput_hoe;
  float ele2_eRegrInput_etaW;
  float ele2_eRegrInput_phiW;
  float ele2_eRegrInput_rho;
  float ele2_eRegrInput_Deta_bC_sC;
  float ele2_eRegrInput_Dphi_bC_sC;
  float ele2_eRegrInput_bCE_Over_sCE;
  float ele2_eRegrInput_e3x3_Over_bCE;
  float ele2_eRegrInput_e5x5_Over_bCE;
  float ele2_eRegrInput_sigietaieta_bC1;
  float ele2_eRegrInput_sigiphiiphi_bC1;
  float ele2_eRegrInput_sigietaiphi_bC1;
  float ele2_eRegrInput_bEMax_Over_bCE;
  float ele2_eRegrInput_bE2nd_Over_bCE;
  float ele2_eRegrInput_bEtop_Over_bCE;
  float ele2_eRegrInput_bEbot_Over_bCE;

  float ele2_eRegrInput_bEleft_Over_bCE;
  float ele2_eRegrInput_bEright_Over_bCE;
  float ele2_eRegrInput_be2x5max_Over_bCE;
  float ele2_eRegrInput_be2x5top_Over_bCE;
  float ele2_eRegrInput_be2x5bottom_Over_bCE;
  float ele2_eRegrInput_be2x5left_Over_bCE;
  float ele2_eRegrInput_be2x5right_Over_bCE;

  float ele2_eRegrInput_seedbC_eta;
  float ele2_eRegrInput_seedbC_phi;
  float ele2_eRegrInput_seedbC_eta_p5;
  float ele2_eRegrInput_seedbC_phi_p2;
  float ele2_eRegrInput_seedbC_bieta;
  float ele2_eRegrInput_seedbC_phi_p20;
  float ele2_eRegrInput_seedbC_etacry;
  float ele2_eRegrInput_seedbC_phicry;

 
  float ele2_eRegrInput_ESoSC;
  float ele2_eRegrInput_nPV;
  float ele2_eRegrInput_SCsize;
 
  float ele2_inner_p;
  float ele2_inner_x;
  float ele2_inner_y;
  float ele2_inner_z;
  float ele2_outer_p;
  float ele2_outer_x;
  float ele2_outer_y;
  float ele2_outer_z;
  int ele2_tangent_n;
  std::vector<float> ele2_tangent_p;
  std::vector<float> ele2_tangent_x;
  std::vector<float> ele2_tangent_y;
  std::vector<float> ele2_tangent_z;
  std::vector<float> ele2_tangent_dP;
  std::vector<float> ele2_tangent_dPerr;
 
  // met variables
  ROOT::Math::XYZTVector met;
  ROOT::Math::XYZTVector* p_met;
  float met_et;
  float met_phi;

  float ele1Met_mt;
  float ele1Met_Dphi;
  
  
  // di-electron variables
  float ele1ele2_m;
  float ele1ele2_scM;
  float ele1ele2_scM_regression;
  float ele1ele2_scMZS;

  float PUit_TrueNumInteractions;
  int PUit_NumInteractions;
  float PUit_zpositions ;
  float PUit_sumpT_lowpT ;
  float PUit_sumpT_highpT ;
  float PUit_ntrks_lowpT ;
  float PUit_ntrks_highpT;

  float PUoot_early_TrueNumInteractions ;
  int PUoot_early ;
  float PUoot_early_zpositions ;
  float PUoot_early_sumpT_lowpT ;
  float PUoot_early_sumpT_highpT;
  float PUoot_early_ntrks_lowpT ;
  float PUoot_early_ntrks_highpT;

  float PUoot_late_TrueNumInteractions;
  int PUoot_late ;
  float PUoot_late_zpositions ;
  float PUoot_late_sumpT_lowpT ;
  float PUoot_late_sumpT_highpT ; 
  float PUoot_late_ntrks_lowpT ;
  float PUoot_late_ntrks_highpT ;
 
  
  // MC Info
  float mcV_E;
  float mcV_Px;
  float mcV_Py;
  float mcV_Pz;
  int mcV_Charge;
  int mcV_PdgId;

  float mcF1_fromV_E;
  float mcF1_fromV_Px;
  float mcF1_fromV_Py;
  float mcF1_fromV_Pz;
  int mcF1_fromV_Charge;
  int mcF1_fromV_PdgId;

  float mcF2_fromV_E;
  float mcF2_fromV_Px;
  float mcF2_fromV_Py;
  float mcF2_fromV_Pz;
  int mcF2_fromV_Charge;
  int mcF2_fromV_PdgId;
} ;

#endif
