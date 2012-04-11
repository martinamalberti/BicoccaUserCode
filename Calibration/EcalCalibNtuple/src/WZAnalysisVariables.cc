#include "Calibration/EcalCalibNtuple/interface/WZAnalysisVariables.h"

TFile* GetOutputRootFile(WZAnalysisVariables& vars)
{
  return vars.m_outputRootFile;
}



void InitializeWZAnalysisTree(WZAnalysisVariables& vars, const std::string& outputRootFileName, bool isCalib)
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
  vars.m_reducedTree -> Branch("eventId",       &vars.eventId,             "eventId/I");

  if(!isCalib)
  {
    vars.m_reducedTree -> Branch("dataFlag",      &vars.dataFlag,           "dataFlag/I");
    vars.m_reducedTree -> Branch("totEvents",     &vars.totEvents,         "totEvents/I");
    vars.m_reducedTree -> Branch("crossSection",  &vars.crossSection,   "crossSection/F");

    vars.m_reducedTree -> Branch("eventNaiveId",  &vars.eventNaiveId,   "eventNaiveId/I");
    vars.m_reducedTree -> Branch("timeStampLow",  &vars.timeStampLow,   "timeStampLow/I");
    vars.m_reducedTree -> Branch("timeStampHigh", &vars.timeStampHigh, "timeStampHigh/I");
  }
  
  vars.m_reducedTree -> Branch("isW", &vars.isW, "isW/I");
  vars.m_reducedTree -> Branch("isZ", &vars.isZ, "isZ/I");
  
  if(!isCalib)
  {
    // PU variables
    vars.m_reducedTree -> Branch("PUit_TrueNumInteraction",  &vars.PUit_TrueNumInteraction,    "PUit_TrueNumInteraction/F");
    vars.m_reducedTree -> Branch("PUit_NumInteractions", &vars.PUit_NumInteraction,   "PUit_NumInteraction/I");
    vars.m_reducedTree -> Branch("PUoot_early",  &vars.PUoot_early, "PUoot_early/I");
    vars.m_reducedTree -> Branch("PUoot_late", &vars.PUoot_late,   "PUoot_late/I");
  
    vars.m_reducedTree -> Branch("rhoForIsolation", &vars.rhoForIsolation, "rhoForIsolation/F");
    vars.m_reducedTree -> Branch("rhoForJets",      &vars.rhoForJets,           "rhoForJets/F");
  
      // PV variables
    vars.m_reducedTree -> Branch("PV_n",     &vars.PV_n,         "PV_n/I");
    vars.m_reducedTree -> Branch("PV_z",     &vars.PV_z,         "PV_z/F");
    vars.m_reducedTree -> Branch("PV_d0",    &vars.PV_d0,       "PV_d0/F");
    vars.m_reducedTree -> Branch("PV_SumPt", &vars.PV_SumPt, "PV_SumPt/F");
  }
    
  // 1st electron variables
  if(isCalib)
  {
    vars.m_reducedTree -> Branch("ele1_recHit_E",           "std::vector<float>", &vars.ele1_recHit_E);
    vars.m_reducedTree -> Branch("ele1_recHit_hashedIndex", "std::vector<int>",   &vars.ele1_recHit_hashedIndex);
    vars.m_reducedTree -> Branch("ele1_recHit_ietaORix", "std::vector<int>",   &vars.ele1_recHit_ieta);
    vars.m_reducedTree -> Branch("ele1_recHit_iphiORiy", "std::vector<int>",   &vars.ele1_recHit_iphi);
    vars.m_reducedTree -> Branch("ele1_recHit_zside", "std::vector<int>",   &vars.ele1_recHit_zside);
    vars.m_reducedTree -> Branch("ele1_recHit_flag", "std::vector<int>",   &vars.ele1_recHit_flag);
  }
  
  vars.m_reducedTree -> Branch("ele1_scERaw",               &vars.ele1_scERaw,                             "ele1_scERaw/F");
  vars.m_reducedTree -> Branch("ele1_scE",                  &vars.ele1_scE,                                   "ele1_scE/F");
  vars.m_reducedTree -> Branch("ele1_es",                   &vars.ele1_es,                                     "ele1_es/F");
  vars.m_reducedTree -> Branch("ele1_fCorrection",          &vars.ele1_fCorrection,                   "ele1_fCorrection/F");
  vars.m_reducedTree -> Branch("ele1_scE_regression",       &vars.ele1_scE_regression,             "ele1_scE_regression/F");
  vars.m_reducedTree -> Branch("ele1_scEerr_regression",       &vars.ele1_scEerr_regression,             "ele1_scEerr_regression/F");
  vars.m_reducedTree -> Branch("ele1_scERaw_PUcleaned",     &vars.ele1_scERaw_PUcleaned,         "ele1_scERaw_PUcleaned/F");
  vars.m_reducedTree -> Branch("ele1_scEtaWidth_PUcleaned", &vars.ele1_scEtaWidth_PUcleaned, "ele1_scEtaWidth_PUcleaned/F");
  vars.m_reducedTree -> Branch("ele1_scPhiWidth_PUcleaned", &vars.ele1_scPhiWidth_PUcleaned, "ele1_scPhiWidth_PUcleaned/F");
  vars.m_reducedTree -> Branch("ele1_fCorrection_PUcleaned",&vars.ele1_fCorrection_PUcleaned,"ele1_fCorrection_PUcleaned/F");

  vars.m_reducedTree -> Branch("ele1_bcN",  &vars.ele1_bcN,   "ele1_bcN/I");
  vars.m_reducedTree -> Branch("ele1_e3x3", &vars.ele1_e3x3, "ele1_e3x3/F");
  vars.m_reducedTree -> Branch("ele1_e5x5", &vars.ele1_e5x5, "ele1_e5x5/F");
  
  vars.m_reducedTree -> Branch("ele1_tkP",  &vars.ele1_tkP,   "ele1_tkP/F");
  
  vars.m_reducedTree -> Branch("ele1_fbrem",  &vars.ele1_fbrem,   "ele1_fbrem/F");
  vars.m_reducedTree -> Branch("ele1_EOverP", &vars.ele1_EOverP, "ele1_EOverP/F");
  
  vars.m_reducedTree -> Branch("ele1_isEB",        &vars.ele1_isEB,               "ele1_isEB/I");
  vars.m_reducedTree -> Branch("ele1_isEBEEGap",   &vars.ele1_isEBEEGap,     "ele1_isEBEEGap/I");
  vars.m_reducedTree -> Branch("ele1_isEBEtaGap",  &vars.ele1_isEBEtaGap,   "ele1_isEBEtaGap/I");
  vars.m_reducedTree -> Branch("ele1_isEBPhiGap",  &vars.ele1_isEBPhiGap,   "ele1_isEBPhiGap/I");
  vars.m_reducedTree -> Branch("ele1_isEEDeeGap",  &vars.ele1_isEEDeeGap,   "ele1_isEEDeeGap/I");
  vars.m_reducedTree -> Branch("ele1_isEERingGap", &vars.ele1_isEERingGap, "ele1_isEERingGap/I");

  vars.m_reducedTree -> Branch("ele1_nRecHits",    &vars.ele1_nRecHits,       "ele1_nRecHits/I");
  
  if(!isCalib)
  {
    vars.m_reducedTree -> Branch("ele1_charge", &vars.ele1_charge, "ele1_charge/F");

    vars.m_reducedTree -> Branch("ele1_p",      &vars.ele1_p,           "ele1_p/F");
    vars.m_reducedTree -> Branch("ele1_pt",     &vars.ele1_pt,         "ele1_pt/F");
    vars.m_reducedTree -> Branch("ele1_eta",    &vars.ele1_eta,       "ele1_eta/F");
    vars.m_reducedTree -> Branch("ele1_phi",    &vars.ele1_phi,       "ele1_phi/F");
  
    vars.m_reducedTree -> Branch("ele1_sigmaIetaIeta", &vars.ele1_sigmaIetaIeta, "ele1_sigmaIetaIeta/F");
    vars.m_reducedTree -> Branch("ele1_DphiIn",        &vars.ele1_DphiIn,               "ele1_DphiIn/F");
    vars.m_reducedTree -> Branch("ele1_DetaIn",        &vars.ele1_DetaIn,               "ele1_DetaIn/F");
    vars.m_reducedTree -> Branch("ele1_HOverE",        &vars.ele1_HOverE,               "ele1_HOverE/F");
    vars.m_reducedTree -> Branch("ele1_tkIso",         &vars.ele1_tkIso,                 "ele1_tkIso/F");
    vars.m_reducedTree -> Branch("ele1_emIso",         &vars.ele1_emIso,                 "ele1_emIso/F");
    vars.m_reducedTree -> Branch("ele1_hadIso",        &vars.ele1_hadIso,               "ele1_hadIso/F");
  
    vars.m_reducedTree -> Branch("ele1_scEtRaw",     &vars.ele1_scEtRaw,         "ele1_scEtRaw/F");
    vars.m_reducedTree -> Branch("ele1_scEt",        &vars.ele1_scEt,               "ele1_scEt/F");
    vars.m_reducedTree -> Branch("ele1_scEta",       &vars.ele1_scEta,             "ele1_scEta/F");
    vars.m_reducedTree -> Branch("ele1_scPhi",       &vars.ele1_scPhi,             "ele1_scPhi/F");
    vars.m_reducedTree -> Branch("ele1_scLocalPhi",       &vars.ele1_scLocalPhi,             "ele1_scLocalPhi/F");
    vars.m_reducedTree -> Branch("ele1_scLocalEta",       &vars.ele1_scLocalEta,             "ele1_scLocalEta/F");
    vars.m_reducedTree -> Branch("ele1_scEtaWidth",  &vars.ele1_scEtaWidth,   "ele1_scEtaWidth/F");
    vars.m_reducedTree -> Branch("ele1_scPhiWidth",  &vars.ele1_scPhiWidth,   "ele1_scPhiWidth/F");
    vars.m_reducedTree -> Branch("ele1_scLaserCorr", &vars.ele1_scLaserCorr, "ele1_scLaserCorr/F");
    vars.m_reducedTree -> Branch("ele1_scCrackCorr", &vars.ele1_scCrackCorr, "ele1_scCrackCorr/F");
    vars.m_reducedTree -> Branch("ele1_scLocalContCorr", &vars.ele1_scLocalContCorr, "ele1_scLocalContCorr/F");
    vars.m_reducedTree -> Branch("ele1_scLocalContCorr_DK", &vars.ele1_scLocalContCorr_DK, "ele1_scLocalContCorr_DK/F");
  
    vars.m_reducedTree -> Branch("ele1_nPh", &vars.ele1_nPh, "ele1_nPh/I");
    vars.m_reducedTree -> Branch("ele1_ph_E", &vars.ele1_ph_E, "ele1_ph_E/F");
    vars.m_reducedTree -> Branch("ele1_ph_scEta", &vars.ele1_ph_scEta, "ele1_ph_scEta/F");
    vars.m_reducedTree -> Branch("ele1_ph_scPhi", &vars.ele1_ph_scPhi, "ele1_ph_scPhi/F");
    vars.m_reducedTree -> Branch("ele1_ph_R9", &vars.ele1_ph_R9, "ele1_ph_R9/F");
 
    vars.m_reducedTree -> Branch("ele1_seedE",          &vars.ele1_seedE,                   "ele1_seedE/F");
    vars.m_reducedTree -> Branch("ele1_seedIeta",       &vars.ele1_seedIeta,             "ele1_seedIeta/I");
    vars.m_reducedTree -> Branch("ele1_seedIphi",       &vars.ele1_seedIphi,             "ele1_seedIphi/I");
    vars.m_reducedTree -> Branch("ele1_seedIx",         &vars.ele1_seedIx,                 "ele1_seedIx/I");
    vars.m_reducedTree -> Branch("ele1_seedIy",         &vars.ele1_seedIy,                 "ele1_seedIy/I");
    vars.m_reducedTree -> Branch("ele1_seedZside",      &vars.ele1_seedZside,           "ele1_seedZside/I");
    vars.m_reducedTree -> Branch("ele1_seedLaserAlpha", &vars.ele1_seedLaserAlpha, "ele1_seedLaserAlpha/F");
    vars.m_reducedTree -> Branch("ele1_seedLaserCorr",  &vars.ele1_seedLaserCorr,   "ele1_seedLaserCorr/F");
  
    vars.m_reducedTree -> Branch("ele1_tkPt", &vars.ele1_tkPt, "ele1_tkPt/F");
  
   }
  
  // 2nd electron variables  
  if(isCalib)
  {
    vars.m_reducedTree -> Branch("ele2_recHit_E",           "std::vector<float>", &vars.ele2_recHit_E);
    vars.m_reducedTree -> Branch("ele2_recHit_hashedIndex", "std::vector<int>",   &vars.ele2_recHit_hashedIndex);
    vars.m_reducedTree -> Branch("ele2_recHit_ietaORix", "std::vector<int>",   &vars.ele2_recHit_ieta);
    vars.m_reducedTree -> Branch("ele2_recHit_iphiORiy", "std::vector<int>",   &vars.ele2_recHit_iphi);
    vars.m_reducedTree -> Branch("ele2_recHit_zside", "std::vector<int>",   &vars.ele2_recHit_zside);
    vars.m_reducedTree -> Branch("ele2_recHit_flag", "std::vector<int>",   &vars.ele2_recHit_flag);
  }
  
  vars.m_reducedTree -> Branch("ele2_scERaw",               &vars.ele2_scERaw,                             "ele2_scERaw/F");
  vars.m_reducedTree -> Branch("ele2_scE",                  &vars.ele2_scE,                                   "ele2_scE/F");
  vars.m_reducedTree -> Branch("ele2_es",                   &vars.ele2_es,                                     "ele2_es/F");
  vars.m_reducedTree -> Branch("ele2_fCorrection",          &vars.ele2_fCorrection,                   "ele2_fCorrection/F");
  vars.m_reducedTree -> Branch("ele2_scE_regression",       &vars.ele2_scE_regression,             "ele2_scE_regression/F");
  vars.m_reducedTree -> Branch("ele2_scEerr_regression",       &vars.ele2_scEerr_regression,             "ele2_scEerr_regression/F");
  vars.m_reducedTree -> Branch("ele2_scERaw_PUcleaned",     &vars.ele2_scERaw_PUcleaned,         "ele2_scERaw_PUcleaned/F");
  vars.m_reducedTree -> Branch("ele2_scEtaWidth_PUcleaned", &vars.ele2_scEtaWidth_PUcleaned, "ele2_scEtaWidth_PUcleaned/F");
  vars.m_reducedTree -> Branch("ele2_scPhiWidth_PUcleaned", &vars.ele2_scPhiWidth_PUcleaned, "ele2_scPhiWidth_PUcleaned/F");
  vars.m_reducedTree -> Branch("ele2_fCorrection_PUcleaned",&vars.ele2_fCorrection_PUcleaned,"ele2_fCorrection_PUcleaned/F");
  
  
  vars.m_reducedTree -> Branch("ele2_bcN",  &vars.ele2_bcN,   "ele2_bcN/I");
  vars.m_reducedTree -> Branch("ele2_e3x3", &vars.ele2_e3x3, "ele2_e3x3/F");
  vars.m_reducedTree -> Branch("ele2_e5x5", &vars.ele2_e5x5, "ele2_e5x5/F");

  vars.m_reducedTree -> Branch("ele2_tkP",  &vars.ele2_tkP,   "ele2_tkP/F");
  
  vars.m_reducedTree -> Branch("ele2_fbrem",  &vars.ele2_fbrem,   "ele2_fbrem/F");
  vars.m_reducedTree -> Branch("ele2_EOverP", &vars.ele2_EOverP, "ele2_EOverP/F");
  
  vars.m_reducedTree -> Branch("ele2_isEB",        &vars.ele2_isEB,               "ele2_isEB/I");
  vars.m_reducedTree -> Branch("ele2_isEBEEGap",   &vars.ele2_isEBEEGap,     "ele2_isEBEEGap/I");
  vars.m_reducedTree -> Branch("ele2_isEBEtaGap",  &vars.ele2_isEBEtaGap,   "ele2_isEBEtaGap/I");
  vars.m_reducedTree -> Branch("ele2_isEBPhiGap",  &vars.ele2_isEBPhiGap,   "ele2_isEBPhiGap/I");
  vars.m_reducedTree -> Branch("ele2_isEEDeeGap",  &vars.ele2_isEEDeeGap,   "ele2_isEEDeeGap/I");
  vars.m_reducedTree -> Branch("ele2_isEERingGap", &vars.ele2_isEERingGap, "ele2_isEERingGap/I");

  vars.m_reducedTree -> Branch("ele2_nRecHits",    &vars.ele2_nRecHits,       "ele2_nRecHits/I");

  if(!isCalib)
  {
   vars.m_reducedTree -> Branch("ele2_charge", &vars.ele2_charge, "ele2_charge/F");
   vars.m_reducedTree -> Branch("ele2_p",      &vars.ele2_p,           "ele2_p/F");
   vars.m_reducedTree -> Branch("ele2_pt",     &vars.ele2_pt,         "ele2_pt/F");
   vars.m_reducedTree -> Branch("ele2_eta",    &vars.ele2_eta,       "ele2_eta/F");
   vars.m_reducedTree -> Branch("ele2_phi",    &vars.ele2_phi,       "ele2_phi/F");
  
   vars.m_reducedTree -> Branch("ele2_sigmaIetaIeta", &vars.ele2_sigmaIetaIeta, "ele2_sigmaIetaIeta/F");
   vars.m_reducedTree -> Branch("ele2_DphiIn",        &vars.ele2_DphiIn,               "ele2_DphiIn/F");
   vars.m_reducedTree -> Branch("ele2_DetaIn",        &vars.ele2_DetaIn,               "ele2_DetaIn/F");
   vars.m_reducedTree -> Branch("ele2_HOverE",        &vars.ele2_HOverE,               "ele2_HOverE/F");
   vars.m_reducedTree -> Branch("ele2_tkIso",         &vars.ele2_tkIso,                 "ele2_tkIso/F");
   vars.m_reducedTree -> Branch("ele2_emIso",         &vars.ele2_emIso,                 "ele2_emIso/F");
   vars.m_reducedTree -> Branch("ele2_hadIso",        &vars.ele2_hadIso,               "ele2_hadIso/F");
  
   vars.m_reducedTree -> Branch("ele2_scEtRaw",     &vars.ele2_scEtRaw,         "ele2_scEtRaw/F");
   vars.m_reducedTree -> Branch("ele2_scEt",        &vars.ele2_scEt,               "ele2_scEt/F");
   vars.m_reducedTree -> Branch("ele2_scEta",       &vars.ele2_scEta,             "ele2_scEta/F");
   vars.m_reducedTree -> Branch("ele2_scPhi",       &vars.ele2_scPhi,             "ele2_scPhi/F");
   vars.m_reducedTree -> Branch("ele2_scLocalPhi",       &vars.ele2_scLocalPhi,             "ele2_scLocalPhi/F");
   vars.m_reducedTree -> Branch("ele2_scLocalEta",       &vars.ele2_scLocalEta,             "ele2_scLocalEta/F");
   vars.m_reducedTree -> Branch("ele2_scEtaWidth",  &vars.ele2_scEtaWidth,   "ele2_scEtaWidth/F");
   vars.m_reducedTree -> Branch("ele2_scPhiWidth",  &vars.ele2_scPhiWidth,   "ele2_scPhiWidth/F");
   vars.m_reducedTree -> Branch("ele2_scLaserCorr", &vars.ele2_scLaserCorr, "ele2_scLaserCorr/F");
   vars.m_reducedTree -> Branch("ele2_scCrackCorr", &vars.ele2_scCrackCorr, "ele2_scCrackCorr/F");
   vars.m_reducedTree -> Branch("ele2_scLocalContCorr", &vars.ele2_scLocalContCorr, "ele2_scLocalContCorr/F");
   vars.m_reducedTree -> Branch("ele2_scLocalContCorr_DK", &vars.ele2_scLocalContCorr_DK, "ele2_scLocalContCorr_DK/F");
  
  
   vars.m_reducedTree -> Branch("ele2_nPh", &vars.ele2_nPh, "ele2_nPh/I");
   vars.m_reducedTree -> Branch("ele2_ph_E", &vars.ele2_ph_E, "ele2_ph_E/F");
   vars.m_reducedTree -> Branch("ele2_ph_scEta", &vars.ele2_ph_scEta, "ele2_ph_scEta/F");
   vars.m_reducedTree -> Branch("ele2_ph_scPhi", &vars.ele2_ph_scPhi, "ele2_ph_scPhi/F");
   vars.m_reducedTree -> Branch("ele2_ph_R9", &vars.ele2_ph_R9, "ele2_ph_R9/F");

   vars.m_reducedTree -> Branch("ele2_seedE",          &vars.ele2_seedE,                   "ele2_seedE/F");
   vars.m_reducedTree -> Branch("ele2_seedIeta",       &vars.ele2_seedIeta,             "ele2_seedIeta/I");
   vars.m_reducedTree -> Branch("ele2_seedIphi",       &vars.ele2_seedIphi,             "ele2_seedIphi/I");
   vars.m_reducedTree -> Branch("ele2_seedIx",         &vars.ele2_seedIx,                 "ele2_seedIx/I");
   vars.m_reducedTree -> Branch("ele2_seedIy",         &vars.ele2_seedIy,                 "ele2_seedIy/I");
   vars.m_reducedTree -> Branch("ele2_seedZside",      &vars.ele2_seedZside,           "ele2_seedZside/I");
   vars.m_reducedTree -> Branch("ele2_seedLaserAlpha", &vars.ele2_seedLaserAlpha, "ele2_seedLaserAlpha/F");
   vars.m_reducedTree -> Branch("ele2_seedLaserCorr",  &vars.ele2_seedLaserCorr,   "ele2_seedLaserCorr/F");
  
   vars.m_reducedTree -> Branch("ele2_tkPt", &vars.ele2_tkPt, "ele2_tkPt/F");
  
 }

 vars.m_reducedTree -> Branch("ele1_E_true", &vars.ele1_E_true, "ele1_E_true/F");
 vars.m_reducedTree -> Branch("ele1_DR", &vars.ele1_DR, "ele1_DR/F");
 vars.m_reducedTree -> Branch("ele2_E_true", &vars.ele2_E_true, "ele2_E_true/F");
 vars.m_reducedTree -> Branch("ele2_DR", &vars.ele2_DR, "ele2_DR/F");
 
 
 // met variables
 //vars.m_reducedTree -> Branch("met", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_met);
 vars.m_reducedTree -> Branch("met_et",  &vars.met_et,   "met_et/F");
 vars.m_reducedTree -> Branch("met_phi", &vars.met_phi, "met_phi/F");
  
 vars.m_reducedTree -> Branch("ele1Met_mt",   &vars.ele1Met_mt,      "lepMet_mt/F");
 vars.m_reducedTree -> Branch("ele1Met_Dphi", &vars.ele1Met_Dphi, "ele1Met_Dphi/F");
  
  
 // di-electron variables
 vars.m_reducedTree -> Branch("ele1ele2_m",   &vars.ele1ele2_m,     "ele1ele2_m/F");
 vars.m_reducedTree -> Branch("ele1ele2_scM", &vars.ele1ele2_scM, "ele1ele2_scM/F");
 vars.m_reducedTree -> Branch("ele1ele2_scM_regression", &vars.ele1ele2_scM_regression, "ele1ele2_scM_regression/F");
}



