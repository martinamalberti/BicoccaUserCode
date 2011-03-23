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
#ifdef __CINT__
  gSystem->Load("libRooUnfold");
#endif
  
  //setTDRStyle();
   int ALGO = 3;
  //enum Algorithm {
  // 	kNone	           0
  //	kBayes	           1
  //	kSVD	             2
  //	kBinByBin	        3
  //	kTUnfold	         4
  //	kInvert	          5
  //	kDagostini	       6
  //

//   Float_t lowEdge[11] = {18,22,27,35,45,57,72,90,120,150,1000};
//   int NBIN = 10;
  
  Float_t lowEdge[8] = {35,45,57,72,90,120,150,1000};
  int NBIN = 7;
  
  TH1F myH("myH","myH",NBIN,lowEdge);

  
  /*Float_t lowEdge[9] = {27,35,45,57,72,90,120,150,1000};
  int NBIN = 8;
  TH1F myH("myH","myH",NBIN,lowEdge);*/
  
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
  
  TFile FileTrainPOW("../output/POWHEG_kt15_PYTHIA_JOB_9_2.root","READ");
  TFile FileTrainPOW7("../output/Hannes_pt7.root","READ");
  

  double lumi = 5.1e-3;
  
  double xsec1Pythia = 8.762e8 / 6190500.;
  double xsec2Pythia = 6.041e7 / 4918016.;
  
  double xsec1Herwig = 714000000. / 1631667.;
  double xsec2Herwig =  49240000. / 1310829.;
  
  double xsecPowheg = 900054123. / 1514265.;
  double xsecPowheg7 = 1.07846619e10 /2000000.;
  
  double Threshold_G_FJet = 57;
  double Threshold_G_CJet = 57;
  
 TTree* TreeTrain1Pythia = (TTree*) FileTrain1Pythia.Get("AnaHiggs");
  TTree* TreeTrain2Pythia = (TTree*) FileTrain2Pythia.Get("AnaHiggs");
 
  TTree* TreeTrain1Herwig = (TTree*) FileTrain1Herwig.Get("AnaHiggs");
  TTree* TreeTrain2Herwig = (TTree*) FileTrain2Herwig.Get("AnaHiggs");
  
  TTree* TreeTrainPOW = (TTree*) FileTrainPOW.Get("AnaHiggs");
  TTree* TreeTrainPOW7 = (TTree*) FileTrainPOW7.Get("AnaHiggs");
  
  Double_t G_FJet_Pt; //~~~~ had
  Double_t S_FJet_Pt; //~~~~ reco

  Double_t G_CJet_Pt; //~~~~ had
  Double_t S_CJet_Pt; //~~~~ reco
  
  Double_t G_FJet_Pt_P; //~~~~ had
  Double_t G_CJet_Pt_P; //~~~~ had
 
  Int_t nCJet_S_FJet; 
  Int_t nFJet_S_CJet;




  
  cout << "==================================== TEST =====================================" << endl;

  double JES = 0.05; //incertezza del 5 per cento sulla scala di energia del jet
  
  TFile FileTest("/home/toliman/Dropbox/QCD_CF/Multiplicity/data_Nov.root","READ");
  
  TTree* TreeTest = (TTree*) FileTest.Get("AnaHiggs");
  TreeTest->SetBranchAddress("S_FJet_Pt",&S_FJet_Pt);
  TreeTest->SetBranchAddress("S_CJet_Pt",&S_CJet_Pt);
  
  TreeTest->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt);
  TreeTest->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt);
  TreeTest->SetBranchAddress("nCJet_S_FJet",&nCJet_S_FJet);
  TreeTest->SetBranchAddress("nFJet_S_CJet",&nFJet_S_CJet);
  
  
  
  TH1D* hMeasFJet = new TH1D ("hMeasFJet", "Measured FJet", NBIN , lowEdge);
  TH1D* hMeasCJet = new TH1D ("hMeasCJet", "Measured CJet", NBIN , lowEdge);

  TH1D* hMeasFJet_Plus = new TH1D ("hMeasFJet_Plus", "Measured FJet_Plus", NBIN , lowEdge);
  TH1D* hMeasCJet_Plus = new TH1D ("hMeasCJet_Plus", "Measured CJet_Plus", NBIN , lowEdge);
  TH1D* hMeasFJet_Minus = new TH1D ("hMeasFJet_Minus", "Measured FJet_Minus", NBIN , lowEdge);
  TH1D* hMeasCJet_Minus = new TH1D ("hMeasCJet_Minus", "Measured CJet_Minus", NBIN , lowEdge);
  
  const int NBIN_ratio = 40;
  const int NBIN_multi = 6;
  const int NBIN_Cpt = 40;

  
  TH1D* hRatio35 = new TH1D ("hRatio35", "Ratio 35", NBIN_ratio, 0., 4.);
  TH1D* hRatio40 = new TH1D ("hRatio40", "Ratio 40", NBIN_ratio, 0., 4.);
  TH1D* hRatio45 = new TH1D ("hRatio45", "Ratio 45", NBIN_ratio, 0., 4.);
  TH1D* hRatio57 = new TH1D ("hRatio57", "Ratio 57", NBIN_ratio, 0., 4.);
  TH1D* hRatio72 = new TH1D ("hRatio72", "Ratio 72", NBIN_ratio, 0., 4.);
  TH1D* hRatio90 = new TH1D ("hRatio90", "Ratio 90", NBIN_ratio, 0., 4.);
  TH1D* hRatio120 = new TH1D ("hRatio120", "Ratio 120", NBIN_ratio, 0., 4.);
  
  TH1D* hMulti35 = new TH1D ("hMulti35", "Multi 35", NBIN_multi, 0., NBIN_multi);
  TH1D* hMulti40 = new TH1D ("hMulti40", "Multi 40", NBIN_multi, 0., NBIN_multi);
  TH1D* hMulti45 = new TH1D ("hMulti45", "Multi 45", NBIN_multi, 0., NBIN_multi);
  TH1D* hMulti57 = new TH1D ("hMulti57", "Multi 57", NBIN_multi, 0., NBIN_multi);
  TH1D* hMulti72 = new TH1D ("hMulti72", "Multi 72", NBIN_multi, 0., NBIN_multi);
  TH1D* hMulti90 = new TH1D ("hMulti90", "Multi 90", NBIN_multi, 0., NBIN_multi);
  TH1D* hMulti120 = new TH1D ("hMulti120", "Multi 120", NBIN_multi, 0., NBIN_multi);
  
  TH1D* hCJet_pt35 = new TH1D ("hCJet_pt35", "CJet_pt35", NBIN_Cpt, 0., 150.);
  TH1D* hCJet_pt45 = new TH1D ("hCJet_pt45", "CJet_pt45", NBIN_Cpt, 0., 150.);
  TH1D* hCJet_pt57 = new TH1D ("hCJet_pt57", "CJet_pt57", NBIN_Cpt, 0., 150.);
  TH1D* hCJet_pt72 = new TH1D ("hCJet_pt72", "CJet_pt72", NBIN_Cpt, 0., 150.);
  TH1D* hCJet_pt90 = new TH1D ("hCJet_pt90", "CJet_pt90", NBIN_Cpt, 0., 150.);
  TH1D* hCJet_pt120 = new TH1D ("hCJet_pt120", "CJet_pt120", NBIN_Cpt, 0., 150.);
  
  

  
  
