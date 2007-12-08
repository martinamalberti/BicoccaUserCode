#ifndef ditausAnalysisMC_H
#define ditausAnalysisMC_H


#include <memory>

// user include files
#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

// necessary objects:
#include "FWCore/Framework/interface/ESHandle.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaReco/interface/ElectronPixelSeed.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/METObjects/interface/MET.h"
#include <DataFormats/METReco/interface/CaloMET.h>
#include <DataFormats/METReco/interface/GenMET.h>
#include <DataFormats/CaloTowers/interface/CaloTower.h>
#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include <DataFormats/EgammaReco/interface/BasicCluster.h>


#include <iostream>

// some root includes
#include <TFile.h>
#include <TH1D.h>
#include <TTree.h>


class ditausAnalysisMC : public edm::EDAnalyzer {
   public:
      explicit ditausAnalysisMC(const edm::ParameterSet&);
      ~ditausAnalysisMC();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
  int nEvt;// used to count the number of events

  double twopi;

  double MZ;

  // to be used for root output tree
  TFile *thefile;
  TTree *smalltree;
  std::string m_nomefile;

  int nMCPar;
  int MCPARMAX;// used to set maximum of arrays
  double MCe1,MCe2,ENue,ENumu,ENutau1,ENutau2;
  double MCp1[3],MCp2[3],PNue[3],PNumu[3],PNutau1[3],PNutau2[3];
  double MCeta1,MCeta2,EtaNue,EtaNumu,EtaNutau1,EtaNutau2;
  double MCphi1,MCphi2,PhiNue,PhiNumu,PhiNutau1,PhiNutau2;
  int MCpid1,MCpid2,PidNue,PidNumu,PidNutau1,PidNutau2;

  int nEleCand;
  int ELEMAX;// used to set maximum of arrays
  double ele_p[10][3];
  double ele_eSC[10];
  double ele_etSC[10];
  double ele_etaSC[10];
  double ele_UncorrEtaSC[10];
  double ele_etaTrk[10];
  double ele_phiSC[10];
  double ele_phiTrk[10];
  double ele_TrkIso02[10];  
  double ele_TrkIso035[10];  
  double ele_TrkIso05[10];  
  int ele_charge[10];

  int nMuonCand;
  int MUONMAX;// used to set maximum of arrays
  double muon_p[10][3];
  double muon_eta[10];
  double muon_phi[10];
  int muon_charge[10];

  int nJetCand;
  int JETMAX;// used to set maximum of arrays
  double JetCand_e[30];
  double JetCand_p[30][3];
  double JetCand_pt[30];
  double JetCand_eta[30];
  double JetCand_phi[30];

  double met;
  double mex;
  double mey;
  double met_phi;
  double Set;

  double GenMet;
  double GenMex;
  double GenMey;
  double GenMet_phi;
  double GenSet;

  double EoP, dEtaTrkClus, dPhiTrkClus, HoE;
  double deta, dphi, PTr02, PTr035, PTr05, R;
  double TrkIsolation02,TrkIsolation035,TrkIsolation05;
  
  double p1,p2,cphi,mass,metz,u;

//   // booking  histograms
//   TH1F *zmass;
//   TH1F *hEoP;
//   TH1F *hdEta;
//   TH1F *hdPhi;
//   //TH1F *hHoE;

//   TH1F *hETeW;
//   TH1F *hETeZ;
//   TH1F *hETeW_cut;
//   TH1F *hETeZ_cut;
//   TH1F *hETeBkg;
//   TH1F *hETeBkg_cut;

 
};

#endif
