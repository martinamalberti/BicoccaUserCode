#ifndef AlCaRecHitsTest_H
#define AlCaRecHitsTest_H 

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


class AlCaRecHitsTest : public edm::EDAnalyzer {
  public:
    explicit AlCaRecHitsTest (const edm::ParameterSet&) ;
    ~AlCaRecHitsTest () {}
     virtual void analyze (const edm::Event& iEvent, 
                           const edm::EventSetup& iSetup) ;
     virtual void beginJob () ;
     virtual void endJob () ;

  private:

     void fillAroundBarrel (const EcalRecHitCollection * recHits, 
                            int eta, int phi, double momentum) ;
     void fillAroundEndcap (const EcalRecHitCollection * recHits, 
                            int ics, int ips, double momentum) ;

     DetId findMax (const EcalRecHitCollection *) ;

  private:

    edm::InputTag m_ElectronLabel ;
    edm::InputTag m_barrelRecHits ;
    edm::InputTag m_endcapRecHits ;
    edm::InputTag m_preshowerRecHits ;

    //! Maps of the electrons hits 
    TH2F * m_barrelGlobalCrystalsMap ;
    TH2F * m_endcapGlobalCrystalsMap ;
    TH2F * m_preshowerGlobalChannelsMap ;
    //! Energy aroung the MOX
    TH2F * m_barrelLocalCrystalsMap ;
    TH2F * m_endcapLocalCrystalsMap ;
    TH2F * m_preshowerLocalChannelsMap ;

    std::string m_outputFileName;
    
} ;
#endif
