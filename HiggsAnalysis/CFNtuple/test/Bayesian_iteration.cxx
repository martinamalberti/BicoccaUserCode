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
using std:cerr;
using std::endl;

#include "TRandom.h"
#include "TH1D.h"
#include "TLine.h"

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


void Bayesian_iteration()
{
#ifdef __CINT__
  gSystem->Load("libRooUnfold");
#endif

  Float_t lowEdge[9] = {27,35,45,57,72,90,120,150,1000};	//mi sto creando un istogramma con bin di grandezza fissata da me che poi passo alla matrice di risposta che lo usa
   int NBIN = 8;
 
//   Float_t lowEdge[16] = {27,31,35,40,45,51,57,64.5,72,81,90,105,120,135,150,1000};	//mi sto creando un istogramma con bin di grandezza fissata da me che poi passo alla matrice di risposta che lo usa
//   int NBIN = 15;
  
//   Float_t lowEdge[20] = {27,29,31,33,35,37.5,40,42.5,45,51,57,64.5,72,81,90,105,120,135,150,1000};	//mi sto creando un istogramma con bin di grandezza fissata da me che poi passo alla matrice di risposta che lo usa
//   int NBIN = 19;
  
  TH1F myH("myH","myH",NBIN,lowEdge);
  TH2D* hResponseMatrixFJet = new TH2D ("hResponseMatrixFJet", "Response Matrix FJet",NBIN,lowEdge,NBIN,lowEdge);
  TH2D* hResponseMatrixCJet = new TH2D ("hResponseMatrixCJet", "Response Matrix CJet",NBIN,lowEdge,NBIN,lowEdge);

  TH2D* hResponseMatrixFJet_temp = new TH2D ("hResponseMatrixFJet_temp", "Response Matrix FJet",NBIN,lowEdge,NBIN,lowEdge);
  TH2D* hResponseMatrixCJet_temp = new TH2D ("hResponseMatrixCJet_temp", "Response Matrix CJet",NBIN,lowEdge,NBIN,lowEdge);

  
  cout << "==================================== TRAIN ====================================" << endl;
  
  RooUnfoldResponse responseFJet (&myH,&myH);
  RooUnfoldResponse responseCJet (&myH,&myH);
  
  
  //PYHTIA SU HERWIG 
  /*
     TFile FileTest1("../input/Unfolding/qcd_15_herwigjimmy.root","READ");
    TFile FileTrain1("../input/Unfolding/qcd_15_pythia.root","READ");
     TFile FileTest2("../input/Unfolding/qcd_30_herwigjimmy.root","READ");
    TFile FileTrain2("../input/Unfolding/qcd_30_pythia.root","READ");*/

   //HERWIG SU PYTHIA
  /*  TFile FileTest1("../input/Unfolding/qcd_15_pythia.root","READ");
   TFile FileTrain1("../input/Unfolding/qcd_15_herwigjimmy.root","READ");
    TFile FileTest2("../input/Unfolding/qcd_30_pythia.root","READ");
   TFile FileTrain2("../input/Unfolding/qcd_30_herwigjimmy.root","READ");*/

 //PYTHIA SU PYTHIA
    TFile FileTest1("../input/Unfolding/qcd_15_pythia.root","READ");
    TFile FileTrain1("../input/Unfolding/qcd_15_pythia.root","READ");
    TFile FileTest2("../input/Unfolding/qcd_30_pythia.root","READ");
    TFile FileTrain2("../input/Unfolding/qcd_30_pythia.root","READ");


  
  double lumi = 5.1e-3;
 /* 
  double xsec1Pythia = 8.762e8 / 6190500.;
  double xsec2Pythia = 6.041e7 / 4918016.;
  double xsec1Herwig = 714000000. / 1631667.;
  double xsec2Herwig =  49240000. / 1310829.;
  double Threshold_G_FJet = 57;
  double Threshold_G_CJet = 57;*/
  
  //PYTHIA SU HERWIG
    double xsec1 =  8.762e8 / 6190500.*1000 ;
    double xsec2 = 6.041e7 / 4918016.*1000 ;
    double xsec1Herwig = 714000000. / 1631667.*1000;
    double xsec2Herwig =  49240000. / 1310829.*1000;
  
  //HERWIG SU PYTHIA
/*   double xsec1Herwig =  8.762e8 / 6190500.*1000 ;
   double xsec2Herwig = 6.041e7 / 4918016.*1000 ;
   double xsec1 = 714000000. / 1631667.*1000;
   double xsec2 =  49240000. / 1310829.*1000;*/

//PYTHIA SU PYTHIA
   /* double xsec1 =  8.762e8 / 6190500.*1000 ;
    double xsec2 = 6.041e7 / 4918016.*1000 ;
    double xsec1Herwig = xsec1;
    double xsec2Herwig =  xsec2;*/
  
  
  
  int test = 1; //if set to 0 use Pythia else use Herwig
  
  double Threshold_G_FJet = 57;
  double Threshold_G_CJet = 57;
  
  TTree* TreeTrain1 = (TTree*) FileTrain1.Get("AnaHiggs");
  TTree* TreeTrain2 = (TTree*) FileTrain2.Get("AnaHiggs");
  
  TTree* TreeTest1 = (TTree*) FileTest1.Get("AnaHiggs");
  TTree* TreeTest2 = (TTree*) FileTest2.Get("AnaHiggs");
  
  Float_t G_FJet_Pt; //~~~~ had
  Float_t S_FJet_Pt; //~~~~ reco

 Float_t G_CJet_Pt; //~~~~ had
 Float_t S_CJet_Pt; //~~~~ reco
 
 TreeTrain1->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt);
 TreeTrain1->SetBranchAddress("S_FJet_Pt",&S_FJet_Pt);
 TreeTrain1->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt);
 TreeTrain1->SetBranchAddress("S_CJet_Pt",&S_CJet_Pt);

 TreeTrain2->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt);
 TreeTrain2->SetBranchAddress("S_FJet_Pt",&S_FJet_Pt);
 TreeTrain2->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt);
 TreeTrain2->SetBranchAddress("S_CJet_Pt",&S_CJet_Pt);
 
 TreeTest1->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt);
 TreeTest1->SetBranchAddress("S_FJet_Pt",&S_FJet_Pt);
 TreeTest1->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt);
 TreeTest1->SetBranchAddress("S_CJet_Pt",&S_CJet_Pt);

 TreeTest2->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt);
 TreeTest2->SetBranchAddress("S_FJet_Pt",&S_FJet_Pt);
 TreeTest2->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt);
 TreeTest2->SetBranchAddress("S_CJet_Pt",&S_CJet_Pt);
 
 //Riempio la response matrix con Pythia 
 
 for (Int_t iEvt= 0; iEvt<TreeTrain1->GetEntries(); iEvt++) {
    TreeTrain1->GetEntry(iEvt);
//     if (S_FJet_Pt>0 && G_FJet_Pt>0)   std::cerr << " S_FJet_Pt = " << S_FJet_Pt << " G_FJet_Pt = " << G_FJet_Pt << std::endl;
    
    if (S_FJet_Pt>0 && G_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0)  {
     if (G_FJet_Pt < Threshold_G_FJet) {
//      responseFJet.Fill (S_FJet_Pt, G_FJet_Pt, xsec1 / xsec_mean);
      hResponseMatrixFJet_temp->Fill(S_FJet_Pt, G_FJet_Pt);
     }
     if (G_CJet_Pt < Threshold_G_CJet) {
//      responseCJet.Fill (S_CJet_Pt, G_CJet_Pt, xsec1 / xsec_mean);
      hResponseMatrixCJet_temp->Fill(S_CJet_Pt, G_CJet_Pt);
     }
    }
//       response.Miss (xt);
  }
  
  
  //Moltiplico il contenuto dei bin della matrice di risposta e i loro errori per la sezione d'urto, se la riempivo direttamente coi
  // bin pesati, l'errore veniva calcolato male (bassissimo poichè il bincontent era molto più alto...
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   for (int iBinY = 0; iBinY<NBIN; iBinY++){
    hResponseMatrixCJet_temp->SetBinError(iBinX+1,iBinY+1,hResponseMatrixCJet_temp->GetBinError(iBinX+1,iBinY+1) * xsec1);
    hResponseMatrixFJet_temp->SetBinError(iBinX+1,iBinY+1,hResponseMatrixFJet_temp->GetBinError(iBinX+1,iBinY+1) * xsec1);

    hResponseMatrixCJet_temp->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixCJet_temp->GetBinContent(iBinX+1,iBinY+1) * xsec1);
    hResponseMatrixFJet_temp->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixFJet_temp->GetBinContent(iBinX+1,iBinY+1) * xsec1);
   }
  }    
  
  //Per i jet sopra i 57 GeV riempio e peso una seconda matrice di risposta. (poi la sommerò alla prima)
  
  for (Int_t iEvt= 0; iEvt<TreeTrain2->GetEntries(); iEvt++) {
   TreeTrain2->GetEntry(iEvt);
   //     if (S_FJet_Pt>0 && G_FJet_Pt>0)   std::cerr << " S_FJet_Pt = " << S_FJet_Pt << " G_FJet_Pt = " << G_FJet_Pt << std::endl;
   
   if (S_FJet_Pt>0 && G_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0)  {
    if (G_FJet_Pt >= Threshold_G_FJet) {
//     responseFJet.Fill (S_FJet_Pt, G_FJet_Pt, xsec2 / xsec_mean);
     hResponseMatrixFJet->Fill(S_FJet_Pt, G_FJet_Pt);
    }
    if (G_CJet_Pt >= Threshold_G_CJet) {
//     responseCJet.Fill (S_CJet_Pt, G_CJet_Pt, xsec2 / xsec_mean);
     hResponseMatrixCJet->Fill(S_CJet_Pt, G_CJet_Pt);
    }
   }
   //       response.Miss (xt);
  }

  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   for (int iBinY = 0; iBinY<NBIN; iBinY++){
    hResponseMatrixCJet->SetBinError(iBinX+1,iBinY+1,hResponseMatrixCJet->GetBinError(iBinX+1,iBinY+1) * xsec2);
    hResponseMatrixFJet->SetBinError(iBinX+1,iBinY+1,hResponseMatrixFJet->GetBinError(iBinX+1,iBinY+1) * xsec2);

    hResponseMatrixCJet->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixCJet->GetBinContent(iBinX+1,iBinY+1) * xsec2);
    hResponseMatrixFJet->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixFJet->GetBinContent(iBinX+1,iBinY+1) * xsec2);
   }
  }    
 
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~ scale for variable size binning ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
//  hResponseMatrixFJet->Sumw2();
//  hResponseMatrixCJet->Sumw2();
  
  
  //aggiungo la prima parte di hResponse alla seconda
  hResponseMatrixFJet->Add(hResponseMatrixFJet_temp);
  hResponseMatrixCJet->Add(hResponseMatrixCJet_temp);
  
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   for (int iBinY = 0; iBinY<NBIN; iBinY++){
   
    
//    hResponseMatrixFJet->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixFJet->GetBinContent(iBinX+1,iBinY+1) / xsecScale ) ;
//    hResponseMatrixCJet->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixCJet->GetBinContent(iBinX+1,iBinY+1) / xsecScale ) ;

    hResponseMatrixFJet->SetBinError(iBinX+1,iBinY+1,hResponseMatrixFJet->GetBinError(iBinX+1,iBinY+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / (lowEdge[iBinY+1] - lowEdge[iBinY]));
    hResponseMatrixCJet->SetBinError(iBinX+1,iBinY+1,hResponseMatrixCJet->GetBinError(iBinX+1,iBinY+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / (lowEdge[iBinY+1] - lowEdge[iBinY]));

    hResponseMatrixFJet->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixFJet->GetBinContent(iBinX+1,iBinY+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / (lowEdge[iBinY+1] - lowEdge[iBinY]));
    hResponseMatrixCJet->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixCJet->GetBinContent(iBinX+1,iBinY+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / (lowEdge[iBinY+1] - lowEdge[iBinY]));

   }
  }
  
  /*TCanvas* ccResponseCJet = new TCanvas("ccResponseCJet","ccResponseCJet",600,600);  
  hResponseMatrixCJet->SetLineColor(kRed);
  hResponseMatrixCJet->SetMarkerColor(kRed);
  hResponseMatrixCJet->SetMarkerStyle(20);
  hResponseMatrixCJet->Draw("LEGOE");
  ccResponseCJet->SetLogz();
  
  TCanvas* ccResponseFJet = new TCanvas("ccResponseFJet","ccResponseFJet",600,600);  
  hResponseMatrixFJet->SetLineColor(kRed);
  hResponseMatrixFJet->SetMarkerColor(kRed);
  hResponseMatrixFJet->SetMarkerStyle(20);
  hResponseMatrixFJet->Draw("LEGOE");
  ccResponseFJet->SetLogz();*/
  
  ///DISTRIBUZIONI SU CUI TESTARE IL METODO: STESSO MC O MC DIVERSO
  
  //STESSO MC: PYTHIA
  
  
  
  cout << "==================================== MC Distributions =====================================" << endl;
    
  TH1D* hTrueFJet_MC_temp = new TH1D ("hTrueFJet_MC_temp", "MC Truth FJet",    NBIN , lowEdge);
  TH1D* hMeasFJet_MC_temp = new TH1D ("hMeasFJet_MC_temp", "MC Measured FJet", NBIN , lowEdge);
  TH1D* hTrueCJet_MC_temp = new TH1D ("hTrueCJet_MC_temp", "MC Truth CJet",    NBIN , lowEdge);
  TH1D* hMeasCJet_MC_temp = new TH1D ("hMeasCJet_MC_temp", "MC Measured CJet", NBIN , lowEdge);

  TH1D* hTrueFJet_MC = new TH1D ("hTrueFJet_MC", "MC Truth FJet",    NBIN , lowEdge);
  TH1D* hMeasFJet_MC = new TH1D ("hMeasFJet_MC", "MC Measured FJet", NBIN , lowEdge);
  TH1D* hTrueCJet_MC = new TH1D ("hTrueCJet_MC", "MC Truth CJet",    NBIN , lowEdge);
  TH1D* hMeasCJet_MC = new TH1D ("hMeasCJet_MC", "MC Measured CJet", NBIN , lowEdge);
  
  for (Int_t iEvt= 0; iEvt<TreeTrain1->GetEntries(); iEvt++) {
   TreeTrain1->GetEntry(iEvt);
   //    std::cerr << " S_FJet_Pt = " << S_FJet_Pt << std::endl;
   if (S_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0 && G_FJet_Pt>0) {
    if (G_FJet_Pt < Threshold_G_FJet) {
     hTrueFJet_MC_temp->Fill(G_FJet_Pt);
     hMeasFJet_MC_temp->Fill(S_FJet_Pt);
    }
    if (G_CJet_Pt < Threshold_G_CJet) {
     hTrueCJet_MC_temp->Fill(G_CJet_Pt);
     hMeasCJet_MC_temp->Fill(S_CJet_Pt);
    }
   }
  }
  
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hTrueFJet_MC_temp->SetBinError(iBinX+1,hTrueFJet_MC_temp->GetBinError(iBinX+1) * xsec1);
   hTrueCJet_MC_temp->SetBinError(iBinX+1,hTrueCJet_MC_temp->GetBinError(iBinX+1) * xsec1); 
   hMeasFJet_MC_temp->SetBinError(iBinX+1,hMeasFJet_MC_temp->GetBinError(iBinX+1) * xsec1);
   hMeasCJet_MC_temp->SetBinError(iBinX+1,hMeasCJet_MC_temp->GetBinError(iBinX+1) * xsec1);

   hTrueFJet_MC_temp->SetBinContent(iBinX+1,hTrueFJet_MC_temp->GetBinContent(iBinX+1) * xsec1);
   hTrueCJet_MC_temp->SetBinContent(iBinX+1,hTrueCJet_MC_temp->GetBinContent(iBinX+1) * xsec1);
   hMeasFJet_MC_temp->SetBinContent(iBinX+1,hMeasFJet_MC_temp->GetBinContent(iBinX+1) * xsec1);
   hMeasCJet_MC_temp->SetBinContent(iBinX+1,hMeasCJet_MC_temp->GetBinContent(iBinX+1) * xsec1);   
  }
  
  
  for (Int_t iEvt= 0; iEvt<TreeTrain2->GetEntries(); iEvt++) {
   TreeTrain2->GetEntry(iEvt);
   //    std::cerr << " S_FJet_Pt = " << S_FJet_Pt << std::endl;
   if (S_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0 && G_FJet_Pt>0) {
    if (G_FJet_Pt > Threshold_G_FJet) {
     hTrueFJet_MC->Fill(G_FJet_Pt);
     hMeasFJet_MC->Fill(S_FJet_Pt);
    }
    if (G_CJet_Pt > Threshold_G_CJet) {
     hTrueCJet_MC->Fill(G_CJet_Pt);
     hMeasCJet_MC->Fill(S_CJet_Pt);
    }
   }
  }  
  
   
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hTrueFJet_MC->SetBinError(iBinX+1,hTrueFJet_MC->GetBinError(iBinX+1) * xsec2);
   hTrueCJet_MC->SetBinError(iBinX+1,hTrueCJet_MC->GetBinError(iBinX+1) * xsec2); 
   hMeasFJet_MC->SetBinError(iBinX+1,hMeasFJet_MC->GetBinError(iBinX+1) * xsec2);
   hMeasCJet_MC->SetBinError(iBinX+1,hMeasCJet_MC->GetBinError(iBinX+1) * xsec2);

   hTrueFJet_MC->SetBinContent(iBinX+1,hTrueFJet_MC->GetBinContent(iBinX+1) * xsec2);
   hTrueCJet_MC->SetBinContent(iBinX+1,hTrueCJet_MC->GetBinContent(iBinX+1) * xsec2);
   hMeasFJet_MC->SetBinContent(iBinX+1,hMeasFJet_MC->GetBinContent(iBinX+1) * xsec2);
   hMeasCJet_MC->SetBinContent(iBinX+1,hMeasCJet_MC->GetBinContent(iBinX+1) * xsec2);   
  }

  hTrueFJet_MC->Sumw2();
  hTrueCJet_MC->Sumw2();
  hMeasFJet_MC->Sumw2();
  hMeasCJet_MC->Sumw2();

  hTrueFJet_MC->Add(hTrueFJet_MC_temp);
  hTrueCJet_MC->Add(hTrueCJet_MC_temp);
  hMeasFJet_MC->Add(hMeasFJet_MC_temp);
  hMeasCJet_MC->Add(hMeasCJet_MC_temp);
  
   for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hTrueFJet_MC->SetBinError(iBinX+1,hTrueFJet_MC->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hTrueCJet_MC->SetBinError(iBinX+1,hTrueCJet_MC->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hMeasFJet_MC->SetBinError(iBinX+1,hMeasFJet_MC->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hMeasCJet_MC->SetBinError(iBinX+1,hMeasCJet_MC->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));  
  
   hTrueFJet_MC->SetBinContent(iBinX+1,hTrueFJet_MC->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hTrueCJet_MC->SetBinContent(iBinX+1,hTrueCJet_MC->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hMeasFJet_MC->SetBinContent(iBinX+1,hMeasFJet_MC->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hMeasCJet_MC->SetBinContent(iBinX+1,hMeasCJet_MC->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));       
  }
  
  
  
  //MC DIVERSO: HERWIG
  cout << "==================================== MC Distributions HERWIG=====================================" << endl;
    
  TH1D* hTrueFJet_MC_temp_test = new TH1D ("hTrueFJet_MC_temp_test", "MC Truth FJet test",    NBIN , lowEdge);
  TH1D* hMeasFJet_MC_temp_test = new TH1D ("hMeasFJet_MC_temp_test", "MC Measured FJet test", NBIN , lowEdge);
  TH1D* hTrueCJet_MC_temp_test = new TH1D ("hTrueCJet_MC_temp_test", "MC Truth CJet test",    NBIN , lowEdge);
  TH1D* hMeasCJet_MC_temp_test = new TH1D ("hMeasCJet_MC_temp_test", "MC Measured CJet test", NBIN , lowEdge);

  TH1D* hTrueFJet_MC_test = new TH1D ("hTrueFJet_MC_test", "MC Truth FJet test",    NBIN , lowEdge);
  TH1D* hMeasFJet_MC_test = new TH1D ("hMeasFJet_MC_test", "MC Measured FJet test", NBIN , lowEdge);
  TH1D* hTrueCJet_MC_test = new TH1D ("hTrueCJet_MC_test", "MC Truth CJet test",    NBIN , lowEdge);
  TH1D* hMeasCJet_MC_test = new TH1D ("hMeasCJet_MC_test", "MC Measured CJet test", NBIN , lowEdge);
  
  for (Int_t iEvt= 0; iEvt<TreeTest1->GetEntries(); iEvt++) {
   TreeTest1->GetEntry(iEvt);
   //    std::cerr << " S_FJet_Pt = " << S_FJet_Pt << std::endl;
   if (S_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0 && G_FJet_Pt>0) {
    if (G_FJet_Pt < Threshold_G_FJet) {
     hTrueFJet_MC_temp_test->Fill(G_FJet_Pt);
     hMeasFJet_MC_temp_test->Fill(S_FJet_Pt);
    }
    if (G_CJet_Pt < Threshold_G_CJet) {
     hTrueCJet_MC_temp_test->Fill(G_CJet_Pt);
     hMeasCJet_MC_temp_test->Fill(S_CJet_Pt);
    }
   }
  }
  
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hTrueFJet_MC_temp_test->SetBinError(iBinX+1,hTrueFJet_MC_temp_test->GetBinError(iBinX+1) * xsec1Herwig);
   hTrueCJet_MC_temp_test->SetBinError(iBinX+1,hTrueCJet_MC_temp_test->GetBinError(iBinX+1) * xsec1Herwig); 
   hMeasFJet_MC_temp_test->SetBinError(iBinX+1,hMeasFJet_MC_temp_test->GetBinError(iBinX+1) * xsec1Herwig);
   hMeasCJet_MC_temp_test->SetBinError(iBinX+1,hMeasCJet_MC_temp_test->GetBinError(iBinX+1) * xsec1Herwig);

   hTrueFJet_MC_temp_test->SetBinContent(iBinX+1,hTrueFJet_MC_temp_test->GetBinContent(iBinX+1) * xsec1Herwig);
   hTrueCJet_MC_temp_test->SetBinContent(iBinX+1,hTrueCJet_MC_temp_test->GetBinContent(iBinX+1) * xsec1Herwig);
   hMeasFJet_MC_temp_test->SetBinContent(iBinX+1,hMeasFJet_MC_temp_test->GetBinContent(iBinX+1) * xsec1Herwig);
   hMeasCJet_MC_temp_test->SetBinContent(iBinX+1,hMeasCJet_MC_temp_test->GetBinContent(iBinX+1) * xsec1Herwig);   
  }
  
  
  for (Int_t iEvt= 0; iEvt<TreeTest2->GetEntries(); iEvt++) {
   TreeTest2->GetEntry(iEvt);
   //    std::cerr << " S_FJet_Pt = " << S_FJet_Pt << std::endl;
   if (S_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0 && G_FJet_Pt>0) {
    if (G_FJet_Pt > Threshold_G_FJet) {
     hTrueFJet_MC_test->Fill(G_FJet_Pt);
     hMeasFJet_MC_test->Fill(S_FJet_Pt);
    }
    if (G_CJet_Pt > Threshold_G_CJet) {
     hTrueCJet_MC_test->Fill(G_CJet_Pt);
     hMeasCJet_MC_test->Fill(S_CJet_Pt);
    }
   }
  }  
  
   
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hTrueFJet_MC_test->SetBinError(iBinX+1,hTrueFJet_MC_test->GetBinError(iBinX+1) * xsec2Herwig);
   hTrueCJet_MC_test->SetBinError(iBinX+1,hTrueCJet_MC_test->GetBinError(iBinX+1) * xsec2Herwig); 
   hMeasFJet_MC_test->SetBinError(iBinX+1,hMeasFJet_MC_test->GetBinError(iBinX+1) * xsec2Herwig);
   hMeasCJet_MC_test->SetBinError(iBinX+1,hMeasCJet_MC_test->GetBinError(iBinX+1) * xsec2Herwig);

   hTrueFJet_MC_test->SetBinContent(iBinX+1,hTrueFJet_MC_test->GetBinContent(iBinX+1) * xsec2Herwig);
   hTrueCJet_MC_test->SetBinContent(iBinX+1,hTrueCJet_MC_test->GetBinContent(iBinX+1) * xsec2Herwig);
   hMeasFJet_MC_test->SetBinContent(iBinX+1,hMeasFJet_MC_test->GetBinContent(iBinX+1) * xsec2Herwig);
   hMeasCJet_MC_test->SetBinContent(iBinX+1,hMeasCJet_MC_test->GetBinContent(iBinX+1) * xsec2Herwig);   
  }

  hTrueFJet_MC_test->Sumw2();
  hTrueCJet_MC_test->Sumw2();
  hMeasFJet_MC_test->Sumw2();
  hMeasCJet_MC_test->Sumw2();

  hTrueFJet_MC_test->Add(hTrueFJet_MC_temp_test);
  hTrueCJet_MC_test->Add(hTrueCJet_MC_temp_test);
  hMeasFJet_MC_test->Add(hMeasFJet_MC_temp_test);
  hMeasCJet_MC_test->Add(hMeasCJet_MC_temp_test);
  
  
   for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hTrueFJet_MC_test->SetBinError(iBinX+1,hTrueFJet_MC_test->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hTrueCJet_MC_test->SetBinError(iBinX+1,hTrueCJet_MC_test->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hMeasFJet_MC_test->SetBinError(iBinX+1,hMeasFJet_MC_test->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hMeasCJet_MC_test->SetBinError(iBinX+1,hMeasCJet_MC_test->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));  
  
   hTrueFJet_MC_test->SetBinContent(iBinX+1,hTrueFJet_MC_test->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hTrueCJet_MC_test->SetBinContent(iBinX+1,hTrueCJet_MC_test->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hMeasFJet_MC_test->SetBinContent(iBinX+1,hMeasFJet_MC_test->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hMeasCJet_MC_test->SetBinContent(iBinX+1,hMeasCJet_MC_test->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));       
  }
  
  
  
  
  cout << "==================================== Create Response Matrix =====================================" << endl;
  //Riempio l'intera matrice di risposta tutta di un botto passandogli due istogrammiinvece che un bin alla volta
   
  
  responseFJet.Setup(hMeasFJet_MC,hTrueFJet_MC,hResponseMatrixFJet);
  responseCJet.Setup(hMeasCJet_MC,hTrueCJet_MC,hResponseMatrixCJet);


 
  cout << "==================================== UNFOLD ON MC ===================================" << endl;

  
  ///Metodo BIN-TO-BIN
  RooUnfoldBinByBin unfoldCJet_Bin (&responseCJet, hMeasCJet_MC_test);
  TH1D* hRecoCJet_Bin = (TH1D*) unfoldCJet_Bin.Hreco();

    // RooUnfoldBinByBin unfoldFJet_5 (&responseFJet, hMeasFJet_MC_test);
  
    ///Metodo Bayesiano
//for 5 iterations    
        // RooUnfoldBayes    unfoldFJet_5 (&responseFJet, hMeasFJet_MC, 10);    // OR
  RooUnfoldBayes    unfoldFJet_1 (&responseFJet, hMeasFJet_MC_test, 5);    // OR
  
  TH1D* hRecoFJet_1 = (TH1D*) unfoldFJet_1.Hreco();
  hRecoFJet_1->SetTitle("FJetUnfold_1");
  // RooUnfoldBayes    unfoldCJet_5 (&responseCJet, hMeasCJet_MC, 10);    // OR
   
  RooUnfoldBayes    unfoldCJet_1 (&responseCJet, hMeasCJet_MC_test, 5);
  TH1D* hRecoCJet_1 = (TH1D*) unfoldCJet_1.Hreco();
  hRecoCJet_1->SetTitle("CJetUnfold_1");
    
  hRecoFJet_1->Sumw2();
  hRecoCJet_1->Sumw2();
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
    hRecoFJet_1->SetBinContent(iBinX+1,hRecoFJet_1->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
    hRecoCJet_1->SetBinContent(iBinX+1,hRecoCJet_1->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));    
   }
   
   //for 10 iterations
    // RooUnfoldBayes    unfoldFJet_5 (&responseFJet, hMeasFJet_MC, 10);    // OR
   RooUnfoldBayes    unfoldFJet_5 (&responseFJet, hMeasFJet_MC_test, 10);    // OR
//   RooUnfoldBayes    unfoldFJet_5 (&responseFJet, hMeasFJet_MC_test, 5);    // OR
  
  TH1D* hRecoFJet_5 = (TH1D*) unfoldFJet_5.Hreco();
  hRecoFJet_5->SetTitle("FJetUnfold_5");
  // RooUnfoldBayes    unfoldCJet_5 (&responseCJet, hMeasCJet_MC, 10);    // OR
   RooUnfoldBayes    unfoldCJet_5 (&responseCJet, hMeasCJet_MC_test, 10);
//   RooUnfoldBayes    unfoldCJet_5 (&responseCJet, hMeasCJet_MC_test, 5);
  TH1D* hRecoCJet_5 = (TH1D*) unfoldCJet_5.Hreco();
  hRecoCJet_5->SetTitle("CJetUnfold_5");
    
  hRecoFJet_5->Sumw2();
  hRecoCJet_5->Sumw2();
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
    hRecoFJet_5->SetBinContent(iBinX+1,hRecoFJet_5->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
    hRecoCJet_5->SetBinContent(iBinX+1,hRecoCJet_5->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));    
   }
 
 
  //for Number of Iteration = 10
 std::cerr<<"Number of Bayesian iterations = 10"<<endl;
  //RooUnfoldBayes    unfoldFJet_10 (&responseFJet, hMeasFJet_MC, 25);    // OR
  RooUnfoldBayes    unfoldFJet_10 (&responseFJet, hMeasFJet_MC_test, 25);    // OR
  TH1D* hRecoFJet_10 = (TH1D*) unfoldFJet_10.Hreco();
  hRecoFJet_10->SetTitle("FJetUnfold_10");
  
  // RooUnfoldBayes    unfoldCJet_10 (&responseCJet, hMeasCJet_MC, 25);    // OR
     RooUnfoldBayes    unfoldCJet_10 (&responseCJet, hMeasCJet_MC_test, 25);    // OR
  TH1D* hRecoCJet_10 = (TH1D*) unfoldCJet_10.Hreco();
  hRecoCJet_10->SetTitle("CJetUnfold_10");
    
  hRecoFJet_10->Sumw2();
  hRecoCJet_10->Sumw2();
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hRecoFJet_10->SetBinContent(iBinX+1,hRecoFJet_10->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hRecoCJet_10->SetBinContent(iBinX+1,hRecoCJet_10->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));    
  }
 
  //for Number of Iteration = 15
  std::cout<<"Number of Bayesian iterations = 15"<<endl;
  // RooUnfoldBayes    unfoldFJet_15 (&responseFJet, hMeasFJet_MC, 50);    // OR
     RooUnfoldBayes    unfoldFJet_15 (&responseFJet, hMeasFJet_MC_test, 50);    // OR
  TH1D* hRecoFJet_15 = (TH1D*) unfoldFJet_15.Hreco();
  hRecoFJet_15->SetTitle("FJetUnfold_15");
  
  //RooUnfoldBayes    unfoldCJet_15 (&responseCJet, hMeasCJet_MC, 50);    // OR
    RooUnfoldBayes    unfoldCJet_15 (&responseCJet, hMeasCJet_MC_test, 50);    // OR
  TH1D* hRecoCJet_15 = (TH1D*) unfoldCJet_15.Hreco();
  hRecoCJet_15->SetTitle("CJetUnfold_15");
    
  hRecoFJet_15->Sumw2();
  hRecoCJet_5->Sumw2();
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hRecoFJet_15->SetBinContent(iBinX+1,hRecoFJet_15->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hRecoCJet_15->SetBinContent(iBinX+1,hRecoCJet_15->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));    
  }
 
  //for Number of Iteration = 20
  std::cout<<"Number of Bayesian iterations = 20"<<endl;
   //RooUnfoldBayes    unfoldFJet_20 (&responseFJet, hMeasFJet_MC, 100);    // OR
     RooUnfoldBayes    unfoldFJet_20 (&responseFJet, hMeasFJet_MC_test, 100);    // OR
  TH1D* hRecoFJet_20 = (TH1D*) unfoldFJet_20.Hreco();
  hRecoFJet_20->SetTitle("FJetUnfold_20");
  
  //RooUnfoldBayes    unfoldCJet_20 (&responseCJet, hMeasCJet_MC, 100);    // OR
    RooUnfoldBayes    unfoldCJet_20 (&responseCJet, hMeasCJet_MC_test, 100);    // OR
  TH1D* hRecoCJet_20 = (TH1D*) unfoldCJet_20.Hreco();
  hRecoCJet_20->SetTitle("CJetUnfold_20");
    
  hRecoFJet_20->Sumw2();
  hRecoCJet_20->Sumw2();
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hRecoFJet_20->SetBinContent(iBinX+1,hRecoFJet_20->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hRecoCJet_20->SetBinContent(iBinX+1,hRecoCJet_20->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));    
  }
  
 //for Number of Iteration = 25
  std::cout<<"Number of Bayesian iterations = 25"<<endl;
   // RooUnfoldBayes    unfoldFJet_25 (&responseFJet, hMeasFJet_MC, 200);    // OR
      RooUnfoldBayes    unfoldFJet_25 (&responseFJet, hMeasFJet_MC_test, 200);    // OR
  TH1D* hRecoFJet_25 = (TH1D*) unfoldFJet_25.Hreco();
  hRecoFJet_25->SetTitle("FJetUnfold_25");
  
  //RooUnfoldBayes    unfoldCJet_25 (&responseCJet, hMeasCJet_MC, 200);    // OR
    RooUnfoldBayes    unfoldCJet_25 (&responseCJet, hMeasCJet_MC_test, 200);    // OR
  TH1D* hRecoCJet_25 = (TH1D*) unfoldCJet_25.Hreco();
  hRecoCJet_25->SetTitle("CJetUnfold_25");
    
  hRecoFJet_25->Sumw2();
  hRecoCJet_25->Sumw2();
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hRecoFJet_25->SetBinContent(iBinX+1,hRecoFJet_25->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hRecoCJet_25->SetBinContent(iBinX+1,hRecoCJet_25->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));    
  }
 
  //for Number of Iteration = 50
  std::cout<<"Number of Bayesian iterations = 50"<<endl;
  //RooUnfoldBayes    unfoldFJet_50 (&responseFJet, hMeasFJet_MC, 500);    // OR
    RooUnfoldBayes    unfoldFJet_50 (&responseFJet, hMeasFJet_MC_test, 500);    // OR
  TH1D* hRecoFJet_50 = (TH1D*) unfoldFJet_50.Hreco();
  hRecoFJet_50->SetTitle("FJetUnfold_50");
  
  //RooUnfoldBayes    unfoldCJet_50 (&responseCJet, hMeasCJet_MC, 500);    // OR
    RooUnfoldBayes    unfoldCJet_50 (&responseCJet, hMeasCJet_MC_test, 500);    // OR
  TH1D* hRecoCJet_50 = (TH1D*) unfoldCJet_50.Hreco();
  hRecoCJet_50->SetTitle("CJetUnfold_50");
    
  hRecoFJet_50->Sumw2();
  hRecoCJet_50->Sumw2();
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hRecoFJet_50->SetBinContent(iBinX+1,hRecoFJet_50->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hRecoCJet_50->SetBinContent(iBinX+1,hRecoCJet_50->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));    
  }
  
    //for Number of Iteration = 100
  std::cout<<"Number of Bayesian iterations = 100"<<endl;
  //RooUnfoldBayes    unfoldFJet_100 (&responseFJet, hMeasFJet_MC, 1000);    // OR
    RooUnfoldBayes    unfoldFJet_100 (&responseFJet, hMeasFJet_MC_test, 1000);    // OR
  TH1D* hRecoFJet_100 = (TH1D*) unfoldFJet_100.Hreco();
  hRecoFJet_100->SetTitle("FJetUnfold_100");
  
  //RooUnfoldBayes    unfoldCJet_100 (&responseCJet, hMeasCJet_MC, 1000);    // OR
    RooUnfoldBayes    unfoldCJet_100 (&responseCJet, hMeasCJet_MC_test, 1000);    // OR
  TH1D* hRecoCJet_100 = (TH1D*) unfoldCJet_100.Hreco();
  hRecoCJet_100->SetTitle("CJetUnfold_100");
    
  hRecoFJet_100->Sumw2();
  hRecoCJet_100->Sumw2();
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hRecoFJet_100->SetBinContent(iBinX+1,hRecoFJet_100->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hRecoCJet_100->SetBinContent(iBinX+1,hRecoCJet_100->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));    
  }


  
  
  cout << "===================================== DRAW ====================================" << endl;
  
  TCanvas* ccFJet = new TCanvas("ccFJet","ccFJet",700,700);  
 // ccFJet->Divide(1,2);
 // ccFJet->cd(1);

 // hTrueFJet_MC->SetLineColor();
  hTrueFJet_MC_test->SetStats(0); 
  hTrueFJet_MC_test->GetXaxis()->SetRangeUser(35.,149.);
  hTrueFJet_MC_test->GetXaxis()->SetTitle("p_{T}");
  hTrueFJet_MC_test->SetMarkerStyle(22);
  hTrueFJet_MC_test->DrawCopy();
  
  hRecoFJet_5->SetLineColor(kBlue);
  hRecoFJet_5->SetMarkerColor(kBlue);
  hRecoFJet_5->SetMarkerStyle(20);
  hRecoFJet_5->SetMarkerSize(1);
  hRecoFJet_5->DrawCopy("SAME");

  hRecoFJet_10->SetLineColor(kRed);
  hRecoFJet_10->SetMarkerColor(kRed);
  hRecoFJet_10->SetMarkerStyle(20);
  hRecoFJet_10->SetMarkerSize(1);
  hRecoFJet_10->DrawCopy("SAME");
  
  hRecoFJet_15->SetLineColor(kViolet);
  hRecoFJet_15->SetMarkerColor(kViolet);
  hRecoFJet_15->SetMarkerStyle(20);
  hRecoFJet_15->SetMarkerSize(1);
  hRecoFJet_15->DrawCopy("SAME");
  
  hRecoFJet_20->SetLineColor(kGreen);
  hRecoFJet_20->SetMarkerColor(kGreen);
  hRecoFJet_20->SetMarkerStyle(20);
  hRecoFJet_20->SetMarkerSize(1);
  hRecoFJet_20->DrawCopy("SAME");
  
  //hRecoFJet_25->SetLineColor(kBrown);
  //hRecoFJet_25->SetMarkerColor(kBrown);
  /*hRecoFJet_25->SetMarkerColor(kRed);
  hRecoFJet_25->SetMarkerStyle(20);
  hRecoFJet_25->SetMarkerSize(1);
  hRecoFJet_25->DrawCopy("SAME");*/
  
  hRecoFJet_50->SetMarkerColor(kRed);
  hRecoFJet_50->SetMarkerStyle(20);
  hRecoFJet_50->SetMarkerSize(1);
  hRecoFJet_50->DrawCopy("SAME");
  
  
