#ifndef VBFMCDumper_h
#define VBFMCDumper_h

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/MCAnalysis.h"
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/MCUtils.h"
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFUtils.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventPrincipal.h" 
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

#include <iostream>
#include <iomanip>

#include "TFile.h"
#include "TH1F.h"
#include "TProfile.h"
#include "TDirectory.h"

#include "CLHEP/Units/PhysicalConstants.h"
#include "CLHEP/Vector/ThreeVector.h"



class VBFMCDumper : public edm::EDAnalyzer
{
 public:
  
  VBFMCDumper(const edm::ParameterSet& iConfig);
  ~VBFMCDumper();
  
  virtual void beginJob(const edm::EventSetup&);
  virtual void endJob();
  
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  
  
 private:
  
  edm::InputTag srcHepMCProduct_p;
  edm::InputTag srcGenParticles_p;
  edm::InputTag srcGenMet_p;
  edm::InputTag srcIC5GenJets_p;
  edm::InputTag srcIC5CaloJets_p;
    
  std::string fileName_p;
  std::string mass_p;
  bool verbosity_p;
  int eventsToPrint_p;

  int eventId_p;
  int naiveId_p;

  
  TFile* outFile_p;
  
  
  // Define histograms
  TH1F* mcHDecayId;
  TH1F* mcV1DecayId;
  TH1F* mcV2DecayId;
  
  
  // higgs histograms
  TH1F* mcH_charge;
  TH1F* mcH_mass;
  TH1F* mcH_energy;
  TH1F* mcH_p;
  TH1F* mcH_pT;
  TH1F* mcH_pL;
  TH1F* mcH_phi;
  TH1F* mcH_eta;

  TH1F* mcZZ_charge;
  TH1F* mcZZ_mass;
  TH1F* mcZZ_energy;
  TH1F* mcZZ_p;
  TH1F* mcZZ_pT;
  TH1F* mcZZ_pL;
  TH1F* mcZZ_phi;
  TH1F* mcZZ_eta;

  TH1F* mc4l_charge;
  TH1F* mc4l_mass;
  TH1F* mc4l_energy;
  TH1F* mc4l_p;
  TH1F* mc4l_pT;
  TH1F* mc4l_pL;
  TH1F* mc4l_phi;
  TH1F* mc4l_eta;


  // vector boson histograms
  TH1F* mcV1_charge;
  TH1F* mcV1_mass;
  TH1F* mcV1_energy;
  TH1F* mcV1_p;
  TH1F* mcV1_pT;
  TH1F* mcV1_pL;
  TH1F* mcV1_phi;
  TH1F* mcV1_eta;

  TH1F* mcFF_fromV1_charge;
  TH1F* mcFF_fromV1_mass;
  TH1F* mcFF_fromV1_energy;
  TH1F* mcFF_fromV1_p;
  TH1F* mcFF_fromV1_pT;
  TH1F* mcFF_fromV1_pL;
  TH1F* mcFF_fromV1_phi;
  TH1F* mcFF_fromV1_eta;

  TH1F* mcV2_charge;
  TH1F* mcV2_mass;
  TH1F* mcV2_energy;
  TH1F* mcV2_p;
  TH1F* mcV2_pT;
  TH1F* mcV2_pL;
  TH1F* mcV2_phi;
  TH1F* mcV2_eta;
  
  TH1F* mcFF_fromV2_charge;
  TH1F* mcFF_fromV2_mass;
  TH1F* mcFF_fromV2_energy;
  TH1F* mcFF_fromV2_p;
  TH1F* mcFF_fromV2_pT;
  TH1F* mcFF_fromV2_pL;
  TH1F* mcFF_fromV2_phi;
  TH1F* mcFF_fromV2_eta;

  TH1F* mcV_deltaPhi;
  TH1F* mcV_deltaEta;

  TH1F* mcFF_deltaPhi;
  TH1F* mcFF_deltaEta;


  // fermion histograms
  TH1F* mcF_fromV1_charge;
  TH1F* mcF_fromV1_mass;
  TH1F* mcF_fromV1_energy;
  TH1F* mcF_fromV1_p;
  TH1F* mcF_fromV1_pT;
  TH1F* mcF_fromV1_pL;
  TH1F* mcF_fromV1_phi;
  TH1F* mcF_fromV1_eta;

  TH1F* mcF_fromV1_deltaPhi;  
  TH1F* mcF_fromV1_deltaEta;

  TH1F* mcF_fromV2_charge;
  TH1F* mcF_fromV2_mass;
  TH1F* mcF_fromV2_energy;
  TH1F* mcF_fromV2_p;
  TH1F* mcF_fromV2_pT;
  TH1F* mcF_fromV2_pL;
  TH1F* mcF_fromV2_phi;
  TH1F* mcF_fromV2_eta;
  
  TH1F* mcF_fromV2_deltaPhi;
  TH1F* mcF_fromV2_deltaEta;


  // iC5GenJets
  TH1F* iC5GenJets_n;
  TH1F* iC5GenJets_charge;  
  TH1F* iC5GenJets_mass;  
  TH1F* iC5GenJets_energy;  
  TH1F* iC5GenJets_p;  
  TH1F* iC5GenJets_pT;  
  TH1F* iC5GenJets_pL;  
  TH1F* iC5GenJets_phi;  
  TH1F* iC5GenJets_eta;  

  TH1F* iC5GenJets_mcF_fromV2_DR;

  TProfile* iC5GenJets_mcF_fromV2_matchEff;
  TProfile* iC5GenJets_mcF_fromV2_matchEff_vsPt;
  TProfile* iC5GenJets_mcF_fromV2_matchEff_vsEta;


  // matched genJets histograms
  TH1F* mcJJ_fromV2_charge;
  TH1F* mcJJ_fromV2_mass;
  TH1F* mcJJ_fromV2_energy;
  TH1F* mcJJ_fromV2_p;
  TH1F* mcJJ_fromV2_pT;
  TH1F* mcJJ_fromV2_pL;
  TH1F* mcJJ_fromV2_phi;
  TH1F* mcJJ_fromV2_eta;

  TH1F* mcJ_fromV2_charge;
  TH1F* mcJ_fromV2_mass;
  TH1F* mcJ_fromV2_energy;
  TH1F* mcJ_fromV2_p;
  TH1F* mcJ_fromV2_pT;
  TH1F* mcJ_fromV2_pL;
  TH1F* mcJ_fromV2_phi;
  TH1F* mcJ_fromV2_eta;


  // iC5CaloJets
  TH1F* iC5CaloJets_n;
  TH1F* iC5CaloJets_charge;  
  TH1F* iC5CaloJets_mass;  
  TH1F* iC5CaloJets_energy;  
  TH1F* iC5CaloJets_p;  
  TH1F* iC5CaloJets_pT;  
  TH1F* iC5CaloJets_pL;  
  TH1F* iC5CaloJets_phi;  
  TH1F* iC5CaloJets_eta;  

  // genMet
  TH1F* genMet_pT;

};

#endif
