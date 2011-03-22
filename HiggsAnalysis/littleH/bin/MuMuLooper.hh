#ifndef MuMuLooper_h
#define MuMuLooper_h

#include <TVector3.h>
#include <TH1F.h>
#include <TH2F.h>

#include "smallHBaseClass.h"
#include <vector>

class MuMuLooper : public smallHBaseClass{
public:
  
  MuMuLooper(TChain *tree=0);
  virtual ~MuMuLooper() {};
  void bookHistos();
  void saveHistos(TFile *f1);
  void Loop(string filename, string flag);
  int theBestQQ() const;
  bool accept_glb_mu(const int mu_index) const;
  bool accept_trk_mu(const int mu_index) const;

private:

  int MIN_nhits_trk;
  float MAX_normchi2_trk;
  float MAX_normchi2_glb;
  int MIN_nhits_pixel;
  float MAX_d0_trk;
  float MAX_dz_trk;
  float MIN_vtxprob;
  float MAX_S3Dip;
  float MAX_muisol;
  float MIN_muP;
  float MIN_pt;

  float weight;
  bool onlyTheBest;

  Float_t invMass;

  TTree *tInvMass;              
  TH1F *hInvMass;

  TH1F * hQQProbChi2;
  TH1F *hQQS3Dip; 
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

