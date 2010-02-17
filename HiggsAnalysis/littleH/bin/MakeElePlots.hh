#ifndef MakeElePlots_h
#define MakeElePlots_h

#include <TVector3.h>
#include <TH1F.h>
#include <TH2F.h>


#include "smallHBaseClass.h"
#include <vector>
#include "hChain.h"
#include "h2Chain.h"

class MakeElePlots : public smallHBaseClass{
public:
  
  MakeElePlots(TChain *tree=0);
  virtual ~MakeElePlots() {};
  void bookHistos();
  void saveHistos(TFile *f1);
  void Loop(int entries);
  int theBestQQ() const;
  bool accept_ele(const int ele_index) const;
  void GetMcInfo() const;
  
private:

  float MIN_vtxprob_jpsi;
 
  bool onlyTheBest;
  
  hChain * hC_EleLowPt ;
  hChain * hC_EleHighPt;
  hChain * hC_ElePt    ;
  hChain * hC_EleIsoTk ;
  hChain * hC_EleIsoEm ;
  hChain * hC_EleIsoHad;
  hChain * hC_EleId;

  hChain * hC_InvMass  ;

  hChain * hC_QQprobChi2;
  hChain * hC_QQS3Dip   ;
  hChain * hC_QQSlxy   ;
  hChain * hC_QQSTip ;
  
  h2Chain * h2C_EleVsElePt;
  
};
#endif