//     TH2D* hScatterPlot = new TH2D ("hScatterPlot", "ScatterPlot", NBIN , lowEdge,  NBIN , lowEdge);
//    TH2D* hRatioBin = new TH2D ("hRatioBin", "RatioBin", NBIN , lowEdge, NBIN, lowEdge);


  for (Int_t iEvt= 0; iEvt<TreeTest->GetEntries(); iEvt++) {
   TreeTest->GetEntry(iEvt);
//    std::cerr << " S_FJet_Pt = " << S_FJet_Pt << std::endl;
   if (S_FJet_Pt>35 && S_CJet_Pt>35) {
    hMeasFJet->Fill(S_FJet_Pt);
    hMeasCJet->Fill(S_CJet_Pt);
    
    hMeasFJet_Plus->Fill(S_FJet_Pt * (1 + JES));
    hMeasCJet_Plus->Fill(S_CJet_Pt * (1 + JES));

    hMeasFJet_Minus->Fill(S_FJet_Pt * (1 - JES));
    hMeasCJet_Minus->Fill(S_CJet_Pt * (1 - JES));
    
    
    if(S_FJet_Pt>35 && S_FJet_Pt<45 && nFJet_S_CJet == 1){
    hRatio35->Fill(S_FJet_Pt/S_CJet_Pt);
    hMulti35->Fill(nCJet_S_FJet);
    hCJet_pt35->Fill(S_CJet_Pt);
    
//     hRatioBin->Fill(S_FJet_Pt, S_FJet_Pt/S_CJet_Pt);
    }
    
    /*  if(S_FJet_Pt>40 && S_FJet_Pt<45){
    hRatio40->Fill(S_FJet_Pt/S_CJet_Pt);
    hMulti40->Fill(nCJet_S_FJet);
//     hCJet_pt40->Fill(S_CJet_Pt);
    
//     hRatioBin->Fill(S_FJet_Pt, S_FJet_Pt/S_CJet_Pt);
    }*/
    
    
     if(S_FJet_Pt>45 && S_FJet_Pt<57){
    hRatio45->Fill(S_FJet_Pt/S_CJet_Pt);
    hMulti45->Fill(nCJet_S_FJet);
    hCJet_pt45->Fill(S_CJet_Pt);
//     hRatioBin->Fill(S_FJet_Pt, S_FJet_Pt/S_CJet_Pt);
    }
     if(S_FJet_Pt>57 && S_FJet_Pt<72){
    hRatio57->Fill(S_FJet_Pt/S_CJet_Pt);
    hMulti57->Fill(nCJet_S_FJet);
    hCJet_pt57->Fill(S_CJet_Pt);
//     hRatioBin->Fill(S_FJet_Pt, S_FJet_Pt/S_CJet_Pt);
    }
     if(S_FJet_Pt>72 && S_FJet_Pt<90){
    hRatio72->Fill(S_FJet_Pt/S_CJet_Pt);
    hMulti72->Fill(nCJet_S_FJet);
    hCJet_pt72->Fill(S_CJet_Pt);
//     hRatioBin->Fill(S_FJet_Pt, S_FJet_Pt/S_CJet_Pt);
    }
     if(S_FJet_Pt>90 && S_FJet_Pt<120){
    hRatio90->Fill(S_FJet_Pt/S_CJet_Pt);
    hMulti90->Fill(nCJet_S_FJet);
    hCJet_pt90->Fill(S_CJet_Pt);
//     hRatioBin->Fill(S_FJet_Pt, S_FJet_Pt/S_CJet_Pt);
    }
     if(S_FJet_Pt>120 && S_FJet_Pt<150){
    hRatio120->Fill(S_FJet_Pt/S_CJet_Pt);
    hMulti120->Fill(nCJet_S_FJet);
    hCJet_pt120->Fill(S_CJet_Pt);
//     hRatioBin->Fill(S_FJet_Pt, S_FJet_Pt/S_CJet_Pt);
    }
    
   }
  }
  
  
  TCanvas* ccScatterPlot = new TCanvas("ccScatterPlot","ccScatter",600,600);  
  
  TreeTest->Draw("S_FJet_Pt:S_CJet_Pt", "S_FJet_Pt>-100 && S_CJet_Pt>-100", "COLZ");	//y:x, tagli
  
  
  
  TCanvas* ccRatio35 = new TCanvas("ccRatio35","ccRatio35",600,600);  
  ccRatio35->Divide(1,2);
  ccRatio35->cd(1);

  hRatio35->GetXaxis()->SetTitle("Forward_pt/Central_pt");
  hRatio35->GetYaxis()->SetTitle("nEvents");
