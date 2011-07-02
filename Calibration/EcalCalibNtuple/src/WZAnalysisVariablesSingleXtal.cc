#include "Calibration/EcalCalibNtuple/interface/WZAnalysisVariablesSingleXtal.h"

TFile* GetOutputRootFile(WZAnalysisVariablesSingleXtal& vars)
{
  return vars.m_outputRootFile;
}



void InitializeWZAnalysisTree(WZAnalysisVariablesSingleXtal& vars, const std::string& outputRootFileName)
{
  //-------------
  // Reduced tree
  //-------------
  
  vars.m_outputRootFile = new TFile(outputRootFileName.c_str(), "RECREATE");  
  
  vars.m_reducedTree = new TTree("ntu", "ntu");
  vars.m_reducedTree -> SetDirectory(vars.m_outputRootFile);
  
  
  // event variables
  vars.m_reducedTree -> Branch("runId",         &vars.runId,                 "runId/I");
  vars.m_reducedTree -> Branch("lumiId",        &vars.lumiId,               "lumiId/I");
  
  vars.m_reducedTree -> Branch("isW", &vars.isW, "isW/I");
  vars.m_reducedTree -> Branch("isZ", &vars.isZ, "isZ/I");
  
  // 1st electron variables
  vars.m_reducedTree -> Branch("ele1_recHit_E",           "std::vector<float>", &vars.ele1_recHit_E);
  vars.m_reducedTree -> Branch("ele1_recHit_hashedIndex", "std::vector<int>",   &vars.ele1_recHit_hashedIndex);
  
  vars.m_reducedTree -> Branch("ele1_scERaw",      &vars.ele1_scERaw,           "ele1_scERaw/F");
  vars.m_reducedTree -> Branch("ele1_scE",         &vars.ele1_scE,                 "ele1_scE/F");
  vars.m_reducedTree -> Branch("ele1_es",          &vars.ele1_es,                   "ele1_es/F");
  
  vars.m_reducedTree -> Branch("ele1_e3x3", &vars.ele1_e3x3, "ele1_e3x3/F");
  
  vars.m_reducedTree -> Branch("ele1_tkP",  &vars.ele1_tkP,   "ele1_tkP/F");
  
  vars.m_reducedTree -> Branch("ele1_fbrem",  &vars.ele1_fbrem,   "ele1_fbrem/F");
  vars.m_reducedTree -> Branch("ele1_EOverP", &vars.ele1_EOverP, "ele1_EOverP/F");
  
  vars.m_reducedTree -> Branch("ele1_isEB",        &vars.ele1_isEB,               "ele1_isEB/I");
  vars.m_reducedTree -> Branch("ele1_isEBEEGap",   &vars.ele1_isEBEEGap,     "ele1_isEBEEGap/I");
  vars.m_reducedTree -> Branch("ele1_isEBEtaGap",  &vars.ele1_isEBEtaGap,   "ele1_isEBEtaGap/I");
  vars.m_reducedTree -> Branch("ele1_isEBPhiGap",  &vars.ele1_isEBPhiGap,   "ele1_isEBPhiGap/I");
  vars.m_reducedTree -> Branch("ele1_isEEDeeGap",  &vars.ele1_isEEDeeGap,   "ele1_isEEDeeGap/I");
  vars.m_reducedTree -> Branch("ele1_isEERingGap", &vars.ele1_isEERingGap, "ele1_isEERingGap/I");
  
  
  // 2nd electron variables  
  vars.m_reducedTree -> Branch("ele2_recHit_E",           "std::vector<float>", &vars.ele2_recHit_E);
  vars.m_reducedTree -> Branch("ele2_recHit_hashedIndex", "std::vector<int>",   &vars.ele2_recHit_hashedIndex);

  vars.m_reducedTree -> Branch("ele2_scERaw",      &vars.ele2_scERaw,           "ele2_scERaw/F");
  vars.m_reducedTree -> Branch("ele2_scE",         &vars.ele2_scE,                 "ele2_scE/F");
  vars.m_reducedTree -> Branch("ele2_es",          &vars.ele2_es,                   "ele2_es/F");
  
  vars.m_reducedTree -> Branch("ele2_e3x3", &vars.ele2_e3x3, "ele2_e3x3/F");
  
  vars.m_reducedTree -> Branch("ele2_tkP",  &vars.ele2_tkP,   "ele2_tkP/F");
  
  vars.m_reducedTree -> Branch("ele2_fbrem",  &vars.ele2_fbrem,   "ele2_fbrem/F");
  vars.m_reducedTree -> Branch("ele2_EOverP", &vars.ele2_EOverP, "ele2_EOverP/F");
  
  vars.m_reducedTree -> Branch("ele2_isEB",        &vars.ele2_isEB,               "ele2_isEB/I");
  vars.m_reducedTree -> Branch("ele2_isEBEEGap",   &vars.ele2_isEBEEGap,     "ele2_isEBEEGap/I");
  vars.m_reducedTree -> Branch("ele2_isEBEtaGap",  &vars.ele2_isEBEtaGap,   "ele2_isEBEtaGap/I");
  vars.m_reducedTree -> Branch("ele2_isEBPhiGap",  &vars.ele2_isEBPhiGap,   "ele2_isEBPhiGap/I");
  vars.m_reducedTree -> Branch("ele2_isEEDeeGap",  &vars.ele2_isEEDeeGap,   "ele2_isEEDeeGap/I");
  vars.m_reducedTree -> Branch("ele2_isEERingGap", &vars.ele2_isEERingGap, "ele2_isEERingGap/I");
}



