#include "PhysicsTools/NtupleUtils/interface/treeReader.h"
#include "PhysicsTools/NtupleUtils/interface/ntpleUtils.h"

#include "TTree.h"



struct WZAnalysisVariables
{
  // tree definition
  TFile* m_outputRootFile;
  TTree* m_reducedTree;
  

  // event variables
  int dataFlag;
  int totEvents;
  float crossSection;
  int runId;
  int lumiId;
  int eventId;
  int eventNaiveId;
  int timeStampLow;
  int timeStampHigh;
  
  int isW;
  int isZ;
  
  
  // PV variables
  int PV_n;
  float PV_z;
  float PV_d0;
  float PV_SumPt;
  
  
  // 1st electron variables
  ROOT::Math::XYZTVector ele1;
  ROOT::Math::XYZTVector* p_ele1;
  float ele1_charge;
  float ele1_p;
  float ele1_pt;
  float ele1_eta;
  float ele1_phi;
  
  float ele1_sigmaIetaIeta;
  float ele1_DphiIn;
  float ele1_DetaIn;
  float ele1_HOverE;
  float ele1_tkIso;
  float ele1_emIso;
  float ele1_hadIso;
  
  float ele1_scERaw;
  float ele1_scEtRaw;
  float ele1_scE;
  float ele1_scEt;
  float ele1_scEta;
  float ele1_scPhi;
  float ele1_scEtaWidth;
  float ele1_scPhiWidth;
  float ele1_scLaserCorr;
  float ele1_scCrackCorr;
  float ele1_es;
  
  float ele1_e3x3;
  
  float ele1_seedE;
  int ele1_seedIeta;
  int ele1_seedIphi;
  int ele1_seedIx;
  int ele1_seedIy;
  int ele1_seedZside;
  float ele1_seedLaserAlpha;
  float ele1_seedLaserCorr;
  
  float ele1_tkP;
  float ele1_tkPt;
  
  float ele1_fbrem;
  float ele1_EOverP;
  
  int ele1_isEB;
  int ele1_isEBEEGap;
  int ele1_isEBEtaGap;
  int ele1_isEBPhiGap;
  int ele1_isEEDeeGap;
  int ele1_isEERingGap;
  
  
  // 2nd electron variables
  ROOT::Math::XYZTVector ele2;
  ROOT::Math::XYZTVector* p_ele2;
  float ele2_charge;
  float ele2_p;
  float ele2_pt;
  float ele2_eta;
  float ele2_phi;
  
  float ele2_sigmaIetaIeta;
  float ele2_DphiIn;
  float ele2_DetaIn;
  float ele2_HOverE;
  float ele2_tkIso;
  float ele2_emIso;
  float ele2_hadIso;
  
  float ele2_scERaw;
  float ele2_scEtRaw;
  float ele2_scE;
  float ele2_scEt;
  float ele2_scEta;
  float ele2_scPhi;
  float ele2_scEtaWidth;
  float ele2_scPhiWidth;
  float ele2_scLaserCorr;
  float ele2_scCrackCorr;
  float ele2_es;
  
  float ele2_e3x3;
  
  float ele2_seedE;
  int ele2_seedIeta;
  int ele2_seedIphi;
  int ele2_seedIx;
  int ele2_seedIy;
  int ele2_seedZside;
  float ele2_seedLaserAlpha;
  float ele2_seedLaserCorr;
  
  float ele2_tkP;
  float ele2_tkPt;
  
  float ele2_fbrem;
  float ele2_EOverP;
  
  int ele2_isEB;
  int ele2_isEBEEGap;
  int ele2_isEBEtaGap;
  int ele2_isEBPhiGap;
  int ele2_isEEDeeGap;
  int ele2_isEERingGap;
  
  
  // met variables
  ROOT::Math::XYZTVector met;
  ROOT::Math::XYZTVector* p_met;
  float met_et;
  float met_phi;

  float ele1Met_mt;
  float ele1Met_Dphi;
  
  
  // di-electron variables
  float ele1ele2_m;
  float ele1ele2_scM;
};



TFile* GetOutputRootFile(WZAnalysisVariables& vars);

void InitializeWZAnalysisTree(WZAnalysisVariables& vars, const std::string& outputRootFileName);
void FillWZAnalysisTree(WZAnalysisVariables& vars);

void ClearWZAnalysisVariables(WZAnalysisVariables&);
void DeleteWZAnalysisVariables(WZAnalysisVariables&);

void SetPVVariables(WZAnalysisVariables& vars, treeReader& reader);
void SetElectron1Variables(WZAnalysisVariables& vars, treeReader& reader, const int& ele1It);
void SetElectron2Variables(WZAnalysisVariables& vars, treeReader& reader, const int& ele2It);
void SetMetVariables(WZAnalysisVariables& vars, treeReader& reader);
void SetDiElectronVariables(WZAnalysisVariables& vars, treeReader& reader);