//   hRatio35->SetFillStyle(3001);  
//   hRatio35->SetFillColor(kYellow);  
  hRatio35->SetLineColor(kBlack);  
//   hRatio35->SetMarkerColor(kYellow); 
  hRatio35->SetMarkerStyle(20);
  hRatio35->SetMarkerSize(1);
  hRatio35->GetXaxis()->SetRangeUser(35.,149.);
//   hRatio35->SetStats(0);
  hRatio35->DrawCopy();
  
  hRatio35->Fit("gaus", "L");
  
  ccRatio35->cd(2);
  hMulti35->GetXaxis()->SetTitle("number of central jets");
  hMulti35->GetYaxis()->SetTitle("nEvents");
//   hMulti35->SetFillStyle(3001);  
//   hMulti35->SetFillColor(kYellow);  
  hMulti35->SetLineColor(kBlack);  
//   hMulti35->SetMarkerColor(kYellow); 
  hMulti35->SetMarkerStyle(20);
  hMulti35->SetMarkerSize(1);
  hMulti35->GetXaxis()->SetRangeUser(0, 5);
//   hMulti35->SetStats(0);
  hMulti35->DrawCopy();
  
  /*TCanvas* ccRatio40 = new TCanvas("ccRatio40","ccRatio40",600,600);  
  ccRatio40->Divide(1,2);
  ccRatio40->cd(1);

  hRatio40->GetXaxis()->SetTitle("Forward_pt/Central_pt");
  hRatio40->GetYaxis()->SetTitle("nEvents");
//   hRatio40->SetFillStyle(3001);  
//   hRatio40->SetFillColor(kYellow);  
  hRatio40->SetLineColor(kBlack);  
//   hRatio40->SetMarkerColor(kYellow); 
  hRatio40->SetMarkerStyle(20);
  hRatio40->SetMarkerSize(1);
  hRatio40->GetXaxis()->SetRangeUser(40.,149.);
//   hRatio40->SetStats(0);
  hRatio40->DrawCopy();
  
  hRatio40->Fit("gaus", "L");
  
  ccRatio40->cd(2);
  hMulti40->GetXaxis()->SetTitle("number of central jets");
  hMulti40->GetYaxis()->SetTitle("nEvents");
//   hMulti40->SetFillStyle(3001);  
//   hMulti40->SetFillColor(kYellow);  
  hMulti40->SetLineColor(kBlack);  
//   hMulti40->SetMarkerColor(kYellow); 
  hMulti40->SetMarkerStyle(20);
  hMulti40->SetMarkerSize(1);
  hMulti40->GetXaxis()->SetRangeUser(0, 5);
//   hMulti40->SetStats(0);
  hMulti40->DrawCopy();*/
  
  
  TCanvas* ccRatio45 = new TCanvas("ccRatio45","ccRatio45",600,600);  

  ccRatio45->Divide(1,2);
  ccRatio45->cd(1);
  
  hRatio45->GetXaxis()->SetTitle("Forward_pt/Central_pt");
  hRatio45->GetYaxis()->SetTitle("nEvents");