//   hTrueFJet->SetLineColor(kGreen);
//   hTrueFJet->DrawCopy("SAME");
  gPad->BuildLegend();
  ccFJet->SetLogy();
 // gPad->SetGrid();
//  ccFJet->cd(2);
//  hResponseMatrixFJet->GetXaxis()->SetTitle("p_{T} Reco");
//  hResponseMatrixFJet->GetYaxis()->SetTitle("p_{T} Had");
//  hResponseMatrixFJet->DrawCopy("colz");
//  gPad->SetGrid();
  
  
  TCanvas* ccCJet = new TCanvas("ccCJet","ccCJet",700,700);  
  //ccCJet->Divide(1,2);
  //ccCJet->cd(1);
 
 // hTrueCJet_MC->SetLineColor();
  hTrueCJet_MC_test->SetStats(0); 
  hTrueCJet_MC_test->GetXaxis()->SetRangeUser(35.,149.);
  hTrueCJet_MC_test->GetXaxis()->SetTitle("p_{T}");
  hTrueCJet_MC_test->SetMarkerStyle(22);
  hTrueCJet_MC_test->DrawCopy();
    
  hRecoCJet_5->SetLineColor(kBlue);
  hRecoCJet_5->SetMarkerColor(kBlue);
  hRecoCJet_5->SetMarkerStyle(20);
  hRecoCJet_5->SetMarkerSize(1);
  hRecoCJet_5->DrawCopy("SAME");

  hRecoCJet_10->SetLineColor(kRed);
  hRecoCJet_10->SetMarkerColor(kRed);
  hRecoCJet_10->SetMarkerStyle(20);
  hRecoCJet_10->SetMarkerSize(1);
  hRecoCJet_10->DrawCopy("SAME");
  
  hRecoCJet_15->SetLineColor(kViolet);
  hRecoCJet_15->SetMarkerColor(kViolet);
  hRecoCJet_15->SetMarkerStyle(20);
  hRecoCJet_15->SetMarkerSize(1);
  hRecoCJet_15->DrawCopy("SAME");
  
  hRecoCJet_20->SetLineColor(kGreen);
  hRecoCJet_20->SetMarkerColor(kGreen);
  hRecoCJet_20->SetMarkerStyle(20);
  hRecoCJet_20->SetMarkerSize(1);
  hRecoCJet_20->DrawCopy("SAME");
  
  //hRecoCJet_25->SetLineColor(kBrown);
  //hRecoCJet_25->SetMarkerColor(Brown);
 /* hRecoCJet_25->SetMarkerColor(kRed);
  hRecoCJet_25->SetMarkerStyle(20);
  hRecoCJet_25->SetMarkerSize(1);
  hRecoCJet_25->DrawCopy("SAME");*/
  
  hRecoCJet_50->SetMarkerColor(kRed);
  hRecoCJet_50->SetMarkerStyle(20);
  hRecoCJet_50->SetMarkerSize(1);
  hRecoCJet_50->DrawCopy("SAME");
  gPad->BuildLegend();
  ccCJet->SetLogy();
 // gPad->SetGrid();