void FillWZAnalysisTree(WZAnalysisVariables& vars)
{
  vars.m_reducedTree -> Fill();
}






void ClearWZAnalysisVariables(WZAnalysisVariables& vars, bool isCalib)
{
  // event variables
  vars.runId = -1;
  vars.lumiId = -1;
  vars.eventId = -1;
  
  if(!isCalib)
  {
    vars.dataFlag = -1;
    vars.totEvents = -1;
    vars.crossSection = -1.;
    vars.eventNaiveId = -1;
    vars.timeStampLow = -1;
    vars.timeStampHigh = -1;
  }
  
  
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
   
     // PU variables
  vars.PUit_TrueNumInteraction = -1;
  vars.PUit_NumInteraction = -1;
  vars.PUoot_early = -1;
  vars.PUoot_late = -1;

  vars.rhoForIsolation = -99.;
  vars.rhoForJets = -99.;
  
  vars.ele1_scEtRaw = -99.;
  vars.ele1_scEt = -99.;
  vars.ele1_scEta = -99.;
  vars.ele1_scPhi = -99.;
  vars.ele1_scLocalEta = -99.;
  vars.ele1_scLocalPhi = -99.;
  vars.ele1_scEtaWidth = -99.;
  vars.ele1_scPhiWidth = -99.;
  vars.ele1_scLaserCorr = -99.;
  vars.ele1_scCrackCorr = -99.;
  vars.ele1_scLocalContCorr = -99.;
  vars.ele1_scLocalContCorr_DK = -99.;
  
  vars.ele1_nPh = -99;
  vars.ele1_ph_E = -99;
  vars.ele1_ph_scEta = -99;
  vars.ele1_ph_scPhi = -99;
  vars.ele1_ph_R9 = -99;

  vars.ele1_seedE = -99.;
  vars.ele1_seedIeta = -99;
  vars.ele1_seedIphi = -99;
  vars.ele1_seedIx = -99;
  vars.ele1_seedIy = -99;
  vars.ele1_seedZside = -99;
  vars.ele1_seedLaserAlpha = -99.;
  vars.ele1_seedLaserCorr = -99.;

  if(isCalib)
  {
   vars.ele1_recHit_E.clear();
   vars.ele1_recHit_hashedIndex.clear();
   vars.ele1_recHit_ieta.clear();
   vars.ele1_recHit_iphi.clear();
   vars.ele1_recHit_zside.clear();
   vars.ele1_recHit_laserCorrection.clear();
   vars.ele1_recHit_flag.clear();
  }

  
  vars.ele1_scERaw = -99.;
  vars.ele1_scE = -99.;
  vars.ele1_es = -99.;
  vars.ele1_fCorrection = -1.;
  vars.ele1_fCorrection_PUcleaned = -1.;
  vars.ele1_scE_regression = -99.;
  vars.ele1_scEerr_regression = -99.;
  vars.ele1_scERaw_PUcleaned = -99.;
  vars.ele1_scEtaWidth_PUcleaned = -99.;
  vars.ele1_scPhiWidth_PUcleaned = -99.;
  
  vars.ele1_bcN = -99;
  vars.ele1_e3x3 = -99.;
  vars.ele1_e5x5 = -99.;
  
  vars.ele1_tkP = -99.;
  
  vars.ele1_fbrem = -99.;
  vars.ele1_EOverP = -99.;
  
  vars.ele1_isEB = -1;
  vars.ele1_isEBEEGap = -1;
  vars.ele1_isEBEtaGap = -1;
  vars.ele1_isEBPhiGap = -1;
  vars.ele1_isEEDeeGap = -1;
  vars.ele1_isEERingGap = -1;
  
  vars.ele1_nRecHits = -1;

  // 2nd electron variables
  vars.ele2 = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_ele2 = &vars.ele2;
  
  if(!isCalib)
  {
   vars.ele2_charge = -1.;
   vars.ele2_p = -99.;
   vars.ele2_pt = -99.;
   vars.ele2_eta = -99.;
   vars.ele2_phi = -99.;
  }
   vars.ele2_sigmaIetaIeta = -99.;
   vars.ele2_DphiIn = -99.;
   vars.ele2_DetaIn = -99.;
   vars.ele2_HOverE = -99.;
   vars.ele2_tkIso = -99.;
   vars.ele2_emIso = -99.;
   vars.ele2_hadIso = -99.;

   vars.ele2_scEtRaw = -99.;
   vars.ele2_scEt = -99.;
   vars.ele2_scEta = -99.;
   vars.ele2_scPhi = -99.;
   vars.ele2_scLocalEta = -99.;
   vars.ele2_scLocalPhi = -99.;
   vars.ele2_scEtaWidth = -99.;
   vars.ele2_scPhiWidth = -99.;
   vars.ele2_scLaserCorr = -99.;
   vars.ele2_scCrackCorr = -99.;
   vars.ele2_scLocalContCorr = -99.;
   vars.ele2_scLocalContCorr_DK = -99.;
   
   vars.ele2_nPh = -99;
   vars.ele2_ph_E = -99;
   vars.ele2_ph_scEta = -99;
   vars.ele2_ph_scPhi = -99;
   vars.ele2_ph_R9 = -99;
  
   vars.ele2_seedE = -99.;
   vars.ele2_seedIeta = -99;
   vars.ele2_seedIphi = -99;
   vars.ele2_seedIx = -99;
   vars.ele2_seedIy = -99;
   vars.ele2_seedZside = -99;
   vars.ele2_seedLaserAlpha = -99.;
   vars.ele2_seedLaserCorr = -99.;
   
  
  if(isCalib)
  {
   vars.ele2_recHit_E.clear();
   vars.ele2_recHit_hashedIndex.clear();
   vars.ele2_recHit_ieta.clear();
   vars.ele2_recHit_iphi.clear();
   vars.ele2_recHit_zside.clear();
   vars.ele2_recHit_laserCorrection.clear();
   vars.ele2_recHit_flag.clear();
  }
  
  vars.ele2_scERaw = -99.;
  vars.ele2_scE = -99.;
  vars.ele2_es = -99.;
  vars.ele2_fCorrection = -1.;
  vars.ele2_fCorrection_PUcleaned = -1.;
  vars.ele2_scE_regression = -99.;
  vars.ele2_scEerr_regression = -99.;
  vars.ele2_scERaw_PUcleaned = -99.;
  vars.ele2_scEtaWidth_PUcleaned = -99.;
  vars.ele2_scPhiWidth_PUcleaned = -99.;
  
  vars.ele2_bcN = -99;
  vars.ele2_e3x3 = -99.;
  vars.ele2_e5x5 = -99.;
  
  vars.ele2_tkP = -99.;
  
  vars.ele2_fbrem = -99.;
  vars.ele2_EOverP = -99.;
  
  vars.ele2_isEB = -1;
  vars.ele2_isEBEEGap = -1;
  vars.ele2_isEBEtaGap = -1;
  vars.ele2_isEBPhiGap = -1;
  vars.ele2_isEEDeeGap = -1;
  vars.ele2_isEERingGap = -1;

  vars.ele2_nRecHits = -1;

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
  vars.ele1ele2_scM_regression = -99.;

  vars.ele1_E_true = -99 ;
  vars.ele1_DR = -99 ;
  vars.ele2_E_true = -99 ; 
  vars.ele2_DR = -99 ;
}



