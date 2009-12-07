#ifndef MCHistograms_h
#define MCHistograms_h

#include "HiggsAnalysis/VBFHiggsToWWTolnujj/interface/VBFNtupleContent.h"
#include "HiggsAnalysis/VBFHiggsToVV/interface/VBFUtils.h"

#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#include "TVector3.h"
#include "TH1F.h"
#include "TLorentzVector.h"
#include "TFile.h"
#include "TDirectory.h"



class MCHistograms
{
 public:
  
  //! ctor
  MCHistograms(const int& step);
  
  //! dtor
  ~MCHistograms();
  
  //! method to be called at each event to fill histograms
  void FillMCHistograms(const VBFNtupleContent& treeVars);
  
  void WriteMCHistograms(TFile* outputRootFile);
  
  
  
 private:
  
  int m_step;
  
  
  
  // tag quarks
  TH1F* mcQ_tag_pt;
  TH1F* mcQ_tag_mass;
  TH1F* mcQ_tag_eta;
  TH1F* mcQ_tag_absEta;
  
  TH1F* mcQQ_tag_pt;
  TH1F* mcQQ_tag_mass;
  TH1F* mcQQ_tag_eta;
  TH1F* mcQQ_tag_absEta;
  TH1F* mcQQ_tag_Dphi;
  TH1F* mcQQ_tag_Deta;
  TH1F* mcQQ_tag_DR;
  TH1F* mcQQ_tag_angle;
  TH1F* mcQQ_tag_etaProd;
  
  
  
  // quarks from W2
  TH1F* mcQ_W_pt;
  TH1F* mcQ_W_mass;
  TH1F* mcQ_W_eta;
  TH1F* mcQ_W_absEta;
  TH1F* mcQ_W_zepp;
  TH1F* mcQ_W_pdgId;
  
  TH1F* mcQQ_W_pt;
  TH1F* mcQQ_W_mass;
  TH1F* mcQQ_W_eta;
  TH1F* mcQQ_W_absEta;
  TH1F* mcQQ_W_Dphi;
  TH1F* mcQQ_W_Deta;
  TH1F* mcQQ_W_DR;
  TH1F* mcQQ_W_angle;
  TH1F* mcQQ_W_zepp;
  
  
  
  // leptons from W1
  TH1F* mcL_W_pt;
  TH1F* mcL_W_mass;
  TH1F* mcL_W_eta;
  TH1F* mcL_W_absEta;
  TH1F* mcL_W_zepp;
  TH1F* mcL_W_pdgId;
  
  TH1F* mcNu_W_pt;
  TH1F* mcNu_W_mass;
  TH1F* mcNu_W_eta;
  TH1F* mcNu_W_absEta;
  TH1F* mcNu_W_zepp;
  TH1F* mcNu_W_pdgId;
  
  TH1F* mcLNu_W_pt;
  TH1F* mcLNu_W_mass;
  TH1F* mcLNu_W_eta;
  TH1F* mcLNu_W_absEta;
  TH1F* mcLNu_W_Dphi;
  TH1F* mcLNu_W_Deta;
  TH1F* mcLNu_W_DR;
  TH1F* mcLNu_W_angle;
  TH1F* mcLNu_W_zepp;
  
  
  
  // higgs
  TH1F* mcLQQ_pt;
  TH1F* mcLQQ_mass;
  TH1F* mcLQQ_eta;
  TH1F* mcLQQ_absEta;
  TH1F* mcLQQ_Dphi;
  TH1F* mcLQQ_Deta;
  TH1F* mcLQQ_DR;
  TH1F* mcLQQ_angle;
  TH1F* mcLQQ_zepp;
  
  TH1F* mcLNuQQ_pt;
  TH1F* mcLNuQQ_mass;
  TH1F* mcLNuQQ_eta;
  TH1F* mcLNuQQ_absEta;
  TH1F* mcLNuQQ_Dphi;
  TH1F* mcLNuQQ_Deta;
  TH1F* mcLNuQQ_DR;
  TH1F* mcLNuQQ_angle;
  TH1F* mcLNuQQ_zepp;

};

#endif