void FillWZAnalysisTree(WZAnalysisVariablesSingleXtal& vars)
{
  vars.m_reducedTree -> Fill();
}






void ClearWZAnalysisVariables(WZAnalysisVariablesSingleXtal& vars)
{
  // event variables
  vars.runId = -1;
  vars.lumiId = -1;
  
  vars.isW = -1;
  vars.isZ = -1;
  
  
  // 1st electron variables
  vars.ele1 = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_ele1 = &vars.ele1;
  vars.ele1_charge = -1.;
  vars.ele1_p = -99.;
  vars.ele1_pt = -99.;
  vars.ele1_eta = -99.;
  vars.ele1_phi = -99.;
  
  vars.ele1_sigmaIetaIeta = -99.;
  vars.ele1_DphiIn = -99.;
  vars.ele1_DetaIn = -99.;
  vars.ele1_HOverE = -99.;
  vars.ele1_tkIso = -99.;
  vars.ele1_emIso = -99.;
  vars.ele1_hadIso = -99.;

  vars.ele1_recHit_E.clear();
  vars.ele1_recHit_hashedIndex.clear();
  
  vars.ele1_scERaw = -99.;
  vars.ele1_scE = -99.;
  vars.ele1_es = -99.;
  
  vars.ele1_e3x3 = -99.;
  
  vars.ele1_tkP = -99.;
  
  vars.ele1_fbrem = -99.;
  vars.ele1_EOverP = -99.;
  
  vars.ele1_isEB = -1;
  vars.ele1_isEBEEGap = -1;
  vars.ele1_isEBEtaGap = -1;
  vars.ele1_isEBPhiGap = -1;
  vars.ele1_isEEDeeGap = -1;
  vars.ele1_isEERingGap = -1;
  
  
  // 2nd electron variables
  vars.ele2 = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_ele2 = &vars.ele2;
  vars.ele2_charge = -1.;
  vars.ele2_p = -99.;
  vars.ele2_pt = -99.;
  vars.ele2_eta = -99.;
  vars.ele2_phi = -99.;
  
  vars.ele2_sigmaIetaIeta = -99.;
  vars.ele2_DphiIn = -99.;
  vars.ele2_DetaIn = -99.;
  vars.ele2_HOverE = -99.;
  vars.ele2_tkIso = -99.;
  vars.ele2_emIso = -99.;
  vars.ele2_hadIso = -99.;

  vars.ele2_recHit_E.clear();
  vars.ele2_recHit_hashedIndex.clear();
  
  vars.ele2_scERaw = -99.;
  vars.ele2_scE = -99.;
  vars.ele2_es = -99.;
  
  vars.ele2_e3x3 = -99.;
  
  vars.ele2_tkP = -99.;
  
  vars.ele2_fbrem = -99.;
  vars.ele2_EOverP = -99.;
  
  vars.ele2_isEB = -1;
  vars.ele2_isEBEEGap = -1;
  vars.ele2_isEBEtaGap = -1;
  vars.ele2_isEBPhiGap = -1;
  vars.ele2_isEEDeeGap = -1;
  vars.ele2_isEERingGap = -1;
  
  // met variables
  vars.met = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_met = NULL;
  vars.met_et = -99.;
  vars.met_phi = -99.;

  vars.ele1Met_mt = -99.;
  vars.ele1Met_Dphi = -99.;
  
  
  // di-electron variables
  vars.ele1ele2_m = -99.;
  vars.ele1ele2_scM = -99.;
}



