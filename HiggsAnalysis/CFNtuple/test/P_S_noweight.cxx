//=====================================================================-*-C++-*-
// File and Version Information:
//      $Id: P_S_noweight.cxx,v 1.1 2011/02/09 23:44:05 govoni Exp $
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

void P_S_noweight()
{
#ifdef __CINT__
  gSystem->Load("libRooUnfold");
#endif
  
//  setTDRStyle();


  
  Float_t lowEdge[8] = {27,35,45,57,72,90,120,150};
  int NBIN = 7;
  TH1F myH("myH","myH",NBIN,lowEdge);
  TH2D* hResponseMatrixFJetPythia15 = new TH2D ("hResponseMatrixFJetPythia15", "Response Matrix FJet Pythia15",NBIN,lowEdge,NBIN,lowEdge);
  TH2D* hResponseMatrixCJetPythia15 = new TH2D ("hResponseMatrixCJetPythia15", "Response Matrix CJet Pythia15",NBIN,lowEdge,NBIN,lowEdge);
  
  TH2D* hResponseMatrixFJetPythia30 = new TH2D ("hResponseMatrixFJetPythia30", "Response Matrix FJet Pythia30",NBIN,lowEdge,NBIN,lowEdge);
  TH2D* hResponseMatrixCJetPythia30 = new TH2D ("hResponseMatrixCJetPythia30", "Response Matrix CJet Pythia30",NBIN,lowEdge,NBIN,lowEdge);

  //TH2D* hResponseMatrixFJet_tempPythia = new TH2D ("hResponseMatrixFJet_tempPythia", "Response Matrix FJet",NBIN,lowEdge,NBIN,lowEdge);
  //TH2D* hResponseMatrixCJet_tempPythia = new TH2D ("hResponseMatrixCJet_tempPythia", "Response Matrix CJet",NBIN,lowEdge,NBIN,lowEdge);

  TH2D* hResponseMatrixFJetHerwig15 = new TH2D ("hResponseMatrixFJetHerwig15", "Response Matrix FJet Herwig15",NBIN,lowEdge,NBIN,lowEdge);
  TH2D* hResponseMatrixCJetHerwig15 = new TH2D ("hResponseMatrixCJetHerwig15", "Response Matrix CJet Herwig15",NBIN,lowEdge,NBIN,lowEdge);
  TH2D* hResponseMatrixFJetHerwig30 = new TH2D ("hResponseMatrixFJetHerwig30", "Response Matrix FJet Herwig30",NBIN,lowEdge,NBIN,lowEdge);
  TH2D* hResponseMatrixCJetHerwig30 = new TH2D ("hResponseMatrixCJetHerwig30", "Response Matrix CJet Herwig30",NBIN,lowEdge,NBIN,lowEdge);
  
  
  
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
   
    if (S_FJet_Pt>0 && G_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0)  {
     
      hResponseMatrixFJetPythia15->Fill(S_FJet_Pt, G_FJet_Pt);
      hResponseMatrixCJetPythia15->Fill(S_CJet_Pt, G_CJet_Pt);
     
    }
  }
  
  
  for (Int_t iEvt= 0; iEvt<TreeTrain2Pythia->GetEntries(); iEvt++) {
   TreeTrain2Pythia->GetEntry(iEvt);
  
   if (S_FJet_Pt>0 && G_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0)  {
     hResponseMatrixFJetPythia30->Fill(S_FJet_Pt, G_FJet_Pt);
     hResponseMatrixCJetPythia30->Fill(S_CJet_Pt, G_CJet_Pt);
    }
  }
 
  
        
  cout << "==================================== Purity and Stability =====================================" << endl;
  //PURITY
  
  //TH::GetYaxis()->SetRangeUser(min,max)
  TH1D* Purity15 = new TH1D ("Purity15", "Pythia15",NBIN,lowEdge);
  TH1D* Stability15 = new TH1D ("Stability15", "Pythia15",NBIN,lowEdge);

  TH1D* Purity30 = new TH1D ("Purity30", "Pythia30",NBIN,lowEdge);
  TH1D* Stability30 = new TH1D ("Stability30", "Pythia30",NBIN,lowEdge);
  