void DeleteWZAnalysisVariables(WZAnalysisVariables& vars)
{
  // save tree
  vars.m_outputRootFile -> cd();
  vars.m_reducedTree -> Write();
  vars.m_outputRootFile -> Close();
}

void SetPUVariables(WZAnalysisVariables& vars, treeReader& reader, const int& dataFlag)
{
  if( dataFlag == 0 )
  {
    vars.PUit_TrueNumInteraction = reader.GetFloat("mc_PUit_TrueNumInteractions")->at(0);
    vars.PUit_NumInteraction = int (reader.GetInt("mc_PUit_NumInteractions")->at(0));
    vars.PUoot_early = (int)(reader.GetInt("mc_PUoot_early_NumInteractions")->at(0));
//     vars.PUoot_late = (int)(reader.GetInt("mc_PUoot_late_NumInteractions")->at(0));
  }
  
   vars.rhoForIsolation = reader.GetFloat("rho_isolation")->at(0);
   vars.rhoForJets = reader.GetFloat("rho_jets")->at(0);
}

void SetPVVariables(WZAnalysisVariables& vars, treeReader& reader)
{
  vars.PV_n = reader.GetFloat("PV_z")->size();
  
  vars.PV_z = reader.GetFloat("PV_z")->at(0);
  vars.PV_d0 = reader.GetFloat("PV_d0")->at(0);
  vars.PV_SumPt = reader.GetFloat("PV_SumPt")->at(0);
}



