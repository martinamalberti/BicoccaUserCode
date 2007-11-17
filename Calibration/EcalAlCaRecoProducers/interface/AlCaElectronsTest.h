#ifndef AlCaElectronsTest_H
#define AlCaElectronsTest_H 

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include <memory>
#include <string>
#include <iostream>
#include <algorithm>
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "TFile.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TH1.h"
#include "TH2.h"
#include "Calibration/EcalAlCaRecoProducers/interface/varHisto.h"


class AlCaElectronsTest : public edm::EDAnalyzer {
  public:
    explicit AlCaElectronsTest (const edm::ParameterSet&) ;
    ~AlCaElectronsTest () {}
     virtual void analyze (const edm::Event& iEvent, 
                           const edm::EventSetup& iSetup) ;
     virtual void beginJob (const edm::EventSetup& iSetup) ;
     virtual void endJob () ;

  private:

     DetId findMaxHit (const std::vector<DetId> & v1,
                       const EBRecHitCollection* EBhits,
                       const EERecHitCollection* EEhits) ;
                       
     double Energy25Barrel (int eta, int phi, const EBRecHitCollection* EBhits) ; //MF
     double Energy25Endcap (int ics, int ips, int z, const EERecHitCollection* EEhits) ; //MF
     
     //!MF riempie grafico 5x5
     void EnergyOn25Barrel (int eta, int phi, const EBRecHitCollection* EBhits) ;
     
     void fillAroundBarrel (const EcalRecHitCollection * recHits, 
                            int eta, int phi, double momentum) ;
     void fillAroundEndcap (const EcalRecHitCollection * recHits, 
                            int ics, int ips, double momentum) ;


  private:

    edm::InputTag m_ElectronLabel ;
    edm::InputTag m_barrelAlCa ;
    edm::InputTag m_endcapAlCa ;

    //! Maps of the electrons hits 
    TH2F * m_barrelGlobalCrystalsMap ;
    TH2F * m_endcapGlobalCrystalsMap ;
    //! Maps of the energy deposits 
    TH2F * m_barrelGlobalCrystalsEnergyMap ;
    TH2F * m_endcapGlobalCrystalsEnergyMap ;
    //! Energy aroung the MOX
    TH2F * m_barrelLocalCrystalsEnergy ;
    TH2F * m_endcapLocalCrystalsEnergy ;
    //! Energy on 25
    TH1F * m_barrelEoE25 ;
    TH1F * m_endcapEoE25 ;
    //!E and p distros
    TH1F * m_barrelPdistro ;
    TH1F * m_endcapPdistro ;
    TH1F * m_barrelEdistro ;
    TH1F * m_endcapEdistro ;
    TH1F * m_barrelE25distro ;
    TH1F * m_endcapE25distro ;
    //EoP 
    TH1F * m_barrelEoPFive ;
    TH1F * m_endcapEoPFive ;
    TH1F * m_barrelEoP ;
    TH1F * m_endcapEoP ;

    TH1F * m_etaTkDistro ;
    TH1F * m_phiTkDistro ;

    TH2F * m_E25oPdiff ;
    TH2F * m_E25oEseedOpout ;
    TH2F * m_E25oESCOpin ;
    TH2F * m_EseedVSesc ;
 
    TH2F * m_E25vsEta ;
    TH2F * m_E25vsEtaIndexEB ;
    TH2F * m_E25vsRadIndexEE ;
    
    varHisto * m_EseedOpout ;
    varHisto * m_ESCOpin ;
    varHisto * m_piMpoOpi ; 
 
} ;
#endif
