//=====================================================================-*-C++-*-
// 
//      ___|                                 | 
//     |       _ \  __ \    _ \   __|  _` |  | 
//     |   |   __/  |   |   __/  |    (   |  | 
//    \____| \___| _|  _| \___| _|   \__,_| _| 
//                                             
//   
//     |   |          _|         |      | _)               
//     |   |  __ \   |     _ \   |   _` |  |  __ \    _` | 
//     |   |  |   |  __|  (   |  |  (   |  |  |   |  (   | 
//    \___/  _|  _| _|   \___/  _| \__,_| _| _|  _| \__, | 
//                                                  |___/  
//   
//   
//   
//
// Description:
//      multiusage program for unfolding. Based on RooUnfold.
//
//==============================================================================

#include <iostream>
#include "TRandom.h"


#include "RooUnfoldResponse.h"
#include "RooUnfoldBayes.h"
#include "RooUnfoldSvd.h"
#include "RooUnfoldBinByBin.h"
#include "RooUnfoldDagostini.h"
#include "RooUnfoldTUnfold.h"

// #include "TDRStyle.h"

#include <math.h>

///---- ConfigParser includes ----
#include "ConfigParser.h"
#include "ntpleUtils.h"

///---- ROOT includes ----
#include "TH1D.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TPad.h"



//==============================================================================
// Unfolding for C-F jet analysis
//==============================================================================

