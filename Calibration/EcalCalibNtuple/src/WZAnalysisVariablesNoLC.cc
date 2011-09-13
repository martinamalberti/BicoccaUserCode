#include "Calibration/EcalCalibNtuple/interface/WZAnalysisVariablesNoLC.h"

// DETID
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"


TFile* GetOutputRootFile(WZAnalysisVariablesNoLC& vars)
{
  return vars.m_outputRootFile;
}



void InitializeWZAnalysisTree(WZAnalysisVariablesNoLC& vars, const std::string& outputRootFileName)
{
  //-------------
  // Reduced tree
  //-------------
  
  vars.m_outputRootFile = new TFile(outputRootFileName.c_str(), "RECREATE");  
  
  vars.m_reducedTree = new TTree("ntu", "ntu");
  vars.m_reducedTree -> SetDirectory(vars.m_outputRootFile);
  
  
  // event variables
  vars.m_reducedTree -> Branch("runId",         &vars.runId,                "runId/I");
  vars.m_reducedTree -> Branch("lumiId",        &vars.lumiId,               "lumiId/I");
  vars.m_reducedTree -> Branch("timeStampLow",  &vars.timeStampLow,         "timeStampLow/I");
  vars.m_reducedTree -> Branch("timeStampHigh", &vars.timeStampHigh,        "timeStampHigh/I");

  vars.m_reducedTree -> Branch("isW", &vars.isW, "isW/I");
  vars.m_reducedTree -> Branch("isZ", &vars.isZ, "isZ/I");
  vars.m_reducedTree -> Branch("ele1ele2_scM", &vars.ele1ele2_scM, "ele1ele2_scM/F");
  
  // 1st electron variables
  vars.m_reducedTree -> Branch("ele1_recHit_E",        "std::vector<float>", &vars.ele1_recHit_E);
  vars.m_reducedTree -> Branch("ele1_recHit_ietaORix", "std::vector<int>",   &vars.ele1_recHit_ietaORix);
  vars.m_reducedTree -> Branch("ele1_recHit_iphiORiy", "std::vector<int>",   &vars.ele1_recHit_iphiORiy);
  vars.m_reducedTree -> Branch("ele1_recHit_zside",    "std::vector<int>",   &vars.ele1_recHit_zside);
  
  vars.m_reducedTree -> Branch("ele1_scE",         &vars.ele1_scE,                 "ele1_scE/F");
  vars.m_reducedTree -> Branch("ele1_scEt",        &vars.ele1_scEt,                "ele1_scEt/F");
  vars.m_reducedTree -> Branch("ele1_es",          &vars.ele1_es,                  "ele1_es/F");
  vars.m_reducedTree -> Branch("ele1_r9",          &vars.ele1_r9,                  "ele1_r9/F");
  
  vars.m_reducedTree -> Branch("ele1_tkP",  &vars.ele1_tkP,   "ele1_tkP/F");
  vars.m_reducedTree -> Branch("ele1_eta",  &vars.ele1_eta,   "ele1_eta/F");
  vars.m_reducedTree -> Branch("ele1_phi",  &vars.ele1_phi,   "ele1_phi/F");
  vars.m_reducedTree -> Branch("ele1_seedIetaORix",  &vars.ele1_seedIetaORix,   "ele1_seedIetaORix/I");
  vars.m_reducedTree -> Branch("ele1_seedIphiORiy",  &vars.ele1_seedIphiORiy,   "ele1_seedIphiORiy/I");

  vars.m_reducedTree -> Branch("ele1_EOverP", &vars.ele1_EOverP, "ele1_EOverP/F");
  
  vars.m_reducedTree -> Branch("ele1_isEB",        &vars.ele1_isEB,               "ele1_isEB/I");
  vars.m_reducedTree -> Branch("ele1_isEBEEGap",   &vars.ele1_isEBEEGap,     "ele1_isEBEEGap/I");
  vars.m_reducedTree -> Branch("ele1_isEBEtaGap",  &vars.ele1_isEBEtaGap,   "ele1_isEBEtaGap/I");
  vars.m_reducedTree -> Branch("ele1_isEBPhiGap",  &vars.ele1_isEBPhiGap,   "ele1_isEBPhiGap/I");
  vars.m_reducedTree -> Branch("ele1_isEEDeeGap",  &vars.ele1_isEEDeeGap,   "ele1_isEEDeeGap/I");
  vars.m_reducedTree -> Branch("ele1_isEERingGap", &vars.ele1_isEERingGap, "ele1_isEERingGap/I");
  
  
  // 2nd electron variables  
  vars.m_reducedTree -> Branch("ele2_recHit_E",        "std::vector<float>", &vars.ele2_recHit_E);
  vars.m_reducedTree -> Branch("ele2_recHit_ietaORix", "std::vector<int>",   &vars.ele2_recHit_ietaORix);
  vars.m_reducedTree -> Branch("ele2_recHit_iphiORiy", "std::vector<int>",   &vars.ele2_recHit_iphiORiy);
  vars.m_reducedTree -> Branch("ele2_recHit_zside",    "std::vector<int>",   &vars.ele2_recHit_zside);
  
  vars.m_reducedTree -> Branch("ele2_scE",         &vars.ele2_scE,                 "ele2_scE/F");
  vars.m_reducedTree -> Branch("ele2_scEt",        &vars.ele2_scEt,                "ele2_scEt/F");
  vars.m_reducedTree -> Branch("ele2_es",          &vars.ele2_es,                  "ele2_es/F");
  vars.m_reducedTree -> Branch("ele2_r9",          &vars.ele2_r9,                  "ele2_r9/F");
  
  vars.m_reducedTree -> Branch("ele2_tkP",  &vars.ele2_tkP,   "ele2_tkP/F");
  vars.m_reducedTree -> Branch("ele2_eta",  &vars.ele2_eta,   "ele2_eta/F");
  vars.m_reducedTree -> Branch("ele2_phi",  &vars.ele2_phi,   "ele2_phi/F");
  vars.m_reducedTree -> Branch("ele2_seedIetaORix",  &vars.ele2_seedIetaORix,   "ele2_seedIetaORix/I");
  vars.m_reducedTree -> Branch("ele2_seedIphiORiy",  &vars.ele2_seedIphiORiy,   "ele2_seedIphiORiy/I");

  vars.m_reducedTree -> Branch("ele2_EOverP", &vars.ele2_EOverP, "ele2_EOverP/F");
  
  vars.m_reducedTree -> Branch("ele2_isEB",        &vars.ele2_isEB,               "ele2_isEB/I");
  vars.m_reducedTree -> Branch("ele2_isEBEEGap",   &vars.ele2_isEBEEGap,     "ele2_isEBEEGap/I");
  vars.m_reducedTree -> Branch("ele2_isEBEtaGap",  &vars.ele2_isEBEtaGap,   "ele2_isEBEtaGap/I");
  vars.m_reducedTree -> Branch("ele2_isEBPhiGap",  &vars.ele2_isEBPhiGap,   "ele2_isEBPhiGap/I");
  vars.m_reducedTree -> Branch("ele2_isEEDeeGap",  &vars.ele2_isEEDeeGap,   "ele2_isEEDeeGap/I");
  vars.m_reducedTree -> Branch("ele2_isEERingGap", &vars.ele2_isEERingGap, "ele2_isEERingGap/I");
}



