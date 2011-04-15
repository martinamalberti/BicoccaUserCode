#include "Calibration/EcalCalibNtuple/interface/WZAnalysisVariables.h"



TFile* GetOutputRootFile(WZAnalysisVariables& vars)
{
  return vars.m_outputRootFile;
}



void InitializeWZAnalysisTree(WZAnalysisVariables& vars, const std::string& outputRootFileName)
{
  //-------------
  // Reduced tree
  //-------------
  
  vars.m_outputRootFile = new TFile(outputRootFileName.c_str(), "RECREATE");  
  
  vars.m_reducedTree = new TTree("ntu", "ntu");
  vars.m_reducedTree -> SetDirectory(vars.m_outputRootFile);
  
  
  // event variables
  vars.m_reducedTree -> Branch("dataFlag",      &vars.dataFlag,           "dataFlag/I");
  vars.m_reducedTree -> Branch("totEvents",     &vars.totEvents,         "totEvents/I");
  vars.m_reducedTree -> Branch("crossSection",  &vars.crossSection,   "crossSection/F");
  vars.m_reducedTree -> Branch("runId",         &vars.runId,                 "runId/I");
  vars.m_reducedTree -> Branch("lumiId",        &vars.lumiId,               "lumiId/I");
  vars.m_reducedTree -> Branch("eventId",       &vars.eventId,             "eventId/I");
  vars.m_reducedTree -> Branch("eventNaiveId",  &vars.eventNaiveId,   "eventNaiveId/I");
  vars.m_reducedTree -> Branch("timeStampLow",  &vars.timeStampLow,   "timeStampLow/I");
  vars.m_reducedTree -> Branch("timeStampHigh", &vars.timeStampHigh, "timeStampHigh/I");
  
  vars.m_reducedTree -> Branch("isW", &vars.isW, "isW/I");
  vars.m_reducedTree -> Branch("isZ", &vars.isZ, "isZ/I");
  
  
  // 1st electron variables
  //vars.m_reducedTree -> Branch("ele1", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_ele1);
  vars.m_reducedTree -> Branch("ele1_charge", &vars.ele1_charge, "ele1_charge/F");
  vars.m_reducedTree -> Branch("ele1_p",      &vars.ele1_p,           "ele1_p/F");
  vars.m_reducedTree -> Branch("ele1_pt",     &vars.ele1_pt,         "ele1_pt/F");
  vars.m_reducedTree -> Branch("ele1_eta",    &vars.ele1_eta,       "ele1_eta/F");
  vars.m_reducedTree -> Branch("ele1_phi",    &vars.ele1_phi,       "ele1_phi/F");
  
  vars.m_reducedTree -> Branch("ele1_scERaw",      &vars.ele1_scERaw,           "ele1_scERaw/F");
  vars.m_reducedTree -> Branch("ele1_scEtRaw",     &vars.ele1_scEtRaw,         "ele1_scEtRaw/F");
  vars.m_reducedTree -> Branch("ele1_scE",         &vars.ele1_scE,                 "ele1_scE/F");
  vars.m_reducedTree -> Branch("ele1_scEt",        &vars.ele1_scEt,               "ele1_scEt/F");
  vars.m_reducedTree -> Branch("ele1_scEta",       &vars.ele1_scEta,             "ele1_scEta/F");
  vars.m_reducedTree -> Branch("ele1_scPhi",       &vars.ele1_scPhi,             "ele1_scPhi/F");
  vars.m_reducedTree -> Branch("ele1_scEtaWidth",  &vars.ele1_scEtaWidth,   "ele1_scEtaWidth/F");
  vars.m_reducedTree -> Branch("ele1_scPhiWidth",  &vars.ele1_scPhiWidth,   "ele1_scPhiWidth/F");
  vars.m_reducedTree -> Branch("ele1_scLaserCorr", &vars.ele1_scLaserCorr, "ele1_scLaserCorr/F");
  vars.m_reducedTree -> Branch("ele1_scCrackCorr", &vars.ele1_scCrackCorr, "ele1_scCrackCorr/F");
  vars.m_reducedTree -> Branch("ele1_es",          &vars.ele1_es,                   "ele1_es/F");
  
  vars.m_reducedTree -> Branch("ele1_seedLaserAlpha", &vars.ele1_seedLaserAlpha, "ele1_seedLaserAlpha/F");
  vars.m_reducedTree -> Branch("ele1_seedLaserCorr",  &vars.ele1_seedLaserCorr,   "ele1_seedLaserCorr/F");
  
  vars.m_reducedTree -> Branch("ele1_tkP",  &vars.ele1_tkP,   "ele1_tkP/F");
  vars.m_reducedTree -> Branch("ele1_tkPt", &vars.ele1_tkPt, "ele1_tkPt/F");
  
  vars.m_reducedTree -> Branch("ele1_fbrem",  &vars.ele1_fbrem,   "ele1_fbrem/F");
  vars.m_reducedTree -> Branch("ele1_EOverP", &vars.ele1_EOverP, "ele1_EOverP/F");
  
  vars.m_reducedTree -> Branch("ele1_isEB",        &vars.ele1_isEB,               "ele1_isEB/I");
  vars.m_reducedTree -> Branch("ele1_isEBEEGap",   &vars.ele1_isEBEEGap,     "ele1_isEBEEGap/I");
  vars.m_reducedTree -> Branch("ele1_isEBEtaGap",  &vars.ele1_isEBEtaGap,   "ele1_isEBEtaGap/I");
  vars.m_reducedTree -> Branch("ele1_isEBPhiGap",  &vars.ele1_isEBPhiGap,   "ele1_isEBPhiGap/I");
  vars.m_reducedTree -> Branch("ele1_isEEDeeGap",  &vars.ele1_isEEDeeGap,   "ele1_isEEDeeGap/I");
  vars.m_reducedTree -> Branch("ele1_isEERingGap", &vars.ele1_isEERingGap, "ele1_isEERingGap/I");
  
  
  // 2nd electron variables  
  //vars.m_reducedTree -> Branch("ele2", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_ele2);
  vars.m_reducedTree -> Branch("ele2_charge", &vars.ele2_charge, "ele2_charge/F");
  vars.m_reducedTree -> Branch("ele2_p",      &vars.ele2_p,           "ele2_p/F");
  vars.m_reducedTree -> Branch("ele2_pt",     &vars.ele2_pt,         "ele2_pt/F");
  vars.m_reducedTree -> Branch("ele2_eta",    &vars.ele2_eta,       "ele2_eta/F");
  vars.m_reducedTree -> Branch("ele2_phi",    &vars.ele2_phi,       "ele2_phi/F");
  
  vars.m_reducedTree -> Branch("ele2_scERaw",      &vars.ele2_scERaw,           "ele2_scERaw/F");
  vars.m_reducedTree -> Branch("ele2_scEtRaw",     &vars.ele2_scEtRaw,         "ele2_scEtRaw/F");
  vars.m_reducedTree -> Branch("ele2_scE",         &vars.ele2_scE,                 "ele2_scE/F");
  vars.m_reducedTree -> Branch("ele2_scEt",        &vars.ele2_scEt,               "ele2_scEt/F");
  vars.m_reducedTree -> Branch("ele2_scEta",       &vars.ele2_scEta,             "ele2_scEta/F");
  vars.m_reducedTree -> Branch("ele2_scPhi",       &vars.ele2_scPhi,             "ele2_scPhi/F");
  vars.m_reducedTree -> Branch("ele2_scEtaWidth",  &vars.ele2_scEtaWidth,   "ele2_scEtaWidth/F");
  vars.m_reducedTree -> Branch("ele2_scPhiWidth",  &vars.ele2_scPhiWidth,   "ele2_scPhiWidth/F");
  vars.m_reducedTree -> Branch("ele2_scLaserCorr", &vars.ele2_scLaserCorr, "ele2_scLaserCorr/F");
  vars.m_reducedTree -> Branch("ele2_scCrackCorr", &vars.ele2_scCrackCorr, "ele2_scCrackCorr/F");
  vars.m_reducedTree -> Branch("ele2_es",          &vars.ele2_es,                   "ele2_es/F");
  
  vars.m_reducedTree -> Branch("ele2_seedLaserAlpha", &vars.ele2_seedLaserAlpha, "ele2_seedLaserAlpha/F");
  vars.m_reducedTree -> Branch("ele2_seedLaserCorr",  &vars.ele2_seedLaserCorr,   "ele2_seedLaserCorr/F");
  
  vars.m_reducedTree -> Branch("ele2_tkP",  &vars.ele2_tkP,   "ele2_tkP/F");
  vars.m_reducedTree -> Branch("ele2_tkPt", &vars.ele2_tkPt, "ele2_tkPt/F");
  
  vars.m_reducedTree -> Branch("ele2_fbrem",  &vars.ele2_fbrem,   "ele2_fbrem/F");
  vars.m_reducedTree -> Branch("ele2_EOverP", &vars.ele2_EOverP, "ele2_EOverP/F");
  
  vars.m_reducedTree -> Branch("ele2_isEB",        &vars.ele2_isEB,               "ele2_isEB/I");
  vars.m_reducedTree -> Branch("ele2_isEBEEGap",   &vars.ele2_isEBEEGap,     "ele2_isEBEEGap/I");
  vars.m_reducedTree -> Branch("ele2_isEBEtaGap",  &vars.ele2_isEBEtaGap,   "ele2_isEBEtaGap/I");
  vars.m_reducedTree -> Branch("ele2_isEBPhiGap",  &vars.ele2_isEBPhiGap,   "ele2_isEBPhiGap/I");
  vars.m_reducedTree -> Branch("ele2_isEEDeeGap",  &vars.ele2_isEEDeeGap,   "ele2_isEEDeeGap/I");
  vars.m_reducedTree -> Branch("ele2_isEERingGap", &vars.ele2_isEERingGap, "ele2_isEERingGap/I");
  
  
  // met variables
  //vars.m_reducedTree -> Branch("met", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_met);
  vars.m_reducedTree -> Branch("met_et",  &vars.met_et,   "met_et/F");
  vars.m_reducedTree -> Branch("met_phi", &vars.met_phi, "met_phi/F");
  
  vars.m_reducedTree -> Branch("ele1Met_mt",   &vars.ele1Met_mt,      "lepMet_mt/F");
  vars.m_reducedTree -> Branch("ele1Met_Dphi", &vars.ele1Met_Dphi, "ele1Met_Dphi/F");
  
  
  // di-electron variables
  vars.m_reducedTree -> Branch("ele1ele2_m",   &vars.ele1ele2_m,     "ele1ele2_m/F");
  vars.m_reducedTree -> Branch("ele1ele2_scM", &vars.ele1ele2_scM, "ele1ele2_scM/F");
 }