//   hRatio45->SetFillStyle(3001);  
//   hRatio45->SetFillColor(kYellow);  
//   hRatio45->SetLineColor(kYellow);  
//   hRatio45->SetMarkerColor(kYellow); 
  hRatio45->SetMarkerStyle(20);
  hRatio45->SetMarkerSize(1);
  hRatio45->GetXaxis()->SetRangeUser(35.,149.);
//   hRatio45->SetStats(0);
  hRatio45->DrawCopy();
  hRatio45->Fit("gaus", "L");
  
  
  ccRatio45->cd(2);
  hMulti45->GetXaxis()->SetTitle("number of central jets");
  hMulti45->GetYaxis()->SetTitle("nEvents");
//   hMulti35->SetFillStyle(3001);  
//   hMulti35->SetFillColor(kYellow);  
  hMulti45->SetLineColor(kBlack);  
//   hMulti35->SetMarkerColor(kYellow); 
  hMulti45->SetMarkerStyle(20);
  hMulti45->SetMarkerSize(1);
  hMulti45->GetXaxis()->SetRangeUser(0, 5);
//   hMulti35->SetStats(0);
  hMulti45->DrawCopy();

  
   TCanvas* ccRatio57 = new TCanvas("ccRatio57","ccRatio35",600,600);  
  ccRatio57->Divide(1,2);
  ccRatio57->cd(1);

  hRatio57->GetXaxis()->SetTitle("Forward_pt/Central_pt");
  hRatio57->GetYaxis()->SetTitle("nEvents");
//   hRatio57->SetFillStyle(3001);  
//   hRatio57->SetFillColor(kYellow);  
//   hRatio57->SetLineColor(kYellow);  
//   hRatio57->SetMarkerColor(kYellow); 
  hRatio57->SetMarkerStyle(20);
  hRatio57->SetMarkerSize(1);
  hRatio57->GetXaxis()->SetRangeUser(35.,149.);
