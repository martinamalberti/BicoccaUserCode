//=====================================================================-*-C++-*-
// File and Version Information:
//      $Id: MC_compare.cxx,v 1.1 2011/03/03 15:24:13 govoni Exp $
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

//==============================================================================
// Global definitions
//==============================================================================

//==============================================================================
// Unfolding for C-F jet analysis
//==============================================================================

void MC_compare()
{
#ifdef __CINT__
  gSystem->Load("libRooUnfold");
#endif

   int NBIN = 40;
   double MIN = 35;
   double MAX = 200;
   
  
  cout << "================================= FILLING HISTOGRAMS =================================" << endl;
 
   TFile FileMC_H15("../input/BO/rootfiles/mc/qcd_15_herwigjimmy.root","READ");
   TFile FileMC_P15("../output/HerwigJimmy15.root","READ");
  
   TFile FileMC_H30("../input/Unfolding/qcd_30_herwigjimmy.root","READ");
   TFile FileMC_P30("../output/HerwigJimmy30.root","READ");
  
  

  

  
  double Threshold_G_FJet = 57;
  double Threshold_G_CJet = 57;
  
  TTree* TreeMC_H15 = (TTree*) FileMC_H15.Get("AnaHiggs");
  TTree* TreeMC_H30 = (TTree*) FileMC_H30.Get("AnaHiggs");
  TTree* TreeMC_P15 = (TTree*) FileMC_P15.Get("AnaHiggs");
  TTree* TreeMC_P30 = (TTree*) FileMC_P30.Get("AnaHiggs");
  
//   double xsec1H = 714000000. / TreeMC_H15->GetEntries();		//cross section HERWIG
//  double xsec2H = 49240000. / TreeMC_H30->GetEntries();
  
    
    double xsec1H = 714000000. / 1631667.;		//cross section HERWIG
    double xsec2H = 49240000. / 1310829.;
    
    double xsec1 = 714000000. / 1532000.;		//cross section HERWIG
    double xsec2 = 49240000. / 1111000.;
    
  
//    double xsec1H = 714000000. / TreeMC_H15->GetEntries();		//cross section HERWIG
//    double xsec2H = 49240000. / TreeMC_H30->GetEntries();
  
//   double xsec1 =  714000000. /TreeMC_P15->GetEntries();		//cross section
//   double xsec2 = 49240000. /TreeMC_P30->GetEntries();
  
  
  Float_t G_FJet_Pt; //~~~~ had
  Float_t S_FJet_Pt; //~~~~ reco

 Float_t G_CJet_Pt; //~~~~ had
 Float_t S_CJet_Pt; //~~~~ reco
 
 Double_t G_FJet_Pt_D; //~~~~ had
  Double_t S_FJet_Pt_D; //~~~~ reco

 Double_t G_CJet_Pt_D; //~~~~ had
 Double_t S_CJet_Pt_D; //~~~~ reco
 
 TreeMC_H15->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt);
 TreeMC_H15->SetBranchAddress("S_FJet_Pt",&S_FJet_Pt);
 TreeMC_H15->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt);
 TreeMC_H15->SetBranchAddress("S_CJet_Pt",&S_CJet_Pt);

 TreeMC_H30->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt);
 TreeMC_H30->SetBranchAddress("S_FJet_Pt",&S_FJet_Pt);
 TreeMC_H30->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt);
 TreeMC_H30->SetBranchAddress("S_CJet_Pt",&S_CJet_Pt);
 
 
 TreeMC_P15->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt_D);
 TreeMC_P15->SetBranchAddress("S_FJet_Pt",&S_FJet_Pt_D);
 TreeMC_P15->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt_D);
 TreeMC_P15->SetBranchAddress("S_CJet_Pt",&S_CJet_Pt_D);

 TreeMC_P30->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt_D);
 TreeMC_P30->SetBranchAddress("S_FJet_Pt",&S_FJet_Pt_D);
 TreeMC_P30->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt_D);
 TreeMC_P30->SetBranchAddress("S_CJet_Pt",&S_CJet_Pt_D);
 
  
  TH1D* PythiaC15 = new TH1D ("PythiaCentral", "Pythia Central 15",    NBIN, MIN, MAX);
  TH1D* PythiaF15 = new TH1D ("PythiaForward", "Pythia Forward 15", NBIN, MIN, MAX);
  TH1D* PythiaC30 = new TH1D ("PythiaCentral", "Pythia Central 30",    NBIN, MIN, MAX);
  TH1D* PythiaF30 = new TH1D ("PythiaForward", "Pythia Forward 30", NBIN, MIN, MAX);
  
  
  TH1D* HerwigC15= new TH1D ("HerwigCentral", "Herwig Central 15",    NBIN, MIN, MAX);
  TH1D* HerwigF15 = new TH1D ("HerwigForward", "Herwig Forward 15", NBIN, MIN, MAX);
  TH1D* HerwigC30= new TH1D ("HerwigCentral", "Herwig Central 30",    NBIN, MIN, MAX);
  TH1D* HerwigF30 = new TH1D ("HerwigForward", "Herwig Forward 30", NBIN, MIN, MAX);
  
  
  TH1D* PythiaC15_reco = new TH1D ("PythiaC15_reco", "Pythia Central 15_reco",    NBIN, MIN, MAX);
  TH1D* PythiaF15_reco = new TH1D ("PythiaF15_reco", "Pythia Forward 15_reco", NBIN, MIN, MAX);
  TH1D* PythiaC30_reco = new TH1D ("PythiaC30_reco", "Pythia Central 30_reco",    NBIN, MIN, MAX);
  TH1D* PythiaF30_reco = new TH1D ("PythiaF30_reco", "Pythia Forward 30_reco", NBIN, MIN, MAX);
  
  
  TH1D* HerwigC15_reco= new TH1D ("HerwigC15_reco", "Herwig Central 15_reco",    NBIN, MIN, MAX);
  TH1D* HerwigF15_reco = new TH1D ("HerwigF15_reco", "Herwig Forward 15_reco", NBIN, MIN, MAX);
  TH1D* HerwigC30_reco= new TH1D ("HerwigC30_reco", "Herwig Central 30_reco",    NBIN, MIN, MAX);
  TH1D* HerwigF30_reco = new TH1D ("HerwigF30_reco", "Herwig Forward 30_reco", NBIN, MIN, MAX);
  
  
  //riempio con Herwig ptHat 15
 for (Int_t iEvt= 0; iEvt<TreeMC_H15->GetEntries(); iEvt++) {
   TreeMC_H15->GetEntry(iEvt);
   //    std::cerr << " S_FJet_Pt = " << S_FJet_Pt << std::endl;
//     if (G_CJet_Pt>0 && G_FJet_Pt>0) {
//     if (S_FJet_Pt>0 && S_CJet_Pt>0 ) {
    if (S_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0 && G_FJet_Pt>0) {
       HerwigF15->Fill(G_FJet_Pt,xsec1H);
       HerwigC15->Fill(G_CJet_Pt,xsec1H);
       
       HerwigF15_reco->Fill(S_FJet_Pt,xsec1H);
       HerwigC15_reco->Fill(S_CJet_Pt,xsec1H);
     
    }
   }
   
 //riempio con herwig ptHat 30
 for (Int_t iEvt= 0; iEvt<TreeMC_H30->GetEntries(); iEvt++) {
   TreeMC_H30->GetEntry(iEvt);
//        if (G_CJet_Pt>0 && G_FJet_Pt>0) {
//     if (S_FJet_Pt>0 && S_CJet_Pt>0 ) {
    if (S_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0 && G_FJet_Pt>0) {
    
     HerwigF30->Fill(G_FJet_Pt,xsec2H);
     HerwigC30->Fill(G_CJet_Pt,xsec2H);
     
     HerwigF30_reco->Fill(S_FJet_Pt,xsec2H);
     HerwigC30_reco->Fill(S_CJet_Pt,xsec2H);
     //hMeasCJet_MC->Fill(S_CJet_Pt,xsec1);
    }
   }
 

  //riempio con pythia ptHat 15
 for (Int_t iEvt= 0; iEvt<TreeMC_P15->GetEntries(); iEvt++) {
   TreeMC_P15->GetEntry(iEvt);
//    if (G_CJet_Pt_D>0 && G_FJet_Pt_D>0) {
//    if (S_FJet_Pt_D>0 && S_CJet_Pt_D>0 ) {
   if (S_FJet_Pt_D>0 && S_CJet_Pt_D>0 && G_CJet_Pt_D>0 && G_FJet_Pt_D>0) {
        PythiaF15->Fill(G_FJet_Pt_D,xsec1);
        PythiaC15->Fill(G_CJet_Pt_D,xsec1);
	
	PythiaF15_reco->Fill(S_FJet_Pt_D,xsec1);
        PythiaC15_reco->Fill(S_CJet_Pt_D,xsec1);
     //hMeasCJet_MC->Fill(S_CJet_Pt,xsec1);
    }
   }
 
 
  //riempio con pythia ptHat 30
 for (Int_t iEvt= 0; iEvt<TreeMC_P30->GetEntries(); iEvt++) {
   TreeMC_P30->GetEntry(iEvt);
   //    std::cerr << " S_FJet_Pt = " << S_FJet_Pt << std::endl;
//       if (G_CJet_Pt_D>0 && G_FJet_Pt_D>0) {
//    if (S_FJet_Pt_D>0 && S_CJet_Pt_D>0 ) {
    if (S_FJet_Pt_D>0 && S_CJet_Pt_D>0 && G_CJet_Pt_D>0 && G_FJet_Pt_D>0) {
    
     PythiaF30->Fill(G_FJet_Pt_D,xsec2);
     PythiaC30->Fill(G_CJet_Pt_D,xsec2);
     
     PythiaF30_reco->Fill(S_FJet_Pt_D,xsec2);
     PythiaC30_reco->Fill(S_CJet_Pt_D,xsec2);
     //hMeasCJet_MC->Fill(S_CJet_Pt,xsec1);
    }
   }
 
    
  cout << "===================================== DRAW ====================================" << endl;

   /*
  TCanvas* Herwig = new TCanvas("Herwig","Herwig",600,1200);  
  //ccFJet->Divide(1,2);
  //ccFJet->cd(1);
  Herwig15->SetLineColor(kRed);
  Herwig15->GetXaxis()->SetTitle("p_{T}");
  Herwig15->DrawCopy();
  Herwig30->SetLineColor(kBlue);
  Herwig30->SetMarkerColor(kBlue);
  Herwig30->SetMarkerStyle(20);
  Herwig30->SetMarkerSize(1);
  Herwig30->DrawCopy("SAME");
//   hTrueFJet->SetLineColor(kGreen);
//   hTrueFJet->DrawCopy("SAME");
  gPad->BuildLegend();
  gPad->SetGrid();
  //ccFJet->cd(2);
  //hResponseMatrixFJet->GetXaxis()->SetTitle("p_{T} Reco");
  //hResponseMatrixFJet->GetYaxis()->SetTitle("p_{T} Had");
  //hResponseMatrixFJet->DrawCopy("colz");
  //gPad->SetGrid();
  */
  /*
  TCanvas* PythiaCentral = new TCanvas("PythiaCentral","Pythia Central",600,600);  
  //CompareMC->Divide(1,2);
  //CompareMC->cd(1);
  PythiaC15->SetStats(0);
  PythiaC15->SetLineColor(kRed);
  PythiaC15->SetMarkerStyle(20);
  PythiaC15->SetMarkerColor(kRed);
  PythiaC15->SetMarkerSize(1);
  PythiaC15->GetXaxis()->SetTitle("Central p_{T}");
  PythiaC15->DrawCopy();
  
  HerwigC15->SetMarkerStyle(20);
  HerwigC15->SetMarkerSize(1);
  HerwigC15->DrawCopy("SAME");

   leg = new TLegend(0.6,0.7,0.9,0.9,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   
   leg->AddEntry("PythiaC15","Herwig 15_MiaNtuple", "lpf");
   //entry = leg->AddEntry(central_M1,"Herwig+Jimmy ","lp");
   leg->AddEntry("HerwigC15","Herwig 15_BoNtuple", "lpf");
   leg->Draw();      
   
   for (int iBin = 0; iBin<NBIN; iBin ++){
   std::cerr<<"Mio/Bo = ["<< iBin << "]" << PythiaC15->GetBinContent(iBin+1)/HerwigC15->GetBinContent(iBin+1) <<std::endl;
   }*/
   
   
   TCanvas* PythiaCentral_reco = new TCanvas("PythiaCentral_reco","Pythia Central Reco",600,600);  
  //CompareMC->Divide(1,2);
  //CompareMC->cd(1);
  PythiaC15_reco->SetStats(0);
  PythiaC15_reco->SetLineColor(kRed);
  PythiaC15_reco->SetMarkerStyle(20);
  PythiaC15_reco->SetMarkerColor(kRed);
  PythiaC15_reco->SetMarkerSize(1);
  PythiaC15_reco->GetXaxis()->SetTitle("Central p_{T}");
  PythiaC15_reco->DrawCopy();
  
  HerwigC15_reco->SetMarkerStyle(20);
  HerwigC15_reco->SetMarkerSize(1);
  HerwigC15_reco->DrawCopy("SAME");

   leg = new TLegend(0.6,0.7,0.9,0.9,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   
   leg->AddEntry("PythiaC15_reco","Herwig 15_MiaNtuple", "lpf");
   //entry = leg->AddEntry(central_M1,"Herwig+Jimmy ","lp");
   leg->AddEntry("HerwigC15_reco","Herwig 15_BoNtuple", "lpf");
   leg->Draw();      
   
   for (int iBin = 0; iBin<NBIN/2; iBin ++){
   std::cerr<<"Mio/Bo_reco = ["<< iBin << "]" << PythiaC15_reco->GetBinContent(iBin+1)/HerwigC15_reco->GetBinContent(iBin+1) <<std::endl;
   }
   
   /*
   TCanvas* PythiaForward = new TCanvas("PythiaForward","Pythia Forward",600,1200);  
  //CompareMC->Divide(1,2);
  //CompareMC->cd(1);
  PythiaF15->SetStats(0);
  PythiaF15->SetLineColor(kRed);
  PythiaF15->SetMarkerStyle(20);
  PythiaF15->SetMarkerSize(1);
  PythiaF15->GetXaxis()->SetTitle("Central p_{T}");
  PythiaF15->DrawCopy();
  
  PythiaF30->SetMarkerStyle(20);
  PythiaF30->SetMarkerSize(1);
  PythiaF30->DrawCopy("SAME");

   leg = new TLegend(0.6,0.7,0.9,0.9,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   
   leg->AddEntry("PythiaF15","Pythia 15", "lpf");
   //entry = leg->AddEntry(central_M1,"Herwig+Jimmy ","lp");
   leg->AddEntry("PythiaF30","Pythia 30", "lpf");
   leg->Draw();  
  
   
  TCanvas* HerwigCentral = new TCanvas("HerwigCentral","Herwig Central",600,1200);  
  //CompareMC->Divide(1,2);
  //CompareMC->cd(1);
  HerwigC15->SetStats(0);
  HerwigC15->SetLineColor(kRed);
  HerwigC15->SetMarkerStyle(20);
  HerwigC15->SetMarkerSize(1);
  HerwigC15->GetXaxis()->SetTitle("Central p_{T}");
  HerwigC15->DrawCopy();
  
  HerwigC30->SetMarkerStyle(20);
  HerwigC30->SetMarkerSize(1);
  HerwigC30->DrawCopy("SAME");

   leg = new TLegend(0.6,0.7,0.9,0.9,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   
   leg->AddEntry("HerwigC15","Herwig 15", "lpf");
   //entry = leg->AddEntry(central_M1,"Herwig+Jimmy ","lp");
   leg->AddEntry("HerwigC30","Herwig 30", "lpf");
   leg->Draw();      
   
   
   TCanvas* HerwigForward = new TCanvas("HerwigForward","Herwig Forward",600,1200);  
  //CompareMC->Divide(1,2);
  //CompareMC->cd(1);
  HerwigF15->SetStats(0);
  HerwigF15->SetLineColor(kRed);
  HerwigF15->SetMarkerStyle(20);
  HerwigF15->SetMarkerSize(1);
  HerwigF15->GetXaxis()->SetTitle("Central p_{T}");
  HerwigF15->DrawCopy();
  
  HerwigF30->SetMarkerStyle(20);
  HerwigF30->SetMarkerSize(1);
  HerwigF30->DrawCopy("SAME");

   leg = new TLegend(0.6,0.7,0.9,0.9,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   
   leg->AddEntry("HerwigF15","Herwig 15", "lpf");
   //entry = leg->AddEntry(central_M1,"Herwig+Jimmy ","lp");
   leg->AddEntry("HerwigF30","Herwig 30", "lpf");
   leg->Draw();  */
   
}
   
  
#ifndef __CINT__
int main () { ptHat(); return 0; }  // Main program when run stand-alone
#endif
