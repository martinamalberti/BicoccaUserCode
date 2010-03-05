#ifndef EleEleLooper_TMVA_h
#define EleEleLooper_TMVA_h

#include <TVector3.h>
#include <TH1F.h>
#include <TH2F.h>

#include "smallHBaseClass.h"
#include <vector>
#include "hChain.h"

#include "TMVA/Tools.h"
#include "TMVA/Reader.h"

class EleEleLooper_TMVA : public smallHBaseClass{
public:
  
  EleEleLooper_TMVA(TChain *tree=0);
  virtual ~EleEleLooper_TMVA() {};
  void bookHistos();
  void saveHistos(TFile *f1);
  void Loop(int entries);
  int theBestQQ(const bool select_ele) const;
  bool accept_ele(const int ele_index) const;
  
private:

  bool onlyTheBest;
  
  float m_QQ_DeltaR;
  float m_QQ_probChi2;
  float m_QQ_STip;
  float m_QQ_SLip;
  float m_QQ_S3Dip;
  
  //High Pt ele
  float m_electronHpT_tkIso;
  float m_electronHpT_emIso;
  float m_electronHpT_hadIso;
  float m_electronHpT_IdBDT;
  float m_electronHpT_EoP;
  float m_electronHpT_HoE;
  float m_electronHpT_dEtaSc;
  float m_electronHpT_dPhiSc;
  float m_electronHpT_SigiEtaiEtaSc;
  float m_electronHpT_Et;
  float m_electronHpT_pAtVtx;
  float m_electronHpT_ptAtVtx;

  //Low Pt ele
  float m_electronLpT_tkIso;
  float m_electronLpT_emIso;
  float m_electronLpT_hadIso;
  float m_electronLpT_IdBDT;
  float m_electronLpT_EoP;
  float m_electronLpT_HoE;
  float m_electronLpT_dEtaSc;
  float m_electronLpT_dPhiSc;
  float m_electronLpT_SigiEtaiEtaSc;
  float m_electronLpT_Et;
  float m_electronLpT_pAtVtx;
  float m_electronLpT_ptAtVtx;
  
  TMVA::Reader *TMVAreader;
  TString methodName,weightfile;
  float MVA_cut_val;
  
  float WIN_EoP ;

  hChain *hC_InvMass;            
  
};
#endif

