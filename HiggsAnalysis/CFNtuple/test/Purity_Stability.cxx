//=====================================================================-*-C++-*-
// File and Version Information:
//      $Id: Purity_Stability.cxx,v 1.1 2011/02/09 16:37:40 amassiro Exp $
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
#include "math.h"

#endif

//#include "tdrStyle.cc"

//==============================================================================
// Global definitions
//==============================================================================

//==============================================================================
// Unfolding for C-F jet analysis
//==============================================================================

void Purity_Stability()
{
#ifdef __CINT__
  gSystem->Load("libRooUnfold");
#endif
  
//  setTDRStyle();


  
  Float_t lowEdge[9] = {27,35,45,57,72,90,120,150,1000};
  int NBIN = 8;
    
 // Float_t lowEdge[11] = {18,22,27,35,45,57,72,90,120,150,1000};
 // int NBIN = 10;
  
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
  
  TFile FileTrain1Pythia("../input/Unfolding/qcd_15_pythia.root","READ");
  TFile FileTrain2Pythia("../input/Unfolding/qcd_30_pythia.root","READ");

  TFile FileTrain1Herwig("../input/Unfolding/qcd_15_herwigjimmy.root","READ");
  TFile FileTrain2Herwig("../input/Unfolding/qcd_30_herwigjimmy.root","READ");


  double lumi = 5.1e-3;
  
  double xsec1Pythia = 8.762e8 / 6190500.;
  double xsec2Pythia = 6.041e7 / 4918016.;
  double xsec1Herwig = 714000000. / 1631667.;
  double xsec2Herwig =  49240000. / 1310829.;
  double Threshold_G_FJet = 57;
  double Threshold_G_CJet = 57;
  
  TTree* TreeTrain1Pythia = (TTree*) FileTrain1Pythia.Get("AnaHiggs");
  TTree* TreeTrain2Pythia = (TTree*) FileTrain2Pythia.Get("AnaHiggs");
 
  TTree* TreeTrain1Herwig = (TTree*) FileTrain1Herwig.Get("AnaHiggs");
  TTree* TreeTrain2Herwig = (TTree*) FileTrain2Herwig.Get("AnaHiggs");
  
  Float_t G_FJet_Pt; //~~~~ had
  Float_t S_FJet_Pt; //~~~~ reco

  Float_t G_CJet_Pt; //~~~~ had
  Float_t S_CJet_Pt; //~~~~ reco
 
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


  cout << "=================================================================================" << endl;
  cout << "==================================== Pythia =====================================" << endl;

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
 
  
//  hResponseMatrixFJet->Sumw2();
//  hResponseMatrixCJet->Sumw2();
  
  hResponseMatrixFJetPythia->Add(hResponseMatrixFJet_tempPythia);
  hResponseMatrixCJetPythia->Add(hResponseMatrixCJet_tempPythia);
  
  
        
  cout << "==================================== Purity and Stability =====================================" << endl;
  //PURITY
  
  //TH::GetYaxis()->SetRangeUser(min,max)
  TH1D* Purity = new TH1D ("Purity", "Pythia",NBIN,lowEdge);
  TH1D* Stability = new TH1D ("Stability", "Pythia",NBIN,lowEdge);
  
//for forward Jets
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
    double purity = 0;
    double column = 0;
    for (int iBinY = 0; iBinY<NBIN; iBinY++){
      
      column = column + hResponseMatrixFJetPythia->GetBinContent(iBinX+1,iBinY+1);
    }
    double diag = hResponseMatrixFJetPythia->GetBinContent(iBinX+1,iBinX+1);
    double offd = column - diag;
    purity = diag/column;
    Purity->SetBinContent(iBinX+1, purity);
    
    Purity->SetBinError(iBinX+1,sqrt((offd)/column/column*(offd)/column/column*diag+(diag/column/column)*(diag/column/column)*(offd)));
  }
  
  for (int iBinY = 0; iBinY<NBIN; iBinY++){
    double stability = 0;
    double row = 0;
    for (int iBinX = 0; iBinX<NBIN; iBinX++){
      
      row = row + hResponseMatrixFJetPythia->GetBinContent(iBinX+1,iBinY+1);
    }
    double diag = hResponseMatrixFJetPythia->GetBinContent(iBinY+1,iBinY+1);
    double offd = row - diag;
    stability = diag/row;
    Stability->SetBinContent(iBinY+1, stability);
    Stability->SetBinError(iBinY+1,sqrt((offd)/row/row*(offd)/row/row*diag+(diag/row/row)*(diag/row/row)*(offd)));
    
  }
  
 /* TCanvas* cPurityF = new TCanvas("cPurityF","cPurityF",600,600);  
  Purity->GetYaxis()->SetRangeUser(0.,1.0);
  Purity->GetXaxis()->SetRangeUser(35.,150);
  Purity->GetYaxis()->SetTitle("Purity");
  Purity->GetXaxis()->SetTitle("forward jet p_{T} [GeV/c]");
  Purity->DrawCopy();
  
  
  TCanvas* cStabilityF = new TCanvas("cStabilityF","cStabilityF",600,600);  
  Stability->GetYaxis()->SetRangeUser(0.,1.0);
  Stability->GetXaxis()->SetRangeUser(35.,150);
  Stability->GetYaxis()->SetTitle("Stability");
  Stability->GetXaxis()->SetTitle("forward jet p_{T} [GeV/c]");
  Stability->DrawCopy();
  */
  
  //for central Jets
  TH1D* PurityC = new TH1D ("PurityC", "Pythia",NBIN,lowEdge);
  TH1D* StabilityC = new TH1D ("StabilityC", "Pythia",NBIN,lowEdge);
  
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
    double purity = 0;
    double column = 0;
    for (int iBinY = 0; iBinY<NBIN; iBinY++){
      
      column = column + hResponseMatrixCJetPythia->GetBinContent(iBinX+1,iBinY+1);
    }
    double diag = hResponseMatrixCJetPythia->GetBinContent(iBinX+1,iBinX+1);
    double offd = column - diag;
    purity = hResponseMatrixCJetPythia->GetBinContent(iBinX+1,iBinX+1)/column;
    PurityC->SetBinContent(iBinX+1, purity);
    PurityC->SetBinError(iBinX+1,sqrt((offd)/column/column*(offd)/column/column*diag+(diag/column/column)*(diag/column/column)*(offd)));

  }
  
  for (int iBinY = 0; iBinY<NBIN; iBinY++){
    double stability = 0;
    double row = 0;
    for (int iBinX = 0; iBinX<NBIN; iBinX++){
      
      row = row + hResponseMatrixCJetPythia->GetBinContent(iBinX+1,iBinY+1);
    }
    double diag = hResponseMatrixCJetPythia->GetBinContent(iBinY+1,iBinY+1);
    double offd = row - diag;
    stability = hResponseMatrixCJetPythia->GetBinContent(iBinY+1,iBinY+1)/row;
    StabilityC->SetBinContent(iBinY+1, stability);
    StabilityC->SetBinError(iBinY+1,sqrt((offd)/row/row*(offd)/row/row*diag+(diag/row/row)*(diag/row/row)*(offd)));

  }
  
  /*TCanvas* cPurityC = new TCanvas("cPurityC","cPurityC",600,600);  
  PurityC->GetYaxis()->SetRangeUser(0.,1.0);
  PurityC->GetXaxis()->SetRangeUser(35.,150);
  PurityC->GetYaxis()->SetTitle("Purity");
  PurityC->GetXaxis()->SetTitle("central jet p_{T} [GeV/c]");
  PurityC->DrawCopy();
  
  
  TCanvas* cStabilityC = new TCanvas("cStabilityC","cStabilityC",600,600);  
  StabilityC->GetYaxis()->SetRangeUser(0.,1.0);
  StabilityC->GetXaxis()->SetRangeUser(35.,150);
  StabilityC->GetYaxis()->SetTitle("Stability");
  StabilityC->GetXaxis()->SetTitle("central jet p_{T} [GeV/c]");
  StabilityC->DrawCopy();
  */
  
  //draw Unweighted response matrix
  
  TCanvas* ccResponseCJetPythia = new TCanvas("ccResponseCJetPythia","ccResponseCJetPythia",600,600);  
  hResponseMatrixCJetPythia->SetLineColor(kRed);
  hResponseMatrixCJetPythia->SetMarkerColor(kRed);
  hResponseMatrixCJetPythia->SetMarkerStyle(20);
  hResponseMatrixCJetPythia->SetStats(0);
  hResponseMatrixCJetPythia->Draw("text");
  ccResponseCJetPythia->SetLogx();
  ccResponseCJetPythia->SetLogy();
  ccResponseCJetPythia->SetLogz();
  
  TCanvas* ccResponseFJetPythia = new TCanvas("ccResponseFJetPythia","ccResponseFJetPythia",600,600);  
  hResponseMatrixFJetPythia->SetLineColor(kRed);
  hResponseMatrixFJetPythia->SetMarkerColor(kRed);
  hResponseMatrixFJetPythia->SetMarkerStyle(20);
  hResponseMatrixFJetPythia->SetStats(0);
  hResponseMatrixFJetPythia->Draw("text");
  ccResponseFJetPythia->SetLogx();
  ccResponseFJetPythia->SetLogy();
  ccResponseFJetPythia->SetLogz();
  
  
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~ scale for variable size binning ~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  
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
   if (S_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0 && G_FJet_Pt) {
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
   if (S_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0 && G_FJet_Pt) {
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





  cout << "=================================================================================" << endl;
  cout << "==================================== Herwig =====================================" << endl;

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
 
//  hResponseMatrixFJet->Sumw2();
//  hResponseMatrixCJet->Sumw2();
  
  hResponseMatrixFJetHerwig->Add(hResponseMatrixFJet_tempHerwig);
  hResponseMatrixCJetHerwig->Add(hResponseMatrixCJet_tempHerwig);
  
  
    cout << "==================================== Purity and Stability =====================================" << endl;
  //PURITY
  
  //TH::GetYaxis()->SetRangeUser(min,max)
  TH1D* PurityH = new TH1D ("PurityH", " Herwig",NBIN,lowEdge);
  TH1D* StabilityH = new TH1D ("StabilityH", "Herwig",NBIN,lowEdge);
  
//for forward Jets
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
    double purity = 0;
    double column = 0;
    for (int iBinY = 0; iBinY<NBIN; iBinY++){
      
      column = column + hResponseMatrixFJetHerwig->GetBinContent(iBinX+1,iBinY+1);
    }
    double diag = hResponseMatrixFJetHerwig->GetBinContent(iBinX+1,iBinX+1);
    double offd = column - diag;
    purity = hResponseMatrixFJetHerwig->GetBinContent(iBinX+1,iBinX+1)/column;
    PurityH->SetBinContent(iBinX+1, purity);
    PurityH->SetBinError(iBinX+1,sqrt((offd)/column/column*(offd)/column/column*diag+(diag/column/column)*(diag/column/column)*(offd)));

  }
  
  for (int iBinY = 0; iBinY<NBIN; iBinY++){
    double stability = 0;
    double row = 0;
    for (int iBinX = 0; iBinX<NBIN; iBinX++){
      
      row = row + hResponseMatrixFJetHerwig->GetBinContent(iBinX+1,iBinY+1);
    }
    double diag = hResponseMatrixFJetHerwig->GetBinContent(iBinY+1,iBinY+1);
    double offd = row - diag;
    stability = hResponseMatrixFJetHerwig->GetBinContent(iBinY+1,iBinY+1)/row;
    StabilityH->SetBinContent(iBinY+1, stability);
    StabilityH->SetBinError(iBinY+1,sqrt((offd)/row/row*(offd)/row/row*diag+(diag/row/row)*(diag/row/row)*(offd)));

  }
  
  TCanvas* cPurityF = new TCanvas("cPurityF","cPurityF",600,600);  
  Purity->GetYaxis()->SetRangeUser(0.,1.0);
  Purity->GetXaxis()->SetRangeUser(35.,149);
  Purity->SetTitle();
  Purity->SetMarkerStyle(20);
  Purity->SetMarkerSize(1);
  Purity->GetYaxis()->SetTitle("Purity");
  Purity->GetXaxis()->SetTitle("forward jet p_{T} [GeV/c]");
  Purity->SetStats(0);
  Purity->DrawCopy("E");
  
  PurityH->GetYaxis()->SetRangeUser(0.,1.0);
  PurityH->GetXaxis()->SetRangeUser(35.,149);
  PurityH->SetTitle();
  PurityH->SetLineColor(kRed);
  PurityH->GetYaxis()->SetTitle("Purity");
  PurityH->SetMarkerColor(kRed);
  PurityH->SetMarkerStyle(20);
  PurityH->SetMarkerSize(1);
  PurityH->GetXaxis()->SetTitle("forward jet p_{T} [GeV/c]");
  PurityH->SetStats(0);
  PurityH->DrawCopy("SAMEE");
  
   leg = new TLegend(0.6,0.7,0.9,0.9,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   
   leg->AddEntry("Purity","Pythia", "lpf");
   //entry = leg->AddEntry(central_M1,"Herwig+Jimmy ","lp");
   leg->AddEntry("PurityH","Herwig", "lpf");
   leg->Draw();
  
  TCanvas* cStabilityF = new TCanvas("cStabilityF","cStabilityF",600,600);  
  Stability->GetYaxis()->SetRangeUser(0.,1.0);
  Stability->GetXaxis()->SetRangeUser(35.,149.);
  Stability->SetTitle();
  Stability->SetMarkerStyle(20);
  Stability->SetMarkerSize(1);
  Stability->GetYaxis()->SetTitle("Stability");
  Stability->GetXaxis()->SetTitle("forward jet p_{T} [GeV/c]");
  Stability->SetStats(0);
  Stability->DrawCopy("E");

  StabilityH->GetYaxis()->SetRangeUser(0.,1.0);
  StabilityH->GetXaxis()->SetRangeUser(35.,149.);
  StabilityH->SetLineColor(kRed);
  StabilityH->SetMarkerColor(kRed);
  StabilityH->SetMarkerStyle(20);
  StabilityH->SetMarkerSize(1);
  StabilityH->GetYaxis()->SetTitle("Stability");
  StabilityH->GetXaxis()->SetTitle("forward jet p_{T} [GeV/c]");
  StabilityH->SetStats(0);
  StabilityH->DrawCopy("SAMEE");
  
   leg = new TLegend(0.6,0.7,0.9,0.9,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   
   leg->AddEntry("Stability","Pythia", "lpf");
   //entry = leg->AddEntry(central_M1,"Herwig+Jimmy ","lp");
   leg->AddEntry("StabilityH","Herwig", "lpf");
   leg->Draw();  
  
  //for central Jets
  TH1D* PurityCH = new TH1D ("PurityCH", "Herwig",NBIN,lowEdge);
  TH1D* StabilityCH = new TH1D ("StabilityCH", "Herwig",NBIN,lowEdge);
  
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
    double purity = 0;
    double column = 0;
    for (int iBinY = 0; iBinY<NBIN; iBinY++){
      
      column = column + hResponseMatrixCJetHerwig->GetBinContent(iBinX+1,iBinY+1);
    }
    double diag = hResponseMatrixCJetHerwig->GetBinContent(iBinX+1,iBinX+1);
    double offd = column - diag;
    purity = hResponseMatrixCJetHerwig->GetBinContent(iBinX+1,iBinX+1)/column;
    PurityCH->SetBinContent(iBinX+1, purity);
    PurityCH->SetBinError(iBinX+1,sqrt((offd)/column/column*(offd)/column/column*diag+(diag/column/column)*(diag/column/column)*(offd)));

  }
  
  for (int iBinY = 0; iBinY<NBIN; iBinY++){
    double stability = 0;
    double row = 0;
    for (int iBinX = 0; iBinX<NBIN; iBinX++){
      
      row = row + hResponseMatrixCJetHerwig->GetBinContent(iBinX+1,iBinY+1);
    }
    double diag = hResponseMatrixCJetHerwig->GetBinContent(iBinY+1,iBinY+1);
    double offd = row - diag;
    stability = hResponseMatrixCJetHerwig->GetBinContent(iBinY+1,iBinY+1)/row;
    StabilityCH->SetBinContent(iBinY+1, stability);
    StabilityCH->SetBinError(iBinY+1,sqrt((offd)/row/row*(offd)/row/row*diag+(diag/row/row)*(diag/row/row)*(offd)));

  }
  
  TCanvas* cPurityC = new TCanvas("cPurityC","cPurityC",600,600);  
  PurityC->GetYaxis()->SetRangeUser(0.,1.0);
  PurityC->GetXaxis()->SetRangeUser(35.,149);
  PurityC->SetTitle();
  PurityC->SetMarkerSize(1);
  PurityC->SetMarkerStyle(20);
  PurityC->GetYaxis()->SetTitle("Purity");
  PurityC->GetXaxis()->SetTitle("central jet p_{T} [GeV/c]");
  PurityC->SetStats(0);
  PurityC->DrawCopy("E");
  
  PurityCH->GetYaxis()->SetRangeUser(0.,1.0);
  PurityCH->GetXaxis()->SetRangeUser(35.,149);
  PurityCH->SetLineColor(kRed);
  PurityCH->SetMarkerColor(kRed);
  PurityCH->SetMarkerSize(1);
  PurityCH->SetMarkerStyle(20);
  PurityCH->GetYaxis()->SetTitle("Purity");
  PurityCH->GetXaxis()->SetTitle("central jet p_{T} [GeV/c]");
  PurityCH->SetStats(0);
  PurityCH->DrawCopy("SAMEE");
  
   leg = new TLegend(0.6,0.7,0.9,0.9,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   
   leg->AddEntry("PurityC","Pythia", "lpf");
   //entry = leg->AddEntry(central_M1,"Herwig+Jimmy ","lp");
   leg->AddEntry("PurityCH","Herwig", "lpf");
   leg->Draw();    
  
  TCanvas* cStabilityC = new TCanvas("cStabilityC","cStabilityC",600,600);  
  StabilityC->GetYaxis()->SetRangeUser(0.,1.0);
  StabilityC->GetXaxis()->SetRangeUser(35.,149);
  StabilityC->SetMarkerStyle(20);
  StabilityC->SetMarkerSize(1);
  StabilityC->SetTitle();
  StabilityC->GetYaxis()->SetTitle("Stability");
  StabilityC->GetXaxis()->SetTitle("central jet p_{T} [GeV/c]");
  StabilityC->SetStats(0);
  StabilityC->DrawCopy("E");
  
  StabilityCH->GetYaxis()->SetRangeUser(0.,1.0);
  StabilityCH->GetXaxis()->SetRangeUser(35.,149);
  StabilityCH->SetLineColor(kRed);
  StabilityCH->SetMarkerColor(kRed);
  StabilityCH->SetMarkerStyle(20);
  StabilityCH->SetMarkerSize(1);
  StabilityCH->GetYaxis()->SetTitle("Stability");
  StabilityCH->GetXaxis()->SetTitle("central jet p_{T} [GeV/c]");
  StabilityCH->SetStats(0);
  StabilityCH->DrawCopy("SAMEE");
  
   leg = new TLegend(0.6,0.7,0.9,0.9,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   
   leg->AddEntry("StabilityC","Pythia", "lpf");
   //entry = leg->AddEntry(central_M1,"Herwig+Jimmy ","lp");
   leg->AddEntry("StabilityCH","Herwig", "lpf");
   leg->Draw();    
   
   //Draw unweighted response matrix
   
  TCanvas* ccResponseCJetHerwig = new TCanvas("ccResponseCJetHerwig","ccResponseCJetHerwig",600,600);  
  hResponseMatrixCJetHerwig->SetLineColor(kRed);
  hResponseMatrixCJetHerwig->SetMarkerColor(kRed);
  hResponseMatrixCJetHerwig->SetMarkerStyle(20);
  hResponseMatrixCJetHerwig->SetStats(0);
  hResponseMatrixCJetHerwig->Draw("text");
  ccResponseFJetPythia->SetLogx();
  ccResponseFJetPythia->SetLogy();
  ccResponseCJetHerwig->SetLogz();
  
  TCanvas* ccResponseFJetHerwig = new TCanvas("ccResponseFJetHerwig","ccResponseFJetHerwig",600,600);  
  hResponseMatrixFJetHerwig->SetLineColor(kRed);
  hResponseMatrixFJetHerwig->SetMarkerColor(kRed);
  hResponseMatrixFJetHerwig->SetMarkerStyle(20);
  hResponseMatrixFJetHerwig->SetStats(0);
  hResponseMatrixFJetHerwig->Draw("text");
  ccResponseFJetPythia->SetLogx();
  ccResponseFJetPythia->SetLogy();
  ccResponseFJetHerwig->SetLogz();
  
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ scale for variable size binning ~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

  
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   for (int iBinY = 0; iBinY<NBIN; iBinY++){

//    hResponseMatrixFJet->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixFJet->GetBinContent(iBinX+1,iBinY+1) / xsecScale ) ;
//    hResponseMatrixCJet->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixCJet->GetBinContent(iBinX+1,iBinY+1) / xsecScale ) ;

    hResponseMatrixFJetHerwig->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixFJetHerwig->GetBinContent(iBinX+1,iBinY+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / (lowEdge[iBinY+1] - lowEdge[iBinY]));
    hResponseMatrixCJetHerwig->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixCJetHerwig->GetBinContent(iBinX+1,iBinY+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / (lowEdge[iBinY+1] - lowEdge[iBinY]));

    hResponseMatrixFJetHerwig->SetBinError(iBinX+1,iBinY+1,hResponseMatrixFJetHerwig->GetBinError(iBinX+1,iBinY+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / (lowEdge[iBinY+1] - lowEdge[iBinY]));
    hResponseMatrixCJetHerwig->SetBinError(iBinX+1,iBinY+1,hResponseMatrixCJetHerwig->GetBinError(iBinX+1,iBinY+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / (lowEdge[iBinY+1] - lowEdge[iBinY]));
    
    
   }
  }
  
  /*TCanvas* ccResponseCJetHerwig = new TCanvas("ccResponseCJetHerwig","ccResponseCJetHerwig",600,600);  
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
   if (S_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0 && G_FJet_Pt) {
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
   if (S_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0 && G_FJet_Pt) {
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
   hTrueFJet_MCHerwig->SetBinError(iBinX+1,hTrueFJet_MCHerwig->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hTrueCJet_MCHerwig->SetBinError(iBinX+1,hTrueCJet_MCHerwig->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hMeasFJet_MCHerwig->SetBinError(iBinX+1,hMeasFJet_MCHerwig->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hMeasCJet_MCHerwig->SetBinError(iBinX+1,hMeasCJet_MCHerwig->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));  
  
   hTrueFJet_MCHerwig->SetBinContent(iBinX+1,hTrueFJet_MCHerwig->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hTrueCJet_MCHerwig->SetBinContent(iBinX+1,hTrueCJet_MCHerwig->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hMeasFJet_MCHerwig->SetBinContent(iBinX+1,hMeasFJet_MCHerwig->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hMeasCJet_MCHerwig->SetBinContent(iBinX+1,hMeasCJet_MCHerwig->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));       
  }
  
  
  
  
  
  
  
}

#ifndef __CINT__
int main () { Purity_Stability(); return 0; }  // Main program when run stand-alone
#endif