void SetElectron1Variables(WZAnalysisVariables& vars, treeReader& reader, const int& ele1It, bool isCalib)
{  
  
  vars.p_ele1 = &vars.ele1;
   
  vars.ele1 = reader.Get4V("electrons")->at(ele1It);
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
 
  vars.ele1_scEtRaw = reader.GetFloat("electrons_scEtRaw")->at(ele1It);
  vars.ele1_scEt = reader.GetFloat("electrons_scEt")->at(ele1It);
  vars.ele1_scEta = reader.GetFloat("electrons_scEta")->at(ele1It);
  vars.ele1_scPhi = reader.GetFloat("electrons_scPhi")->at(ele1It);
  vars.ele1_scLocalEta = reader.GetFloat("electrons_scLocalPositionEtaCry")->at(ele1It);
  vars.ele1_scLocalPhi = reader.GetFloat("electrons_scLocalPositionPhiCry")->at(ele1It);
  vars.ele1_scEtaWidth = reader.GetFloat("electrons_scEtaWidth")->at(ele1It);
  vars.ele1_scPhiWidth = reader.GetFloat("electrons_scPhiWidth")->at(ele1It);
  vars.ele1_scLaserCorr = reader.GetFloat("electrons_scAvgLaserCorrection")->at(ele1It);
  vars.ele1_scCrackCorr = reader.GetFloat("electrons_scCrackCorrection")->at(ele1It);
  vars.ele1_scLocalContCorr = reader.GetFloat("electrons_scLocalContCorrection")->at(ele1It);

  float var[5];
  SetPhotonMatchingEle( var, reader, ele1It);
  vars.ele1_nPh = (int) var[0];
  vars.ele1_ph_E = var[1];
  vars.ele1_ph_scEta = var[2];
  vars.ele1_ph_scPhi = var[3];
  vars.ele1_ph_R9 = var[4];
  

  vars.ele1_seedE = reader.GetFloat("electrons_seedE")->at(ele1It);
  vars.ele1_seedIeta = reader.GetInt("electrons_seedIeta")->at(ele1It);
  vars.ele1_seedIphi = reader.GetInt("electrons_seedIphi")->at(ele1It);
  vars.ele1_seedIx = reader.GetInt("electrons_seedIx")->at(ele1It);
  vars.ele1_seedIy = reader.GetInt("electrons_seedIy")->at(ele1It);
  vars.ele1_seedZside = reader.GetInt("electrons_seedZside")->at(ele1It);
  vars.ele1_seedLaserAlpha = reader.GetFloat("electrons_seedLaserAlpha")->at(ele1It);
  vars.ele1_seedLaserCorr = reader.GetFloat("electrons_seedLaserCorrection")->at(ele1It);
  
  vars.ele1_tkPt = (reader.Get3V("electrons_p_atVtx")->at(ele1It)).Rho();
  
  if(isCalib)
  {
   int theRecHitBeginIndex = 0;
   if ( ele1It == 0 ) theRecHitBeginIndex = 0;
   else 
   for ( int iEle = 0; iEle < ele1It; iEle++ ) theRecHitBeginIndex += reader.GetInt("recHit_n")->at(iEle);
  
   for ( int iRecHit = theRecHitBeginIndex; iRecHit < theRecHitBeginIndex + reader.GetInt("recHit_n")->at(ele1It); iRecHit++ ){
    
    int iRecHit_zside = reader.GetInt("recHit_zside")->at(iRecHit);
    float iRecHit_E = reader.GetFloat("recHit_E")->at(iRecHit);
    int iRecHit_hashedIndex = reader.GetInt("recHit_hashedIndex")->at(iRecHit);
    int iRecHit_flag = reader.GetInt("recHit_flag")->at(iRecHit);
    int iRecHit_ietaORix, iRecHit_iphiORiy;
    if ( iRecHit_zside == 0 ) {
      iRecHit_ietaORix = reader.GetInt("recHit_ieta")->at(iRecHit);
      iRecHit_iphiORiy = reader.GetInt("recHit_iphi")->at(iRecHit);
    }
    else {
      iRecHit_ietaORix = reader.GetInt("recHit_ix")->at(iRecHit);
      iRecHit_iphiORiy = reader.GetInt("recHit_iy")->at(iRecHit);
    }
    vars.ele1_recHit_zside.push_back(iRecHit_zside);
    vars.ele1_recHit_E.push_back(iRecHit_E);
    vars.ele1_recHit_hashedIndex.push_back(iRecHit_hashedIndex);
    vars.ele1_recHit_ieta.push_back(iRecHit_ietaORix);
    vars.ele1_recHit_iphi.push_back(iRecHit_iphiORiy);
    vars.ele1_recHit_flag.push_back(iRecHit_flag);
   }  
  }
  
  vars.ele1_scERaw = reader.GetFloat("electrons_scERaw")->at(ele1It);
  vars.ele1_scE = reader.GetFloat("electrons_scE")->at(ele1It);
  vars.ele1_es = reader.GetFloat("electrons_ES")->at(ele1It);
  vars.ele1_fCorrection = reader.GetFloat("electrons_sc_fCorrection")->at(ele1It);
  vars.ele1_scE_regression = reader.GetFloat("electrons_scE_regression")->at(ele1It);
  vars.ele1_scEerr_regression = reader.GetFloat("electrons_scEerr_regression")->at(ele1It);
  vars.ele1_scERaw_PUcleaned = reader.GetFloat("electrons_scERaw_PUcleaned")->at(ele1It);
  vars.ele1_scEtaWidth_PUcleaned = reader.GetFloat("electrons_scEtaWidth_PUcleaned")->at(ele1It);
  vars.ele1_scPhiWidth_PUcleaned = reader.GetFloat("electrons_scPhiWidth_PUcleaned")->at(ele1It);
  vars.ele1_fCorrection_PUcleaned = reader.GetFloat("electrons_sc_fCorrection_PUcleaned")->at(ele1It);

  vars.ele1_bcN = reader.GetInt("electrons_basicClustersSize")->at(ele1It);
  vars.ele1_e3x3 = reader.GetFloat("electrons_e3x3")->at(ele1It);
  vars.ele1_e5x5 = reader.GetFloat("electrons_e5x5")->at(ele1It);

  vars.ele1_tkP = (reader.Get3V("electrons_p_atVtx")->at(ele1It)).R();
  
  vars.ele1_fbrem = reader.GetFloat("electrons_fbrem")->at(ele1It);
  vars.ele1_EOverP = reader.GetFloat("electrons_eOverP")->at(ele1It);
  
  vars.ele1_isEB = reader.GetInt("electrons_isEB")->at(ele1It);
  vars.ele1_isEBEEGap = reader.GetInt("electrons_isEBEEGap")->at(ele1It);
  vars.ele1_isEBEtaGap = reader.GetInt("electrons_isEBEtaGap")->at(ele1It);
  vars.ele1_isEBPhiGap = reader.GetInt("electrons_isEBPhiGap")->at(ele1It);
  vars.ele1_isEEDeeGap = reader.GetInt("electrons_isEEDeeGap")->at(ele1It);
  vars.ele1_isEERingGap = reader.GetInt("electrons_isEERingGap")->at(ele1It);

  vars.ele1_nRecHits = reader.GetInt("recHit_n")->at(ele1It);
  

 }


