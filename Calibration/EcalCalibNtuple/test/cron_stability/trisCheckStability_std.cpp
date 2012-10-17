// g++ -Wall -o trisCheckStability_std `root-config --cflags --glibs` setTDRStyle.cc ntupleUtils.cc stabilityUtils.cc ConvoluteTemplate.cc histoFunc.h trisCheckStability_std.cpp

// ***************************************************
// Plot EB or EE stability vs time and ancillary plots
// ***************************************************

#include "setTDRStyle.h"
#include "histoFunc.h"
#include "ConvoluteTemplate.h"
#include "ntupleUtils.h"
#include "stabilityUtils.h"

#include "TROOT.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TFile.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TPaveStats.h"
#include "TLegend.h"
#include "TChain.h"
#include "TVirtualFitter.h"
#include "TLatex.h"

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



float timeLapse = 24.; // in hours
int t1 = 1267401600;   //  1 Mar 2010
int t2 = 1325289600;   // 31 Dec 2011 

float yMIN = 0.700;
float yMAX = 1.100;






int main(int argc, char** argv)
{
  // Set style options
  setTDRStyle();
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetOptTitle(0); 
  gStyle->SetOptStat(1110); 
  gStyle->SetOptFit(1); 
  
  // Set fitting options
  TVirtualFitter::SetDefaultFitter("Fumili2");
  
  
  
  
  
  
  //-----------------
  // Input parameters
  
  std::cout << "\n***************************************************************************************************************************" << std::endl;
  std::cout << "arcg: " << argc << std::endl;
  char* EBEE = argv[1];
  int evtsPerPoint = atoi(argv[2]);
  int useRegression = atoi(argv[3]);
  std::string dayMin = "";
  std::string dayMax = "";
  std::string dayMinLabel = "";
  std::string dayMaxLabel = "";
  float absEtaMin = -1.;
  float absEtaMax = -1.;
  int IetaMin = -1;
  int IetaMax = -1;
  int IphiMin = -1;
  int IphiMax = -1;
  if(argc >= 5)
  {
    dayMin = std::string(argv[4])+" "+std::string(argv[5])+" "+std::string(argv[6]);
    dayMax = std::string(argv[7])+" "+std::string(argv[8])+" "+std::string(argv[9]);
    dayMinLabel = std::string(argv[4])+"_"+std::string(argv[5])+"_"+std::string(argv[6]);
    dayMaxLabel = std::string(argv[7])+"_"+std::string(argv[8])+"_"+std::string(argv[9]);
    
    t1 = dateToInt(dayMin);
    t2 = dateToInt(dayMax);
  }
  if(argc >= 11)
  {
    yMIN = atof(argv[10]);
    yMAX = atof(argv[11]);
  }
  if(argc >= 13)
  {
    absEtaMin = atof(argv[12]);
    absEtaMax = atof(argv[13]);
  }
  if(argc >= 15)
  {
    IetaMin = atoi(argv[14]);
    IetaMax = atoi(argv[15]);
    IphiMin = atoi(argv[16]);
    IphiMax = atoi(argv[17]);
  }
  
  std::cout << "EBEE: "          << EBEE          << std::endl;
  std::cout << "evtsPerPoint: "  << evtsPerPoint  << std::endl;
  std::cout << "useRegression: " << useRegression << std::endl;
  std::cout << "dayMin: "        << dayMin        << std::endl;
  std::cout << "dayMax: "        << dayMax        << std::endl;
  std::cout << "yMin: "          << yMIN          << std::endl;
  std::cout << "yMax: "          << yMAX          << std::endl;
  std::cout << "absEtaMin: "     << absEtaMin     << std::endl;
  std::cout << "absEtaMax: "     << absEtaMax     << std::endl;
  std::cout << "IetaMin: "       << IetaMin       << std::endl;
  std::cout << "IetaMax: "       << IetaMax       << std::endl;
  std::cout << "IphiMin: "       << IphiMin       << std::endl;
  std::cout << "IphiMax: "       << IphiMax       << std::endl;
  
  
  
  
  
  
  //-------------------
  // Define in/outfiles
  
  std::string folderName = std::string(EBEE) + "_" + dayMinLabel + "_" + dayMaxLabel;
  if( (absEtaMin != -1.) && (absEtaMax != -1.) )
  {
    char absEtaBuffer[50];
    sprintf(absEtaBuffer,"_%.2f-%.2f",absEtaMin,absEtaMax);
    folderName += std::string(absEtaBuffer);
  } 
  
  if( (IetaMin != -1.) && (IetaMax != -1.) && (IphiMin != -1.) && (IphiMax != -1.) )
  {
    char absEtaBuffer[50];
    sprintf(absEtaBuffer,"_Ieta_%d-%d_Iphi_%d-%d",IetaMin,IetaMax,IphiMin,IphiMax);
    folderName += std::string(absEtaBuffer);
  } 
  
  gSystem->mkdir(folderName.c_str());
  TFile* o = new TFile((folderName+"/"+folderName+"_histos.root").c_str(),"RECREATE");
  
  
  
  // Get trees
  std::cout << std::endl;
  
  TChain* ntu_DA = new TChain("simpleNtupleEoverP/SimpleNtupleEoverP");
  FillChain(ntu_DA,"inputDATA.txt");
  std::cout << "     DATA: " << std::setw(8) << ntu_DA->GetEntries() << " entries" << std::endl;
  
  TChain* ntu_MC = new TChain("simpleNtupleEoverP/SimpleNtupleEoverP");
  FillChain(ntu_MC,"inputMC.txt");
  std::cout << "REFERENCE: " << std::setw(8) << ntu_MC->GetEntries() << " entries" << std::endl;
  
  if (ntu_DA->GetEntries() == 0 || ntu_MC->GetEntries() == 0 )
  {
    std::cout << "Error: At least one file is empty" << std::endl; 
    return -1;
  }
  
  
  
  // Set branch addresses
  int runId;
  int timeStampHigh;
  int PV_n;
  float scLaserCorr, seedLaserAlpha, EoP, scEta, scPhi, scE, ES, P;
  int seedIeta, seedIphi, seedIx, seedIy, seedZside;
  
  ntu_DA->SetBranchStatus("*",0);
  ntu_DA->SetBranchStatus("runId",1);  
  ntu_DA->SetBranchStatus("timeStampHigh",1);
  ntu_DA->SetBranchStatus("PV_n",1);
  ntu_DA->SetBranchStatus("ele1_scLaserCorr",1);
  ntu_DA->SetBranchStatus("ele1_seedLaserAlpha",1);
  ntu_DA->SetBranchStatus("ele1_EOverP",1);
  ntu_DA->SetBranchStatus("ele1_scEta",1);
  ntu_DA->SetBranchStatus("ele1_scPhi",1);
  ntu_DA->SetBranchStatus("ele1_scE",1);
  ntu_DA->SetBranchStatus("ele1_scE_regression",1);
  ntu_DA->SetBranchStatus("ele1_scE",1);
  ntu_DA->SetBranchStatus("ele1_es",1);
  ntu_DA->SetBranchStatus("ele1_tkP",1);
  ntu_DA->SetBranchStatus("ele1_seedIeta",1);
  ntu_DA->SetBranchStatus("ele1_seedIphi",1);
  ntu_DA->SetBranchStatus("ele1_seedIx",1);
  ntu_DA->SetBranchStatus("ele1_seedIy",1);
  ntu_DA->SetBranchStatus("ele1_seedZside",1);
    
  ntu_DA->SetBranchAddress("runId", &runId);  
  ntu_DA->SetBranchAddress("timeStampHigh", &timeStampHigh);
  ntu_DA->SetBranchAddress("PV_n", &PV_n);
  ntu_DA->SetBranchAddress("ele1_scLaserCorr", &scLaserCorr);
  ntu_DA->SetBranchAddress("ele1_seedLaserAlpha", &seedLaserAlpha);
  ntu_DA->SetBranchAddress("ele1_EOverP", &EoP);
  ntu_DA->SetBranchAddress("ele1_scEta", &scEta);
  ntu_DA->SetBranchAddress("ele1_scPhi", &scPhi);
  if( useRegression < 1 )
    ntu_DA->SetBranchAddress("ele1_scE", &scE);
  else
    ntu_DA->SetBranchAddress("ele1_scE_regression", &scE);
  ntu_DA->SetBranchAddress("ele1_scE", &scE);
  ntu_DA->SetBranchAddress("ele1_es", &ES);
  ntu_DA->SetBranchAddress("ele1_tkP", &P);
  ntu_DA->SetBranchAddress("ele1_seedIeta", &seedIeta);
  ntu_DA->SetBranchAddress("ele1_seedIphi", &seedIphi);
  ntu_DA->SetBranchAddress("ele1_seedIx", &seedIx);
  ntu_DA->SetBranchAddress("ele1_seedIy", &seedIy);
  ntu_DA->SetBranchAddress("ele1_seedZside", &seedZside);
  
  
  ntu_MC->SetBranchStatus("*",0);
  ntu_MC->SetBranchStatus("runId",1);
  ntu_MC->SetBranchStatus("PV_n",1);
  ntu_MC->SetBranchStatus("ele1_scEta",1);
  ntu_MC->SetBranchStatus("ele1_EOverP",1);
  ntu_MC->SetBranchStatus("ele1_scE",1);
  ntu_MC->SetBranchStatus("ele1_scE_regression",1);
  ntu_MC->SetBranchStatus("ele1_es",1);
  ntu_MC->SetBranchStatus("ele1_tkP",1);
  ntu_MC->SetBranchStatus("ele1_seedIeta",1);
  ntu_MC->SetBranchStatus("ele1_seedIphi",1);
  ntu_MC->SetBranchStatus("ele1_seedIx",1);
  ntu_MC->SetBranchStatus("ele1_seedIy",1);
  ntu_MC->SetBranchStatus("ele1_seedZside",1);
  
  ntu_MC->SetBranchAddress("runId", &runId);  
  ntu_MC->SetBranchAddress("PV_n", &PV_n);
  ntu_MC->SetBranchAddress("ele1_scEta", &scEta);
  ntu_MC->SetBranchAddress("ele1_EOverP", &EoP);
  if( useRegression < 1 )
    ntu_MC->SetBranchAddress("ele1_scE", &scE);
  else
    ntu_MC->SetBranchAddress("ele1_scE_regression", &scE);
  ntu_MC->SetBranchAddress("ele1_es", &ES);
  ntu_MC->SetBranchAddress("ele1_tkP", &P);
  ntu_MC->SetBranchAddress("ele1_seedIeta", &seedIeta);
  ntu_MC->SetBranchAddress("ele1_seedIphi", &seedIphi);
  ntu_MC->SetBranchAddress("ele1_seedIx", &seedIx);
  ntu_MC->SetBranchAddress("ele1_seedIy", &seedIy);
  ntu_MC->SetBranchAddress("ele1_seedZside", &seedZside);
  
  
  
  
  
  
  //--------------------------------------------------------
  // Define PU correction (to be used if useRegression == 0)
  
  // corr = p0 + p1 * PV_n
  float p0_EB;
  float p1_EB;
  float p0_EE;
  float p1_EE;
  
  if( useRegression == 0 )
  {
    //2012 EB
    p0_EB = 0.9991;
    p1_EB = 0.0001635;
    //2012 EE
    p0_EE = 0.9968;
    p1_EE = 0.001046;
  }
  else
  {
    //2012 EB
    p0_EB = 1.001;
    p1_EB = -0.000143;
    //2012 EE
    p0_EE = 1.00327;
    p1_EE = -0.000432;
  }
  
  float p0 = -1.;
  float p1 = -1.;
  
  if( strcmp(EBEE,"EB") == 0 )
  {
    p0 = p0_EB;
    p1 = p1_EB;
  }
  else
  {
    p0 = p0_EE;
    p1 = p1_EE;
  }
  
  
  
  
  
  
  //---------------------------------
  // Build the reference distribution
  
  std::cout << std::endl;
  std::cout << "***** Build reference for " << EBEE << " *****" << std::endl;
  
  TH1F* h_template = new TH1F("template", "", 2000, 0., 5.);
  
  for(int ientry = 0; ientry < ntu_MC->GetEntries(); ++ientry)
  {
    if( (ientry%100000 == 0) ) std::cout << "reading MC entry " << ientry << "\r" << std::flush;
    ntu_MC->GetEntry(ientry);
    
    // selections
    if( (strcmp(EBEE,"EB") == 0) && (fabs(scEta) > 1.45) )                    continue; // barrel
    if( (strcmp(EBEE,"EE") == 0) && (fabs(scEta) < 1.47 || fabs(scEta)>2.7) ) continue; // endcap

    if( (absEtaMin != -1.) && (absEtaMax != -1.) )
    {
      if( (fabs(scEta) < absEtaMin) || (fabs(scEta) > absEtaMax) ) continue;
    }
    
    if( (IetaMin != -1.) && (IetaMax != -1.) && (IphiMin != -1.) && (IphiMax != -1.) )
    {
      if( (seedIeta < IetaMin) || (seedIeta > IetaMax) ) continue;
      if( (seedIphi < IphiMin) || (seedIphi > IphiMax) ) continue;
    }
    
    // PU correction
    float PUCorr = (p0 + p1*PV_n);
    //std::cout << "p0: " << p0  << "   p1: " << p1 << "   PV_n: " << PV_n << std::endl;
    
    // fill the template histogram
    h_template -> Fill( (scE-ES)/(P-ES) / PUCorr );
  }
  
  std::cout << "Reference built for " << EBEE << " - " << h_template->GetEntries() << " events" << std::endl;
  
  
  
  
  
  
  //---------------------
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
    
    // selections
    if( (strcmp(EBEE,"EB") == 0) && (fabs(scEta) > 1.45) )                    continue; // barrel
    if( (strcmp(EBEE,"EE") == 0) && (fabs(scEta) < 1.47 || fabs(scEta)>2.7) ) continue; // endcap
    
    if( (absEtaMin != -1.) && (absEtaMax != -1.) )
    {
      if( (fabs(scEta) < absEtaMin) || (fabs(scEta) > absEtaMax) ) continue;
    }
    
    if( (IetaMin != -1.) && (IetaMax != -1.) && (IphiMin != -1.) && (IphiMax != -1.) )
    {
      if( (seedIeta < IetaMin) || (seedIeta > IetaMax) ) continue;
      if( (seedIphi < IphiMin) || (seedIphi > IphiMax) ) continue;
    }
    
    if( timeStampHigh < t1 ) continue;
    if( timeStampHigh > t2 ) continue;
    
    isSavedEntries.at(ientry) = true;
    
    
    // fill sorter
    Sorter dummy;
    dummy.time = timeStampHigh;
    dummy.entry = ientry;
    sortedEntries.push_back(dummy);
    
    ++nSavePts;
  }
  
  // sort events
  std::sort(sortedEntries.begin(),sortedEntries.end(),Sorter());
  std::cout << "Data sorted in " << EBEE << " - " << nSavePts << " events" << std::endl;
  
  std::map<int,int> antiMap;
  for(unsigned int iSaved = 0; iSaved < sortedEntries.size(); ++iSaved)
    antiMap[sortedEntries.at(iSaved).entry] = iSaved;
  
  
  //---------------------
  // Loop and define bins
  
  // "wide" bins - find events with time separation bigger than 1 day
  int nWideBins = 1;
  std::vector<int> wideBinEntryMax;
  int timeStampOld = -1;
  
  wideBinEntryMax.push_back(0);  
  for(int iSaved = 0; iSaved < nSavePts; ++iSaved)
  {
    if( iSaved%100000 == 0 ) std::cout << "reading saved entry " << iSaved << "\r" << std::flush;
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
  std::cout << std::endl;
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
  
  
  
  
  
  
  //---------------------
  // histogram definition
  
  TH1F* h_scOccupancy_eta  = new TH1F("h_scOccupancy_eta","", 298, -2.6, 2.6);
  TH1F* h_scOccupancy_phi  = new TH1F("h_scOccupancy_phi","", 363, -3.1765, 3.159);
  SetHistoStyle(h_scOccupancy_eta);
  SetHistoStyle(h_scOccupancy_phi);
  
  TH2F* h_seedOccupancy_EB  = new TH2F("h_seedOccupancy_EB","",  171, -85., 86., 361,   0.,361.);
  TH2F* h_seedOccupancy_EEp = new TH2F("h_seedOccupancy_EEp","", 101,   0.,101., 100,   0.,101.);
  TH2F* h_seedOccupancy_EEm = new TH2F("h_seedOccupancy_EEm","", 101,   0.,101., 100,   0.,101.);
  SetHistoStyle(h_seedOccupancy_EB);
  SetHistoStyle(h_seedOccupancy_EEp);
  SetHistoStyle(h_seedOccupancy_EEm);
  
  TH1F* h_EoP_spread = new TH1F("h_EoP_spread","",100,yMIN,yMAX);
  TH1F* h_EoC_spread = new TH1F("h_EoC_spread","",100,yMIN,yMAX);
  TH1F* h_EoP_spread_run = new TH1F("h_EoP_spread_run","",100,yMIN,yMAX);
  TH1F* h_EoC_spread_run = new TH1F("h_EoC_spread_run","",100,yMIN,yMAX);
  SetHistoStyle(h_EoP_spread,"EoP");
  SetHistoStyle(h_EoC_spread,"EoC");
  SetHistoStyle(h_EoP_spread_run,"EoP");
  SetHistoStyle(h_EoC_spread_run,"EoC");
  
  TH1F* h_EoP_chi2 = new TH1F("h_EoP_chi2","",50,0.,5.);
  TH1F* h_EoC_chi2 = new TH1F("h_EoC_chi2","",50,0.,5.);
  SetHistoStyle(h_EoP_chi2,"EoP");
  SetHistoStyle(h_EoC_chi2,"EoC");  
  
  TH1F** h_EoP = new TH1F*[nBins];
  TH1F** h_EoC = new TH1F*[nBins];
  TH1F** h_Las = new TH1F*[nBins];
  TH1F** h_Tsp = new TH1F*[nBins];
  TH1F** h_Cvl = new TH1F*[nBins];
  
  for(int i = 0; i < nBins; ++i)
  {
    char histoName[80];
    
    sprintf(histoName, "EoP_%d", i);
    h_EoP[i] = new TH1F(histoName, histoName, 2000, 0., 5.);
    SetHistoStyle(h_EoP[i],"EoP");
    
    sprintf(histoName, "EoC_%d", i);
    h_EoC[i] = new TH1F(histoName, histoName, 2000, 0., 5.);
    SetHistoStyle(h_EoC[i],"EoC");
    
    sprintf(histoName, "Las_%d", i);
    h_Las[i] = new TH1F(histoName, histoName, 500, 0.5, 1.5);
    
    sprintf(histoName, "Tsp_%d", i);
    h_Tsp[i] = new TH1F(histoName, histoName, 500, 0.5, 1.5);
  }
  
  
  // function definition
  TF1** f_EoP = new TF1*[nBins];
  TF1** f_EoC = new TF1*[nBins];
  
  
  // graphs definition
  TGraphAsymmErrors* g_fit   = new TGraphAsymmErrors();
  TGraphAsymmErrors* g_c_fit = new TGraphAsymmErrors();
  
  TGraphAsymmErrors* g_fit_run   = new TGraphAsymmErrors();
  TGraphAsymmErrors* g_c_fit_run = new TGraphAsymmErrors();  
  
  TGraph* g_las = new TGraph();
  
  TGraphErrors* g_LT = new TGraphErrors();
  
  g_fit->GetXaxis()->SetTimeFormat("%d/%m%F1970-01-01 00:00:00");
  g_fit->GetXaxis()->SetTimeDisplay(1);
  g_c_fit->GetXaxis()->SetTimeFormat("%d/%m%F1970-01-01 00:00:00");
  g_c_fit->GetXaxis()->SetTimeDisplay(1);
  g_las->GetXaxis()->SetTimeFormat("%d/%m%F1970-01-01 00:00:00");
  g_las->GetXaxis()->SetTimeDisplay(1);
  g_LT->GetXaxis()->SetTimeFormat("%d/%m%F1970-01-01 00:00:00");
  g_LT->GetXaxis()->SetTimeDisplay(1);
    
  
  
  
  
  
  //------------------------------------
  // loop on the saved and sorted events
  
  std::cout << std::endl;
  std::cout << "***** Fill and fit histograms *****" << std::endl;

  std::vector<int> Entries(nBins);  
  std::vector<double> AveTime(nBins);
  std::vector<int> MinTime(nBins);
  std::vector<int> MaxTime(nBins);
  std::vector<double> AveRun(nBins);    
  std::vector<int> MinRun(nBins);
  std::vector<int> MaxRun(nBins);
  std::vector<double> AveLT(nBins);
  std::vector<double> AveLT2(nBins);
    
  int iSaved = -1;
  for(int ientry = 0; ientry < nEntries; ++ientry)
  {
    if( (ientry%100000 == 0) ) std::cout << "reading entry " << ientry << "\r" << std::flush;
    
    if( isSavedEntries.at(ientry) == false ) continue;
    
    ++iSaved;
    
    int iSaved = antiMap[ientry];
    int bin = -1;
    for(bin = 0; bin < nBins; ++bin)
      if( iSaved >= binEntryMax.at(bin) && iSaved < binEntryMax.at(bin+1) )
	break;
    
    //std::cout << "bin = " << bin << "   iSaved = "<< iSaved << std::endl;
    ntu_DA->GetEntry(ientry);
    
    
    
    Entries[bin] += 1;
    
    if( iSaved == binEntryMax.at(bin)+1 )   MinTime[bin] = timeStampHigh;
    if( iSaved == binEntryMax.at(bin+1)-1 ) MaxTime[bin] = timeStampHigh;
    AveTime[bin] += timeStampHigh;
    
    if( iSaved == binEntryMax.at(bin)+1 )   MinRun[bin] = runId;
    if( iSaved == binEntryMax.at(bin+1)-1 ) MaxRun[bin] = runId;
    AveRun[bin] += runId;
    
    float LT = (-1. / seedLaserAlpha * log(scLaserCorr));
    AveLT[bin] += LT;
    AveLT2[bin] += LT*LT;
    
    // PU correction
    float PUCorr = (p0 + p1*PV_n);
    
    // fill the histograms
    (h_EoP[bin]) -> Fill( (scE-ES)/(P-ES) / scLaserCorr / PUCorr);
    (h_EoC[bin]) -> Fill( (scE-ES)/(P-ES) / PUCorr );
    
    (h_Las[bin]) -> Fill(scLaserCorr);
    (h_Tsp[bin]) -> Fill(1./scLaserCorr);
    
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
    AveRun[bin]  = 1. * AveRun[bin]  / (binEntryMax.at(bin+1)-binEntryMax.at(bin));
    AveLT[bin]   = 1. * AveLT[bin]   / (binEntryMax.at(bin+1)-binEntryMax.at(bin));
    AveLT2[bin]  = 1. * AveLT2[bin]  / (binEntryMax.at(bin+1)-binEntryMax.at(bin));
    //std::cout << date << " " << AveTime[i] << " " << MinTime[i] << " " << MaxTime[i] << std::endl;
  }
  
  
  
  
  
  
  int rebin = 2;
  if( strcmp(EBEE,"EE") == 0 ) rebin *= 2;
  
  h_template -> Rebin(rebin);
  
  
  
  float EoP_scale = 0.;
  float EoP_err = 0.;
  int   EoP_nActiveBins = 0;
  
  float EoC_scale = 0.;
  float EoC_err = 0.;
  int   EoC_nActiveBins = 0;
  
  float LCInv_scale = 0;
  
  std::vector<int> validBins;
  for(int i = 0; i < nBins; ++i)
  {
    bool isValid = true;
    
    h_EoP[i] -> Rebin(rebin);
    h_EoC[i] -> Rebin(rebin);
    
    
    
    //------------------------------------
    // Fill the graph for uncorrected data
    
    // define the fitting function
    // N.B. [0] * ( [1] * f( [1]*(x-[2]) ) )
    
    //o -> cd();
    char convolutionName[50];
    sprintf(convolutionName,"h_convolution_%d",i);
    //h_Cvl[i] = ConvoluteTemplate(std::string(convolutionName),h_template,h_Las[i],32768,-5.,5.);
    h_Cvl[i] = MellinConvolution(std::string(convolutionName),h_template,h_Tsp[i]);
    
    histoFunc* templateHistoFunc = new histoFunc(h_template);
    histoFunc* templateConvolutedHistoFunc = new histoFunc(h_Cvl[i]);
    char funcName[50];

    sprintf(funcName,"f_EoP_%d",i);
    
    if( strcmp(EBEE,"EB") == 0 )
      f_EoP[i] = new TF1(funcName, templateConvolutedHistoFunc, 0.8*(h_Tsp[i]->GetMean()), 1.4*(h_Tsp[i]->GetMean()), 3, "histoFunc");
    else
      f_EoP[i] = new TF1(funcName, templateConvolutedHistoFunc, 0.75*(h_Tsp[i]->GetMean()), 1.5*(h_Tsp[i]->GetMean()), 3, "histoFunc");
    
    f_EoP[i] -> SetParName(0,"Norm"); 
    f_EoP[i] -> SetParName(1,"Scale factor"); 
    f_EoP[i] -> SetLineWidth(1); 
    f_EoP[i] -> SetNpx(10000);
    
    double xNorm = h_EoP[i]->GetEntries()/h_template->GetEntries() *
      h_EoP[i]->GetBinWidth(1)/h_template->GetBinWidth(1); 
    
    f_EoP[i] -> FixParameter(0, xNorm);
    f_EoP[i] -> SetParameter(1, 1.);
    f_EoP[i] -> FixParameter(2, 0.);
    f_EoP[i] -> SetLineColor(kRed+2); 
    
    int fStatus = 0;
    int nTrials = 0;
    TFitResultPtr rp;
    
    rp = h_EoP[i] -> Fit(funcName, "QERLS+");
    while( (fStatus != 0) && (nTrials < 10) )
    {
      rp = h_EoP[i] -> Fit(funcName, "QERLS+");
      fStatus = rp;
      if(fStatus == 0) break;
      ++nTrials;
    }
    
    // fill the graph
    double eee = f_EoP[i]->GetParError(1);
    //float k    = f_EoP[i]->GetParameter(1);
    float k    = f_EoP[i]->GetParameter(1) / h_Tsp[i]->GetMean(); //needed when using mellin's convolution 
    
    if( (h_EoP[i]->GetEntries() > 3) && (fStatus == 0) && (eee > 0.05*h_template->GetRMS()/sqrt(evtsPerPoint)) )
    {
      float date = (float)AveTime[i];
      float dLow = (float)(AveTime[i]-MinTime[i]); 
      float dHig = (float)(MaxTime[i]-AveTime[i]);
      float run = (float)AveRun[i];
      float rLow = (float)(AveRun[i]-MinRun[i]); 
      float rHig = (float)(MaxRun[i]-AveRun[i]);
      
      g_fit -> SetPoint(i,  date , 1./k);
      g_fit -> SetPointError(i, dLow , dHig, eee/k/k, eee/k/k);
      
      g_fit_run -> SetPoint(i,  run , 1./k);
      g_fit_run -> SetPointError(i, rLow , rHig, eee/k/k, eee/k/k);
      
      h_EoP_chi2 -> Fill(f_EoP[i]->GetChisquare()/f_EoP[i]->GetNDF());
      
      EoP_scale += 1./k;
      EoP_err += eee/k/k;
      ++EoP_nActiveBins;
    }
    else
    {
      std::cout << "Fitting uncorrected time bin: " << i << "   Fail status: " << fStatus << "   sigma: " << eee << std::endl;
      isValid = false;
    }  
    
    //----------------------------------
    // Fill the graph for corrected data
    
    // define the fitting function
    // N.B. [0] * ( [1] * f( [1]*(x-[2]) ) )

    sprintf(funcName,"f_EoC_%d",i);
    if( strcmp(EBEE,"EB") == 0 )
      f_EoC[i] = new TF1(funcName, templateHistoFunc, 0.8, 1.4, 3, "histoFunc");
    else
      f_EoC[i] = new TF1(funcName, templateHistoFunc, 0.75, 1.5, 3, "histoFunc");
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
    
    
    rp = h_EoC[i] -> Fit(funcName, "QERLS+");
    fStatus = rp;
    nTrials = 0;
    while( (fStatus != 0) && (nTrials < 10) )
    {
      rp = h_EoC[i] -> Fit(funcName, "QERLS+");
      fStatus = rp;
      if(fStatus == 0) break;
      ++nTrials;
    }
    
    // fill the graph
    k   = f_EoC[i]->GetParameter(1);
    eee = f_EoC[i]->GetParError(1); 
    
    if( (h_EoC[i]->GetEntries() > 10) && (fStatus == 0) && (eee > 0.05*h_template->GetRMS()/sqrt(evtsPerPoint)) )
    {
      float date = (float)AveTime[i]; 
      float dLow = (float)(AveTime[i]-MinTime[i]); 
      float dHig = (float)(MaxTime[i]-AveTime[i]);
      float run = (float)AveRun[i];
      float rLow = (float)(AveRun[i]-MinRun[i]); 
      float rHig = (float)(MaxRun[i]-AveRun[i]);
      
      g_c_fit -> SetPoint(i,  date , 1./k);
      g_c_fit -> SetPointError(i, dLow , dHig , eee/k/k, eee/k/k);
      
      g_c_fit_run -> SetPoint(i,  run , 1./k);
      g_c_fit_run -> SetPointError(i, rLow , rHig, eee/k/k, eee/k/k);
      
      h_EoC_chi2 -> Fill(f_EoC[i]->GetChisquare()/f_EoP[i]->GetNDF());
      
      EoC_scale += 1./k;
      EoC_err += eee/k/k;
      ++EoC_nActiveBins;
    }
    else
    {
      std::cout << "Fitting corrected time bin: " << i << "   Fail status: " << fStatus << "   sigma: " << eee << std::endl;
      isValid = false;
    }
    
    if( isValid == true ) validBins.push_back(i);
  }
  
  EoP_scale /= EoP_nActiveBins;
  EoP_err   /= EoP_nActiveBins;
  
  EoC_scale /= EoC_nActiveBins;
  EoC_err   /= EoC_nActiveBins;
  
  
  
  
  
  
  //----------------------------------------
  // Fill the graph for avg laser correction
  
  //fede
  for(unsigned int itr = 0; itr < validBins.size(); ++itr)
  {  
    int i = validBins.at(itr);
    g_las -> SetPoint(itr, (float)AveTime[i], h_Tsp[i]->GetMean() );
    g_LT  -> SetPoint(itr, (float)AveTime[i], AveLT[i] );
    g_LT  -> SetPointError(itr, 0., sqrt(AveLT2[i]-AveLT[i]*AveLT[i]) / sqrt(Entries[i]) );
        
    LCInv_scale += h_Tsp[i]->GetMean();
  }  
  
  LCInv_scale /= validBins.size();
  
  
  
  
  
  
  //---------------
  // Rescale graphs
  
  float yscale = 1.;
  //float yscale = 1./EoC_scale;
  
  for(unsigned int itr = 0; itr < validBins.size(); ++itr)
  {
    double x,y; 
    g_fit -> GetPoint(itr,x,y); 
    g_fit -> SetPoint(itr,x,y*yscale);
    if ( (x > t1) && (x < t2) ) h_EoP_spread -> Fill(y*yscale);
    
    g_c_fit -> GetPoint(itr,x,y); 
    g_c_fit -> SetPoint(itr,x,y*yscale);
    if ( (x > t1) && (x < t2) ) h_EoC_spread -> Fill(y*yscale);
    
    g_fit_run -> GetPoint(itr,x,y); 
    g_fit_run -> SetPoint(itr,x,y*yscale); 
    if ( (x > t1) && (x < t2) ) h_EoP_spread_run -> Fill(y*yscale);
    
    g_c_fit_run -> GetPoint(itr,x,y); 
    g_c_fit_run -> SetPoint(itr,x,y*yscale);
    if ( (x > t1) && (x < t2) ) h_EoC_spread_run -> Fill(y*yscale);
    
    g_las -> GetPoint(itr,x,y);
    g_las -> SetPoint(itr,x,y*yscale*EoP_scale/LCInv_scale);
  }
  
  TF1 EoC_pol0("EoC_pol0","pol0",t1,t2);
  EoC_pol0.SetLineColor(kGreen+2);
  EoC_pol0.SetLineWidth(2);
  EoC_pol0.SetLineStyle(2);
  g_c_fit -> Fit("EoC_pol0","QNR");
  
  
  
  
  
  
  
  
  //----------------------------
  // Print out global quantities
  
  std::cout << std::endl;
  std::cout << "***** Mean scales and errors *****" << std::endl; 
  std::cout << std::fixed;
  std::cout << std::setprecision(4);
  std::cout << "Mean EoP scale: "  << std::setw(6) << EoP_scale   << "   mean EoP error: " << std::setw(8) << EoP_err << std::endl;
  std::cout << "Mean EoC scale: "  << std::setw(6) << EoC_scale   << "   mean EoC error: " << std::setw(8) << EoC_err << std::endl;
  std::cout << "Mean 1/LC scale: " << std::setw(6) << LCInv_scale << std::endl;
  
  
  
  
  
  
  //-----------------
  // Occupancy plots
  //-----------------
  
  TCanvas* c_scOccupancy = new TCanvas("c_scOccupancy","SC occupancy",0,0,1000,500);
  c_scOccupancy -> Divide(2,1);
  
  c_scOccupancy -> cd(1);
  h_scOccupancy_eta -> GetXaxis() -> SetTitle("sc #eta");
  h_scOccupancy_eta -> GetYaxis() -> SetTitle("events");
  h_scOccupancy_eta -> Draw();
  
  c_scOccupancy -> cd(2);
  h_scOccupancy_phi -> GetXaxis() -> SetTitle("sc #phi");
  h_scOccupancy_phi -> GetYaxis() -> SetTitle("events");
  h_scOccupancy_phi -> Draw();
  
  TCanvas* c_seedOccupancy = new TCanvas("c_seedOccupancy","seed occupancy",0,0,1500,500);
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
  
  
  
  //-----------
  // Chi2 plots
  //-----------
  
  TCanvas* c_chi2 = new TCanvas("c_chi2","fit chi2",0,0,500,500);
  c_chi2 -> cd();
  
  h_EoC_chi2 -> GetXaxis() -> SetTitle("#chi^{2}/N_{dof}");
  h_EoC_chi2 -> Draw("");
  gPad -> Update();
    
  TPaveStats* s_EoC = new TPaveStats;
  s_EoC = (TPaveStats*)(h_EoC_chi2->GetListOfFunctions()->FindObject("stats"));
  s_EoC -> SetStatFormat("1.4g");
  s_EoC -> SetTextColor(kGreen+2);
  s_EoC->SetY1NDC(0.59);
  s_EoC->SetY2NDC(0.79);
  s_EoC -> Draw("sames");
  gPad -> Update();
  
  h_EoP_chi2 -> GetXaxis() -> SetTitle("#chi^{2}/N_{dof}");
  h_EoP_chi2 -> Draw("sames");
  gPad -> Update();
  
  TPaveStats* s_EoP = new TPaveStats;
  s_EoP = (TPaveStats*)(h_EoP_chi2->GetListOfFunctions()->FindObject("stats"));
  s_EoP -> SetStatFormat("1.4g");
  s_EoP -> SetTextColor(kRed+2);
  s_EoP->SetY1NDC(0.79);
  s_EoP->SetY2NDC(0.99);
  s_EoP -> Draw("sames");
  gPad -> Update();
  
  
  
  
  
  
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

  float tYoffset = 1.0; 
  float labSize = 0.05;
  float labSize2 = 0.06;

  cLeft->cd(); 
  
  cLeft->SetGridx();
  cLeft->SetGridy();
  
  TH1F *hPad = (TH1F*)gPad->DrawFrame(t1,0.9,t2,1.05);
  hPad->GetXaxis()->SetTimeFormat("%d/%m%F1970-01-01 00:00:00");
  hPad->GetXaxis()->SetTimeDisplay(1);
  hPad->GetXaxis() -> SetRangeUser(MinTime[0]-43200,MaxTime[nBins-1]+43200);
  hPad->GetXaxis()->SetTitle("date (day/month)");
  if( strcmp(EBEE,"EB") == 0 )
    hPad->GetYaxis()->SetTitle("Relative E/p scale"); 
  else
    hPad->GetYaxis()->SetTitle("Relative E/p scale"); 
  hPad->GetYaxis()->SetTitleOffset(tYoffset);
  hPad->GetXaxis()->SetLabelSize(labSize);
  hPad->GetXaxis()->SetTitleSize(labSize2);
  hPad->GetYaxis()->SetLabelSize(labSize);
  hPad->GetYaxis()->SetTitleSize(labSize2);
  hPad -> SetMinimum(yMIN);
  hPad -> SetMaximum(yMAX);
  
  // draw history plot
  g_fit -> SetMarkerStyle(24);
  g_fit -> SetMarkerSize(0.7);
  g_fit -> SetMarkerColor(kRed+2);
  g_fit -> SetLineColor(kRed+2);
  g_fit -> Draw("P");
  g_c_fit -> SetMarkerStyle(20);
  g_c_fit -> SetMarkerColor(kGreen+2);
  g_c_fit -> SetLineColor(kGreen+2);
  g_c_fit -> SetMarkerSize(0.7);
  g_c_fit -> Draw("P,same");
  g_las -> SetLineColor(kAzure-2);
  g_las -> SetLineWidth(2);
  g_las -> Draw("L,same");
  
  TLegend* legend = new TLegend(0.60,0.78,0.90,0.94);
  legend -> SetLineColor(kWhite);
  legend -> SetLineWidth(0);
  legend -> SetFillColor(kWhite);
  legend -> SetFillStyle(0);
  legend -> SetTextFont(42);
  legend -> SetTextSize(0.04);
  legend -> AddEntry(g_c_fit,"with LM correction","PL");
  legend -> AddEntry(g_fit,  "without LM correction","PL");
  legend -> AddEntry(g_las,  "1 / LM correction","L");
  legend -> Draw("same");
  
  char latexBuffer[250];
  
  sprintf(latexBuffer,"CMS 2012 Preliminary");
  TLatex* latex = new TLatex(0.18,0.89,latexBuffer);  
  latex -> SetNDC();
  latex -> SetTextFont(62);
  latex -> SetTextSize(0.05);
  latex -> Draw("same");
  
  //sprintf(latexBuffer,"#sqrt{s} = 8 TeV   L = 3.95 fb^{-1}");
  sprintf(latexBuffer,"#sqrt{s} = 8 TeV");
  TLatex* latex2 = new TLatex(0.18,0.84,latexBuffer);  
  latex2 -> SetNDC();
  latex2 -> SetTextFont(42);
  latex2 -> SetTextSize(0.05);
  latex2 -> Draw("same");
  
  if( strcmp(EBEE,"EB") == 0 )
    sprintf(latexBuffer,"ECAL Barrel");
  else
    sprintf(latexBuffer,"ECAL Endcap");
  TLatex* latex3 = new TLatex(0.18,0.19,latexBuffer);
  latex3 -> SetNDC();
  latex3 -> SetTextFont(42);
  latex3 -> SetTextSize(0.05);
  latex3 -> Draw("same");
  
  //sprintf(latexBuffer,"%.2E events",1.*nSavePts);
  //TLatex* latex4 = new TLatex(0.18,0.24,latexBuffer);  
  //latex4 -> SetNDC();
  //latex4 -> SetTextFont(42);
  //latex4 -> SetTextSize(0.04);
  //latex4 -> Draw("same");
  //
  //sprintf(latexBuffer,"%d events/bin - %d bins",evtsPerPoint,nBins);
  //TLatex* latex5 = new TLatex(0.18,0.19,latexBuffer);  
  //latex5 -> SetNDC();
  //latex5 -> SetTextFont(42);
  //latex5 -> SetTextSize(0.04);
  //latex5 -> Draw("same");
  
  
  cRight -> cd();
  
  TPaveStats* s_EoP_spread = new TPaveStats();
  TPaveStats* s_EoC_spread = new TPaveStats();
  
  
  h_EoC_spread -> SetFillStyle(3001);
  h_EoC_spread -> SetFillColor(kGreen+2);
  h_EoC_spread->GetYaxis()->SetLabelSize(0.09);
  h_EoC_spread->GetYaxis()->SetLabelOffset(-0.03);
  h_EoC_spread->GetYaxis()->SetTitleSize(0.08);
  h_EoC_spread->GetYaxis()->SetNdivisions(505);
  h_EoC_spread->GetXaxis()->SetLabelOffset(1000);
  
  h_EoC_spread -> Draw("hbar");
  gPad -> Update();
  
  s_EoC_spread = (TPaveStats*)(h_EoC_spread->GetListOfFunctions()->FindObject("stats"));
  s_EoC_spread -> SetStatFormat("1.4g");
  s_EoC_spread->SetX1NDC(0.06); //new x start position
  s_EoC_spread->SetX2NDC(0.71); //new x end position
  s_EoC_spread->SetY1NDC(0.93); //new x start position
  s_EoC_spread->SetY2NDC(0.84); //new x end position
  s_EoC_spread -> SetOptStat(1100);
  s_EoC_spread ->SetTextColor(kGreen+2);
  s_EoC_spread ->SetTextSize(0.08);
  s_EoC_spread -> Draw("sames");
  
  
  h_EoP_spread -> SetFillStyle(3001);
  h_EoP_spread -> SetFillColor(kRed+2);
  h_EoP_spread -> Draw("hbarsames");
  gPad -> Update();
  s_EoP_spread = (TPaveStats*)(h_EoP_spread->GetListOfFunctions()->FindObject("stats"));
  s_EoP_spread -> SetStatFormat("1.4g");
  s_EoP_spread->SetX1NDC(0.06); //new x start position
  s_EoP_spread->SetX2NDC(0.71); //new x end position
  s_EoP_spread->SetY1NDC(0.83); //new x start position
  s_EoP_spread->SetY2NDC(0.74); //new x end position
  s_EoP_spread ->SetOptStat(1100);
  s_EoP_spread ->SetTextColor(kRed+2);
  s_EoP_spread ->SetTextSize(0.08);
  s_EoP_spread -> Draw("sames");
  
  /*
  h_EoP_spread -> SetFillStyle(3001);
  h_EoP_spread -> SetFillColor(kRed+2);
  h_EoP_spread -> Draw("hbarsame");
  gPad -> Update();
  */
  
  
  
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
  if( strcmp(EBEE,"EB") == 0 )
    hPad->GetYaxis()->SetTitle("Relative E/p scale"); 
  else
    hPad->GetYaxis()->SetTitle("Relative E/p scale"); 
  hPad->GetYaxis()->SetTitleOffset(tYoffset);
  hPad->GetXaxis()->SetLabelSize(labSize);
  hPad->GetXaxis()->SetTitleSize(labSize);
  hPad->GetYaxis()->SetLabelSize(labSize);
  hPad->GetYaxis()->SetTitleSize(labSize);
  hPad -> SetMinimum(yMIN);
  hPad -> SetMaximum(yMAX);
  
  // draw history plot
  g_fit_run -> SetMarkerStyle(20);
  g_fit_run -> SetMarkerSize(0.7);
  g_fit_run -> SetMarkerColor(kRed+2);
  g_fit_run -> SetLineColor(kRed+2);
  g_fit_run -> Draw("P");
  g_c_fit_run -> SetMarkerStyle(20);
  g_c_fit_run -> SetMarkerColor(kGreen+2);
  g_c_fit_run -> SetLineColor(kGreen+2);
  g_c_fit_run -> SetMarkerSize(0.7);
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
  
  
  
  
  
  
  c_chi2 -> Print((folderName+"/"+folderName+"_fitChi2.png").c_str(),"png");
  c_scOccupancy -> Print((folderName+"/"+folderName+"_scOccupancy.png").c_str(),"png");
  c_seedOccupancy -> Print((folderName+"/"+folderName+"_seedOccupancy.png").c_str(),"png");
  cplot -> Print((folderName+"/"+folderName+"_history_vsTime.png").c_str(),"png");
  cplot_run -> Print((folderName+"/"+folderName+"_history_vsRun.png").c_str(),"png");
  
  c_chi2 -> Print((folderName+"/"+folderName+"_fitChi2.pdf").c_str(),"pdf");
  c_scOccupancy -> Print((folderName+"/"+folderName+"_scOccupancy.pdf").c_str(),"pdf");
  c_seedOccupancy -> Print((folderName+"/"+folderName+"_seedOccupancy.pdf").c_str(),"pdf");
  cplot -> Print((folderName+"/"+folderName+"_history_vsTime.pdf").c_str(),"pdf");
  cplot_run -> Print((folderName+"/"+folderName+"_history_vsRun.pdf").c_str(),"pdf");
  
  cplot -> SaveAs((folderName+"/"+folderName+"_history_vsTime.C").c_str());
  cplot_run -> SaveAs((folderName+"/"+folderName+"_history_vsRun.C").c_str());
  
  
  
   o -> cd();
  
   h_template -> Write();

   h_scOccupancy_eta   -> Write();
   h_scOccupancy_phi   -> Write();
   h_seedOccupancy_EB  -> Write(); 
   h_seedOccupancy_EEp -> Write();
   h_seedOccupancy_EEm -> Write();

   g_fit   -> Write("g_fit");
   g_c_fit -> Write("g_c_fit");
   g_fit_run   -> Write("g_fit_run");
   g_c_fit_run -> Write("g_c_fit_run");
   g_las -> Write("g_las");
   g_LT -> Write("g_LT");
   
   h_EoP_chi2 -> Write();
   h_EoC_chi2 -> Write();
  
   //for(int i = 0; i < nBins; ++i)
   //{
   //  h_EoP[i] -> GetXaxis() -> SetTitle("E/p");
   //  h_EoP[i] -> Write();
   //
   //  h_EoC[i] -> GetXaxis() -> SetTitle("E/p");
   //  h_EoC[i] -> Write();
   //
   //  h_Tsp[i] -> Write();
   //
   //  h_Cvl[i] -> Write();
   //}

  o -> Close();
}
