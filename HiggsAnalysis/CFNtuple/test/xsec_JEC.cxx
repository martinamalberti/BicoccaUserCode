//=====================================================================-*-C++-*-
// File and Version Information:
//      $Id: RooUnfoldExample.cxx 248 2010-10-04 22:18:19Z T.J.Adye $
//
// Description:
//      Simple example usage of the RooUnfold package using toy MC.
//
// Authors: Tim Adye <T.J.Adye@rl.ac.uk> and Fergus Wilson <fwilson@slac.stanford.edu>
//
//==============================================================================

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <iostream>
using std::cout;
using std::endl;

#include "TRandom.h"
#include "TH1D.h"

#include "RooUnfoldResponse.h"
#include "RooUnfoldBayes.h"
#include "RooUnfoldSvd.h"
#include "RooUnfoldBinByBin.h"

#endif

//#include "tdrStyle.cc"

//==============================================================================
// Global definitions
//==============================================================================

//==============================================================================
// Unfolding for C-F jet analysis
//==============================================================================

// void CrossSection()
{
  
  gSystem->Load("libFWCoreFWLite.so");
  AutoLibraryLoader::enable();
  JetCorrectionUncertainty *jecUnc = new JetCorrectionUncertainty("/home/toliman/Dropbox/QCD_CF/START38_V13_Uncertainty_AK5Calo.txt");
  
  Float_t lowEdge[8] = {35,45,57,72,90,120,150,1000};
  const int NBIN = 7;
  
   TFile FileTrain1Pythia("~/Dropbox/QCD_CF/NtupleBo/ntuple/qcd_15_pythia.root","READ");
   TFile FileTrain2Pythia("~/Dropbox/QCD_CF/NtupleBo/ntuple/qcd_30_pythia.root","READ");
   
   TFile FileTrain3Pythia("~/Dropbox/QCD_CF/NtupleBo/ntuple/qcd_15_pythia_TuneZ2.root","READ");


//   TFile FileTrain1Herwig("../Unfolding/qcd_15_herwigjimmy.root","READ");
//   TFile FileTrain2Herwig("../Unfolding/qcd_30_herwigjimmy.root","READ");

  TFile FileTrain1Herwig("~/Dropbox/QCD_CF/NtupleBo/ntuple/qcd_15_herwigjimmy.root","READ");
  TFile FileTrain2Herwig("~/Dropbox/QCD_CF/NtupleBo/ntuple/qcd_30_herwigjimmy.root","READ");
  
  TFile FileTrainPOWPY("~/Dropbox/QCD_CF/NtupleMiBi/Powheg/POWHEG_kt15_v1.root","READ");
  TFile FileTrainPOWHE("~/Dropbox/QCD_CF/NtupleMiBi/Powheg/POWHEG_HERWIG6_kt15_A.root","READ");
//    TFile FileTrainPOWHE("~/Dropbox/QCD_CF/NtupleMiBi/Powheg/POWHEG_HERWIG6_kt15_cut35_oldNtuple.root","READ");


  double lumi = 0.0426;
  
  double xsec1Pythia = 8.762e8 / 6190500.;
  double xsec2Pythia = 6.041e7 / 4918016.;
  
  double xsec1Herwig = 714000000. / 1631667.;
  double xsec2Herwig =  49240000. / 1310829.;
  
  double xsecPowheg =  900054123. / 15999208.;		//POWHEG + PYTHIA6  900054123. 
  double xsecPowheg7 = 900054123./  9000000.;		//POWHEG + HERWIG6
  
//   double xsecPowheg7 = 1.07846619e10 /2000000.;
  
  double Threshold_G_FJet = 57;
  double Threshold_G_CJet = 57;
  
  double etaF = 1/3.;
  double etaC = 1/5.6;
  
  TTree* TreeTrain1Pythia = (TTree*) FileTrain1Pythia.Get("AnaHiggs");
  TTree* TreeTrain2Pythia = (TTree*) FileTrain2Pythia.Get("AnaHiggs");
  TTree* TreeTrain3Pythia = (TTree*) FileTrain3Pythia.Get("AnaHiggs");
 
  TTree* TreeTrain1Herwig = (TTree*) FileTrain1Herwig.Get("AnaHiggs");
  TTree* TreeTrain2Herwig = (TTree*) FileTrain2Herwig.Get("AnaHiggs");
  
  TTree* TreeTrainPOWPY = (TTree*) FileTrainPOWPY.Get("AnaHiggs");
  TTree* TreeTrainPOWHE = (TTree*) FileTrainPOWHE.Get("AnaHiggs");
  
  Float_t G_FJet_Pt; //~~~~ had
  Float_t S_FJet_Pt; //~~~~ reco

  Float_t G_CJet_Pt; //~~~~ had
  Float_t S_CJet_Pt; //~~~~ reco
  
  Double_t G_FJet_Pt_P; //~~~~ had
  Double_t G_CJet_Pt_P; //~~~~ had
 
 TreeTrain1Pythia->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt);
 TreeTrain1Pythia->SetBranchAddress("S_FJet_Pt",&S_FJet_Pt);
 TreeTrain1Pythia->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt);
 TreeTrain1Pythia->SetBranchAddress("S_CJet_Pt",&S_CJet_Pt);

 TreeTrain2Pythia->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt);
 TreeTrain2Pythia->SetBranchAddress("S_FJet_Pt",&S_FJet_Pt);
 TreeTrain2Pythia->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt);
 TreeTrain2Pythia->SetBranchAddress("S_CJet_Pt",&S_CJet_Pt);
 
 TreeTrain3Pythia->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt);	// pythia Z2 Tune
 TreeTrain3Pythia->SetBranchAddress("S_FJet_Pt",&S_FJet_Pt);
 TreeTrain3Pythia->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt);
 TreeTrain3Pythia->SetBranchAddress("S_CJet_Pt",&S_CJet_Pt);
 
 TreeTrain1Herwig->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt);
 TreeTrain1Herwig->SetBranchAddress("S_FJet_Pt",&S_FJet_Pt);
 TreeTrain1Herwig->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt);
 TreeTrain1Herwig->SetBranchAddress("S_CJet_Pt",&S_CJet_Pt);

 TreeTrain2Herwig->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt);
 TreeTrain2Herwig->SetBranchAddress("S_FJet_Pt",&S_FJet_Pt);
 TreeTrain2Herwig->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt);
 TreeTrain2Herwig->SetBranchAddress("S_CJet_Pt",&S_CJet_Pt);
 
 TreeTrainPOWPY->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt_P);
 TreeTrainPOWPY->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt_P);

 TreeTrainPOWHE->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt_P);
 TreeTrainPOWHE->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt_P);
 
 
 /*
cout << "==================================== POWHEG + PYTHIA Distributions ===================================" << endl;

  TH1D* hTrueFJet_MCPowheg = new TH1D ("hTrueFJet_MCPowheg", "MC Truth FJet Powheg",    NBIN , lowEdge);
  TH1D* hTrueCJet_MCPowheg = new TH1D ("hTrueCJet_MCPowheg", "MC Truth CJet Powheg",    NBIN , lowEdge);
   
  for (Int_t iEvt= 0; iEvt<TreeTrainPOWPY->GetEntries(); iEvt++) {
   TreeTrainPOWPY->GetEntry(iEvt);
   //    std::cerr << " S_FJet_Pt = " << S_FJet_Pt << std::endl;
   if (G_CJet_Pt_P>35 && G_FJet_Pt_P>35) {
    
     hTrueFJet_MCPowheg->Fill(G_FJet_Pt_P);
     hTrueCJet_MCPowheg->Fill(G_CJet_Pt_P);
   }
  }  
  
   
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hTrueFJet_MCPowheg->SetBinError(iBinX+1,hTrueFJet_MCPowheg->GetBinError(iBinX+1) * xsecPowheg);
   hTrueCJet_MCPowheg->SetBinError(iBinX+1,hTrueCJet_MCPowheg->GetBinError(iBinX+1) * xsecPowheg); 
  
   hTrueFJet_MCPowheg->SetBinContent(iBinX+1,hTrueFJet_MCPowheg->GetBinContent(iBinX+1) * xsecPowheg);
   hTrueCJet_MCPowheg->SetBinContent(iBinX+1,hTrueCJet_MCPowheg->GetBinContent(iBinX+1) * xsecPowheg);
  }
  
   for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hTrueFJet_MCPowheg->SetBinError(iBinX+1,hTrueFJet_MCPowheg->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX])*etaF);
   hTrueCJet_MCPowheg->SetBinError(iBinX+1,hTrueCJet_MCPowheg->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX])*etaC);
   
   hTrueFJet_MCPowheg->SetBinContent(iBinX+1,hTrueFJet_MCPowheg->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX])*etaF);
   hTrueCJet_MCPowheg->SetBinContent(iBinX+1,hTrueCJet_MCPowheg->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX])*etaC);
   
  }
  
  
 */
 /*
cout << "==================================== POWHEG + HERWIG Distributions ===================================" << endl;

  TH1D* hTrueFJet_MCPowheg7 = new TH1D ("hTrueFJet_MCPowheg7", "MC Truth FJet Powheg7",    NBIN , lowEdge);
  TH1D* hTrueCJet_MCPowheg7 = new TH1D ("hTrueCJet_MCPowheg7", "MC Truth CJet Powheg7",    NBIN , lowEdge);
   
  for (Int_t iEvt= 0; iEvt<TreeTrainPOWHE->GetEntries(); iEvt++) {
    for (Int_t iEvt= 0; iEvt<TreeTrainPOWHE->GetEntries(); iEvt++) {
   TreeTrainPOWHE->GetEntry(iEvt);
   //    std::cerr << " S_FJet_Pt = " << S_FJet_Pt << std::endl;
   if (G_CJet_Pt_P>35 && G_FJet_Pt_P>35) {
    
     hTrueFJet_MCPowheg7->Fill(G_FJet_Pt_P);
     hTrueCJet_MCPowheg7->Fill(G_CJet_Pt_P);
   }
  }  
  
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hTrueFJet_MCPowheg7->SetBinError(iBinX+1,hTrueFJet_MCPowheg7->GetBinError(iBinX+1) * xsecPowheg7);
   hTrueCJet_MCPowheg7->SetBinError(iBinX+1,hTrueCJet_MCPowheg7->GetBinError(iBinX+1) * xsecPowheg7); 
  
   hTrueFJet_MCPowheg7->SetBinContent(iBinX+1,hTrueFJet_MCPowheg7->GetBinContent(iBinX+1) * xsecPowheg7);
   hTrueCJet_MCPowheg7->SetBinContent(iBinX+1,hTrueCJet_MCPowheg7->GetBinContent(iBinX+1) * xsecPowheg7);
  }
  
   for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hTrueFJet_MCPowheg7->SetBinError(iBinX+1,hTrueFJet_MCPowheg7->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX])*etaF);
   hTrueCJet_MCPowheg7->SetBinError(iBinX+1,hTrueCJet_MCPowheg7->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX])*etaC);
   
   hTrueFJet_MCPowheg7->SetBinContent(iBinX+1,hTrueFJet_MCPowheg7->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX])*etaF);
   hTrueCJet_MCPowheg7->SetBinContent(iBinX+1,hTrueCJet_MCPowheg7->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX])*etaC);
   
  }
   */   
  
  cout << "==================================== MC Distributions Pythia =====================================" << endl;
    
  TH1D* hTrueFJet_MC_tempPythia = new TH1D ("hTrueFJet_MC_tempPythia", "MC Truth FJet Pythia",    NBIN , lowEdge);
  TH1D* hMeasFJet_MC_tempPythia = new TH1D ("hMeasFJet_MC_tempPythia", "MC Measured FJet Pythia", NBIN , lowEdge);
  TH1D* hTrueCJet_MC_tempPythia = new TH1D ("hTrueCJet_MC_tempPythia", "MC Truth CJet Pythia",    NBIN , lowEdge);
  TH1D* hMeasCJet_MC_tempPythia = new TH1D ("hMeasCJet_MC_tempPythia", "MC Measured CJet Pythia", NBIN , lowEdge);

  TH1D* hTrueFJet_MCPythia = new TH1D ("hTrueFJet_MCPythia", "MC Truth FJet Pythia",    NBIN , lowEdge);
  TH1D* hMeasFJet_MCPythia = new TH1D ("hMeasFJet_MCPythia", "MC Measured FJet Pythia", NBIN , lowEdge);
  TH1D* hTrueCJet_MCPythia = new TH1D ("hTrueCJet_MCPythia", "MC Truth CJet Pythia",    NBIN , lowEdge);
  TH1D* hMeasCJet_MCPythia = new TH1D ("hMeasCJet_MCPythia", "MC Measured CJet Pythia", NBIN , lowEdge);
  
  for (Int_t iEvt= 0; iEvt<TreeTrain1Pythia->GetEntries(); iEvt++) {
   TreeTrain1Pythia->GetEntry(iEvt);
   //    std::cerr << " S_FJet_Pt = " << S_FJet_Pt << std::endl;
      if ( G_CJet_Pt>35 && G_FJet_Pt>35 ) {	//S_FJet_Pt>0 && S_CJet_Pt>0 &&
//     if ( S_CJet_Pt>35 && S_FJet_Pt>35 ) {	//S_FJet_Pt>0 && S_CJet_Pt>0 &&
//     if (S_CJet_Pt>35 && S_FJet_Pt>35 && S_FJet_Pt>60) {
    if (G_FJet_Pt < Threshold_G_FJet) {
     hTrueFJet_MC_tempPythia->Fill(G_FJet_Pt);
     hMeasFJet_MC_tempPythia->Fill(S_FJet_Pt);
    }
    if (G_CJet_Pt < Threshold_G_CJet) {
     hTrueCJet_MC_tempPythia->Fill(G_CJet_Pt);
     hMeasCJet_MC_tempPythia->Fill(S_CJet_Pt);
    }
   }
  }
  
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hTrueFJet_MC_tempPythia->SetBinError(iBinX+1,hTrueFJet_MC_tempPythia->GetBinError(iBinX+1) * xsec1Pythia);
   hTrueCJet_MC_tempPythia->SetBinError(iBinX+1,hTrueCJet_MC_tempPythia->GetBinError(iBinX+1) * xsec1Pythia); 
   hMeasFJet_MC_tempPythia->SetBinError(iBinX+1,hMeasFJet_MC_tempPythia->GetBinError(iBinX+1) * xsec1Pythia);
   hMeasCJet_MC_tempPythia->SetBinError(iBinX+1,hMeasCJet_MC_tempPythia->GetBinError(iBinX+1) * xsec1Pythia);

   hTrueFJet_MC_tempPythia->SetBinContent(iBinX+1,hTrueFJet_MC_tempPythia->GetBinContent(iBinX+1) * xsec1Pythia);
   hTrueCJet_MC_tempPythia->SetBinContent(iBinX+1,hTrueCJet_MC_tempPythia->GetBinContent(iBinX+1) * xsec1Pythia);
   hMeasFJet_MC_tempPythia->SetBinContent(iBinX+1,hMeasFJet_MC_tempPythia->GetBinContent(iBinX+1) * xsec1Pythia);
   hMeasCJet_MC_tempPythia->SetBinContent(iBinX+1,hMeasCJet_MC_tempPythia->GetBinContent(iBinX+1) * xsec1Pythia);   
  }
  
  
  for (Int_t iEvt= 0; iEvt<TreeTrain2Pythia->GetEntries(); iEvt++) {
   TreeTrain2Pythia->GetEntry(iEvt);
   //    std::cerr << " S_FJet_Pt = " << S_FJet_Pt << std::endl;
      if ( G_CJet_Pt>35 && G_FJet_Pt>35 ) {	//S_FJet_Pt>0 && S_CJet_Pt>0 &&
//     if ( S_CJet_Pt>35 && S_FJet_Pt>35 && S_FJet_Pt>60) {
//     if ( S_CJet_Pt>35 && S_FJet_Pt>35 ) {
    if (G_FJet_Pt > Threshold_G_FJet) {
     hTrueFJet_MCPythia->Fill(G_FJet_Pt);
     hMeasFJet_MCPythia->Fill(S_FJet_Pt);
    }
    if (G_CJet_Pt > Threshold_G_CJet) {
     hTrueCJet_MCPythia->Fill(G_CJet_Pt);
     hMeasCJet_MCPythia->Fill(S_CJet_Pt);
    }
   }
  }  
  
   
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hTrueFJet_MCPythia->SetBinError(iBinX+1,hTrueFJet_MCPythia->GetBinError(iBinX+1) * xsec2Pythia);
   hTrueCJet_MCPythia->SetBinError(iBinX+1,hTrueCJet_MCPythia->GetBinError(iBinX+1) * xsec2Pythia); 
   hMeasFJet_MCPythia->SetBinError(iBinX+1,hMeasFJet_MCPythia->GetBinError(iBinX+1) * xsec2Pythia);
   hMeasCJet_MCPythia->SetBinError(iBinX+1,hMeasCJet_MCPythia->GetBinError(iBinX+1) * xsec2Pythia);

   hTrueFJet_MCPythia->SetBinContent(iBinX+1,hTrueFJet_MCPythia->GetBinContent(iBinX+1) * xsec2Pythia);
   hTrueCJet_MCPythia->SetBinContent(iBinX+1,hTrueCJet_MCPythia->GetBinContent(iBinX+1) * xsec2Pythia);
   hMeasFJet_MCPythia->SetBinContent(iBinX+1,hMeasFJet_MCPythia->GetBinContent(iBinX+1) * xsec2Pythia);
   hMeasCJet_MCPythia->SetBinContent(iBinX+1,hMeasCJet_MCPythia->GetBinContent(iBinX+1) * xsec2Pythia);   
  }

  hTrueFJet_MCPythia->Sumw2();
  hTrueCJet_MCPythia->Sumw2();
  hMeasFJet_MCPythia->Sumw2();
  hMeasCJet_MCPythia->Sumw2();

  hTrueFJet_MCPythia->Add(hTrueFJet_MC_tempPythia);
  hTrueCJet_MCPythia->Add(hTrueCJet_MC_tempPythia);
  hMeasFJet_MCPythia->Add(hMeasFJet_MC_tempPythia);
  hMeasCJet_MCPythia->Add(hMeasCJet_MC_tempPythia);
  
  
   for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hTrueFJet_MCPythia->SetBinError(iBinX+1,hTrueFJet_MCPythia->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX])*etaF);
   hTrueCJet_MCPythia->SetBinError(iBinX+1,hTrueCJet_MCPythia->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX])*etaC);
   hMeasFJet_MCPythia->SetBinError(iBinX+1,hMeasFJet_MCPythia->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX])*etaF);
   hMeasCJet_MCPythia->SetBinError(iBinX+1,hMeasCJet_MCPythia->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX])*etaC);  
  
   hTrueFJet_MCPythia->SetBinContent(iBinX+1,hTrueFJet_MCPythia->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX])*etaF);
   hTrueCJet_MCPythia->SetBinContent(iBinX+1,hTrueCJet_MCPythia->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX])*etaC);
   hMeasFJet_MCPythia->SetBinContent(iBinX+1,hMeasFJet_MCPythia->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX])*etaF);
   hMeasCJet_MCPythia->SetBinContent(iBinX+1,hMeasCJet_MCPythia->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX])*etaC);       
  }
  
  
  
  cout << "==================================== MC Distributions Herwig =====================================" << endl;
    
  TH1D* hTrueFJet_MC_tempHerwig = new TH1D ("hTrueFJet_MC_tempHerwig", "MC Truth FJet Herwig",    NBIN , lowEdge);
  TH1D* hMeasFJet_MC_tempHerwig = new TH1D ("hMeasFJet_MC_tempHerwig", "MC Measured FJet Herwig", NBIN , lowEdge);
  TH1D* hTrueCJet_MC_tempHerwig = new TH1D ("hTrueCJet_MC_tempHerwig", "MC Truth CJet Herwig",    NBIN , lowEdge);
  TH1D* hMeasCJet_MC_tempHerwig = new TH1D ("hMeasCJet_MC_tempHerwig", "MC Measured CJet Herwig", NBIN , lowEdge);

  TH1D* hTrueFJet_MCHerwig = new TH1D ("hTrueFJet_MCHerwig", "MC Truth FJet Herwig",    NBIN , lowEdge);
  TH1D* hMeasFJet_MCHerwig = new TH1D ("hMeasFJet_MCHerwig", "MC Measured FJet Herwig", NBIN , lowEdge);
  TH1D* hTrueCJet_MCHerwig = new TH1D ("hTrueCJet_MCHerwig", "MC Truth CJet Herwig",    NBIN , lowEdge);
  TH1D* hMeasCJet_MCHerwig = new TH1D ("hMeasCJet_MCHerwig", "MC Measured CJet Herwig", NBIN , lowEdge);
  
  for (Int_t iEvt= 0; iEvt<TreeTrain1Herwig->GetEntries(); iEvt++) {
   TreeTrain1Herwig->GetEntry(iEvt);
   //    std::cerr << " S_FJet_Pt = " << S_FJet_Pt << std::endl;
//     if ( S_CJet_Pt>35 && S_FJet_Pt>35 && S_FJet_Pt>60) {
     if ( G_CJet_Pt>35 && G_FJet_Pt>35) {
//      if ( S_CJet_Pt>35 && S_FJet_Pt>35) {
    if (G_FJet_Pt < Threshold_G_FJet) {
     hTrueFJet_MC_tempHerwig->Fill(G_FJet_Pt);
     hMeasFJet_MC_tempHerwig->Fill(S_FJet_Pt);
    }
    if (G_CJet_Pt < Threshold_G_CJet) {
     hTrueCJet_MC_tempHerwig->Fill(G_CJet_Pt);
     hMeasCJet_MC_tempHerwig->Fill(S_CJet_Pt);
    }
   }
  }
  
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hTrueFJet_MC_tempHerwig->SetBinError(iBinX+1,hTrueFJet_MC_tempHerwig->GetBinError(iBinX+1) * xsec1Herwig);
   hTrueCJet_MC_tempHerwig->SetBinError(iBinX+1,hTrueCJet_MC_tempHerwig->GetBinError(iBinX+1) * xsec1Herwig); 
   hMeasFJet_MC_tempHerwig->SetBinError(iBinX+1,hMeasFJet_MC_tempHerwig->GetBinError(iBinX+1) * xsec1Herwig);
   hMeasCJet_MC_tempHerwig->SetBinError(iBinX+1,hMeasCJet_MC_tempHerwig->GetBinError(iBinX+1) * xsec1Herwig);

   hTrueFJet_MC_tempHerwig->SetBinContent(iBinX+1,hTrueFJet_MC_tempHerwig->GetBinContent(iBinX+1) * xsec1Herwig);
   hTrueCJet_MC_tempHerwig->SetBinContent(iBinX+1,hTrueCJet_MC_tempHerwig->GetBinContent(iBinX+1) * xsec1Herwig);
   hMeasFJet_MC_tempHerwig->SetBinContent(iBinX+1,hMeasFJet_MC_tempHerwig->GetBinContent(iBinX+1) * xsec1Herwig);
   hMeasCJet_MC_tempHerwig->SetBinContent(iBinX+1,hMeasCJet_MC_tempHerwig->GetBinContent(iBinX+1) * xsec1Herwig);   
  }
  
  
  for (Int_t iEvt= 0; iEvt<TreeTrain2Herwig->GetEntries(); iEvt++) {
   TreeTrain2Herwig->GetEntry(iEvt);
   //    std::cerr << " S_FJet_Pt = " << S_FJet_Pt << std::endl;
//     if (S_CJet_Pt>35 && S_FJet_Pt>35 && S_FJet_Pt>60) {	//S_FJet_Pt>0 && S_CJet_Pt>0 && 
     if ( G_CJet_Pt>35 && G_FJet_Pt>35) {
//    if ( S_CJet_Pt>35 && S_FJet_Pt>35) {
    if (G_FJet_Pt > Threshold_G_FJet) {
     hTrueFJet_MCHerwig->Fill(G_FJet_Pt);
     hMeasFJet_MCHerwig->Fill(S_FJet_Pt);
    }
    if (G_CJet_Pt > Threshold_G_CJet) {
     hTrueCJet_MCHerwig->Fill(G_CJet_Pt);
     hMeasCJet_MCHerwig->Fill(S_CJet_Pt);
    }
   }
  }  
  
   
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hTrueFJet_MCHerwig->SetBinError(iBinX+1,hTrueFJet_MCHerwig->GetBinError(iBinX+1) * xsec2Herwig);
   hTrueCJet_MCHerwig->SetBinError(iBinX+1,hTrueCJet_MCHerwig->GetBinError(iBinX+1) * xsec2Herwig); 
   hMeasFJet_MCHerwig->SetBinError(iBinX+1,hMeasFJet_MCHerwig->GetBinError(iBinX+1) * xsec2Herwig);
   hMeasCJet_MCHerwig->SetBinError(iBinX+1,hMeasCJet_MCHerwig->GetBinError(iBinX+1) * xsec2Herwig);

   hTrueFJet_MCHerwig->SetBinContent(iBinX+1,hTrueFJet_MCHerwig->GetBinContent(iBinX+1) * xsec2Herwig);
   hTrueCJet_MCHerwig->SetBinContent(iBinX+1,hTrueCJet_MCHerwig->GetBinContent(iBinX+1) * xsec2Herwig);
   hMeasFJet_MCHerwig->SetBinContent(iBinX+1,hMeasFJet_MCHerwig->GetBinContent(iBinX+1) * xsec2Herwig);
   hMeasCJet_MCHerwig->SetBinContent(iBinX+1,hMeasCJet_MCHerwig->GetBinContent(iBinX+1) * xsec2Herwig);   
  }

  hTrueFJet_MCHerwig->Sumw2();
  hTrueCJet_MCHerwig->Sumw2();
  hMeasFJet_MCHerwig->Sumw2();
  hMeasCJet_MCHerwig->Sumw2();

  hTrueFJet_MCHerwig->Add(hTrueFJet_MC_tempHerwig);
  hTrueCJet_MCHerwig->Add(hTrueCJet_MC_tempHerwig);
  hMeasFJet_MCHerwig->Add(hMeasFJet_MC_tempHerwig);
  hMeasCJet_MCHerwig->Add(hMeasCJet_MC_tempHerwig);
  
   for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hTrueFJet_MCHerwig->SetBinError(iBinX+1,hTrueFJet_MCHerwig->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX])*etaF);
   hTrueCJet_MCHerwig->SetBinError(iBinX+1,hTrueCJet_MCHerwig->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX])*etaC);
   hMeasFJet_MCHerwig->SetBinError(iBinX+1,hMeasFJet_MCHerwig->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX])*etaF);
   hMeasCJet_MCHerwig->SetBinError(iBinX+1,hMeasCJet_MCHerwig->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX])*etaC);  
  
   hTrueFJet_MCHerwig->SetBinContent(iBinX+1,hTrueFJet_MCHerwig->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX])*etaF);
   hTrueCJet_MCHerwig->SetBinContent(iBinX+1,hTrueCJet_MCHerwig->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX])*etaC);
   hMeasFJet_MCHerwig->SetBinContent(iBinX+1,hMeasFJet_MCHerwig->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX])*etaF);
   hMeasCJet_MCHerwig->SetBinContent(iBinX+1,hMeasCJet_MCHerwig->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX])*etaC);       
  }



  cout << "==================================== TEST =====================================" << endl;

  