//for forward Jets smaller than 57
  for (int iBinX = 0; iBinX<3; iBinX++){
    double purity = 0;
    double column = 0;
    for (int iBinY = 0; iBinY<NBIN; iBinY++){
      
      column = column + hResponseMatrixFJetPythia15->GetBinContent(iBinX+1,iBinY+1);
    }
    double diag = hResponseMatrixFJetPythia15->GetBinContent(iBinX+1,iBinX+1);
    double offd = column - diag;
    purity = diag/column;
    Purity15->SetBinContent(iBinX+1, purity);
    Purity15->SetBinError(iBinX+1,sqrt((offd)/column/column*(offd)/column/column*diag+(diag/column/column)*(diag/column/column)*(offd)));
  }
  
  for (int iBinY = 0; iBinY<3; iBinY++){
    double stability = 0;
    double row = 0;
    for (int iBinX = 0; iBinX<NBIN; iBinX++){
      
      row = row + hResponseMatrixFJetPythia15->GetBinContent(iBinX+1,iBinY+1);
    }
    double diag = hResponseMatrixFJetPythia15->GetBinContent(iBinY+1,iBinY+1);
    double offd = row - diag;
    stability = diag/row;
    Stability15->SetBinContent(iBinY+1, stability);
    Stability15->SetBinError(iBinY+1,sqrt((offd)/row/row*(offd)/row/row*diag+(diag/row/row)*(diag/row/row)*(offd)));
    
  }
  
  //for forward Jets greater than 57
  for (int iBinX = 3; iBinX<NBIN; iBinX++){
    double purity = 0;
    double column = 0;
    for (int iBinY = 0; iBinY<NBIN; iBinY++){
      
      column = column + hResponseMatrixFJetPythia30->GetBinContent(iBinX+1,iBinY+1);
    }
    double diag = hResponseMatrixFJetPythia30->GetBinContent(iBinX+1,iBinX+1);
    double offd = column - diag;
    purity = diag/column;
    Purity30->SetBinContent(iBinX+1, purity);
    
    Purity30->SetBinError(iBinX+1,sqrt((offd)/column/column*(offd)/column/column*diag+(diag/column/column)*(diag/column/column)*(offd)));
  }
  
  for (int iBinY = 3; iBinY<NBIN; iBinY++){
    double stability = 0;
    double row = 0;
    for (int iBinX = 0; iBinX<NBIN; iBinX++){
      
      row = row + hResponseMatrixFJetPythia30->GetBinContent(iBinX+1,iBinY+1);
    }
    double diag = hResponseMatrixFJetPythia30->GetBinContent(iBinY+1,iBinY+1);
    double offd = row - diag;
    stability = diag/row;
    Stability30->SetBinContent(iBinY+1, stability);
    Stability30->SetBinError(iBinY+1,sqrt((offd)/row/row*(offd)/row/row*diag+(diag/row/row)*(diag/row/row)*(offd)));
    
  }
  
  Purity15->Add(Purity30);
  Stability15->Add(Stability30);
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
  TH1D* PurityC15 = new TH1D ("PurityC15", "Pythia15",NBIN,lowEdge);
  TH1D* StabilityC15 = new TH1D ("StabilityC15", "Pythia15",NBIN,lowEdge);
  TH1D* PurityC30 = new TH1D ("PurityC30", "Pythia30",NBIN,lowEdge);
  TH1D* StabilityC30 = new TH1D ("StabilityC30", "Pythia30",NBIN,lowEdge);

  
  
  //for central Jets smaller than 57
  for (int iBinX = 0; iBinX<3; iBinX++){
    double purity = 0;
    double column = 0;
    for (int iBinY = 0; iBinY<NBIN; iBinY++){
      
      column = column + hResponseMatrixCJetPythia15->GetBinContent(iBinX+1,iBinY+1);
    }
    double diag = hResponseMatrixCJetPythia15->GetBinContent(iBinX+1,iBinX+1);
    double offd = column - diag;
    purity = diag/column;
    PurityC15->SetBinContent(iBinX+1, purity);
    PurityC15->SetBinError(iBinX+1,sqrt((offd)/column/column*(offd)/column/column*diag+(diag/column/column)*(diag/column/column)*(offd)));
  }
  
  for (int iBinY = 0; iBinY<3; iBinY++){
    double stability = 0;
    double row = 0;
    for (int iBinX = 0; iBinX<NBIN; iBinX++){
      
      row = row + hResponseMatrixCJetPythia15->GetBinContent(iBinX+1,iBinY+1);
    }
    double diag = hResponseMatrixCJetPythia15->GetBinContent(iBinY+1,iBinY+1);
    double offd = row - diag;
    stability = diag/row;
    StabilityC15->SetBinContent(iBinY+1, stability);
    StabilityC15->SetBinError(iBinY+1,sqrt((offd)/row/row*(offd)/row/row*diag+(diag/row/row)*(diag/row/row)*(offd)));
    
  }
  
  //for central Jets greater than 57
  for (int iBinX = 3; iBinX<NBIN; iBinX++){
    double purity = 0;
    double column = 0;
    for (int iBinY = 0; iBinY<NBIN; iBinY++){
      
      column = column + hResponseMatrixCJetPythia30->GetBinContent(iBinX+1,iBinY+1);
    }
    double diag = hResponseMatrixCJetPythia30->GetBinContent(iBinX+1,iBinX+1);
    double offd = column - diag;
    purity = diag/column;
    PurityC30->SetBinContent(iBinX+1, purity);
    
    PurityC30->SetBinError(iBinX+1,sqrt((offd)/column/column*(offd)/column/column*diag+(diag/column/column)*(diag/column/column)*(offd)));
  }
  
  for (int iBinY = 3; iBinY<NBIN; iBinY++){
    double stability = 0;
    double row = 0;
    for (int iBinX = 0; iBinX<NBIN; iBinX++){
      
      row = row + hResponseMatrixCJetPythia30->GetBinContent(iBinX+1,iBinY+1);
    }
    double diag = hResponseMatrixCJetPythia30->GetBinContent(iBinY+1,iBinY+1);
    double offd = row - diag;
    stability = diag/row;
    StabilityC30->SetBinContent(iBinY+1, stability);
    StabilityC30->SetBinError(iBinY+1,sqrt((offd)/row/row*(offd)/row/row*diag+(diag/row/row)*(diag/row/row)*(offd)));
    
  }
  
  PurityC15->Add(PurityC30);
  StabilityC15->Add(StabilityC30);
  
  
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
  hResponseMatrixCJetPythia15->SetLineColor(kRed);
  hResponseMatrixCJetPythia15->SetMarkerColor(kRed);
  hResponseMatrixCJetPythia15->SetMarkerStyle(20);
  hResponseMatrixCJetPythia15->SetStats(0);
  hResponseMatrixCJetPythia15->Draw("text");
  ccResponseCJetPythia->SetLogx();
  ccResponseCJetPythia->SetLogy();
  ccResponseCJetPythia->SetLogz();
  
  TCanvas* ccResponseFJetPythia = new TCanvas("ccResponseFJetPythia","ccResponseFJetPythia",600,600);  
  hResponseMatrixFJetPythia15->SetLineColor(kRed);
  hResponseMatrixFJetPythia15->SetMarkerColor(kRed);
  hResponseMatrixFJetPythia15->SetMarkerStyle(20);
  hResponseMatrixFJetPythia15->SetStats(0);
  hResponseMatrixFJetPythia15->Draw("text");
  ccResponseFJetPythia->SetLogx();
  ccResponseFJetPythia->SetLogy();
  ccResponseFJetPythia->SetLogz();
    

