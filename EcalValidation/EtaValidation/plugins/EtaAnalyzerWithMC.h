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
class TFile;


class EtaAnalyzerWithMC : public edm::EDAnalyzer {
 public:
  explicit EtaAnalyzerWithMC( const edm::ParameterSet& );
  ~EtaAnalyzerWithMC();


  virtual void analyze( const edm::Event&, const edm::EventSetup& );
  virtual void beginJob(edm::EventSetup const&);
  virtual void endJob();
 private:

  float  etaTransformation( float a, float b);

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
    
  TH1F *hNphotons_;

  TH1F* hSCe1_;
  TH1F* hSCet1_;
  TH1F* hSCet2_;
  TH1F* hInvMass_;
  TH1F* hInvMassEta_;
  TH1F* hInvMassEtaEndcap_;
  TH1F* hInvMassEtaBarrel_;
  TH1F* hInvMassNoEta_;
  TH1F* hInvMassNoEtaEndcap_;
  TH1F* hInvMassNoEtaBarrel_;
  
  TH1F* hEtaPhEnergy_;
  TH2F* hKindOfParticle_;
  TH2F* hKindOfGeneratedParticle_;
  
  TH1F* hPtgg_;

  TH1F* hS4oS9_;

  TH1F* hR_;
  TH1F* hIsolation_;



};
#endif
