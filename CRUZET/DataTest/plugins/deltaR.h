// -*- C++ -*-
//
// Package:   deltaR
// Class:     deltaR
//
/**\class deltaR deltaR.h

Description: <one line class summary>

Implementation:
<Notes on implementation>
 */
// $Id: deltaR.h,v 1.3 2008/11/13 17:33:37 govoni Exp $
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

#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalUncalibratedRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalRawData/interface/EcalRawDataCollections.h"

#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"

#include "CaloOnlineTools/EcalTools/interface/EcalTrackLengthMeasurement.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TGraph.h"
#include "TTree.h"

#include <vector>

// *** for TrackAssociation
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/Common/interface/Handle.h"
#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
#include "TrackingTools/TrackAssociator/interface/TrackAssociatorParameters.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
// ***

//for track length
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/EcalAlgo/interface/EcalBarrelGeometry.h"
#include "RecoCaloTools/Navigation/interface/CaloNavigator.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
//




class deltaR : public edm::EDAnalyzer 
{
   public:

      explicit deltaR (const edm::ParameterSet&) ;
      ~deltaR () ;

   private:

      virtual void beginJob (const edm::EventSetup&) {} ;
      virtual void analyze (const edm::Event&, const edm::EventSetup&) ;
      virtual void endJob () ;
      void initHists (int) ;

   private:

      edm::InputTag barrelSuperClusterCollection_ ;
      edm::InputTag endcapSuperClusterCollection_ ;
      edm::InputTag muonTracksCollection_ ;
  
      TrackDetectorAssociator trackAssociator_ ;
      TrackAssociatorParameters trackParameters_ ;
      
      TH1F * m_deltaRSCMu ;
      TH1F * m_deltaPhiSCMu ;
      TH1F * m_deltaEtaSCMu ;
      
      int m_totalMuons ;
    
} ;
