#ifndef AlCaL1Efficiencies_H
#define AlCaL1Efficiencies_H 

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <memory>
#include <string>
#include <iostream>
#include <algorithm>

#include "TFile.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"


class AlCaL1Efficiencies : public edm::EDAnalyzer {

  public:

    explicit AlCaL1Efficiencies (const edm::ParameterSet&) ;
    ~AlCaL1Efficiencies () {}
    virtual void analyze (const edm::Event& iEvent, 
                          const edm::EventSetup& iSetup) ;
    virtual void beginJob () ;
    virtual void endJob () ;

  private:

    std::string m_outputFileName ;
    /// InputTag for the L1 Global Trigger DAQ readout record
    edm::InputTag m_L1GtReadoutRecordTag ;
       
} ;
#endif
