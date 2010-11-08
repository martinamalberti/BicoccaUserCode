# include <iostream>
# include <cstdlib>
# include "TH2D.h"
# include "TFile.h"
# include <fstream>
# include <cmath>
# include <TCanvas.h>
# include <TPaveStats.h>

/// Macros for the visualization of the ABCD distributions obtained

int Print_Histo_Cut(TString Var1,TString Var2,TString Directory = "output/Histogram-Cut/")
{ 
  /// opening the ABCD analysis result results
  TString NameFile=Form("%s%s_%s.root",Directory.Data(),Var1.Data(),Var2.Data());
  std::cout << " NameFile = " << NameFile.Data() << std::endl;
  TFile* imput_cut=TFile::Open(NameFile);
 
  TH2D* histoA = (TH2D*)imput_cut->Get("HA");
  TH2D* histoB = (TH2D*)imput_cut->Get("HB");
  TH2D* histoC = (TH2D*)imput_cut->Get("HC");
  TH2D* histoD = (TH2D*)imput_cut->Get("HD");
  TH1D* KolA = (TH1D*)imput_cut->Get("Kolmogorov_pA");
  TH1D* KolB = (TH1D*)imput_cut->Get("Kolmogorov_pB");
  TH1D* KolC = (TH1D*)imput_cut->Get("Kolmogorov_pC");
  TH1D* KolD = (TH1D*)imput_cut->Get("Kolmogorov_pD");
  
  
  TCanvas * window = new TCanvas("window1","ABCD METHOD",1);
  TPad* padA = new TPad("padA","zone A",0.01,0.01,0.49,0.49);
  TPad* padB = new TPad("padB","zone B",0.01,0.51,0.49,0.99);
  TPad* padC = new TPad("padC","zone C",0.51,0.01,0.99,0.49);
  TPad* padD = new TPad("padD","zone D",0.51,0.51,0.99,0.99);
  padA->Draw();
  padB->Draw();
  padC->Draw();
  padD->Draw();
  padA->cd();
  padA->SetGrid();
  histoA->GetXaxis()->SetTitle(Var1);
  histoA->GetYaxis()->SetTitle(Var2);
  histoA->Draw("colz");
  padB->cd();
  padB->SetGrid();
  histoB->GetXaxis()->SetTitle(Var1);
  histoB->GetYaxis()->SetTitle(Var2);
  histoB->Draw("colz");
  padC->cd();
  padC->SetGrid();
  histoC->GetXaxis()->SetTitle(Var1);
  histoC->GetYaxis()->SetTitle(Var2);
  histoC->Draw("colz");
  padD->cd();
  padD->SetGrid();
  histoD->GetXaxis()->SetTitle(Var1);
  histoD->GetYaxis()->SetTitle(Var2);
  histoD->Draw("colz");
  

  gPad->Update();

  TPaveStats *stA = (TPaveStats*)histoA->FindObject("stats");
  std::cerr<<"<<<<<"<<stA->SetX1NDC(0.60)<<std::endl;
  stA->SetX2NDC(0.80);
  stA->SetY1NDC(0.60);
  stA->SetY2NDC(0.80);
  stA->SetTextColor(kBlue);
  TPaveStats *stB = (TPaveStats*)histoB->FindObject("stats");
  stB->SetX1NDC(0.60);
  stB->SetX2NDC(0.80);
  stB->SetY1NDC(0.80);
  stB->SetY2NDC(0.99);
  stB->SetTextColor(kRed);
  TPaveStats *stC = (TPaveStats*)histoC->FindObject("stats");
  stC->SetX1NDC(0.80);
  stC->SetX2NDC(0.99);
  stC->SetY1NDC(0.60);
  stC->SetY2NDC(0.80);
  stC->SetTextColor(kGreen);
  TPaveStats *stD = (TPaveStats*)histoD->FindObject("stats");
  stD->SetX1NDC(0.80);
  stD->SetX2NDC(0.99);
  stD->SetY1NDC(0.80);
  stD->SetY2NDC(0.99);
  stD->SetTextColor(kYellow);
  
  
  TCanvas * windowLeg = new TCanvas("windowLeg","ABCD METHOD",1);
//   histoA->Draw();
//   histoB->Draw("same");
//   histoC->Draw("same");
//   histoD->Draw("same");
  
/*  TPad* padALeg = new TPad("padALeg","zone A",);
  TPad* padBLeg = new TPad("padBLeg","zone B",0.01,0.51,0.49,0.99);
  TPad* padCLeg = new TPad("padCLeg","zone C",0.51,0.01,0.99,0.49);
  TPad* padDLeg = new TPad("padDLeg","zone D",0.51,0.51,0.99,0.99);
  padALeg->Draw();
  padBLeg->Draw();
  padCLeg->Draw();
  padDLeg->Draw();*/
  
//   padALeg->cd();

  windowLeg->cd();
  
  TPaveText *ptA = new TPaveText(0.2,0.2,0.3,0.3,"br");
  ptA->SetTextColor(kBlue);
  ptA->AddText("A");
  ptA->Draw("same");
  
//   padBLeg->cd();
  TPaveText *ptB = new TPaveText(0.2,0.7,0.3,0.8,"br");
  ptB->SetTextColor(kRed);
  ptB->AddText("B");
  ptB->Draw("same");
  
//   padCLeg->cd();
  TPaveText *ptC = new TPaveText(0.7,0.2,0.8,0.3,"br");
  ptC->SetTextColor(kGreen);
  ptC->AddText("C");
  ptC->Draw("same");
  
//   padDLeg->cd();
  TPaveText *ptD = new TPaveText(0.7,0.7,0.8,0.8,"br");
  ptD->SetTextColor(kYellow);
  ptD->AddText("D");
  ptD->Draw("same");
   
    
  TCanvas * win1 =new TCanvas ("win1","Histgramm",2);
  win1->cd();
  win1->SetGrid();
  histoA->SetMarkerColor(kBlue);
  histoA->SetMarkerStyle(8);
  histoA->SetMarkerSize(0.9);
  histoB->SetMarkerColor(kRed);
  histoB->SetMarkerStyle(8);
  histoB->SetMarkerSize(0.9);
  histoC->SetMarkerColor(kGreen);
  histoC->SetMarkerStyle(8);
  histoC->SetMarkerSize(0.9);
  histoD->SetMarkerColor(kYellow);
  histoD->SetMarkerStyle(8);
  histoD->SetMarkerSize(0.9);
  histoA->Draw();
  histoB->Draw("same");
  histoC->Draw("same");
  histoD->Draw("same");

  if (KolA){
   TString nameHisto = Form("Kolmogovor Histogram %s",Var1.Data());
   TCanvas * window2=new TCanvas("window2",nameHisto,1);
   window2->SetGrid();
   KolA->SetMarkerColor(kBlue);
   KolA->SetLineColor(kBlue);
   KolA->SetMarkerStyle(8);
   KolA->SetMarkerSize(1);
   KolA->SetLineWidth(1);
   KolA->GetXaxis()->SetTitle(Var1);
   KolB->SetLineColor(kRed);
   KolB->SetMarkerColor(kRed);
   KolB->SetMarkerStyle(8);
   KolB->SetMarkerSize(1);
   KolB->SetLineWidth(1);
   KolC->SetLineColor(kGreen);
   KolC->SetMarkerColor(kGreen);
   KolC->SetMarkerStyle(8);
   KolC->SetMarkerSize(1);
   KolC->SetLineWidth(1);
   KolD->SetLineColor(kYellow);
   KolD->SetMarkerColor(kYellow);
   KolD->SetMarkerStyle(8);
   KolD->SetLineWidth(1);
   KolD->SetMarkerSize(1);
   KolA->Draw();
   KolB->Draw("same");
   KolC->Draw("same");
   KolD->Draw("same");    
   double A = KolA->GetMaximum();
   double B = KolB->GetMaximum();
   double C = KolC->GetMaximum();
   double D = KolD->GetMaximum();
   double max;
   
   if(A>B && A>C && A>D)
    max = A;
   if(B>A && B>C && B>D)
    max = B;
   if(C>A && C>B && C>D)
    max = C;
   if(D>A && D>C && D>B)  
    max = D;
   std::cerr << max << std::endl;  
   KolA->SetAxisRange(0,1.2*max,"Y");
   
  }
  
  ///==== invert axis ====
  TString NameFile2 = Form("%s%s_%s.root",Directory.Data(),Var2.Data(),Var1.Data());
  std::cout << " NameFile 2 = " << NameFile2.Data() << std::endl;
  TFile* imput_cut2 = TFile::Open(NameFile2);
  if (imput_cut2){
   TH1D* KolA2 = (TH1D*) (imput_cut2->Get("Kolmogorov_pA")->Clone());
   if (KolA2) KolA2->SetName("Kolmogorov_pA_bis");
   TH1D* KolB2 = (TH1D*) (imput_cut2->Get("Kolmogorov_pB")->Clone());
   if (KolB2) KolB2->SetName("Kolmogorov_pB_bis");
   TH1D* KolC2 = (TH1D*) (imput_cut2->Get("Kolmogorov_pC")->Clone());
   if (KolC2) KolC2->SetName("Kolmogorov_pC_bis");
   TH1D* KolD2 = (TH1D*) (imput_cut2->Get("Kolmogorov_pD")->Clone());
   if (KolD2) KolD2->SetName("Kolmogorov_pD_bis");
   
   if (KolA2){
    TString nameHisto = Form("Kolmogovor Histogram %s",Var2.Data());
    TCanvas * window3 = new TCanvas("window3",nameHisto,1);
    window3->SetGrid();
    KolA2->SetMarkerColor(kBlue);
    KolA2->SetLineColor(kBlue);
    KolA2->SetMarkerStyle(8);
    KolA2->SetMarkerSize(1);
    KolA2->SetLineWidth(1);
    KolA2->GetXaxis()->SetTitle(Var2);
    KolB2->SetLineColor(kGreen);
    KolB2->SetMarkerColor(kGreen);
    KolB2->SetMarkerStyle(8);
    KolB2->SetMarkerSize(1);
    KolB2->SetLineWidth(1);
    KolC2->SetLineColor(kRed);
    KolC2->SetMarkerColor(kRed);
    KolC2->SetMarkerStyle(8);
    KolC2->SetMarkerSize(1);
    KolC2->SetLineWidth(1);
    KolD2->SetLineColor(kYellow);
    KolD2->SetMarkerColor(kYellow);
    KolD2->SetMarkerStyle(8);
    KolD2->SetLineWidth(1);
    KolD2->SetMarkerSize(1);
    KolA2->Draw();
    KolB2->Draw("same");
    KolC2->Draw("same");
    KolD2->Draw("same");    
    double A = KolA2->GetMaximum();
    double B = KolB2->GetMaximum();
    double C = KolC2->GetMaximum();
    double D = KolD2->GetMaximum();
    double max;
    
    if(A>B && A>C && A>D)
     max = A;
    if(B>A && B>C && B>D)
     max = B;
    if(C>A && C>B && C>D)
     max = C;
    if(D>A && D>C && D>B)  
     max = D;
    std::cerr << max << std::endl;  
    KolA2->SetAxisRange(0,1.2*max,"Y");
    
   }
   
  }
  
  return 0;
  
}  