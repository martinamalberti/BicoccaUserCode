#ifndef VBFFirstFilterTreeProducer_H
#define VBFFirstFilterTreeProducer_H

/** \class VBFFirstFilterTreeProducer
 *
 * analyzer to produce selection tree for First Filter VBF H->WW->2l2nu analysis
 */  

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/Common/interface/EDProduct.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DataFormats/Common/interface/Handle.h" 
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"


#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"


#include <TFile.h>
#include <TTree.h>

#include <CLHEP/Vector/LorentzVector.h>

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>



class TFile;
class TTree;

class VBFFirstFilterTreeProducer : public edm::EDAnalyzer 
{
 public:
  
  VBFFirstFilterTreeProducer(const edm::ParameterSet& conf);
  
  ~VBFFirstFilterTreeProducer();
  
  void analyze(const edm::Event& e, const edm::EventSetup& c);
  void beginJob(const edm::EventSetup &iSetup);
  void endJob();
  
  void DefineBranches();
  
  
  
 private:
       
  edm::InputTag srcElectrons_;
  edm::InputTag srcMuons_;
  edm::InputTag srcJets_;
  
  edm::InputTag eleTkIso_;
  edm::InputTag eleEcalIso_;
  edm::InputTag eleHcalIso_;

  
    
  bool verbosity_;       
  bool verbosity_mc_;    

   
  unsigned int nAnalyzed_;

  TTree * outTree_;

   //-------------------
  //---- MonteCarlo ----
  //--------------------
  
  int flagVBFH_;
  int flagVBFHWW_;
   
    
  //---- mc Muon ----
  int mcMu_n_;
  std::vector<double> * mcMu_px_;
  std::vector<double> * mcMu_py_;
  std::vector<double> * mcMu_pz_;
  std::vector<double> * mcMu_e_;
  std::vector<double> * mcMu_eta_;
  std::vector<double> * mcMu_phi_;
  std::vector<double> * mcMu_pT_;
  std::vector<int> * mcMu_charge_;
 
   
  
  //---- mc Electron ----
  int mcEle_n_;
  std::vector<double> * mcEle_px_;
  std::vector<double> * mcEle_py_;
  std::vector<double> * mcEle_pz_;
  std::vector<double> * mcEle_e_;
  std::vector<double> * mcEle_eta_;
  std::vector<double> * mcEle_phi_;
  std::vector<double> * mcEle_pT_;
  std::vector<int> * mcEle_charge_;
 
     
 
  
  
  
  //--------------
  //---- RECO ----
  //--------------
  
  //---- reco Muon ----
  int recoMu_n_;
  std::vector<double> * recoMu_px_;
  std::vector<double> * recoMu_py_;
  std::vector<double> * recoMu_pz_;
  std::vector<double> * recoMu_e_;
  std::vector<double> * recoMu_eta_;
  std::vector<double> * recoMu_phi_;
  std::vector<double> * recoMu_pT_;
  std::vector<int> * recoMu_charge_;
 
  std::vector<double> * recoMu_isolationR03_sumPt;
  std::vector<double> * recoMu_isolationR03_emEt;
  std::vector<double> * recoMu_isolationR03_hadEt;  
 
  std::vector<double> * recoMu_isolationR05_sumPt;
  std::vector<double> * recoMu_isolationR05_emEt;
  std::vector<double> * recoMu_isolationR05_hadEt;  
  
  
  //---- reco Electron ----
  int recoEle_n_;
  std::vector<double> * recoEle_px_;
  std::vector<double> * recoEle_py_;
  std::vector<double> * recoEle_pz_;
  std::vector<double> * recoEle_e_;
  std::vector<double> * recoEle_eta_;
  std::vector<double> * recoEle_phi_;
  std::vector<double> * recoEle_pT_;
  std::vector<int> * recoEle_charge_;
 
  std::vector<double> * recoEle_IsoTk_;
  std::vector<double> * recoEle_IsoEcal_;
  std::vector<double> * recoEle_IsoHcal_;
  
  std::vector<double> * recoEle_eId_;
  std::vector<double> * recoEle_eIdLikelihood_;
 
 
  //---- reco Jet ----
  int recoJet_n_;
  std::vector<double> * recoJet_px_;
  std::vector<double> * recoJet_py_;
  std::vector<double> * recoJet_pz_;
  std::vector<double> * recoJet_e_;
  std::vector<double> * recoJet_eta_;
  std::vector<double> * recoJet_phi_;
  std::vector<double> * recoJet_pT_;
  std::vector<double> * recoJet_et_;
  


};
  
#endif
 


