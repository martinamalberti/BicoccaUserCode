//=====================================================================-*-C++-*-
// File and Version Information:
//      $Id: P_S_noweight.cxx,v 1.2 2011/02/10 16:16:45 govoni Exp $
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


#include "math.h"

#endif

//==============================================================================
// Global definitions
//==============================================================================

//==============================================================================
// Unfolding for C-F jet analysis
//==============================================================================

// void CorrectionFactors()
{ 
  
  gSystem->Load("libFWCoreFWLite.so");
  AutoLibraryLoader::enable();
  JetCorrectionUncertainty *jecUnc = new JetCorrectionUncertainty("START38_V13_AK5Calo_Uncertainty.txt");



  Float_t lowEdge[8] = {35,45,57,72,90,120,150,1000};
  const int NBIN = 7;
  
  int tresholdBIN= 2;
  
  double Threshold_G_FJet = 57;
  double Threshold_G_CJet = 57;

  int debug = 0;		//debug = 0 if no debug
  double lumi = 0.0426;

   std::cout << "==================================== TEST =====================================" << std::endl;

  double JES = 0.05;



  
//   TFile FileTest("/data_Nov.root","READ");
  TFile FileTest("/gwpool/users/mlucchin/Unfolding/CFNtuple/input/ntuples/Jet_2010A.root","READ");

  Float_t G_FJet_Pt_2; //~~~~ had
  Float_t S_FJet_Pt_2; //~~~~ reco

  Float_t G_CJet_Pt_2; //~~~~ had
  Float_t S_CJet_Pt_2; //~~~~ reco

  Float_t G_FJet_Eta; //~~~~ had
  Float_t S_FJet_Eta; //~~~~ reco

  Float_t G_CJet_Eta; //~~~~ had
  Float_t S_CJet_Eta; //~~~~ reco
   
  TTree* TreeTest = (TTree*) FileTest.Get("AnaHiggs");
  TreeTest->SetBranchAddress("S_FJet_Pt",&S_FJet_Pt_2);
  TreeTest->SetBranchAddress("S_CJet_Pt",&S_CJet_Pt_2);
  
  TreeTest->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt_2);
  TreeTest->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt_2);
  
  TreeTest->SetBranchAddress("G_FJet_Eta",&G_FJet_Eta);
  TreeTest->SetBranchAddress("S_FJet_Eta",&S_FJet_Eta);
  TreeTest->SetBranchAddress("G_CJet_Eta",&G_CJet_Eta);
  TreeTest->SetBranchAddress("S_CJet_Eta",&S_CJet_Eta);

  TreeTest->SetBranchAddress("G_FJet_Eta",&G_FJet_Eta);
  TreeTest->SetBranchAddress("S_FJet_Eta",&S_FJet_Eta);
  TreeTest->SetBranchAddress("G_CJet_Eta",&G_CJet_Eta);
  TreeTest->SetBranchAddress("S_CJet_Eta",&S_CJet_Eta);


  char fileName[30] = "JEC.root";
  cout<< "Output fileName = "<< fileName <<endl;

  TFile outFile(fileName,"RECREATE");
  outFile.cd();

  TH1D* hMeasFJet = new TH1D ("hMeasFJet", "Measured FJet", NBIN , lowEdge);
  TH1D* hMeasCJet = new TH1D ("hMeasCJet", "Measured CJet", NBIN , lowEdge);

  TGraph* gMeasFJet_JecUnc = new TGraph();
  TGraph* gMeasCJet_JecUnc = new TGraph();
  TGraph* gMeasFJet_JecUnc_eta = new TGraph();
  TGraph* gMeasCJet_JecUnc_eta = new TGraph();

  TGraph2D* gMeasFJet_JecUnc2D = new TGraph2D();
  TGraph2D* gMeasCJet_JecUnc2D = new TGraph2D();

