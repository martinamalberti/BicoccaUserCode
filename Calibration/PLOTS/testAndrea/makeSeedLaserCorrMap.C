#include "ntupleUtils.h"
#include "ntupleUtils.cc"

#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"
#include "TPaveStats.h"
#include "TLegend.h"
#include "TChain.h"

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



void makeSeedLaserCorrMap()
{
  // Get trees
  std::cout << std::endl;
  
  TChain* ntu_DA = new TChain("ntu");
  FillChain(ntu_DA,"inputDATA.txt");
  std::cout << "     DATA: " << std::setw(8) << ntu_DA->GetEntries() << " entries" << std::endl;
  
  if( ntu_DA->GetEntries() == 0 )
  {
    std::cout << "CHK-STB Error: At least one file is empty" << std::endl; 
    return;
  }
  
  
  
  // Set branch addresses  
  int isEB;
  int timeStampHigh;
  float seedLaserCorr, seedLaserAlpha;
  int seedIeta,seedIphi;
  int seedIx,seedIy,seedZside;
  float scLaserCorr;
  float scEta;

  ntu_DA->SetBranchAddress("timeStampHigh",       &timeStampHigh);
  ntu_DA->SetBranchAddress("ele1_isEB",           &isEB);
  ntu_DA->SetBranchAddress("ele1_seedLaserCorr",  &seedLaserCorr);
  ntu_DA->SetBranchAddress("ele1_seedLaserAlpha", &seedLaserAlpha);
  ntu_DA->SetBranchAddress("ele1_seedIeta",       &seedIeta);
  ntu_DA->SetBranchAddress("ele1_seedIphi",       &seedIphi);
  ntu_DA->SetBranchAddress("ele1_seedIx",         &seedIx);
  ntu_DA->SetBranchAddress("ele1_seedIy",         &seedIy);
  ntu_DA->SetBranchAddress("ele1_seedZside",      &seedZside);
  ntu_DA->SetBranchAddress("ele1_scLaserCorr",    &scLaserCorr);
  ntu_DA->SetBranchAddress("ele1_scEta",          &scEta);
  
  
  
  // Define histograms
  TH2F* h_seedOccupancy_EB  = new TH2F("h_seedOccupancy_EB",  "", 171,-85., 86.,361,0.,361.);
  TH2F* h_seedOccupancy_EEp = new TH2F("h_seedOccupancy_EEp", "", 101,  0.,101.,101,0.,101.);
  TH2F* h_seedOccupancy_EEm = new TH2F("h_seedOccupancy_EEm", "", 101,  0.,101.,101,0.,101.);
  
  TH1F* h_seedLaserCorrDistr_EB  = new TH1F("h_seedLaserCorrDistr_EB", "", 600,0.8,1.2);
  TH1F* h_seedLaserCorrDistr_EEp = new TH1F("h_seedLaserCorrDistr_EEp","", 600,0.8,1.2);
  TH1F* h_seedLaserCorrDistr_EEm = new TH1F("h_seedLaserCorrDistr_EEm","", 600,0.8,1.2);
  
  TProfile* p_seedLaserCorrAvg_vsEta     = new TProfile("p_seedLaserCorrAvg_vsEta",     "",  100, -2.5,  2.5,0.8,1.2);
  TProfile* p_seedLaserCorrAvg_vsEta_EB  = new TProfile("p_seedLaserCorrAvg_vsEta_EB",  "",   60, -1.5,  1.5,0.8,1.2);
  TProfile* p_seedLaserCorrAvg_vsEta_EEp = new TProfile("p_seedLaserCorrAvg_vsEta_EEp", "",   20,  1.5,  2.5,0.8,1.2);
  TProfile* p_seedLaserCorrAvg_vsEta_EEm = new TProfile("p_seedLaserCorrAvg_vsEta_EEm", "",   20, -2.5, -1.5,0.8,1.2);
  
  TProfile2D* p_seedLaserCorrMap_EB  = new TProfile2D("p_seedLaserCorrMap_EB","", 171,-85., 86.,361,0.,361., 0.8,1.2);
  TProfile2D* p_seedLaserCorrMap_EEp = new TProfile2D("p_seedLaserCorrMap_EEp","",101,  0.,101.,101,0.,101., 0.8,1.2);
  TProfile2D* p_seedLaserCorrMap_EEm = new TProfile2D("p_seedLaserCorrMap_EEm","",101,  0.,101.,101,0.,101., 0.8,1.2);
  
  
  
  // Loop over entries
  int nEntries = ntu_DA -> GetEntriesFast();
  for(int ientry = 0; ientry < nEntries; ++ientry)
  {
    if( ientry%100000 == 0 ) std::cout << "reading saved entry " << ientry << std::endl;
    ntu_DA -> GetEntry(ientry); 
    
    if( seedLaserAlpha < 1.5 ) continue;
    //if( scLaserCorr >  1.04 ) continue;
    //if( seedLaserCorr == 1. ) continue;
    //if( timeStampHigh > 1303862400 ) continue;
        
//    if( seedZside < 0 )
//      if( (seedIx > 20 ) && (seedIx < 50) && (seedIy > 85) && (seedIy < 92) ) continue;
//    if( seedZside == -1 )
//      if( (seedIx > 35 ) && (seedIx < 55) && (seedIy > 80) && (seedIy < 87) ) continue;
//    if( seedZside > 0 )
//      if( (seedIx > 65 ) && (seedIx < 77) && (seedIy > 33) && (seedIy < 57) ) continue;
//    if( seedZside > 0 )
//      if( (seedIx > 75 ) && (seedIx < 93) && (seedIy > 18) && (seedIy < 37) ) continue;
    
    

    p_seedLaserCorrAvg_vsEta -> Fill(scEta,seedLaserCorr);
    
    if( isEB == 1 )
    {
      h_seedOccupancy_EB -> Fill(seedIeta,seedIphi);
      h_seedLaserCorrDistr_EB -> Fill(seedLaserCorr);
      p_seedLaserCorrMap_EB -> Fill(seedIeta,seedIphi,seedLaserCorr);
      p_seedLaserCorrAvg_vsEta_EB -> Fill(scEta,seedLaserCorr);
    }
    
    else
    {
      if( seedZside > 0)
      {
        h_seedOccupancy_EEp -> Fill(seedIx,seedIy);
        h_seedLaserCorrDistr_EEp -> Fill(seedLaserCorr);
        p_seedLaserCorrMap_EEp -> Fill(seedIx,seedIy,seedLaserCorr);
        p_seedLaserCorrAvg_vsEta_EEp -> Fill(scEta,seedLaserCorr);
      }
      else
      {
        h_seedOccupancy_EEm -> Fill(seedIx,seedIy);
        h_seedLaserCorrDistr_EEm -> Fill(seedLaserCorr);
        p_seedLaserCorrMap_EEm -> Fill(seedIx,seedIy,seedLaserCorr);
        p_seedLaserCorrAvg_vsEta_EEm -> Fill(scEta,seedLaserCorr);
      }
    }
    
  }
  
  

  TCanvas* c_seedOccupancy = new TCanvas("c_seedOccupancy","seedOccupancy",100,100,1350,500);
  c_seedOccupancy -> Divide(3,1);
  
  c_seedOccupancy -> cd(1);
  h_seedOccupancy_EB -> GetXaxis() -> SetTitle("i#eta");
  h_seedOccupancy_EB -> GetYaxis() -> SetTitle("i#phi");
  h_seedOccupancy_EB -> GetZaxis() -> SetTitle("events");
  h_seedOccupancy_EB -> Draw("COLZ");
  
  c_seedOccupancy -> cd(2);
  h_seedOccupancy_EEp -> GetXaxis() -> SetTitle("ix");
  h_seedOccupancy_EEp -> GetYaxis() -> SetTitle("iy");
  h_seedOccupancy_EEp -> GetZaxis() -> SetTitle("events");
  h_seedOccupancy_EEp -> Draw("COLZ");
  
  c_seedOccupancy -> cd(3);
  h_seedOccupancy_EEm -> GetXaxis() -> SetTitle("ix");
  h_seedOccupancy_EEm -> GetYaxis() -> SetTitle("iy");
  h_seedOccupancy_EEm -> GetZaxis() -> SetTitle("events");
  h_seedOccupancy_EEm -> Draw("COLZ");  
  
  
  
  TCanvas* c_seedLaserCorr_EB = new TCanvas("c_seedLaserCorr_EB","seedLaserCorr_EB",100,100,1350,500);
  c_seedLaserCorr_EB -> Divide(3,1);
  
  c_seedLaserCorr_EB -> cd(1);
  p_seedLaserCorrMap_EB -> GetXaxis() -> SetTitle("i#eta");
  p_seedLaserCorrMap_EB -> GetYaxis() -> SetTitle("i#phi");
  p_seedLaserCorrMap_EB -> GetZaxis() -> SetTitle("<seedLaserCorr>");
  p_seedLaserCorrMap_EB -> GetZaxis() -> SetRangeUser(0.95,1.1);
  p_seedLaserCorrMap_EB -> Draw("COLZ");
  
  c_seedLaserCorr_EB -> cd(2);
  gPad -> SetLogy();
  h_seedLaserCorrDistr_EB -> GetXaxis() -> SetTitle("seedLaserCorr");
  h_seedLaserCorrDistr_EB -> Draw();
  
  c_seedLaserCorr_EB -> cd(3);  
  p_seedLaserCorrAvg_vsEta_EB -> GetXaxis() -> SetTitle("#eta");
  p_seedLaserCorrAvg_vsEta_EB -> GetYaxis() -> SetTitle("<seedLaserCorr>");
  p_seedLaserCorrAvg_vsEta_EB -> GetYaxis() -> SetRangeUser(1.0,1.1);
  p_seedLaserCorrAvg_vsEta_EB -> Draw("COLZ");
  
  
  
  TCanvas* c_seedLaserCorr_EEp = new TCanvas("c_seedLaserCorr_EEp","seedLaserCorr_EEp",100,100,1350,500);
  c_seedLaserCorr_EEp -> Divide(3,1);
  
  c_seedLaserCorr_EEp -> cd(1);
  p_seedLaserCorrMap_EEp -> GetXaxis() -> SetTitle("ix");
  p_seedLaserCorrMap_EEp -> GetYaxis() -> SetTitle("iy");
  p_seedLaserCorrMap_EEp -> GetZaxis() -> SetTitle("<seedLaserCorr>");
  p_seedLaserCorrMap_EEp -> GetZaxis() -> SetRangeUser(0.95,1.15);
  p_seedLaserCorrMap_EEp -> Draw("COLZ");
  
  c_seedLaserCorr_EEp -> cd(2);
  gPad -> SetLogy();
  h_seedLaserCorrDistr_EEp -> GetXaxis() -> SetTitle("seedLaserCorr");
  h_seedLaserCorrDistr_EEp -> Draw();
  
  c_seedLaserCorr_EEp -> cd(3);  
  p_seedLaserCorrAvg_vsEta_EEp -> GetXaxis() -> SetTitle("#eta");
  p_seedLaserCorrAvg_vsEta_EEp -> GetYaxis() -> SetTitle("<seedLaserCorr>");
  p_seedLaserCorrAvg_vsEta_EEp -> GetYaxis() -> SetRangeUser(1.0,1.1);
  p_seedLaserCorrAvg_vsEta_EEp -> Draw("COLZ");
  
  
  
  TCanvas* c_seedLaserCorr_EEm = new TCanvas("c_seedLaserCorr_EEm","seedLaserCorr_EEm",100,100,1350,500);
  c_seedLaserCorr_EEm -> Divide(3,1);
  
  c_seedLaserCorr_EEm -> cd(1);
  p_seedLaserCorrMap_EEm -> GetXaxis() -> SetTitle("ix");
  p_seedLaserCorrMap_EEm -> GetYaxis() -> SetTitle("iy");
  p_seedLaserCorrMap_EEm -> GetZaxis() -> SetTitle("<seedLaserCorr>");
  p_seedLaserCorrMap_EEm -> GetZaxis() -> SetRangeUser(0.95,1.15);
  p_seedLaserCorrMap_EEm -> Draw("COLZ");
  
  c_seedLaserCorr_EEm -> cd(2);
  gPad -> SetLogy();
  h_seedLaserCorrDistr_EEm -> GetXaxis() -> SetTitle("seedLaserCorr");
  h_seedLaserCorrDistr_EEm -> Draw();
  
  c_seedLaserCorr_EEm -> cd(3);  
  p_seedLaserCorrAvg_vsEta_EEm -> GetXaxis() -> SetTitle("#eta");
  p_seedLaserCorrAvg_vsEta_EEm -> GetYaxis() -> SetTitle("<seedLaserCorr>");
  p_seedLaserCorrAvg_vsEta_EEm -> GetYaxis() -> SetRangeUser(1.0,1.1);
  p_seedLaserCorrAvg_vsEta_EEm -> Draw("COLZ");  
  
  
  
  TCanvas* c_seedLaserCorrAvgMaps = new TCanvas("c_seedLaserCorrAvgMaps","seedLaserCorrAvgMaps",100,100,1350,500);  
  c_seedLaserCorrAvgMaps -> Divide(3);
  
  c_seedLaserCorrAvgMaps -> cd(1);
  p_seedLaserCorrMap_EB -> Draw("COLZ");
  c_seedLaserCorrAvgMaps -> cd(2);
  p_seedLaserCorrMap_EEp -> Draw("COLZ");  
  c_seedLaserCorrAvgMaps -> cd(3);
  p_seedLaserCorrMap_EEm -> Draw("COLZ");
  
  
  
  TCanvas* c_seedLaserCorrAvg_vsEta = new TCanvas("c_seedLaserCorrAvg_vsEta","seedLaserCorrAvg_vsEta");
  c_seedLaserCorrAvg_vsEta -> cd();
  
  p_seedLaserCorrAvg_vsEta -> GetXaxis() -> SetTitle("#eta");
  p_seedLaserCorrAvg_vsEta -> GetYaxis() -> SetTitle("<seedLaserCorr>");
  p_seedLaserCorrAvg_vsEta -> Draw();
  
  
  
  TFile* o = new TFile("makeSeedLaserCorrMapHistos.root","RECREATE");
  o -> cd();
  
  o -> Close();
}