//  ccCJet->cd(2);
//  hResponseMatrixCJet->GetXaxis()->SetTitle("p_{T} Reco");
//  hResponseMatrixCJet->GetYaxis()->SetTitle("p_{T} Had");
//  hResponseMatrixCJet->DrawCopy("colz");
//  gPad->SetGrid();
 
 ///Draw single bin Response
 /*
  Float_t lowEdge2[8] = {0,10,25,50,100,200,500,1000};	//mi sto creando un istogramma con bin di grandezza fissata da me che poi passo alla matrice di risposta che lo usa
  int NBIN2 = 7;
  TH1F myH2("myH2","myH2",NBIN2,lowEdge2);
  
  //Bin 35
  TCanvas* ccBinResponse = new TCanvas("ccBinResponse","ccBinResponse",700,700);  
 
  TH1D* binResponse35 = new TH1D ("binResponse35", "binResponse35", NBIN2, lowEdge2);
  binResponse35->SetBinContent(1, hRecoCJet_5->GetBinContent(2));
  binResponse35->SetBinContent(2, hRecoCJet_10->GetBinContent(2));
  binResponse35->SetBinContent(3, hRecoCJet_15->GetBinContent(2));
  binResponse35->SetBinContent(4, hRecoCJet_20->GetBinContent(2));
  binResponse35->SetBinContent(5, hRecoCJet_25->GetBinContent(2));
  binResponse35->SetBinContent(6, hRecoCJet_50->GetBinContent(2));
  binResponse35->SetBinContent(7, hRecoCJet_100->GetBinContent(2));
  
  double val = hTrueCJet_MC->GetBinContent(2);
  std::cout<<"val = "<< val <<endl;
  
  binResponse35->GetYaxis()->SetRangeUser( hRecoCJet_5->GetBinContent(2), val*1.005);
  binResponse35->GetYaxis()->SetTitle("Events number");
  binResponse35->GetXaxis()->SetTitle("Unfolding iterations");
  binResponse35->SetStats(0);
  binResponse35->DrawCopy();
  
  
  TLine* lin35 = new TLine (0., val, 1000., val);
     
  lin35->SetLineWidth(1);
  lin35->SetLineColor(kRed);
  lin35->Draw();
  
  //Bin 45
  TCanvas* ccBinResponse45 = new TCanvas("ccBinResponse45","ccBinResponse45",700,700);  
 
  TH1D* binResponse45 = new TH1D ("binResponse45", "binResponse45", NBIN2, lowEdge2);
  binResponse45->SetBinContent(1, hRecoCJet_5->GetBinContent(3));
  binResponse45->SetBinContent(2, hRecoCJet_10->GetBinContent(3));
  binResponse45->SetBinContent(3, hRecoCJet_15->GetBinContent(3));
  binResponse45->SetBinContent(4, hRecoCJet_20->GetBinContent(3));
  binResponse45->SetBinContent(5, hRecoCJet_25->GetBinContent(3));
  binResponse45->SetBinContent(6, hRecoCJet_50->GetBinContent(3));
  binResponse45->SetBinContent(7, hRecoCJet_100->GetBinContent(3));
  
  double val = hTrueCJet_MC->GetBinContent(3);
  std::cout<<"val = "<< val <<endl;
  
  binResponse45->GetYaxis()->SetRangeUser( hRecoCJet_5->GetBinContent(3), val*1.005);
  binResponse45->GetYaxis()->SetTitle("Events number");
  binResponse45->GetXaxis()->SetTitle("Unfolding iterations");
  binResponse45->SetStats(0);
  binResponse45->DrawCopy();
  
  
  TLine* lin45 = new TLine (0., val, 1000., val);
     
  lin45->SetLineWidth(1);
  lin45->SetLineColor(kRed);
  lin45->Draw();
  
    //Bin 57
  TCanvas* ccBinResponse57 = new TCanvas("ccBinResponse57","ccBinResponse57",700,700);  
 
  TH1D* binResponse57 = new TH1D ("binResponse57", "binResponse57", NBIN2, lowEdge2);
  binResponse57->SetBinContent(1, hRecoCJet_5->GetBinContent(4));
  binResponse57->SetBinContent(2, hRecoCJet_10->GetBinContent(4));
  binResponse57->SetBinContent(3, hRecoCJet_15->GetBinContent(4));
  binResponse57->SetBinContent(4, hRecoCJet_20->GetBinContent(4));
  binResponse57->SetBinContent(5, hRecoCJet_25->GetBinContent(4));
  binResponse57->SetBinContent(6, hRecoCJet_50->GetBinContent(4));
  binResponse57->SetBinContent(7, hRecoCJet_100->GetBinContent(4));
  
  double val = hTrueCJet_MC->GetBinContent(4);
  std::cout<<"val = "<< val <<endl;
  
  binResponse57->GetYaxis()->SetRangeUser( hRecoCJet_5->GetBinContent(4), val*1.005);
  binResponse57->GetYaxis()->SetTitle("Events number");
  binResponse57->GetXaxis()->SetTitle("Unfolding iterations");
  binResponse57->SetStats(0);
  binResponse57->DrawCopy();
  
  
  TLine* lin57 = new TLine (0., val, 1000., val);
     
  lin57->SetLineWidth(1);
  lin57->SetLineColor(kRed);
  lin57->Draw();
  
   //Bin 72
  TCanvas* ccBinResponse72 = new TCanvas("ccBinResponse72","ccBinResponse72",700,700);  
 
  TH1D* binResponse72 = new TH1D ("binResponse72", "binResponse72", NBIN2, lowEdge2);
  binResponse72->SetBinContent(1, hRecoCJet_5->GetBinContent(5));
  binResponse72->SetBinContent(2, hRecoCJet_10->GetBinContent(5));
  binResponse72->SetBinContent(3, hRecoCJet_15->GetBinContent(5));
  binResponse72->SetBinContent(4, hRecoCJet_20->GetBinContent(5));
  binResponse72->SetBinContent(5, hRecoCJet_25->GetBinContent(5));
  binResponse72->SetBinContent(6, hRecoCJet_50->GetBinContent(5));
  binResponse72->SetBinContent(7, hRecoCJet_100->GetBinContent(5));
  
  double val = hTrueCJet_MC->GetBinContent(5);
  std::cout<<"val = "<< val <<endl;
  
  binResponse72->GetYaxis()->SetRangeUser( hRecoCJet_5->GetBinContent(5), val*1.005);
  binResponse72->GetYaxis()->SetTitle("Events number");
  binResponse72->GetXaxis()->SetTitle("Unfolding iterations");
  binResponse72->SetStats(0);
  binResponse72->DrawCopy();
  
  
  TLine* lin72 = new TLine (0., val, 1000., val);
     
  lin72->SetLineWidth(1);
  lin72->SetLineColor(kRed);
  lin72->Draw();

  //Bin 90
  
  TCanvas* ccBinResponse90 = new TCanvas("ccBinResponse90","ccBinResponse90",700,700);  
 
  TH1D* binResponse90 = new TH1D ("binResponse90", "binResponse90", NBIN2, lowEdge2);
  binResponse90->SetBinContent(1, hRecoCJet_5->GetBinContent(6));
  binResponse90->SetBinContent(2, hRecoCJet_10->GetBinContent(6));
  binResponse90->SetBinContent(3, hRecoCJet_15->GetBinContent(6));
  binResponse90->SetBinContent(4, hRecoCJet_20->GetBinContent(6));
  binResponse90->SetBinContent(5, hRecoCJet_25->GetBinContent(6));
  binResponse90->SetBinContent(6, hRecoCJet_50->GetBinContent(6));
  binResponse90->SetBinContent(7, hRecoCJet_100->GetBinContent(6));
  
  double val2 = hTrueCJet_MC->GetBinContent(6);
  std::cout<<"val2 = "<< val2 <<endl;
  
  binResponse90->GetYaxis()->SetRangeUser( hRecoCJet_5->GetBinContent(6), val2*1.005);
  binResponse90->GetYaxis()->SetTitle("Events number");
  binResponse90->GetXaxis()->SetTitle("Unfolding iterations");
  binResponse90->SetStats(0);
  binResponse90->DrawCopy();
  
  
  TLine* lin90 = new TLine (0., val2, 1000., val2);
     
  lin90->SetLineWidth(1);
  lin90->SetLineColor(kRed);
  lin90->Draw();
  
   //Bin 120
  TCanvas* ccBinResponse120 = new TCanvas("ccBinResponse120","ccBinResponse120",700,700);  
 
  TH1D* binResponse120 = new TH1D ("binResponse120", "binResponse120", NBIN2, lowEdge2);
  binResponse120->SetBinContent(1, hRecoCJet_5->GetBinContent(7));
  binResponse120->SetBinContent(2, hRecoCJet_10->GetBinContent(7));
  binResponse120->SetBinContent(3, hRecoCJet_15->GetBinContent(7));
  binResponse120->SetBinContent(4, hRecoCJet_20->GetBinContent(7));
  binResponse120->SetBinContent(5, hRecoCJet_25->GetBinContent(7));
  binResponse120->SetBinContent(6, hRecoCJet_50->GetBinContent(7));
  binResponse120->SetBinContent(7, hRecoCJet_100->GetBinContent(7));
  
  double val = hTrueCJet_MC->GetBinContent(7);
  std::cout<<"val = "<< val <<endl;
  
  binResponse120->GetYaxis()->SetRangeUser( hRecoCJet_5->GetBinContent(7), val*1.005);
  binResponse120->GetYaxis()->SetTitle("Events number");
  binResponse120->GetXaxis()->SetTitle("Unfolding iterations");
  binResponse120->SetStats(0);
  binResponse120->DrawCopy();
  
  ccBinResponse120->SetLogx(2);
  
  TLine* lin120 = new TLine (0., val, 1000., val);
     
  lin120->SetLineWidth(1);
  lin120->SetLineColor(kRed);
  lin120->Draw();*/
 
 ///Draw Graph for single Bin Response
  
   const Int_t n = 8;
   Double_t x[n], y[n];
      
   x[0] = 5;
   x[1] = 10;
   x[2] = 25;
   x[3] = 50;
   x[4] = 100;
   x[5] = 200;
   x[6] = 500;
   x[7] = 1000;
   
   
   //for Bin 1
   int iBin = 1;
   TCanvas* cgBinResponse35 = new TCanvas("cgBinResponse35","cgBinResponse35", 600, 600);  
    double True = hTrueCJet_MC_test->GetBinContent(iBin);
   
   y[0] = hRecoCJet_1->GetBinContent(iBin)/True;
   y[1] = hRecoCJet_5->GetBinContent(iBin)/True;
   y[2] = hRecoCJet_10->GetBinContent(iBin)/True;
   y[3] = hRecoCJet_15->GetBinContent(iBin)/True;
   y[4] = hRecoCJet_20->GetBinContent(iBin)/True;
   y[5] = hRecoCJet_25->GetBinContent(iBin)/True;
   y[6] = hRecoCJet_50->GetBinContent(iBin)/True;
   y[7] = hRecoCJet_100->GetBinContent(iBin)/True;
   
   std::cout<<"Unfolding - Bin number =  " << iBin <<std::endl;
   std::cout<<"Err[%] - Bin-to-Bin   = " << fabs(hRecoCJet_Bin->GetBinContent(iBin)-True)/True*100 <<std::endl;
   std::cout<<"Err[%] - Bayes (10)   = " << fabs(hRecoCJet_5->GetBinContent(iBin)-True)/True*100 <<std::endl;
   std::cout<<"Err[%] - Bayes (1000) = " << fabs(hRecoCJet_100->GetBinContent(iBin)-True)/True*100 <<std::endl;
   
   
   gr7 = new TGraph(n,x,y);
   gr7->SetLineColor(2);
   gr7->SetLineWidth(2);
   gr7->SetFillColor(0);
   //gr7->SetMarkerColor(3);
   gr7->SetMarkerStyle(21);
   gr7->SetTitle("Bin 35");
   gr7->GetXaxis()->SetTitle("Iterations number");
   gr7->GetYaxis()->SetTitle("Bin Entries");
   gr7->GetYaxis()->SetRangeUser(0.6,1.4);
   gr7->Draw("ALP");
  
   //double val = hTrueCJet_MC->GetBinContent(iBin);
   double val = 1;	//Valore atteso da HERWIG!
   double val2 = hRecoCJet_Bin->GetBinContent(iBin)/hTrueCJet_MC_test->GetBinContent(iBin); //Valore del BinByBin
   //double valP = hTrueCJet_MC->GetBinContent(iBin);
   TLine* lin35 = new TLine (0., val, 1100., val);
   lin35->SetLineWidth(1);
   lin35->SetLineColor(kBlue);
   lin35->Draw();
   
   TLine* lin35Bin = new TLine (0., val2, 1100., val2);
   lin35Bin->SetLineWidth(1);
   //lin120->SetLineColor(kRed);
   lin35Bin->Draw();
   
      leg = new TLegend(0.6,0.7,0.9,0.9,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   
   leg->AddEntry(gr7,"Bayesian", "lpf");
   leg->AddEntry(lin35,"True", "lpf");
   leg->AddEntry(lin35Bin,"Bin-by-bin", "lpf");
   
   leg->Draw(); 
  
   
   //for first Bin
   iBin = 2;
   TCanvas* cgBinResponse45 = new TCanvas("cgBinResponse45","cgBinResponse45", 600, 600);  
   double True = hTrueCJet_MC_test->GetBinContent(iBin);
   
   y[0] = hRecoCJet_1->GetBinContent(iBin)/True;
   y[1] = hRecoCJet_5->GetBinContent(iBin)/True;
   y[2] = hRecoCJet_10->GetBinContent(iBin)/True;
   y[3] = hRecoCJet_15->GetBinContent(iBin)/True;
   y[4] = hRecoCJet_20->GetBinContent(iBin)/True;
   y[5] = hRecoCJet_25->GetBinContent(iBin)/True;
   y[6] = hRecoCJet_50->GetBinContent(iBin)/True;
   y[7] = hRecoCJet_100->GetBinContent(iBin)/True;
   
   std::cout<<"Unfolding - Bin number =  " << iBin <<std::endl;
   std::cout<<"Err[%] - Bin-to-Bin   = " << fabs(hRecoCJet_Bin->GetBinContent(iBin)-True)/True*100 <<std::endl;
   std::cout<<"Err[%] - Bayes (10)   = " << fabs(hRecoCJet_5->GetBinContent(iBin)-True)/True*100 <<std::endl;
   std::cout<<"Err[%] - Bayes (1000) = " << fabs(hRecoCJet_100->GetBinContent(iBin)-True)/True*100 <<std::endl;
   
   gr7 = new TGraph(n,x,y);
   gr7->SetLineColor(2);
   gr7->SetLineWidth(2);
   gr7->SetFillColor(0);
   //gr7->SetMarkerColor(3);
   gr7->SetMarkerStyle(21);
   gr7->SetTitle("Bin 45");
   gr7->GetXaxis()->SetTitle("Iterations number");
   gr7->GetYaxis()->SetTitle("Bin Entries");
   gr7->GetYaxis()->SetRangeUser(0.6,1.4);
   gr7->Draw("ALP");
  
   //double val = hTrueCJet_MC->GetBinContent(iBin);
   double val = 1;	//Valore atteso da HERWIG!
   double val2 = hRecoCJet_Bin->GetBinContent(iBin)/hTrueCJet_MC_test->GetBinContent(iBin); //Valore del BinByBin
   //double valP = hTrueCJet_MC->GetBinContent(iBin);
   TLine* lin45 = new TLine (0., val, 1100., val);
   lin45->SetLineWidth(1);
   lin45->SetLineColor(kBlue);
   lin45->Draw();
   TLine* lin45Bin = new TLine (0., val2, 1100., val2);
   lin45Bin->SetLineWidth(1);
   lin45Bin->Draw();
   
      leg = new TLegend(0.6,0.7,0.9,0.9,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   
   leg->AddEntry(gr7,"Bayesian", "lpf");
   leg->AddEntry(lin45,"True", "lpf");
   leg->AddEntry(lin45Bin,"Bin-by-bin", "lpf");
   
   leg->Draw(); 
   
   //for first Bin
   iBin = 3;
   TCanvas* cgBinResponse57 = new TCanvas("cgBinResponse57","cgBinResponse57", 600, 600);  
   double True = hTrueCJet_MC_test->GetBinContent(iBin);
   
   y[0] = hRecoCJet_1->GetBinContent(iBin)/True;
   y[1] = hRecoCJet_5->GetBinContent(iBin)/True;
   y[2] = hRecoCJet_10->GetBinContent(iBin)/True;
   y[3] = hRecoCJet_15->GetBinContent(iBin)/True;
   y[4] = hRecoCJet_20->GetBinContent(iBin)/True;
   y[5] = hRecoCJet_25->GetBinContent(iBin)/True;
   y[6] = hRecoCJet_50->GetBinContent(iBin)/True;
   y[7] = hRecoCJet_100->GetBinContent(iBin)/True;
   
   std::cout<<"Unfolding - Bin number =  " << iBin <<std::endl;
   std::cout<<"Err[%] - Bin-to-Bin   = " << fabs(hRecoCJet_Bin->GetBinContent(iBin)-True)/True*100 <<std::endl;
   std::cout<<"Err[%] - Bayes (10)   = " << fabs(hRecoCJet_5->GetBinContent(iBin)-True)/True*100 <<std::endl;
   std::cout<<"Err[%] - Bayes (1000) = " << fabs(hRecoCJet_100->GetBinContent(iBin)-True)/True*100 <<std::endl;
   
   gr7 = new TGraph(n,x,y);
   gr7->SetLineColor(2);
   gr7->SetLineWidth(2);
   gr7->SetFillColor(0);
   //gr7->SetMarkerColor(3);
   gr7->SetMarkerStyle(21);
   gr7->SetTitle("Bin 57");
   gr7->GetXaxis()->SetTitle("Iterations number");
   gr7->GetYaxis()->SetTitle("Bin Entries");
   gr7->GetYaxis()->SetRangeUser(0.6,1.4);
   gr7->Draw("ALP");
  
    //double val = hTrueCJet_MC->GetBinContent(iBin);
   double val = 1;	//Valore atteso da HERWIG!
   double val2 = hRecoCJet_Bin->GetBinContent(iBin)/hTrueCJet_MC_test->GetBinContent(iBin); //Valore del BinByBin
   //double valP = hTrueCJet_MC->GetBinContent(iBin);
   TLine* lin57 = new TLine (0., val, 1100., val);
   lin57->SetLineWidth(1);
   lin57->SetLineColor(kBlue);
   lin57->Draw();
   TLine* lin57Bin = new TLine (0., val2, 1100., val2);
   lin57Bin->SetLineWidth(1);
   //lin120->SetLineColor(kRed);
   lin57Bin->Draw();
      
   leg = new TLegend(0.6,0.7,0.9,0.9,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   
   leg->AddEntry(gr7,"Bayesian", "lpf");
   leg->AddEntry(lin57,"True", "lpf");
   leg->AddEntry(lin57Bin,"Bin-by-bin", "lpf");
   
   leg->Draw(); 
   
   //for first Bin
   iBin = 4;
   TCanvas* cgBinResponse72 = new TCanvas("cgBinResponse72","cgBinResponse72", 600, 600);  
   double True = hTrueCJet_MC_test->GetBinContent(iBin);
   
   y[0] = hRecoCJet_1->GetBinContent(iBin)/True;
   y[1] = hRecoCJet_5->GetBinContent(iBin)/True;
   y[2] = hRecoCJet_10->GetBinContent(iBin)/True;
   y[3] = hRecoCJet_15->GetBinContent(iBin)/True;
   y[4] = hRecoCJet_20->GetBinContent(iBin)/True;
   y[5] = hRecoCJet_25->GetBinContent(iBin)/True;
   y[6] = hRecoCJet_50->GetBinContent(iBin)/True;
   y[7] = hRecoCJet_100->GetBinContent(iBin)/True;
   
   std::cout<<"Unfolding - Bin number =  " << iBin <<std::endl;
   std::cout<<"Err[%] - Bin-to-Bin   = " << fabs(hRecoCJet_Bin->GetBinContent(iBin)-True)/True*100 <<std::endl;
   std::cout<<"Err[%] - Bayes (10)   = " << fabs(hRecoCJet_5->GetBinContent(iBin)-True)/True*100 <<std::endl;
   std::cout<<"Err[%] - Bayes (1000) = " << fabs(hRecoCJet_100->GetBinContent(iBin)-True)/True*100 <<std::endl;
   
   
   gr7 = new TGraph(n,x,y);
   gr7->SetLineColor(2);
   gr7->SetLineWidth(2);
   gr7->SetFillColor(0);
   //gr7->SetMarkerColor(3);
   gr7->SetMarkerStyle(21);
   gr7->SetTitle("Bin 72");
   gr7->GetXaxis()->SetTitle("Iterations number");
   gr7->GetYaxis()->SetTitle("Bin Entries");
   gr7->GetYaxis()->SetRangeUser(0.6,1.4);
   gr7->Draw("ALP");
  
  //double val = hTrueCJet_MC->GetBinContent(iBin);
   double val = 1;	//Valore atteso da HERWIG!
   double val2 = hRecoCJet_Bin->GetBinContent(iBin)/hTrueCJet_MC_test->GetBinContent(iBin); //Valore del BinByBin
   //double valP = hTrueCJet_MC->GetBinContent(iBin);
   TLine* lin72 = new TLine (0., val, 1100., val);
   lin72->SetLineWidth(1);
   lin72->SetLineColor(kBlue);
   lin72->Draw();
   TLine* lin72Bin = new TLine (0., val2, 1100., val2);
   lin72Bin->SetLineWidth(1);
   //lin120->SetLineColor(kRed);
   lin72Bin->Draw();
   
      leg = new TLegend(0.6,0.7,0.9,0.9,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   
   leg->AddEntry(gr7,"Bayesian", "lpf");
   leg->AddEntry(lin72,"True", "lpf");
   leg->AddEntry(lin72Bin,"Bin-by-bin", "lpf");
   
   leg->Draw(); 
   
   
   //for first Bin
   iBin = 5;
   TCanvas* cgBinResponse90 = new TCanvas("cgBinResponse90","cgBinResponse90", 600, 600);  
   double True = hTrueCJet_MC_test->GetBinContent(iBin);
   
   y[0] = hRecoCJet_1->GetBinContent(iBin)/True;
   y[1] = hRecoCJet_5->GetBinContent(iBin)/True;
   y[2] = hRecoCJet_10->GetBinContent(iBin)/True;
   y[3] = hRecoCJet_15->GetBinContent(iBin)/True;
   y[4] = hRecoCJet_20->GetBinContent(iBin)/True;
   y[5] = hRecoCJet_25->GetBinContent(iBin)/True;
   y[6] = hRecoCJet_50->GetBinContent(iBin)/True;
   y[7] = hRecoCJet_100->GetBinContent(iBin)/True;
   
   std::cout<<"Unfolding - Bin number =  " << iBin <<std::endl;
   std::cout<<"Err[%] - Bin-to-Bin   = " << fabs(hRecoCJet_Bin->GetBinContent(iBin)-True)/True*100 <<std::endl;
   std::cout<<"Err[%] - Bayes (10)   = " << fabs(hRecoCJet_5->GetBinContent(iBin)-True)/True*100 <<std::endl;
   std::cout<<"Err[%] - Bayes (1000) = " << fabs(hRecoCJet_100->GetBinContent(iBin)-True)/True*100 <<std::endl;
   
   
   gr7 = new TGraph(n,x,y);
   gr7->SetLineColor(2);
   gr7->SetLineWidth(2);
   gr7->SetFillColor(0);
   //gr7->SetMarkerColor(3);
   gr7->SetMarkerStyle(21);
   
   gr7->SetTitle("Bin 90");
   gr7->GetXaxis()->SetTitle("Iterations number");
   gr7->GetYaxis()->SetTitle("Bin Entries");
   gr7->GetYaxis()->SetRangeUser(0.6,1.4);
   gr7->Draw("ALP");
   //double val = hTrueCJet_MC->GetBinContent(iBin);
   double val = 1;	//Valore atteso da HERWIG!
   double val2 = hRecoCJet_Bin->GetBinContent(iBin)/hTrueCJet_MC_test->GetBinContent(iBin); //Valore del BinByBin
   //double valP = hTrueCJet_MC->GetBinContent(iBin);
   TLine* lin90 = new TLine (0., val, 1100., val);
   lin90->SetLineWidth(1);
   lin90->SetLineColor(kBlue);
   //lin120->SetLineColor(kRed);
   lin90->Draw();
   TLine* lin90Bin = new TLine (0., val2, 1100., val2);
   lin90Bin->SetLineWidth(1);
   
   lin90Bin->Draw();
   
      leg = new TLegend(0.6,0.7,0.9,0.9,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   
   leg->AddEntry(gr7,"Bayesian", "lpf");
   leg->AddEntry(lin90,"True", "lpf");
   leg->AddEntry(lin90Bin,"Bin-by-bin", "lpf");
   
   leg->Draw(); 
   
   
   
   //for bin 6
   iBin = 6;
   TCanvas* cgBinResponse120 = new TCanvas("cgBinResponse120","cgBinResponse120", 600, 600);  
   double True = hTrueCJet_MC_test->GetBinContent(iBin);
   
   y[0] = hRecoCJet_1->GetBinContent(iBin)/True;
   y[1] = hRecoCJet_5->GetBinContent(iBin)/True;
   y[2] = hRecoCJet_10->GetBinContent(iBin)/True;
   y[3] = hRecoCJet_15->GetBinContent(iBin)/True;
   y[4] = hRecoCJet_20->GetBinContent(iBin)/True;
   y[5] = hRecoCJet_25->GetBinContent(iBin)/True;
   y[6] = hRecoCJet_50->GetBinContent(iBin)/True;
   y[7] = hRecoCJet_100->GetBinContent(iBin)/True;
   
   std::cout<<"Unfolding - Bin number =  " << iBin <<std::endl;
   std::cout<<"Err[%] - Bin-to-Bin   = " << fabs(hRecoCJet_Bin->GetBinContent(iBin)-True)/True*100 <<std::endl;
   std::cout<<"Err[%] - Bayes (10)   = " << fabs(hRecoCJet_5->GetBinContent(iBin)-True)/True*100 <<std::endl;
   std::cout<<"Err[%] - Bayes (1000) = " << fabs(hRecoCJet_100->GetBinContent(iBin)-True)/True*100 <<std::endl;
   
   
   gr7 = new TGraph(n,x,y);
   gr7->SetLineColor(2);
   gr7->SetLineWidth(2);
   gr7->SetFillColor(0);
   //gr7->SetMarkerColor(3);
   gr7->SetMarkerStyle(21);
   
   gr7->SetTitle("Bin 120");
   gr7->GetXaxis()->SetTitle("Iterations number");
   gr7->GetYaxis()->SetTitle("Bin Entries");
   gr7->GetYaxis()->SetRangeUser(0.6,1.4);
   gr7->Draw("ALP");
  
   //double val = hTrueCJet_MC->GetBinContent(iBin);
   double val = 1;	//Valore atteso da HERWIG!
   double val2 = hRecoCJet_Bin->GetBinContent(iBin)/hTrueCJet_MC_test->GetBinContent(iBin); //Valore del BinByBin
   //double valP = hTrueCJet_MC->GetBinContent(iBin);
   TLine* lin120 = new TLine (0., val, 1100., val);
   lin120->SetLineWidth(1);
   lin120->SetLineColor(kBlue);
   lin120->Draw();
   
   TLine* lin120Bin = new TLine (0., val2, 1100., val2);
   lin120Bin->SetLineWidth(1);
   //lin120->SetLineColor(kRed);
   lin120Bin->Draw();
   
   leg = new TLegend(0.6,0.7,0.9,0.9,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   
   leg->AddEntry(gr7,"Bayesian", "lpf");
   leg->AddEntry(lin120,"True", "lpf");
   leg->AddEntry(lin120Bin,"Bin-by-bin", "lpf");
   
   leg->Draw();    
   /*
   //for bin 6
   iBin = 7;
   TCanvas* cgBinResponse72 = new TCanvas("cgBinResponse72","cgBinResponse120");  
   
   y[0] = hRecoCJet_5->GetBinContent(iBin);
   y[1] = hRecoCJet_10->GetBinContent(iBin);
   y[2] = hRecoCJet_15->GetBinContent(iBin);
   y[3] = hRecoCJet_20->GetBinContent(iBin);
   y[4] = hRecoCJet_25->GetBinContent(iBin);
   y[5] = hRecoCJet_50->GetBinContent(iBin);
   y[6] = hRecoCJet_100->GetBinContent(iBin);
   
   gr7 = new TGraph(n,x,y);
   gr7->SetLineColor(2);
   gr7->SetLineWidth(2);
   gr7->SetMarkerColor(3);
   gr7->SetMarkerStyle(21);
   //gr->SetTitle("Unfolding iterations");
   gr7->GetXaxis()->SetTitle("Iterations number");
   gr7->GetYaxis()->SetTitle("Bin Entries");
   gr7->Draw("ACP");
  
   double val = hTrueCJet_MC->GetBinContent(iBin);
   TLine* lin72 = new TLine (0., val, 1200., val);
   lin72->SetLineWidth(1);
   //lin120->SetLineColor(kRed);
   lin72->Draw();
   
   */
}

#ifndef __CINT__
int main () { Bayesian_iteration(); return 0; }  // Main program when run stand-alone
#endif
