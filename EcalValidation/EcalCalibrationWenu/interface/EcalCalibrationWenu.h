// -*- C++ -*-
//
// Package:    EcalCalibrationWenu
// Class:      EcalCalibrationWenu
// 
/**\class EcalCalibrationWenu EcalCalibrationWenu.h EcalValidation/EcalCalibrationWenu/interface/EcalCalibrationWenu.h

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Andrea Massironi
//         Created:  Wed Oct 27 12:13:56 CEST 2010
// $Id$
//
//


#ifndef EcalCalibrationWenu_h
#define EcalCalibrationWenu_h

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TH1.h"
#include "TTree.h"
#include "TMath.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Electron.h"


#include "Geometry/EcalAlgo/interface/EcalBarrelGeometry.h"
#include "Geometry/EcalAlgo/interface/EcalEndcapGeometry.h"

#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"
#include "CondFormats/EcalObjects/interface/EcalChannelStatus.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"

#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/deltaPhi.h"



//
// class declaration
//

class EcalCalibrationWenu : public edm::EDAnalyzer {
   public:
      explicit EcalCalibrationWenu(const edm::ParameterSet&);
      ~EcalCalibrationWenu();


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
      
       std::vector<std::string> eleId_names_;
       edm::InputTag recHitCollection_EB_;
       edm::InputTag recHitCollection_EE_;
       edm::InputTag EleTag_;
       edm::InputTag CALOMetTag_;
       edm::InputTag TrackTag_;

       TTree* myTree_;
       
       double HoE_;
       double SigmaIEtaIEta_;
       double eleTrkIso_;
       double eleEcalIso_;
       double eleHcalIsoD1_;
       double eleHcalIsoD2_;
       unsigned int eleMisHits_;
       double met_;
       double sumEt_;
       double eta_;
       double phi_;
       double pT_;
       double ET_;
       double MT_;
       double EoP_;
       double eleFBrem_;
       double eleES_;
       double E2x2_;
       double E3x3_;
       double E5x5_;
       double ESC_;
       double ETSC_;
       double Sigma_Phi_;
       double Sigma_Eta_;
       double pIn_;
       double pOut_;
       double pAtCalo_;
       double p_;
       double E9oE25_;
       double DeltaEtaIn_;
       double DeltaPhiIn_;
       double etaSC_;
       double phiSC_;
       int nGoodElectrons_;
       int iSM_;
       int iSC_;
       double dphiMETEle_;
       double eleCharge_;
       double eleSwissCross_;
       int seedSeverityLevel_;
       int* eleId_;
       int iDetEB_;
       int iDetEE_;
       
       int eventNaiveId_;
       int runId_;
       int lumiId_;
       int eventId_;
       int BXId_;

};


#endif
