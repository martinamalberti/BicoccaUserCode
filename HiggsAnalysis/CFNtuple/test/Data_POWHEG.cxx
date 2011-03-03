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

void Data_POWHEG()
{
#ifdef __CINT__
  gSystem->Load("libRooUnfold");
#endif
  
//  setTDRStyle();

  int matching = 0;	//if matching = 1 require S_Jet >0 for filling MC histos else do not
  
  Float_t lowEdge[8] = {35,45,57,72,90,120,150,1000};
  const int NBIN = 7;
  TH1F myH("myH","myH",NBIN,lowEdge);
  TH2D* hResponseMatrixFJetPythia = new TH2D ("hResponseMatrixFJetPythia", "Response Matrix FJet Pythia",NBIN,lowEdge,NBIN,lowEdge);
  TH2D* hResponseMatrixCJetPythia = new TH2D ("hResponseMatrixCJetPythia", "Response Matrix CJet Pythia",NBIN,lowEdge,NBIN,lowEdge);

  TH2D* hResponseMatrixFJet_tempPythia = new TH2D ("hResponseMatrixFJet_tempPythia", "Response Matrix FJet",NBIN,lowEdge,NBIN,lowEdge);
  TH2D* hResponseMatrixCJet_tempPythia = new TH2D ("hResponseMatrixCJet_tempPythia", "Response Matrix CJet",NBIN,lowEdge,NBIN,lowEdge);

  TH2D* hResponseMatrixFJetHerwig = new TH2D ("hResponseMatrixFJetHerwig", "Response Matrix FJet Herwig",NBIN,lowEdge,NBIN,lowEdge);
  TH2D* hResponseMatrixCJetHerwig = new TH2D ("hResponseMatrixCJetHerwig", "Response Matrix CJet Herwig",NBIN,lowEdge,NBIN,lowEdge);

  TH2D* hResponseMatrixFJet_tempHerwig = new TH2D ("hResponseMatrixFJet_tempHerwig", "Response Matrix FJet",NBIN,lowEdge,NBIN,lowEdge);
  TH2D* hResponseMatrixCJet_tempHerwig = new TH2D ("hResponseMatrixCJet_tempHerwig", "Response Matrix CJet",NBIN,lowEdge,NBIN,lowEdge);

  
  cout << "==================================== TRAIN ====================================" << endl;
  
  RooUnfoldResponse responseFJetPythia (&myH,&myH);
  RooUnfoldResponse responseCJetPythia (&myH,&myH);

  RooUnfoldResponse responseFJetHerwig (&myH,&myH);
  RooUnfoldResponse responseCJetHerwig (&myH,&myH);
  
  TFile FileTrain1Pythia("../input/BO/rootfiles/mc/qcd_15_pythia.root","READ");
  TFile FileTrain2Pythia("../input/BO/rootfiles/mc/qcd_30_pythia.root","READ");

   TFile FileTrain1Herwig("../input/BO/rootfiles/mc/qcd_15_herwigjimmy.root","READ");
   TFile FileTrain2Herwig("../input/BO/rootfiles/mc/qcd_30_herwigjimmy.root","READ");
  
  
//   TFile FileTrain1Pythia("../input/Unfolding/qcd_15_pythia.root","READ");
//   TFile FileTrain2Pythia("../input/Unfolding/qcd_30_pythia.root","READ");

//    TFile FileTrain1Herwig("../output/HerwigJimmy15.root","READ");
//    TFile FileTrain2Herwig("../output/HerwigJimmy30.root","READ");
  
     TFile FileTrainPOW("../output/powheg15_JOB_8_2.root","READ");
//     TFile FileTrainPOW("../output/powheg15.root","READ");
//     TFile FileTrainPOW("../output/powheg10.root","READ");
//      TFile FileTrainPOW("../output/powheg25.root","READ");




  double lumi = 5.1e-3;
//     double lumi = 5.1e-3;	//lumiNew

  
  double xsec1Pythia = 8.762e8 / 6190500.;
  double xsec2Pythia = 6.041e7 / 4918016.;
  double xsec1Herwig = 714000000. / 1631667.;
  double xsec2Herwig =  49240000. / 1310829.;
  
  double xsecMine = 1;
  //double xsecPowheg = 900054123. / 998347.;
//   double xsecPowheg = 900336752. / 998347.;
//   double xsecPowheg = 900336751.9368 / 998347.;
  
  double Threshold_G_FJet = 57;
  double Threshold_G_CJet = 57;
  
  TTree* TreeTrain1Pythia = (TTree*) FileTrain1Pythia.Get("AnaHiggs");
  TTree* TreeTrain2Pythia = (TTree*) FileTrain2Pythia.Get("AnaHiggs");
 
  TTree* TreeTrain1Herwig = (TTree*) FileTrain1Herwig.Get("AnaHiggs");
  TTree* TreeTrain2Herwig = (TTree*) FileTrain2Herwig.Get("AnaHiggs");
  
  TTree* TreeTrainPOW = (TTree*) FileTrainPOW.Get("AnaHiggs");
  

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
 
 TreeTrain1Herwig->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt);
 TreeTrain1Herwig->SetBranchAddress("S_FJet_Pt",&S_FJet_Pt);
 TreeTrain1Herwig->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt);
 TreeTrain1Herwig->SetBranchAddress("S_CJet_Pt",&S_CJet_Pt);

 TreeTrain2Herwig->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt);
 TreeTrain2Herwig->SetBranchAddress("S_FJet_Pt",&S_FJet_Pt);
 TreeTrain2Herwig->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt);
 TreeTrain2Herwig->SetBranchAddress("S_CJet_Pt",&S_CJet_Pt);
 
 TreeTrainPOW->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt_P);
 TreeTrainPOW->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt_P);

 


  cout << "=================================================================================" << endl;
  cout << "==================================== Pythia =====================================" << endl;