//   TFile FileTest("~/Dropbox/QCD_CF/NtupleBo/ntuple/data/Jet_2010A.root","READ");
  TFile FileTest("~/Dropbox/QCD_CF/NtupleBo/ntuple/data/Jet_2010A.root","READ");
  
  TTree* TreeTest = (TTree*) FileTest.Get("AnaHiggs");
  TreeTest->SetBranchAddress("S_FJet_Pt",&S_FJet_Pt);
  TreeTest->SetBranchAddress("S_CJet_Pt",&S_CJet_Pt);
  
  TreeTest->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt);
  TreeTest->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt);
  
  TH1D* hMeasFJet = new TH1D ("hMeasFJet", "Measured FJet", NBIN , lowEdge);
  TH1D* hMeasCJet = new TH1D ("hMeasCJet", "Measured CJet", NBIN , lowEdge);

  TH1D* hMeasFJet_Plus = new TH1D ("hMeasFJet_Plus", "Measured FJet_Plus", NBIN , lowEdge);
  TH1D* hMeasCJet_Plus = new TH1D ("hMeasCJet_Plus", "Measured CJet_Plus", NBIN , lowEdge);
  TH1D* hMeasFJet_Minus = new TH1D ("hMeasFJet_Minus", "Measured FJet_Minus", NBIN , lowEdge);
  TH1D* hMeasCJet_Minus = new TH1D ("hMeasCJet_Minus", "Measured CJet_Minus", NBIN , lowEdge);

  for (Int_t iEvt= 0; iEvt<TreeTest->GetEntries(); iEvt++) {
   TreeTest->GetEntry(iEvt);
//    std::cerr << " S_FJet_Pt = " << S_FJet_Pt << std::endl;
     if (S_FJet_Pt>35 && S_CJet_Pt>35) {
//      if ( S_CJet_Pt>35 && S_FJet_Pt>60) {
    hMeasFJet->Fill(S_FJet_Pt);
    hMeasCJet->Fill(S_CJet_Pt);
    
    jecUnc->setJetEta(S_FJet_Eta);
    jecUnc->setJetPt(S_FJet_Pt); // IMPORTANT: the uncertainty is a function of the CORRECTED pt
    double unc_F = jecUnc->getUncertainty(true);
    
    hMeasFJet_Plus->Fill(S_FJet_Pt * (1 + unc_F));
    hMeasFJet_Minus->Fill(S_FJet_Pt * (1 - unc_F));
    
    jecUnc->setJetEta(S_CJet_Eta);
    jecUnc->setJetPt(S_CJet_Pt); // IMPORTANT: the uncertainty is a function of the CORRECTED pt
    double unc_C = jecUnc->getUncertainty(true);
    
    hMeasCJet_Plus->Fill(S_CJet_Pt * (1 + unc_C));
    hMeasCJet_Minus->Fill(S_CJet_Pt * (1 - unc_C));

   }
  }
  
  hMeasFJet->Sumw2();
  hMeasCJet->Sumw2();
  hMeasFJet_Plus->Sumw2();
  hMeasCJet_Plus->Sumw2();
  hMeasFJet_Minus->Sumw2();
  hMeasCJet_Minus->Sumw2();
  
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
      
   hMeasFJet->SetBinError(iBinX+1,hMeasFJet->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / lumi*etaF);
   hMeasCJet->SetBinError(iBinX+1,hMeasCJet->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / lumi*etaC);
   hMeasFJet->SetBinContent(iBinX+1,hMeasFJet->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / lumi*etaF);
   hMeasCJet->SetBinContent(iBinX+1,hMeasCJet->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / lumi*etaC);       

   hMeasFJet_Plus->SetBinError(iBinX+1,hMeasFJet_Plus->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / lumi*etaF);
   hMeasCJet_Plus->SetBinError(iBinX+1,hMeasCJet_Plus->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / lumi*etaC);
   hMeasFJet_Plus->SetBinContent(iBinX+1,hMeasFJet_Plus->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / lumi*etaF);
   hMeasCJet_Plus->SetBinContent(iBinX+1,hMeasCJet_Plus->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / lumi*etaC);       

   hMeasFJet_Minus->SetBinError(iBinX+1,hMeasFJet_Minus->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / lumi*etaF);
   hMeasCJet_Minus->SetBinError(iBinX+1,hMeasCJet_Minus->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / lumi*etaC);    
   hMeasFJet_Minus->SetBinContent(iBinX+1,hMeasFJet_Minus->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / lumi*etaF);
   hMeasCJet_Minus->SetBinContent(iBinX+1,hMeasCJet_Minus->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / lumi*etaC);       

  }

  
  cout << "==================================== UNFOLD Pythia ===================================" << endl;

  //get the correction factors from file
  TFile* _file0 = new TFile("/home/toliman/Scrivania/Tesi/CFNtuple/test/CorrectionFactors/correction_mean.root","READ");
  TH1D* Correction_Weight_F = (TH1D*) _file0->Get("Correction_Weight_F_mean");
  TH1D* Correction_Weight_C = (TH1D*) _file0->Get("Correction_Weight_C_mean");
  
  //create reco distributions
  
  TH1D* hRecoFJet = new TH1D ("hRecoFJet", "hRecoFJet", NBIN , lowEdge);
  TH1D* hRecoCJet = new TH1D ("hRecoCJet", "hRecoCJet", NBIN , lowEdge);
  
  TH1D* hRecoFJet_Plus = new TH1D ("hRecoFJet_Plus", "hRecoFJet_Plus", NBIN , lowEdge);
  TH1D* hRecoCJet_Plus = new TH1D ("hRecoCJet_Plus", "hRecoCJet_Plus", NBIN , lowEdge);
  
  TH1D* hRecoFJet_Minus = new TH1D ("hRecoFJet_Minus", "hRecoFJet_Minus", NBIN , lowEdge);
  TH1D* hRecoCJet_Minus = new TH1D ("hRecoCJet_Minus", "hRecoCJet_Minus", NBIN , lowEdge);
  
  //compute total systematic + statistical uncertainties and fill histograms
  
  
  for (int iBin = 0; iBin<NBIN; iBin++){
  hRecoFJet->SetBinContent(iBin+1, hMeasFJet->GetBinContent(iBin+1)*Correction_Weight_F->GetBinContent(iBin+1));
  hRecoCJet->SetBinContent(iBin+1, hMeasCJet->GetBinContent(iBin+1)*Correction_Weight_C->GetBinContent(iBin+1));
  
  hRecoFJet_Minus->SetBinContent(iBin+1, hMeasFJet_Minus->GetBinContent(iBin+1)*Correction_Weight_F->GetBinContent(iBin+1));
  hRecoCJet_Minus->SetBinContent(iBin+1, hMeasCJet_Minus->GetBinContent(iBin+1)*Correction_Weight_C->GetBinContent(iBin+1));
  
  hRecoFJet_Plus->SetBinContent(iBin+1, hMeasFJet_Plus->GetBinContent(iBin+1)*Correction_Weight_F->GetBinContent(iBin+1));
  hRecoCJet_Plus->SetBinContent(iBin+1, hMeasCJet_Plus->GetBinContent(iBin+1)*Correction_Weight_C->GetBinContent(iBin+1));
  
  double statF = hMeasFJet->GetBinError(iBin+1)*Correction_Weight_F->GetBinContent(iBin+1);
  double statC = hMeasCJet->GetBinError(iBin+1)*Correction_Weight_C->GetBinContent(iBin+1);
  
  double JES_F = (fabs(hRecoFJet_Plus->GetBinContent(iBin+1) - hRecoFJet_Minus->GetBinContent(iBin+1)))/2 ;
  double JES_C = (fabs(hRecoCJet_Plus->GetBinContent(iBin+1) - hRecoCJet_Minus->GetBinContent(iBin+1)))/2 ;
  
  hRecoFJet->SetBinError(iBin+1, sqrt(statF*statF+JES_F*JES_F));
  hRecoCJet->SetBinError(iBin+1, sqrt(statC*statC+JES_C*JES_C));
  }
  
  
  cout << "==================================== Ratio Plot =====================================" << endl;
  
  //ratio histograms
  
  TH1D* hRecoFJet_ratio = new TH1D ("hRecoFJet_ratio", "hRecoFJet_ratio", NBIN , lowEdge);
  TH1D* hRecoCJet_ratio = new TH1D ("hRecoCJet_ratio", "hRecoCJet_ratio", NBIN , lowEdge);
  
  TH1D* hTrueFJet_MCHerwig_ratio = new TH1D ("hTrueFJet_MCHerwig_ratio", "hTrueFJet_MCHerwig_ratio", NBIN , lowEdge);
  TH1D* hTrueCJet_MCHerwig_ratio = new TH1D ("hTrueCJet_MCHerwig_ratio", "hTrueCJet_MCHerwig_ratio", NBIN , lowEdge);
  
  TH1D* hTrueFJet_MCPythia_ratio = new TH1D ("hTrueFJet_MCPythia_ratio", "hTrueFJet_MCPythia_ratio", NBIN , lowEdge);
  TH1D* hTrueCJet_MCPythia_ratio = new TH1D ("hTrueCJet_MCPythia_ratio", "hTrueCJet_MCPythia_ratio", NBIN , lowEdge);
  
