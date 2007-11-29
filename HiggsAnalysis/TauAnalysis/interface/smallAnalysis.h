#ifndef smallAnalysis_H
#define smallAnalysis_H


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


class smallAnalysis : public edm::EDAnalyzer {
   public:
      explicit smallAnalysis(const edm::ParameterSet&);
      ~smallAnalysis();


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

  int nMCPar;
  int MCPARMAX;// used to set maximum of arrays
  double MCpV[3];
  double MCp1[3];                            //1=e
  double MCp2[3];                            //2=mu
  double MCeta1, MCeta2, MCetaV;
  double MCphi1, MCphi2, MCphiV;
  int pid1,pid2,pidV;

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
  int muon_charge[10];

  int nJetCand;
  int JETMAX;// used to set maximum of arrays
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