//   hRatio57->SetStats(0);
  hRatio57->DrawCopy();
  hRatio57->Fit("gaus+gaus", "L");
  
  ccRatio57->cd(2);
  hMulti57->GetXaxis()->SetTitle("number of central jets");
  hMulti57->GetYaxis()->SetTitle("nEvents");
//   hMulti57->SetFillStyle(3001);  
//   hMulti57->SetFillColor(kYellow);  
  hMulti57->SetLineColor(kBlack);  
//   hMulti57->SetMarkerColor(kYellow); 
  hMulti57->SetMarkerStyle(20);
  hMulti57->SetMarkerSize(1);
  hMulti57->GetXaxis()->SetRangeUser(0, 5);
//   hMulti57->SetStats(0);
  hMulti57->DrawCopy();

  
  TCanvas* ccRatio72 = new TCanvas("ccRatio72","ccRatio72",600,600);  
  ccRatio72->Divide(1,2);
  ccRatio72->cd(1);

  hRatio72->GetXaxis()->SetTitle("Forward_pt/Central_pt");
  hRatio72->GetYaxis()->SetTitle("nEvents");
//   hRatio72->SetFillStyle(3001);  
//   hRatio72->SetFillColor(kYellow);  
//   hRatio72->SetLineColor(kYellow);  
//   hRatio72->SetMarkerColor(kYellow); 
  hRatio72->SetMarkerStyle(20);
  hRatio72->SetMarkerSize(1);
  hRatio72->GetXaxis()->SetRangeUser(35.,149.);
//   hRatio72->SetStats(0);
  hRatio72->DrawCopy();
  hRatio72->Fit("gaus", "L");
  
  
  ccRatio72->cd(2);
  hMulti72->GetXaxis()->SetTitle("number of central jets");
  hMulti72->GetYaxis()->SetTitle("nEvents");
//   hMulti72->SetFillStyle(3001);  
//   hMulti72->SetFillColor(kYellow);  
  hMulti72->SetLineColor(kBlack);  
//   hMulti72->SetMarkerColor(kYellow); 
  hMulti72->SetMarkerStyle(20);
  hMulti72->SetMarkerSize(1);
  hMulti72->GetXaxis()->SetRangeUser(0, 5);
//   hMulti72->SetStats(0);
  hMulti72->DrawCopy();
  
  TCanvas* ccRatio90 = new TCanvas("ccRatio90","ccRatio35",600,600);  
  ccRatio90->Divide(1,2);
  ccRatio90->cd(1);

  hRatio90->GetXaxis()->SetTitle("Forward_pt/Central_pt");
  hRatio90->GetYaxis()->SetTitle("nEvents");
//   hRatio90->SetFillStyle(3001);  
//   hRatio90->SetFillColor(kYellow);  
//   hRatio90->SetLineColor(kYellow);  
//   hRatio90->SetMarkerColor(kYellow); 
  hRatio90->SetMarkerStyle(20);
  hRatio90->SetMarkerSize(1);
  hRatio90->GetXaxis()->SetRangeUser(35.,149.);
//   hRatio90->SetStats(0);
  hRatio90->DrawCopy();
  
  hRatio90->Fit("gaus", "L");
  
  
  ccRatio90->cd(2);
  hMulti90->GetXaxis()->SetTitle("number of central jets");
  hMulti90->GetYaxis()->SetTitle("nEvents");
//   hMulti90->SetFillStyle(3001);  
//   hMulti90->SetFillColor(kYellow);  
  hMulti90->SetLineColor(kBlack);  
//   hMulti90->SetMarkerColor(kYellow); 
  hMulti90->SetMarkerStyle(20);
  hMulti90->SetMarkerSize(1);
  hMulti90->GetXaxis()->SetRangeUser(0, 5);
//   hMulti90->SetStats(0);
  hMulti90->DrawCopy();
  
  TCanvas* ccRatio120 = new TCanvas("ccRatio120","ccRatio35",600,600);  
  ccRatio120->Divide(1,2);
  ccRatio120->cd(1);

  hRatio120->GetXaxis()->SetTitle("Forward_pt/Central_pt");
  hRatio120->GetYaxis()->SetTitle("nEvents");