void SetElectron2Variables(WZAnalysisVariables& vars, treeReader& reader, const int& ele2It, bool isCalib)
{
  vars.p_ele2 = &vars.ele2;

  vars.ele2 = reader.Get4V("electrons")->at(ele2It);
  
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

  vars.ele2_scEtRaw = reader.GetFloat("electrons_scEtRaw")->at(ele2It);
  vars.ele2_scEt = reader.GetFloat("electrons_scEt")->at(ele2It);
  vars.ele2_scEta = reader.GetFloat("electrons_scEta")->at(ele2It);
  vars.ele2_scPhi = reader.GetFloat("electrons_scPhi")->at(ele2It);
  vars.ele2_scLocalEta = reader.GetFloat("electrons_scLocalPositionEtaCry")->at(ele2It);
  vars.ele2_scLocalPhi = reader.GetFloat("electrons_scLocalPositionPhiCry")->at(ele2It);
  vars.ele2_scEtaWidth = reader.GetFloat("electrons_scEtaWidth")->at(ele2It);
  vars.ele2_scPhiWidth = reader.GetFloat("electrons_scPhiWidth")->at(ele2It);
  vars.ele2_scLaserCorr = reader.GetFloat("electrons_scAvgLaserCorrection")->at(ele2It);
  vars.ele2_scCrackCorr = reader.GetFloat("electrons_scCrackCorrection")->at(ele2It);
  vars.ele2_scLocalContCorr = reader.GetFloat("electrons_scLocalContCorrection")->at(ele2It);
   
  
   float var[5];
   SetPhotonMatchingEle( var, reader, ele2It);
   vars.ele2_nPh = (int) var[0];
   vars.ele2_ph_E = var[1];
   vars.ele2_ph_scEta = var[2];
   vars.ele2_ph_scPhi = var[3];
   vars.ele2_ph_R9 = var[4];

   vars.ele2_seedE = reader.GetFloat("electrons_seedE")->at(ele2It);
   vars.ele2_seedIeta = reader.GetInt("electrons_seedIeta")->at(ele2It);
   vars.ele2_seedIphi = reader.GetInt("electrons_seedIphi")->at(ele2It);
   vars.ele2_seedIx = reader.GetInt("electrons_seedIx")->at(ele2It);
   vars.ele2_seedIy = reader.GetInt("electrons_seedIy")->at(ele2It);
   vars.ele2_seedZside = reader.GetInt("electrons_seedZside")->at(ele2It);
   vars.ele2_seedLaserAlpha = reader.GetFloat("electrons_seedLaserAlpha")->at(ele2It);
   vars.ele2_seedLaserCorr = reader.GetFloat("electrons_seedLaserCorrection")->at(ele2It);

   vars.ele2_tkPt = (reader.Get3V("electrons_p_atVtx")->at(ele2It)).Rho();
 
 if(isCalib)
 {
  int theRecHitBeginIndex = 0;
  if ( ele2It == 0 ) theRecHitBeginIndex = 0;
  else 
    for ( int iEle = 0; iEle < ele2It; iEle++ ) theRecHitBeginIndex += reader.GetInt("recHit_n")->at(iEle);
  
  for ( int iRecHit = theRecHitBeginIndex; iRecHit < theRecHitBeginIndex + reader.GetInt("recHit_n")->at(ele2It); iRecHit++ ){
    
    int iRecHit_zside = reader.GetInt("recHit_zside")->at(iRecHit);
    float iRecHit_E = reader.GetFloat("recHit_E")->at(iRecHit);
    int iRecHit_hashedIndex = reader.GetInt("recHit_hashedIndex")->at(iRecHit);
    int iRecHit_flag = reader.GetInt("recHit_flag")->at(iRecHit);
    int iRecHit_ietaORix, iRecHit_iphiORiy;
    if ( iRecHit_zside == 0 ) {
      iRecHit_ietaORix = reader.GetInt("recHit_ieta")->at(iRecHit);
      iRecHit_iphiORiy = reader.GetInt("recHit_iphi")->at(iRecHit);
    }
    else {
      iRecHit_ietaORix = reader.GetInt("recHit_ix")->at(iRecHit);
      iRecHit_iphiORiy = reader.GetInt("recHit_iy")->at(iRecHit);
    }
    vars.ele2_recHit_zside.push_back(iRecHit_zside);
    vars.ele2_recHit_E.push_back(iRecHit_E);
    vars.ele2_recHit_hashedIndex.push_back(iRecHit_hashedIndex);
    vars.ele2_recHit_ieta.push_back(iRecHit_ietaORix);
    vars.ele2_recHit_iphi.push_back(iRecHit_iphiORiy);
    vars.ele2_recHit_flag.push_back(iRecHit_flag);
    }
 }
 
 vars.ele2_scERaw = reader.GetFloat("electrons_scERaw")->at(ele2It);
 vars.ele2_scE = reader.GetFloat("electrons_scE")->at(ele2It);
 vars.ele2_es = reader.GetFloat("electrons_ES")->at(ele2It);
 vars.ele2_fCorrection = reader.GetFloat("electrons_sc_fCorrection")->at(ele2It);
 vars.ele2_scE_regression = reader.GetFloat("electrons_scE_regression")->at(ele2It);
 vars.ele2_scEerr_regression = reader.GetFloat("electrons_scEerr_regression")->at(ele2It);
 vars.ele2_scERaw_PUcleaned = reader.GetFloat("electrons_scERaw_PUcleaned")->at(ele2It);
 vars.ele2_scEtaWidth_PUcleaned = reader.GetFloat("electrons_scEtaWidth_PUcleaned")->at(ele2It);
 vars.ele2_scPhiWidth_PUcleaned = reader.GetFloat("electrons_scPhiWidth_PUcleaned")->at(ele2It);
 vars.ele2_fCorrection_PUcleaned = reader.GetFloat("electrons_sc_fCorrection_PUcleaned")->at(ele2It);
 
 vars.ele2_bcN = reader.GetInt("electrons_basicClustersSize")->at(ele2It);
 vars.ele2_e3x3 = reader.GetFloat("electrons_e3x3")->at(ele2It);
 vars.ele2_e5x5 = reader.GetFloat("electrons_e5x5")->at(ele2It);

 vars.ele2_tkP = (reader.Get3V("electrons_p_atVtx")->at(ele2It)).R();
  
 vars.ele2_fbrem = reader.GetFloat("electrons_fbrem")->at(ele2It);
 vars.ele2_EOverP = reader.GetFloat("electrons_eOverP")->at(ele2It);
  
 vars.ele2_isEB = reader.GetInt("electrons_isEB")->at(ele2It);
 vars.ele2_isEBEEGap = reader.GetInt("electrons_isEBEEGap")->at(ele2It);
 vars.ele2_isEBEtaGap = reader.GetInt("electrons_isEBEtaGap")->at(ele2It);
 vars.ele2_isEBPhiGap = reader.GetInt("electrons_isEBPhiGap")->at(ele2It);
 vars.ele2_isEEDeeGap = reader.GetInt("electrons_isEEDeeGap")->at(ele2It);
 vars.ele2_isEERingGap = reader.GetInt("electrons_isEERingGap")->at(ele2It);

 vars.ele2_nRecHits = reader.GetInt("recHit_n")->at(ele2It);

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
  
  ROOT::Math::PtEtaPhiEVector ele1_sc(vars.ele1_scE*sin(2*atan(exp(-1.*vars.ele1_eta))),vars.ele1_eta,vars.ele1_phi,vars.ele1_scE);
  ROOT::Math::PtEtaPhiEVector ele2_sc(vars.ele2_scE*sin(2*atan(exp(-1.*vars.ele2_eta))),vars.ele2_eta,vars.ele2_phi,vars.ele2_scE);
  vars.ele1ele2_scM = (ele1_sc + ele2_sc).mass();

  ROOT::Math::PtEtaPhiEVector ele1_sc_regression(vars.ele1_scE_regression*sin(2*atan(exp(-1.*vars.ele1_eta))),vars.ele1_eta,vars.ele1_phi,vars.ele1_scE_regression);
  ROOT::Math::PtEtaPhiEVector ele2_sc_regression(vars.ele2_scE_regression*sin(2*atan(exp(-1.*vars.ele2_eta))),vars.ele2_eta,vars.ele2_phi,vars.ele2_scE_regression);
  vars.ele1ele2_scM_regression = (ele1_sc_regression + ele2_sc_regression).mass();
}


