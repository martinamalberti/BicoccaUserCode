#ifndef hlt_optimizer_H
#define hlt_optimizer_H


//#include <memory>
//
//// user include files
//#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
//
//#include "FWCore/Framework/interface/Event.h"
//#include "FWCore/Framework/interface/MakerMacros.h"
//
//#include "FWCore/ParameterSet/interface/ParameterSet.h"
//
//// necessary objects:
//#include "FWCore/Framework/interface/ESHandle.h"
//#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
//#include "DataFormats/JetReco/interface/CaloJetCollection.h"
//#include "DataFormats/JetReco/interface/CaloJet.h"
////#include "DataFormats/EgammaCandidates/interface/Electron.h"
//#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
//#include "DataFormats/EgammaReco/interface/ElectronPixelSeed.h"
//#include "DataFormats/TrackReco/interface/Track.h"
//#include "DataFormats/METObjects/interface/MET.h"
//#include <DataFormats/METReco/interface/CaloMET.h>
//#include <DataFormats/METReco/interface/GenMET.h>
//#include <DataFormats/CaloTowers/interface/CaloTower.h>
//#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"
//#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
//#include <DataFormats/EgammaReco/interface/BasicCluster.h>

#include "HLTrigger/HLTcore/interface/HLTFilter.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//#include <iostream>


class hlt_optimizer : public edm::EDAnalyzer
{
   public:
      explicit hlt_optimizer(const edm::ParameterSet&);
      ~hlt_optimizer();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
 
};

#endif