void DeleteWZAnalysisVariables(WZAnalysisVariablesSingleXtal& vars)
{
  // save tree
  vars.m_outputRootFile -> cd();
  vars.m_reducedTree -> Write();
  vars.m_outputRootFile -> Close();
}

void SetElectron1Variables(WZAnalysisVariablesSingleXtal& vars, treeReader& reader, const int& ele1It)
{  
  vars.ele1 = reader.Get4V("electrons")->at(ele1It);
  vars.p_ele1 = &vars.ele1;
  vars.ele1_charge = reader.GetFloat("electrons_charge")->at(ele1It);
  vars.ele1_p = vars.ele1.P();
  vars.ele1_pt = vars.ele1.pt();
  vars.ele1_eta = vars.ele1.eta();
  vars.ele1_phi = vars.ele1.phi();
  
  vars.ele1_sigmaIetaIeta = reader.GetFloat("electrons_sigmaIetaIeta")->at(ele1It);
  vars.ele1_DphiIn = reader.GetFloat("electrons_deltaPhiIn")->at(ele1It);
  vars.ele1_DetaIn = reader.GetFloat("electrons_deltaEtaIn")->at(ele1It);
  vars.ele1_HOverE = reader.GetFloat("electrons_hOverE")->at(ele1It);
  vars.ele1_tkIso = reader.GetFloat("electrons_tkIso03")->at(ele1It);
  vars.ele1_emIso = reader.GetFloat("electrons_emIso03")->at(ele1It);
  vars.ele1_hadIso = reader.GetFloat("electrons_hadIso03_1")->at(ele1It) +
      reader.GetFloat("electrons_hadIso03_2")->at(ele1It);
  
  int theRecHitBeginIndex = 0;
  if ( ele1It == 0 ) theRecHitBeginIndex = 0;
  else 
    for ( int iEle = 0; iEle < ele1It - 1; iEle++ ) theRecHitBeginIndex += reader.GetInt("recHit_n")->at(iEle);
  
  for ( int iRecHit = theRecHitBeginIndex; iRecHit < theRecHitBeginIndex + reader.GetInt("recHit_n")->at(ele1It); iRecHit++ ){
    float iRecHit_E = reader.GetFloat("recHit_E")->at(iRecHit);
    int iRecHit_hashedIndex = reader.GetInt("recHit_hashedIndex")->at(iRecHit);
    vars.ele1_recHit_E.push_back(iRecHit_E);
    vars.ele1_recHit_hashedIndex.push_back(iRecHit_hashedIndex);

  }  

  vars.ele1_scERaw = reader.GetFloat("electrons_scERaw")->at(ele1It);
  vars.ele1_scE = reader.GetFloat("electrons_scE")->at(ele1It);
  vars.ele1_es = reader.GetFloat("electrons_ES")->at(ele1It);
  
  vars.ele1_e3x3 = reader.GetFloat("electrons_e3x3")->at(ele1It);

  vars.ele1_tkP = (reader.Get3V("electrons_p_atVtx")->at(ele1It)).R();
  
  vars.ele1_fbrem = reader.GetFloat("electrons_fbrem")->at(ele1It);
  vars.ele1_EOverP = reader.GetFloat("electrons_eOverP")->at(ele1It);
  
  vars.ele1_isEB = reader.GetInt("electrons_isEB")->at(ele1It);
  vars.ele1_isEBEEGap = reader.GetInt("electrons_isEBEEGap")->at(ele1It);
  vars.ele1_isEBEtaGap = reader.GetInt("electrons_isEBEtaGap")->at(ele1It);
  vars.ele1_isEBPhiGap = reader.GetInt("electrons_isEBPhiGap")->at(ele1It);
  vars.ele1_isEEDeeGap = reader.GetInt("electrons_isEEDeeGap")->at(ele1It);
  vars.ele1_isEERingGap = reader.GetInt("electrons_isEERingGap")->at(ele1It);

}