//   TH1D* hRecoFJet_ratio = new TH1D ("hRecoFJet_ratio", "hRecoFJet_ratio", NBIN , lowEdge);
//   TH1D* hRecoCJet_ratio = new TH1D ("hRecoCJet_ratio", "hRecoCJet_ratio", NBIN , lowEdge);
  
   
  for (int iBin = 0; iBin<NBIN; iBin++){
  hRecoFJet_ratio->SetBinContent(iBin+1, 1.);
  hRecoCJet_ratio->SetBinContent(iBin+1, 1.);

  hTrueFJet_MCHerwig_ratio->SetBinContent(iBin+1, hTrueFJet_MCHerwig->GetBinContent(iBin+1)/hRecoFJet->GetBinContent(iBin+1));
  hTrueCJet_MCHerwig_ratio->SetBinContent(iBin+1, hTrueCJet_MCHerwig->GetBinContent(iBin+1)/hRecoCJet->GetBinContent(iBin+1));
  
  hTrueFJet_MCPythia_ratio->SetBinContent(iBin+1, hTrueFJet_MCPythia->GetBinContent(iBin+1)/hRecoFJet->GetBinContent(iBin+1));
  hTrueCJet_MCPythia_ratio->SetBinContent(iBin+1, hTrueCJet_MCPythia->GetBinContent(iBin+1)/hRecoCJet->GetBinContent(iBin+1));
  
  
   hRecoFJet_ratio->SetBinError(iBin+1, hRecoFJet->GetBinError(iBin+1)/hRecoFJet->GetBinContent(iBin+1));
   hRecoCJet_ratio->SetBinError(iBin+1, hRecoCJet->GetBinError(iBin+1)/hRecoCJet->GetBinContent(iBin+1));
  }
  
 
  
  cout << "==================================== JES test =====================================" << endl;

   
      //Mean of Both MC
     
  TCanvas* ccCJet_JESHerwigPythiaUMean = new TCanvas("ccCJet_JESHerwigPythiaUMean","ccCJet_JESHerwigPythiaUMean",600,600);  

  hRecoCJet->GetYaxis()->SetTitle("d#sigma/dp_{T} (pb/GeV)");
  hRecoCJet->GetXaxis()->SetTitle("central jet p_{T} (GeV)");
  hRecoCJet->SetFillStyle(3001);  
  hRecoCJet->SetFillColor(kYellow);  
  hRecoCJet->SetLineColor(kBlack);  
  hRecoCJet->SetMarkerStyle(20);
  hRecoCJet->SetMarkerSize(1);
  hRecoCJet->SetMarkerColor(kBlack); 
  hRecoCJet->SetStats(0);
  hRecoCJet->GetXaxis()->SetRangeUser(35.,149.);
  hRecoCJet->DrawCopy("E2");

 /* hRecoCJetPythia->GetXaxis()->SetTitle("p_{T} had");
  hRecoCJetPythia->SetLineColor(kOrange);
  hRecoCJetPythia->SetMarkerColor(kOrange);
  hRecoCJetPythia->SetMarkerStyle(20);
  hRecoCJetPythia->SetMarkerSize(1);
  hRecoCJetPythia->DrawCopy("SAMEE");*/


  /*hRecoCJet_MeanPythia->SetFillStyle(3004);  
  hRecoCJet_MeanPythia->SetFillColor(kOrange);  
  hRecoCJet_MeanPythia->SetLineColor(kOrange);  
  hRecoCJet_MeanPythia->SetMarkerColor(kOrange); 
  hRecoCJet_MeanPythia->SetStats(0);
  hRecoCJet_MeanPythia->DrawCopy("SAMEE2");*/