for (Int_t iEvt= 0; iEvt<TreeTest->GetEntries(); iEvt++) {
   TreeTest->GetEntry(iEvt);
//    if (debug) std::cerr << " S_FJet_Pt = " << S_FJet_Pt << std::endl;
   if (S_FJet_Pt_2>35 && S_CJet_Pt_2>35) {
    hMeasFJet->Fill(S_FJet_Pt_2);
    hMeasCJet->Fill(S_CJet_Pt_2);
    
    jecUnc->setJetEta(S_FJet_Eta);
    jecUnc->setJetPt(S_FJet_Pt_2); // IMPORTANT: the uncertainty is a function of the CORRECTED pt
    Double_t unc_F = jecUnc->getUncertainty(true);
    
    gMeasFJet_JecUnc->SetPoint(iEvt, S_FJet_Pt_2, unc_F);
    gMeasFJet_JecUnc_eta->SetPoint(iEvt, S_FJet_Eta, unc_F);
    gMeasFJet_JecUnc2D->SetPoint(iEvt, S_FJet_Pt_2, S_FJet_Eta, unc_F);
    
    jecUnc->setJetEta(S_CJet_Eta);
    jecUnc->setJetPt(S_CJet_Pt_2); // IMPORTANT: the uncertainty is a function of the CORRECTED pt
    Double_t unc_C = jecUnc->getUncertainty(true);
    
    gMeasCJet_JecUnc->SetPoint(iEvt, S_CJet_Pt_2, unc_C);
    gMeasCJet_JecUnc_eta->SetPoint(iEvt, S_CJet_Eta, unc_C);
    gMeasCJet_JecUnc2D->SetPoint(iEvt, S_CJet_Pt_2, S_CJet_Eta, unc_C);
    
    
    }
  }


 gMeasFJet_JecUnc->SetName("gMeasFJet_JecUnc");
 gMeasCJet_JecUnc->SetName("gMeasCJet_JecUnc");
 gMeasFJet_JecUnc_eta->SetName("gMeasFJet_JecUnc_eta");
 gMeasCJet_JecUnc_eta->SetName("gMeasCJet_JecUnc_eta");

 gMeasFJet_JecUnc2D->SetName("gMeasFJet_JecUnc2D");
 gMeasCJet_JecUnc2D->SetName("gMeasCJet_JecUnc2D");



  gMeasFJet_JecUnc->Write();
  gMeasCJet_JecUnc->Write();
  gMeasFJet_JecUnc_eta->Write();
  gMeasCJet_JecUnc_eta->Write();

  gMeasFJet_JecUnc2D->Write();
  gMeasCJet_JecUnc2D->Write();
  
  hMeasFJet->Sumw2();
  hMeasCJet->Sumw2();
 
  
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hMeasFJet->SetBinError(iBinX+1,hMeasFJet->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / lumi);
   hMeasCJet->SetBinError(iBinX+1,hMeasCJet->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / lumi);
   hMeasFJet->SetBinContent(iBinX+1,hMeasFJet->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / lumi);
   hMeasCJet->SetBinContent(iBinX+1,hMeasCJet->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / lumi);       
     

  }



  
  TCanvas* ccFJet_JecUnc = new TCanvas("ccFJet_JecUnc","ccFJet_JecUnc",600,600);  
  gMeasFJet_JecUnc->GetXaxis()->SetTitle("forward jet Pt (GeV)");
  gMeasFJet_JecUnc->GetYaxis()->SetTitle("JEC uncertainty at different eta");
  gMeasFJet_JecUnc->Draw("APE");
  
  TCanvas* ccCJet_JecUnc = new TCanvas("ccCJet_JecUnc","ccCJet_JecUnc",600,600);  
  gMeasCJet_JecUnc->GetXaxis()->SetTitle("central jet Pt (GeV)");
  gMeasCJet_JecUnc->GetYaxis()->SetTitle("JEC uncertainty at different eta");
  gMeasCJet_JecUnc->Draw("APE");
  
  TCanvas* ccFJet_JecUnc_eta = new TCanvas("ccFJet_JecUnc_eta","ccFJet_JecUnc_eta",600,600); 
  gMeasFJet_JecUnc_eta->GetXaxis()->SetTitle("forward jet Eta");
  gMeasFJet_JecUnc_eta->GetYaxis()->SetTitle("JEC uncertainty at different pt");
  gMeasFJet_JecUnc_eta->Draw("APE");
  
  TCanvas* ccCJet_JecUnc_eta = new TCanvas("ccCJet_JecUnc_eta","ccCJet_JecUnc_eta",600,600); 
  gMeasCJet_JecUnc_eta->GetXaxis()->SetTitle("central jet Eta");
  gMeasCJet_JecUnc_eta->GetYaxis()->SetTitle("JEC uncertainty at different pt");
  gMeasCJet_JecUnc_eta->Draw("APE");

  // TCanvas* ccFJet_JecUnc2D = new TCanvas("ccFJet_JecUnc2D","ccFJet_JecUnc2D",0,0,600,600); 
  //  gMeasFJet_JecUnc_eta->GetXaxis()->SetTitle("forward jet Eta");
  //  gMeasFJet_JecUnc_eta->GetYaxis()->SetTitle("JEC uncertainty at different pt");
  //gStyle->SetPalette(1);
  //gMeasFJet_JecUnc2D->Draw("surf");
  
  //TCanvas* ccCJet_JecUnc2D = new TCanvas("ccCJet_JecUnc2D","ccCJet_JecUnc2D",0,0,600,600); 
  //  gMeasCJet_JecUnc_eta->GetXaxis()->SetTitle("central jet Eta");
  //  gMeasCJet_JecUnc_eta->GetYaxis()->SetTitle("JEC uncertainty at different pt");
  //gStyle->SetPalette(1);
  //gMeasCJet_JecUnc2D->Draw("surf");


  outFile.Write();
  outFile.Close();
  
    
}