void SetElectron2Variables(WZAnalysisVariablesSingleXtal& vars, treeReader& reader, const int& ele2It)
{
  vars.ele2 = reader.Get4V("electrons")->at(ele2It);
  vars.p_ele2 = &vars.ele2;
  vars.ele2_charge = reader.GetFloat("electrons_charge")->at(ele2It);
  vars.ele2_p = vars.ele2.P();
  vars.ele2_pt = vars.ele2.pt();
  vars.ele2_eta = vars.ele2.eta();
  vars.ele2_phi = vars.ele2.phi();
  
  vars.ele2_sigmaIetaIeta = reader.GetFloat("electrons_sigmaIetaIeta")->at(ele2It);
  vars.ele2_DphiIn = reader.GetFloat("electrons_deltaPhiIn")->at(ele2It);
  vars.ele2_DetaIn = reader.GetFloat("electrons_deltaEtaIn")->at(ele2It);
  vars.ele2_HOverE = reader.GetFloat("electrons_hOverE")->at(ele2It);
  vars.ele2_tkIso = reader.GetFloat("electrons_tkIso03")->at(ele2It);
  vars.ele2_emIso = reader.GetFloat("electrons_emIso03")->at(ele2It);
  vars.ele2_hadIso = reader.GetFloat("electrons_hadIso03_1")->at(ele2It) +
      reader.GetFloat("electrons_hadIso03_2")->at(ele2It);

  int theRecHitBeginIndex = 0;
  if ( ele2It == 0 ) theRecHitBeginIndex = 0;
  else 
    for ( int iEle = 0; iEle < ele2It - 1; iEle++ ) theRecHitBeginIndex += reader.GetInt("recHit_n")->at(iEle);
  
  for ( int iRecHit = theRecHitBeginIndex; iRecHit < theRecHitBeginIndex + reader.GetInt("recHit_n")->at(ele2It); iRecHit++ ){
    float iRecHit_E = reader.GetFloat("recHit_E")->at(iRecHit);
    int iRecHit_hashedIndex = reader.GetInt("recHit_hashedIndex")->at(iRecHit);
    vars.ele2_recHit_E.push_back(iRecHit_E);
    vars.ele2_recHit_hashedIndex.push_back(iRecHit_hashedIndex);
  }

  vars.ele2_scERaw = reader.GetFloat("electrons_scERaw")->at(ele2It);
  vars.ele2_scE = reader.GetFloat("electrons_scE")->at(ele2It);
  vars.ele2_es = reader.GetFloat("electrons_ES")->at(ele2It);
  
  vars.ele2_e3x3 = reader.GetFloat("electrons_e3x3")->at(ele2It);

  vars.ele2_tkP = (reader.Get3V("electrons_p_atVtx")->at(ele2It)).R();
  
  vars.ele2_fbrem = reader.GetFloat("electrons_fbrem")->at(ele2It);
  vars.ele2_EOverP = reader.GetFloat("electrons_eOverP")->at(ele2It);
  
  vars.ele2_isEB = reader.GetInt("electrons_isEB")->at(ele2It);
  vars.ele2_isEBEEGap = reader.GetInt("electrons_isEBEEGap")->at(ele2It);
  vars.ele2_isEBEtaGap = reader.GetInt("electrons_isEBEtaGap")->at(ele2It);
  vars.ele2_isEBPhiGap = reader.GetInt("electrons_isEBPhiGap")->at(ele2It);
  vars.ele2_isEEDeeGap = reader.GetInt("electrons_isEEDeeGap")->at(ele2It);
  vars.ele2_isEERingGap = reader.GetInt("electrons_isEERingGap")->at(ele2It);
}

void SetMetVariables(WZAnalysisVariablesSingleXtal& vars, treeReader& reader)
{
  vars.met = reader.Get4V("PFMet")->at(0);
  vars.p_met = &vars.met;
  vars.met_et = vars.p_met->Et();
  vars.met_phi = vars.p_met->phi();
  
  vars.ele1Met_mt = sqrt( 2. * vars.ele1_pt * vars.met_et * ( 1 - cos( deltaPhi(vars.ele1_phi,vars.met_phi) ) ) );
  vars.ele1Met_Dphi = deltaPhi(vars.ele1_phi,vars.met_phi);
}



void SetDiElectronVariables(WZAnalysisVariablesSingleXtal& vars, treeReader& reader)
{
  vars.ele1ele2_m = (vars.ele1 + vars.ele2).mass();
  
  ROOT::Math::PtEtaPhiEVector ele1_sc(vars.ele1_scE*sin(2*atan(exp(-1.*vars.ele1_eta))),vars.ele1_eta,vars.ele1_phi,vars.ele1_scE);
  ROOT::Math::PtEtaPhiEVector ele2_sc(vars.ele2_scE*sin(2*atan(exp(-1.*vars.ele2_eta))),vars.ele2_eta,vars.ele2_phi,vars.ele2_scE);
  vars.ele1ele2_scM = (ele1_sc + ele2_sc).mass();
}