/*
  hRecoCJetPythia->GetXaxis()->SetTitle("p_{T} had");
//   hRecoCJetPythia->SetLineColor(kAzure);
//   hRecoCJetPythia->SetMarkerColor(kAzure);
  hRecoCJetPythia->SetMarkerStyle(20);
  hRecoCJetPythia->SetMarkerSize(1);
  hRecoCJetPythia->DrawCopy("SAMEE");
  
  hRecoCJetHerwig->GetXaxis()->SetTitle("p_{T} had");
//   hRecoCJetPythia->SetLineColor(kAzure);
//   hRecoCJetPythia->SetMarkerColor(kAzure);
  hRecoCJetHerwig->SetMarkerStyle(20);
  hRecoCJetHerwig->SetMarkerSize(1);
  hRecoCJetHerwig->SetStats(0);
  hRecoCJetHerwig->DrawCopy("E");*/

  hTrueCJet_MCHerwig->SetLineColor(kRed);
  hTrueCJet_MCHerwig->SetMarkerColor(kRed);
  hTrueCJet_MCHerwig->SetMarkerStyle(21);
  hTrueCJet_MCHerwig->SetMarkerSize(1);
  hTrueCJet_MCHerwig->DrawCopy("SAMEE");

  hTrueCJet_MCPythia->SetLineColor(kGreen);
  hTrueCJet_MCPythia->SetMarkerColor(kGreen);
  hTrueCJet_MCPythia->SetMarkerStyle(21);
  hTrueCJet_MCPythia->SetMarkerSize(1);
  hTrueCJet_MCPythia->DrawCopy("SAMEE");
  
  /*hTrueCJet_MCPowheg->SetLineColor(kBlue);
  hTrueCJet_MCPowheg->SetMarkerColor(kBlue);
  hTrueCJet_MCPowheg->SetMarkerStyle(21);
  hTrueCJet_MCPowheg->SetMarkerSize(1);
  hTrueCJet_MCPowheg->DrawCopy("SAMEE");
  */
  /*hTrueCJet_MCPowheg7->SetLineColor(kViolet);
  hTrueCJet_MCPowheg7->SetMarkerColor(kViolet);
  hTrueCJet_MCPowheg7->SetMarkerStyle(21);
  hTrueCJet_MCPowheg7->SetMarkerSize(1);
  hTrueCJet_MCPowheg7->DrawCopy("SAMEE");*/
  
  
  ccCJet_JESHerwigPythiaUMean->SetLogy();

   leg = new TLegend(0.6,0.7,0.9,0.9,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   
//    leg->AddEntry("hTrueCJet_MCPowheg","Powheg", "lpf");
   leg->AddEntry("hTrueCJet_MCHerwig","Herwig", "lpf");
   leg->AddEntry("hTrueCJet_MCPythia","Pythia", "lpf");
//    leg->AddEntry("hRecoCJetPythia","JES", "lpf");
   leg->AddEntry("hRecoCJet","DATA", "lpf");
//    leg->AddEntry("hTrueCJet_MCPowheg7","Powheg + Herwig", "lpf");

   leg->Draw();    

  
  TCanvas* ccFJet_JESHerwigPythiaUMean = new TCanvas("ccFJet_JESHerwigPythiaUMean","ccFJet_JESHerwigPythiaUMean",600,600);  

  hRecoFJet->GetYaxis()->SetTitle("d#sigma/dp_{T} (pb/GeV)");
  hRecoFJet->GetXaxis()->SetTitle("forward jet p_{T} (GeV)");
  hRecoFJet->SetFillStyle(3001);  
  hRecoFJet->SetFillColor(kYellow);  
  hRecoFJet->SetMarkerStyle(20);
  hRecoFJet->SetMarkerSize(1);
  hRecoFJet->SetLineColor(kBlack);  
  hRecoFJet->SetMarkerColor(kBlack); 
  hRecoFJet->GetXaxis()->SetRangeUser(35.,149.);
  hRecoFJet->SetStats(0);
  hRecoFJet->DrawCopy("E2");

 /* hRecoFJetPythia->GetXaxis()->SetTitle("p_{T} had");
  hRecoFJetPythia->SetLineColor(kOrange);
  hRecoFJetPythia->SetMarkerColor(kOrange);
  hRecoFJetPythia->SetMarkerStyle(20);
  hRecoFJetPythia->SetMarkerSize(1);
  hRecoFJetPythia->DrawCopy("SAMEE");*/

  /*hRecoFJet_MeanPythia->SetFillStyle(3004);  
  hRecoFJet_MeanPythia->SetStats(0);
  hRecoFJet_MeanPythia->SetFillColor(kOrange);  
  hRecoFJet_MeanPythia->SetLineColor(kOrange);  
  hRecoFJet_MeanPythia->SetMarkerColor(kOrange); 
  hRecoFJet_MeanPythia->DrawCopy("SAMEE2");*/
/*
  hRecoFJetPythia->GetXaxis()->SetTitle("p_{T} had");
//   hRecoFJetPythia->SetLineColor(kAzure);
//   hRecoFJetPythia->SetMarkerColor(kAzure);
  hRecoFJetPythia->SetMarkerStyle(20);
  hRecoFJetPythia->SetMarkerSize(1);
  hRecoFJetPythia->DrawCopy("SAMEE");
  
  hRecoFJetHerwig->GetXaxis()->SetTitle("p_{T} had");
//   hRecoCJetPythia->SetLineColor(kAzure);
//   hRecoCJetPythia->SetMarkerColor(kAzure);
  hRecoFJetHerwig->SetMarkerStyle(20);
  hRecoFJetHerwig->SetMarkerSize(1);
  hRecoFJetHerwig->SetStats(0);
  hRecoFJetHerwig->DrawCopy("E");*/

  hTrueFJet_MCHerwig->SetLineColor(kRed);
  hTrueFJet_MCHerwig->SetMarkerColor(kRed);
  hTrueFJet_MCHerwig->SetMarkerStyle(21);
  hTrueFJet_MCHerwig->SetMarkerSize(1);
  hTrueFJet_MCHerwig->DrawCopy("SAMEE");

  hTrueFJet_MCPythia->SetLineColor(kGreen);
  hTrueFJet_MCPythia->SetMarkerColor(kGreen);
  hTrueFJet_MCPythia->SetMarkerStyle(21);
  hTrueFJet_MCPythia->SetMarkerSize(1);
  hTrueFJet_MCPythia->DrawCopy("SAMEE");
  
 /* hTrueFJet_MCPowheg->SetLineColor(kBlue);
  hTrueFJet_MCPowheg->SetMarkerColor(kBlue);
  hTrueFJet_MCPowheg->SetMarkerStyle(21);
  hTrueFJet_MCPowheg->SetMarkerSize(1);
  hTrueFJet_MCPowheg->DrawCopy("SAMEE");
  */
  /*hTrueFJet_MCPowheg7->SetLineColor(kViolet);
  hTrueFJet_MCPowheg7->SetMarkerColor(kViolet);
  hTrueFJet_MCPowheg7->SetMarkerStyle(21);
  hTrueFJet_MCPowheg7->SetMarkerSize(1);
  hTrueFJet_MCPowheg7->DrawCopy("SAMEE");*/
  
  ccFJet_JESHerwigPythiaUMean->SetLogy();

    leg = new TLegend(0.6,0.7,0.9,0.9,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   
//    leg->AddEntry("hTrueFJet_MCPowheg","Powheg", "lpf");
   leg->AddEntry("hTrueFJet_MCHerwig","Herwig", "lpf");
   leg->AddEntry("hTrueFJet_MCPythia","Pythia", "lpf");
//    leg->AddEntry("hRecoFJetPythia","JES", "lpf");
   leg->AddEntry("hRecoFJet","DATA", "lpf");
//    leg->AddEntry("hTrueFJet_MCPowheg7","Powheg + Herwig", "lpf");
   leg->Draw();
   
//    for (int iBinX = 0; iBinX<NBIN; iBinX++){
//      std::cout<<"Bin ["<<iBinX<<
     
//    }
   
    cout << "==================================== DRAW RATIO =====================================" << endl;

   
      //Mean of Both MC
     
  TCanvas* ccCJet_ratio = new TCanvas("ccCJet_ratio","ccCJet_ratio",600,600);  

  hRecoCJet_ratio->GetYaxis()->SetTitle("d#sigma/dp_{T} (pb/GeV)");
  hRecoCJet_ratio->GetXaxis()->SetTitle("central jet p_{T} (GeV)");
  hRecoCJet_ratio->SetFillStyle(3001);  
  hRecoCJet_ratio->SetFillColor(kYellow);  
  hRecoCJet_ratio->SetLineColor(kBlack);  
  hRecoCJet_ratio->SetMarkerStyle(20);
  hRecoCJet_ratio->SetMarkerSize(1);
  hRecoCJet_ratio->SetMarkerColor(kBlack); 
  hRecoCJet_ratio->SetStats(0);
  hRecoCJet_ratio->GetXaxis()->SetRangeUser(35.,149.);
  hRecoCJet_ratio->GetYaxis()->SetRangeUser(0.,4);
  hRecoCJet_ratio->DrawCopy("E2");
   
  hTrueCJet_MCHerwig_ratio->SetLineColor(kRed);
  hTrueCJet_MCHerwig_ratio->SetMarkerColor(kRed);
  hTrueCJet_MCHerwig_ratio->SetMarkerStyle(21);
  hTrueCJet_MCHerwig_ratio->SetMarkerSize(1);
  hTrueCJet_MCHerwig_ratio->DrawCopy("SAME");

  hTrueCJet_MCPythia_ratio->SetLineColor(kGreen);
  hTrueCJet_MCPythia_ratio->SetMarkerColor(kGreen);
  hTrueCJet_MCPythia_ratio->SetMarkerStyle(21);
  hTrueCJet_MCPythia_ratio->SetMarkerSize(1);
  hTrueCJet_MCPythia_ratio->DrawCopy("SAME");
  
  
  
  
  TCanvas* ccFJet_ratio = new TCanvas("ccFJet_ratio","ccFJet_ratio",600,600);  

  hRecoFJet_ratio->GetYaxis()->SetTitle("d#sigma/dp_{T} (pb/GeV)");
  hRecoFJet_ratio->GetXaxis()->SetTitle("central jet p_{T} (GeV)");
  hRecoFJet_ratio->SetFillStyle(3001);  
  hRecoFJet_ratio->SetFillColor(kYellow);  
  hRecoFJet_ratio->SetLineColor(kBlack);  
  hRecoFJet_ratio->SetMarkerStyle(20);
  hRecoFJet_ratio->SetMarkerSize(1);
  hRecoFJet_ratio->SetMarkerColor(kBlack); 
  hRecoFJet_ratio->SetStats(0);
  hRecoFJet_ratio->GetXaxis()->SetRangeUser(35.,149.);
  hRecoFJet_ratio->GetYaxis()->SetRangeUser(0.,4);
  hRecoFJet_ratio->DrawCopy("E2");
   
  hTrueFJet_MCHerwig_ratio->SetLineColor(kRed);
  hTrueFJet_MCHerwig_ratio->SetMarkerColor(kRed);
  hTrueFJet_MCHerwig_ratio->SetMarkerStyle(21);
  hTrueFJet_MCHerwig_ratio->SetMarkerSize(1);
  hTrueFJet_MCHerwig_ratio->DrawCopy("SAME");

  hTrueFJet_MCPythia_ratio->SetLineColor(kGreen);
  hTrueFJet_MCPythia_ratio->SetMarkerColor(kGreen);
  hTrueFJet_MCPythia_ratio->SetMarkerStyle(21);
  hTrueFJet_MCPythia_ratio->SetMarkerSize(1);
  hTrueFJet_MCPythia_ratio->DrawCopy("SAME");
   
}

#ifndef __CINT__
int main () { CrossSection(); return 0; }  // Main program when run stand-alone
#endif
