#ifndef SimpleNtple_h
#define SimpleNtple_h

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

#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"

#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "RecoEgamma/EgammaTools/interface/ConversionFinder.h"
#include "RecoEgamma/EgammaTools/interface/ConversionInfo.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
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
#include "DataFormats/Common/interface/TriggerResults.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"

#include "PhysicsTools/NtupleUtils/interface/NtupleFactory.h"

using namespace cms ;
using namespace edm ;
using namespace std ;
using namespace reco;



class SimpleNtple : public edm::EDAnalyzer {
   
 public:

  explicit SimpleNtple (const edm::ParameterSet&) ;
  ~SimpleNtple () ;

      
 private:  
  virtual void analyze (const edm::Event&, const edm::EventSetup&) ;

 protected:

  void fillL1Info (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillHLTInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillEleInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillMuInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillCALOMetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillTCMetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillPFMetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillJetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;
  void fillPFJetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) ;

      // ----------member data ---------------------------

      ///---- input tag ----
  edm::InputTag recHitCollection_EB_;
  edm::InputTag recHitCollection_EE_;
  edm::InputTag HLTTag_;
  edm::InputTag L1Tag_;
  edm::InputTag EleTag_;
  edm::InputTag MuTag_;
  edm::InputTag CALOMetTag_;
  edm::InputTag TCMetTag_;
  edm::InputTag PFMetTag_;
  edm::InputTag JetTag_;
  edm::InputTag TrackTag_;
  edm::InputTag DCSTag_;

  std::vector<std::string> eleId_names_;
  
     ///---- flags ----
  bool dataFlag_ ;
  bool saveL1_ ;
  bool saveHLT_ ;
  bool saveMu_ ;
  bool saveEle_ ;
  bool saveEleShape_ ;
  bool saveJet_ ;
  bool saveCALOMet_ ;
  bool saveTCMet_ ;
  bool savePFMet_ ;

  bool verbosity_; //---- true = loquacious     false = silence  
  
  int eventNaiveId_;


      ///---- output ----
  TTree* outTreeNameHLT;
  TTree* outTreeNameEleId;
  TTree* outTree_;
  NtupleFactory* NtupleFactory_;


} ;

#endif
