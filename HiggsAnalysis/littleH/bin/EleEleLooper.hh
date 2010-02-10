#ifndef EleEleLooper_h
#define EleEleLooper_h

#include <TVector3.h>
#include <TH1F.h>
#include <TH2F.h>


#include "smallHBaseClass.h"
#include <vector>
#include "hChain.h"

class EleEleLooper : public smallHBaseClass{
public:
  
  EleEleLooper(TChain *tree=0);
  virtual ~EleEleLooper() {};
  void bookHistos();
  void saveHistos(TFile *f1);
  void Loop();
  int theBestQQ() const;
  bool accept_ele(const int ele_index) const;
  void GetMcInfo() const;
  
private:

  float MIN_vtxprob_jpsi;
 
  bool onlyTheBest;
  
  hChain *hC_ElePt;            
  hChain *hC_InvMass;            
  
};
#endif