/*  
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



*/

  cout << "=================================================================================" << endl;
  cout << "==================================== Herwig =====================================" << endl;

 for (Int_t iEvt= 0; iEvt<TreeTrain1Herwig->GetEntries(); iEvt++) {
    TreeTrain1Herwig->GetEntry(iEvt);
//     if (S_FJet_Pt>0 && G_FJet_Pt>0)   std::cerr << " S_FJet_Pt = " << S_FJet_Pt << " G_FJet_Pt = " << G_FJet_Pt << std::endl;
    
    if (S_FJet_Pt>0 && G_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0)  {
//      responseFJet.Fill (S_FJet_Pt, G_FJet_Pt, xsec1 / xsec_mean);
      hResponseMatrixFJetHerwig15->Fill(S_FJet_Pt, G_FJet_Pt);
     
//      responseCJet.Fill (S_CJet_Pt, G_CJet_Pt, xsec1 / xsec_mean);
      hResponseMatrixCJetHerwig15->Fill(S_CJet_Pt, G_CJet_Pt);
     }
    }
//

  
  for (Int_t iEvt= 0; iEvt<TreeTrain2Herwig->GetEntries(); iEvt++) {
   TreeTrain2Herwig->GetEntry(iEvt);
   
   if (S_FJet_Pt>0 && G_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0)  {
      
     hResponseMatrixFJetHerwig30->Fill(S_FJet_Pt, G_FJet_Pt);
    
//     responseCJet.Fill (S_CJet_Pt, G_CJet_Pt, xsec2 / xsec_mean);
     hResponseMatrixCJetHerwig30->Fill(S_CJet_Pt, G_CJet_Pt);
    }
   }

  
  
    cout << "==================================== Purity and Stability Herwig=====================================" << endl;
  //PURITY
  
  //TH::GetYaxis()->SetRangeUser(min,max)
  TH1D* PurityH15 = new TH1D ("PurityH", " Herwig15",NBIN,lowEdge);
  TH1D* StabilityH15 = new TH1D ("StabilityH", "Herwig15",NBIN,lowEdge);

  TH1D* PurityH30 = new TH1D ("PurityH", " Herwig30",NBIN,lowEdge);
  TH1D* StabilityH30 = new TH1D ("StabilityH", "Herwig30",NBIN,lowEdge);
  
