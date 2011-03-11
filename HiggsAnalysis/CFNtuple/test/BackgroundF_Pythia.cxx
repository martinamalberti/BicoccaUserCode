


#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#endif



#if !defined(__CINT__) || defined(__MAKECINT__)
#include <iostream>
using std::cout;
using std::endl;

#include "TRandom.h"
#include "TH1D.h"
#include "treeReader.h"
#include "hFactory.h"
#include "hFunctions.h"
#include "stdHisto.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TMinuit.h"
#include "Math/GenVector/VectorUtil.h"
#include "TRandom3.h"
#include <time.h>
#include <sstream>
#include "CFNtuple.h"


#include "math.h"

#endif

//==============================================================================
// Global definitions
//==============================================================================

//==============================================================================
// Unfolding for C-F jet analysis
//==============================================================================

//void PurityF_Pythia()
{ 
#include "utils.cxx"

      Float_t lowEdge[11] = {18,22,27,35,45,57,72,90,120,150,1000};
      int NBIN = 10;
      int tresholdBIN= 5;
//    Float_t lowEdge[10] = {18,22,27,35,45,57,72,90,120,150};
//   int NBIN = 9;

//     Float_t lowEdge[8] = {35,45,57,72,90,120,150,1000};
//     int NBIN = 7;  
//     int tresholdBIN= 2;
  
  int debug = 0;
  double Threshold_G_FJet = 57;
  double Threshold_G_CJet = 57;
  
  TH2D* hResponseMatrixFJetPythia15 = new TH2D ("hResponseMatrixFJetPythia15", "Response Matrix FJet Pythia15",NBIN,lowEdge,NBIN,lowEdge);
  TH2D* hResponseMatrixFJetPythia30 = new TH2D ("hResponseMatrixFJetPythia30", "Response Matrix FJet Pythia30",NBIN,lowEdge,NBIN,lowEdge);

  // TFile FileTrain1PythiaF("/home/toliman/Dropbox/QCD_CF/Unfolding/NewNtuple/cut_18_35/qcd_15_pythia.root","READ");
   TFile FileTrain1PythiaF("~/Dropbox/QCD_CF/Unfolding/NewNtuple/cut_18_35/qcd_15_pythia.root","READ");
   TFile FileTrain2PythiaF("~/Dropbox/QCD_CF/Unfolding/NewNtuple/cut_18_35/qcd_30_pythia.root","READ");
  
   
//    TFile FileTrain1PythiaF("../input/BO/rootfiles/mc/qcd_15_pythia.root","READ");
//    TFile FileTrain2PythiaF("../input/BO/rootfiles/mc/qcd_30_pythia.root","READ");
   
     
  TTree* TreeTrain1PythiaF = (TTree*) FileTrain1PythiaF.Get("AnaHiggs");
  TTree* TreeTrain2PythiaF = (TTree*) FileTrain2PythiaF.Get("AnaHiggs");

  
  Float_t G_FJet_Pt; //~~~~ had
  Float_t S_FJet_Pt; //~~~~ reco

  Float_t G_CJet_Pt; //~~~~ had
  Float_t S_CJet_Pt; //~~~~ reco
  
  Float_t G_FJet_Phi; //~~~~ had
  Float_t S_FJet_Phi; //~~~~ reco

  Float_t G_CJet_Phi; //~~~~ had
  Float_t S_CJet_Phi; //~~~~ reco
  
  Float_t G_FJet_Eta; //~~~~ had
  Float_t S_FJet_Eta; //~~~~ reco

  Float_t G_CJet_Eta; //~~~~ had
  Float_t S_CJet_Eta; //~~~~ reco
 
 
 TreeTrain1PythiaF->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt);
 TreeTrain1PythiaF->SetBranchAddress("S_FJet_Pt",&S_FJet_Pt);
 TreeTrain1PythiaF->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt);
 TreeTrain1PythiaF->SetBranchAddress("S_CJet_Pt",&S_CJet_Pt);

 TreeTrain2PythiaF->SetBranchAddress("G_FJet_Pt",&G_FJet_Pt);
 TreeTrain2PythiaF->SetBranchAddress("S_FJet_Pt",&S_FJet_Pt);
 TreeTrain2PythiaF->SetBranchAddress("G_CJet_Pt",&G_CJet_Pt);
 TreeTrain2PythiaF->SetBranchAddress("S_CJet_Pt",&S_CJet_Pt);
 
 TreeTrain1PythiaF->SetBranchAddress("G_FJet_Phi",&G_FJet_Phi);
 TreeTrain1PythiaF->SetBranchAddress("S_FJet_Phi",&S_FJet_Phi);
 TreeTrain1PythiaF->SetBranchAddress("G_CJet_Phi",&G_CJet_Phi);
 TreeTrain1PythiaF->SetBranchAddress("S_CJet_Phi",&S_CJet_Phi);

 TreeTrain2PythiaF->SetBranchAddress("G_FJet_Phi",&G_FJet_Phi);
 TreeTrain2PythiaF->SetBranchAddress("S_FJet_Phi",&S_FJet_Phi);
 TreeTrain2PythiaF->SetBranchAddress("G_CJet_Phi",&G_CJet_Phi);
 TreeTrain2PythiaF->SetBranchAddress("S_CJet_Phi",&S_CJet_Phi);
 
 TreeTrain1PythiaF->SetBranchAddress("G_FJet_Eta",&G_FJet_Eta);
 TreeTrain1PythiaF->SetBranchAddress("S_FJet_Eta",&S_FJet_Eta);
 TreeTrain1PythiaF->SetBranchAddress("G_CJet_Eta",&G_CJet_Eta);
 TreeTrain1PythiaF->SetBranchAddress("S_CJet_Eta",&S_CJet_Eta);

 TreeTrain2PythiaF->SetBranchAddress("G_FJet_Eta",&G_FJet_Eta);
 TreeTrain2PythiaF->SetBranchAddress("S_FJet_Eta",&S_FJet_Eta);
 TreeTrain2PythiaF->SetBranchAddress("G_CJet_Eta",&G_CJet_Eta);
 TreeTrain2PythiaF->SetBranchAddress("S_CJet_Eta",&S_CJet_Eta);



  cout << "=================================================================================" << endl;
  cout << "====================== Filling Matrix with Matched Jets ========================" << endl;
  
   double DR_F_max =  0.2;
             
  for (Int_t iEvt= 0; iEvt<TreeTrain1PythiaF->GetEntries(); iEvt++) {
    TreeTrain1PythiaF->GetEntry(iEvt);
        if (S_FJet_Pt>18 && G_FJet_Pt>18 && S_CJet_Pt>35 && G_CJet_Pt>35)  {
	double Delta_R = deltaR(S_FJet_Eta,S_FJet_Phi,G_FJet_Eta,G_FJet_Phi);
	if (Delta_R<DR_F_max){
        	if (debug ==1) std::cerr<<"Delta_R = "<<Delta_R << std::endl;
        	hResponseMatrixFJetPythia15->Fill(S_FJet_Pt, G_FJet_Pt);
	}
      }
   }
   
  
  for (Int_t iEvt= 0; iEvt<TreeTrain2PythiaF->GetEntries(); iEvt++) {
    TreeTrain2PythiaF->GetEntry(iEvt);
//       if (S_FJet_Pt>0 && G_FJet_Pt>18 && S_CJet_Pt>0 && G_CJet_Pt>35)  {
      if (S_FJet_Pt>18 && G_FJet_Pt>18 && S_CJet_Pt>35 && G_CJet_Pt>35)  {
//      if (S_FJet_Pt>18 && G_FJet_Pt>18 && S_CJet_Pt>35 && G_CJet_Pt>35)  {
	
	double Delta_R = deltaR(S_FJet_Eta,S_FJet_Phi,G_FJet_Eta,G_FJet_Phi);
	if (Delta_R<DR_F_max){
	if (debug ==1) std::cerr<<"Delta_R = "<<Delta_R << std::endl;
//    if (S_FJet_Pt>0 && G_FJet_Pt>0)  { 
	hResponseMatrixFJetPythia30->Fill(S_FJet_Pt, G_FJet_Pt);
	}
     }
  }
  
  
  cout << "=================================================================================" << endl;
  cout << "======================== Filling Histos with All Jets ==========================" << endl;
  
     
 
 TH1D* BackNon_matchedF_P = new TH1D ("BackNon_matchedF_P", "BackNon_matchedF_P",NBIN,lowEdge);
 TH1D* BackGroundF_Pythia = new TH1D ("BackgroundF_P", "BackgroundF_P",NBIN,lowEdge);
 
 //Background
  for (Int_t iEvt= 0; iEvt<TreeTrain1PythiaF->GetEntries(); iEvt++) {
   TreeTrain1PythiaF->GetEntry(iEvt);
//    if(S_FJet_Pt>18 && S_CJet_Pt>35 && G_FJet_Pt>18 && G_CJet_Pt>35 && S_FJet_Pt>=Threshold_G_FJet){	//ATTENZIONE! QUI STIAMO FACENDO LA TRESHOLD DI S_JET ! 
    if(S_FJet_Pt>18 && S_CJet_Pt>35 && S_FJet_Pt<Threshold_G_FJet){
      BackNon_matchedF_P->Fill(S_FJet_Pt);
     }
 }
 
  for (Int_t iEvt= 0; iEvt<TreeTrain2PythiaF->GetEntries(); iEvt++) {
   TreeTrain2PythiaF->GetEntry(iEvt);
   
//    if(S_FJet_Pt>18 && S_CJet_Pt>35 && G_FJet_Pt>18 && G_CJet_Pt>35 && S_FJet_Pt>=Threshold_G_FJet){	//ATTENZIONE! QUI STIAMO FACENDO LA TRESHOLD DI S_JET !
    if(S_FJet_Pt>18 && S_CJet_Pt>35 && S_FJet_Pt>=Threshold_G_FJet){	//ATTENZIONE! QUI STIAMO FACENDO LA TRESHOLD DI S_JET !
      BackNon_matchedF_P->Fill(S_FJet_Pt);
     }
 }
  
  

          
  cout << "==================================== Background =====================================" << endl;
 
  
  //for forward Jets smaller than 57
  for (int iBinX = 0; iBinX<tresholdBIN; iBinX++){
    double column = 0;
    for (int iBinY = 0; iBinY<NBIN; iBinY++){
       column = column + hResponseMatrixFJetPythia15->GetBinContent(iBinX+1,iBinY+1);
    }
    double N_all_back = BackNon_matchedF_P->GetBinContent(iBinX +1);
    std::cout<<"Column = " <<column <<endl;
    BackGroundF_Pythia->SetBinContent(iBinX+1, 1- column/N_all_back);
    BackGroundF_Pythia->SetBinError(iBinX+1, (sqrt(1./column)+sqrt(1./N_all_back))*column/N_all_back);
  }
  
    //for forward Jets greater than 57
  for (int iBinX = tresholdBIN; iBinX<NBIN; iBinX++){
    double column = 0;
    for (int iBinY = 0; iBinY<NBIN; iBinY++){
       column = column + hResponseMatrixFJetPythia30->GetBinContent(iBinX+1,iBinY+1);
    }
    double N_all_back = BackNon_matchedF_P->GetBinContent(iBinX +1);
    std::cout<<"Column = " <<column <<endl;
    BackGroundF_Pythia->SetBinContent(iBinX+1, 1- column/N_all_back);
    BackGroundF_Pythia->SetBinError(iBinX+1, (sqrt(1./column)+sqrt(1./N_all_back))*column/N_all_back);
  }
  
  
  
  
