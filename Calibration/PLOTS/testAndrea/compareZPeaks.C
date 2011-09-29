#include "histoFunc.h"
#include "ntupleUtils.h"
#include "ntupleUtils.cc"

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

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooBreitWigner.h"
#include "RooCBShape.h"
#include "RooNumConvPdf.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <ctime>
#include <map>
#include <algorithm>
#include <math.h>
#include <vector>

TH1F* templateHisto;
TF1* templateFunc;
std::vector<double>* mydata;

std::map<std::string,TH1F*> h_mZ;
std::map<std::string,TH1F*> h_mZc;

RooRealVar* x;
std::map<std::string,RooDataSet*> d_mZ;
std::map<std::string,RooDataSet*> d_mZc;

void DrawZPeak(const std::string& category, const int& rebin);



void compareZPeaks()
{
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
  FillChain(ntu_DA,"inputDATA.txt");
  std::cout << "     DATA: " << std::setw(8) << ntu_DA->GetEntries() << " entries" << std::endl;
  
  TChain* ntu_MC = new TChain("ntu");
  FillChain(ntu_MC,"inputMC.txt");
  std::cout << "REFERENCE: " << std::setw(8) << ntu_MC->GetEntries() << " entries" << std::endl;
  
  if (ntu_DA->GetEntries() == 0 || ntu_MC->GetEntries() == 0 )
  {
    std::cout << "CHK-STB Error: At least one file is empty" << std::endl; 
    return;
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

  x = new RooRealVar("x","Mee",60.,120.);
    
  for(unsigned int i = 0; i < categories.size(); ++i)
  {
    std::string category = categories.at(i);
    
    std::string histoName = "h_mZ_"+category;
    h_mZ[category] = new TH1F(histoName.c_str(),"",240,60.,120.);
    std::string datasetName = "d_mZ_"+category;
    d_mZ[category] = new RooDataSet(datasetName.c_str(),"",RooArgSet(*x));
    
    histoName = "h_mZc_"+category;
    h_mZc[category] = new TH1F(histoName.c_str(),"",240,60.,120.);    
    datasetName = "d_mZc_"+category;
    d_mZc[category] = new RooDataSet(datasetName.c_str(),"",RooArgSet(*x));
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
    float scaleEB = 1.;
    float scaleEE = 1.;
    
    if( (isEB == 1) && (isEB2 == 1) )
    {
      h_mZc["EB-EB"] ->  Fill( mZ * sqrt(scaleEB*scaleEB) );
      (*x) = mZ * sqrt(scaleEB*scaleEB);
      d_mZc["EB-EB"] -> add(RooArgSet(*x));
    }    
    else if( (isEB == 0) && (isEB2 == 0) )
    {
      h_mZc["EE-EE"] ->  Fill( mZ * sqrt(scaleEE*scaleEE) );
      (*x) = mZ * sqrt(scaleEE*scaleEE);
      d_mZc["EE-EE"] -> add(RooArgSet(*x));
    }    
    else
    {
      h_mZc["EB-EE"] ->  Fill( mZ * sqrt(scaleEB*scaleEE) );
      (*x) = mZ * sqrt(scaleEB*scaleEE);
      d_mZc["EB-EE"] -> add(RooArgSet(*x));
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
      (*x) = mZ * sqrt(scaleEB*scaleEB);
      d_mZ["EB-EB"] -> add(RooArgSet(*x));
    }    
    else if( (isEB == 0) && (isEB2 == 0) )
    {
      h_mZ["EE-EE"] -> Fill( mZ * sqrt(scaleEE*scaleEE) );
      (*x) = mZ * sqrt(scaleEE*scaleEE);
      d_mZ["EE-EE"] -> add(RooArgSet(*x));
    }    
    else
    {
      h_mZ["EB-EE"] -> Fill( mZ * sqrt(scaleEB*scaleEE) );
      (*x) = mZ * sqrt(scaleEB*scaleEE);
      d_mZ["EB-EE"] -> add(RooArgSet(*x));
    }
  }
  
  
  
  // Drawings    
  DrawZPeak("EB-EB",2);
  DrawZPeak("EB-EE",2);
  DrawZPeak("EE-EE",4);
  
  
  //TF1* bw = new TF1("bw",breitWigner,60.,120.,3);
  //bw -> SetParameters(1,90.1876,2.4952);
  //bw -> SetNpx(10000);
  //bw -> SetLineColor(kRed);
  //bw -> SetLineWidth(1);
  //bw -> SetLineStyle(2);
  //bw -> SetParameter(0,1./bw->Integral(60.,120.));
  //
  //TF1* cb = new TF1("cb",crystalBallLow,60.,120.,5);
  //cb -> SetParameters(1,90.1876,1.449,1.178,1.934);
  //cb -> SetNpx(10000);
  //cb -> SetLineColor(kBlue);
  //cb -> SetLineWidth(1);
  //cb -> SetLineStyle(2);
  //cb -> SetParameter(0,1./cb->Integral(60.,120.));
  //  
  //TF1* bw_cb = new TF1("bw_cb",breitWigner_crystalBallLow,60.,120.,7);
  //bw_cb -> SetParameters(1.,90.1876,2.4952,0.159,1.449,1.178,1.934);
  //bw_cb -> SetNpx(10000);
  //bw_cb -> SetLineColor(kBlack);
  //bw_cb -> SetLineWidth(2);
  //bw_cb -> SetLineStyle(1);
  //bw_cb -> SetParameter(0,1./bw_cb->Integral(60.,120.));
  //        
  //TCanvas* c_bw = new TCanvas("c_bw","breit-wigner");
  //c_bw -> cd();
  //bw -> Draw();
  //cb -> Draw("same");
  //bw_cb -> Draw("same");
}



void DrawZPeak(const std::string& category, const int& rebin)
{
  TCanvas* c = new TCanvas(("c_mZ_"+category).c_str(),("mZ - "+category).c_str(),0,0,700,600);
  c -> cd();
  c -> SetGridx();
  c -> SetGridy();
  
  h_mZc[category] -> Scale( 1. * h_mZ[category]->GetEntries() / h_mZc[category]->GetEntries() );
  
  h_mZ[category] -> Rebin(rebin);
  h_mZ[category] -> GetXaxis() -> SetTitle(("m(SC_{1}+SC_{2}) - "+category).c_str());
  char axisTitle[50];
  sprintf(axisTitle,"events / %.2e GeV/c^{2}",h_mZ[category]->GetBinWidth(1));
  h_mZ[category] -> GetYaxis() -> SetTitle(axisTitle);
  h_mZ[category] -> GetYaxis() -> SetTitleOffset(1.5);
  h_mZ[category] -> SetLineWidth(2);
  h_mZ[category] -> SetLineColor(kRed+2);
  h_mZ[category] -> SetMarkerColor(kRed+2);
  h_mZ[category] -> SetMarkerSize(0.7);
  h_mZ[category] -> Draw("");
  gPad->Update();
  
  h_mZc[category] -> Rebin(rebin);
  h_mZc[category] -> GetXaxis() -> SetTitle(("m(SC_{1}+SC_{2}) - "+category).c_str());
  sprintf(axisTitle,"events / %.2e GeV/c^{2}",h_mZc[category]->GetBinWidth(1));
  h_mZc[category] -> GetYaxis() -> SetTitle(axisTitle);
  h_mZc[category] -> GetYaxis() -> SetTitleOffset(1.5);
  h_mZc[category] -> SetLineWidth(2);
  h_mZc[category] -> SetLineColor(kGreen+2);
  h_mZc[category] -> SetMarkerColor(kGreen+2);
  h_mZc[category] -> SetMarkerSize(0.7);
  h_mZc[category] -> Draw("sames");
  gPad->Update();
  
  double maximum = std::max(h_mZ[category]->GetMaximum(),h_mZc[category]->GetMaximum());
  h_mZ[category]  -> GetYaxis() -> SetRangeUser(0.,1.05*maximum);
  h_mZc[category] -> GetYaxis() -> SetRangeUser(0.,1.05*maximum);
  gPad->Update();
      
  TPaveStats* p_mZ = (TPaveStats*)(h_mZ[category]->GetListOfFunctions()->FindObject("stats"));  
  p_mZ->SetX1NDC(0.59);
  p_mZ->SetY1NDC(0.79);
  p_mZ->SetY2NDC(0.99);
  p_mZ->SetTextColor(kRed+2);
  p_mZ->Draw("same");
    
  TPaveStats* p_mZc = (TPaveStats*)(h_mZc[category]->GetListOfFunctions()->FindObject("stats"));  
  p_mZc->SetX1NDC(0.59);
  p_mZc->SetY1NDC(0.59);
  p_mZc->SetY2NDC(0.79);
  p_mZc->SetTextColor(kGreen+2);
  p_mZc->Draw("same");
  
  
  
  
  //RooRealVar* mass = new RooRealVar("mass","mass",90.1876,90.1876,90.1876);
  //RooRealVar* width = new RooRealVar("width","width",2.4952,2.4952,2.4952);
  //RooBreitWigner* bw = new RooBreitWigner("bw","bw",*x,*mass,*width);
  //
  //RooRealVar* deltam = new RooRealVar("deltam","#Deltam",0.,-10.,10.);
  //RooRealVar* sigmacb = new RooRealVar("sigmacb","#sigma_{CB}",1.,0.,10.);
  //RooRealVar* alpha = new RooRealVar("alpha","#alpha",1.,0.,10.);
  //RooRealVar* gamma = new RooRealVar("gamma","#gamma",1.,0.,10.);
  //RooCBShape* cb = new RooCBShape("cb","cb",*x,*deltam,*sigmacb,*alpha,*gamma);
  //
  //RooNumConvPdf* pdf = new RooNumConvPdf("pdf","BreitWigner*CrystalBall",*x,*bw,*cb);
  //pdf -> fitTo(*d_mZ[category],RooFit::PrintLevel(-1));
  //
  //
  //
  //TCanvas* c2 = new TCanvas(("c2_mZ_"+category).c_str(),("mZ - "+category).c_str(),0,0,700,600);
  //c2 -> cd();
  //c2 -> SetGridx();
  //c2 -> SetGridy();
  //  
  //RooPlot* rooPlot = x->frame();
  //d_mZ[category] -> plotOn(rooPlot,RooFit::MarkerSize(0.7),RooFit::MarkerColor(kRed+2));
  //pdf -> plotOn(rooPlot,RooFit::LineColor(kRed+2));
  //rooPlot -> Draw();
}
