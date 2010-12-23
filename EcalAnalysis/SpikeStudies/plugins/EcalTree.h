#ifndef EcalTree_h
#define EcalTree_h

// -*- C++ -*-
//
// Package:   ESCosmicsTree
// Class:     ESCosmicsTree
//
/**\class ESCosmicsTree ESCosmicsTree.h

Description: <one line class summary>

Implementation:
<Notes on implementation>
 */
//
// Original Authors:  Federico DE GUIO Martina MALBERTI
//         Created:  Mo Jul 14 5:46:22 CEST 2008
// $Id: EcalTree.h,v 1.2 2010/07/07 14:57:16 deguio Exp $
//
//

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
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"

#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalUncalibratedRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"

#include "CondFormats/EcalObjects/interface/EcalIntercalibConstants.h"

#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"

#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronCore.h"

#include "TFile.h"
#include "TTree.h"

#include "EcalAnalysis/SpikeStudies/interface/EcalTreeContent.h"

using namespace reco;

class EcalTree : public edm::EDAnalyzer 
{
   
   public:

      explicit EcalTree (const edm::ParameterSet&) ;
      ~EcalTree () ;

      
   private:  
      virtual void beginJob ();
      virtual void analyze (const edm::Event&, const edm::EventSetup&) ;
      virtual void endJob () ;

    
   protected:
 
     
      /*       ! dump  information */
      void dumpL1Info(edm::Handle<L1GlobalTriggerReadoutRecord>  gtRecord,
		      EcalTreeContent & myTreeVariables) ;

      //! dump  information
      bool dumpBarrelInfo( const CaloTopology* theCaloTopology,
			   const CaloGeometry* theCaloGeometry,
			   const EBDigiCollection* theEcalBarrelDigis,
			   const EcalRecHitCollection* theBarrelEcalRecHits,
			   const GsfElectronCollection* electrons,
			   EcalTreeContent & myTreeVariables_) ;

      bool dumpEndcapInfo( const CaloTopology* theCaloTopology,
			   const CaloGeometry* theCaloGeometry,
			   const EEDigiCollection* theEcalEndcapDigis,
			   const EcalRecHitCollection* theEndcapEcalRecHits,
			   const GsfElectronCollection* electrons,
			   EcalTreeContent & myTreeVariables_) ;

      void dumpJetInfo(const CaloTopology* theCaloTopology,
		       const CaloGeometry* theCaloGeometry,
		       const reco::CaloJetCollection* theJetCollection,
		       EcalTreeContent & myTreeVariables_) ;


      edm::InputTag ebRecHitCollection_ ;
      //edm::InputTag ebClusterCollection_ ;
      edm::InputTag ebDigiCollection_;
      edm::InputTag eeRecHitCollection_ ;
      //edm::InputTag eeClusterCollection_ ;
      edm::InputTag eeDigiCollection_;
      edm::InputTag GsfEleTag_;
      edm::InputTag L1InputTag_;
      edm::InputTag ak5CaloJets_;
      edm::InputTag MetTag_;
      edm::InputTag PFMetTag_;
      edm::InputTag TcMetTag_;


      double radiusForIso_;
      double energyCutForIso_;
      double minRecHitEnergy_;
    
      int naiveId_;
    
      EcalTreeContent myTreeVariables_ ;

      TTree* tree_ ;

     
} ;

#endif