void FillWZAnalysisTree(WZAnalysisVariablesNoLC& vars)
{
  vars.m_reducedTree -> Fill();
}






void ClearWZAnalysisVariables(WZAnalysisVariablesNoLC& vars)
{
  // event variables
  vars.runId = -1;
  vars.lumiId = -1;
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
  
  vars.ele1_sigmaIetaIeta = -99.;
  vars.ele1_DphiIn = -99.;
  vars.ele1_DetaIn = -99.;
  vars.ele1_HOverE = -99.;
  vars.ele1_tkIso = -99.;
  vars.ele1_emIso = -99.;
  vars.ele1_hadIso = -99.;

  vars.ele1_recHit_E.clear();
  vars.ele1_recHit_ietaORix.clear();
  vars.ele1_recHit_iphiORiy.clear();
  vars.ele1_recHit_zside.clear();

  
  vars.ele1_scE = -99.;
  vars.ele1_scEt = -99.;
  vars.ele1_es = -99.;
    
  vars.ele1_tkP = -99.;
  vars.ele1_seedIetaORix = -999;
  vars.ele1_seedIphiORiy = -999;
      
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
  vars.ele2_recHit_ietaORix.clear();
  vars.ele2_recHit_iphiORiy.clear();
  vars.ele2_recHit_zside.clear();

  
  vars.ele2_scE = -99.;
  vars.ele2_scEt = -99.;
  vars.ele2_es = -99.;
    
  vars.ele2_tkP = -99.;
  vars.ele2_seedIetaORix = -999;
  vars.ele2_seedIphiORiy = -999;
      
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



void DeleteWZAnalysisVariables(WZAnalysisVariablesNoLC& vars)
{
  // save tree
  vars.m_outputRootFile -> cd();
  vars.m_reducedTree -> Write();
  vars.m_outputRootFile -> Close();
}

void SetElectron1Variables(WZAnalysisVariablesNoLC& vars, treeReader& reader, const int& ele1It)
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
    for ( int iEle = 0; iEle < ele1It; iEle++ ) theRecHitBeginIndex += reader.GetInt("recHit_n")->at(iEle);
  
  for ( int iRecHit = theRecHitBeginIndex; iRecHit < theRecHitBeginIndex + reader.GetInt("recHit_n")->at(ele1It); iRecHit++ ){
        
    int iRecHit_zside = reader.GetInt("recHit_zside")->at(iRecHit);
    
    //deconcolve the LC
    float thisRhLC = reader.GetFloat("recHit_laserCorrection")->at(iRecHit);
    float iRecHit_E = reader.GetFloat("recHit_E")->at(iRecHit)/thisRhLC;
    int iRecHit_ietaORix, irecHit_iphiORiy;
    if ( iRecHit_zside == 0 ) {
      iRecHit_ietaORix = reader.GetInt("recHit_ieta")->at(iRecHit);
      irecHit_iphiORiy = reader.GetInt("recHit_iphi")->at(iRecHit);
    }
    else {
      iRecHit_ietaORix = reader.GetInt("recHit_ix")->at(iRecHit);
      irecHit_iphiORiy = reader.GetInt("recHit_iy")->at(iRecHit);
    }
    
    if ( iRecHit_zside == 0 ) {
      EBDetId thisDetId(iRecHit_ietaORix, irecHit_iphiORiy, 0);
    }
    else {
      EEDetId thisDetId(iRecHit_ietaORix, irecHit_iphiORiy, iRecHit_zside, 0);
    }
    


    
    vars.ele1_recHit_zside.push_back(iRecHit_zside);
    vars.ele1_recHit_E.push_back(iRecHit_E);
    vars.ele1_recHit_ietaORix.push_back(iRecHit_ietaORix);
    vars.ele1_recHit_iphiORiy.push_back(irecHit_iphiORiy);

  }
    
  //deconcolve the LC
  float thisScLC = reader.GetFloat("electrons_scAvgLaserCorrection")->at(ele1It);
  vars.ele1_scE = reader.GetFloat("electrons_scE")->at(ele1It)/thisScLC;
  vars.ele1_scEt = reader.GetFloat("electrons_scEt")->at(ele1It)/thisScLC;
  vars.ele1_EOverP = reader.GetFloat("electrons_eOverP")->at(ele1It)/thisScLC;  
  
  vars.ele1_es = reader.GetFloat("electrons_ES")->at(ele1It);
  vars.ele1_r9 = reader.GetFloat("electrons_seedE")->at(ele1It)/reader.GetFloat("electrons_e3x3")->at(ele1It);
      
  vars.ele1_isEB = reader.GetInt("electrons_isEB")->at(ele1It);
  vars.ele1_isEBEEGap = reader.GetInt("electrons_isEBEEGap")->at(ele1It);
  vars.ele1_isEBEtaGap = reader.GetInt("electrons_isEBEtaGap")->at(ele1It);
  vars.ele1_isEBPhiGap = reader.GetInt("electrons_isEBPhiGap")->at(ele1It);
  vars.ele1_isEEDeeGap = reader.GetInt("electrons_isEEDeeGap")->at(ele1It);
  vars.ele1_isEERingGap = reader.GetInt("electrons_isEERingGap")->at(ele1It);

  vars.ele1_tkP = (reader.Get3V("electrons_p_atVtx")->at(ele1It)).R();
  if ( reader.GetInt("electrons_seedZside")->at(ele1It) == 0 ) {
    vars.ele1_seedIetaORix = reader.GetInt("electrons_seedIeta")->at(ele1It);
    vars.ele1_seedIphiORiy = reader.GetInt("electrons_seedIphi")->at(ele1It);
  }
  else {
    vars.ele1_seedIetaORix = reader.GetInt("electrons_seedIx")->at(ele1It);
    vars.ele1_seedIphiORiy = reader.GetInt("electrons_seedIy")->at(ele1It);
  }

}