//   hRatio120->SetFillStyle(3001);  
//   hRatio120->SetFillColor(kYellow);  
//   hRatio120->SetLineColor(kYellow);  
//   hRatio120->SetMarkerColor(kYellow); 
  hRatio120->SetMarkerStyle(20);
  hRatio120->SetMarkerSize(1);
  hRatio120->GetXaxis()->SetRangeUser(35.,149.);
//   hRatio120->SetStats(0);
  hRatio120->DrawCopy();
  
  hRatio120->Fit("gaus", "L");
  
  
  ccRatio120->cd(2);
  hMulti120->GetXaxis()->SetTitle("number of central jets");
  hMulti120->GetYaxis()->SetTitle("nEvents");
//   hMulti120->SetFillStyle(3001);  
//   hMulti120->SetFillColor(kYellow);  
  hMulti120->SetLineColor(kBlack);  
//   hMulti120->SetMarkerColor(kYellow); 
  hMulti120->SetMarkerStyle(20);
  hMulti120->SetMarkerSize(1);
  hMulti120->GetXaxis()->SetRangeUser(0, 5);
//   hMulti120->SetStats(0);
  hMulti120->DrawCopy();
  
//     ccRatio->SetLogy();


TCanvas* ccCJet_pt35 = new TCanvas("ccCJet_pt35","ccCJet_pt35",600,600);  

  hCJet_pt35->GetXaxis()->SetTitle("Central_pt");
  hCJet_pt35->GetYaxis()->SetTitle("nEvents");
//   hCJet_pt35->SetFillStyle(3001);  
//   hCJet_pt35->SetFillColor(kYellow);  
  hCJet_pt35->SetLineColor(kBlack);  
//   hCJet_pt35->SetMarkerColor(kYellow); 
  hCJet_pt35->SetMarkerStyle(20);
  hCJet_pt35->SetMarkerSize(1);
  hCJet_pt35->GetXaxis()->SetRangeUser(35.,149.);
//   hCJet_pt35->SetStats(0);
  hCJet_pt35->DrawCopy();
  
  TCanvas* ccCJet_pt45 = new TCanvas("ccCJet_pt45","ccCJet_pt45",600,600);  

  hCJet_pt45->GetXaxis()->SetTitle("Central_pt");
  hCJet_pt45->GetYaxis()->SetTitle("nEvents");
//   hCJet_pt45->SetFillStyle(3001);  
//   hCJet_pt45->SetFillColor(kYellow);  
  hCJet_pt45->SetLineColor(kBlack);  
//   hCJet_pt45->SetMarkerColor(kYellow); 
  hCJet_pt45->SetMarkerStyle(20);
  hCJet_pt45->SetMarkerSize(1);
  hCJet_pt45->GetXaxis()->SetRangeUser(35.,149.);
//   hCJet_pt45->SetStats(0);
  hCJet_pt45->DrawCopy();
  
  TCanvas* ccCJet_pt72 = new TCanvas("ccCJet_pt72","ccCJet_pt72",600,600);  

  hCJet_pt72->GetXaxis()->SetTitle("Central_pt");
  hCJet_pt72->GetYaxis()->SetTitle("nEvents");
//   hCJet_pt72->SetFillStyle(3001);  
//   hCJet_pt72->SetFillColor(kYellow);  
  hCJet_pt72->SetLineColor(kBlack);  
//   hCJet_pt72->SetMarkerColor(kYellow); 
  hCJet_pt72->SetMarkerStyle(20);
  hCJet_pt72->SetMarkerSize(1);
  hCJet_pt72->GetXaxis()->SetRangeUser(35.,149.);
//   hCJet_pt72->SetStats(0);
  hCJet_pt72->DrawCopy();
  

  
  /*TCanvas* ccRatioBin = new TCanvas("ccRatioBin","ccRatioBin",600,600);  

  hRatioBin->SetLineColor(kRed);
  hRatioBin->SetMarkerColor(kRed);
  hRatioBin->SetMarkerStyle(21);
  hRatioBin->SetMarkerSize(1);
  hRatioBin->DrawCopy("E");

  
   leg = new TLegend(0.6,0.7,0.9,0.9,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   */
   
//    leg->AddEntry(hRatioBin,"Herwig", "lpf");
   
//    leg->Draw();
  
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
}

#ifndef __CINT__
int main () { CrossSection(); return 0; }  // Main program when run stand-alone
#endif
