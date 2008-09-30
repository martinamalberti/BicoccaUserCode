#ifndef EcalValidation_EtaValidation_EtaAnalyzerWithMCTreeGenerator_h
#define EcalValidation_EtaValidation_EtaAnalyzerWithMCTreeGenerator_h
/**\class EtaAnalyzerMCTreeGenerator
 **
 ** Description: Get Photon collection from the event and make very basic histos
 ** $Date: 2008/09/26 14:36:23 $
 ** $Revision: 1.4 $
 ** \author Nancy Marinelli, U. of Notre Dame, US
 **
 **/
//
//



// system include files
#include <memory>

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//---- local containment corrections ----
#include "CondFormats/EcalCorrections/interface/EcalGlobalShowerContainmentCorrectionsVsEta.h"
#include "CondFormats/DataRecord/interface/EcalGlobalShowerContainmentCorrectionsVsEtaRcd.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"


#include <FWCore/Framework/interface/EDAnalyzer.h>
#include <FWCore/Framework/interface/Event.h>
#include <FWCore/Framework/interface/MakerMacros.h>

#include "CondFormats/EcalCorrections/interface/EcalGlobalShowerContainmentCorrectionsVsEta.h"
#include "CondFormats/DataRecord/interface/EcalGlobalShowerContainmentCorrectionsVsEtaRcd.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/EcalDetId/interface/EBDetId.h"

#include "RecoEcal/EgammaCoreTools/interface/PositionCalc.h"








#include <string>
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TTree.h"
class TFile;


class EtaAnalyzerWithMCTreeGenerator : public edm::EDAnalyzer {
 public:
  explicit EtaAnalyzerWithMCTreeGenerator( const edm::ParameterSet& );
  ~EtaAnalyzerWithMCTreeGenerator();


  virtual void analyze(const edm::Event&, const edm::EventSetup& );
  virtual void beginJob(edm::EventSetup const&);
  virtual void endJob();
  
 private:
  
  double deltaPhi(double phi1, double phi2);
  float  etaTransformation(float a, float b);

  std::string mcProducer_;
  std::string mcCollection_;
  std::string photonCollectionProducer_;       
  std::string photonCollection_;       

  edm::InputTag IsolationProducer_;

  edm::InputTag barrelEcalHits_;
  edm::InputTag endcapEcalHits_;

//   edm::InputTag islandBarrelBasicClusters_;
//   edm::InputTag islandBarrelShapeAssoc_;
//   edm::InputTag islandEndcapBasicClusters_;
//   edm::InputTag islandEndcapShapeAssoc_;

  std::string islandBarrelBasicClustersProducer_;       
  std::string islandBarrelBasicClusters_;       

  std::string islandBarrelShapeAssocProducer_;       
  std::string islandBarrelShapeAssoc_;       

  std::string islandEndcapBasicClustersProducer_;       
  std::string islandEndcapBasicClusters_;       

  std::string islandEndcapShapeAssocProducer_;       
  std::string islandEndcapShapeAssoc_;       

  
    
  double MinClusterEt_;
  double MinPtGamma_;
  double MinPtGammaPair_ ;
  double IsolationCut_ ;
  double MinE4oE9_;
  double RMax_ ;
  double DeltaEtaMax_ ;  

  edm::ESHandle<CaloTopology> theCaloTopo_;

  std::string vertexProducer_;
  
  TTree * tTreeUtilities_;
  int numC_;
  int numCB_;
  int numCE_;
  std::vector<double> * etaC_;
  std::vector<double> * thetaC_;
  std::vector<double> * phiC_;
  std::vector<double> * S4oS9C_;
  std::vector<double> * S4C_;
  std::vector<double> * S9C_;
  std::vector<double> * S16C_;
  std::vector<double> * S25C_;
  std::vector<double> * pxC_;
  std::vector<double> * pyC_;
  std::vector<double> * pzC_;
  std::vector<double> * etC_;
  std::vector<double> * covEtaEtaC_;//---- appena aggiunti ----
  std::vector<double> * covEtaPhiC_;//---- appena aggiunti ----
  std::vector<double> * covPhiPhiC_;//---- appena aggiunti ----
  std::vector<int> * HitsC_;
  std::vector<double> * HitsEnergyC_;
  std::vector<int> * HitsRawIdC_;
  
  int numEta_;
  std::vector<int> * numPh_; //---- number of photons for each Eta ----
  std::vector<double> * thetaPh_;
  std::vector<double> * etaPh_;
  std::vector<double> * phiPh_;
  std::vector<double> * pxPh_;
  std::vector<double> * pyPh_;
  std::vector<double> * pzPh_;
  

};
#endif