int main(int argc, char** argv)
{
//   TDRStyle();

 //Check if all nedeed arguments to parse are there
 if(argc != 2)
 {
  std::cerr << ">>>>> GeneralUnfold.cpp::usage: " << argv[0] << " configFileName" << std::endl ;
  return 1;
 }
 
 // Parse the config file --> legge le info del file .cfg
 parseConfigFile (argv[1]) ;

 bool  debug = false; 
 try {
  debug = gConfigParser -> readBoolOption("Input::debug");
 }
 catch (char const* exceptionString){
  std::cerr << " exception = " << exceptionString << std::endl;
 }
 std::cout << ">>>>> input::debug  " << debug  << std::endl;  

 //==== output file ====
 std::string OutFileName    = gConfigParser -> readStringOption("Output::outFileName");
 std::cout << ">>>>> Output::outFileName  " << OutFileName  << std::endl;  
 
 
  
  //==== input file ====
  double  normalizationMCLowA;
  double normalizationMCHighA;
  double  normalizationMCLowB;
  double normalizationMCHighB;

  std::string  inputFileMCLowA;
  std::string inputFileMCHighA;
  std::string  inputFileMCLowB;
  std::string inputFileMCHighB;
  
  normalizationMCLowA  = gConfigParser -> readDoubleOption("Input::normalizationMCLowA");
  normalizationMCHighA = gConfigParser -> readDoubleOption("Input::normalizationMCHighA");
  normalizationMCLowB  = gConfigParser -> readDoubleOption("Input::normalizationMCLowB");
  normalizationMCHighB = gConfigParser -> readDoubleOption("Input::normalizationMCHighB");     

  try {
   inputFileMCLowA = gConfigParser -> readStringOption("Input::inputFileMCLowA");
  }
  catch (char const* exceptionString){
   std::cerr << " exception = " << exceptionString << std::endl;
   return 1;
  }
 try {
   inputFileMCHighA = gConfigParser -> readStringOption("Input::inputFileMCHighA");
  }
  catch (char const* exceptionString){
   std::cerr << " exception = " << exceptionString << std::endl;
   return 1;
  }
  try {
   inputFileMCLowB = gConfigParser -> readStringOption("Input::inputFileMCLowB");
  }
  catch (char const* exceptionString){
   std::cerr << " exception = " << exceptionString << std::endl;
   return 1;
  }
 try {
   inputFileMCHighB = gConfigParser -> readStringOption("Input::inputFileMCHighB");
  }
  catch (char const* exceptionString){
   std::cerr << " exception = " << exceptionString << std::endl;
   return 1;
  }

 //==== name ====
 std::string nameTypeFileA = gConfigParser -> readStringOption("Input::nameTypeFileA");
 std::string nameTypeFileB = gConfigParser -> readStringOption("Input::nameTypeFileB");

 //==== tree name ====
 std::string treeName = gConfigParser -> readStringOption("Input::treeName");

 //==== DATA file ====
 std::string inputFileDATA;
 try {
   inputFileDATA = gConfigParser -> readStringOption("Input::inputFileDATA");
  }
  catch (char const* exceptionString){
   std::cerr << " exception = " << exceptionString << std::endl;
   return 1;
  }

  double lumi = gConfigParser -> readDoubleOption("Input::lumi");

  //==== type of unfolding ====
  int ALGO = 3;
  try {
   ALGO = gConfigParser -> readIntOption("Options::ALGO");
  }
  catch (char const* exceptionString){
   std::cerr << " exception = " << exceptionString << std::endl;
   return 1;
  }
  //enum Algorithm {
  // 	kNone	           0
  //	kBayes	           1
  //	kSVD	           2
  //	kBinByBin	   3
  //	kTUnfold	   4
  //	kInvert	           5
  //	kDagostini	   6
  //};
  
  
  //==== BinEdges ====
  std::vector<double> v_lowEdge = gConfigParser -> readDoubleListOption("Options::EDGE");
  
  Float_t* lowEdge = new Float_t [ v_lowEdge.size() ];
  for (int iEdge = 0; iEdge < v_lowEdge.size(); iEdge++){
   lowEdge[iEdge] = static_cast<float> (v_lowEdge.at(iEdge));
  }
  int NBIN = v_lowEdge.size() - 1; ///~~~~ number of bins is number  of edges -1 !
  
 //==== Threshold for A / B ====
  double Threshold_G_FJet = gConfigParser -> readDoubleOption("Options::ThresholdFJet");
  double Threshold_G_CJet = gConfigParser -> readDoubleOption("Options::ThresholdCJet"); 
  
  
  
  TFile outFile(OutFileName.c_str(),"RECREATE");
  outFile.cd();

  
  TH1F myH("myH","myH",NBIN,lowEdge);
  TH2D* hResponseMatrixFJet_A = new TH2D ("hResponseMatrixFJet_A", (TString) Form("Response Matrix FJet %s", nameTypeFileA.c_str()),NBIN,lowEdge,NBIN,lowEdge);
  TH2D* hResponseMatrixCJet_A = new TH2D ("hResponseMatrixCJet_A", (TString) Form("Response Matrix CJet %s", nameTypeFileA.c_str()),NBIN,lowEdge,NBIN,lowEdge);

  TH2D* hResponseMatrixFJet_tempA = new TH2D ("hResponseMatrixFJet_tempA", "Response Matrix FJet",NBIN,lowEdge,NBIN,lowEdge);
  TH2D* hResponseMatrixCJet_tempA = new TH2D ("hResponseMatrixCJet_tempA", "Response Matrix CJet",NBIN,lowEdge,NBIN,lowEdge);

  TH2D* hResponseMatrixFJet_B = new TH2D ("hResponseMatrixFJet_B", (TString) Form("Response Matrix FJet %s", nameTypeFileB.c_str()),NBIN,lowEdge,NBIN,lowEdge);
  TH2D* hResponseMatrixCJet_B = new TH2D ("hResponseMatrixCJet_B", (TString) Form("Response Matrix CJet %s", nameTypeFileB.c_str()),NBIN,lowEdge,NBIN,lowEdge);

  TH2D* hResponseMatrixFJet_tempB = new TH2D ("hResponseMatrixFJet_tempB", "Response Matrix FJet",NBIN,lowEdge,NBIN,lowEdge);
  TH2D* hResponseMatrixCJet_tempB = new TH2D ("hResponseMatrixCJet_tempB", "Response Matrix CJet",NBIN,lowEdge,NBIN,lowEdge);

  
  std::cout << "==================================== TRAIN ====================================" << std::endl;
  
  RooUnfoldResponse responseFJet_A (&myH,&myH);
  RooUnfoldResponse responseCJet_A (&myH,&myH);

  RooUnfoldResponse responseFJet_B (&myH,&myH);
  RooUnfoldResponse responseCJet_B (&myH,&myH);
  
  TFile FileTrain1_A(inputFileMCLowA.c_str(),"READ");
  TFile FileTrain2_A(inputFileMCHighA.c_str(),"READ");

  TFile FileTrain1_B(inputFileMCLowB.c_str(),"READ");
  TFile FileTrain2_B(inputFileMCHighB.c_str(),"READ");
  
  TTree* TreeTrain1_A = (TTree*) FileTrain1_A.Get(treeName.c_str());
  TTree* TreeTrain2_A = (TTree*) FileTrain2_A.Get(treeName.c_str());
 
  TTree* TreeTrain1_B = (TTree*) FileTrain1_B.Get(treeName.c_str());
  TTree* TreeTrain2_B = (TTree*) FileTrain2_B.Get(treeName.c_str());
  
  Float_t G_FJet_Pt; //~~~~ had
  Float_t S_FJet_Pt; //~~~~ reco

  Float_t G_CJet_Pt; //~~~~ had
  Float_t S_CJet_Pt; //~~~~ reco
 
 TreeTrain1_A->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt);
 TreeTrain1_A->SetBranchAddress("S_FJet_Pt",&S_FJet_Pt);
 TreeTrain1_A->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt);
 TreeTrain1_A->SetBranchAddress("S_CJet_Pt",&S_CJet_Pt);

 TreeTrain2_A->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt);
 TreeTrain2_A->SetBranchAddress("S_FJet_Pt",&S_FJet_Pt);
 TreeTrain2_A->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt);
 TreeTrain2_A->SetBranchAddress("S_CJet_Pt",&S_CJet_Pt);
 
 TreeTrain1_B->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt);
 TreeTrain1_B->SetBranchAddress("S_FJet_Pt",&S_FJet_Pt);
 TreeTrain1_B->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt);
 TreeTrain1_B->SetBranchAddress("S_CJet_Pt",&S_CJet_Pt);

 TreeTrain2_B->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt);
 TreeTrain2_B->SetBranchAddress("S_FJet_Pt",&S_FJet_Pt);
 TreeTrain2_B->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt);
 TreeTrain2_B->SetBranchAddress("S_CJet_Pt",&S_CJet_Pt);


  std::cout << "=================================================================================" << std::endl;
  std::cout << "==================================== _A =====================================" << std::endl;

 for (Int_t iEvt= 0; iEvt<TreeTrain1_A->GetEntries(); iEvt++) {
    TreeTrain1_A->GetEntry(iEvt);
    
    if (S_FJet_Pt>0 && G_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0)  {
     if (G_FJet_Pt < Threshold_G_FJet) {
      hResponseMatrixFJet_tempA->Fill(S_FJet_Pt, G_FJet_Pt);
     }
     if (G_CJet_Pt < Threshold_G_CJet) {
      hResponseMatrixCJet_tempA->Fill(S_CJet_Pt, G_CJet_Pt);
     }
    }
//       response.Miss (xt);
  }
  
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   for (int iBinY = 0; iBinY<NBIN; iBinY++){
    hResponseMatrixCJet_tempA->SetBinError(iBinX+1,iBinY+1,hResponseMatrixCJet_tempA->GetBinError(iBinX+1,iBinY+1) * normalizationMCLowA);
    hResponseMatrixFJet_tempA->SetBinError(iBinX+1,iBinY+1,hResponseMatrixFJet_tempA->GetBinError(iBinX+1,iBinY+1) * normalizationMCLowA);

    hResponseMatrixCJet_tempA->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixCJet_tempA->GetBinContent(iBinX+1,iBinY+1) * normalizationMCLowA);
    hResponseMatrixFJet_tempA->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixFJet_tempA->GetBinContent(iBinX+1,iBinY+1) * normalizationMCLowA);
   }
  }    
  
  
  for (Int_t iEvt= 0; iEvt<TreeTrain2_A->GetEntries(); iEvt++) {
   TreeTrain2_A->GetEntry(iEvt);
   if (S_FJet_Pt>0 && G_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0)  {
    if (G_FJet_Pt >= Threshold_G_FJet) {
     hResponseMatrixFJet_A->Fill(S_FJet_Pt, G_FJet_Pt);
    }
    if (G_CJet_Pt >= Threshold_G_CJet) {
     hResponseMatrixCJet_A->Fill(S_CJet_Pt, G_CJet_Pt);
    }
   }
   //       response.Miss (xt);
  }

  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   for (int iBinY = 0; iBinY<NBIN; iBinY++){
    hResponseMatrixCJet_A->SetBinError(iBinX+1,iBinY+1,hResponseMatrixCJet_A->GetBinError(iBinX+1,iBinY+1) * normalizationMCHighA);
    hResponseMatrixFJet_A->SetBinError(iBinX+1,iBinY+1,hResponseMatrixFJet_A->GetBinError(iBinX+1,iBinY+1) * normalizationMCHighA);

    hResponseMatrixCJet_A->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixCJet_A->GetBinContent(iBinX+1,iBinY+1) * normalizationMCHighA);
    hResponseMatrixFJet_A->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixFJet_A->GetBinContent(iBinX+1,iBinY+1) * normalizationMCHighA);
   }
  }    
 
  std::cout << "~~~ scale for variable size binning ~~~~" << std::endl;  
  hResponseMatrixFJet_A->Add(hResponseMatrixFJet_tempA);
  hResponseMatrixCJet_A->Add(hResponseMatrixCJet_tempA);
  
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   for (int iBinY = 0; iBinY<NBIN; iBinY++){

    hResponseMatrixFJet_A->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixFJet_A->GetBinContent(iBinX+1,iBinY+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / (lowEdge[iBinY+1] - lowEdge[iBinY]));
    hResponseMatrixCJet_A->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixCJet_A->GetBinContent(iBinX+1,iBinY+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / (lowEdge[iBinY+1] - lowEdge[iBinY]));

    hResponseMatrixFJet_A->SetBinError(iBinX+1,iBinY+1,hResponseMatrixFJet_A->GetBinError(iBinX+1,iBinY+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / (lowEdge[iBinY+1] - lowEdge[iBinY]));
    hResponseMatrixCJet_A->SetBinError(iBinX+1,iBinY+1,hResponseMatrixCJet_A->GetBinError(iBinX+1,iBinY+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / (lowEdge[iBinY+1] - lowEdge[iBinY]));
    
    
   }
  }
  
  TCanvas* ccResponseCJet_A = new TCanvas("ccResponseCJet_A","ccResponseCJet_A",600,600);  
  hResponseMatrixCJet_A->SetLineColor(kRed);
  hResponseMatrixCJet_A->SetMarkerColor(kRed);
  hResponseMatrixCJet_A->SetMarkerStyle(20);
  hResponseMatrixCJet_A->Draw("LEGOE");
  ccResponseCJet_A->SetLogz();
  
  TCanvas* ccResponseFJet_A = new TCanvas("ccResponseFJet_A","ccResponseFJet_A",600,600);  
  hResponseMatrixFJet_A->SetLineColor(kRed);
  hResponseMatrixFJet_A->SetMarkerColor(kRed);
  hResponseMatrixFJet_A->SetMarkerStyle(20);
  hResponseMatrixFJet_A->Draw("LEGOE");
  ccResponseFJet_A->SetLogz();
  
  
  
  std::cout << "==================================== MC Distributions _A =====================================" << std::endl;
               
  TH1D* hTrueFJet_MC_tempA = new TH1D ("hTrueFJet_MC_tempA", (TString) Form("MC Truth FJet %s", nameTypeFileA.c_str()),    NBIN , lowEdge);
  TH1D* hMeasFJet_MC_tempA = new TH1D ("hMeasFJet_MC_tempA", (TString) Form("MC Measured FJet %s", nameTypeFileA.c_str()), NBIN , lowEdge);
  TH1D* hTrueCJet_MC_tempA = new TH1D ("hTrueCJet_MC_tempA", (TString) Form("MC Truth CJet %s", nameTypeFileA.c_str()),    NBIN , lowEdge);
  TH1D* hMeasCJet_MC_tempA = new TH1D ("hMeasCJet_MC_tempA", (TString) Form("MC Measured CJet %s", nameTypeFileA.c_str()), NBIN , lowEdge);

  TH1D* hTrueFJet_MC_A = new TH1D ("hTrueFJet_MC_A", (TString) Form("MC Truth FJet %s", nameTypeFileA.c_str()),    NBIN , lowEdge);
  TH1D* hMeasFJet_MC_A = new TH1D ("hMeasFJet_MC_A", (TString) Form("MC Measured FJet %s", nameTypeFileA.c_str()), NBIN , lowEdge);
  TH1D* hTrueCJet_MC_A = new TH1D ("hTrueCJet_MC_A", (TString) Form("MC Truth CJet %s", nameTypeFileA.c_str()),    NBIN , lowEdge);
  TH1D* hMeasCJet_MC_A = new TH1D ("hMeasCJet_MC_A", (TString) Form("MC Measured CJet %s", nameTypeFileA.c_str()), NBIN , lowEdge);
  
  for (Int_t iEvt= 0; iEvt<TreeTrain1_A->GetEntries(); iEvt++) {
   TreeTrain1_A->GetEntry(iEvt);
   if (debug) std::cerr << " S_FJet_Pt = " << S_FJet_Pt << std::endl;
   if (S_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0 && G_FJet_Pt) {
    if (G_FJet_Pt < Threshold_G_FJet) {
     hTrueFJet_MC_tempA->Fill(G_FJet_Pt);
     hMeasFJet_MC_tempA->Fill(S_FJet_Pt);
    }
    if (G_CJet_Pt < Threshold_G_CJet) {
     hTrueCJet_MC_tempA->Fill(G_CJet_Pt);
     hMeasCJet_MC_tempA->Fill(S_CJet_Pt);
    }
   }
  }
  
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hTrueFJet_MC_tempA->SetBinError(iBinX+1,hTrueFJet_MC_tempA->GetBinError(iBinX+1) * normalizationMCLowA);
   hTrueCJet_MC_tempA->SetBinError(iBinX+1,hTrueCJet_MC_tempA->GetBinError(iBinX+1) * normalizationMCLowA); 
   hMeasFJet_MC_tempA->SetBinError(iBinX+1,hMeasFJet_MC_tempA->GetBinError(iBinX+1) * normalizationMCLowA);
   hMeasCJet_MC_tempA->SetBinError(iBinX+1,hMeasCJet_MC_tempA->GetBinError(iBinX+1) * normalizationMCLowA);

   hTrueFJet_MC_tempA->SetBinContent(iBinX+1,hTrueFJet_MC_tempA->GetBinContent(iBinX+1) * normalizationMCLowA);
   hTrueCJet_MC_tempA->SetBinContent(iBinX+1,hTrueCJet_MC_tempA->GetBinContent(iBinX+1) * normalizationMCLowA);
   hMeasFJet_MC_tempA->SetBinContent(iBinX+1,hMeasFJet_MC_tempA->GetBinContent(iBinX+1) * normalizationMCLowA);
   hMeasCJet_MC_tempA->SetBinContent(iBinX+1,hMeasCJet_MC_tempA->GetBinContent(iBinX+1) * normalizationMCLowA);   
  }
  
  
  for (Int_t iEvt= 0; iEvt<TreeTrain2_A->GetEntries(); iEvt++) {
   TreeTrain2_A->GetEntry(iEvt);
   if (debug) std::cerr << " S_FJet_Pt = " << S_FJet_Pt << std::endl;
   if (S_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0 && G_FJet_Pt) {
    if (G_FJet_Pt > Threshold_G_FJet) {
     hTrueFJet_MC_A->Fill(G_FJet_Pt);
     hMeasFJet_MC_A->Fill(S_FJet_Pt);
    }
    if (G_CJet_Pt > Threshold_G_CJet) {
     hTrueCJet_MC_A->Fill(G_CJet_Pt);
     hMeasCJet_MC_A->Fill(S_CJet_Pt);
    }
   }
  }  
  
   
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hTrueFJet_MC_A->SetBinError(iBinX+1,hTrueFJet_MC_A->GetBinError(iBinX+1) * normalizationMCHighA);
   hTrueCJet_MC_A->SetBinError(iBinX+1,hTrueCJet_MC_A->GetBinError(iBinX+1) * normalizationMCHighA); 
   hMeasFJet_MC_A->SetBinError(iBinX+1,hMeasFJet_MC_A->GetBinError(iBinX+1) * normalizationMCHighA);
   hMeasCJet_MC_A->SetBinError(iBinX+1,hMeasCJet_MC_A->GetBinError(iBinX+1) * normalizationMCHighA);

   hTrueFJet_MC_A->SetBinContent(iBinX+1,hTrueFJet_MC_A->GetBinContent(iBinX+1) * normalizationMCHighA);
   hTrueCJet_MC_A->SetBinContent(iBinX+1,hTrueCJet_MC_A->GetBinContent(iBinX+1) * normalizationMCHighA);
   hMeasFJet_MC_A->SetBinContent(iBinX+1,hMeasFJet_MC_A->GetBinContent(iBinX+1) * normalizationMCHighA);
   hMeasCJet_MC_A->SetBinContent(iBinX+1,hMeasCJet_MC_A->GetBinContent(iBinX+1) * normalizationMCHighA);   
  }

  hTrueFJet_MC_A->Sumw2();
  hTrueCJet_MC_A->Sumw2();
  hMeasFJet_MC_A->Sumw2();
  hMeasCJet_MC_A->Sumw2();

  hTrueFJet_MC_A->Add(hTrueFJet_MC_tempA);
  hTrueCJet_MC_A->Add(hTrueCJet_MC_tempA);
  hMeasFJet_MC_A->Add(hMeasFJet_MC_tempA);
  hMeasCJet_MC_A->Add(hMeasCJet_MC_tempA);
  
   for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hTrueFJet_MC_A->SetBinError(iBinX+1,hTrueFJet_MC_A->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hTrueCJet_MC_A->SetBinError(iBinX+1,hTrueCJet_MC_A->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hMeasFJet_MC_A->SetBinError(iBinX+1,hMeasFJet_MC_A->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hMeasCJet_MC_A->SetBinError(iBinX+1,hMeasCJet_MC_A->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));  
  
   hTrueFJet_MC_A->SetBinContent(iBinX+1,hTrueFJet_MC_A->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hTrueCJet_MC_A->SetBinContent(iBinX+1,hTrueCJet_MC_A->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hMeasFJet_MC_A->SetBinContent(iBinX+1,hMeasFJet_MC_A->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hMeasCJet_MC_A->SetBinContent(iBinX+1,hMeasCJet_MC_A->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));       
  }
  
  
  
  
  
  std::cout << "==================================== Create Response Matrix _A =====================================" << std::endl;
  
  responseFJet_A.Setup(hMeasFJet_MC_A,hTrueFJet_MC_A,hResponseMatrixFJet_A);
  responseCJet_A.Setup(hMeasCJet_MC_A,hTrueCJet_MC_A,hResponseMatrixCJet_A);


  std::cout << "=================================================================================" << std::endl;
  std::cout << "==================================== _B =====================================" << std::endl;

 for (Int_t iEvt= 0; iEvt<TreeTrain1_B->GetEntries(); iEvt++) {
    TreeTrain1_B->GetEntry(iEvt);
    if (debug && S_FJet_Pt>0 && G_FJet_Pt>0)   std::cerr << " S_FJet_Pt = " << S_FJet_Pt << " G_FJet_Pt = " << G_FJet_Pt << std::endl;
    
    if (S_FJet_Pt>0 && G_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0)  {
     if (G_FJet_Pt < Threshold_G_FJet) {
      hResponseMatrixFJet_tempB->Fill(S_FJet_Pt, G_FJet_Pt);
     }
     if (G_CJet_Pt < Threshold_G_CJet) {
      hResponseMatrixCJet_tempB->Fill(S_CJet_Pt, G_CJet_Pt);
     }
    }
//       response.Miss (xt);
  }
  
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   for (int iBinY = 0; iBinY<NBIN; iBinY++){
    hResponseMatrixCJet_tempB->SetBinError(iBinX+1,iBinY+1,hResponseMatrixCJet_tempB->GetBinError(iBinX+1,iBinY+1) * normalizationMCLowB);
    hResponseMatrixFJet_tempB->SetBinError(iBinX+1,iBinY+1,hResponseMatrixFJet_tempB->GetBinError(iBinX+1,iBinY+1) * normalizationMCLowB);

    hResponseMatrixCJet_tempB->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixCJet_tempB->GetBinContent(iBinX+1,iBinY+1) * normalizationMCLowB);
    hResponseMatrixFJet_tempB->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixFJet_tempB->GetBinContent(iBinX+1,iBinY+1) * normalizationMCLowB);
   }
  }    
  
  
  for (Int_t iEvt= 0; iEvt<TreeTrain2_B->GetEntries(); iEvt++) {
   TreeTrain2_B->GetEntry(iEvt);
   if (debug && S_FJet_Pt>0 && G_FJet_Pt>0)   std::cerr << " S_FJet_Pt = " << S_FJet_Pt << " G_FJet_Pt = " << G_FJet_Pt << std::endl;
   
   if (S_FJet_Pt>0 && G_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0)  {
    if (G_FJet_Pt >= Threshold_G_FJet) {
     hResponseMatrixFJet_B->Fill(S_FJet_Pt, G_FJet_Pt);
    }
    if (G_CJet_Pt >= Threshold_G_CJet) {
     hResponseMatrixCJet_B->Fill(S_CJet_Pt, G_CJet_Pt);
    }
   }
   //       response.Miss (xt);
  }

  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   for (int iBinY = 0; iBinY<NBIN; iBinY++){
    hResponseMatrixCJet_B->SetBinError(iBinX+1,iBinY+1,hResponseMatrixCJet_B->GetBinError(iBinX+1,iBinY+1) * normalizationMCHighB);
    hResponseMatrixFJet_B->SetBinError(iBinX+1,iBinY+1,hResponseMatrixFJet_B->GetBinError(iBinX+1,iBinY+1) * normalizationMCHighB);

    hResponseMatrixCJet_B->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixCJet_B->GetBinContent(iBinX+1,iBinY+1) * normalizationMCHighB);
    hResponseMatrixFJet_B->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixFJet_B->GetBinContent(iBinX+1,iBinY+1) * normalizationMCHighB);
   }
  }    
 
  std::cout << "~~~ scale for variable size binning ~~~~" << std::endl;
  
  hResponseMatrixFJet_B->Add(hResponseMatrixFJet_tempB);
  hResponseMatrixCJet_B->Add(hResponseMatrixCJet_tempB);
  
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   for (int iBinY = 0; iBinY<NBIN; iBinY++){

    hResponseMatrixFJet_B->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixFJet_B->GetBinContent(iBinX+1,iBinY+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / (lowEdge[iBinY+1] - lowEdge[iBinY]));
    hResponseMatrixCJet_B->SetBinContent(iBinX+1,iBinY+1,hResponseMatrixCJet_B->GetBinContent(iBinX+1,iBinY+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / (lowEdge[iBinY+1] - lowEdge[iBinY]));

    hResponseMatrixFJet_B->SetBinError(iBinX+1,iBinY+1,hResponseMatrixFJet_B->GetBinError(iBinX+1,iBinY+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / (lowEdge[iBinY+1] - lowEdge[iBinY]));
    hResponseMatrixCJet_B->SetBinError(iBinX+1,iBinY+1,hResponseMatrixCJet_B->GetBinError(iBinX+1,iBinY+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]) / (lowEdge[iBinY+1] - lowEdge[iBinY]));
    
    
   }
  }
  
  TCanvas* ccResponseCJet_B = new TCanvas("ccResponseCJet_B","ccResponseCJet_B",600,600);  
  hResponseMatrixCJet_B->SetLineColor(kRed);
  hResponseMatrixCJet_B->SetMarkerColor(kRed);
  hResponseMatrixCJet_B->SetMarkerStyle(20);
  hResponseMatrixCJet_B->Draw("LEGOE");
  ccResponseCJet_B->SetLogz();
  
  TCanvas* ccResponseFJet_B = new TCanvas("ccResponseFJet_B","ccResponseFJet_B",600,600);  
  hResponseMatrixFJet_B->SetLineColor(kRed);
  hResponseMatrixFJet_B->SetMarkerColor(kRed);
  hResponseMatrixFJet_B->SetMarkerStyle(20);
  hResponseMatrixFJet_B->Draw("LEGOE");
  ccResponseFJet_B->SetLogz();
  
  
  
  std::cout << "==================================== MC Distributions _B =====================================" << std::endl;
  
  TH1D* hTrueFJet_MC_tempB = new TH1D ("hTrueFJet_MC_tempB", (TString) Form("MC Truth FJet %s", nameTypeFileB.c_str()),    NBIN , lowEdge);
  TH1D* hMeasFJet_MC_tempB = new TH1D ("hMeasFJet_MC_tempB", (TString) Form("MC Measured FJet %s", nameTypeFileB.c_str()), NBIN , lowEdge);
  TH1D* hTrueCJet_MC_tempB = new TH1D ("hTrueCJet_MC_tempB", (TString) Form("MC Truth CJet %s", nameTypeFileB.c_str()),    NBIN , lowEdge);
  TH1D* hMeasCJet_MC_tempB = new TH1D ("hMeasCJet_MC_tempB", (TString) Form("MC Measured CJet %s", nameTypeFileB.c_str()), NBIN , lowEdge);

  TH1D* hTrueFJet_MC_B = new TH1D ("hTrueFJet_MC_B", (TString) Form("MC Truth FJet %s", nameTypeFileB.c_str()),    NBIN , lowEdge);
  TH1D* hMeasFJet_MC_B = new TH1D ("hMeasFJet_MC_B", (TString) Form("MC Measured FJet %s", nameTypeFileB.c_str()), NBIN , lowEdge);
  TH1D* hTrueCJet_MC_B = new TH1D ("hTrueCJet_MC_B", (TString) Form("MC Truth CJet %s", nameTypeFileB.c_str()),    NBIN , lowEdge);
  TH1D* hMeasCJet_MC_B = new TH1D ("hMeasCJet_MC_B", (TString) Form("MC Measured CJet %s", nameTypeFileB.c_str()), NBIN , lowEdge);
  
  for (Int_t iEvt= 0; iEvt<TreeTrain1_B->GetEntries(); iEvt++) {
   TreeTrain1_B->GetEntry(iEvt);
   if (debug) std::cerr << " S_FJet_Pt = " << S_FJet_Pt << std::endl;
   if (S_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0 && G_FJet_Pt) {
    if (G_FJet_Pt < Threshold_G_FJet) {
     hTrueFJet_MC_tempB->Fill(G_FJet_Pt);
     hMeasFJet_MC_tempB->Fill(S_FJet_Pt);
    }
    if (G_CJet_Pt < Threshold_G_CJet) {
     hTrueCJet_MC_tempB->Fill(G_CJet_Pt);
     hMeasCJet_MC_tempB->Fill(S_CJet_Pt);
    }
   }
  }
  
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hTrueFJet_MC_tempB->SetBinError(iBinX+1,hTrueFJet_MC_tempB->GetBinError(iBinX+1) * normalizationMCLowB);
   hTrueCJet_MC_tempB->SetBinError(iBinX+1,hTrueCJet_MC_tempB->GetBinError(iBinX+1) * normalizationMCLowB); 
   hMeasFJet_MC_tempB->SetBinError(iBinX+1,hMeasFJet_MC_tempB->GetBinError(iBinX+1) * normalizationMCLowB);
   hMeasCJet_MC_tempB->SetBinError(iBinX+1,hMeasCJet_MC_tempB->GetBinError(iBinX+1) * normalizationMCLowB);

   hTrueFJet_MC_tempB->SetBinContent(iBinX+1,hTrueFJet_MC_tempB->GetBinContent(iBinX+1) * normalizationMCLowB);
   hTrueCJet_MC_tempB->SetBinContent(iBinX+1,hTrueCJet_MC_tempB->GetBinContent(iBinX+1) * normalizationMCLowB);
   hMeasFJet_MC_tempB->SetBinContent(iBinX+1,hMeasFJet_MC_tempB->GetBinContent(iBinX+1) * normalizationMCLowB);
   hMeasCJet_MC_tempB->SetBinContent(iBinX+1,hMeasCJet_MC_tempB->GetBinContent(iBinX+1) * normalizationMCLowB);   
  }
  
  
  for (Int_t iEvt= 0; iEvt<TreeTrain2_B->GetEntries(); iEvt++) {
   TreeTrain2_B->GetEntry(iEvt);
   if (debug) std::cerr << " S_FJet_Pt = " << S_FJet_Pt << std::endl;
   if (S_FJet_Pt>0 && S_CJet_Pt>0 && G_CJet_Pt>0 && G_FJet_Pt) {
    if (G_FJet_Pt > Threshold_G_FJet) {
     hTrueFJet_MC_B->Fill(G_FJet_Pt);
     hMeasFJet_MC_B->Fill(S_FJet_Pt);
    }
    if (G_CJet_Pt > Threshold_G_CJet) {
     hTrueCJet_MC_B->Fill(G_CJet_Pt);
     hMeasCJet_MC_B->Fill(S_CJet_Pt);
    }
   }
  }  
  
   
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hTrueFJet_MC_B->SetBinError(iBinX+1,hTrueFJet_MC_B->GetBinError(iBinX+1) * normalizationMCHighB);
   hTrueCJet_MC_B->SetBinError(iBinX+1,hTrueCJet_MC_B->GetBinError(iBinX+1) * normalizationMCHighB); 
   hMeasFJet_MC_B->SetBinError(iBinX+1,hMeasFJet_MC_B->GetBinError(iBinX+1) * normalizationMCHighB);
   hMeasCJet_MC_B->SetBinError(iBinX+1,hMeasCJet_MC_B->GetBinError(iBinX+1) * normalizationMCHighB);

   hTrueFJet_MC_B->SetBinContent(iBinX+1,hTrueFJet_MC_B->GetBinContent(iBinX+1) * normalizationMCHighB);
   hTrueCJet_MC_B->SetBinContent(iBinX+1,hTrueCJet_MC_B->GetBinContent(iBinX+1) * normalizationMCHighB);
   hMeasFJet_MC_B->SetBinContent(iBinX+1,hMeasFJet_MC_B->GetBinContent(iBinX+1) * normalizationMCHighB);
   hMeasCJet_MC_B->SetBinContent(iBinX+1,hMeasCJet_MC_B->GetBinContent(iBinX+1) * normalizationMCHighB);   
  }

  hTrueFJet_MC_B->Sumw2();
  hTrueCJet_MC_B->Sumw2();
  hMeasFJet_MC_B->Sumw2();
  hMeasCJet_MC_B->Sumw2();

  hTrueFJet_MC_B->Add(hTrueFJet_MC_tempB);
  hTrueCJet_MC_B->Add(hTrueCJet_MC_tempB);
  hMeasFJet_MC_B->Add(hMeasFJet_MC_tempB);
  hMeasCJet_MC_B->Add(hMeasCJet_MC_tempB);
  
   for (int iBinX = 0; iBinX<NBIN; iBinX++){
   hTrueFJet_MC_B->SetBinError(iBinX+1,hTrueFJet_MC_B->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hTrueCJet_MC_B->SetBinError(iBinX+1,hTrueCJet_MC_B->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hMeasFJet_MC_B->SetBinError(iBinX+1,hMeasFJet_MC_B->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hMeasCJet_MC_B->SetBinError(iBinX+1,hMeasCJet_MC_B->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));  
  
   hTrueFJet_MC_B->SetBinContent(iBinX+1,hTrueFJet_MC_B->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hTrueCJet_MC_B->SetBinContent(iBinX+1,hTrueCJet_MC_B->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hMeasFJet_MC_B->SetBinContent(iBinX+1,hMeasFJet_MC_B->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hMeasCJet_MC_B->SetBinContent(iBinX+1,hMeasCJet_MC_B->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));       
  }
  
  
  
  
  
  std::cout << "==================================== Create Response Matrix _B =====================================" << std::endl;
  
  responseFJet_B.Setup(hMeasFJet_MC_B,hTrueFJet_MC_B,hResponseMatrixFJet_B);
  responseCJet_B.Setup(hMeasCJet_MC_B,hTrueCJet_MC_B,hResponseMatrixCJet_B);








  
  std::cout << "==================================== TEST =====================================" << std::endl;

  double JES = 0.05;
  
  TFile FileTest(inputFileDATA.c_str(),"READ");
  
  TTree* TreeTest = (TTree*) FileTest.Get(treeName.c_str());
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
   if (debug) std::cerr << " S_FJet_Pt = " << S_FJet_Pt << std::endl;
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
  
  std::cout << "==================================== UNFOLD _A ===================================" << std::endl;
  RooUnfold* unfoldFJet_A = RooUnfold::New((RooUnfold::Algorithm) ALGO,&responseFJet_A, hMeasFJet);
  TH1D* hRecoFJet_A = (TH1D*) unfoldFJet_A->Hreco((RooUnfold::ErrorTreatment) 3);
  hRecoFJet_A->SetName((TString) Form("FJetUnfold%s", nameTypeFileA.c_str()));
  hRecoFJet_A->SetTitle((TString) Form("FJet Unfold %s", nameTypeFileA.c_str()));
    
  RooUnfold* unfoldCJet_A = RooUnfold::New((RooUnfold::Algorithm) ALGO,&responseCJet_A, hMeasCJet);
  TH1D* hRecoCJet_A = (TH1D*) unfoldCJet_A->Hreco((RooUnfold::ErrorTreatment) 3);
  hRecoCJet_A->SetName((TString) Form("CJetUnfold%s", nameTypeFileA.c_str()));
  hRecoCJet_A->SetTitle((TString) Form("CJet Unfold %s", nameTypeFileA.c_str()));

  RooUnfold* unfoldFJet_Plus_A = RooUnfold::New((RooUnfold::Algorithm) ALGO,&responseFJet_A, hMeasFJet_Plus);
  TH1D* hRecoFJet_Plus_A = (TH1D*) unfoldFJet_Plus_A->Hreco((RooUnfold::ErrorTreatment) 3);
  hRecoFJet_Plus_A->SetName((TString) Form("FJetUnfold_Plus%s", nameTypeFileA.c_str()));
  
  RooUnfold* unfoldCJet_Plus_A = RooUnfold::New((RooUnfold::Algorithm) ALGO,&responseCJet_A, hMeasCJet_Plus);
  TH1D* hRecoCJet_Plus_A = (TH1D*) unfoldCJet_Plus_A->Hreco((RooUnfold::ErrorTreatment) 3);
  hRecoCJet_Plus_A->SetName((TString) Form("CJetUnfold_Plus%s", nameTypeFileA.c_str()));

  RooUnfold* unfoldFJet_Minus_A = RooUnfold::New((RooUnfold::Algorithm) ALGO,&responseFJet_A, hMeasFJet_Minus);
  TH1D* hRecoFJet_Minus_A = (TH1D*) unfoldFJet_Minus_A->Hreco((RooUnfold::ErrorTreatment) 3);
  hRecoFJet_Minus_A->SetName((TString) Form("FJetUnfold_Minus%s", nameTypeFileA.c_str()));
  
  RooUnfold* unfoldCJet_Minus_A = RooUnfold::New((RooUnfold::Algorithm) ALGO,&responseCJet_A, hMeasCJet_Minus);
  TH1D* hRecoCJet_Minus_A = (TH1D*) unfoldCJet_Minus_A->Hreco((RooUnfold::ErrorTreatment) 3);
  hRecoCJet_Minus_A->SetName((TString) Form("CJetUnfold_Minus%s", nameTypeFileA.c_str()));
    
  hRecoFJet_A->Sumw2();
  hRecoCJet_A->Sumw2();
  hRecoFJet_Plus_A->Sumw2();
  hRecoCJet_Plus_A->Sumw2();
  hRecoFJet_Minus_A->Sumw2();
  hRecoCJet_Minus_A->Sumw2();
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
  if (ALGO == 1 || ALGO == 2){
   hRecoFJet_A->SetBinError(iBinX+1,hRecoFJet_A->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hRecoCJet_A->SetBinError(iBinX+1,hRecoCJet_A->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));    
   hRecoFJet_Plus_A->SetBinError(iBinX+1,hRecoFJet_Plus_A->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hRecoCJet_Plus_A->SetBinError(iBinX+1,hRecoCJet_Plus_A->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));    
   hRecoFJet_Minus_A->SetBinError(iBinX+1,hRecoFJet_Minus_A->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hRecoCJet_Minus_A->SetBinError(iBinX+1,hRecoCJet_Minus_A->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));    

   hRecoFJet_A->SetBinContent(iBinX+1,hRecoFJet_A->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hRecoCJet_A->SetBinContent(iBinX+1,hRecoCJet_A->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));    
   hRecoFJet_Plus_A->SetBinContent(iBinX+1,hRecoFJet_Plus_A->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hRecoCJet_Plus_A->SetBinContent(iBinX+1,hRecoCJet_Plus_A->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));    
   hRecoFJet_Minus_A->SetBinContent(iBinX+1,hRecoFJet_Minus_A->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hRecoCJet_Minus_A->SetBinContent(iBinX+1,hRecoCJet_Minus_A->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));    
   }
  }

  std::cout << "===================================== DRAW ====================================" << std::endl;
  
 
  
  TCanvas* ccFJet_A = new TCanvas((TString) Form("ccFJet%s", nameTypeFileA.c_str()),(TString) Form("ccFJet%s", nameTypeFileA.c_str()),600,600);  
  hRecoFJet_A->GetYaxis()->SetTitle("d#sigma/dp_{T} (pb/GeV)");
  hRecoFJet_A->GetXaxis()->SetTitle("forward jet p_{T} (GeV)");
  hRecoFJet_A->SetLineColor(kBlue);
  hRecoFJet_A->SetMarkerColor(kBlue);
  hRecoFJet_A->SetMarkerStyle(20);
  hRecoFJet_A->SetMarkerSize(1);
  hRecoFJet_A->DrawCopy();

  hRecoFJet_Plus_A->GetXaxis()->SetTitle("p_{T} had");
  hRecoFJet_Plus_A->SetFillColor(kRed);  
  hRecoFJet_Plus_A->SetLineColor(kRed);
  hRecoFJet_Plus_A->SetMarkerColor(kRed);
  hRecoFJet_Plus_A->SetMarkerStyle(20);
  hRecoFJet_Plus_A->SetMarkerSize(1);
  hRecoFJet_Plus_A->DrawCopy("SAME");
  
  hRecoFJet_Minus_A->GetXaxis()->SetTitle("p_{T} had");
  hRecoFJet_Minus_A->SetFillColor(kRed);
  hRecoFJet_Minus_A->SetLineColor(kRed);
  hRecoFJet_Minus_A->SetMarkerColor(kRed);
  hRecoFJet_Minus_A->SetMarkerStyle(20);
  hRecoFJet_Minus_A->SetMarkerSize(1);
  hRecoFJet_Minus_A->DrawCopy("SAME");
  
  hRecoFJet_A->DrawCopy("SAME");
 // gPad->BuildLegend();
  gPad->SetGrid();
 
  
  TCanvas* ccCJet_A = new TCanvas((TString) Form("ccCJet%s", nameTypeFileA.c_str()),(TString) Form("ccCJet%s", nameTypeFileA.c_str()),600,600);  
  hRecoCJet_A->GetYaxis()->SetTitle("d#sigma/dp_{T} (pb/GeV)");
  hRecoCJet_A->GetXaxis()->SetTitle("central jet p_{T} (GeV)");
  hRecoCJet_A->SetLineColor(kBlue);
  hRecoCJet_A->SetMarkerColor(kBlue);
  hRecoCJet_A->SetMarkerStyle(20);
  hRecoCJet_A->SetMarkerSize(1);
  hRecoCJet_A->DrawCopy();

  hRecoCJet_Plus_A->GetXaxis()->SetTitle("p_{T} had");
  hRecoCJet_Plus_A->SetFillColor(kRed);
  hRecoCJet_Plus_A->SetLineColor(kRed);
  hRecoCJet_Plus_A->SetMarkerColor(kRed);
  hRecoCJet_Plus_A->SetMarkerStyle(20);
  hRecoCJet_Plus_A->SetMarkerSize(1);
  hRecoCJet_Plus_A->DrawCopy("SAME");
  
  hRecoCJet_Minus_A->GetXaxis()->SetTitle("p_{T} had");
  hRecoCJet_Minus_A->SetFillColor(kRed);
  hRecoCJet_Minus_A->SetLineColor(kRed);
  hRecoCJet_Minus_A->SetMarkerColor(kRed);
  hRecoCJet_Minus_A->SetMarkerStyle(20);
  hRecoCJet_Minus_A->SetMarkerSize(1);
  hRecoCJet_Minus_A->DrawCopy("SAME");

  hRecoCJet_A->DrawCopy("SAME");
 // gPad->BuildLegend();
  gPad->SetGrid();
  
  
  std::cout << "===================================== DRAW ====================================" << std::endl;

  
  
  
  
  
      
  TH1D* hRecoFJet_Mean_A = new TH1D ((TString) Form("hRecoFJet_Mean%s", nameTypeFileA.c_str()), (TString) Form("UnFolded FJet JES %s", nameTypeFileA.c_str()), NBIN , lowEdge);
  TH1D* hRecoCJet_Mean_A = new TH1D ((TString) Form("hRecoCJet_Mean%s", nameTypeFileA.c_str()), (TString) Form("UnFolded CJet JES %s", nameTypeFileA.c_str()), NBIN , lowEdge);  

 for (int iBinX = 0; iBinX<NBIN; iBinX++){
  hRecoFJet_Mean_A->SetBinContent(iBinX+1,(hRecoFJet_Plus_A->GetBinContent(iBinX+1) + hRecoFJet_Minus_A->GetBinContent(iBinX+1))/2.);
  hRecoCJet_Mean_A->SetBinContent(iBinX+1,(hRecoCJet_Plus_A->GetBinContent(iBinX+1) + hRecoCJet_Minus_A->GetBinContent(iBinX+1))/2.);

  hRecoFJet_Mean_A->SetBinError(iBinX+1,fabs(hRecoFJet_Plus_A->GetBinContent(iBinX+1) - hRecoFJet_Minus_A->GetBinContent(iBinX+1))/2.);
  hRecoCJet_Mean_A->SetBinError(iBinX+1,fabs(hRecoCJet_Plus_A->GetBinContent(iBinX+1) - hRecoCJet_Minus_A->GetBinContent(iBinX+1))/2.);     
 }

 
 
 
  TCanvas* ccFJet_JES_A = new TCanvas((TString) Form("ccFJet_JES%s", nameTypeFileA.c_str()),(TString) Form("ccFJet_JES%s", nameTypeFileA.c_str()),600,600);  

  hRecoFJet_Mean_A->GetYaxis()->SetTitle("d#sigma/dp_{T} (pb/GeV)");
  hRecoFJet_Mean_A->GetXaxis()->SetTitle("forward jet p_{T} (GeV)");
  hRecoFJet_Mean_A->SetFillColor(kRed);  
  hRecoFJet_Mean_A->SetLineColor(kRed);  
  hRecoFJet_Mean_A->SetMarkerColor(kRed); 
  hRecoFJet_Mean_A->SetMarkerSize(0); 
  hRecoFJet_Mean_A->SetLineWidth(0);  
  hRecoFJet_Mean_A->DrawCopy("E2");

  hRecoFJet_A->GetXaxis()->SetTitle("p_{T} had");
  hRecoFJet_A->SetLineColor(kBlue);
  hRecoFJet_A->SetMarkerColor(kBlue);
  hRecoFJet_A->SetMarkerStyle(20);
  hRecoFJet_A->SetMarkerSize(1);
  hRecoFJet_A->DrawCopy("SAMEE");
  
  hTrueFJet_MC_A->SetLineColor(kGreen);
  hTrueFJet_MC_A->SetMarkerColor(kGreen);
  hTrueFJet_MC_A->SetMarkerStyle(21);
  hTrueFJet_MC_A->SetMarkerSize(1);
  hTrueFJet_MC_A->DrawCopy("SAMEE");
  
//  gPad->BuildLegend();
  gPad->SetGrid();

  
  
  TCanvas* ccCJet_JES_A = new TCanvas((TString) Form("ccCJet_JES%s", nameTypeFileA.c_str()),(TString) Form("ccCJet_JES%s", nameTypeFileA.c_str()),600,600);  

  hRecoCJet_Mean_A->GetYaxis()->SetTitle("d#sigma/dp_{T} (pb/GeV)");
  hRecoCJet_Mean_A->GetXaxis()->SetTitle("central jet p_{T} (GeV)");
  hRecoCJet_Mean_A->SetFillColor(kRed);  
  hRecoCJet_Mean_A->SetLineColor(kRed);  
  hRecoCJet_Mean_A->SetMarkerColor(kRed); 
  hRecoCJet_Mean_A->SetMarkerSize(0); 
  hRecoCJet_Mean_A->SetLineWidth(0);  
  hRecoCJet_Mean_A->DrawCopy("E2");

  hRecoCJet_A->GetXaxis()->SetTitle("p_{T} had");
  hRecoCJet_A->SetLineColor(kBlue);
  hRecoCJet_A->SetMarkerColor(kBlue);
  hRecoCJet_A->SetMarkerStyle(20);
  hRecoCJet_A->SetMarkerSize(1);
  hRecoCJet_A->DrawCopy("SAMEE");

  hTrueCJet_MC_A->SetLineColor(kGreen);
  hTrueCJet_MC_A->SetMarkerColor(kGreen);
  hTrueCJet_MC_A->SetMarkerStyle(21);
  hTrueCJet_MC_A->SetMarkerSize(1);
  hTrueCJet_MC_A->DrawCopy("SAMEE");

 // gPad->BuildLegend();
  gPad->SetGrid(); 
  
  std::cout << "==================================== JES test =====================================" << std::endl;
    
  
  
  
  
  
  std::cout << "==================================== UNFOLD _B ===================================" << std::endl;
  RooUnfold* unfoldFJet_B = RooUnfold::New((RooUnfold::Algorithm) ALGO,&responseFJet_B, hMeasFJet);
  TH1D* hRecoFJet_B = (TH1D*) unfoldFJet_B->Hreco((RooUnfold::ErrorTreatment) 3);
  hRecoFJet_B->SetName((TString) Form("FJetUnfold%s", nameTypeFileB.c_str()));
  hRecoFJet_B->SetTitle((TString) Form("FJet Unfold %s", nameTypeFileB.c_str()));
    
  RooUnfold* unfoldCJet_B = RooUnfold::New((RooUnfold::Algorithm) ALGO,&responseCJet_B, hMeasCJet);
  TH1D* hRecoCJet_B = (TH1D*) unfoldCJet_B->Hreco((RooUnfold::ErrorTreatment) 3);
  hRecoCJet_B->SetName((TString) Form("CJetUnfold%s", nameTypeFileB.c_str()));
  hRecoCJet_B->SetTitle((TString) Form("CJet Unfold %s", nameTypeFileB.c_str()));
 
  RooUnfold* unfoldFJet_Plus_B = RooUnfold::New((RooUnfold::Algorithm) ALGO,&responseFJet_B, hMeasFJet_Plus);
  TH1D* hRecoFJet_Plus_B = (TH1D*) unfoldFJet_Plus_B->Hreco((RooUnfold::ErrorTreatment) 3);
  hRecoFJet_Plus_B->SetName((TString) Form("FJetUnfold_Plus%s", nameTypeFileB.c_str()));
   RooUnfold* unfoldCJet_Plus_B = RooUnfold::New((RooUnfold::Algorithm) ALGO,&responseCJet_B, hMeasCJet_Plus);
  TH1D* hRecoCJet_Plus_B = (TH1D*) unfoldCJet_Plus_B->Hreco((RooUnfold::ErrorTreatment) 3);
  hRecoCJet_Plus_B->SetName((TString) Form("CJetUnfold_Plus%s", nameTypeFileB.c_str()));

  RooUnfold* unfoldFJet_Minus_B = RooUnfold::New((RooUnfold::Algorithm) ALGO,&responseFJet_B, hMeasFJet_Minus);
  TH1D* hRecoFJet_Minus_B = (TH1D*) unfoldFJet_Minus_B->Hreco((RooUnfold::ErrorTreatment) 3);
  hRecoFJet_Minus_B->SetName((TString) Form("FJetUnfold_Minus%s", nameTypeFileB.c_str()));

  RooUnfold* unfoldCJet_Minus_B = RooUnfold::New((RooUnfold::Algorithm) ALGO,&responseCJet_B, hMeasCJet_Minus);
  TH1D* hRecoCJet_Minus_B = (TH1D*) unfoldCJet_Minus_B->Hreco((RooUnfold::ErrorTreatment) 3);
  hRecoCJet_Minus_B->SetName((TString) Form("CJetUnfold_Minus%s", nameTypeFileB.c_str()));
    
  hRecoFJet_B->Sumw2();
  hRecoCJet_B->Sumw2();
  hRecoFJet_Plus_B->Sumw2();
  hRecoCJet_Plus_B->Sumw2();
  hRecoFJet_Minus_B->Sumw2();
  hRecoCJet_Minus_B->Sumw2();
  
  for (int iBinX = 0; iBinX<NBIN; iBinX++){
   if (ALGO == 1 || ALGO == 2) {
   hRecoFJet_B->SetBinError(iBinX+1,hRecoFJet_B->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hRecoCJet_B->SetBinError(iBinX+1,hRecoCJet_B->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));    
   hRecoFJet_Plus_B->SetBinError(iBinX+1,hRecoFJet_Plus_B->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hRecoCJet_Plus_B->SetBinError(iBinX+1,hRecoCJet_Plus_B->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));    
   hRecoFJet_Minus_B->SetBinError(iBinX+1,hRecoFJet_Minus_B->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hRecoCJet_Minus_B->SetBinError(iBinX+1,hRecoCJet_Minus_B->GetBinError(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));    

   hRecoFJet_B->SetBinContent(iBinX+1,hRecoFJet_B->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hRecoCJet_B->SetBinContent(iBinX+1,hRecoCJet_B->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));    
   hRecoFJet_Plus_B->SetBinContent(iBinX+1,hRecoFJet_Plus_B->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hRecoCJet_Plus_B->SetBinContent(iBinX+1,hRecoCJet_Plus_B->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));    
   hRecoFJet_Minus_B->SetBinContent(iBinX+1,hRecoFJet_Minus_B->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));
   hRecoCJet_Minus_B->SetBinContent(iBinX+1,hRecoCJet_Minus_B->GetBinContent(iBinX+1) / (lowEdge[iBinX+1] - lowEdge[iBinX]));    
   }
  }

  std::cout << "===================================== DRAW ====================================" << std::endl;
  
  TCanvas* ccFJet_B = new TCanvas((TString) Form("ccFJet%s", nameTypeFileB.c_str()),(TString) Form("ccFJet%s", nameTypeFileB.c_str()),600,600);  
  hRecoFJet_B->GetYaxis()->SetTitle("d#sigma/dp_{T} (pb/GeV)");
  hRecoFJet_B->GetXaxis()->SetTitle("forward jet p_{T} (GeV)");
  hRecoFJet_B->SetLineColor(kBlue);
  hRecoFJet_B->SetMarkerColor(kBlue);
  hRecoFJet_B->SetMarkerStyle(20);
  hRecoFJet_B->SetMarkerSize(1);
  hRecoFJet_B->DrawCopy();

  hRecoFJet_Plus_B->GetXaxis()->SetTitle("p_{T} had");
  hRecoFJet_Plus_B->SetFillColor(kRed);  
  hRecoFJet_Plus_B->SetLineColor(kRed);
  hRecoFJet_Plus_B->SetMarkerColor(kRed);
  hRecoFJet_Plus_B->SetMarkerStyle(20);
  hRecoFJet_Plus_B->SetMarkerSize(1);
  hRecoFJet_Plus_B->DrawCopy("SAME");
  
  hRecoFJet_Minus_B->GetXaxis()->SetTitle("p_{T} had");
  hRecoFJet_Minus_B->SetFillColor(kRed);
  hRecoFJet_Minus_B->SetLineColor(kRed);
  hRecoFJet_Minus_B->SetMarkerColor(kRed);
  hRecoFJet_Minus_B->SetMarkerStyle(20);
  hRecoFJet_Minus_B->SetMarkerSize(1);
  hRecoFJet_Minus_B->DrawCopy("SAME");
  
  hRecoFJet_B->DrawCopy("SAME");
 // gPad->BuildLegend();
  gPad->SetGrid();
 
  
  TCanvas* ccCJet_B = new TCanvas((TString) Form("ccCJet%s", nameTypeFileB.c_str()),(TString) Form("ccCJet%s", nameTypeFileB.c_str()),600,600);  
  hRecoCJet_B->GetYaxis()->SetTitle("d#sigma/dp_{T} (pb/GeV)");
  hRecoCJet_B->GetXaxis()->SetTitle("central jet p_{T} (GeV)");
  hRecoCJet_B->SetLineColor(kBlue);
  hRecoCJet_B->SetMarkerColor(kBlue);
  hRecoCJet_B->SetMarkerStyle(20);
  hRecoCJet_B->SetMarkerSize(1);
  hRecoCJet_B->DrawCopy();

  hRecoCJet_Plus_B->GetXaxis()->SetTitle("p_{T} had");
  hRecoCJet_Plus_B->SetFillColor(kRed);
  hRecoCJet_Plus_B->SetLineColor(kRed);
  hRecoCJet_Plus_B->SetMarkerColor(kRed);
  hRecoCJet_Plus_B->SetMarkerStyle(20);
  hRecoCJet_Plus_B->SetMarkerSize(1);
  hRecoCJet_Plus_B->DrawCopy("SAME");
  
  hRecoCJet_Minus_B->GetXaxis()->SetTitle("p_{T} had");
  hRecoCJet_Minus_B->SetFillColor(kRed);
  hRecoCJet_Minus_B->SetLineColor(kRed);
  hRecoCJet_Minus_B->SetMarkerColor(kRed);
  hRecoCJet_Minus_B->SetMarkerStyle(20);
  hRecoCJet_Minus_B->SetMarkerSize(1);
  hRecoCJet_Minus_B->DrawCopy("SAME");

  hRecoCJet_B->DrawCopy("SAME");
//  gPad->BuildLegend();
  gPad->SetGrid();
  
  
  std::cout << "===================================== DRAW ====================================" << std::endl;

  TH1D* hRecoFJet_Mean_B = new TH1D ((TString) Form("hRecoFJet_Mean%s", nameTypeFileB.c_str()), (TString) Form("UnFolded FJet JES %s", nameTypeFileB.c_str()), NBIN , lowEdge);
  TH1D* hRecoCJet_Mean_B = new TH1D ((TString) Form("hRecoCJet_Mean%s", nameTypeFileB.c_str()), (TString) Form("UnFolded CJet JES %s", nameTypeFileB.c_str()), NBIN , lowEdge);  

 for (int iBinX = 0; iBinX<NBIN; iBinX++){
  hRecoFJet_Mean_B->SetBinContent(iBinX+1,(hRecoFJet_Plus_B->GetBinContent(iBinX+1) + hRecoFJet_Minus_B->GetBinContent(iBinX+1))/2.);
  hRecoCJet_Mean_B->SetBinContent(iBinX+1,(hRecoCJet_Plus_B->GetBinContent(iBinX+1) + hRecoCJet_Minus_B->GetBinContent(iBinX+1))/2.);

  hRecoFJet_Mean_B->SetBinError(iBinX+1,fabs(hRecoFJet_Plus_B->GetBinContent(iBinX+1) - hRecoFJet_Minus_B->GetBinContent(iBinX+1))/2.);
  hRecoCJet_Mean_B->SetBinError(iBinX+1,fabs(hRecoCJet_Plus_B->GetBinContent(iBinX+1) - hRecoCJet_Minus_B->GetBinContent(iBinX+1))/2.);     
 }

 
  TCanvas* ccFJet_JES_B = new TCanvas((TString) Form("ccFJet_JES%s", nameTypeFileB.c_str()),(TString) Form("ccFJet_JES%s", nameTypeFileB.c_str()),600,600);  

  hRecoFJet_Mean_B->GetYaxis()->SetTitle("d#sigma/dp_{T} (pb/GeV)");
  hRecoFJet_Mean_B->GetXaxis()->SetTitle("forward jet p_{T} (GeV)");
  hRecoFJet_Mean_B->SetFillColor(kRed);  
  hRecoFJet_Mean_B->SetLineColor(kRed);  
  hRecoFJet_Mean_B->SetMarkerColor(kRed);
  hRecoFJet_Mean_B->SetMarkerSize(0); 
  hRecoFJet_Mean_B->SetLineWidth(0);  
  hRecoFJet_Mean_B->DrawCopy("E2");

  hRecoFJet_B->GetXaxis()->SetTitle("p_{T} had");
  hRecoFJet_B->SetLineColor(kBlue);
  hRecoFJet_B->SetMarkerColor(kBlue);
  hRecoFJet_B->SetMarkerStyle(20);
  hRecoFJet_B->SetMarkerSize(1);
  hRecoFJet_B->DrawCopy("SAMEE");
  
  hTrueFJet_MC_B->SetLineColor(kGreen);
  hTrueFJet_MC_B->SetMarkerColor(kGreen);
  hTrueFJet_MC_B->SetMarkerStyle(21);
  hTrueFJet_MC_B->SetMarkerSize(1);
  hTrueFJet_MC_B->DrawCopy("SAMEE");
  
//  gPad->BuildLegend();
  gPad->SetGrid();

  
  
  TCanvas* ccCJet_JES_B = new TCanvas((TString) Form("ccCJet_JES%s", nameTypeFileB.c_str()),(TString) Form("ccCJet_JES%s", nameTypeFileB.c_str()),600,600);  

  hRecoCJet_Mean_B->GetYaxis()->SetTitle("d#sigma/dp_{T} (pb/GeV)");
  hRecoCJet_Mean_B->GetXaxis()->SetTitle("central jet p_{T} (GeV)");
  hRecoCJet_Mean_B->SetFillColor(kRed);  
  hRecoCJet_Mean_B->SetLineColor(kRed);  
  hRecoCJet_Mean_B->SetMarkerColor(kRed); 
  hRecoCJet_Mean_B->SetMarkerSize(0); 
  hRecoCJet_Mean_B->SetLineWidth(0); 
  hRecoCJet_Mean_B->DrawCopy("E2");

  hRecoCJet_B->GetXaxis()->SetTitle("p_{T} had");
  hRecoCJet_B->SetLineColor(kBlue);
  hRecoCJet_B->SetMarkerColor(kBlue);
  hRecoCJet_B->SetMarkerStyle(20);
  hRecoCJet_B->SetMarkerSize(1);
  hRecoCJet_B->DrawCopy("SAMEE");

  hTrueCJet_MC_B->SetLineColor(kGreen);
  hTrueCJet_MC_B->SetMarkerColor(kGreen);
  hTrueCJet_MC_B->SetMarkerStyle(21);
  hTrueCJet_MC_B->SetMarkerSize(1);
  hTrueCJet_MC_B->DrawCopy("SAMEE");

//  gPad->BuildLegend();
  gPad->SetGrid(); 
  
  std::cout << "==================================== JES test =====================================" << std::endl;
    
  
  
  TCanvas* ccCJet_JES_B_A = new TCanvas((TString) Form("ccCJet_JES%s%s", nameTypeFileA.c_str(),nameTypeFileB.c_str()),(TString) Form("ccCJet_JES%s%s", nameTypeFileA.c_str(),nameTypeFileB.c_str()),600,600);  

  hRecoCJet_Mean_B->GetYaxis()->SetTitle("d#sigma/dp_{T} (pb/GeV)");
  hRecoCJet_Mean_B->GetXaxis()->SetTitle("central jet p_{T} (GeV)");
  hRecoCJet_Mean_B->SetFillStyle(3001);  
  hRecoCJet_Mean_B->SetFillColor(kYellow);  
  hRecoCJet_Mean_B->SetLineColor(kYellow);  
  hRecoCJet_Mean_B->SetMarkerColor(kYellow); 
  hRecoCJet_Mean_B->SetMarkerSize(0); 
  hRecoCJet_Mean_B->SetLineWidth(0); 
  hRecoCJet_Mean_B->DrawCopy("E2");

  hRecoCJet_B->GetXaxis()->SetTitle("p_{T} had");
  hRecoCJet_B->SetLineColor(kOrange);
  hRecoCJet_B->SetMarkerColor(kOrange);
  hRecoCJet_B->SetMarkerStyle(20);
  hRecoCJet_B->SetMarkerSize(1);
  hRecoCJet_B->DrawCopy("SAMEE");

  hRecoCJet_Mean_A->SetFillStyle(3004);  
  hRecoCJet_Mean_A->SetFillColor(kBlue);  
  hRecoCJet_Mean_A->SetLineColor(kBlue);  
  hRecoCJet_Mean_A->SetMarkerColor(kBlue); 
  hRecoCJet_Mean_A->SetMarkerSize(0); 
  hRecoCJet_Mean_A->SetLineWidth(0); 
  hRecoCJet_Mean_A->DrawCopy("E2SAME");

  hRecoCJet_A->GetXaxis()->SetTitle("p_{T} had");
  hRecoCJet_A->SetLineColor(kAzure);
  hRecoCJet_A->SetMarkerColor(kAzure);
  hRecoCJet_A->SetMarkerStyle(20);
  hRecoCJet_A->SetMarkerSize(1);
  hRecoCJet_A->DrawCopy("SAMEE");

  hTrueCJet_MC_B->SetLineColor(kRed);
  hTrueCJet_MC_B->SetMarkerColor(kRed);
  hTrueCJet_MC_B->SetMarkerStyle(21);
  hTrueCJet_MC_B->SetMarkerSize(1);
  hTrueCJet_MC_B->DrawCopy("SAMEE");

  hTrueCJet_MC_A->SetLineColor(kGreen);
  hTrueCJet_MC_A->SetMarkerColor(kGreen);
  hTrueCJet_MC_A->SetMarkerStyle(21);
  hTrueCJet_MC_A->SetMarkerSize(1);
  hTrueCJet_MC_A->DrawCopy("SAMEE");

//  gPad->BuildLegend();
  gPad->SetGrid(); 

  
  TCanvas* ccFJet_JES_B_A = new TCanvas((TString) Form("ccFJet_JES%s%s", nameTypeFileA.c_str(),nameTypeFileB.c_str()),(TString) Form("ccFJet_JES%s%s", nameTypeFileA.c_str(),nameTypeFileB.c_str()),600,600);  

  hRecoFJet_Mean_B->GetYaxis()->SetTitle("d#sigma/dp_{T} (pb/GeV)");
  hRecoFJet_Mean_B->GetXaxis()->SetTitle("forward jet p_{T} (GeV)");
  hRecoFJet_Mean_B->SetFillStyle(3001);  
  hRecoFJet_Mean_B->SetFillColor(kYellow);  
  hRecoFJet_Mean_B->SetLineColor(kYellow);  
  hRecoFJet_Mean_B->SetMarkerColor(kYellow); 
  hRecoFJet_Mean_B->SetMarkerSize(0); 
  hRecoFJet_Mean_B->SetLineWidth(0); 
  hRecoFJet_Mean_B->DrawCopy("E2");

  hRecoFJet_B->GetXaxis()->SetTitle("p_{T} had");
  hRecoFJet_B->SetLineColor(kOrange);
  hRecoFJet_B->SetMarkerColor(kOrange);
  hRecoFJet_B->SetMarkerStyle(20);
  hRecoFJet_B->SetMarkerSize(1);
  hRecoFJet_B->DrawCopy("SAMEE");

  hRecoFJet_Mean_A->SetFillStyle(3004);  
  hRecoFJet_Mean_A->SetFillColor(kBlue);  
  hRecoFJet_Mean_A->SetLineColor(kBlue);  
  hRecoFJet_Mean_A->SetMarkerColor(kBlue); 
  hRecoFJet_Mean_A->SetMarkerSize(0); 
  hRecoFJet_Mean_A->SetLineWidth(0); 
  hRecoFJet_Mean_A->DrawCopy("E2SAME");

  hRecoFJet_A->GetXaxis()->SetTitle("p_{T} had");
  hRecoFJet_A->SetLineColor(kAzure);
  hRecoFJet_A->SetMarkerColor(kAzure);
  hRecoFJet_A->SetMarkerStyle(20);
  hRecoFJet_A->SetMarkerSize(1);
  hRecoFJet_A->DrawCopy("SAMEE");

  hTrueFJet_MC_B->SetLineColor(kRed);
  hTrueFJet_MC_B->SetMarkerColor(kRed);
  hTrueFJet_MC_B->SetMarkerStyle(21);
  hTrueFJet_MC_B->SetMarkerSize(1);
  hTrueFJet_MC_B->DrawCopy("SAMEE");

  hTrueFJet_MC_A->SetLineColor(kGreen);
  hTrueFJet_MC_A->SetMarkerColor(kGreen);
  hTrueFJet_MC_A->SetMarkerStyle(21);
  hTrueFJet_MC_A->SetMarkerSize(1);
  hTrueFJet_MC_A->DrawCopy("SAMEE");

 // gPad->BuildLegend();
  gPad->SetGrid(); 

  
  


  //==== write to output file ====
  
  outFile.cd();
  
  hResponseMatrixCJet_A->Write();  
  hResponseMatrixCJet_B->Write();
  hRecoCJet_Mean_A->Write();
  hRecoCJet_Mean_B->Write();
  
  hRecoCJet_A->Write();
  hTrueCJet_MC_A->Write();
  hRecoCJet_B->Write();
  hTrueCJet_MC_B->Write();
    
  hResponseMatrixFJet_A->Write();
  hResponseMatrixFJet_B->Write();
  hRecoFJet_Mean_A->Write();
  hRecoFJet_Mean_B->Write();
  
  hRecoFJet_A->Write();
  hTrueFJet_MC_A->Write();
  hRecoFJet_B->Write();
  hTrueFJet_MC_B->Write();
  
  
  ccResponseCJet_B->Write();
  ccResponseCJet_A->Write();
  ccResponseFJet_B->Write();
  ccResponseFJet_A->Write();

  ccCJet_A->Write();
  ccCJet_B->Write();
  ccFJet_A->Write();
  ccFJet_B->Write();
  
  ccFJet_JES_A->Write(); 
  ccCJet_JES_A->Write(); 
  ccFJet_JES_B->Write(); 
  ccCJet_JES_B->Write(); 
  ccFJet_JES_B_A->Write(); 
  ccCJet_JES_B_A->Write(); 
  
  
  outFile.Close();
  
}

