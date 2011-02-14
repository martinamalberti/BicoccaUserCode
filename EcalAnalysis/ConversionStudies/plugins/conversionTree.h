#ifndef conversionTree_h
#define conversionTree_h

// system include files
#include "PhysicsTools/NtupleUtils/interface/NtupleFactory.h"

#include <memory>
#include <vector>
#include <map>
#include <set>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/Common/interface/AssociationMap.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/RefToBase.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "RecoTracker/Record/interface/TrackerRecoGeometryRecord.h"

//----objects----
#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "DataFormats/CaloRecHit/interface/CaloCluster.h"
#include "DataFormats/CaloRecHit/interface/CaloClusterFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFClusterFwd.h"

//----refit----
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "RecoVertex/PrimaryVertexProducer/interface/PrimaryVertexSorter.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "RecoVertex/GaussianSumVertexFit/interface/GsfVertexFitter.h"
#include "RecoVertex/GaussianSumVertexFit/interface/AdaptiveGsfVertexFitter.h"

//trigger
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"

//MC dumper
#include "EcalAnalysis/ConversionStudies/interface/MCDumperPi0.h"

using namespace edm;

class conversionTree : public edm::EDAnalyzer 
{
 public:
  
  //  typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > vector4D;
  
  explicit conversionTree (const edm::ParameterSet&) ;
  ~conversionTree () ;
  
 protected:
  
  virtual void beginJob () ;
  virtual void analyze (const edm::Event&, const edm::EventSetup&) ;
  virtual void endJob () ;
  
  void fillHLTInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillConversionInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillPhotonInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillPFPhotonInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillPFClusterInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillIslandClusterInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillBSInfo(const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillPVInfo(const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillMCInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillMCPi0Info (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;

  
 protected:
  
  TTree* outTree_ ;
  NtupleFactory* NtupleFactory_;

  math::XYZPoint BSPoint_;
  math::XYZPoint PVPoint_;
    
  ///---- input tag ----
  edm::InputTag TriggerEventTag_;
  edm::InputTag TriggerResultsTag_;
  edm::InputTag PVTag_;
  edm::InputTag MCtruthTag_;
  edm::InputTag PhotonTag_;
  edm::InputTag EBRechitTag_;
  edm::InputTag EERechitTag_;

  bool savePV_;
  bool saveBS_;
  bool saveHLT_;
  bool saveConversion_;
  bool savePhoton_;
  bool savePFPhoton_;
  bool savePFCluster_;
  bool saveIslandCluster_;
  bool saveMC_;
  ParameterSet gsfPSet_;

  bool saveMCPi0_ ;

  int eventType_; //---- 0 = signal    1 = background 
  bool verbosity_; //---- true = loquacious    false = silence  
  int eventNaiveId_;

  MCDumperPi0*    mcAnalysisPi0_;
} ;

#endif
