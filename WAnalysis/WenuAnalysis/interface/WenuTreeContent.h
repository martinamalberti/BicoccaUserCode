#ifndef WenuTreeContent_h
#define WenuTreeContent_h

#include "TChain.h" 

#define MAXELECTRONS 10
#define MAXMUONS 10
#define MAXJETS 100
#define MAXTECHL1BITS 64
#define MAXALGOL1BITS 128


struct WenuTreeContent
{
  // Flags
  static bool electronVariables;
  static bool metVariables;
  static bool jetVariables;
  static bool muonVariables;
  static bool HLTrigVariables;
  static bool L1TrigVariables;
  
  unsigned int BX;
  unsigned int lumiId;
  unsigned int runId;
  unsigned int eventId;
  unsigned int eventNaiveId;
 

  // electron variables
  int nElectrons;
  float elePx[MAXELECTRONS];
  float elePy[MAXELECTRONS];
  float elePz[MAXELECTRONS];
  float eleE[MAXELECTRONS];
  float eleEt[MAXELECTRONS];
  float eleEta[MAXELECTRONS];
  float elePhi[MAXELECTRONS];
  float eleId[MAXELECTRONS];
  float eleSigmaIEtaIEta[MAXELECTRONS];
  float eleE1x5[MAXELECTRONS];
  float eleE2x5[MAXELECTRONS];
  float eleE5x5[MAXELECTRONS];
  float eleSeedSwissCross[MAXELECTRONS];
  int   eleCharge[MAXELECTRONS];

  float eleTrkIso[MAXELECTRONS];
  float eleEcalIso[MAXELECTRONS];
  float eleHcalIsoD1[MAXELECTRONS];
  float eleHcalIsoD2[MAXELECTRONS];

  float genelePt[MAXELECTRONS];
  float geneleEta[MAXELECTRONS];
  float genelePhi[MAXELECTRONS];

  float eleSeed[MAXELECTRONS];
  float eleFBrem[MAXELECTRONS];
  float elePin[MAXELECTRONS];
  float elePout[MAXELECTRONS];
  float eleHOverE[MAXELECTRONS];
  float eleDeltaPhiIn[MAXELECTRONS];
  float eleDeltaEtaIn[MAXELECTRONS];
  unsigned int eleMisHits[MAXELECTRONS];

  float eleES[MAXELECTRONS];


  // MET VARIABLES
  float caloMet;
  float caloMex;
  float caloMey;
  float caloMetPhi;
  float caloSumEt;

  float tcMet;
  float tcMex;
  float tcMey;
  float tcMetPhi;
  float tcSumEt;


  float pfMet;
  float pfMex;
  float pfMey;
  float pfMetPhi;
  float pfSumEt; 

  // JET VARIABLES
  int nJets;
  float jetPx[MAXJETS];
  float jetPy[MAXJETS];
  float jetPz[MAXJETS];
  float jetPt[MAXJETS];
  float jetEta[MAXJETS];
  float jetPhi[MAXJETS];
  float jetBdisc[MAXJETS];

  float genjetPt[MAXJETS];
  float genjetEta[MAXJETS];
  float genjetPhi[MAXJETS];


  // MUON VARIABLES
  int nMuons;
  float muonPx[MAXMUONS];
  float muonPy[MAXMUONS];
  float muonPz[MAXMUONS];
  float muonPt[MAXMUONS];
  float muonEta[MAXMUONS];
  float muonPhi[MAXMUONS];
  

  // HLT VARIABLES
  int HLT_Ele15_LW_L1R;
  int HLT_Photon10_L1R;
  int HLT_Photon15_L1R;
  int HLT_Photon20_L1R;

  // L1 trigger variables
  int techL1Bit[MAXTECHL1BITS];
  int algoL1Bit[MAXALGOL1BITS];


};







// ------------------------------------------------------------------------
//! branch addresses settings

void setBranchAddresses(TTree* chain, WenuTreeContent& treeVars);






// ------------------------------------------------------------------------
//! create branches for a tree

void setBranches(TTree* chain, WenuTreeContent& treeVars);






// ------------------------------------------------------------------------
//! initialize branches

void initializeBranches(TTree* chain, WenuTreeContent& treeVars);



#endif
