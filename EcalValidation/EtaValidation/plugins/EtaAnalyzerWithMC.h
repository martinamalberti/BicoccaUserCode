#ifndef EcalValidation_EtaValidation_EtaAnalyzerWithMC_h
#define EcalValidation_EtaValidation_EtaAnalyzerWithMC_h
/**\class EtaAnalyzer
 **
 ** Description: Get Photon collection from the event and make very basic histos
 ** $Date: 2008/08/28 12:52:42 $
 ** $Revision: 1.1 $
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

#include <string>
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TTree.h"
class TFile;


class EtaAnalyzerWithMC : public edm::EDAnalyzer {
 public:
  explicit EtaAnalyzerWithMC( const edm::ParameterSet& );
  ~EtaAnalyzerWithMC();


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

  double MinClusterEt_;
  double MinPtGamma_;
  double MinPtGammaPair_ ;
  double IsolationCut_ ;
  double MinE4oE9_;
  double RMax_ ;
  double DeltaEtaMax_ ;  

  edm::ESHandle<CaloTopology> theCaloTopo_;

  std::string vertexProducer_;
    
  
  
  TH1F *hNEtaInEvent_;
  TH1F *hNphotonsFromEta_;
  TH1F *hNphotonsInEvent_;
  
  TH1F* hSCe1_;
  TH1F* hSCet1_;
  TH1F* hSCet2_;
  TH1F* hInvMass_;
  TH1F* hInvMassEta_;
  TH1F* hInvMassEtaEE_;
  TH1F* hInvMassEtaBB_;
  TH1F* hInvMassEtaEB_;
  TH1F* hInvMassNoEta_;
  TH1F* hInvMassNoEtaEndcap_;
  TH1F* hInvMassNoEtaBarrel_;
  
  TH1F* hInvMassPi0_;
  TH1F* hInvMassPi0EE_;
  TH1F* hInvMassPi0BB_;
  TH1F* hInvMassPi0EB_;
  TH1F* hInvMassNoPi0_;
  
  
  
  TH1F* hEtaPhEnergy_;
  TH2F* hKindOfParticle_;
  TH2F* hKindOfGeneratedParticle_;
  
  TH1F* hPtgg_;

  TH1F* hS4oS9_;

  TH1F* hR_;
  TH1F* hIsolation_;

  TH1F* hInvMassEtaPh_;//---- invariant mass of n-photons from eta from MC truth
  TH1F* hInvMassEtaPhCMatch_; //---- invariant mass after matching two photons with two Clusters. Using MC truth to calculate invariant mass

  TH2F* hInvMassMCAndC_;
  
  
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
  std::vector<int> * HitsC_;
  std::vector<double> * HitsEnergyC_;
  
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
