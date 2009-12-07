#ifndef RecoHistograms_h
#define RecoHistograms_h

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



class RecoHistograms
{
 public:
  
  //! ctor
  RecoHistograms(const int& step);
  
  //! dtor
  ~RecoHistograms();
  
  //! method to be called at each event to fill histograms
  void FillRecoHistograms(const VBFNtupleContent& treeVars,
                          const std::vector<int>* recoJet_tag_it = 0,
                          const std::vector<int>* recoJet_W_it = 0,
			  const std::vector<int>* recoL_W_it = 0);
  
  void WriteRecoHistograms(TFile* outputRootFile);
  
  
  
 private:
  
  int m_step;
  
  
  
  // reco met
  TH1F* recoCaloMet_pt;
  TH1F* recoMuonCorrectedCaloMet_pt;
  
  
  
  // tag jets
  TH1F* recoJ_tag_pt;
  TH1F* recoJ_tag_pt1;
  TH1F* recoJ_tag_pt2;  
  TH1F* recoJ_tag_mass;
  TH1F* recoJ_tag_eta;
  TH1F* recoJ_tag_absEta;
  
  TH1F* recoJJ_tag_pt;
  TH1F* recoJJ_tag_mass;
  TH1F* recoJJ_tag_eta;
  TH1F* recoJJ_tag_absEta;
  TH1F* recoJJ_tag_Dphi;
  TH1F* recoJJ_tag_Deta;
  TH1F* recoJJ_tag_DR;
  TH1F* recoJJ_tag_angle;
  TH1F* recoJJ_tag_etaProd;
  
  
  
  // jets from W2
  TH1F* recoJ_W_pt;
  TH1F* recoJ_W_mass;
  TH1F* recoJ_W_eta;
  TH1F* recoJ_W_absEta;
  TH1F* recoJ_W_zepp;
  
  TH1F* recoJJ_W_pt;
  TH1F* recoJJ_W_mass;
  TH1F* recoJJ_W_eta;
  TH1F* recoJJ_W_absEta;
  TH1F* recoJJ_W_Dphi;
  TH1F* recoJJ_W_Deta;
  TH1F* recoJJ_W_DR;
  TH1F* recoJJ_W_angle;
  TH1F* recoJJ_W_etRatio;
  TH1F* recoJJ_W_zepp;
  
  
  
  // central jets
  TH1F* recoJ_central_n;
  
  
  
  // lepton from W1
  TH1F* recoL_W_pt;
  TH1F* recoL_W_mass;
  TH1F* recoL_W_eta;
  TH1F* recoL_W_absEta;
  TH1F* recoL_W_zepp;
  TH1F* recoL_W_tkIsoOverPt;
  TH1F* recoL_W_emIsoOverPt;
  TH1F* recoL_W_hadIsoOverPt;
  
  
  // reco higgs
  TH1F* recoLJJ_pt;
  TH1F* recoLJJ_mass;
  TH1F* recoLJJ_eta;
  TH1F* recoLJJ_absEta;
  TH1F* recoLJJ_Dphi;
  TH1F* recoLJJ_Deta;
  TH1F* recoLJJ_DR;
  TH1F* recoLJJ_angle;
  TH1F* recoLJJ_zepp;
};

#endif