//Filling the response matrix
  
 for (Int_t iEvt= 0; iEvt<TreeTrain1Pythia->GetEntries(); iEvt++) {
    TreeTrain1Pythia->GetEntry(iEvt);
//     if (S_FJet_Pt>0 && G_FJet_Pt>0)   std::cerr << " S_FJet_Pt = " << S_FJet_Pt << " G_FJet_Pt = " << G_FJet_Pt << std::endl;
    
    if (S_FJet_Pt>0 && G_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0)  {
     if (G_FJet_Pt < Threshold_G_FJet) {
//      responseFJet.Fill (S_FJet_Pt, G_FJet_Pt, xsec1 / xsec_mean);
      hResponseMatrixFJet_tempPythia->Fill(S_FJet_Pt, G_FJet_Pt);
     }
     if (G_CJet_Pt < Threshold_G_CJet) {
//      responseCJet.Fill (S_CJet_Pt, G_CJet_Pt, xsec1 / xsec_mean);
      hResponseMatrixCJet_tempPythia->Fill(S_CJet_Pt, G_CJet_Pt);
     }
    }
//       response.Miss (xt);
  }
  
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   for (int iBinY = 0; iBinY<NBIN; iBinY++){
    hResponseMatrixCJet_tempPythia->SetBinError(iBinX+1,iBinY+1,hResponseMatrixCJet_tempPythia->GetBinError(iBinX+1,iBinY+1) * xsec1Pythia);
    hResponseMatrixFJet_tempPythia->SetBinError(iBinX+1,iBinY+1,hResponseMatrixFJet_tempPythia->GetBinError(iBinX+1,iBinY+1) * xsec1Pythia);

    hResponseMatrixCJet_tempPythia->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixCJet_tempPythia->GetBinContent(iBinX+1,iBinY+1) * xsec1Pythia);
    hResponseMatrixFJet_tempPythia->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixFJet_tempPythia->GetBinContent(iBinX+1,iBinY+1) * xsec1Pythia);
   }
  }    
  
  
  for (Int_t iEvt= 0; iEvt<TreeTrain2Pythia->GetEntries(); iEvt++) {
   TreeTrain2Pythia->GetEntry(iEvt);
   //     if (S_FJet_Pt>0 && G_FJet_Pt>0)   std::cerr << " S_FJet_Pt = " << S_FJet_Pt << " G_FJet_Pt = " << G_FJet_Pt << std::endl;
   
   if (S_FJet_Pt>0 && G_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0)  {
    if (G_FJet_Pt >= Threshold_G_FJet) {
//     responseFJet.Fill (S_FJet_Pt, G_FJet_Pt, xsec2 / xsec_mean);
     hResponseMatrixFJetPythia->Fill(S_FJet_Pt, G_FJet_Pt);
    }
    if (G_CJet_Pt >= Threshold_G_CJet) {
//     responseCJet.Fill (S_CJet_Pt, G_CJet_Pt, xsec2 / xsec_mean);
     hResponseMatrixCJetPythia->Fill(S_CJet_Pt, G_CJet_Pt);
    }
   }
   //       response.Miss (xt);
  }

  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   for (int iBinY = 0; iBinY<NBIN; iBinY++){
    hResponseMatrixCJetPythia->SetBinError(iBinX+1,iBinY+1,hResponseMatrixCJetPythia->GetBinError(iBinX+1,iBinY+1) * xsec2Pythia);
    hResponseMatrixFJetPythia->SetBinError(iBinX+1,iBinY+1,hResponseMatrixFJetPythia->GetBinError(iBinX+1,iBinY+1) * xsec2Pythia);

    hResponseMatrixCJetPythia->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixCJetPythia->GetBinContent(iBinX+1,iBinY+1) * xsec2Pythia);
    hResponseMatrixFJetPythia->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixFJetPythia->GetBinContent(iBinX+1,iBinY+1) * xsec2Pythia);
   }
  } 
  
    
  hResponseMatrixFJetPythia->Add(hResponseMatrixFJet_tempPythia);
  hResponseMatrixCJetPythia->Add(hResponseMatrixCJet_tempPythia);
  
  
    cout << "==================================== Computing Acceptance ===================================" << endl;

    
  ///ACCEPTANCE
  
 TH1D* Non_matchedF_P = new TH1D ("Non_matchedF_P", "Non_matchedF_P",NBIN,lowEdge);

 
  double AcceptanceF_Pythia[NBIN];

  //Accettanza for forward jets
 for (Int_t iEvt= 0; iEvt<TreeTrain1Pythia->GetEntries(); iEvt++) {
   TreeTrain1Pythia->GetEntry(iEvt);
    
   if(G_FJet_Pt>0 && G_FJet_Pt<Threshold_G_FJet){
      Non_matchedF_P->Fill(G_FJet_Pt, xsec1Pythia);
     }
 }
 
  for (Int_t iEvt= 0; iEvt<TreeTrain2Pythia->GetEntries(); iEvt++) {
   TreeTrain2Pythia->GetEntry(iEvt);
    
   if(G_FJet_Pt>=Threshold_G_FJet){
      Non_matchedF_P->Fill(G_FJet_Pt, xsec2Pythia);
     }
 }
 
  for (int iBinY = 0; iBinY<NBIN; iBinY++){
    double row = 0;
    for (int iBinX = 0; iBinX<NBIN; iBinX++){
      row = row + hResponseMatrixFJetPythia->GetBinContent(iBinX+1,iBinY+1);
    }
    
    double N_all = Non_matchedF_P->GetBinContent(iBinY +1);
    std::cerr<<"Row = " <<row <<std::endl;
     AcceptanceF_Pythia[iBinY] = row/N_all;
     std::cout<<"Acceptance = " <<AcceptanceF_Pythia[iBinY] <<endl;
//     AcceptanceF_Pythia->SetBinError(iBinY+1, (sqrt(1./row)+sqrt(1./N_all))*row/N_all);
  }
 
 //Acceptance for central jets
 
 TH1D* Non_matchedC_P = new TH1D ("Non_matchedC_P", "Non_matchedC_P",NBIN,lowEdge);
 
 double AcceptanceC_Pythia[NBIN];


 for (Int_t iEvt= 0; iEvt<TreeTrain1Pythia->GetEntries(); iEvt++) {
   TreeTrain1Pythia->GetEntry(iEvt);
    
   if(G_CJet_Pt>0 && G_CJet_Pt<Threshold_G_CJet){
      Non_matchedC_P->Fill(G_CJet_Pt, xsec1Pythia);
     }
 }
 
  for (Int_t iEvt= 0; iEvt<TreeTrain2Pythia->GetEntries(); iEvt++) {
   TreeTrain2Pythia->GetEntry(iEvt);
    
   if(G_CJet_Pt>=Threshold_G_CJet){
      Non_matchedC_P->Fill(G_CJet_Pt, xsec2Pythia);
     }
 }
 
  for (int iBinY = 0; iBinY<NBIN; iBinY++){
    double stability = 0;
    double row = 0;
    for (int iBinX = 0; iBinX<NBIN; iBinX++){
      row = row + hResponseMatrixCJetPythia->GetBinContent(iBinX+1,iBinY+1);
    }
    
    double N_all = Non_matchedC_P->GetBinContent(iBinY +1);
    std::cout<<"Row = " <<row <<endl;
    AcceptanceC_Pythia[iBinY] = row/N_all;
    std::cout<<"Acceptance = " <<AcceptanceC_Pythia[iBinY] <<endl;
//     AcceptanceF_Pythia->SetBinError(iBinY+1, (sqrt(1./row)+sqrt(1./N_all))*row/N_all);
  }



 
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~ scale for variable size binning ~~~~~~~~~~~~~~~~~~~~~~~" << endl;
//  hResponseMatrixFJet->Sumw2();
//  hResponseMatrixCJet->Sumw2();

  
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   for (int iBinY = 0; iBinY<NBIN; iBinY++){

//    hResponseMatrixFJet->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixFJet->GetBinContent(iBinX+1,iBinY+1) / xsecScale ) ;
//    hResponseMatrixCJet->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixCJet->GetBinContent(iBinX+1,iBinY+1) / xsecScale ) ;

    hResponseMatrixFJetPythia->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixFJetPythia->GetBinContent(iBinX+1,iBinY+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / (lowEdge[iBinY+1] - lowEdge[iBinY]));
    hResponseMatrixCJetPythia->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixCJetPythia->GetBinContent(iBinX+1,iBinY+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / (lowEdge[iBinY+1] - lowEdge[iBinY]));

    hResponseMatrixFJetPythia->SetBinError(iBinX+1,iBinY+1,hResponseMatrixFJetPythia->GetBinError(iBinX+1,iBinY+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / (lowEdge[iBinY+1] - lowEdge[iBinY]));
    hResponseMatrixCJetPythia->SetBinError(iBinX+1,iBinY+1,hResponseMatrixCJetPythia->GetBinError(iBinX+1,iBinY+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / (lowEdge[iBinY+1] - lowEdge[iBinY]));
    
    
   }
  }
  
  /*TCanvas* ccResponseCJetPythia = new TCanvas("ccResponseCJetPythia","ccResponseCJetPythia",600,600);  
  hResponseMatrixCJetPythia->SetLineColor(kRed);
  hResponseMatrixCJetPythia->SetMarkerColor(kRed);
  hResponseMatrixCJetPythia->SetMarkerStyle(20);
  hResponseMatrixCJetPythia->Draw("LEGOE");
  ccResponseCJetPythia->SetLogz();
  
  TCanvas* ccResponseFJetPythia = new TCanvas("ccResponseFJetPythia","ccResponseFJetPythia",600,600);  
  hResponseMatrixFJetPythia->SetLineColor(kRed);
  hResponseMatrixFJetPythia->SetMarkerColor(kRed);
  hResponseMatrixFJetPythia->SetMarkerStyle(20);
  hResponseMatrixFJetPythia->Draw("LEGOE");
  ccResponseFJetPythia->SetLogz();*/
  
  
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
   
   if (matching ==1){
   if (S_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0 && G_FJet_Pt>0) {
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
  
  else {
   if (G_CJet_Pt>0 && G_FJet_Pt>0) {
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
   if (matching ==1){
   if (S_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0 && G_FJet_Pt>0) {
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
  
  else {
   if (G_CJet_Pt>0 && G_FJet_Pt>0) {
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
   hTrueFJet_MCPythia->SetBinError(iBinX+1,hTrueFJet_MCPythia->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hTrueCJet_MCPythia->SetBinError(iBinX+1,hTrueCJet_MCPythia->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hMeasFJet_MCPythia->SetBinError(iBinX+1,hMeasFJet_MCPythia->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hMeasCJet_MCPythia->SetBinError(iBinX+1,hMeasCJet_MCPythia->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));  
  
   hTrueFJet_MCPythia->SetBinContent(iBinX+1,hTrueFJet_MCPythia->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hTrueCJet_MCPythia->SetBinContent(iBinX+1,hTrueCJet_MCPythia->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hMeasFJet_MCPythia->SetBinContent(iBinX+1,hMeasFJet_MCPythia->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hMeasCJet_MCPythia->SetBinContent(iBinX+1,hMeasCJet_MCPythia->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));       
  }
  
  
  
  
  
  cout << "==================================== Create Response Matrix Pythia =====================================" << endl;
  
  responseFJetPythia.Setup(hMeasFJet_MCPythia,hTrueFJet_MCPythia,hResponseMatrixFJetPythia);
  responseCJetPythia.Setup(hMeasCJet_MCPythia,hTrueCJet_MCPythia,hResponseMatrixCJetPythia);



/*

  cout << "=================================================================================" << endl;
  cout << "==================================== Herwig =====================================" << endl;
//Filling the response matrix
 for (Int_t iEvt= 0; iEvt<TreeTrain1Herwig->GetEntries(); iEvt++) {
    TreeTrain1Herwig->GetEntry(iEvt);
//     if (S_FJet_Pt>0 && G_FJet_Pt>0)   std::cerr << " S_FJet_Pt = " << S_FJet_Pt << " G_FJet_Pt = " << G_FJet_Pt << std::endl;
    
    if (S_FJet_Pt>0 && G_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0)  {
     if (G_FJet_Pt < Threshold_G_FJet) {
//      responseFJet.Fill (S_FJet_Pt, G_FJet_Pt, xsec1 / xsec_mean);
      hResponseMatrixFJet_tempHerwig->Fill(S_FJet_Pt, G_FJet_Pt);
     }
     if (G_CJet_Pt < Threshold_G_CJet) {
//      responseCJet.Fill (S_CJet_Pt, G_CJet_Pt, xsec1 / xsec_mean);
      hResponseMatrixCJet_tempHerwig->Fill(S_CJet_Pt, G_CJet_Pt);
     }
    }
//       response.Miss (xt);
  }
  
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   for (int iBinY = 0; iBinY<NBIN; iBinY++){
    hResponseMatrixCJet_tempHerwig->SetBinError(iBinX+1,iBinY+1,hResponseMatrixCJet_tempHerwig->GetBinError(iBinX+1,iBinY+1) * xsec1Herwig);
    hResponseMatrixFJet_tempHerwig->SetBinError(iBinX+1,iBinY+1,hResponseMatrixFJet_tempHerwig->GetBinError(iBinX+1,iBinY+1) * xsec1Herwig);

    hResponseMatrixCJet_tempHerwig->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixCJet_tempHerwig->GetBinContent(iBinX+1,iBinY+1) * xsec1Herwig);
    hResponseMatrixFJet_tempHerwig->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixFJet_tempHerwig->GetBinContent(iBinX+1,iBinY+1) * xsec1Herwig);
   }
  }    
  
  
  for (Int_t iEvt= 0; iEvt<TreeTrain2Herwig->GetEntries(); iEvt++) {
   TreeTrain2Herwig->GetEntry(iEvt);
   //     if (S_FJet_Pt>0 && G_FJet_Pt>0)   std::cerr << " S_FJet_Pt = " << S_FJet_Pt << " G_FJet_Pt = " << G_FJet_Pt << std::endl;
   
   if (S_FJet_Pt>0 && G_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0)  {
    if (G_FJet_Pt >= Threshold_G_FJet) {
//     responseFJet.Fill (S_FJet_Pt, G_FJet_Pt, xsec2 / xsec_mean);
     hResponseMatrixFJetHerwig->Fill(S_FJet_Pt, G_FJet_Pt);
    }
    if (G_CJet_Pt >= Threshold_G_CJet) {
//     responseCJet.Fill (S_CJet_Pt, G_CJet_Pt, xsec2 / xsec_mean);
     hResponseMatrixCJetHerwig->Fill(S_CJet_Pt, G_CJet_Pt);
    }
   }
   //       response.Miss (xt);
  }

  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   for (int iBinY = 0; iBinY<NBIN; iBinY++){
    hResponseMatrixCJetHerwig->SetBinError(iBinX+1,iBinY+1,hResponseMatrixCJetHerwig->GetBinError(iBinX+1,iBinY+1) * xsec2Herwig);
    hResponseMatrixFJetHerwig->SetBinError(iBinX+1,iBinY+1,hResponseMatrixFJetHerwig->GetBinError(iBinX+1,iBinY+1) * xsec2Herwig);

    hResponseMatrixCJetHerwig->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixCJetHerwig->GetBinContent(iBinX+1,iBinY+1) * xsec2Herwig);
    hResponseMatrixFJetHerwig->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixFJetHerwig->GetBinContent(iBinX+1,iBinY+1) * xsec2Herwig);
   }
  }    
 
  cout << "~~~ scale for variable size binning ~~~~" << endl;
//  hResponseMatrixFJet->Sumw2();
//  hResponseMatrixCJet->Sumw2();
  
  hResponseMatrixFJetHerwig->Add(hResponseMatrixFJet_tempHerwig);
  hResponseMatrixCJetHerwig->Add(hResponseMatrixCJet_tempHerwig);
  
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   for (int iBinY = 0; iBinY<NBIN; iBinY++){

//    hResponseMatrixFJet->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixFJet->GetBinContent(iBinX+1,iBinY+1) / xsecScale ) ;
//    hResponseMatrixCJet->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixCJet->GetBinContent(iBinX+1,iBinY+1) / xsecScale ) ;

    hResponseMatrixFJetHerwig->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixFJetHerwig->GetBinContent(iBinX+1,iBinY+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / (lowEdge[iBinY+1] - lowEdge[iBinY]));
    hResponseMatrixCJetHerwig->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixCJetHerwig->GetBinContent(iBinX+1,iBinY+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / (lowEdge[iBinY+1] - lowEdge[iBinY]));

    hResponseMatrixFJetHerwig->SetBinError(iBinX+1,iBinY+1,hResponseMatrixFJetHerwig->GetBinError(iBinX+1,iBinY+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / (lowEdge[iBinY+1] - lowEdge[iBinY]));
    hResponseMatrixCJetHerwig->SetBinError(iBinX+1,iBinY+1,hResponseMatrixCJetHerwig->GetBinError(iBinX+1,iBinY+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / (lowEdge[iBinY+1] - lowEdge[iBinY]));
    
    
   }
  }*/
  /*
  TCanvas* ccResponseCJetHerwig = new TCanvas("ccResponseCJetHerwig","ccResponseCJetHerwig",600,600);  
  hResponseMatrixCJetHerwig->SetLineColor(kRed);
  hResponseMatrixCJetHerwig->SetMarkerColor(kRed);
  hResponseMatrixCJetHerwig->SetMarkerStyle(20);
  hResponseMatrixCJetHerwig->Draw("LEGOE");
  ccResponseCJetHerwig->SetLogz();
  
  TCanvas* ccResponseFJetHerwig = new TCanvas("ccResponseFJetHerwig","ccResponseFJetHerwig",600,600);  
  hResponseMatrixFJetHerwig->SetLineColor(kRed);
  hResponseMatrixFJetHerwig->SetMarkerColor(kRed);
  hResponseMatrixFJetHerwig->SetMarkerStyle(20);
  hResponseMatrixFJetHerwig->Draw("LEGOE");
  ccResponseFJetHerwig->SetLogz();*/
  
  
  
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
   
   if (matching == 1) {
   if (S_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0 && G_FJet_Pt>0) {
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
  
  else {
   if (G_CJet_Pt>0 && G_FJet_Pt>0) {
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
   
   if (matching ==1){
   if (S_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0 && G_FJet_Pt>0) {
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
  
  else {
   if (G_CJet_Pt>0 && G_FJet_Pt>0) {
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
   hTrueFJet_MCHerwig->SetBinError(iBinX+1,hTrueFJet_MCHerwig->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hTrueCJet_MCHerwig->SetBinError(iBinX+1,hTrueCJet_MCHerwig->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hMeasFJet_MCHerwig->SetBinError(iBinX+1,hMeasFJet_MCHerwig->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hMeasCJet_MCHerwig->SetBinError(iBinX+1,hMeasCJet_MCHerwig->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));  
  
   hTrueFJet_MCHerwig->SetBinContent(iBinX+1,hTrueFJet_MCHerwig->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hTrueCJet_MCHerwig->SetBinContent(iBinX+1,hTrueCJet_MCHerwig->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hMeasFJet_MCHerwig->SetBinContent(iBinX+1,hMeasFJet_MCHerwig->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hMeasCJet_MCHerwig->SetBinContent(iBinX+1,hMeasCJet_MCHerwig->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));       
  }
  
  
  
  
  
//   cout << "==================================== Create Response Matrix Herwig =====================================" << endl;
  
//   responseFJetHerwig.Setup(hMeasFJet_MCHerwig,hTrueFJet_MCHerwig,hResponseMatrixFJetHerwig);
//   responseCJetHerwig.Setup(hMeasCJet_MCHerwig,hTrueCJet_MCHerwig,hResponseMatrixCJetHerwig);








  
  cout << "==================================== TEST =====================================" << endl;

  double JES = 0.05;
  
//   TFile FileTest("../input/Unfolding/Jet_Jun14_jet_v2.root","READ");
    TFile FileTest("../input/BO/rootfiles/data/Jet_Jun14.root","READ");
//     TFile FileTest("../input/MiBiCommonNT_23_1_xGH.root","READ");
    

  
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
   if (S_FJet_Pt>0 && S_CJet_Pt>0) {
    hMeasFJet->Fill(S_FJet_Pt);
    hMeasCJet->Fill(S_CJet_Pt);
    
    hMeasFJet_Plus->Fill(S_FJet_Pt * (1 + JES));
    hMeasCJet_Plus->Fill(S_CJet_Pt * (1 + JES));

    hMeasFJet_Minus->Fill(S_FJet_Pt * (1 - JES));
    hMeasCJet_Minus->Fill(S_CJet_Pt * (1 - JES));

   }
  }
  
  hMeasFJet->Sumw2();
  hMeasCJet->Sumw2();
  hMeasFJet_Plus->Sumw2();
  hMeasCJet_Plus->Sumw2();
  hMeasFJet_Minus->Sumw2();
  hMeasCJet_Minus->Sumw2();
  
  
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hMeasFJet->SetBinError(iBinX+1,hMeasFJet->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / lumi);
   hMeasCJet->SetBinError(iBinX+1,hMeasCJet->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / lumi);
   hMeasFJet->SetBinContent(iBinX+1,hMeasFJet->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / lumi);
   hMeasCJet->SetBinContent(iBinX+1,hMeasCJet->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / lumi);       

   hMeasFJet_Plus->SetBinError(iBinX+1,hMeasFJet_Plus->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / lumi);
   hMeasCJet_Plus->SetBinError(iBinX+1,hMeasCJet_Plus->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / lumi);
   hMeasFJet_Plus->SetBinContent(iBinX+1,hMeasFJet_Plus->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / lumi);
   hMeasCJet_Plus->SetBinContent(iBinX+1,hMeasCJet_Plus->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / lumi);       

   hMeasFJet_Minus->SetBinError(iBinX+1,hMeasFJet_Minus->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / lumi);
   hMeasCJet_Minus->SetBinError(iBinX+1,hMeasCJet_Minus->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / lumi);    
   hMeasFJet_Minus->SetBinContent(iBinX+1,hMeasFJet_Minus->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / lumi);
   hMeasCJet_Minus->SetBinContent(iBinX+1,hMeasCJet_Minus->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / lumi);       

  }
  
 
cout << "==================================== POWHEG Distributions ===================================" << endl;

  TH1D* hTrueFJet_MCPowheg = new TH1D ("hTrueFJet_MCPowheg", "MC Truth FJet Powheg",    NBIN , lowEdge);
  TH1D* hTrueCJet_MCPowheg = new TH1D ("hTrueCJet_MCPowheg", "MC Truth CJet Powheg",    NBIN , lowEdge);
   
  for (Int_t iEvt= 0; iEvt<TreeTrainPOW->GetEntries(); iEvt++) {
   TreeTrainPOW->GetEntry(iEvt);
   //    std::cerr << " S_FJet_Pt = " << S_FJet_Pt << std::endl;
   if (G_CJet_Pt_P>0 && G_FJet_Pt_P>0) {
    
     hTrueFJet_MCPowheg->Fill(G_FJet_Pt_P);
     hTrueCJet_MCPowheg->Fill(G_CJet_Pt_P);
   }
  }  
  
//   double xsecPowheg = 901.6;
//    double xsecPowheg = 900054123. / TreeTrainPOW->GetEntries();
//    double xsecPowheg = 900054123. / 1000000; //xsec15
    double xsecPowheg = 900054123. / 1517099; //sample parallel15_2/JOB_8
//     double xsecPowheg = 3523524384. / 500000;	//xsec10
//    double xsecPowheg =    142380275.4 / 500000;	//xsec25

   

   
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hTrueFJet_MCPowheg->SetBinError(iBinX+1,hTrueFJet_MCPowheg->GetBinError(iBinX+1) * xsecPowheg);
   hTrueCJet_MCPowheg->SetBinError(iBinX+1,hTrueCJet_MCPowheg->GetBinError(iBinX+1) * xsecPowheg); 
  
   hTrueFJet_MCPowheg->SetBinContent(iBinX+1,hTrueFJet_MCPowheg->GetBinContent(iBinX+1) * xsecPowheg);
   hTrueCJet_MCPowheg->SetBinContent(iBinX+1,hTrueCJet_MCPowheg->GetBinContent(iBinX+1) * xsecPowheg);
  }
  
   for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hTrueFJet_MCPowheg->SetBinError(iBinX+1,hTrueFJet_MCPowheg->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hTrueCJet_MCPowheg->SetBinError(iBinX+1,hTrueCJet_MCPowheg->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   
   hTrueFJet_MCPowheg->SetBinContent(iBinX+1,hTrueFJet_MCPowheg->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hTrueCJet_MCPowheg->SetBinContent(iBinX+1,hTrueCJet_MCPowheg->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   
  }
  
   //Weight for Pythia Acceptance 
   /*
   for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hTrueFJet_MCPowheg->SetBinError(iBinX+1,hTrueFJet_MCPowheg->GetBinError(iBinX+1) * AcceptanceF_Pythia[iBinX]);
   hTrueCJet_MCPowheg->SetBinError(iBinX+1,hTrueCJet_MCPowheg->GetBinError(iBinX+1) * AcceptanceC_Pythia[iBinX]);
   
   hTrueFJet_MCPowheg->SetBinContent(iBinX+1,hTrueFJet_MCPowheg->GetBinContent(iBinX+1) * AcceptanceF_Pythia[iBinX]);
   hTrueCJet_MCPowheg->SetBinContent(iBinX+1,hTrueCJet_MCPowheg->GetBinContent(iBinX+1) * AcceptanceC_Pythia[iBinX]);
   
  }*/
  

  
  cout << "==================================== UNFOLD Pythia ===================================" << endl;
 // RooUnfoldBayes    unfoldFJetPythia (&responseFJetPythia, hMeasFJet, 20);    // OR
  // RooUnfoldSvd      unfoldFJetPythia (&responseFJetPythia, hMeasFJet, 20);   // OR
  RooUnfoldBinByBin unfoldFJetPythia (&responseFJetPythia, hMeasFJet);
  TH1D* hRecoFJetPythia = (TH1D*) unfoldFJetPythia.Hreco(3);
  hRecoFJetPythia->SetName("FJetUnfoldPythia");
  hRecoFJetPythia->SetTitle("FJet Unfold Pythia");
    
 // RooUnfoldBayes    unfoldCJetPythia (&responseCJetPythia, hMeasCJet, 20);    // OR
  // RooUnfoldSvd      unfoldCJetPythia (&responseCJetPythia, hMeasCJet, 20);   // OR
  RooUnfoldBinByBin unfoldCJetPythia (&responseCJetPythia, hMeasCJet);
  TH1D* hRecoCJetPythia = (TH1D*) unfoldCJetPythia.Hreco(3);
  hRecoCJetPythia->SetName("CJetUnfold");


// RooUnfoldBayes    unfoldFJet_PlusPythia (&responseFJetPythia, hMeasFJet_Plus, 20);    // OR
  // RooUnfoldSvd      unfoldFJet_PlusPythia (&responseFJetPythia, hMeasFJet_Plus, 20);   // OR
  RooUnfoldBinByBin unfoldFJet_PlusPythia (&responseFJetPythia, hMeasFJet_Plus);
  TH1D* hRecoFJet_PlusPythia = (TH1D*) unfoldFJet_PlusPythia.Hreco(3);
  hRecoFJet_PlusPythia->SetName("FJetUnfold_PlusPythia");
  
//  RooUnfoldBayes    unfoldCJet_PlusPythia (&responseCJetPythia, hMeasCJet_Plus, 20);    // OR
  // RooUnfoldSvd      unfoldCJet_PlusPythia (&responseCJetPythia, hMeasCJet_Plus, 20);   // OR
  RooUnfoldBinByBin unfoldCJet_PlusPythia (&responseCJetPythia, hMeasCJet_Plus);
  TH1D* hRecoCJet_PlusPythia = (TH1D*) unfoldCJet_PlusPythia.Hreco(3);
  hRecoCJet_PlusPythia->SetName("CJetUnfold_PlusPythia");


//  RooUnfoldBayes    unfoldFJet_MinusPythia (&responseFJetPythia, hMeasFJet_Minus, 20);    // OR
  // RooUnfoldSvd      unfoldFJet_MinusPythia (&responseFJetPythia, hMeasFJet_Minus, 20);   // OR
  RooUnfoldBinByBin unfoldFJet_MinusPythia (&responseFJetPythia, hMeasFJet_Minus);
  TH1D* hRecoFJet_MinusPythia = (TH1D*) unfoldFJet_MinusPythia.Hreco(3);
  hRecoFJet_MinusPythia->SetName("FJetUnfold_MinusPythia");
  
//  RooUnfoldBayes    unfoldCJet_MinusPythia (&responseCJetPythia, hMeasCJet_Minus, 20);    // OR
  // RooUnfoldSvd      unfoldCJet_MinusPythia (&responseCJetPythia, hMeasCJet_Minus, 20);   // OR
  RooUnfoldBinByBin unfoldCJet_MinusPythia (&responseCJetPythia, hMeasCJet_Minus);
  TH1D* hRecoCJet_MinusPythia = (TH1D*) unfoldCJet_MinusPythia.Hreco(3);
  hRecoCJet_MinusPythia->SetName("CJetUnfold_MinusPythia");
    
  hRecoFJetPythia->Sumw2();
  hRecoCJetPythia->Sumw2();
  hRecoFJet_PlusPythia->Sumw2();
  hRecoCJet_PlusPythia->Sumw2();
  hRecoFJet_MinusPythia->Sumw2();
  hRecoCJet_MinusPythia->Sumw2();
  
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   /*hRecoFJetPythia->SetBinContent(iBinX+1,hRecoFJetPythia->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hRecoCJetPythia->SetBinContent(iBinX+1,hRecoCJetPythia->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));    
   hRecoFJet_PlusPythia->SetBinContent(iBinX+1,hRecoFJet_PlusPythia->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hRecoCJet_PlusPythia->SetBinContent(iBinX+1,hRecoCJet_PlusPythia->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));    
   hRecoFJet_MinusPythia->SetBinContent(iBinX+1,hRecoFJet_MinusPythia->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hRecoCJet_MinusPythia->SetBinContent(iBinX+1,hRecoCJet_MinusPythia->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));    */
  }

 /* cout << "===================================== DRAW ====================================" << endl;
  
  TCanvas* ccFJetPythia = new TCanvas("ccFJetPythia","ccFJetPythia",600,600);  
  hRecoFJetPythia->GetYaxis()->SetTitle("d#sigma/dp_{T} (pb/GeV)");
  hRecoFJetPythia->GetXaxis()->SetTitle("forward jet p_{T} (GeV)");
  hRecoFJetPythia->SetLineColor(kBlue);
  hRecoFJetPythia->SetMarkerColor(kBlue);
  hRecoFJetPythia->SetMarkerStyle(20);
  hRecoFJetPythia->SetMarkerSize(1);
  hRecoFJetPythia->DrawCopy();

  hRecoFJet_PlusPythia->GetXaxis()->SetTitle("p_{T} had");
  hRecoFJet_PlusPythia->SetFillColor(kRed);  
  hRecoFJet_PlusPythia->SetLineColor(kRed);
  hRecoFJet_PlusPythia->SetMarkerColor(kRed);
  hRecoFJet_PlusPythia->SetMarkerStyle(20);
  hRecoFJet_PlusPythia->SetMarkerSize(1);
  hRecoFJet_PlusPythia->DrawCopy("SAME");
  
  hRecoFJet_MinusPythia->GetXaxis()->SetTitle("p_{T} had");
  hRecoFJet_MinusPythia->SetFillColor(kRed);
  hRecoFJet_MinusPythia->SetLineColor(kRed);
  hRecoFJet_MinusPythia->SetMarkerColor(kRed);
  hRecoFJet_MinusPythia->SetMarkerStyle(20);
  hRecoFJet_MinusPythia->SetMarkerSize(1);
  hRecoFJet_MinusPythia->DrawCopy("SAME");
  
  hRecoFJetPythia->DrawCopy("SAME");
  gPad->BuildLegend();
  gPad->SetGrid();
 
  
  TCanvas* ccCJetPythia = new TCanvas("ccCJetPythia","ccCJetPythia",600,600);  
  hRecoCJetPythia->GetYaxis()->SetTitle("d#sigma/dp_{T} (pb/GeV)");
  hRecoCJetPythia->GetXaxis()->SetTitle("central jet p_{T} (GeV)");
  hRecoCJetPythia->SetLineColor(kBlue);
  hRecoCJetPythia->SetMarkerColor(kBlue);
  hRecoCJetPythia->SetMarkerStyle(20);
  hRecoCJetPythia->SetMarkerSize(1);
  hRecoCJetPythia->DrawCopy();

  hRecoCJet_PlusPythia->GetXaxis()->SetTitle("p_{T} had");
  hRecoCJet_PlusPythia->SetFillColor(kRed);
  hRecoCJet_PlusPythia->SetLineColor(kRed);
  hRecoCJet_PlusPythia->SetMarkerColor(kRed);
  hRecoCJet_PlusPythia->SetMarkerStyle(20);
  hRecoCJet_PlusPythia->SetMarkerSize(1);
  hRecoCJet_PlusPythia->DrawCopy("SAME");
  
  hRecoCJet_MinusPythia->GetXaxis()->SetTitle("p_{T} had");
  hRecoCJet_MinusPythia->SetFillColor(kRed);
  hRecoCJet_MinusPythia->SetLineColor(kRed);
  hRecoCJet_MinusPythia->SetMarkerColor(kRed);
  hRecoCJet_MinusPythia->SetMarkerStyle(20);
  hRecoCJet_MinusPythia->SetMarkerSize(1);
  hRecoCJet_MinusPythia->DrawCopy("SAME");

  hRecoCJetPythia->DrawCopy("SAME");
  gPad->BuildLegend();
  gPad->SetGrid();
  */
  
  cout << "===================================== DRAW ====================================" << endl;

  TH1D* hRecoFJet_MeanPythia = new TH1D ("hRecoFJet_MeanPythia", "UnFolded FJet JES Pythia", NBIN , lowEdge);
  TH1D* hRecoCJet_MeanPythia = new TH1D ("hRecoCJet_MeanPythia", "UnFolded CJet JES Pythia", NBIN , lowEdge);  

 for (int iBinX = 0; iBinX<NBIN; iBinX++){
  hRecoFJet_MeanPythia->SetBinContent(iBinX+1,(hRecoFJet_PlusPythia->GetBinContent(iBinX+1) + hRecoFJet_MinusPythia->GetBinContent(iBinX+1))/2.);
  hRecoCJet_MeanPythia->SetBinContent(iBinX+1,(hRecoCJet_PlusPythia->GetBinContent(iBinX+1) + hRecoCJet_MinusPythia->GetBinContent(iBinX+1))/2.);

  hRecoFJet_MeanPythia->SetBinError(iBinX+1,fabs(hRecoFJet_PlusPythia->GetBinContent(iBinX+1) - hRecoFJet_MinusPythia->GetBinContent(iBinX+1))/2.);
  hRecoCJet_MeanPythia->SetBinError(iBinX+1,fabs(hRecoCJet_PlusPythia->GetBinContent(iBinX+1) - hRecoCJet_MinusPythia->GetBinContent(iBinX+1))/2.);     
 }

 /*
  TCanvas* ccFJet_JESPythia = new TCanvas("ccFJet_JESPythia","ccFJet_JESPythia",600,600);  

  hRecoFJet_MeanPythia->GetYaxis()->SetTitle("d#sigma/dp_{T} (pb/GeV)");
  hRecoFJet_MeanPythia->GetXaxis()->SetTitle("forward jet p_{T} (GeV)");
  hRecoFJet_MeanPythia->SetFillColor(kRed);  
  hRecoFJet_MeanPythia->SetLineColor(kRed);  
  hRecoFJet_MeanPythia->SetMarkerColor(kRed); 
  hRecoFJet_MeanPythia->DrawCopy("E2");

  hRecoFJetPythia->GetXaxis()->SetTitle("p_{T} had");
  hRecoFJetPythia->SetLineColor(kBlue);
  hRecoFJetPythia->SetMarkerColor(kBlue);
  hRecoFJetPythia->SetMarkerStyle(20);
  hRecoFJetPythia->SetMarkerSize(1);
  hRecoFJetPythia->DrawCopy("SAMEE");
  
  hTrueFJet_MCPythia->SetLineColor(kGreen);
  hTrueFJet_MCPythia->SetMarkerColor(kGreen);
  hTrueFJet_MCPythia->SetMarkerStyle(21);
  hTrueFJet_MCPythia->SetMarkerSize(1);
  hTrueFJet_MCPythia->DrawCopy("SAMEE");
  
  gPad->BuildLegend();
  gPad->SetGrid();

  
  TCanvas* ccCJet_JESPythia = new TCanvas("ccCJet_JESPythia","ccCJet_JESPythia",600,600);  

  hRecoCJet_MeanPythia->GetYaxis()->SetTitle("d#sigma/dp_{T} (pb/GeV)");
  hRecoCJet_MeanPythia->GetXaxis()->SetTitle("central jet p_{T} (GeV)");
  hRecoCJet_MeanPythia->SetFillColor(kRed);  
  hRecoCJet_MeanPythia->SetLineColor(kRed);  
  hRecoCJet_MeanPythia->SetMarkerColor(kRed); 
  hRecoCJet_MeanPythia->DrawCopy("E2");

  hRecoCJetPythia->GetXaxis()->SetTitle("p_{T} had");
  hRecoCJetPythia->SetLineColor(kBlue);
  hRecoCJetPythia->SetMarkerColor(kBlue);
  hRecoCJetPythia->SetMarkerStyle(20);
  hRecoCJetPythia->SetMarkerSize(1);
  hRecoCJetPythia->DrawCopy("SAMEE");

  hTrueCJet_MCPythia->SetLineColor(kGreen);
  hTrueCJet_MCPythia->SetMarkerColor(kGreen);
  hTrueCJet_MCPythia->SetMarkerStyle(21);
  hTrueCJet_MCPythia->SetMarkerSize(1);
  hTrueCJet_MCPythia->DrawCopy("SAMEE");

  gPad->BuildLegend();
  gPad->SetGrid(); 
  
  cout << "==================================== JES test =====================================" << endl;
    
  
  
  
  
  
  cout << "==================================== UNFOLD Herwig ===================================" << endl;
//  RooUnfoldBayes    unfoldFJetHerwig (&responseFJetHerwig, hMeasFJet, 10);    // OR
  // RooUnfoldSvd      unfoldFJetHerwig (&responseFJetHerwig, hMeasFJet, 20);   // OR
  RooUnfoldBinByBin unfoldFJetHerwig (&responseFJetHerwig, hMeasFJet);
  TH1D* hRecoFJetHerwig = (TH1D*) unfoldFJetHerwig.Hreco(3);
  hRecoFJetHerwig->SetName("FJetUnfoldHerwig");
  hRecoFJetHerwig->SetTitle("FJet Unfold Herwig");
    
//  RooUnfoldBayes    unfoldCJetHerwig (&responseCJetHerwig, hMeasCJet, 10);    // OR
  // RooUnfoldSvd      unfoldCJetHerwig (&responseCJetHerwig, hMeasCJet, 20);   // OR
  RooUnfoldBinByBin unfoldCJetHerwig (&responseCJetHerwig, hMeasCJet);
  TH1D* hRecoCJetHerwig = (TH1D*) unfoldCJetHerwig.Hreco(3);
  hRecoCJetHerwig->SetName("CJetUnfold");


//  RooUnfoldBayes    unfoldFJet_PlusHerwig (&responseFJetHerwig, hMeasFJet_Plus, 10);    // OR
  // RooUnfoldSvd      unfoldFJet_PlusHerwig (&responseFJetHerwig, hMeasFJet_Plus, 20);   // OR
  RooUnfoldBinByBin unfoldFJet_PlusHerwig (&responseFJetHerwig, hMeasFJet_Plus);
  TH1D* hRecoFJet_PlusHerwig = (TH1D*) unfoldFJet_PlusHerwig.Hreco(3);
  hRecoFJet_PlusHerwig->SetName("FJetUnfold_PlusHerwig");
  
//  RooUnfoldBayes    unfoldCJet_PlusHerwig (&responseCJetHerwig, hMeasCJet_Plus, 10);    // OR
  // RooUnfoldSvd      unfoldCJet_PlusHerwig (&responseCJetHerwig, hMeasCJet_Plus, 20);   // OR
  RooUnfoldBinByBin unfoldCJet_PlusHerwig (&responseCJetHerwig, hMeasCJet_Plus);
  TH1D* hRecoCJet_PlusHerwig = (TH1D*) unfoldCJet_PlusHerwig.Hreco(3);
  hRecoCJet_PlusHerwig->SetName("CJetUnfold_PlusHerwig");


//  RooUnfoldBayes    unfoldFJet_MinusHerwig (&responseFJetHerwig, hMeasFJet_Minus, 10);    // OR
  // RooUnfoldSvd      unfoldFJet_MinusHerwig (&responseFJetHerwig, hMeasFJet_Minus, 20);   // OR
  RooUnfoldBinByBin unfoldFJet_MinusHerwig (&responseFJetHerwig, hMeasFJet_Minus);
  TH1D* hRecoFJet_MinusHerwig = (TH1D*) unfoldFJet_MinusHerwig.Hreco(3);
  hRecoFJet_MinusHerwig->SetName("FJetUnfold_MinusHerwig");
  
//  RooUnfoldBayes    unfoldCJet_MinusHerwig (&responseCJetHerwig, hMeasCJet_Minus, 10);    // OR
  // RooUnfoldSvd      unfoldCJet_MinusHerwig (&responseCJetHerwig, hMeasCJet_Minus, 20);   // OR
  RooUnfoldBinByBin unfoldCJet_MinusHerwig (&responseCJetHerwig, hMeasCJet_Minus);
  TH1D* hRecoCJet_MinusHerwig = (TH1D*) unfoldCJet_MinusHerwig.Hreco(3);
  hRecoCJet_MinusHerwig->SetName("CJetUnfold_MinusHerwig");
    
  hRecoFJetHerwig->Sumw2();
  hRecoCJetHerwig->Sumw2();
  hRecoFJet_PlusHerwig->Sumw2();
  hRecoCJet_PlusHerwig->Sumw2();
  hRecoFJet_MinusHerwig->Sumw2();
  hRecoCJet_MinusHerwig->Sumw2();
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
//   hRecoFJetHerwig->SetBinContent(iBinX+1,hRecoFJetHerwig->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
//   hRecoCJetHerwig->SetBinContent(iBinX+1,hRecoCJetHerwig->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));    
//   hRecoFJet_PlusHerwig->SetBinContent(iBinX+1,hRecoFJet_PlusHerwig->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
//   hRecoCJet_PlusHerwig->SetBinContent(iBinX+1,hRecoCJet_PlusHerwig->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));    
//   hRecoFJet_MinusHerwig->SetBinContent(iBinX+1,hRecoFJet_MinusHerwig->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
//   hRecoCJet_MinusHerwig->SetBinContent(iBinX+1,hRecoCJet_MinusHerwig->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));    
  }

  cout << "===================================== DRAW ====================================" << endl;
  
  TCanvas* ccFJetHerwig = new TCanvas("ccFJetHerwig","ccFJetHerwig",600,600);  
  hRecoFJetHerwig->GetYaxis()->SetTitle("d#sigma/dp_{T} (pb/GeV)");
  hRecoFJetHerwig->GetXaxis()->SetTitle("forward jet p_{T} (GeV)");
  hRecoFJetHerwig->SetLineColor(kBlue);
  hRecoFJetHerwig->SetMarkerColor(kBlue);
  hRecoFJetHerwig->SetMarkerStyle(20);
  hRecoFJetHerwig->SetMarkerSize(1);
  hRecoFJetHerwig->DrawCopy();

  hRecoFJet_PlusHerwig->GetXaxis()->SetTitle("p_{T} had");
  hRecoFJet_PlusHerwig->SetFillColor(kRed);  
  hRecoFJet_PlusHerwig->SetLineColor(kRed);
  hRecoFJet_PlusHerwig->SetMarkerColor(kRed);
  hRecoFJet_PlusHerwig->SetMarkerStyle(20);
  hRecoFJet_PlusHerwig->SetMarkerSize(1);
  hRecoFJet_PlusHerwig->DrawCopy("SAME");
  
  hRecoFJet_MinusHerwig->GetXaxis()->SetTitle("p_{T} had");
  hRecoFJet_MinusHerwig->SetFillColor(kRed);
  hRecoFJet_MinusHerwig->SetLineColor(kRed);
  hRecoFJet_MinusHerwig->SetMarkerColor(kRed);
  hRecoFJet_MinusHerwig->SetMarkerStyle(20);
  hRecoFJet_MinusHerwig->SetMarkerSize(1);
  hRecoFJet_MinusHerwig->DrawCopy("SAME");
  
  hRecoFJetHerwig->DrawCopy("SAME");
  gPad->BuildLegend();
  gPad->SetGrid();
 
  
  TCanvas* ccCJetHerwig = new TCanvas("ccCJetHerwig","ccCJetHerwig",600,600);  
  hRecoCJetHerwig->GetYaxis()->SetTitle("d#sigma/dp_{T} (pb/GeV)");
  hRecoCJetHerwig->GetXaxis()->SetTitle("central jet p_{T} (GeV)");
  hRecoCJetHerwig->SetLineColor(kBlue);
  hRecoCJetHerwig->SetMarkerColor(kBlue);
  hRecoCJetHerwig->SetMarkerStyle(20);
  hRecoCJetHerwig->SetMarkerSize(1);
  hRecoCJetHerwig->DrawCopy();

  hRecoCJet_PlusHerwig->GetXaxis()->SetTitle("p_{T} had");
  hRecoCJet_PlusHerwig->SetFillColor(kRed);
  hRecoCJet_PlusHerwig->SetLineColor(kRed);
  hRecoCJet_PlusHerwig->SetMarkerColor(kRed);
  hRecoCJet_PlusHerwig->SetMarkerStyle(20);
  hRecoCJet_PlusHerwig->SetMarkerSize(1);
  hRecoCJet_PlusHerwig->DrawCopy("SAME");
  
  hRecoCJet_MinusHerwig->GetXaxis()->SetTitle("p_{T} had");
  hRecoCJet_MinusHerwig->SetFillColor(kRed);
  hRecoCJet_MinusHerwig->SetLineColor(kRed);
  hRecoCJet_MinusHerwig->SetMarkerColor(kRed);
  hRecoCJet_MinusHerwig->SetMarkerStyle(20);
  hRecoCJet_MinusHerwig->SetMarkerSize(1);
  hRecoCJet_MinusHerwig->DrawCopy("SAME");

  hRecoCJetHerwig->DrawCopy("SAME");
  gPad->BuildLegend();
  gPad->SetGrid();
  
  
  cout << "===================================== DRAW ====================================" << endl;

  TH1D* hRecoFJet_MeanHerwig = new TH1D ("hRecoFJet_MeanHerwig", "UnFolded FJet JES Herwig", NBIN , lowEdge);
  TH1D* hRecoCJet_MeanHerwig = new TH1D ("hRecoCJet_MeanHerwig", "UnFolded CJet JES Herwig", NBIN , lowEdge);  

 for (int iBinX = 0; iBinX<NBIN; iBinX++){
  hRecoFJet_MeanHerwig->SetBinContent(iBinX+1,(hRecoFJet_PlusHerwig->GetBinContent(iBinX+1) + hRecoFJet_MinusHerwig->GetBinContent(iBinX+1))/2.);
  hRecoCJet_MeanHerwig->SetBinContent(iBinX+1,(hRecoCJet_PlusHerwig->GetBinContent(iBinX+1) + hRecoCJet_MinusHerwig->GetBinContent(iBinX+1))/2.);

  hRecoFJet_MeanHerwig->SetBinError(iBinX+1,fabs(hRecoFJet_PlusHerwig->GetBinContent(iBinX+1) - hRecoFJet_MinusHerwig->GetBinContent(iBinX+1))/2.);
  hRecoCJet_MeanHerwig->SetBinError(iBinX+1,fabs(hRecoCJet_PlusHerwig->GetBinContent(iBinX+1) - hRecoCJet_MinusHerwig->GetBinContent(iBinX+1))/2.);     
 }

 
  TCanvas* ccFJet_JESHerwig = new TCanvas("ccFJet_JESHerwig","ccFJet_JESHerwig",600,600);  

  hRecoFJet_MeanHerwig->GetYaxis()->SetTitle("d#sigma/dp_{T} (pb/GeV)");
  hRecoFJet_MeanHerwig->GetXaxis()->SetTitle("forward jet p_{T} (GeV)");
  hRecoFJet_MeanHerwig->SetFillColor(kRed);  
  hRecoFJet_MeanHerwig->SetLineColor(kRed);  
  hRecoFJet_MeanHerwig->SetMarkerColor(kRed); 
  hRecoFJet_MeanHerwig->DrawCopy("E2");

  hRecoFJetHerwig->GetXaxis()->SetTitle("p_{T} had");
  hRecoFJetHerwig->SetLineColor(kBlue);
  hRecoFJetHerwig->SetMarkerColor(kBlue);
  hRecoFJetHerwig->SetMarkerStyle(20);
  hRecoFJetHerwig->SetMarkerSize(1);
  hRecoFJetHerwig->DrawCopy("SAMEE");
  
  hTrueFJet_MCHerwig->SetLineColor(kGreen);
  hTrueFJet_MCHerwig->SetMarkerColor(kGreen);
  hTrueFJet_MCHerwig->SetMarkerStyle(21);
  hTrueFJet_MCHerwig->SetMarkerSize(1);
  hTrueFJet_MCHerwig->DrawCopy("SAMEE");
  
  gPad->BuildLegend();
  gPad->SetGrid();

  
  TCanvas* ccCJet_JESHerwig = new TCanvas("ccCJet_JESHerwig","ccCJet_JESHerwig",600,600);  

  hRecoCJet_MeanHerwig->GetYaxis()->SetTitle("d#sigma/dp_{T} (pb/GeV)");
  hRecoCJet_MeanHerwig->GetXaxis()->SetTitle("central jet p_{T} (GeV)");
  hRecoCJet_MeanHerwig->SetFillColor(kRed);  
  hRecoCJet_MeanHerwig->SetLineColor(kRed);  
  hRecoCJet_MeanHerwig->SetMarkerColor(kRed); 
  hRecoCJet_MeanHerwig->DrawCopy("E2");

  hRecoCJetHerwig->GetXaxis()->SetTitle("p_{T} had");
  hRecoCJetHerwig->SetLineColor(kBlue);
  hRecoCJetHerwig->SetMarkerColor(kBlue);
  hRecoCJetHerwig->SetMarkerStyle(20);
  hRecoCJetHerwig->SetMarkerSize(1);
  hRecoCJetHerwig->DrawCopy("SAMEE");

  hTrueCJet_MCHerwig->SetLineColor(kGreen);
  hTrueCJet_MCHerwig->SetMarkerColor(kGreen);
  hTrueCJet_MCHerwig->SetMarkerStyle(21);
  hTrueCJet_MCHerwig->SetMarkerSize(1);
  hTrueCJet_MCHerwig->DrawCopy("SAMEE");

  gPad->BuildLegend();
  gPad->SetGrid(); */
  
  cout << "==================================== JES test =====================================" << endl;
    
  
  
  TCanvas* ccCJet_JESHerwigPythia = new TCanvas("ccCJet_JESHerwigPythia","ccCJet_JESHerwigPythia",600,600);  

 /* hRecoCJet_MeanHerwig->GetYaxis()->SetTitle("d#sigma/dp_{T} (pb/GeV)");
  hRecoCJet_MeanHerwig->GetXaxis()->SetTitle("central jet p_{T} (GeV)");
  hRecoCJet_MeanHerwig->SetFillStyle(3001);  
  hRecoCJet_MeanHerwig->SetFillColor(kYellow);  
  hRecoCJet_MeanHerwig->SetLineColor(kYellow);  
  hRecoCJet_MeanHerwig->SetMarkerColor(kYellow); 
  hRecoCJet_MeanHerwig->DrawCopy("E2");

  hRecoCJetHerwig->GetXaxis()->SetTitle("p_{T} had");
  hRecoCJetHerwig->SetLineColor(kOrange);
  hRecoCJetHerwig->SetMarkerColor(kOrange);
  hRecoCJetHerwig->SetMarkerStyle(20);
  hRecoCJetHerwig->SetMarkerSize(1);
  hRecoCJetHerwig->DrawCopy("SAMEE");*/

  hRecoCJet_MeanPythia->GetYaxis()->SetTitle("d#sigma/dp_{T} (pb/GeV)");
  hRecoCJet_MeanPythia->GetXaxis()->SetTitle("central jet p_{T} (GeV)");
  hRecoCJet_MeanPythia->SetFillStyle(3004);  
  hRecoCJet_MeanPythia->SetStats(0);
  hRecoCJet_MeanPythia->SetFillColor(kBlue);  
  hRecoCJet_MeanPythia->SetLineColor(kBlue);  
  hRecoCJet_MeanPythia->SetMarkerColor(kBlue); 
  hRecoCJet_MeanPythia->GetXaxis()->SetRangeUser(35.,149);
  hRecoCJet_MeanPythia->DrawCopy("E2");

  hRecoCJetPythia->GetXaxis()->SetTitle("p_{T} had");
  hRecoCJetPythia->SetLineColor(kAzure);
  hRecoCJetPythia->SetMarkerColor(kAzure);
  hRecoCJetPythia->SetMarkerStyle(20);
  hRecoCJetPythia->SetMarkerSize(1);
  hRecoCJetPythia->DrawCopy("SAMEE");

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
  
  //hTrueCJet_MCPowheg->SetLineColor(kGreen);
  //hTrueCJet_MCPowheg->SetMarkerColor(kGreen);
  hTrueCJet_MCPowheg->SetMarkerStyle(21);
  hTrueCJet_MCPowheg->SetMarkerSize(1);
  hTrueCJet_MCPowheg->DrawCopy("SAMEE");
  
  ccCJet_JESHerwigPythia->SetLogy();

  gPad->BuildLegend();
  gPad->SetGrid(); 

 
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
//    std::cerr << " iBinX " << iBinX << "[" << lowEdge[iBinX] << " ~ " << lowEdge[iBinX+1] << "] :: POWHEG :: " << hTrueCJet_MCPowheg->GetBinContent(iBinX+1) << std::endl;
   std::cerr << " iBinX " << iBinX << "[" << lowEdge[iBinX] << " ~ " << lowEdge[iBinX+1] << "] :: Pythia/POWHEG :: " << hTrueCJet_MCPythia->GetBinContent(iBinX+1)/hTrueCJet_MCPowheg->GetBinContent(iBinX+1) << std::endl;
   std::cerr << " iBinX " << iBinX << "[" << lowEdge[iBinX] << " ~ " << lowEdge[iBinX+1] << "] :: Herwig/POWHEG :: " << hTrueCJet_MCHerwig->GetBinContent(iBinX+1)/hTrueCJet_MCPowheg->GetBinContent(iBinX+1) << std::endl;

  }
  
  
  TCanvas* ccFJet_JESHerwigPythia = new TCanvas("ccFJet_JESHerwigPythia","ccFJet_JESHerwigPythia",600,600);  

 /* hRecoFJet_MeanHerwig->GetYaxis()->SetTitle("d#sigma/dp_{T} (pb/GeV)");
  hRecoFJet_MeanHerwig->GetXaxis()->SetTitle("forward jet p_{T} (GeV)");
  hRecoFJet_MeanHerwig->SetFillStyle(3001);  
  hRecoFJet_MeanHerwig->SetFillColor(kYellow);  
  hRecoFJet_MeanHerwig->SetLineColor(kYellow);  
  hRecoFJet_MeanHerwig->SetMarkerColor(kYellow); 
  hRecoFJet_MeanHerwig->DrawCopy("E2");

  hRecoFJetHerwig->GetXaxis()->SetTitle("p_{T} had");
  hRecoFJetHerwig->SetLineColor(kOrange);
  hRecoFJetHerwig->SetMarkerColor(kOrange);
  hRecoFJetHerwig->SetMarkerStyle(20);
  hRecoFJetHerwig->SetMarkerSize(1);
  hRecoFJetHerwig->DrawCopy("SAMEE");*/
  
  hRecoFJet_MeanPythia->GetYaxis()->SetTitle("d#sigma/dp_{T} (pb/GeV)");
  hRecoFJet_MeanPythia->GetXaxis()->SetTitle("forward jet p_{T} (GeV)");
  hRecoFJet_MeanPythia->SetFillStyle(3004);  
  hRecoFJet_MeanPythia->SetStats(0);
  hRecoFJet_MeanPythia->SetFillColor(kBlue);  
  hRecoFJet_MeanPythia->SetLineColor(kBlue);  
  hRecoFJet_MeanPythia->SetMarkerColor(kBlue);
  //hRecoFJet_MeanPythia->GetYaxis()->SetRangeUser(0.,1.0);
  hRecoFJet_MeanPythia->GetXaxis()->SetRangeUser(35.,149);
  hRecoFJet_MeanPythia->DrawCopy("E2");

  hRecoFJetPythia->GetXaxis()->SetTitle("p_{T} had");
  hRecoFJetPythia->SetLineColor(kAzure);
  hRecoFJetPythia->SetMarkerColor(kAzure);
  hRecoFJetPythia->SetMarkerStyle(20);
  hRecoFJetPythia->SetMarkerSize(1);
  hRecoFJetPythia->DrawCopy("SAMEE");

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
  
  //hTrueCJet_MCPowheg->SetLineColor(kGreen);
  //hTrueCJet_MCPowheg->SetMarkerColor(kGreen);
  hTrueFJet_MCPowheg->SetMarkerStyle(21);
  hTrueFJet_MCPowheg->SetMarkerSize(1);
  hTrueFJet_MCPowheg->DrawCopy("SAMEE");
  
  ccFJet_JESHerwigPythia->SetLogy();

  gPad->BuildLegend();
  gPad->SetGrid(); 

  
  
}

#ifndef __CINT__
int main () { Data_POWHEG(); return 0; }  // Main program when run stand-alone
#endif
