/* ******************************************************
// Plot EB or EE stability vs time and ancillary plots
// Input1 filename (data)
// Input2 MC or data used ti build the template reference
// Input3 EB or EE qualifier (EB default) 
// Input4 Number of events per poiny
//
// Compilation:
//     .L trisCheckStability.C+
// Execution:
//     trisCheckStability(input1,input2,input3,input4)
// Example: 
//     trisCheckStability("inputFiles/WZAnalysis_ric-rerecoDec22WithV3laser_136035-149294.root","inputFiles/WZAnalysis_MC.root","EB",3000)
//
// *************************************************** */

#include "histoFunc.h"
#include "ntupleUtils.h"
#include "ntupleUtils.cc"

#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"
#include "TPaveStats.h"
#include "TLegend.h"
#include "TChain.h"
#include "TVirtualFitter.h"

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



float timeLapse = 48.; // in hours
int t1 = 1267401600; //  1 Mar 2010
int t2 = 1325289600; // 31 Dec 2011 

float yMIN_EB = 0.960;
float yMAX_EB = 1.020;
float yMIN_EE = 0.880;
float yMAX_EE = 1.060;


void trisCheckStability(Char_t* EBEE = 0,
                        Int_t evtsPerPoint = 0,
                        std::string dayMin = "",
                        std::string dayMax = "",
                        float absEtaMin = -1.,
                        float absEtaMax = -1.)
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
  
  TChain* ntu_MC = new TChain("ntu");
  FillChain(ntu_MC,"inputMC.txt");
  std::cout << "REFERENCE: " << std::setw(8) << ntu_MC->GetEntries() << " entries" << std::endl;
  
  if (ntu_DA->GetEntries() == 0 || ntu_MC->GetEntries() == 0 )
  {
    std::cout << "CHK-STB Error: At least one file is empty" << std::endl; 
    return;
  }
  
  
  
  // Set branch addresses
  int runId;
  int timeStampHigh;
  int isW;
  float scLaserCorr, EoP, scEta, scPhi, scE, ES;
  int seedIeta, seedIphi, seedIx, seedIy, seedZside;

  ntu_DA->SetBranchAddress("runId", &runId);  
  ntu_DA->SetBranchAddress("timeStampHigh", &timeStampHigh);
  ntu_DA->SetBranchAddress("isW", &isW);
  ntu_DA->SetBranchAddress("ele1_scLaserCorr", &scLaserCorr);
  ntu_DA->SetBranchAddress("ele1_EOverP", &EoP);
  ntu_DA->SetBranchAddress("ele1_scEta", &scEta);
  ntu_DA->SetBranchAddress("ele1_scPhi", &scPhi);
  ntu_DA->SetBranchAddress("ele1_scE", &scE);
  ntu_DA->SetBranchAddress("ele1_es", &ES);
  ntu_DA->SetBranchAddress("ele1_seedIeta", &seedIeta);
  ntu_DA->SetBranchAddress("ele1_seedIphi", &seedIphi);
  ntu_DA->SetBranchAddress("ele1_seedIx", &seedIx);
  ntu_DA->SetBranchAddress("ele1_seedIy", &seedIy);
  ntu_DA->SetBranchAddress("ele1_seedZside", &seedZside);
  
  ntu_MC->SetBranchAddress("isW", &isW);
  ntu_MC->SetBranchAddress("ele1_scEta", &scEta);
  ntu_MC->SetBranchAddress("ele1_EOverP", &EoP);
  
  
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
  
  
  
  // Build the reference from 'infile2'
  std::cout << std::endl;
  std::cout << "***** Build reference for " << EBEE << " *****" << std::endl;
  
  TH1F* h_template = new TH1F("template", "", 1200, 0., 3.);

  for(int ientry = 0; ientry < ntu_MC->GetEntries(); ++ientry) {
    if( (ientry%100000 == 0) ) std::cout << "reading MC entry " << ientry << std::endl;
    ntu_MC->GetEntry(ientry);
    if (strcmp(EBEE,"EB")==0 && fabs(scEta) > 1.45) continue;                       // barrel
    if (strcmp(EBEE,"EE")==0 && (fabs(scEta) < 1.47 || fabs(scEta)>2.7 )) continue; // endcap
    
    if( (absEtaMin != -1.) && (absEtaMax != -1.) )
    {
      if( (fabs(scEta) < absEtaMin) || (fabs(scEta) > absEtaMax) ) continue;
    }
    
    h_template -> Fill(EoP);
  }
  
  std::cout << "Reference built for " << EBEE << " - " << h_template->GetEntries() << " events" << std::endl;
  TCanvas* c_reference = new TCanvas("c_reference","reference",100,100,400,400);
  c_reference -> cd();
  h_template -> Draw();
  
  
  
  // Loop and sort events
  std::cout << std::endl;
  std::cout << "***** Sort events and define bins *****" << std::endl;
  
  int nEntries = ntu_DA -> GetEntriesFast(); 
  int nSavePts = 0; 
  std::vector<bool> isSavedEntries(nEntries);
  std::vector<Sorter> sortedEntries;
  std::vector<int> timeStampFirst;
    
  for(int ientry = 0; ientry < nEntries; ++ientry)  {
    ntu_DA -> GetEntry(ientry);
    isSavedEntries.at(ientry) = false;
    
    // save only what is needed for the analysis!!!
    if (strcmp(EBEE,"EB")==0 && fabs(scEta) > 1.45) continue;                       // barrel
    if (strcmp(EBEE,"EE")==0 && (fabs(scEta) < 1.47 || fabs(scEta)>2.7 )) continue; // endcap
    if( timeStampHigh < t1 ) continue;
    if( timeStampHigh > t2 ) continue;
    
    if( (absEtaMin != -1.) && (absEtaMax != -1.) )
    {
      if( (fabs(scEta) < absEtaMin) || (fabs(scEta) > absEtaMax) ) continue;
    }
    
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
  std::vector<int> binEntryMax;
  
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
  TVirtualFitter::SetDefaultFitter("Fumili2");
  
  
  
  // histogram definition
  TH1F* h_scOccupancy_eta  = new TH1F("h_scOccupancy_eta","", 285, -2.5, 2.5);
  TH1F* h_scOccupancy_phi  = new TH1F("h_scOccupancy_phi","", 360, -3.14159, 3.14159);
  
  TH2F* h_seedOccupancy_EB  = new TH2F("h_seedOccupancy_EB","",  171, -85., 86., 361,   0.,361.);
  TH2F* h_seedOccupancy_EEp = new TH2F("h_seedOccupancy_EEp","", 101,   0.,101., 100,   0.,101.);
  TH2F* h_seedOccupancy_EEm = new TH2F("h_seedOccupancy_EEm","", 101,   0.,101., 100,   0.,101.);
  
  TH1F* h_EoP_spread;
  TH1F* h_EoC_spread;
  TH1F* h_EoP_spread_run;
  TH1F* h_EoC_spread_run;
    
  if ( strcmp(EBEE,"EB")==0 )
  {  
    h_EoP_spread = new TH1F("h_EoP_spread","",100,yMIN_EB,yMAX_EB);
    h_EoC_spread = new TH1F("h_EoC_spread","",100,yMIN_EB,yMAX_EB);
    h_EoP_spread_run = new TH1F("h_EoP_spread_run","",100,yMIN_EB,yMAX_EB);
    h_EoC_spread_run = new TH1F("h_EoC_spread_run","",100,yMIN_EB,yMAX_EB);
  }
  else 
  {  
    h_EoP_spread = new TH1F("h_EoP_spread","",100,yMIN_EE,yMAX_EE);
    h_EoC_spread = new TH1F("h_EoC_spread","",100,yMIN_EE,yMAX_EE);
    h_EoP_spread_run = new TH1F("h_EoP_spread_run","",100,yMIN_EE,yMAX_EE);
    h_EoC_spread_run = new TH1F("h_EoC_spread_run","",100,yMIN_EE,yMAX_EE);
  }
  
  h_EoP_spread -> SetLineColor(kRed+2);
  h_EoP_spread -> SetLineWidth(2);
  h_EoP_spread -> GetXaxis() -> SetTitle("Relative E/p scale");
  
  h_EoC_spread -> SetLineColor(kGreen+2);
  h_EoC_spread -> SetLineWidth(2);
  h_EoC_spread -> GetXaxis() -> SetTitle("Relative E/p scale");
  
  h_EoP_spread_run -> SetLineColor(kRed+2);
  h_EoP_spread_run -> SetLineWidth(2);
  h_EoP_spread_run -> GetXaxis() -> SetTitle("Relative E/p scale");
  
  h_EoC_spread_run -> SetLineColor(kGreen+2);
  h_EoC_spread_run -> SetLineWidth(2);
  h_EoC_spread_run -> GetXaxis() -> SetTitle("Relative E/p scale");
  
  
  
  TH1F** h_EoP = new TH1F*[nBins];
  TH1F** h_EoC = new TH1F*[nBins];
  TH1F** h_Las = new TH1F*[nBins];
  
  for(int i = 0; i < nBins; ++i)
  {
    char histoName[80];
    
    sprintf(histoName, "EoP_%d", i);
    h_EoP[i] = new TH1F(histoName, histoName, 1200, 0., 3.);
    h_EoP[i] -> SetFillColor(kRed+2);
    h_EoP[i] -> SetFillStyle(3004);
    h_EoP[i] -> SetMarkerStyle(7);
    h_EoP[i] -> SetMarkerColor(kRed+2);
    h_EoP[i] -> SetLineColor(kRed+2);
    
    sprintf(histoName, "EoC_%d", i);
    h_EoC[i] = new TH1F(histoName, histoName, 1200, 0., 3.);
    h_EoC[i] -> SetFillColor(kGreen+2);
    h_EoC[i] -> SetFillStyle(3004);
    h_EoC[i] -> SetMarkerStyle(7);
    h_EoC[i] -> SetMarkerColor(kGreen+2);
    h_EoC[i] -> SetLineColor(kGreen+2);
    
    sprintf(histoName, "Las_%d", i);
    h_Las[i] = new TH1F(histoName, histoName, 100, 0.5, 1.5);
  }
  
  
  // function definition
  TF1** f_EoP = new TF1*[nBins];
  TF1** f_EoC = new TF1*[nBins];
  
  
  
  // loop on the saved and sorted events
  std::cout << std::endl;
  std::cout << "***** Fill and fit histograms *****" << std::endl;

  vector<double> AveTime(nBins);
  vector<int> MinTime(nBins);
  vector<int> MaxTime(nBins);
  vector<double> AveRun(nBins);    
  vector<int> MinRun(nBins);
  vector<int> MaxRun(nBins);
  
  int iSaved = -1;
  for(int ientry = 0; ientry < nEntries; ++ientry)
  {
    if( (ientry%100000 == 0) ) std::cout << "reading entry " << ientry << std::endl;
    
    if( isSavedEntries.at(ientry) == false ) continue;
    
    ++iSaved;
    //iSaved = -1;
    //for(iSaved = 0; iSaved < nSavePts; ++iSaved)
    //  if( ientry == sortedEntries[iSaved].entry ) break;
    
    int bin = -1;
    for(bin = 0; bin < nBins; ++bin)
      if( iSaved >= binEntryMax.at(bin) && iSaved < binEntryMax.at(bin+1) )
        break;

    //std::cout << "bin = " << bin << "   iSaved = "<< iSaved << std::endl;
    
    ntu_DA->GetEntry(ientry);
        
    
    
    if( iSaved == binEntryMax.at(bin)+1 ) MinTime[bin] = timeStampHigh;
    if( iSaved == binEntryMax.at(bin+1)-1 ) MaxTime[bin] = timeStampHigh;
    AveTime[bin] += timeStampHigh;
    
    if( iSaved == binEntryMax.at(bin)+1 ) MinRun[bin] = runId;
    if( iSaved == binEntryMax.at(bin+1)-1 ) MaxRun[bin] = runId;
    AveRun[bin] += runId;    
    
    
    // fill the bins
    if( fabs(scEta) < 1.479 )
      (h_EoP[bin]) -> Fill(EoP/scLaserCorr);
    else
      (h_EoP[bin]) -> Fill( EoP * ((scE-ES)/scLaserCorr+ES) / scE );
    
    (h_EoC[bin]) -> Fill(EoP);
    
    (h_Las[bin]) -> Fill(scLaserCorr);    
    
    h_scOccupancy_eta -> Fill(scEta);
    h_scOccupancy_phi -> Fill(scPhi);
    if(seedZside == 0)
      h_seedOccupancy_EB -> Fill(seedIeta,seedIphi);
    if(seedZside > 0)
      h_seedOccupancy_EEp -> Fill(seedIx,seedIy);
    if(seedZside < 0)
      h_seedOccupancy_EEm -> Fill(seedIx,seedIy);
  }
  
  for(int bin = 0; bin < nBins; ++bin)
  {
    AveTime[bin] = 1. * AveTime[bin] / (binEntryMax.at(bin+1)-binEntryMax.at(bin));
    AveRun[bin] = 1. * AveRun[bin] / (binEntryMax.at(bin+1)-binEntryMax.at(bin));
    //std::cout << date << " " << AveTime[i] << " " << MinTime[i] << " " << MaxTime[i] << std::endl;
  }
  
  
  // Define grpah and histograms
  TGraphAsymmErrors* g_fit   = new TGraphAsymmErrors();
  TGraphAsymmErrors* g_c_fit   = new TGraphAsymmErrors();
  TGraph* g_las = new TGraph();
  g_fit->GetXaxis()->SetTimeFormat("%d/%m%F1970-01-01 00:00:00");
  g_fit->GetXaxis()->SetTimeDisplay(1);
  g_c_fit->GetXaxis()->SetTimeFormat("%d/%m%F1970-01-01 00:00:00");
  g_c_fit->GetXaxis()->SetTimeDisplay(1);
  g_las->GetXaxis()->SetTimeFormat("%d/%m%F1970-01-01 00:00:00");
  g_las->GetXaxis()->SetTimeDisplay(1);
  
  TGraphAsymmErrors* g_fit_run   = new TGraphAsymmErrors();
  TGraphAsymmErrors* g_c_fit_run = new TGraphAsymmErrors();  
  
  
  int rebin = 4;
  if (strcmp(EBEE,"EB")==0) rebin = 2; 
  
  h_template -> Rebin(rebin*4);
  
  
  for(int i = 0; i < nBins; ++i)
  {
    h_EoP[i] -> Rebin(rebin*4);    
    h_EoC[i] -> Rebin(rebin*4);    
    
    
    
    //------------------------------------
    // Fill the graph for uncorrected data
    
    // define the fitting function
    // N.B. [0] * ( [1] * f( [1]*(x-[2]) ) )

    histoFunc* templateHistoFunc = new histoFunc(h_template);
    char funcName[50];
    sprintf(funcName,"f_EoP_%d",i);
    f_EoP[i] = new TF1(funcName, templateHistoFunc, 0.7, 1.3, 3, "histoFunc");
    f_EoP[i] -> SetParName(0,"Norm"); 
    f_EoP[i] -> SetParName(1,"Scale factor"); 
    f_EoP[i] -> SetLineWidth(1); 
    f_EoP[i] -> SetNpx(10000);
    
    double xNorm = h_EoP[i]->GetEntries()/h_template->GetEntries() *
      h_EoP[i]->GetBinWidth(1)/h_template->GetBinWidth(1); 
    
    f_EoP[i] -> FixParameter(0, xNorm);
    f_EoP[i] -> SetParameter(1, 0.99);
    f_EoP[i] -> FixParameter(2, 0.);
    f_EoP[i] -> SetLineColor(kRed+2); 
    
    
    TFitResultPtr rp = h_EoP[i] -> Fit(funcName, "QNERLS+");
    int fStatus = rp;
    
    // fill the graph
    double eee = f_EoP[i]->GetParError(1); 
    if (fStatus!=4 && eee>0.1*h_template->GetRMS()/sqrt(evtsPerPoint))
    {
      float date = (float)AveTime[i];
      float dLow = (float)(AveTime[i]-MinTime[i]); 
      float dHig = (float)(MaxTime[i]-AveTime[i]);
      float run = (float)AveRun[i];
      float rLow = (float)(AveRun[i]-MinRun[i]); 
      float rHig = (float)(MaxRun[i]-AveRun[i]);
      
      g_fit -> SetPoint(i,  date , 1./f_EoP[i]->GetParameter(1));
      g_fit -> SetPointError(i, dLow , dHig, eee, eee);
      if ( (date > t1) && (date < t2) )
        h_EoP_spread -> Fill(1./f_EoP[i]->GetParameter(1));
      
      g_fit_run -> SetPoint(i,  run , 1./f_EoP[i]->GetParameter(1));
      g_fit_run -> SetPointError(i, rLow , rHig, eee, eee);
      if ( (date > t1) && (date < t2) )
        h_EoP_spread_run -> Fill(1./f_EoP[i]->GetParameter(1));
    }
    else
      std::cout << "Fitting uncorrected time bin: " << i << "   Fail status: " << fStatus << "   sigma: " << eee << endl;
    
    
    
    //----------------------------------
    // Fill the graph for corrected data
    
    // define the fitting function
    // N.B. [0] * ( [1] * f( [1]*(x-[2]) ) )

    sprintf(funcName,"f_EoC_%d",i);
    f_EoC[i] = new TF1(funcName, templateHistoFunc, 0.7, 1.3, 3, "histoFunc");
    f_EoC[i] -> SetParName(0,"Norm"); 
    f_EoC[i] -> SetParName(1,"Scale factor"); 
    f_EoC[i] -> SetLineWidth(1); 
    f_EoC[i] -> SetNpx(10000);
    
    xNorm = h_EoC[i]->GetEntries()/h_template->GetEntries() *
            h_EoC[i]->GetBinWidth(1)/h_template->GetBinWidth(1); 

    f_EoC[i] -> FixParameter(0, xNorm);
    f_EoC[i] -> SetParameter(1, 0.99);
    f_EoC[i] -> FixParameter(2, 0.);
    f_EoC[i] -> SetLineColor(kGreen+2); 
    
    
    TFitResultPtr rc = h_EoC[i] -> Fit(funcName, "QNERLS+");
    fStatus = rc;
    
    // fill the graph
    eee = f_EoC[i]->GetParError(1); 
    if (fStatus!=4 && eee>0.1*h_template->GetRMS()/sqrt(evtsPerPoint))
    {
      float date = (float)AveTime[i]; 
      float dLow = (float)(AveTime[i]-MinTime[i]); 
      float dHig = (float)(MaxTime[i]-AveTime[i]);
      float run = (float)AveRun[i];
      float rLow = (float)(AveRun[i]-MinRun[i]); 
      float rHig = (float)(MaxRun[i]-AveRun[i]);
      
      g_c_fit -> SetPoint(i,  date , 1./f_EoC[i]->GetParameter(1));
      g_c_fit -> SetPointError(i, dLow , dHig , eee, eee);
      if ( (date > t1) && (date < t2) )
        h_EoC_spread -> Fill(1./f_EoC[i]->GetParameter(1));
      
      g_c_fit_run -> SetPoint(i,  run , 1./f_EoC[i]->GetParameter(1));
      g_c_fit_run -> SetPointError(i, rLow , rHig, eee, eee);
      if ( (date > t1) && (date < t2) )
        h_EoC_spread_run -> Fill(1./f_EoC[i]->GetParameter(1));
    }
    else
      std::cout << "Fitting corrected time bin: " << i << "   Fail status: " << fStatus << "   sigma: " << eee << endl;
    
  }
  
  
  
  
  
  
  // Drawings
  TCanvas* c_scOccupancy = new TCanvas("c_scOccupancy","SC occupancy",100,100,700,400);
  c_scOccupancy -> Divide(2,1);
  
  c_scOccupancy -> cd(1);
  h_scOccupancy_eta -> GetXaxis() -> SetTitle("sc #eta");
  h_scOccupancy_eta -> GetYaxis() -> SetTitle("events");
  h_scOccupancy_eta -> Draw();
  
  c_scOccupancy -> cd(2);
  h_scOccupancy_phi -> GetXaxis() -> SetTitle("sc #phi");
  h_scOccupancy_phi -> GetYaxis() -> SetTitle("events");
  h_scOccupancy_phi -> Draw();
  
  
  TCanvas* c_seedOccupancy = new TCanvas("c_seedOccupancy","seed occupancy",100,100,1100,400);
  c_seedOccupancy -> Divide(3,1);
  
  c_seedOccupancy -> cd(1);
  h_seedOccupancy_EB -> GetXaxis() -> SetTitle("seed i#eta");
  h_seedOccupancy_EB -> GetYaxis() -> SetTitle("seed i#phi");
  h_seedOccupancy_EB -> Draw("COLZ");
  
  c_seedOccupancy -> cd(2);
  h_seedOccupancy_EEp -> GetXaxis() -> SetTitle("seed ix");
  h_seedOccupancy_EEp -> GetYaxis() -> SetTitle("seed iy");
  h_seedOccupancy_EEp -> Draw("COLZ");
  
  c_seedOccupancy -> cd(3);
  h_seedOccupancy_EEm -> GetXaxis() -> SetTitle("seed ix");
  h_seedOccupancy_EEm -> GetYaxis() -> SetTitle("seed iy");
  h_seedOccupancy_EEm -> Draw("COLZ");
  
  
  
  //TPaveStats** s_EoP = new TPaveStats*[nBins];
  //TPaveStats** s_EoC = new TPaveStats*[nBins];
  //
  //TCanvas *c1[100]; 
  //for(int i = 0; i < nBins; ++i)
  //{
  //  char canvasName[50];
  //  if (i%6==0)
  //  {
  //    sprintf(canvasName, "Fits-%0d", i/6); 
  //    c1[i/6] = new TCanvas(canvasName, canvasName);
  //    c1[i/6] -> Divide(3,2);
  //  }
  //  c1[i/6] -> cd (i%6+1);
  //  
  //  h_EoP[i] -> GetXaxis() -> SetTitle("E/p");
  //  h_EoP[i] -> GetXaxis() -> SetRangeUser(0.5,1.5); 
  //  h_EoP[i] -> Draw("e");
  //  gPad->Update(); 
  //  s_EoP[i]= (TPaveStats*)(h_EoP[i]->GetListOfFunctions()->FindObject("stats"));
  //  s_EoP[i]->SetTextColor(kRed+2);
  //
  //  h_EoC[i] -> Draw("esames");
  //  gPad->Update(); 
  //  s_EoC[i]= (TPaveStats*)(h_EoC[i]->GetListOfFunctions()->FindObject("stats"));
  //  s_EoC[i]->SetY1NDC(0.59); //new x start position
  //  s_EoC[i]->SetY2NDC(0.79); //new x end position
  //  s_EoC[i]->SetTextColor(kGreen+2);
  //  s_EoC[i]->Draw("sames");
  //  
  //  f_EoP[i]->Draw("same");
  //  f_EoC[i]->Draw("same");
  //}
  
  
  
  
  
  
  TF1 pol0("pol0","pol0",t1,t2);
  pol0.SetLineColor(kGreen+2);
  pol0.SetLineWidth(2);
  pol0.SetLineStyle(2);
  g_c_fit -> Fit("pol0","QNR");
  
  TF1 pol0_May5("pol0_May5","pol0",1302757200,1304571600);
  g_c_fit -> Fit("pol0_May5","QNR");
    
  //float yscale = pol0.GetParameter(0); 
  float emean = 0, eclean = 0 ;
  float yscale = 1.;

  for(int i = 0; i < nBins; ++i){
    eclean += g_c_fit->GetErrorYlow(i); 
    emean += g_c_fit->GetErrorYlow(i)*sqrt(pol0.GetChisquare()/pol0.GetNDF()); 
    // rescale to mean
    double x,y; 
    g_fit -> GetPoint(i,x,y); 
    g_fit -> SetPoint(i,x,y/yscale); 
    g_c_fit -> GetPoint(i,x,y); 
    g_c_fit -> SetPoint(i,x,y/yscale);
    g_las -> GetPoint(i,x,y); 
    g_las -> SetPoint(i,x,y/yscale);
    g_fit_run -> GetPoint(i,x,y); 
    g_fit_run -> SetPoint(i,x,y/yscale); 
    g_c_fit_run -> GetPoint(i,x,y); 
    g_c_fit_run -> SetPoint(i,x,y/yscale);

  }
  std::cout << "Mean Error: " << emean/(float)nBins << " " << eclean/(float)nBins << std::endl; 
  g_c_fit -> Fit("pol0","QR+");
  
  

  //----------------------------------------
  // Fill the graph for avg laser correction
    
  for(int i = 0; i < nBins; ++i)
  {  
    g_las -> SetPoint(i, (float)AveTime[i], pol0_May5.GetParameter(0)*(1./h_Las[i]->GetMean()));
  }  
  
  
  
  
  //-------------------
  // Final plot vs date
  //-------------------
  TCanvas* cplot = new TCanvas("cplot", "history plot vs date",100,100,1000,500);
  cplot->cd();

  TPad *cLeft  = new TPad("pad_0","pad_0",0.00,0.00,0.75,1.00);
  TPad *cRight = new TPad("pad_1","pad_1",0.75,0.00,1.00,1.00);

  cLeft->SetLeftMargin(0.15); 
  cLeft->SetRightMargin(0.025); 
  cRight->SetLeftMargin(0.025); 

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
  hPad->GetYaxis()->SetTitle("Relative E/p scale"); 
  hPad->GetYaxis()->SetTitleOffset(tYoffset);
  hPad->GetXaxis()->SetLabelSize(labSize);
  hPad->GetXaxis()->SetTitleSize(labSize);
  hPad->GetYaxis()->SetLabelSize(labSize);
  hPad->GetYaxis()->SetTitleSize(labSize);
  
  if ( strcmp(EBEE,"EB")==0 )
  {  
    hPad -> SetMinimum(yMIN_EB);
    hPad -> SetMaximum(yMAX_EB);
  }
  else 
  {  
    hPad -> SetMinimum(yMIN_EE);
    hPad -> SetMaximum(yMAX_EE);
  }
  
  // draw history plot
  g_fit -> SetMarkerStyle(20);
  g_fit -> SetMarkerSize(0.75);
  g_fit -> SetMarkerColor(kRed+2);
  g_fit -> SetLineColor(kRed+2);
  g_fit -> Draw("P");
  g_c_fit -> SetMarkerStyle(20);
  g_c_fit -> SetMarkerColor(kGreen+2);
  g_c_fit -> SetLineColor(kGreen+2);
  g_c_fit -> SetMarkerSize(0.75);
  g_c_fit -> Draw("P,same");
  g_las -> SetLineColor(kAzure-2);
  g_las -> SetLineWidth(2);
  g_las -> Draw("L,same");
  
  
  
  cRight -> cd();
  
  TPaveStats* s_EoP_spread = new TPaveStats();
  TPaveStats* s_EoC_spread = new TPaveStats();
  
  
  h_EoC_spread -> SetFillStyle(3001);
  h_EoC_spread -> SetFillColor(kGreen+2);
  h_EoC_spread->GetYaxis()->SetLabelSize(labSize2);
  h_EoC_spread->GetYaxis()->SetTitleSize(labSize2);
  h_EoC_spread->GetYaxis()->SetNdivisions(505);
  h_EoC_spread->GetYaxis()->SetLabelOffset(-0.02);
  h_EoC_spread->GetXaxis()->SetLabelOffset(1000);

  h_EoC_spread -> Draw("hbar");
  gPad -> Update();

  s_EoC_spread = (TPaveStats*)(h_EoC_spread->GetListOfFunctions()->FindObject("stats"));
  s_EoC_spread ->SetTextColor(kGreen+2);
  s_EoC_spread ->SetTextSize(0.06);
  s_EoC_spread->SetX1NDC(0.49); //new x start position
  s_EoC_spread->SetX2NDC(0.99); //new x end position
  s_EoC_spread->SetY1NDC(0.875); //new x start position
  s_EoC_spread->SetY2NDC(0.990); //new x end position
  s_EoC_spread -> SetOptStat(1100);
  s_EoC_spread -> Draw("sames");

  h_EoP_spread -> SetFillStyle(3001);
  h_EoP_spread -> SetFillColor(kRed+2);
  h_EoP_spread -> Draw("hbarsames");
  gPad -> Update();
  s_EoP_spread = (TPaveStats*)(h_EoP_spread->GetListOfFunctions()->FindObject("stats"));
  s_EoP_spread->SetX1NDC(0.49); //new x start position
  s_EoP_spread->SetX2NDC(0.99); //new x end position
  s_EoP_spread->SetY1NDC(0.750); //new x start position
  s_EoP_spread->SetY2NDC(0.875); //new x end position
  s_EoP_spread ->SetOptStat(1100);
  s_EoP_spread ->SetTextColor(kRed+2);
  s_EoP_spread ->SetTextSize(0.06);
  s_EoP_spread -> Draw("sames");
  
  
  
  //------------------
  // Final plot vs run
  //------------------
  TCanvas* cplot_run = new TCanvas("cplot_run", "history plot vs run",100,100,1000,500);
  cplot_run->cd();
  
  cLeft  = new TPad("pad_0_run","pad_0_run",0.00,0.00,0.75,1.00);
  cRight = new TPad("pad_1_run","pad_1_run",0.75,0.00,1.00,1.00);

  cLeft->SetLeftMargin(0.15); 
  cLeft->SetRightMargin(0.025); 
  cRight->SetLeftMargin(0.025); 

  cLeft->Draw();
  cRight->Draw();

  tYoffset = 1.5; 
  labSize = 0.04;
  labSize2 = 0.07;

  cLeft->cd(); 

  cLeft->SetGridx();
  cLeft->SetGridy();
  
  hPad = (TH1F*)gPad->DrawFrame(MinRun[0]-1000,0.9,MaxRun[nBins-1]+1000,1.05);
  hPad->GetXaxis()->SetTitle("run");
  hPad->GetYaxis()->SetTitle("Relative E/p scale"); 
  hPad->GetYaxis()->SetTitleOffset(tYoffset);
  hPad->GetXaxis()->SetLabelSize(labSize);
  hPad->GetXaxis()->SetTitleSize(labSize);
  hPad->GetYaxis()->SetLabelSize(labSize);
  hPad->GetYaxis()->SetTitleSize(labSize);
  
  if ( strcmp(EBEE,"EB")==0 )
  {  
    hPad -> SetMinimum(yMIN_EB);
    hPad -> SetMaximum(yMAX_EB);
  }
  else 
  {  
    hPad -> SetMinimum(yMIN_EE);
    hPad -> SetMaximum(yMAX_EE);
  }
  
  // draw history plot
  g_fit_run -> SetMarkerStyle(20);
  g_fit_run -> SetMarkerSize(0.75);
  g_fit_run -> SetMarkerColor(kRed+2);
  g_fit_run -> SetLineColor(kRed+2);
  g_fit_run -> Draw("P");
  g_c_fit_run -> SetMarkerStyle(20);
  g_c_fit_run -> SetMarkerColor(kGreen+2);
  g_c_fit_run -> SetLineColor(kGreen+2);
  g_c_fit_run -> SetMarkerSize(0.75);
  g_c_fit_run -> Draw("P,same");
  
  
  cRight -> cd();
  
  s_EoP_spread = new TPaveStats();
  s_EoC_spread = new TPaveStats();
  
  
  h_EoC_spread_run -> SetFillStyle(3001);
  h_EoC_spread_run -> SetFillColor(kGreen+2);
  h_EoC_spread_run->GetYaxis()->SetLabelSize(labSize2);
  h_EoC_spread_run->GetYaxis()->SetTitleSize(labSize2);
  h_EoC_spread_run->GetYaxis()->SetNdivisions(505);
  h_EoC_spread_run->GetYaxis()->SetLabelOffset(-0.02);
  h_EoC_spread_run->GetXaxis()->SetLabelOffset(1000);

  h_EoC_spread_run -> Draw("hbar");
  gPad -> Update();
  
  s_EoC_spread = (TPaveStats*)(h_EoC_spread_run->GetListOfFunctions()->FindObject("stats"));
  s_EoC_spread ->SetTextColor(kGreen+2);
  s_EoC_spread ->SetTextSize(0.06);
  s_EoC_spread->SetX1NDC(0.49); //new x start position
  s_EoC_spread->SetX2NDC(0.99); //new x end position
  s_EoC_spread->SetY1NDC(0.875); //new x start position
  s_EoC_spread->SetY2NDC(0.990); //new x end position
  s_EoC_spread -> SetOptStat(1100);
  s_EoC_spread -> Draw("sames");

  h_EoP_spread_run -> SetFillStyle(3001);
  h_EoP_spread_run -> SetFillColor(kRed+2);
  h_EoP_spread_run -> Draw("hbarsames");
  gPad -> Update();
  
  s_EoP_spread = (TPaveStats*)(h_EoP_spread_run->GetListOfFunctions()->FindObject("stats"));
  s_EoP_spread->SetX1NDC(0.49); //new x start position
  s_EoP_spread->SetX2NDC(0.99); //new x end position
  s_EoP_spread->SetY1NDC(0.750); //new x start position
  s_EoP_spread->SetY2NDC(0.875); //new x end position
  s_EoP_spread ->SetOptStat(1100);
  s_EoP_spread ->SetTextColor(kRed+2);
  s_EoP_spread ->SetTextSize(0.06);
  s_EoP_spread -> Draw("sames");
  
  
    
  TFile* o = new TFile("trisCheckStabilityHistos.root","RECREATE");
  o -> cd();
  
  g_fit -> Write("g_fit");
  g_c_fit -> Write("g_c_fit");
  g_las -> Write("g_las");
  g_fit_run -> Write("g_fit_run");
  g_c_fit_run -> Write("g_c_fit_run");
  
  o -> Close();
  
  
  
  std::string folderName = std::string(EBEE);
  if( (absEtaMin != -1.) && (absEtaMax != -1.) )
  {
    char absEtaBuffer[50];
    sprintf(absEtaBuffer,"_%.2f-%.2f",absEtaMin,absEtaMax);
    folderName += std::string(absEtaBuffer);
  } 
  
  c_scOccupancy -> Print((folderName+"/"+folderName+"_scOccupancy.png").c_str(),"png");
  c_seedOccupancy -> Print((folderName+"/"+folderName+"_seedOccupancy.png").c_str(),"png");
  cplot -> Print((folderName+"/"+folderName+"_history_vsTime.png").c_str(),"png");
  cplot_run -> Print((folderName+"/"+folderName+"_history_vsRun.png").c_str(),"png");

}
