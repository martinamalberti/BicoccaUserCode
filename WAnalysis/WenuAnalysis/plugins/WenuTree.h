#ifndef WenuTree_h
#define WenuTree_h

// system include files
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

#include "Geometry/CaloTopology/interface/CaloTopology.h"

#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"

#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"


#include "TFile.h"
#include "TTree.h"

#include "WAnalysis/WenuAnalysis/interface/WenuTreeContent.h"

using namespace cms ;
using namespace edm ;
using namespace std ;
using namespace reco;

class WenuTree : public edm::EDAnalyzer 
{
   
   public:

      explicit WenuTree (const edm::ParameterSet&) ;
      ~WenuTree () ;

      
   private:  
      virtual void beginJob ();
      virtual void analyze (const edm::Event&, const edm::EventSetup&) ;
      virtual void endJob () ;

    
   protected:
 
     
     
      void dumpL1Info(edm::Handle<L1GlobalTriggerReadoutRecord>  gtRecord,
		      WenuTreeContent & myTreeVariables) ;


      void dumpHLTInfo(Handle<TriggerResults> hltresults,
		       const edm::TriggerNames & triggerNames,
		       WenuTreeContent & myTreeVariables) ;
     
      void dumpElectronInfo( View<pat::Electron>  electrons, 
			     const EcalRecHitCollection* theBarrelEcalRecHits,
			     const EcalRecHitCollection* theEndcapEcalRecHits,
			     const CaloTopology* theCaloTopology, 
			     WenuTreeContent & myTreeVariables_) ;
  
      void dumpCALOMetInfo ( View<pat::MET>            mets, WenuTreeContent & myTreeVariables_) ;
      void dumpTCMetInfo   ( View<pat::MET>            mets, WenuTreeContent & myTreeVariables_) ;
      void dumpPFMetInfo   ( View<pat::MET>            mets, WenuTreeContent & myTreeVariables_) ;
 
      void dumpJetInfo ( View<pat::Jet>            jets, WenuTreeContent & myTreeVariables_) ;
      void dumpMuonInfo( View<pat::Muon>          muons, WenuTreeContent & myTreeVariables_) ;
      

        


      // ----------member data ---------------------------
      edm::InputTag recHitCollection_EB_;
      edm::InputTag recHitCollection_EE_;
      edm::InputTag eleLabel_ ;
      edm::InputTag calometLabel_ ;
      edm::InputTag tcmetLabel_ ;
      edm::InputTag pfmetLabel_ ;
      edm::InputTag jetLabel_ ;
      edm::InputTag muonLabel_ ;
      std::string   electronID_;
      std::string   btagAlgo_;
      edm::InputTag TrkIsolationProducer_ ;
      edm::InputTag EcalIsolationProducer_ ;
      edm::InputTag HcalIsolationProducerDepth1_ ;
      edm::InputTag HcalIsolationProducerDepth2_ ;
      
      edm::InputTag L1InputTag_;
      edm::InputTag HLTInputTag_ ;
     
      edm::InputTag pdfWeightsLabel_ ;
      bool storePDFWeights_;
      bool runOnMC_;
      
      int naiveId_;
    
      WenuTreeContent myTreeVariables_ ;

      TTree* tree_ ;

     
} ;

#endif