//for forward Jets smaller than 57
  for (int iBinX = 0; iBinX<3; iBinX++){
    double purity = 0;
    double column = 0;
    for (int iBinY = 0; iBinY<NBIN; iBinY++){
      
      column = column + hResponseMatrixFJetHerwig15->GetBinContent(iBinX+1,iBinY+1);
    }
    double diag = hResponseMatrixFJetHerwig15->GetBinContent(iBinX+1,iBinX+1);
    double offd = column - diag;
    purity = diag/column;
    PurityH15->SetBinContent(iBinX+1, purity);
    PurityH15->SetBinError(iBinX+1,sqrt((offd)/column/column*(offd)/column/column*diag+(diag/column/column)*(diag/column/column)*(offd)));
  }
  
  for (int iBinY = 0; iBinY<3; iBinY++){
    double stability = 0;
    double row = 0;
    for (int iBinX = 0; iBinX<NBIN; iBinX++){
      
      row = row + hResponseMatrixFJetHerwig15->GetBinContent(iBinX+1,iBinY+1);
    }
    double diag = hResponseMatrixFJetHerwig15->GetBinContent(iBinY+1,iBinY+1);
    double offd = row - diag;
    stability = diag/row;
    StabilityH15->SetBinContent(iBinY+1, stability);
    StabilityH15->SetBinError(iBinY+1,sqrt((offd)/row/row*(offd)/row/row*diag+(diag/row/row)*(diag/row/row)*(offd)));
    
  }
  
  //for forward Jets greater than 57
  for (int iBinX = 3; iBinX<NBIN; iBinX++){
    double purity = 0;
    double column = 0;
    for (int iBinY = 0; iBinY<NBIN; iBinY++){
      
      column = column + hResponseMatrixFJetHerwig30->GetBinContent(iBinX+1,iBinY+1);
    }
    double diag = hResponseMatrixFJetHerwig30->GetBinContent(iBinX+1,iBinX+1);
    double offd = column - diag;
    purity = diag/column;
    PurityH30->SetBinContent(iBinX+1, purity);
    
    PurityH30->SetBinError(iBinX+1,sqrt((offd)/column/column*(offd)/column/column*diag+(diag/column/column)*(diag/column/column)*(offd)));
  }
  
  for (int iBinY = 3; iBinY<NBIN; iBinY++){
    double stability = 0;
    double row = 0;
    for (int iBinX = 0; iBinX<NBIN; iBinX++){
      
      row = row + hResponseMatrixFJetHerwig30->GetBinContent(iBinX+1,iBinY+1);
    }
    double diag = hResponseMatrixFJetHerwig30->GetBinContent(iBinY+1,iBinY+1);
    double offd = row - diag;
    stability = diag/row;
    StabilityH30->SetBinContent(iBinY+1, stability);
    StabilityH30->SetBinError(iBinY+1,sqrt((offd)/row/row*(offd)/row/row*diag+(diag/row/row)*(diag/row/row)*(offd)));
    
  }
  
  PurityH15->Add(PurityH30);
  StabilityH15->Add(StabilityH30);
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
  TH1D* PurityCH15 = new TH1D ("PurityC15", "PythiaH15",NBIN,lowEdge);
  TH1D* StabilityCH15 = new TH1D ("StabilityC15", "PythiaH15",NBIN,lowEdge);
  TH1D* PurityCH30 = new TH1D ("PurityC30", "PythiaH30",NBIN,lowEdge);
  TH1D* StabilityCH30 = new TH1D ("StabilityC30", "PythiaH30",NBIN,lowEdge);

  
  
  //for central Jets smaller than 57
  for (int iBinX = 0; iBinX<3; iBinX++){
    double purity = 0;
    double column = 0;
    for (int iBinY = 0; iBinY<NBIN; iBinY++){
      
      column = column + hResponseMatrixCJetHerwig15->GetBinContent(iBinX+1,iBinY+1);
    }
    double diag = hResponseMatrixCJetHerwig15->GetBinContent(iBinX+1,iBinX+1);
    double offd = column - diag;
    purity = diag/column;
    PurityCH15->SetBinContent(iBinX+1, purity);
    PurityCH15->SetBinError(iBinX+1,sqrt((offd)/column/column*(offd)/column/column*diag+(diag/column/column)*(diag/column/column)*(offd)));
  }
  
  for (int iBinY = 0; iBinY<3; iBinY++){
    double stability = 0;
    double row = 0;
    for (int iBinX = 0; iBinX<NBIN; iBinX++){
      
      row = row + hResponseMatrixCJetHerwig15->GetBinContent(iBinX+1,iBinY+1);
    }
    double diag = hResponseMatrixCJetHerwig15->GetBinContent(iBinY+1,iBinY+1);
    double offd = row - diag;
    stability = diag/row;
    StabilityCH15->SetBinContent(iBinY+1, stability);
    StabilityCH15->SetBinError(iBinY+1,sqrt((offd)/row/row*(offd)/row/row*diag+(diag/row/row)*(diag/row/row)*(offd)));
    
  }
  
  //for central Jets greater than 57
  for (int iBinX = 3; iBinX<NBIN; iBinX++){
    double purity = 0;
    double column = 0;
    for (int iBinY = 0; iBinY<NBIN; iBinY++){
      
      column = column + hResponseMatrixCJetHerwig30->GetBinContent(iBinX+1,iBinY+1);
    }
    double diag = hResponseMatrixCJetHerwig30->GetBinContent(iBinX+1,iBinX+1);
    double offd = column - diag;
    purity = diag/column;
    PurityCH30->SetBinContent(iBinX+1, purity);
    PurityCH30->SetBinError(iBinX+1,sqrt((offd)/column/column*(offd)/column/column*diag+(diag/column/column)*(diag/column/column)*(offd)));
  }
  
  for (int iBinY = 3; iBinY<NBIN; iBinY++){
    double stability = 0;
    double row = 0;
    for (int iBinX = 0; iBinX<NBIN; iBinX++){
      
      row = row + hResponseMatrixCJetHerwig30->GetBinContent(iBinX+1,iBinY+1);
    }
    double diag = hResponseMatrixCJetHerwig30->GetBinContent(iBinY+1,iBinY+1);
    double offd = row - diag;
    stability = diag/row;
    StabilityCH30->SetBinContent(iBinY+1, stability);
    StabilityCH30->SetBinError(iBinY+1,sqrt((offd)/row/row*(offd)/row/row*diag+(diag/row/row)*(diag/row/row)*(offd)));
    
  }
  PurityCH15->Add(PurityCH30);
  StabilityCH15->Add(StabilityCH30);
  
  //DRAW
  
  TCanvas* cPurityF = new TCanvas("cPurityF","cPurityF",600,600);  
  Purity15->GetYaxis()->SetRangeUser(0.,1.0);
  Purity15->GetXaxis()->SetRangeUser(35.,150);
  Purity15->SetTitle();
  Purity15->SetMarkerStyle(20);
  Purity15->SetMarkerSize(1);
  Purity15->GetYaxis()->SetTitle("Purity");
  Purity15->GetXaxis()->SetTitle("forward jet p_{T} [GeV/c]");
  Purity15->SetStats(0);
  Purity15->DrawCopy("E");
  
  PurityH15->GetYaxis()->SetRangeUser(0.,1.0);
  PurityH15->GetXaxis()->SetRangeUser(35.,150);
  PurityH15->SetTitle();
  PurityH15->SetLineColor(kRed);
  PurityH15->GetYaxis()->SetTitle("Purity");
  PurityH15->SetMarkerColor(kRed);
  PurityH15->SetMarkerStyle(20);
  PurityH15->SetMarkerSize(1);
  PurityH15->GetXaxis()->SetTitle("forward jet p_{T} [GeV/c]");
  PurityH15->SetStats(0);
  PurityH15->DrawCopy("SAMEE");
  
   leg = new TLegend(0.6,0.7,0.9,0.9,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   
   leg->AddEntry("Purity15","Pythia", "lpf");
   //entry = leg->AddEntry(central_M1,"Herwig+Jimmy ","lp");
   leg->AddEntry("PurityH15","Herwig", "lpf");
   leg->Draw();
  
  TCanvas* cStabilityF = new TCanvas("cStabilityF","cStabilityF",600,600);  
  Stability15->GetYaxis()->SetRangeUser(0.,1.0);
  Stability15->GetXaxis()->SetRangeUser(35.,150);
  Stability15->SetTitle();
  Stability15->SetMarkerStyle(20);
  Stability15->SetMarkerSize(1);
  Stability15->GetYaxis()->SetTitle("Stability");
  Stability15->GetXaxis()->SetTitle("forward jet p_{T} [GeV/c]");
  Stability15->SetStats(0);
  Stability15->DrawCopy("E");

  StabilityH15->GetYaxis()->SetRangeUser(0.,1.0);
  StabilityH15->GetXaxis()->SetRangeUser(35.,150);
  StabilityH15->SetLineColor(kRed);
  StabilityH15->SetMarkerColor(kRed);
  StabilityH15->SetMarkerStyle(20);
  StabilityH15->SetMarkerSize(1);
  StabilityH15->GetYaxis()->SetTitle("Stability");
  StabilityH15->GetXaxis()->SetTitle("forward jet p_{T} [GeV/c]");
  StabilityH15->SetStats(0);
  StabilityH15->DrawCopy("SAMEE");
  
   leg = new TLegend(0.6,0.7,0.9,0.9,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   
   leg->AddEntry("Stability15","Pythia", "lpf");
   //entry = leg->AddEntry(central_M1,"Herwig+Jimmy ","lp");
   leg->AddEntry("StabilityH15","Herwig", "lpf");
   leg->Draw();  
  
  TCanvas* cPurityC = new TCanvas("cPurityC","cPurityC",600,600);  
  PurityC15->GetYaxis()->SetRangeUser(0.,1.0);
  PurityC15->GetXaxis()->SetRangeUser(35.,150);
  PurityC15->SetTitle();
  PurityC15->SetMarkerSize(1);
  PurityC15->SetMarkerStyle(20);
  PurityC15->GetYaxis()->SetTitle("Purity");
  PurityC15->GetXaxis()->SetTitle("central jet p_{T} [GeV/c]");
  PurityC15->SetStats(0);
  PurityC15->DrawCopy("E");
  
  PurityCH15->GetYaxis()->SetRangeUser(0.,1.0);
  PurityCH15->GetXaxis()->SetRangeUser(35.,150);
  PurityCH15->SetLineColor(kRed);
  PurityCH15->SetMarkerColor(kRed);
  PurityCH15->SetMarkerSize(1);
  PurityCH15->SetMarkerStyle(20);
  PurityCH15->GetYaxis()->SetTitle("Purity");
  PurityCH15->GetXaxis()->SetTitle("central jet p_{T} [GeV/c]");
  PurityCH15->SetStats(0);
  PurityCH15->DrawCopy("SAMEE");
  
   leg = new TLegend(0.6,0.7,0.9,0.9,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   
   leg->AddEntry("PurityC15","Pythia", "lpf");
   //entry = leg->AddEntry(central_M1,"Herwig+Jimmy ","lp");
   leg->AddEntry("PurityCH15","Herwig", "lpf");
   leg->Draw();    
  
  TCanvas* cStabilityC = new TCanvas("cStabilityC","cStabilityC",600,600);  
  StabilityC15->GetYaxis()->SetRangeUser(0.,1.0);
  StabilityC15->GetXaxis()->SetRangeUser(35.,150);
  StabilityC15->SetMarkerStyle(20);
  StabilityC15->SetMarkerSize(1);
  StabilityC15->SetTitle();
  StabilityC15->GetYaxis()->SetTitle("Stability");
  StabilityC15->GetXaxis()->SetTitle("central jet p_{T} [GeV/c]");
  StabilityC15->SetStats(0);
  StabilityC15->DrawCopy("E");
  
  StabilityCH15->GetYaxis()->SetRangeUser(0.,1.0);
  StabilityCH15->GetXaxis()->SetRangeUser(35.,150);
  StabilityCH15->SetLineColor(kRed);
  StabilityCH15->SetMarkerColor(kRed);
  StabilityCH15->SetMarkerStyle(20);
  StabilityCH15->SetMarkerSize(1);
  StabilityCH15->GetYaxis()->SetTitle("Stability");
  StabilityCH15->GetXaxis()->SetTitle("central jet p_{T} [GeV/c]");
  StabilityCH15->SetStats(0);
  StabilityCH15->DrawCopy("SAMEE");
  
   leg = new TLegend(0.6,0.7,0.9,0.9,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   
   leg->AddEntry("StabilityC15","Pythia", "lpf");
   //entry = leg->AddEntry(central_M1,"Herwig+Jimmy ","lp");
   leg->AddEntry("StabilityCH15","Herwig", "lpf");
   leg->Draw();    
   
   //Draw unweighted response matrix
   
  TCanvas* ccResponseCJetHerwig = new TCanvas("ccResponseCJetHerwig","ccResponseCJetHerwig",600,600);  
  hResponseMatrixCJetHerwig15->SetLineColor(kRed);
  hResponseMatrixCJetHerwig15->SetMarkerColor(kRed);
  hResponseMatrixCJetHerwig15->SetMarkerStyle(20);
  hResponseMatrixCJetHerwig15->SetStats(0);
  hResponseMatrixCJetHerwig15->Draw("text");
  ccResponseFJetPythia->SetLogx();
  ccResponseFJetPythia->SetLogy();
  ccResponseCJetHerwig->SetLogz();
  
  TCanvas* ccResponseFJetHerwig = new TCanvas("ccResponseFJetHerwig","ccResponseFJetHerwig",600,600);  
  hResponseMatrixFJetHerwig15->SetLineColor(kRed);
  hResponseMatrixFJetHerwig15->SetMarkerColor(kRed);
  hResponseMatrixFJetHerwig15->SetMarkerStyle(20);
  hResponseMatrixFJetHerwig15->SetStats(0);
  hResponseMatrixFJetHerwig15->Draw("text");
  ccResponseFJetPythia->SetLogx();
  ccResponseFJetPythia->SetLogy();
  ccResponseFJetHerwig->SetLogz();
  
  
  
  
  
}

#ifndef __CINT__
int main () { P_S_noweight(); return 0; }  // Main program when run stand-alone
#endif
