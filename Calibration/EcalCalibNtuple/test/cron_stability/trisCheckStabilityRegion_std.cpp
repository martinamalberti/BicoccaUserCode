// g++ -Wall -o trisCheckStabilityRegion_std `root-config --cflags --glibs` -L/gwteraw/cmssw/slc5_amd64_gcc462/external/gcc/4.6.2/lib64/ setTDRStyle.cc ntupleUtils.cc geometryUtils.cc stabilityUtils.cc ConvoluteTemplate.cc histoFunc.h trisCheckStabilityRegion_std.cpp

//./trisCheckStabilityRegion_std EB 5000 1 SM 0        1 1 2012    31 12 2012    0.900 1.050   -1. -1.
//./trisCheckStabilityRegion_std EE 4000 1 RING 1      1 1 2012    31 12 2012    0.700 1.100   -1. -1.

// ***************************************************
// Plot EB or EE stability vs time and ancillary plots
// ***************************************************

#include "setTDRStyle.h"
#include "histoFunc.h"
#include "ConvoluteTemplate.h"
#include "ntupleUtils.h"
#include "stabilityUtils.h"
#include "geometryUtils.h"

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
  std::string regionType = std::string(argv[4]);
  int multiTemplate = atoi(argv[5]);
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
  if(argc >= 6)
  {
    dayMin = std::string(argv[6])+" "+std::string(argv[7])+" "+std::string(argv[8]);
    dayMax = std::string(argv[9])+" "+std::string(argv[10])+" "+std::string(argv[11]);
    dayMinLabel = std::string(argv[6])+"_"+std::string(argv[7])+"_"+std::string(argv[8]);
    dayMaxLabel = std::string(argv[9])+"_"+std::string(argv[10])+"_"+std::string(argv[11]);
    
    t1 = dateToInt(dayMin);
    t2 = dateToInt(dayMax);
  }
  if(argc >= 13)
  {
    yMIN = atof(argv[12]);
    yMAX = atof(argv[13]);
  }
  if(argc >= 15)
  {
    absEtaMin = atof(argv[14]);
    absEtaMax = atof(argv[15]);
  }
  if(argc >= 17)
  {
    IetaMin = atoi(argv[16]);
    IetaMax = atoi(argv[17]);
    IphiMin = atoi(argv[18]);
    IphiMax = atoi(argv[19]);
  }
  
  std::cout << "EBEE: "          << EBEE          << std::endl;
  std::cout << "evtsPerPoint: "  << evtsPerPoint  << std::endl;
  std::cout << "useRegression: " << useRegression << std::endl;
  std::cout << "regionType:    " << regionType    << std::endl;
  std::cout << "multiTemplate:" << multiTemplate<< std::endl;
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
  TFile* o = new TFile((folderName+"/"+folderName+"_"+regionType+"_histos.root").c_str(),"RECREATE");
  
  
  
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
  
  
  
  //---------------------
  // Set branch addresses
  
  int runId;
  int timeStampHigh;
  int PV_n;
  int isEB;
  float scLaserCorr, seedLaserAlpha, EoP, scEta, scPhi, scE, ES, P;
  int seedIeta, seedIphi, seedIx, seedIy, seedZside;
  
  ntu_MC->SetBranchStatus("*",0);
  ntu_MC->SetBranchStatus("runId",1);               ntu_MC->SetBranchAddress("runId", &runId);   
  ntu_MC->SetBranchStatus("timeStampHigh",1);       ntu_MC->SetBranchAddress("timeStampHigh", &timeStampHigh);
  ntu_MC->SetBranchStatus("PV_n",1);                ntu_MC->SetBranchAddress("PV_n", &PV_n);
  ntu_MC->SetBranchStatus("ele1_isEB",1);           ntu_MC->SetBranchAddress("ele1_isEB", &isEB);
  ntu_MC->SetBranchStatus("ele1_scLaserCorr",1);    ntu_MC->SetBranchAddress("ele1_scLaserCorr", &scLaserCorr);
  ntu_MC->SetBranchStatus("ele1_seedLaserAlpha",1); ntu_MC->SetBranchAddress("ele1_seedLaserAlpha", &seedLaserAlpha);
  ntu_MC->SetBranchStatus("ele1_EOverP",1);         ntu_MC->SetBranchAddress("ele1_EOverP", &EoP);
  ntu_MC->SetBranchStatus("ele1_scEta",1);          ntu_MC->SetBranchAddress("ele1_scEta", &scEta);
  ntu_MC->SetBranchStatus("ele1_scPhi",1);          ntu_MC->SetBranchAddress("ele1_scPhi", &scPhi);
  ntu_MC->SetBranchStatus("ele1_es",1);             ntu_MC->SetBranchAddress("ele1_es", &ES);
  ntu_MC->SetBranchStatus("ele1_tkP",1);            ntu_MC->SetBranchAddress("ele1_tkP", &P);
  ntu_MC->SetBranchStatus("ele1_seedIeta",1);       ntu_MC->SetBranchAddress("ele1_seedIeta", &seedIeta);
  ntu_MC->SetBranchStatus("ele1_seedIphi",1);       ntu_MC->SetBranchAddress("ele1_seedIphi", &seedIphi);
  ntu_MC->SetBranchStatus("ele1_seedIx",1);         ntu_MC->SetBranchAddress("ele1_seedIx", &seedIx);
  ntu_MC->SetBranchStatus("ele1_seedIy",1);         ntu_MC->SetBranchAddress("ele1_seedIy", &seedIy);
  ntu_MC->SetBranchStatus("ele1_seedZside",1);      ntu_MC->SetBranchAddress("ele1_seedZside", &seedZside);
  if( useRegression < 1 ) {
    ntu_MC->SetBranchStatus("ele1_scE",1);            ntu_MC->SetBranchAddress("ele1_scE", &scE);}
  else {
    ntu_MC->SetBranchStatus("ele1_scE_regression",1); ntu_MC->SetBranchAddress("ele1_scE_regression", &scE);}
  
  ntu_DA->SetBranchStatus("*",0);
  ntu_DA->SetBranchStatus("runId",1);               ntu_DA->SetBranchAddress("runId", &runId);   
  ntu_DA->SetBranchStatus("timeStampHigh",1);       ntu_DA->SetBranchAddress("timeStampHigh", &timeStampHigh);
  ntu_DA->SetBranchStatus("PV_n",1);                ntu_DA->SetBranchAddress("PV_n", &PV_n);
  ntu_DA->SetBranchStatus("ele1_isEB",1);           ntu_DA->SetBranchAddress("ele1_isEB", &isEB);
  ntu_DA->SetBranchStatus("ele1_scLaserCorr",1);    ntu_DA->SetBranchAddress("ele1_scLaserCorr", &scLaserCorr);
  ntu_DA->SetBranchStatus("ele1_seedLaserAlpha",1); ntu_DA->SetBranchAddress("ele1_seedLaserAlpha", &seedLaserAlpha);
  ntu_DA->SetBranchStatus("ele1_EOverP",1);         ntu_DA->SetBranchAddress("ele1_EOverP", &EoP);
  ntu_DA->SetBranchStatus("ele1_scEta",1);          ntu_DA->SetBranchAddress("ele1_scEta", &scEta);
  ntu_DA->SetBranchStatus("ele1_scPhi",1);          ntu_DA->SetBranchAddress("ele1_scPhi", &scPhi);
  ntu_DA->SetBranchStatus("ele1_es",1);             ntu_DA->SetBranchAddress("ele1_es", &ES);
  ntu_DA->SetBranchStatus("ele1_tkP",1);            ntu_DA->SetBranchAddress("ele1_tkP", &P);
  ntu_DA->SetBranchStatus("ele1_seedIeta",1);       ntu_DA->SetBranchAddress("ele1_seedIeta", &seedIeta);
  ntu_DA->SetBranchStatus("ele1_seedIphi",1);       ntu_DA->SetBranchAddress("ele1_seedIphi", &seedIphi);
  ntu_DA->SetBranchStatus("ele1_seedIx",1);         ntu_DA->SetBranchAddress("ele1_seedIx", &seedIx);
  ntu_DA->SetBranchStatus("ele1_seedIy",1);         ntu_DA->SetBranchAddress("ele1_seedIy", &seedIy);
  ntu_DA->SetBranchStatus("ele1_seedZside",1);      ntu_DA->SetBranchAddress("ele1_seedZside", &seedZside);
  if( useRegression < 1 ) {
    ntu_DA->SetBranchStatus("ele1_scE",1);            ntu_DA->SetBranchAddress("ele1_scE", &scE);}
  else {
    ntu_DA->SetBranchStatus("ele1_scE_regression",1); ntu_DA->SetBranchAddress("ele1_scE_regression", &scE);}  
  
  
  
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

  //-----------------------
  //---- define region ----
  TBarrelRegions *ebRegion;
  TEndcapRegions *eeRegion;

  TH2F* templateRegions;
  TH2F* regions;


  int nRegions = 1;
  int tRegions = 1;
  

  if( strcmp(EBEE,"EB") == 0 ) 
    {
      ebRegion = new TBarrelRegions();
      templateRegions = new TH2F("templateRegions", "",171,-85,86,360,1,361);
      regions = new TH2F("regions", "",171,-85,86,360,1,361);

      nRegions = ebRegion->GetNRegions(regionType);
      if(multiTemplate == 1)
	tRegions = ebRegion->GetNRegionsIeta(regionType);
      templateRegions -> GetZaxis() -> SetRangeUser(-0.1,tRegions);
      regions -> GetZaxis() -> SetRangeUser(-0.1,nRegions);
    }
  
  if( strcmp(EBEE,"EE") == 0 )
    {
      eeRegion = new TEndcapRegions();
      templateRegions = new TH2F("templateRegions","",100,1.,101.,100,1.,101.);
      regions = new TH2F("regions","",100,1.,101.,100,1.,101.);

      nRegions = 6;  //HARCODED. FIXME
      if(multiTemplate == 1)
	tRegions = nRegions;
      templateRegions -> GetZaxis() -> SetRangeUser(-0.1,tRegions);
      regions -> GetZaxis() -> SetRangeUser(-0.1,nRegions);
    }

  //------------------------------------
  //---- define template per region ----
  TH1F* h_template[tRegions];
  for(int ii = 0; ii < tRegions; ++ii)
    {
      char histoName[80];
      sprintf(histoName, "template_%d", ii);
      h_template[ii] = new TH1F(histoName, "", 2000, 0., 5.);
    }

  //--------------------------
  //---- fill region maps ----
  for(int iBin = 1; iBin <= templateRegions->GetNbinsX(); ++iBin)
    for(int jBin = 1; jBin <= templateRegions->GetNbinsY(); ++jBin)
      {
	int iEta = int( templateRegions->GetXaxis()->GetBinLowEdge(iBin) );
	int iPhi = int( templateRegions->GetYaxis()->GetBinLowEdge(jBin) );

	int regionId = -1;
	if( strcmp(EBEE,"EB") == 0 )
	  {
	    regionId = ebRegion->GetRegionId(iEta,iPhi,regionType);
	    templateRegions -> SetBinContent(iBin,jBin,ebRegion->GetRegionIdIeta(regionId,regionType));
	    regions -> SetBinContent(iBin,jBin,regionId);
	  }
	if( strcmp(EBEE,"EE") == 0 )
	  {
	    regionId = eeRegion->GetEndcapRing(iEta,iPhi,1,nRegions);
	    templateRegions -> SetBinContent(iBin,jBin,regionId);
	    regions -> SetBinContent(iBin,jBin,1,regionId);
	  }
      }

  
  for(int ientry = 0; ientry < ntu_MC->GetEntries(); ++ientry)
  {
    if( (ientry%100000 == 0) ) std::cout << "reading MC entry " << ientry << "\r" << std::flush;
    ntu_MC->GetEntry(ientry);

    
    // selections
    if( (strcmp(EBEE,"EB") == 0) && (isEB != 1) ) continue; // barrel
    if( (strcmp(EBEE,"EE") == 0) && (isEB != 0) ) continue; // endcap
    
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
    
    //choose region
    int regionId = -1;
    int regionIdIeta = -1;
    if( strcmp(EBEE,"EB") == 0 )
      {
	regionId     = ebRegion->GetRegionId(seedIeta, seedIphi, regionType);
	regionIdIeta = ebRegion->GetRegionIdIeta(regionId, regionType);	
      }
    if( strcmp(EBEE,"EE") == 0 )
      {
	regionId = eeRegion->GetEndcapRing(seedIx, seedIy, 1, nRegions);
	regionIdIeta = regionId;
      }
	
    // fill the template histogram
    if(multiTemplate == 0) regionIdIeta = 0;
    h_template[regionIdIeta] -> Fill( (scE-ES)/(P-ES) / PUCorr );
  }
  
  for(int ii=0; ii<tRegions; ++ii)
    std::cout << "Reference built for " << EBEE << " - " << h_template[ii]->GetEntries() << " events" << std::endl;
    
  
  
  //---------------------
  // Loop and sort events
  
  std::cout << std::endl;
  std::cout << "***** Sort events and define bins *****" << std::endl;
  
  int nEntries = ntu_DA -> GetEntriesFast(); 
  int nSavePts = 0; 
  std::vector<bool> isSavedEntries(nEntries);
  std::vector<myEvent> sortedEntries;
  std::vector<int> timeStampFirst;
  
  for(int ientry = 0; ientry < nEntries; ++ientry)
    {
      ntu_DA -> GetEntry(ientry);
      if( ientry%100000 == 0 ) std::cout << ">>> reading entry " << ientry << " / " << nEntries << "\r"<< std::flush;
      
      isSavedEntries.at(ientry) = false;
      
      // selections
      if( (strcmp(EBEE,"EB") == 0) && (isEB != 1) ) continue; // barrel
      if( (strcmp(EBEE,"EE") == 0) && (isEB != 0) ) continue; // endcap
      
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
      
      if( scLaserCorr <= 0. ) continue;
      
      isSavedEntries.at(ientry) = true;
      
      
      //choose region
      int regionId = -1;
      if( strcmp(EBEE,"EB") == 0 )
	regionId = ebRegion->GetRegionId(seedIeta, seedIphi, regionType);
      
      if( strcmp(EBEE,"EE") == 0 )
	regionId = eeRegion->GetEndcapRing(seedIx, seedIy, 1, nRegions);
      
      // fill sorter
      myEvent dummy;
      float PUCorr = (p0 + p1*PV_n);
      
      dummy.runId = runId;
      dummy.timeStampHigh = timeStampHigh;
      dummy.region = regionId;
      dummy.scE = (scE-ES) / PUCorr;
      dummy.P = (P - ES);
      dummy.scLaserCorr = scLaserCorr;
      dummy.seedLaserAlpha = seedLaserAlpha;
      
      sortedEntries.push_back(dummy);
      
      ++nSavePts;
    }
  
  // sort events
  std::sort(sortedEntries.begin(),sortedEntries.end(),myEvent());
  std::cout << "Data sorted in " << EBEE << " - " << nSavePts << " events" << std::endl;
  
  
  
  //---------------------
  // Loop and define bins
  int nBins = (int)(sortedEntries.size()/evtsPerPoint/nRegions*2.)+50; //+5 is needed for the interruptions
  
  std::cout << "nRegions = " << nRegions << std::endl;
  std::cout << "nBins = " << nBins << std::endl;
  
  
  //time infos
  std::map<int,std::map<int,int> > Entries;
  std::map<int,std::map<int,double> > AveTime;
  std::map<int,std::map<int,int> > MinTime;
  std::map<int,std::map<int,int> > MaxTime;
  std::map<int,std::map<int,double> > AveRun;    
  std::map<int,std::map<int,int> > MinRun;
  std::map<int,std::map<int,int> > MaxRun;
  std::map<int,std::map<int,double> > AveLT;
  std::map<int,std::map<int,double> > AveLT2;
  
  std::map<int,std::map<int,TH1F*> > h_EoP;
  std::map<int,std::map<int,TH1F*> > h_EoC;
  std::map<int,std::map<int,TH1F*> > h_Las;
  std::map<int,std::map<int,TH1F*> > h_Tsp;
  std::map<int,std::map<int,TH1F*> > h_Cvl;
  
  
  
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
  
  for(int i = 0; i < nRegions; ++i)
    for(int k = 0; k < nBins; ++k)
      {
	char histoName[80];
	
	sprintf(histoName, "EoP_%d_%d", i, k);
	h_EoP[i][k] = new TH1F(histoName, histoName, 2000, 0., 5.);
	SetHistoStyle(h_EoP[i][k],"EoP");
	
	sprintf(histoName, "EoC_%d_%d", i, k);
	h_EoC[i][k] = new TH1F(histoName, histoName, 2000, 0., 5.);
	SetHistoStyle(h_EoC[i][k],"EoC");
	
	sprintf(histoName, "Las_%d_%d", i, k);
	h_Las[i][k] = new TH1F(histoName, histoName, 500, 0.5, 1.5);
	
	sprintf(histoName, "Tsp_%d_%d", i, k);
	h_Tsp[i][k] = new TH1F(histoName, histoName, 500, 0.5, 1.5);
      }
  
  
  
  //-----------------------
  //loop over sorted enties
  
  int fillingBin[nRegions];
  for(int iSaved = 0; iSaved < nSavePts; ++iSaved)
    {
      if( iSaved%100000 == 0 ) std::cout << "reading saved entry " << iSaved << "\r" << std::flush;

      if( iSaved == 0 )
	for(int k=0; k<nRegions; ++k)
	  {
	    fillingBin[k] = 0;
	  }

      int reg = sortedEntries[iSaved].region;
      
      if( Entries[reg][fillingBin[reg]] == 0 )
	{
	  MinTime[reg][fillingBin[reg]] = sortedEntries[iSaved].timeStampHigh;
          MinRun[reg][fillingBin[reg]] = sortedEntries[iSaved].runId;
	}
       
      if( (sortedEntries[iSaved].timeStampHigh-MinTime[reg][fillingBin[reg]])/3600. > timeLapse )
	{
	  ++fillingBin[reg];
	  
	  Entries[reg][fillingBin[reg]] = 0;
	  AveTime[reg][fillingBin[reg]] = 0;
	  MinTime[reg][fillingBin[reg]] = sortedEntries[iSaved].timeStampHigh;
	  MaxTime[reg][fillingBin[reg]] = 0;
	  AveRun[reg][fillingBin[reg]] = 0;
	  MinRun[reg][fillingBin[reg]] = sortedEntries[iSaved].runId;
	  MaxRun[reg][fillingBin[reg]] = 0;
	  AveLT[reg][fillingBin[reg]] = 0;
	  AveLT2[reg][fillingBin[reg]] = 0;
      }
      
      if(h_EoP[reg][fillingBin[reg]]->GetEntries() > evtsPerPoint-1)
	{
	  ++fillingBin[reg];
	  
	  Entries[reg][fillingBin[reg]] = 0;
	  AveTime[reg][fillingBin[reg]] = 0;
	  MinTime[reg][fillingBin[reg]] = sortedEntries[iSaved].timeStampHigh;
	  MaxTime[reg][fillingBin[reg]] = 0;
	  AveRun[reg][fillingBin[reg]] = 0;
	  MinRun[reg][fillingBin[reg]] = sortedEntries[iSaved].runId;
	  MaxRun[reg][fillingBin[reg]] = 0;
	  AveLT[reg][fillingBin[reg]] = 0;
	  AveLT2[reg][fillingBin[reg]] = 0;
	}
      
      h_EoP[reg][fillingBin[reg]] -> Fill( sortedEntries[iSaved].scE / sortedEntries[iSaved].scLaserCorr / sortedEntries[iSaved].P );
      h_EoC[reg][fillingBin[reg]] -> Fill( sortedEntries[iSaved].scE/sortedEntries[iSaved].P );
      h_Las[reg][fillingBin[reg]] -> Fill( sortedEntries[iSaved].scLaserCorr );
      h_Tsp[reg][fillingBin[reg]] -> Fill( 1./sortedEntries[iSaved].scLaserCorr );
      
      float LT = (-1. / sortedEntries[iSaved].seedLaserAlpha * log(sortedEntries[iSaved].scLaserCorr));
      
      ++Entries[reg][fillingBin[reg]];
      AveTime[reg][fillingBin[reg]] += sortedEntries[iSaved].timeStampHigh;
      MaxTime[reg][fillingBin[reg]] = sortedEntries[iSaved].timeStampHigh;
      AveRun[reg][fillingBin[reg]] += sortedEntries[iSaved].runId;    
      MaxRun[reg][fillingBin[reg]] = sortedEntries[iSaved].runId;
      AveLT[reg][fillingBin[reg]] += LT;
      AveLT2[reg][fillingBin[reg]] += LT*LT;

    }
  std::cout << std::endl;
  
  
  
  //calculate averages
  for(int ii = 0; ii < nRegions; ++ii)
    for(int kk = 0; kk < nBins; ++kk)
      {
	AveTime[ii][kk] /= Entries[ii][kk];
	AveRun[ii][kk] /= Entries[ii][kk];
	AveLT[ii][kk] /= Entries[ii][kk];
	AveLT2[ii][kk] /= Entries[ii][kk];
      }
  
  
  //---------------------
  // histogram definition
  
  TH1F* h_EoP_spread[nRegions];
  TH1F* h_EoC_spread[nRegions];
  TH1F* h_EoP_spread_run[nRegions];
  TH1F* h_EoC_spread_run[nRegions];

  TH1F* h_EoP_chi2[nRegions];
  TH1F* h_EoC_chi2[nRegions];

  // function definition
  std::map<int,std::map<int,TF1*> > f_EoP;
  std::map<int,std::map<int,TF1*> > f_EoC;
  
  // graphs definition
  TGraphAsymmErrors* g_fit[nRegions];
  TGraphAsymmErrors* g_c_fit[nRegions];
  
  TGraphAsymmErrors* g_fit_run[nRegions];
  TGraphAsymmErrors* g_c_fit_run[nRegions];
  
  TGraph* g_las[nRegions];
  TGraphErrors* g_LT[nRegions];

  int rebin = 2;
  if( strcmp(EBEE,"EE") == 0 ) rebin *= 2;
  
  for(int region=0; region<nRegions; ++region)
    {
      char histoname[50];
      
      sprintf(histoname,"h_EoP_spread_%d",region);
      h_EoP_spread[region] = new TH1F(histoname,"",100,yMIN,yMAX);
      
      sprintf(histoname,"h_EoC_spread_%d",region);
      h_EoC_spread[region] = new TH1F(histoname,"",100,yMIN,yMAX);
      
      sprintf(histoname,"h_EoP_spread_run_%d",region);
      h_EoP_spread_run[region] = new TH1F(histoname,"",100,yMIN,yMAX);
      
      sprintf(histoname,"h_EoC_spread_run_%d",region);
      h_EoC_spread_run[region] = new TH1F(histoname,"",100,yMIN,yMAX);
      
      sprintf(histoname,"h_EoP_chi2_%d",region);
      h_EoP_chi2[region] = new TH1F(histoname,"",50,0.,5.);
      
      sprintf(histoname,"h_EoC_chi2_%d",region);
      h_EoC_chi2[region] = new TH1F(histoname,"",50,0.,5.);
      
      SetHistoStyle(h_EoP_spread[region],"EoP");
      SetHistoStyle(h_EoC_spread[region],"EoC");
      SetHistoStyle(h_EoP_spread_run[region],"EoP");
      SetHistoStyle(h_EoC_spread_run[region],"EoC");
      
      SetHistoStyle(h_EoP_chi2[region],"EoP");
      SetHistoStyle(h_EoC_chi2[region],"EoC");  
      
      //graphs
      g_fit[region] = new TGraphAsymmErrors();
      g_c_fit[region] = new TGraphAsymmErrors();      
      g_fit_run[region] = new TGraphAsymmErrors();
      g_c_fit_run[region] = new TGraphAsymmErrors();
      g_las[region] = new TGraph();
      g_LT[region] = new TGraphErrors();
      
      g_fit[region]->GetXaxis()->SetTimeFormat("%d/%m%F1970-01-01 00:00:00");
      g_fit[region]->GetXaxis()->SetTimeDisplay(1);
      g_c_fit[region]->GetXaxis()->SetTimeFormat("%d/%m%F1970-01-01 00:00:00");
      g_c_fit[region]->GetXaxis()->SetTimeDisplay(1);
      g_las[region]->GetXaxis()->SetTimeFormat("%d/%m%F1970-01-01 00:00:00");
      g_las[region]->GetXaxis()->SetTimeDisplay(1);
      g_LT[region]->GetXaxis()->SetTimeFormat("%d/%m%F1970-01-01 00:00:00");
      g_LT[region]->GetXaxis()->SetTimeDisplay(1);
      
    }


  for(int region=0; region<tRegions; ++region)
    h_template[region] -> Rebin(rebin);

  
  std::vector<int> nValidBins(nRegions);
  
  std::vector<float> EoP_scale(nRegions);
  std::vector<float> EoP_err(nRegions);
  
  std::vector<float> EoC_scale(nRegions);
  std::vector<float> EoC_err(nRegions);
   
  std::vector<float> LCInv_scale(nRegions);
  
  for(int ii = 0; ii < nRegions; ++ii)
    {
      for(int kk = 0; kk < nBins; ++kk)
	{
	  int tRegion = 0;
	  if( strcmp(EBEE,"EB") == 0 && multiTemplate == 1) 
	    tRegion = ebRegion->GetRegionIdIeta(ii, regionType);
	  if( strcmp(EBEE,"EE") == 0 && multiTemplate == 1) 
	    tRegion = ii;

          // sanity check
	  if(h_EoP[ii][kk]->GetEntries() < 100)
	    continue;
	  if(h_EoC[ii][kk]->GetEntries() < 100)
	    continue;
	  
	  h_EoP[ii][kk] -> Rebin(rebin);
	  h_EoC[ii][kk] -> Rebin(rebin);
	  
	  
	  
	  //-----------------------------------
	  // Fit the graph for uncorrected data
	  
	  // define the fitting function
	  // N.B. [0] * ( [1] * f( [1]*(x-[2]) ) )
	  
	  char convolutionName[50];
	  sprintf(convolutionName,"h_convolution_%d_%d",ii, kk);
	  //h_Cvl[ii][kk] = ConvoluteTemplate(std::string(convolutionName),h_template[ii],h_Las[ii][kk],32768,-5.,5.);
	  h_Cvl[ii][kk] = MellinConvolution(std::string(convolutionName),h_template[tRegion],h_Tsp[ii][kk]);
	  
	  histoFunc* templateHistoFunc = new histoFunc(h_template[tRegion]);
	  histoFunc* templateConvolutedHistoFunc = new histoFunc(h_Cvl[ii][kk]);
	  char funcName[50];
	  
	  sprintf(funcName,"f_EoP_%d_%d", ii, kk);
	  
	  if( strcmp(EBEE,"EB") == 0 )
	    f_EoP[ii][kk] = new TF1(funcName, templateConvolutedHistoFunc, 0.8*(h_Tsp[ii][kk]->GetMean()), 1.4*(h_Tsp[ii][kk]->GetMean()), 3, "histoFunc");
	  else
	    f_EoP[ii][kk] = new TF1(funcName, templateConvolutedHistoFunc, 0.75*(h_Tsp[ii][kk]->GetMean()), 1.5*(h_Tsp[ii][kk]->GetMean()), 3, "histoFunc");

	  f_EoP[ii][kk] -> SetParName(0,"Norm"); 
	  f_EoP[ii][kk] -> SetParName(1,"Scale factor"); 
	  f_EoP[ii][kk] -> SetLineWidth(1); 
	  f_EoP[ii][kk] -> SetNpx(10000);
	  
	  double xNorm = h_EoP[ii][kk]->GetEntries()/h_template[tRegion]->GetEntries() *
	    h_EoP[ii][kk]->GetBinWidth(1)/h_template[tRegion]->GetBinWidth(1); 
	  
	  f_EoP[ii][kk] -> FixParameter(0, xNorm);
	  f_EoP[ii][kk] -> SetParameter(1, 1.);
	  f_EoP[ii][kk] -> FixParameter(2, 0.);
	  f_EoP[ii][kk] -> SetLineColor(kRed+2); 
	  
	  int fStatus_EoP = 0;
	  int nTrials_EoP = 0;
	  TFitResultPtr rp_EoP;
	  
	  rp_EoP = h_EoP[ii][kk] -> Fit(funcName, "QERLS+");
	  while( (fStatus_EoP != 0) && (nTrials_EoP < 10) )
	    {
	      rp_EoP = h_EoP[ii][kk] -> Fit(funcName, "QERLS+");
	      fStatus_EoP = rp_EoP;
	      if(fStatus_EoP == 0) break;
	      ++nTrials_EoP;
	    }
	  
	  double eee_EoP = f_EoP[ii][kk]->GetParError(1);
	  //double k_EoP = f_EoP[ii][kk]->GetParameter(1);
	  double k_EoP = f_EoP[ii][kk]->GetParameter(1) / h_Tsp[ii][kk]->GetMean(); //needed when using mellin's convolution 
	  
	  
	  
	  //---------------------------------
	  // Fit the graph for corrected data
	  
	  // define the fitting function
	  // N.B. [0] * ( [1] * f( [1]*(x-[2]) ) )
	  
	  sprintf(funcName,"f_EoC_%d_%d",ii, kk);
	  if( strcmp(EBEE,"EB") == 0 )
	    f_EoC[ii][kk] = new TF1(funcName, templateHistoFunc, 0.8, 1.4, 3, "histoFunc");
	  else
	    f_EoC[ii][kk] = new TF1(funcName, templateHistoFunc, 0.75, 1.5, 3, "histoFunc");
	  f_EoC[ii][kk] -> SetParName(0,"Norm"); 
	  f_EoC[ii][kk] -> SetParName(1,"Scale factor"); 
	  f_EoC[ii][kk] -> SetLineWidth(1); 
	  f_EoC[ii][kk] -> SetNpx(10000);
	  
	  xNorm = h_EoC[ii][kk]->GetEntries()/h_template[tRegion]->GetEntries() *
	    h_EoC[ii][kk]->GetBinWidth(1)/h_template[tRegion]->GetBinWidth(1); 
	  
	  f_EoC[ii][kk] -> FixParameter(0, xNorm);
	  f_EoC[ii][kk] -> SetParameter(1, 0.99);
	  f_EoC[ii][kk] -> FixParameter(2, 0.);
	  f_EoC[ii][kk] -> SetLineColor(kGreen+2); 
	  
	  
	  int fStatus_EoC = 0;
	  int nTrials_EoC = 0;
	  TFitResultPtr rp_EoC;
	  
	  rp_EoC = h_EoC[ii][kk] -> Fit(funcName, "QERLS+");
	  fStatus_EoC = rp_EoC;
	  nTrials_EoC = 0;
	  while( (fStatus_EoC != 0) && (nTrials_EoC < 10) )
	    {
	      rp_EoC = h_EoC[ii][kk] -> Fit(funcName, "QERLS+");
	      fStatus_EoC = rp_EoC;
	      if(fStatus_EoC == 0) break;
	      ++nTrials_EoC;
	    }
	  
	  double k_EoC   = f_EoC[ii][kk]->GetParameter(1);
	  double eee_EoC = f_EoC[ii][kk]->GetParError(1); 
	  
	  
	  
	  //----------------
	  // fill the graphs
	  if( (fStatus_EoP == 0) && (eee_EoP > 0.05*h_template[tRegion]->GetRMS()/sqrt(evtsPerPoint)) &&
	      (fStatus_EoC == 0) && (eee_EoC > 0.05*h_template[tRegion]->GetRMS()/sqrt(evtsPerPoint)) )
	    {
	      float date = (float)AveTime[ii][kk];
	      float dLow = (float)(AveTime[ii][kk]-MinTime[ii][kk]); 
	      float dHig = (float)(MaxTime[ii][kk]-AveTime[ii][kk]);
	      float run = (float)AveRun[ii][kk];
	      float rLow = (float)(AveRun[ii][kk]-MinRun[ii][kk]); 
	      float rHig = (float)(MaxRun[ii][kk]-AveRun[ii][kk]);
	      
	      g_las[ii] -> SetPoint(nValidBins[ii], (float)AveTime[ii][kk], h_Tsp[ii][kk]->GetMean() );
	      g_LT[ii]  -> SetPoint(nValidBins[ii], (float)AveTime[ii][kk], AveLT[ii][kk] );
	      g_LT[ii]  -> SetPointError(nValidBins[ii], 0., sqrt(AveLT2[ii][kk]-AveLT[ii][kk]*AveLT[ii][kk]) / sqrt(Entries[ii][kk]) );
	      
	      g_fit[ii] -> SetPoint(nValidBins[ii],  date , 1./k_EoP);
	      g_fit[ii] -> SetPointError(nValidBins[ii], dLow , dHig, eee_EoP/k_EoP/k_EoP, eee_EoP/k_EoP/k_EoP);
	      g_c_fit[ii] -> SetPoint(nValidBins[ii],  date , 1./k_EoC);
	      g_c_fit[ii] -> SetPointError(nValidBins[ii], dLow , dHig , eee_EoC/k_EoC/k_EoC, eee_EoC/k_EoC/k_EoC);
	      
	      g_fit_run[ii] -> SetPoint(nValidBins[ii],  run , 1./k_EoP);
	      g_fit_run[ii] -> SetPointError(nValidBins[ii], rLow , rHig, eee_EoP/k_EoP/k_EoP, eee_EoP/k_EoP/k_EoP);
	      g_c_fit_run[ii] -> SetPoint(nValidBins[ii],  run , 1./k_EoC);
	      g_c_fit_run[ii] -> SetPointError(nValidBins[ii], rLow , rHig, eee_EoC/k_EoC/k_EoC, eee_EoC/k_EoC/k_EoC);
	      
	      h_EoP_chi2[ii] -> Fill(f_EoP[ii][kk]->GetChisquare()/f_EoP[ii][kk]->GetNDF());
	      h_EoC_chi2[ii] -> Fill(f_EoC[ii][kk]->GetChisquare()/f_EoP[ii][kk]->GetNDF());
	      
	      LCInv_scale[ii] += h_Tsp[ii][kk]->GetMean();
	      EoP_scale[ii] += 1./k_EoP;
	      EoP_err[ii] += eee_EoP/k_EoP/k_EoP;
	      EoC_scale[ii] += 1./k_EoC;
	      EoC_err[ii] += eee_EoC/k_EoC/k_EoC;
	      
	      ++nValidBins[ii];
	    }
	  else
	    {
	      std::cout << "Fitting uncorrected time bin: " << kk << " in region: " << ii << "   Fail status: " << fStatus_EoP << "   sigma: " << eee_EoP << std::endl;
	      std::cout << "Fitting   corrected time bin: " << kk << " in region: " << ii << "   Fail status: " << fStatus_EoC << "   sigma: " << eee_EoC << std::endl;
	    }
	}
  
      
      LCInv_scale[ii] /= nValidBins[ii];
      EoP_scale[ii] /= nValidBins[ii];
      EoP_err[ii]   /= nValidBins[ii];
      EoC_scale[ii] /= nValidBins[ii];
      EoC_err[ii]   /= nValidBins[ii];
    }
  
  
  
  
  
  
  //---------------
  // Rescale graphs
  
  float yscale = 1.;
  //float yscale = 1./EoC_scale;
  
  for(int ii = 0; ii < nRegions; ++ii)
    {
      for(int kk = 0; kk < nValidBins[ii]; ++kk)
	{
	  double x,y; 
	  g_fit[ii] -> GetPoint(kk,x,y); 
	  g_fit[ii] -> SetPoint(kk,x,y*yscale);
	  h_EoP_spread[ii] -> Fill(y*yscale);
	  
	  g_c_fit[ii] -> GetPoint(kk,x,y); 
	  g_c_fit[ii] -> SetPoint(kk,x,y*yscale);
	  h_EoC_spread[ii] -> Fill(y*yscale);
	  
	  g_fit_run[ii] -> GetPoint(kk,x,y); 
	  g_fit_run[ii] -> SetPoint(kk,x,y*yscale); 
	  h_EoP_spread_run[ii] -> Fill(y*yscale);
	  
	  g_c_fit_run[ii] -> GetPoint(kk,x,y); 
	  g_c_fit_run[ii] -> SetPoint(kk,x,y*yscale);
	  h_EoC_spread_run[ii] -> Fill(y*yscale);
	  
	  g_las[ii] -> GetPoint(kk,x,y);
	  //g_las[ii] -> SetPoint(kk,x,y*yscale*EoP_scale[ii]/LCInv_scale[ii]);
	  g_las[ii] -> SetPoint(kk,x,y*yscale);
	}
      
      // TF1 EoC_pol0("EoC_pol0","pol0",t1,t2);
      // EoC_pol0.SetLineColor(kGreen+2);
      // EoC_pol0.SetLineWidth(2);
      // EoC_pol0.SetLineStyle(2);
      // g_c_fit[ii] -> Fit("EoC_pol0","QNR");
    }
  
  
  
  
  
  
  //----------------------------
  // Print out global quantities
  
  std::cout << std::endl;
  std::cout << "***** Mean scales and errors *****" << std::endl; 
  std::cout << std::fixed;
  std::cout << std::setprecision(4);
  for(int ii = 0; ii < nRegions; ++ii)
    {
      std::cout << "Region: " << ii << std::endl;
      std::cout << "Mean EoP scale: "  << std::setw(6) << EoP_scale[ii]   << "   mean EoP error: " << std::setw(8) << EoP_err[ii] << std::endl;
      std::cout << "Mean EoC scale: "  << std::setw(6) << EoC_scale[ii]   << "   mean EoC error: " << std::setw(8) << EoC_err[ii] << std::endl;
      std::cout << "Mean 1/LC scale: " << std::setw(6) << LCInv_scale[ii] << std::endl;
    }
  
  
  
  
  
  
  //-----------------
  // Occupancy plots
  //-----------------
  
  // TCanvas* c_scOccupancy = new TCanvas("c_scOccupancy","SC occupancy",0,0,1000,500);
  // c_scOccupancy -> Divide(2,1);
  
  // c_scOccupancy -> cd(1);
  // h_scOccupancy_eta -> GetXaxis() -> SetTitle("sc #eta");
  // h_scOccupancy_eta -> GetYaxis() -> SetTitle("events");
  // h_scOccupancy_eta -> Draw();
  
  // c_scOccupancy -> cd(2);
  // h_scOccupancy_phi -> GetXaxis() -> SetTitle("sc #phi");
  // h_scOccupancy_phi -> GetYaxis() -> SetTitle("events");
  // h_scOccupancy_phi -> Draw();
  
  // TCanvas* c_seedOccupancy = new TCanvas("c_seedOccupancy","seed occupancy",0,0,1500,500);
  // c_seedOccupancy -> Divide(3,1);
  
  // c_seedOccupancy -> cd(1);
  // h_seedOccupancy_EB -> GetXaxis() -> SetTitle("seed i#eta");
  // h_seedOccupancy_EB -> GetYaxis() -> SetTitle("seed i#phi");
  // h_seedOccupancy_EB -> Draw("COLZ");
  
  // c_seedOccupancy -> cd(2);
  // h_seedOccupancy_EEp -> GetXaxis() -> SetTitle("seed ix");
  // h_seedOccupancy_EEp -> GetYaxis() -> SetTitle("seed iy");
  // h_seedOccupancy_EEp -> Draw("COLZ");
  
  // c_seedOccupancy -> cd(3);
  // h_seedOccupancy_EEm -> GetXaxis() -> SetTitle("seed ix");
  // h_seedOccupancy_EEm -> GetYaxis() -> SetTitle("seed iy");
  // h_seedOccupancy_EEm -> Draw("COLZ");
  
  
  
  
  
  
  for(int ii=0; ii<nRegions; ++ii)
    {
      //-----------
      // Chi2 plots
      //-----------
      
      char canvaName[80];
      sprintf(canvaName,"c_chi2_%d",ii);

      TCanvas* c_chi2 = new TCanvas(canvaName,"fit chi2",0,0,500,500);
      c_chi2 -> cd();
      
      h_EoC_chi2[ii] -> GetXaxis() -> SetTitle("#chi^{2}/N_{dof}");
      h_EoC_chi2[ii] -> Draw("");
      gPad -> Update();
      
      TPaveStats* s_EoC = new TPaveStats;
      s_EoC = (TPaveStats*)(h_EoC_chi2[ii]->GetListOfFunctions()->FindObject("stats"));
      s_EoC -> SetStatFormat("1.4g");
      s_EoC -> SetTextColor(kGreen+2);
      s_EoC->SetY1NDC(0.59);
      s_EoC->SetY2NDC(0.79);
      s_EoC -> Draw("sames");
      gPad -> Update();
      
      h_EoP_chi2[ii] -> GetXaxis() -> SetTitle("#chi^{2}/N_{dof}");
      h_EoP_chi2[ii] -> Draw("sames");
      gPad -> Update();
      
      TPaveStats* s_EoP = new TPaveStats;
      s_EoP = (TPaveStats*)(h_EoP_chi2[ii]->GetListOfFunctions()->FindObject("stats"));
      s_EoP -> SetStatFormat("1.4g");
      s_EoP -> SetTextColor(kRed+2);
      s_EoP->SetY1NDC(0.79);
      s_EoP->SetY2NDC(0.99);
      s_EoP -> Draw("sames");
      gPad -> Update();
      
      
      
      //-------------------
      // Final plot vs date
      //-------------------
      sprintf(canvaName,"c_cplot_%d",ii);
      
      TCanvas* cplot = new TCanvas(canvaName, "history plot vs date",100,100,1000,500);
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
      hPad->GetXaxis() -> SetRangeUser(sortedEntries[0].timeStampHigh-43200,sortedEntries.back().timeStampHigh+43200);
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
      g_fit[ii] -> SetMarkerStyle(24);
      g_fit[ii] -> SetMarkerSize(0.7);
      g_fit[ii] -> SetMarkerColor(kRed+2);
      g_fit[ii] -> SetLineColor(kRed+2);
      g_fit[ii] -> Draw("P");
      g_c_fit[ii] -> SetMarkerStyle(20);
      g_c_fit[ii] -> SetMarkerColor(kGreen+2);
      g_c_fit[ii] -> SetLineColor(kGreen+2);
      g_c_fit[ii] -> SetMarkerSize(0.7);
      g_c_fit[ii] -> Draw("P,same");
      g_las[ii] -> SetLineColor(kAzure-2);
      g_las[ii] -> SetLineWidth(2);
      g_las[ii] -> Draw("L,same");
      
      TLegend* legend = new TLegend(0.60,0.78,0.90,0.94);
      legend -> SetLineColor(kWhite);
      legend -> SetLineWidth(0);
      legend -> SetFillColor(kWhite);
      legend -> SetFillStyle(0);
      legend -> SetTextFont(42);
      legend -> SetTextSize(0.04);
      legend -> AddEntry(g_c_fit[ii],"with LM correction","PL");
      legend -> AddEntry(g_fit[ii],  "without LM correction","PL");
      legend -> AddEntry(g_las[ii],  "1 / LM correction","L");
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
      
      
      h_EoC_spread[ii] -> SetFillStyle(3001);
      h_EoC_spread[ii] -> SetFillColor(kGreen+2);
      h_EoC_spread[ii]->GetYaxis()->SetLabelSize(0.09);
      h_EoC_spread[ii]->GetYaxis()->SetLabelOffset(-0.03);
      h_EoC_spread[ii]->GetYaxis()->SetTitleSize(0.08);
      h_EoC_spread[ii]->GetYaxis()->SetNdivisions(505);
      h_EoC_spread[ii]->GetXaxis()->SetLabelOffset(1000);
  		  
      h_EoC_spread[ii] -> Draw("hbar");
      gPad -> Update();
  
      s_EoC_spread = (TPaveStats*)(h_EoC_spread[ii]->GetListOfFunctions()->FindObject("stats"));
      s_EoC_spread -> SetStatFormat("1.4g");
      s_EoC_spread->SetX1NDC(0.06); //new x start position
      s_EoC_spread->SetX2NDC(0.71); //new x end position
      s_EoC_spread->SetY1NDC(0.93); //new x start position
      s_EoC_spread->SetY2NDC(0.84); //new x end position
      s_EoC_spread -> SetOptStat(1100);
      s_EoC_spread ->SetTextColor(kGreen+2);
      s_EoC_spread ->SetTextSize(0.08);
      s_EoC_spread -> Draw("sames");
  
  
      h_EoP_spread[ii] -> SetFillStyle(3001);
      h_EoP_spread[ii] -> SetFillColor(kRed+2);
      h_EoP_spread[ii] -> Draw("hbarsames");
      gPad -> Update();
      s_EoP_spread = (TPaveStats*)(h_EoP_spread[ii]->GetListOfFunctions()->FindObject("stats"));
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
      
      sprintf(canvaName,"cplot_run_%d",ii);
      TCanvas* cplot_run = new TCanvas(canvaName, "history plot vs run",100,100,1000,500);
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
      
      hPad = (TH1F*)gPad->DrawFrame(sortedEntries[0].runId-1000,0.9,sortedEntries.back().runId+1000,1.05);
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
      g_fit_run[ii] -> SetMarkerStyle(20);
      g_fit_run[ii] -> SetMarkerSize(0.7);
      g_fit_run[ii] -> SetMarkerColor(kRed+2);
      g_fit_run[ii] -> SetLineColor(kRed+2);
      g_fit_run[ii] -> Draw("P");
      g_c_fit_run[ii] -> SetMarkerStyle(20);
      g_c_fit_run[ii] -> SetMarkerColor(kGreen+2);
      g_c_fit_run[ii] -> SetLineColor(kGreen+2);
      g_c_fit_run[ii] -> SetMarkerSize(0.7);
      g_c_fit_run[ii] -> Draw("P,same");
      
      
      cRight -> cd();
      
      s_EoP_spread = new TPaveStats();
      s_EoC_spread = new TPaveStats();
      
      
      h_EoC_spread_run[ii] -> SetFillStyle(3001);
      h_EoC_spread_run[ii] -> SetFillColor(kGreen+2);
      h_EoC_spread_run[ii] ->GetYaxis()->SetLabelSize(labSize2);
      h_EoC_spread_run[ii] ->GetYaxis()->SetTitleSize(labSize2);
      h_EoC_spread_run[ii] ->GetYaxis()->SetNdivisions(505);
      h_EoC_spread_run[ii] ->GetYaxis()->SetLabelOffset(-0.02);
      h_EoC_spread_run[ii] ->GetXaxis()->SetLabelOffset(1000);
      
      h_EoC_spread_run[ii] -> Draw("hbar");
      gPad -> Update();
      
      s_EoC_spread = (TPaveStats*)(h_EoC_spread_run[ii]->GetListOfFunctions()->FindObject("stats"));
      s_EoC_spread ->SetTextColor(kGreen+2);
      s_EoC_spread ->SetTextSize(0.06);
      s_EoC_spread->SetX1NDC(0.49); //new x start position
      s_EoC_spread->SetX2NDC(0.99); //new x end position
      s_EoC_spread->SetY1NDC(0.875); //new x start position
      s_EoC_spread->SetY2NDC(0.990); //new x end position
      s_EoC_spread -> SetOptStat(1100);
      s_EoC_spread -> Draw("sames");
      
      h_EoP_spread_run[ii] -> SetFillStyle(3001);
      h_EoP_spread_run[ii] -> SetFillColor(kRed+2);
      h_EoP_spread_run[ii] -> Draw("hbarsames");
      gPad -> Update();
      
      s_EoP_spread = (TPaveStats*)(h_EoP_spread_run[ii]->GetListOfFunctions()->FindObject("stats"));
      s_EoP_spread->SetX1NDC(0.49); //new x start position
      s_EoP_spread->SetX2NDC(0.99); //new x end position
      s_EoP_spread->SetY1NDC(0.750); //new x start position
      s_EoP_spread->SetY2NDC(0.875); //new x end position
      s_EoP_spread ->SetOptStat(1100);
      s_EoP_spread ->SetTextColor(kRed+2);
      s_EoP_spread ->SetTextSize(0.06);
      s_EoP_spread -> Draw("sames");
      
      
      
      char counter[50];
      sprintf(counter,"%d",ii);
      std::string countString(counter);
      
      c_chi2 -> Print((folderName+"/"+folderName+"_"+regionType+"_fitChi2_"+countString+".png").c_str(),"png");
      //c_scOccupancy -> Print((folderName+"/"+folderName+"_"+regionType+"_scOccupancy.png").c_str(),"png");
      //c_seedOccupancy -> Print((folderName+"/"+folderName+"_"+regionType+"_seedOccupancy.png").c_str(),"png");
      cplot -> Print((folderName+"/"+folderName+"_"+regionType+"_history_vsTime_"+countString+".png").c_str(),"png");
      cplot_run -> Print((folderName+"/"+folderName+"_"+regionType+"_history_vsRun_"+countString+".png").c_str(),"png");
      
      c_chi2 -> Print((folderName+"/"+folderName+"_"+regionType+"_fitChi2_"+countString+".pdf").c_str(),"pdf");
      // c_scOccupancy -> Print((folderName+"/"+folderName+"_"+regionType+"_scOccupancy.pdf").c_str(),"pdf");
      // c_seedOccupancy -> Print((folderName+"/"+folderName+"_"+regionType+"_seedOccupancy.pdf").c_str(),"pdf");
      cplot -> Print((folderName+"/"+folderName+"_"+regionType+"_history_vsTime_"+countString+".pdf").c_str(),"pdf");
      cplot_run -> Print((folderName+"/"+folderName+"_"+regionType+"_history_vsRun_"+countString+".pdf").c_str(),"pdf");
      
      cplot -> SaveAs((folderName+"/"+folderName+"_"+regionType+"_history_vsTime_"+countString+".C").c_str());
      cplot_run -> SaveAs((folderName+"/"+folderName+"_"+regionType+"_history_vsRun_"+countString+".C").c_str());
      
      //organize in subdir
      char regionName[50];
      sprintf(regionName,"region_%d",ii);
      o -> mkdir(regionName);
      o -> cd(regionName);

      char name[50];
      sprintf(name,"g_fit_%d",ii);
      g_fit[ii] -> Write(name);
      sprintf(name,"g_c_fit_%d",ii);
      g_c_fit[ii] -> Write(name);
      sprintf(name,"g_fit_run_%d",ii);
      g_fit_run[ii] -> Write(name);
      sprintf(name,"g_c_fit_run_%d",ii);
      g_c_fit_run[ii] -> Write(name);
      sprintf(name,"g_las_%d",ii);
      g_las[ii] -> Write(name);
      sprintf(name,"g_LT_%d",ii);
      g_LT[ii] -> Write(name);
      
      h_EoP_chi2[ii] -> Write();
      h_EoC_chi2[ii] -> Write();
      
      for(int kk = 0; kk < nBins; ++kk)
      	{
	  if(h_EoP[ii][kk]->GetEntries() < 100 && h_EoC[ii][kk]->GetEntries() < 100) continue;
      	  h_EoP[ii][kk] -> GetXaxis() -> SetTitle("E/p");
      	  h_EoP[ii][kk] -> Write();
	  
      	  h_EoC[ii][kk] -> GetXaxis() -> SetTitle("E/p");
      	  h_EoC[ii][kk] -> Write();
	}
    }

  o -> cd();
  o -> mkdir("templates");
  o -> cd("templates");
  for(int region=0; region<tRegions; ++region)
    {      
      int tRegion = 0;
      if(multiTemplate == 1) tRegion = region;
      h_template[tRegion] -> Write();
    }
  o -> cd();

  TCanvas* cTemplateRegions = new TCanvas("cTemplateRegions", "template regions",0,0,1000,500);
  cTemplateRegions -> cd();
  templateRegions->Draw("colz");
  cTemplateRegions -> Print((folderName+"/"+folderName+"_"+regionType+"_templateRegions.png").c_str(),"png");
  cTemplateRegions -> Print((folderName+"/"+folderName+"_"+regionType+"_templateRegions.pdf").c_str(),"pdf");
  TCanvas* cRegions = new TCanvas("cRegions", "regions",0,0,1000,500);
  cRegions -> cd();
  regions->Draw("colz");
  cRegions -> Print((folderName+"/"+folderName+"_"+regionType+"_regions.png").c_str(),"png");
  cRegions -> Print((folderName+"/"+folderName+"_"+regionType+"_regions.pdf").c_str(),"pdf");
  
  templateRegions->Write();
  regions->Write();
  
  o -> Close();
}
