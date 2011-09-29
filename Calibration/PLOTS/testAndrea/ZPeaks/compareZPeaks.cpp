// g++ -Wall -o compareZPeaks `root-config --cflags --glibs` setTDRStyle.cc ntupleUtils.cc compareZPeaks.cpp
#include "ntupleUtils.h"
#include "setTDRStyle.h"

#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TF1.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"
#include "TPaveStats.h"
#include "TLegend.h"
#include "TChain.h"
#include "TVirtualFitter.h"
#include "TFitResultPtr.h"
#include "TFitResult.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <cmath>

TH1F* templateHisto;
TF1* templateFunc;
std::vector<double>* mydata;

std::map<std::string,TH1F*> h_mZ;
std::map<std::string,TH1F*> h_mZc;

void DrawZPeak(const std::string& category, const int& rebin);



int main()
{
  setTDRStyle();
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetOptTitle(0); 
  gStyle->SetOptStat(1110); 
  gStyle->SetOptFit(1);
  
  
  
  // Get trees
  std::cout << std::endl;
  
  TChain* ntu_DA = new TChain("ntu");
  FillChain(ntu_DA,"inputDATA_Z.txt");
  std::cout << "     DATA: " << std::setw(8) << ntu_DA->GetEntries() << " entries" << std::endl;
  
  TChain* ntu_MC = new TChain("ntu");
  FillChain(ntu_MC,"inputMC_Z.txt");
  std::cout << "REFERENCE: " << std::setw(8) << ntu_MC->GetEntries() << " entries" << std::endl;
  
  if (ntu_DA->GetEntries() == 0 || ntu_MC->GetEntries() == 0 )
  {
    std::cout << ">>>compareZPeaks::Error: at least one file is empty" << std::endl; 
    return -1;
  }
  
  
    
  // Set branch addresses  
  int runId,isZ;
  float mZ;
  int isEB,isEB2;
  float scEta1,scEta2;
  
  ntu_DA->SetBranchAddress("runId",      &runId); 
  ntu_DA->SetBranchAddress("isZ",        &isZ);
  ntu_DA->SetBranchAddress("ele1ele2_m", &mZ);
  ntu_DA->SetBranchAddress("ele1_isEB",  &isEB);
  ntu_DA->SetBranchAddress("ele2_isEB",  &isEB2);
  ntu_DA->SetBranchAddress("ele1_scEta", &scEta1);
  ntu_DA->SetBranchAddress("ele2_scEta", &scEta2);
  
  ntu_MC->SetBranchAddress("runId",      &runId); 
  ntu_MC->SetBranchAddress("isZ",        &isZ);
  ntu_MC->SetBranchAddress("ele1ele2_m", &mZ);
  ntu_MC->SetBranchAddress("ele1_isEB",  &isEB);
  ntu_MC->SetBranchAddress("ele2_isEB",  &isEB2);
  ntu_MC->SetBranchAddress("ele1_scEta", &scEta1);
  ntu_MC->SetBranchAddress("ele2_scEta", &scEta2);
  
  
  
  // Define histograms
  std::vector<std::string> categories;
  categories.push_back("EB-EB");
  categories.push_back("EB-EE");
  categories.push_back("EE-EE");  
  
  for(unsigned int i = 0; i < categories.size(); ++i)
  {
    std::string category = categories.at(i);
    
    std::string histoName = "h_mZ_"+category;
    h_mZ[category] = new TH1F(histoName.c_str(),"",240,60.,120.);
    h_mZ[category] -> Sumw2();
    
    histoName = "h_mZc_"+category;
    h_mZc[category] = new TH1F(histoName.c_str(),"",240,60.,120.);    
    h_mZc[category] -> Sumw2();
  }
  
  
  
  // Loop over entries
  std::cout << std::endl;
  std::cout << "***** Read data from 1st sample *****" << std::endl;
  
  int nEntries = ntu_DA -> GetEntriesFast();
  for(int ientry = 0; ientry < nEntries; ++ientry)
  {
    if( ientry%100000 == 0 ) std::cout << "reading saved entry " << ientry << std::endl;
    ntu_DA -> GetEntry(ientry); 
    
    
    // selections
    if( isZ != 1 ) continue;
    if( fabs(scEta1) > 2.5    || fabs(scEta2) > 2.5  ) continue;
    if( fabs(scEta1) > 1.4442 && fabs(scEta1) < 1.56 ) continue;
    if( fabs(scEta2) > 1.4442 && fabs(scEta2) < 1.56 ) continue;
    
    
    // fill histograms
    //float scaleEB = 1.0073;
    //float scaleEE = 1.0336;
    float scaleEB = 1.05;
    float scaleEE = 1.05;
    
    if( (isEB == 1) && (isEB2 == 1) )
    {
      h_mZc["EB-EB"] ->  Fill( mZ * sqrt(scaleEB*scaleEB) );
    }    
    else if( (isEB == 0) && (isEB2 == 0) )
    {
      h_mZc["EE-EE"] ->  Fill( mZ * sqrt(scaleEE*scaleEE) );
    }    
    else
    {
      h_mZc["EB-EE"] ->  Fill( mZ * sqrt(scaleEB*scaleEE) );
    }
  }
  
  
  
  // Loop over entries
  std::cout << std::endl;
  std::cout << "***** Read data from 2nd sample *****" << std::endl;
  
  nEntries = ntu_MC -> GetEntriesFast();
  for(int ientry = 0; ientry < nEntries; ++ientry)
  {
    if( ientry%100000 == 0 ) std::cout << "reading saved entry " << ientry << std::endl;
    ntu_MC -> GetEntry(ientry); 
    
    
    // selections
    if( isZ != 1 ) continue;
    if( fabs(scEta1) > 2.5    || fabs(scEta2) > 2.5  ) continue;
    if( fabs(scEta1) > 1.4442 && fabs(scEta1) < 1.56 ) continue;
    if( fabs(scEta2) > 1.4442 && fabs(scEta2) < 1.56 ) continue;
    
    
    // fill histograms
    float scaleEB = 1.;
    float scaleEE = 1.;
    
    if( (isEB == 1) && (isEB2 == 1) )
    {
      h_mZ["EB-EB"] -> Fill( mZ * sqrt(scaleEB*scaleEB) );
    }    
    else if( (isEB == 0) && (isEB2 == 0) )
    {
      h_mZ["EE-EE"] -> Fill( mZ * sqrt(scaleEE*scaleEE) );
    }    
    else
    {
      h_mZ["EB-EE"] -> Fill( mZ * sqrt(scaleEB*scaleEE) );
    }
  }
  
  
  
  // Drawings    
  DrawZPeak("EB-EB",1);
  DrawZPeak("EB-EE",1);
  DrawZPeak("EE-EE",2);
}



