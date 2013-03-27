#ifndef SimpleNtupleRecHits_h
#define SimpleNtupleRecHits_h

#include <vector>
#include <map>
#include "TFile.h"
#include "TTree.h"

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
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "RecoVertex/PrimaryVertexProducer/interface/PrimaryVertexSorter.h"
#include "RecoVertex/PrimaryVertexProducer/interface/VertexHigherPtSquared.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalDetId/interface/EcalDetIdCollections.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalDetId/interface/EcalTrigTowerDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h" 

#include "PhysicsTools/NtupleUtils/interface/NtupleFactory.h"



class SimpleNtupleRecHits : public edm::EDAnalyzer
{
 public:
  
  explicit SimpleNtupleRecHits(const edm::ParameterSet&);
  ~SimpleNtupleRecHits();
  
      
 private:  
  
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  
  
 protected:

  void fillPVInfo (const edm::Event & iEvent, const edm::EventSetup & iSetup) ;
  void fillRhoInfo(const edm::Event & iEvent, const edm::EventSetup & iSetup) ;
  void fillMCPUInfo (const edm::Event & iEvent, const edm::EventSetup & iSetup) ;
  void fillRecHitsInfo (const edm::Event & iEvent, const edm::EventSetup & iSetup);
  
  
  ///---- input tag ----
  edm::InputTag PVTag_;
  edm::InputTag rhoTag_;
  edm::InputTag MCPileupTag_;
  edm::InputTag digiCollection_EB_;
  edm::InputTag digiCollection_EE_;
  edm::InputTag recHitCollection_EB_;
  edm::InputTag recHitCollection_EE_;
  
  bool saveMCPU_;
  bool verbosity_; //---- true = loquacious     false = silence  
  bool dataFlag_;
  
  int eventNaiveId_;
 
  math::XYZPoint PVPoint_;

  ///---- output ----
  TTree* outTree_;
  
  // event variables
  long int bxId;
  long int eventId;
  int lumiId;
  int runId;
  int timeStampHigh;

  // rho variables
  float rho;
  
  // PV variables
  int PV_n;
  float PV_z;
  float PV_d0;
  float PV_SumPt;
  
  // recHit variable
  std::vector<float> EBRecHit_E;
  std::vector<int> EBRecHit_flag;
  std::vector<int> EBRecHit_hashedIndex;
  std::vector<int> EBRecHit_ietaORix;
  std::vector<int> EBRecHit_iphiORiy;
  std::vector<int> EBRecHit_zside;
  std::vector<float> EBRecHit_laserCorrection;
  std::vector<float> EBRecHit_ICConstant;
  std::vector<float> EBRecHit_samples;
  
  std::vector<float> EERecHit_E;
  std::vector<int> EERecHit_flag;
  std::vector<int> EERecHit_hashedIndex;
  std::vector<int> EERecHit_ietaORix;
  std::vector<int> EERecHit_iphiORiy;
  std::vector<int> EERecHit_zside;
  std::vector<float> EERecHit_laserCorrection;
  std::vector<float> EERecHit_ICConstant;
  std::vector<float> EERecHit_samples;
  
  // PU variables
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
};

#endif
