#include "PhysicsTools/NtupleUtils/interface/treeReader.h"
#include "PhysicsTools/NtupleUtils/interface/ntpleUtils.h"

#include "TTree.h"



struct WZAnalysisVariablesSingleXtal
{
  // tree definition
  TFile* m_outputRootFile;
  TTree* m_reducedTree;
  

  // event variables
  int runId;
  int lumiId;
  
  int isW;
  int isZ;
  
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

  std::vector<float> ele1_recHit_E;
  std::vector<int> ele1_recHit_hashedIndex;
    
  float ele1_scERaw;
  float ele1_scE;
  float ele1_es;
  
  float ele1_e3x3;
    
  float ele1_tkP;
  
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
  
  std::vector<float> ele2_recHit_E;
  std::vector<int> ele2_recHit_hashedIndex;
    
  float ele2_scERaw;
  float ele2_scE;
  float ele2_es;
  
  float ele2_e3x3;
    
  float ele2_tkP;
  
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



TFile* GetOutputRootFile(WZAnalysisVariablesSingleXtal& vars);

void InitializeWZAnalysisTree(WZAnalysisVariablesSingleXtal& vars, const std::string& outputRootFileName);
void FillWZAnalysisTree(WZAnalysisVariablesSingleXtal& vars);

void ClearWZAnalysisVariables(WZAnalysisVariablesSingleXtal&);
void DeleteWZAnalysisVariables(WZAnalysisVariablesSingleXtal&);

void SetElectron1Variables(WZAnalysisVariablesSingleXtal& vars, treeReader& reader, const int& ele1It);
void SetElectron2Variables(WZAnalysisVariablesSingleXtal& vars, treeReader& reader, const int& ele2It);
void SetMetVariables(WZAnalysisVariablesSingleXtal& vars, treeReader& reader);
void SetDiElectronVariables(WZAnalysisVariablesSingleXtal& vars, treeReader& reader);
