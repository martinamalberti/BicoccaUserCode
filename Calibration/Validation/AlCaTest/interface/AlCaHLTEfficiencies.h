#ifndef AlCaHLTEfficiencies_H
#define AlCaHLTEfficiencies_H 

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
#include "DataFormats/Provenance/interface/ParameterSetID.h"

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


class AlCaHLTEfficiencies : public edm::EDAnalyzer {

  public:

    explicit AlCaHLTEfficiencies (const edm::ParameterSet&) ;
    ~AlCaHLTEfficiencies () {}
    virtual void analyze (const edm::Event& iEvent, 
                          const edm::EventSetup& iSetup) ;
    virtual void beginJob () ;
    virtual void endJob () ;

  private:

    edm::InputTag m_HLTResultsTag ;
    std::string m_outputFileName ;
    
    const edm::ParameterSetID m_HLTPSetID ;
       
} ;
#endif
