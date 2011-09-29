#ifdef _MAKECINT_
#pragma link C++ class map<int,TH1F*>+;
#endif

#include "ntupleUtils.h"
#include "ntupleUtils.cc"

#include "TROOT.h"
#include "TStyle.h"
#include "TH1.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TTree.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <ctime>
#include <map>
#include <algorithm>
#include <math.h>
#include <vector>

float timeLapse = 24.; // in hours
int t1 = 1267401600; //  1 Mar 2010
int t2 = 1325289600; // 31 Dec 2011 



void trisCheckLaserCorr(Char_t* EBEE = 0,
                        Int_t evtsPerPoint = 0,
                        std::string dayMin = "",
                        std::string dayMax = "")
{
  // Set style options
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetOptTitle(0); 
  gStyle->SetOptStat(1110); 
  gStyle->SetOptFit(1); 
  
  
  
  // Check qualifiers
  if ( strcmp(EBEE,"EB")!=0 && strcmp(EBEE,"EE")!=0 )
  {
    std::cout << "CHK-STB Error: unknown partition " << EBEE << std::endl;
    std::cout << "CHK-STB Select either EB or EE ! " << std::endl;
    return;
  }  
  
  
  
  // Get trees
  std::cout << std::endl;
  
  TChain* ntu_DA = new TChain("ntu");
  FillChain(ntu_DA,"inputDATA.txt");
  std::cout << "     DATA: " << std::setw(8) << ntu_DA->GetEntries() << " entries" << std::endl;
    
  if (ntu_DA->GetEntries() == 0 )
  {
    std::cout << "CHK-STB Error: At least one file is empty" << std::endl; 
    return;
  }
  
  
  
  // Set branch addresses
  int timeStampHigh,runId;
  float seedLaserCorr,scLaserCorr,scEta;
  
  ntu_DA->SetBranchAddress("timeStampHigh",      &timeStampHigh);
  ntu_DA->SetBranchAddress("runId",              &runId);
  ntu_DA->SetBranchAddress("ele1_seedLaserCorr", &seedLaserCorr);
  ntu_DA->SetBranchAddress("ele1_scLaserCorr",   &scLaserCorr);
  ntu_DA->SetBranchAddress("ele1_scEta",         &scEta);
  
  if( (dayMax != "") && (dayMin != "") )
  {
    int day,month,year;
    
    std::stringstream ssMin(dayMin);
    ssMin >> day >> month >> year;
    tm tmMin;
    tmMin.tm_sec = 0;
    tmMin.tm_min = 0;
    tmMin.tm_hour = 0;
    tmMin.tm_mday = day;
    tmMin.tm_mon = month-1;
    tmMin.tm_year = year-1900;
    t1 = timegm(&tmMin);
    
    std::stringstream ssMax(dayMax);
    ssMax >> day >> month >> year;    
    tm tmMax;
    tmMax.tm_sec = 0;
    tmMax.tm_min = 0;
    tmMax.tm_hour = 0;
    tmMax.tm_mday = day;
    tmMax.tm_mon = month-1;
    tmMax.tm_year = year-1900;
    t2 = timegm(&tmMax);
  }
  
  
  
  
  
  
  // Loop and sort events
  std::cout << std::endl;
  std::cout << "***** Sort events and define bins *****" << std::endl;
  
  int nEntries = ntu_DA -> GetEntriesFast(); 
  int nSavePts = 0; 
  std::vector<bool> isSavedEntries(nEntries);
  std::vector<Sorter> sortedEntries;
  std::vector<int> timeStampFirst;
    
  for(int ientry = 0; ientry < nEntries; ++ientry)
  {
    ntu_DA -> GetEntry(ientry);
    isSavedEntries.at(ientry) = false;
    
    // save only what is needed for the analysis!!!
    if (strcmp(EBEE,"EB")==0 && fabs(scEta) > 1.45) continue;                       // barrel
    if (strcmp(EBEE,"EE")==0 && (fabs(scEta) < 1.47 || fabs(scEta)>2.7 )) continue; // endcap
    if( timeStampHigh < t1 ) continue;
    if( timeStampHigh > t2 ) continue;
        
    isSavedEntries.at(ientry) = true;
        
    Sorter dummy;
    dummy.time = timeStampHigh;
    dummy.entry = ientry;
    sortedEntries.push_back(dummy);
    
    nSavePts++; 
  }
  std::sort(sortedEntries.begin(),sortedEntries.end(),Sorter());
  std::cout << "Data sorted in " << EBEE << " - " << nSavePts << " events" << std::endl;
  
  
  
  // Loop and define bins
  
  // "wide" bins - find events with time separation bigger than 1 day
  int nWideBins = 1;
  std::vector<int> wideBinEntryMax;
  int timeStampOld = -1;
  
  wideBinEntryMax.push_back(0);  
  for(int iSaved = 0; iSaved < nSavePts; ++iSaved)
  {
    if( iSaved%100000 == 0 ) std::cout << "reading saved entry " << iSaved << std::endl;
    ntu_DA->GetEntry(sortedEntries[iSaved].entry);  
    
    if( iSaved == 0 )
    {
      timeStampOld = timeStampHigh;
      continue;
    }
    
    if( (timeStampHigh-timeStampOld)/3600. > timeLapse )
    {
      ++nWideBins;
      wideBinEntryMax.push_back(iSaved-1);
    }
    
    timeStampOld = timeStampHigh;
  }
  wideBinEntryMax.push_back(nSavePts);
  
  // bins with approximatively evtsPerPoint events per bin
  int nBins = 0;
  int nBins_runId = 0;
  std::vector<int> binEntryMax;
  std::vector<int> runIds;
  
  binEntryMax.push_back(0);
  for(int wideBin = 0; wideBin < nWideBins; ++wideBin)
  {
    int nTempBins = std::max(1,int( (wideBinEntryMax.at(wideBin+1)-wideBinEntryMax.at(wideBin))/evtsPerPoint ));
    int nTempBinEntries = int( (wideBinEntryMax.at(wideBin+1)-wideBinEntryMax.at(wideBin))/nTempBins );
    
    for(int tempBin = 0; tempBin < nTempBins; ++tempBin)
    {
      ++nBins;
      if( tempBin < nTempBins - 1 )
        binEntryMax.push_back( wideBinEntryMax.at(wideBin) + (tempBin+1)*nTempBinEntries );
      else
        binEntryMax.push_back( wideBinEntryMax.at(wideBin+1) );
    }
  }
  
  std::cout << "nBins = " << nBins << std::endl;
  //for(int bin = 0; bin < nBins; ++bin)
  //  std::cout << "bin: " << bin
  //            << "   entry min: " << setw(6) << binEntryMax.at(bin)
  //            << "   entry max: " << setw(6) << binEntryMax.at(bin+1)
  //            << "   events: "    << setw(6) << binEntryMax.at(bin+1)-binEntryMax.at(bin)
  //            << std::endl;
  
  
  
  // histogram definition
  TH1F** h_seedLaserCorr = new TH1F*[nBins];
  std::map<int,TH1F*> h_seedLaserCorr_vsRunId;
  
  for(int i = 0; i < nBins; ++i)
  {
    char histoName[80];
    
    sprintf(histoName, "h_seedLaserCorr_%d", i);
    h_seedLaserCorr[i] = new TH1F(histoName, histoName, 400, 0.8, 1.2);
  }
  
  
  
  // loop on the saved and sorted events
  std::cout << std::endl;
  std::cout << "***** Fill histograms *****" << std::endl;
  
  vector<double> AveTime(nBins);
  vector<int> MinTime(nBins);
  vector<int> MaxTime(nBins);
   
  for(int ientry = 0; ientry < nEntries; ++ientry)
  {
    if( (ientry%100000 == 0) ) std::cout << "reading entry " << ientry << std::endl;
    
    if( isSavedEntries.at(ientry) == false ) continue;
    
    int iSaved = -1;
    for(iSaved = 0; iSaved < nSavePts; ++iSaved)
      if( ientry == sortedEntries[iSaved].entry ) break;
    
    int bin = -1;
    for(bin = 0; bin < nBins; ++bin)
      if( iSaved >= binEntryMax.at(bin) && iSaved < binEntryMax.at(bin+1) )
        break;

    //std::cout << "bin = " << bin << "   iSaved = "<< iSaved << std::endl;
    
    ntu_DA->GetEntry(ientry);
        
    
    
    if( iSaved == binEntryMax.at(bin)+1 ) MinTime[bin] = timeStampHigh;
    if( iSaved == binEntryMax.at(bin+1)-1 ) MaxTime[bin] = timeStampHigh;
    AveTime[bin] += timeStampHigh;
    
    
    
    // fill the bins 
    h_seedLaserCorr[bin] -> Fill(seedLaserCorr);
    
    if(h_seedLaserCorr_vsRunId[runId] == NULL)
    {
      char histoName[80];
      sprintf(histoName, "h_seedLaserCorr_vsRunId_%d", runId);
      h_seedLaserCorr_vsRunId[runId] = new TH1F(histoName, histoName, 400, 0.8, 1.2);
      ++nBins_runId;
      runIds.push_back(runId);
    }
    h_seedLaserCorr_vsRunId[runId] -> Fill(seedLaserCorr);
    
  }
  
  for(int bin = 0; bin < nBins; ++bin)
  {
    AveTime[bin] = 1. * AveTime[bin] / (binEntryMax.at(bin+1)-binEntryMax.at(bin));
    //std::cout << date << " " << AveTime[i] << " " << MinTime[i] << " " << MaxTime[i] << std::endl;
  }
  
  
  
  
  
  
  // Define grpah and histograms
  TGraphAsymmErrors* g_seedLaserCorr = new TGraphAsymmErrors();
  g_seedLaserCorr->GetXaxis()->SetTimeFormat("%d/%m%F1970-01-01 00:00:00");
  g_seedLaserCorr->GetXaxis()->SetTimeDisplay(1);
  g_seedLaserCorr->GetXaxis()->SetTimeFormat("%d/%m%F1970-01-01 00:00:00");
  g_seedLaserCorr->GetXaxis()->SetTimeDisplay(1);
  
  for(int i = 0; i < nBins; ++i)
  {
    float date = (float)AveTime[i]; 
    float dLow = (float)(AveTime[i]-MinTime[i]); 
    float dHig = (float)(MaxTime[i]-AveTime[i]);
    
    g_seedLaserCorr -> SetPoint(i, date, h_seedLaserCorr[i]->GetMean());
    g_seedLaserCorr -> SetPointError(i, dLow, dHig, h_seedLaserCorr[i]->GetRMS(), h_seedLaserCorr[i]->GetRMS());
  }
  
  
  
  TGraphAsymmErrors* g_seedLaserCorr_vsRunId = new TGraphAsymmErrors();
  
  for(int i = 0; i < nBins_runId; ++i)
  {
    float date = runIds.at(i);
    
    g_seedLaserCorr_vsRunId -> SetPoint(i, date, h_seedLaserCorr_vsRunId[date]->GetMean());
    g_seedLaserCorr_vsRunId -> SetPointError(i, 0., 0., h_seedLaserCorr_vsRunId[date]->GetRMS(), h_seedLaserCorr_vsRunId[date]->GetRMS());
  }
  
  
  
  
  
  
  // Drawings
  // Final plots
  TCanvas* cplot = new TCanvas("cplot", "history plot",100,100,1200,500);
  cplot->cd();

  TPad *cLeft  = new TPad("pad_0","pad_0",0.00,0.00,0.48,1.00);
  TPad *cRight = new TPad("pad_1","pad_1",0.52,0.00,1.00,1.00);

  cLeft->SetLeftMargin(0.15);
  cLeft->SetRightMargin(0.025);
  cRight->SetLeftMargin(0.15);
  cRight->SetRightMargin(0.025);
  
  cLeft->Draw();
  cRight->Draw();

  float tYoffset = 1.5; 
  float labSize = 0.04;
  float labSize2 = 0.07;

  cLeft->cd(); 

  cLeft->SetGridx();
  cLeft->SetGridy();
  
  TH1F *hPad = (TH1F*)gPad->DrawFrame(t1,0.9,t2,1.05);
  hPad->GetXaxis()->SetTimeFormat("%d/%m%F1970-01-01 00:00:00");
  hPad->GetXaxis()->SetTimeDisplay(1);
  hPad->GetXaxis() -> SetRangeUser(MinTime[0]-43200,MaxTime[nBins-1]+43200);
  hPad->GetXaxis()->SetTitle("date");
  hPad->GetYaxis()->SetTitle("<seedLaserCorr>"); 
  hPad->GetYaxis()->SetTitleOffset(tYoffset);
  hPad->GetXaxis()->SetLabelSize(labSize);
  hPad->GetXaxis()->SetTitleSize(labSize);
  hPad->GetYaxis()->SetLabelSize(labSize);
  hPad->GetYaxis()->SetTitleSize(labSize);
  
  hPad -> SetMinimum(0.8);
  hPad -> SetMaximum(1.2);
  
  // draw history plot
  g_seedLaserCorr -> SetMarkerStyle(20);
  g_seedLaserCorr -> SetMarkerSize(0.75);
  g_seedLaserCorr -> SetMarkerColor(kBlue+2);
  g_seedLaserCorr -> SetLineColor(kBlue+2);
  g_seedLaserCorr -> Draw("P");
  
  
  
  cRight->cd(); 

  cRight->SetGridx();
  cRight->SetGridy();
  
  hPad = (TH1F*)gPad->DrawFrame(runIds.at(0)-100,0.9,runIds.at(nBins_runId-1)+100,1.05);
  hPad->GetXaxis()->SetTitle("runId");
  hPad->GetYaxis()->SetTitle("<seedLaserCorr>"); 
  hPad->GetYaxis()->SetTitleOffset(tYoffset);
  hPad->GetXaxis()->SetLabelSize(labSize);
  hPad->GetXaxis()->SetTitleSize(labSize);
  hPad->GetYaxis()->SetLabelSize(labSize);
  hPad->GetYaxis()->SetTitleSize(labSize);
  
  hPad -> SetMinimum(0.8);
  hPad -> SetMaximum(1.2);
  
  // draw history plot
  g_seedLaserCorr_vsRunId -> SetMarkerStyle(20);
  g_seedLaserCorr_vsRunId -> SetMarkerSize(0.75);
  g_seedLaserCorr_vsRunId -> SetMarkerColor(kBlue+2);
  g_seedLaserCorr_vsRunId -> SetLineColor(kBlue+2);
  g_seedLaserCorr_vsRunId -> Draw("P");  
}