//  Purity15->Add(Purity30);
  
  TCanvas* cPurityF_P = new TCanvas("cBackgroundF_P","cBackgroundF_P",600,600);  
    
  TFile* _file0 = new TFile("~/Dropbox/QCD_CF/F_hist_pB.root","READ");

  TH1F* Bop = (TH1F*) _file0->Get("hist_p");
  Bop->GetYaxis()->SetRangeUser(0.,1.0);
  Bop->SetMarkerColor(kRed);
  Bop->SetLineColor(kRed);
  Bop->SetMarkerSize(1);
  Bop->SetStats(0);
  Bop->Draw();

 
  std::cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << std::endl; 
  for (int iBinY = 0; iBinY<NBIN; iBinY++){
   std::cout << " bin[" << iBinY << "] = " << Bop->GetBinContent(iBinY+1) << " : " << BackGroundF_Pythia->GetBinContent(iBinY+1) << std::endl;
  }

  BackGroundF_Pythia->GetYaxis()->SetRangeUser(0.,1.0);
//  BackGroundF_Pythia->GetXaxis()->SetRangeUser(18.,150.);
  BackGroundF_Pythia->SetTitle("");
  BackGroundF_Pythia->SetMarkerStyle(21);
//   BackGroundF_Pythia->SetMarkerSize(1);
  BackGroundF_Pythia->GetYaxis()->SetTitle("Purity");
  BackGroundF_Pythia->GetXaxis()->SetTitle("forward jet p_{T} [GeV/c]");
  BackGroundF_Pythia->SetStats(0);
  BackGroundF_Pythia->SetMarkerColor(kBlue);
  BackGroundF_Pythia->SetLineColor(kBlue);
  BackGroundF_Pythia->SetMarkerSize(1);
  BackGroundF_Pythia->SetMarkerStyle(20);
//   BackGroundF_Pythia->Draw("same");
  BackGroundF_Pythia->DrawCopy("same");
  
 /* Purity30->SetMarkerColor(kGreen);
  Purity30->SetLineColor(kGreen);
  Purity30->SetMarkerSize(2);
  Purity30->SetMarkerStyle(20);
  Purity30->Draw("same");
  Purity30->DrawCopy("same");*/

   TLegend* leg = new TLegend(0.6,0.7,0.9,0.9,"","brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   
   leg->AddEntry(BackGroundF_Pythia,"Background", "PL");
//    leg->AddEntry(Purity30,"Purity 30", "PL");
   leg->AddEntry(Bop,"Bo Background", "PL");
   leg->Draw();

  
}