void SetElectron2Variables(WZAnalysisVariablesNoLC& vars, treeReader& reader, const int& ele2It)
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
    for ( int iEle = 0; iEle < ele2It; iEle++ ) theRecHitBeginIndex += reader.GetInt("recHit_n")->at(iEle);
  
  for ( int iRecHit = theRecHitBeginIndex; iRecHit < theRecHitBeginIndex + reader.GetInt("recHit_n")->at(ele2It); iRecHit++ ){
    
    int iRecHit_zside = reader.GetInt("recHit_zside")->at(iRecHit);
    //deconcolve the LC
    float thisRhLC = reader.GetFloat("recHit_laserCorrection")->at(iRecHit);
    float iRecHit_E = reader.GetFloat("recHit_E")->at(iRecHit)/thisRhLC;
    int iRecHit_ietaORix, irecHit_iphiORiy;
    if ( iRecHit_zside == 0 ) {
      iRecHit_ietaORix = reader.GetInt("recHit_ieta")->at(iRecHit);
      irecHit_iphiORiy = reader.GetInt("recHit_iphi")->at(iRecHit);
    }
    else {
      iRecHit_ietaORix = reader.GetInt("recHit_ix")->at(iRecHit);
      irecHit_iphiORiy = reader.GetInt("recHit_iy")->at(iRecHit);
    }
    vars.ele2_recHit_zside.push_back(iRecHit_zside);
    vars.ele2_recHit_E.push_back(iRecHit_E);
    vars.ele2_recHit_ietaORix.push_back(iRecHit_ietaORix);
    vars.ele2_recHit_iphiORiy.push_back(irecHit_iphiORiy);

  }  

  //deconcolve the LC
  float thisScLC = reader.GetFloat("electrons_scAvgLaserCorrection")->at(ele2It);
  vars.ele2_scE = reader.GetFloat("electrons_scE")->at(ele2It)/thisScLC;
  vars.ele2_scEt = reader.GetFloat("electrons_scEt")->at(ele2It)/thisScLC;
  vars.ele2_EOverP = reader.GetFloat("electrons_eOverP")->at(ele2It)/thisScLC;

  vars.ele2_es = reader.GetFloat("electrons_ES")->at(ele2It);
  vars.ele2_r9 = reader.GetFloat("electrons_seedE")->at(ele2It)/reader.GetFloat("electrons_e3x3")->at(ele2It);

  vars.ele2_isEB = reader.GetInt("electrons_isEB")->at(ele2It);
  vars.ele2_isEBEEGap = reader.GetInt("electrons_isEBEEGap")->at(ele2It);
  vars.ele2_isEBEtaGap = reader.GetInt("electrons_isEBEtaGap")->at(ele2It);
  vars.ele2_isEBPhiGap = reader.GetInt("electrons_isEBPhiGap")->at(ele2It);
  vars.ele2_isEEDeeGap = reader.GetInt("electrons_isEEDeeGap")->at(ele2It);
  vars.ele2_isEERingGap = reader.GetInt("electrons_isEERingGap")->at(ele2It);

  vars.ele2_tkP = (reader.Get3V("electrons_p_atVtx")->at(ele2It)).R();
  if ( reader.GetInt("electrons_seedZside")->at(ele2It) == 0 ) {
    vars.ele2_seedIetaORix = reader.GetInt("electrons_seedIeta")->at(ele2It);
    vars.ele2_seedIphiORiy = reader.GetInt("electrons_seedIphi")->at(ele2It);
  }
  else {
    vars.ele2_seedIetaORix = reader.GetInt("electrons_seedIx")->at(ele2It);
    vars.ele2_seedIphiORiy = reader.GetInt("electrons_seedIy")->at(ele2It);
  }
  
}

