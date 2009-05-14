#ifndef VBFMCH4jAnalysis_h
#define VBFMCH4jAnalysis_h

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
#include "TH2F.h"
#include "TProfile.h"
#include "TDirectory.h"

#include "CLHEP/Units/PhysicalConstants.h"
#include "CLHEP/Vector/ThreeVector.h"



class VBFMCH4jAnalysis : public edm::EDAnalyzer
{
 public:
  
  VBFMCH4jAnalysis(const edm::ParameterSet& iConfig);
  ~VBFMCH4jAnalysis();
  
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
  
  TH2F* mcV1V2_eta;
  
  TH2F* mcVPTFDeta;
  
  
};

#endif
