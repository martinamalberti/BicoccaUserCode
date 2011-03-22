#ifndef MuMuCutOptimizer_h
#define MuMuCutOptimizer_h

#include <TVector3.h>
#include <TH1F.h>

#include "smallHBaseClass.h"
#include <vector>

class MuMuCutOptimizer : public smallHBaseClass{
public:
  
  MuMuCutOptimizer(TChain *tree=0);
  virtual ~MuMuCutOptimizer() {};
  void bookHistos();
  void Loop(string filename, TString flag, TString optVar);
  bool passCuts(int iqq, float _cut, TString optVar) const;
  bool accept_glb_mu(const int mu_index, float cut, TString optVar) const;
  bool accept_trk_mu(const int mu_index, float cut, TString optVar) const;
  void fillGlbMuonHistos(const int mu_index, float weight) const;
  void fillTrkMuonHistos(const int mu_index, float weight) const;
  void saveHistos(TFile* f1);

private:

  float MAX_normchi2_glb;
  float MAX_S3Dip;
  float MIN_vtxprob;
  float MAX_muisol;
  float MIN_muP;
  float minPt;

  float cut[40];
  int n_step;
  int n_signal;

  float weight;
  TH1F* hInvMass[40];

  TH1F* hInvMass_all;

  TH1F *hIsoVar03_glb_TKECAL;
  TH1F *hIsoVar03_glb_TKECALHCAL;

  TH1F *hIsoVar03_glb_TKECAL_pt;
  TH1F *hIsoVar03_glb_TKECALHCAL_pt;

  TH1F *hIsoVar03_glb_TK;
  TH1F *hIsoVar03_glb_ECAL;
  TH1F *hIsoVar03_glb_HCAL;

  TH1F *hIsoVar03_trk_TKECAL;
  TH1F *hIsoVar03_trk_TKECALHCAL;
  TH1F *hIsoVar03_trk_TKECAL_pt;
  TH1F *hIsoVar03_trk_TKECALHCAL_pt;

  TH1F *hIsoVar03_trk_TK;
  TH1F *hIsoVar03_trk_ECAL;
  TH1F *hIsoVar03_trk_HCAL;

  TH1F * hQQProbChi2;
  TH1F *hQQS3Dip; 
  TH1F *hQQSTip ;
  TH1F *hQQEta   ;
  TH1F *hQQPt    ;
  TH1F *hMuEtaTk   ;
  TH1F * hMuPtTk    ;
  TH1F *hMuEtaGlb   ;
  TH1F * hMuPtGlb    ;
  TH1F * hMuPGlb    ;
  TH1F * hMuPTk    ;

};
#endif