void DrawZPeak(const std::string& category, const int& rebin)
{
  TCanvas* c = new TCanvas(("c_mZ_"+category).c_str(),("mZ - "+category).c_str(),0,0,600,600);
  c -> cd();
  c -> SetGridx();
  c -> SetGridy();
  //c -> SetLogy();
  
  c -> SetLeftMargin(0.15);
  c -> SetRightMargin(0.15);
  
  
  
  //-----------
  // draw peaks
  std::cout << ">>>compareZPeaks::DrawZPeak::draw peaks" << std::endl;
  
  h_mZc[category] -> Scale( 1. * h_mZ[category]->GetEntries() / h_mZc[category]->GetEntries() );

  char axisTitle[50];
  sprintf(axisTitle,"events / %.2e GeV/c^{2}",h_mZ[category]->GetBinWidth(1));
  h_mZ[category] -> Rebin(rebin);
  h_mZ[category] -> GetXaxis() -> SetLabelSize(0.03);
  h_mZ[category] -> GetXaxis() -> SetLabelFont(42);
  h_mZ[category] -> GetXaxis() -> SetTitleSize(0.03);
  h_mZ[category] -> GetXaxis() -> SetTitleOffset(1.5);
  h_mZ[category] -> GetXaxis() -> SetTitle(("m(SC_{1}+SC_{2}) - "+category).c_str());
  h_mZ[category] -> GetYaxis() -> SetLabelSize(0.03);
  h_mZ[category] -> GetYaxis() -> SetLabelFont(42);
  h_mZ[category] -> GetYaxis() -> SetTitleSize(0.03);
  h_mZ[category] -> GetYaxis() -> SetTitleOffset(2.0);
  h_mZ[category] -> GetYaxis() -> SetTitle(axisTitle);
  
  h_mZ[category] -> SetLineWidth(2);
  h_mZ[category] -> SetLineColor(kRed+2);
  h_mZ[category] -> SetMarkerColor(kBlack);
  h_mZ[category] -> SetMarkerStyle(20);
  h_mZ[category] -> SetMarkerSize(0.5);
  h_mZ[category] -> Draw("P");
  gPad->Update();
  
  sprintf(axisTitle,"events / %.2e GeV/c^{2}",h_mZc[category]->GetBinWidth(1));
  h_mZc[category] -> Rebin(rebin);
  h_mZc[category] -> GetXaxis() -> SetLabelSize(0.03);
  h_mZc[category] -> GetXaxis() -> SetLabelFont(42);
  h_mZc[category] -> GetXaxis() -> SetTitleSize(0.03);
  h_mZc[category] -> GetXaxis() -> SetTitleOffset(1.5);  
  h_mZc[category] -> GetXaxis() -> SetTitle(("m(SC_{1}+SC_{2}) - "+category).c_str());
  h_mZc[category] -> GetYaxis() -> SetLabelSize(0.03);
  h_mZc[category] -> GetYaxis() -> SetLabelFont(0.03);
  h_mZc[category] -> GetYaxis() -> SetTitleSize(0.03);
  h_mZc[category] -> GetYaxis() -> SetTitleOffset(2.0);  
  h_mZc[category] -> GetYaxis() -> SetTitle(axisTitle);
  
  h_mZc[category] -> SetLineWidth(2);
  h_mZc[category] -> SetLineColor(kGreen+2);
  h_mZc[category] -> SetMarkerColor(kBlack);
  h_mZc[category] -> SetMarkerStyle(20);
  h_mZc[category] -> SetMarkerSize(0.5);
  h_mZc[category] -> Draw("P,sames");
  gPad->Update();
  
  double maximum = std::max(h_mZ[category]->GetMaximum(),h_mZc[category]->GetMaximum());
  h_mZ[category]  -> GetYaxis() -> SetRangeUser(0.,1.05*maximum);
  h_mZc[category] -> GetYaxis() -> SetRangeUser(0.,1.05*maximum);
  gPad->Update();
  
  
  
  //----------
  // fit peaks
  std::cout << ">>>compareZPeaks::DrawZPeak::fit peaks" << std::endl;  
  std::string funcName = "bw_cb_" + category;
  TF1* bw_cb = new TF1(funcName.c_str(),breitWigner_crystalBallLow,70.,110.,7);
  bw_cb -> SetParameters(1000.,90.1876,2.4952,0.159,1.449,1.178,1.934);
  bw_cb -> FixParameter(1,90.1876);
  bw_cb -> FixParameter(2,2.4952);
  bw_cb -> SetParName(0,"N");
  bw_cb -> SetParName(3,"#Deltam");
  bw_cb -> SetParName(4,"#sigma_{CB}");
  bw_cb -> SetParName(5,"#alpha");
  bw_cb -> SetParName(6,"n");
  bw_cb -> SetNpx(10000);
  bw_cb -> SetLineColor(kRed+2);
  bw_cb -> SetLineWidth(2);
  bw_cb -> SetLineStyle(1);
  //bw_cb -> SetParameter(0,1./bw_cb->Integral(60.,120.));
  h_mZ[category] -> Fit(funcName.c_str(),"SR0Q+");
  bw_cb -> Draw("same");
  
  std::string c_funcName = "c_bw_cb_" + category;
  TF1* c_bw_cb = new TF1(c_funcName.c_str(),breitWigner_crystalBallLow,70.,110.,7);
  c_bw_cb -> SetParameters(1000.,90.1876,2.4952,0.159,1.449,1.178,1.934);
  c_bw_cb -> FixParameter(1,90.1876);
  c_bw_cb -> FixParameter(2,2.4952);
  c_bw_cb -> SetNpx(10000);
  c_bw_cb -> SetParName(0,"N");
  c_bw_cb -> SetParName(3,"#Deltam");
  c_bw_cb -> SetParName(4,"#sigma_{CB}");
  c_bw_cb -> SetParName(5,"#alpha");
  c_bw_cb -> SetParName(6,"n");
  c_bw_cb -> SetLineColor(kGreen+2);
  c_bw_cb -> SetLineWidth(2);
  c_bw_cb -> SetLineStyle(1);
  //c_bw_cb -> SetParameter(0,1./bw_cb->Integral(60.,120.));
  h_mZc[category] -> Fit(c_funcName.c_str(),"SR0Q+");  
  c_bw_cb -> Draw("same");
  
  
  
  // ------------------
  // get the statistics
  
  TPaveStats* p_mZ = (TPaveStats*)(h_mZ[category]->GetListOfFunctions()->FindObject("stats"));  
  p_mZ->SetX1NDC(0.69);
  p_mZ->SetX2NDC(0.99);
  p_mZ->SetY1NDC(0.69);
  p_mZ->SetY2NDC(0.99);
  p_mZ->SetTextColor(kRed+2);
  p_mZ->Draw("same");
    
  TPaveStats* p_mZc = (TPaveStats*)(h_mZc[category]->GetListOfFunctions()->FindObject("stats"));  
  p_mZc->SetX1NDC(0.69);
  p_mZc->SetX2NDC(0.99);
  p_mZc->SetY1NDC(0.39);
  p_mZc->SetY2NDC(0.69);
  p_mZc->SetTextColor(kGreen+2);
  p_mZc->Draw("same");
  
  
  
  // ---------------
  // print the plots
  
  c -> Print((category+".png").c_str(),"png");
  delete c;
}