void SetPhotonMatchingEle( float* const var, treeReader& reader, const int& eleIt){

  int ele_nPh = 0;
  float ele_ph_E=-999;
  float ele_ph_scEta=-999;
  float ele_ph_scPhi=-999;
  float ele_ph_R9=-999;

  float DrMin = 1000;
  int pItMin = 0;

  for(unsigned int phIt = 0; phIt < (reader.Get4V("photons")->size()); ++phIt)
    {
      ROOT::Math::XYZTVector photon = reader.Get4V("photons")->at(phIt);
      ROOT::Math::XYZTVector photonSC = reader.Get4V("photons_SC")->at(phIt);
      

      float deltaeta = photonSC.eta() - reader.GetFloat("electrons_scEta")->at(eleIt);
      double deltaphi = fabs(photonSC.phi() - reader.GetFloat("electrons_scPhi")->at(eleIt) );
      if (deltaphi > 6.283185308) deltaphi -= 6.283185308;
      if (deltaphi > 3.141592654) deltaphi = 6.283185308 - deltaphi;

      float deltaR = sqrt(deltaeta*deltaeta + deltaphi*deltaphi);
      float eratio = photon.E()/reader.GetFloat("electrons_scE")->at(eleIt);

      if ( deltaR < 0.15 &&  eratio > 0.5 && eratio < 1.5 ){
	ele_nPh += 1; 
	if( deltaR < DrMin){
	  DrMin = deltaR;
	  pItMin = phIt;
	}
      }

    }
  
  if( ele_nPh > 0 ){
    ROOT::Math::XYZTVector thePhoton = reader.Get4V("photons")->at(pItMin);
    ROOT::Math::XYZTVector thePhotonSC = reader.Get4V("photons_SC")->at(pItMin);
    ele_ph_E = thePhoton.E(); 
    ele_ph_scEta = thePhotonSC.eta();
    ele_ph_scPhi = thePhotonSC.phi();
    ele_ph_R9 = reader.GetFloat("photons_r9")->at(pItMin);
  }

  
  var[0] = float (ele_nPh);
  var[1]= ele_ph_E;
  var[2]= ele_ph_scEta;
  var[3]= ele_ph_scPhi;
  var[4]= ele_ph_R9;

}