void SetMetVariables(WZAnalysisVariablesNoLC& vars, treeReader& reader)
{
  vars.met = reader.Get4V("PFMet")->at(0);
  vars.p_met = &vars.met;
  vars.met_et = vars.p_met->Et();
  vars.met_phi = vars.p_met->phi();
  
  vars.ele1Met_mt = sqrt( 2. * vars.ele1_pt * vars.met_et * ( 1 - cos( deltaPhi(vars.ele1_phi,vars.met_phi) ) ) );
  vars.ele1Met_Dphi = deltaPhi(vars.ele1_phi,vars.met_phi);
}



void SetDiElectronVariables(WZAnalysisVariablesNoLC& vars, treeReader& reader, const int& ele1It, const int& ele2It)
{
  vars.ele1ele2_m = (vars.ele1 + vars.ele2).mass();
  
  ROOT::Math::PtEtaPhiEVector ele1_sc(vars.ele1_scE*sin(2*atan(exp(-1.*vars.ele1_eta))),vars.ele1_eta,vars.ele1_phi,vars.ele1_scE);
  ROOT::Math::PtEtaPhiEVector ele2_sc(vars.ele2_scE*sin(2*atan(exp(-1.*vars.ele2_eta))),vars.ele2_eta,vars.ele2_phi,vars.ele2_scE);
  //Deconvolve the LC and save the mass
  float theMassCorrection = sqrt(
      reader.GetFloat("electrons_scAvgLaserCorrection")->at(ele1It)*
      reader.GetFloat("electrons_scAvgLaserCorrection")->at(ele2It));
  vars.ele1ele2_scM = (ele1_sc + ele2_sc).mass()/theMassCorrection;
}
