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
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TFile.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"


class AlCaElectronsTest : public edm::EDAnalyzer {
  public:
    explicit AlCaElectronsTest (const edm::ParameterSet&) ;
    ~AlCaElectronsTest () {}
     virtual void analyze (const edm::Event& iEvent, 
                           const edm::EventSetup& iSetup) ;
     virtual void beginJob () ;
     virtual void endJob () ;

  private:

     double Energy25Barrel (int eta, int phi,int side, const EBRecHitCollection* EBhits) ; //MF
     double Energy25Endcap (int ics, int ips, int z,int side, const EERecHitCollection* EEhits) ; //MF
     
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
    //! Energy aroung the MOX
    TH2F * m_barrelLocalCrystalsEnergy ;
    TH2F * m_endcapLocalCrystalsEnergy ;
    //! Energy on 25
    TH1F * Zee ; 
    std::string m_outputFileName;
    TFile * m_File;
    TTree * m_tree;
    float m_eta;
    float m_phi;
    float m_pTk;
    float m_MaxEnergy;
    float m_energy;
    float m_Energy25;
    float m_ESCoP;
    float m_eSeedOverPout;
    float m_pOut;
    float m_preshower;
    float m_Energy9;
    float m_Energy49;
    float m_pErr;
    int m_class;
    float m_recHits;
    double m_chis;
    int m_lost;
    int m_found;
    double m_momentum;
    double m_ndof;
    double m_chisN;
} ;
#endif