void SetGenLeptonInformation (WZAnalysisVariables& vars, treeReader& reader, const int & dataFlag, int isWZ)
{
  
 if(dataFlag!=0) return;

 if(isWZ==0)
 {
  if(reader.Get4V("mcF1_fromV")->size() == 1)
  {
   vars.ele1_E_true = reader.Get4V("mcF1_fromV")->at(0).E();
   float Deta = fabs (reader.Get4V("mcF1_fromV")->at(0).Eta()-vars.ele1_eta);
   float Dphi = fabs (reader.Get4V("mcF1_fromV")->at(0).Phi()-vars.ele1_phi);
   if(Dphi>3.141592) Dphi=Dphi-3.141592;
   vars.ele1_DR = sqrt(Deta*Deta+Dphi*Dphi);
  }
 
 }

 if(isWZ==1)
 {
   if(reader.Get4V("mcF1_fromV")->size() == 1 && reader.Get4V("mcF2_fromV")->size() == 1)
   {
     if(vars.ele1_pt>vars.ele2_pt)
     {
          vars.ele1_E_true = reader.Get4V("mcF1_fromV")->at(0).E();
          float Deta = fabs (reader.Get4V("mcF1_fromV")->at(0).Eta()-vars.ele1_eta);
          float Dphi = fabs (reader.Get4V("mcF1_fromV")->at(0).Phi()-vars.ele1_phi);
          if(Dphi>3.141592) Dphi=Dphi-3.141592;
          vars.ele1_DR = sqrt(Deta*Deta+Dphi*Dphi);
          
          vars.ele2_E_true = reader.Get4V("mcF2_fromV")->at(0).E();
          Deta = fabs (reader.Get4V("mcF2_fromV")->at(0).Eta()-vars.ele2_eta);
          Dphi = fabs (reader.Get4V("mcF2_fromV")->at(0).Phi()-vars.ele2_phi);
          if(Dphi>3.141592) Dphi=Dphi-3.141592;
          vars.ele2_DR = sqrt(Deta*Deta+Dphi*Dphi);
     }
     else{
          vars.ele2_E_true = reader.Get4V("mcF1_fromV")->at(0).E();
          float Deta = fabs (reader.Get4V("mcF1_fromV")->at(0).Eta()-vars.ele2_eta);
          float Dphi = fabs (reader.Get4V("mcF1_fromV")->at(0).Phi()-vars.ele2_phi);
          if(Dphi>3.141592) Dphi=Dphi-3.141592;
          vars.ele2_DR = sqrt(Deta*Deta+Dphi*Dphi);
          
          vars.ele1_E_true = reader.Get4V("mcF2_fromV")->at(0).E();
          Deta = fabs (reader.Get4V("mcF2_fromV")->at(0).Eta()-vars.ele1_eta);
          Dphi = fabs (reader.Get4V("mcF2_fromV")->at(0).Phi()-vars.ele1_phi);
          if(Dphi>3.141592) Dphi=Dphi-3.141592;
          vars.ele1_DR = sqrt(Deta*Deta+Dphi*Dphi);

         }

    }
  }

}