void FillWZAnalysisTree(WZAnalysisVariables& vars)
{
  vars.m_reducedTree -> Fill();
}






void ClearWZAnalysisVariables(WZAnalysisVariables& vars)
{
  // event variables
  vars.dataFlag = -1;
  vars.totEvents = -1;
  vars.crossSection = -1.;
  vars.runId = -1;
  vars.lumiId = -1;
  vars.eventId = -1;
  vars.eventNaiveId = -1;
  vars.timeStampLow = -1;
  vars.timeStampHigh = -1;
  
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
  
  vars.ele1_scERaw = -99.;
  vars.ele1_scEtRaw = -99.;
  vars.ele1_scE = -99.;
  vars.ele1_scEt = -99.;
  vars.ele1_scEta = -99.;
  vars.ele1_scPhi = -99.;
  vars.ele1_scEtaWidth = -99.;
  vars.ele1_scPhiWidth = -99.;
  vars.ele1_scLaserCorr = -99.;
  vars.ele1_scCrackCorr = -99.;
  vars.ele1_es = -99.;
  
  vars.ele1_seedLaserAlpha = -99.;
  vars.ele1_seedLaserCorr = -99.;
  
  vars.ele1_tkP = -99.;
  vars.ele1_tkPt = -99.;
  
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
  
  vars.ele2_scERaw = -99.;
  vars.ele2_scEtRaw = -99.;
  vars.ele2_scE = -99.;
  vars.ele2_scEt = -99.;
  vars.ele2_scEta = -99.;
  vars.ele2_scPhi = -99.;
  vars.ele2_scEtaWidth = -99.;
  vars.ele2_scPhiWidth = -99.;
  vars.ele2_scLaserCorr = -99.;
  vars.ele2_scCrackCorr = -99.;
  vars.ele1_es = -99.;  

  vars.ele2_seedLaserAlpha = -99.;
  vars.ele2_seedLaserCorr = -99.;
  
  vars.ele2_tkP = -99.;
  vars.ele2_tkPt = -99.;
  
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



void DeleteWZAnalysisVariables(WZAnalysisVariables& vars)
{
  // save tree
  vars.m_outputRootFile -> cd();
  vars.m_reducedTree -> Write();
  vars.m_outputRootFile -> Close();
}






void SetElectron1Variables(WZAnalysisVariables& vars, treeReader& reader, const int& ele1It)
{
  vars.ele1 = reader.Get4V("electrons")->at(ele1It);
  vars.p_ele1 = &vars.ele1;
  vars.ele1_charge = reader.GetFloat("electrons_charge")->at(ele1It);
  vars.ele1_p = vars.ele1.P();
  vars.ele1_pt = vars.ele1.pt();
  vars.ele1_eta = vars.ele1.eta();
  vars.ele1_phi = vars.ele1.phi();
  
  vars.ele1_scERaw = reader.GetFloat("electrons_scERaw")->at(ele1It);
  vars.ele1_scEtRaw = reader.GetFloat("electrons_scEtRaw")->at(ele1It);
  vars.ele1_scE = reader.GetFloat("electrons_scE")->at(ele1It);
  vars.ele1_scEt = reader.GetFloat("electrons_scEt")->at(ele1It);
  vars.ele1_scEta = reader.GetFloat("electrons_scEta")->at(ele1It);
  vars.ele1_scPhi = reader.GetFloat("electrons_scPhi")->at(ele1It);
  vars.ele1_scEtaWidth = reader.GetFloat("electrons_scEtaWidth")->at(ele1It);
  vars.ele1_scPhiWidth = reader.GetFloat("electrons_scPhiWidth")->at(ele1It);
  vars.ele1_scLaserCorr = reader.GetFloat("electrons_scAvgLaserCorrection")->at(ele1It);
  vars.ele1_scCrackCorr = reader.GetFloat("electrons_scCrackCorrection")->at(ele1It);
  vars.ele1_es = reader.GetFloat("electrons_ES")->at(ele1It);
  
  vars.ele1_seedLaserAlpha = reader.GetFloat("electrons_seedLaserAlpha")->at(ele1It);
  vars.ele1_seedLaserCorr = reader.GetFloat("electrons_seedLaserCorrection")->at(ele1It);
  
  vars.ele1_tkP = (reader.Get3V("electrons_p_atVtx")->at(ele1It)).R();
  vars.ele1_tkPt = (reader.Get3V("electrons_p_atVtx")->at(ele1It)).Rho();
  
  vars.ele1_fbrem = reader.GetFloat("electrons_fbrem")->at(ele1It);
  vars.ele1_EOverP = reader.GetFloat("electrons_eOverP")->at(ele1It);
  
  vars.ele1_isEB = reader.GetInt("electrons_isEB")->at(ele1It);
  vars.ele1_isEBEEGap = reader.GetInt("electrons_isEBEEGap")->at(ele1It);
  vars.ele1_isEBEtaGap = reader.GetInt("electrons_isEBEtaGap")->at(ele1It);
  vars.ele1_isEBPhiGap = reader.GetInt("electrons_isEBPhiGap")->at(ele1It);
  vars.ele1_isEEDeeGap = reader.GetInt("electrons_isEEDeeGap")->at(ele1It);
  vars.ele1_isEERingGap = reader.GetInt("electrons_isEERingGap")->at(ele1It);
}



void SetElectron2Variables(WZAnalysisVariables& vars, treeReader& reader, const int& ele2It)
{
  vars.ele2 = reader.Get4V("electrons")->at(ele2It);
  vars.p_ele2 = &vars.ele2;
  vars.ele2_charge = reader.GetFloat("electrons_charge")->at(ele2It);
  vars.ele2_p = vars.ele2.P();
  vars.ele2_pt = vars.ele2.pt();
  vars.ele2_eta = vars.ele2.eta();
  vars.ele2_phi = vars.ele2.phi();
  
  vars.ele2_scERaw = reader.GetFloat("electrons_scERaw")->at(ele2It);
  vars.ele2_scEtRaw = reader.GetFloat("electrons_scEtRaw")->at(ele2It);
  vars.ele2_scE = reader.GetFloat("electrons_scE")->at(ele2It);
  vars.ele2_scEt = reader.GetFloat("electrons_scEt")->at(ele2It);
  vars.ele2_scEta = reader.GetFloat("electrons_scEta")->at(ele2It);
  vars.ele2_scPhi = reader.GetFloat("electrons_scPhi")->at(ele2It);
  vars.ele2_scEtaWidth = reader.GetFloat("electrons_scEtaWidth")->at(ele2It);
  vars.ele2_scPhiWidth = reader.GetFloat("electrons_scPhiWidth")->at(ele2It);
  vars.ele2_scLaserCorr = reader.GetFloat("electrons_scAvgLaserCorrection")->at(ele2It);
  vars.ele2_scCrackCorr = reader.GetFloat("electrons_scCrackCorrection")->at(ele2It);
  vars.ele2_es = reader.GetFloat("electrons_ES")->at(ele2It);
  
  vars.ele2_seedLaserAlpha = reader.GetFloat("electrons_seedLaserAlpha")->at(ele2It);
  vars.ele2_seedLaserCorr = reader.GetFloat("electrons_seedLaserCorrection")->at(ele2It);

  vars.ele2_tkP = (reader.Get3V("electrons_p_atVtx")->at(ele2It)).R();
  vars.ele2_tkPt = (reader.Get3V("electrons_p_atVtx")->at(ele2It)).Rho();

  vars.ele2_fbrem = reader.GetFloat("electrons_fbrem")->at(ele2It);
  vars.ele2_EOverP = reader.GetFloat("electrons_eOverP")->at(ele2It);
  
  vars.ele2_isEB = reader.GetInt("electrons_isEB")->at(ele2It);
  vars.ele2_isEBEEGap = reader.GetInt("electrons_isEBEEGap")->at(ele2It);
  vars.ele2_isEBEtaGap = reader.GetInt("electrons_isEBEtaGap")->at(ele2It);
  vars.ele2_isEBPhiGap = reader.GetInt("electrons_isEBPhiGap")->at(ele2It);
  vars.ele2_isEEDeeGap = reader.GetInt("electrons_isEEDeeGap")->at(ele2It);
  vars.ele2_isEERingGap = reader.GetInt("electrons_isEERingGap")->at(ele2It);
}



void SetMetVariables(WZAnalysisVariables& vars, treeReader& reader)
{
  vars.met = reader.Get4V("PFMet")->at(0);
  vars.p_met = &vars.met;
  vars.met_et = vars.p_met->Et();
  vars.met_phi = vars.p_met->phi();
  
  vars.ele1Met_mt = sqrt( 2. * vars.ele1_pt * vars.met_et * ( 1 - cos( deltaPhi(vars.ele1_phi,vars.met_phi) ) ) );
  vars.ele1Met_Dphi = deltaPhi(vars.ele1_phi,vars.met_phi);
}



void SetDiElectronVariables(WZAnalysisVariables& vars, treeReader& reader)
{
  vars.ele1ele2_m = (vars.ele1 + vars.ele2).mass();
  
  ROOT::Math::PtEtaPhiEVector ele1_sc(vars.ele1_scEt,vars.ele1_eta,vars.ele1_phi,vars.ele1_scE);
  ROOT::Math::PtEtaPhiEVector ele2_sc(vars.ele2_scEt,vars.ele2_eta,vars.ele2_phi,vars.ele2_scE);
  vars.ele1ele2_scM = (ele1_sc + ele2_sc).mass();
}
