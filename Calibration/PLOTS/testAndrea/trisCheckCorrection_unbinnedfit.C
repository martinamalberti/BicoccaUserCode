/* ******************************************************
// Plot EB or EE stability vs LaserCorrection
// Input1 filename (data)
// Input2 MC or data used ti build the template reference
// Input3 EB or EE qualifier (EB default) 
// Input4 Number of events per poiny
//
// Compilation:
//     .L trisCheckCorrection.C+
// Execution:
//     trisCheckCorrection(input1,input2,input3,input4)
// Example: 
//     trisCheckCorrection("inputFiles/WZAnalysis_ric-rerecoDec22WithV3laser_136035-149294.root","inputFiles/WZAnalysis_MC.root","EB",3000)
//
// *************************************************** */

#include "histoFunc.h"
#include "ntupleUtils.h"
#include "ntupleUtils.cc"
#include "EnergyCorrectionFunctions.h"
#include "EnergyCorrectionFunctions.cc"

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
#include "TTree.h"
#include "TVirtualFitter.h"
#include "TFitterMinuit.h"
#include "TStyle.h"

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

float lcMin = 0.99;
float lcMax = 1.10;





void trisCheckCorrection_unbinnedfit(Char_t* EBEE = 0,
                         Int_t evtsPerPoint = 0,
                         float laserCorrMin = -1.,
                         float laserCorrMax = -1.)
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
  
  if ( strcmp(EBEE,"EB") == 0 )
  {
    lcMin = 0.99;
    lcMax = 1.05;
  }
  else
  {
    lcMin = 0.99;
    lcMax = 1.11;
  }
  
  if( (laserCorrMin != -1.) && (laserCorrMax != -1.) )
  {
    lcMin = laserCorrMin;
    lcMax = laserCorrMax;
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
  int isW, isZ;
  int timeStampHigh;
  float seedLaserAlpha;
  float avgLaserCorrection, scCrackCorrection;
  float EoP;
  float scE, scERaw, scEta, scEtaWidth, scPhiWidth;
  int seedIeta,seedIphi;
  int seedIx,seedIy,seedZside;
  float esE;
  float seedLaserCorrection;
  int iPhi,iEta;
  
  ntu_DA->SetBranchAddress("runId", &runId);  
  ntu_DA->SetBranchAddress("isW", &isW);
  //ntu_DA->SetBranchAddress("isZ", &isZ);
  ntu_DA->SetBranchAddress("timeStampHigh", &timeStampHigh);
  ntu_DA->SetBranchAddress("ele1_scCrackCorr", &scCrackCorrection);
  ntu_DA->SetBranchAddress("ele1_scLaserCorr", &avgLaserCorrection);
  ntu_DA->SetBranchAddress("ele1_seedLaserCorr", &seedLaserCorrection);
  ntu_DA->SetBranchAddress("ele1_seedLaserAlpha", &seedLaserAlpha);
  ntu_DA->SetBranchAddress("ele1_es", &esE);
  ntu_DA->SetBranchAddress("ele1_scE", &scE);
  ntu_DA->SetBranchAddress("ele1_scERaw", &scERaw);
  ntu_DA->SetBranchAddress("ele1_scEta", &scEta);
  ntu_DA->SetBranchAddress("ele1_scEtaWidth", &scEtaWidth);
  ntu_DA->SetBranchAddress("ele1_scPhiWidth", &scPhiWidth);
  ntu_DA->SetBranchAddress("ele1_EOverP", &EoP);
  ntu_DA->SetBranchAddress("ele1_seedIphi", &iPhi);
  ntu_DA->SetBranchAddress("ele1_seedIeta", &iEta);
  ntu_DA->SetBranchAddress("ele1_seedIeta",       &seedIeta);
  ntu_DA->SetBranchAddress("ele1_seedIphi",       &seedIphi);
  ntu_DA->SetBranchAddress("ele1_seedIx",         &seedIx);
  ntu_DA->SetBranchAddress("ele1_seedIy",         &seedIy);
  ntu_DA->SetBranchAddress("ele1_seedZside",      &seedZside);
  
  ntu_MC->SetBranchAddress("isW", &isW);
  ntu_MC->SetBranchAddress("isZ", &isZ);
  ntu_MC->SetBranchAddress("ele1_scEta", &scEta);
  ntu_MC->SetBranchAddress("ele1_EOverP", &EoP);
  
  

  float params[42];
  InitializeParams(params);
  
  
  // Build the reference from 'infile2'
  std::cout << std::endl;
  std::cout << "***** Build reference for " << EBEE << " *****" << std::endl;
  
  templateHisto = new TH1F("templateHisto", "", 1200, 0., 5.);

  for(int ientry = 0; ientry < ntu_MC->GetEntries(); ++ientry)
  {
    if( (ientry%100000 == 0) ) std::cout << "reading MC entry " << ientry << std::endl;
    ntu_MC->GetEntry(ientry);
    
    if (strcmp(EBEE,"EB")==0 && fabs(scEta) > 1.4442) continue;                       // barrel
    if (strcmp(EBEE,"EE")==0 && (fabs(scEta) < 1.56 || fabs(scEta) > 2.5 )) continue; // endcap
    
    //if( seedLaserAlpha > 1.5 ) continue;
        
    //if( fabs(scEta) > 0.44 ) continue;
    //if( fabs(scEta) < 0.44 || fabs(scEta) > 0.77 ) continue;
    //if( fabs(scEta) < 0.77 || fabs(scEta) > 1.10 ) continue;
    //if( fabs(scEta) < 1.10 || fabs(scEta) > 1.56 ) continue;
    //if( fabs(scEta) < 1.56 || fabs(scEta) > 2.00 ) continue;
    //if( fabs(scEta) < 2.00 ) continue;
        
    templateHisto -> Fill(EoP);
  }

  int rebin = 4;
  if (strcmp(EBEE,"EB")==0) rebin = 2;
  templateHisto -> Rebin(rebin);  
  FitTemplate();
  
  std::cout << "Reference built for " << EBEE << " - " << templateHisto->GetEntries() << " events" << std::endl;
  
  
  
  // Loop and sort events
  std::cout << std::endl;
  std::cout << "***** Sort events and define bins *****" << std::endl;
  
  int nEntries = ntu_DA -> GetEntriesFast(); 
  int nSavePts = 0; 
  std::vector<bool> isSavedEntries(nEntries);
  std::vector<SorterLC> sortedEntries;
  
  for(int ientry = 0; ientry < nEntries; ++ientry)
  {
    ntu_DA -> GetEntry(ientry); 
    isSavedEntries.at(ientry) = false;
    
    // save only what is needed for the analysis!!!
    if (strcmp(EBEE,"EB")==0 && fabs(scEta) > 1.4442) continue;                       // barrel
    if (strcmp(EBEE,"EE")==0 && (fabs(scEta) < 1.56 || fabs(scEta) > 2.5 )) continue; // endcap
    
    //if( fabs(scEta) > 0.44 ) continue;
    //if( fabs(scEta) < 0.44 || fabs(scEta) > 0.77 ) continue;
    //if( fabs(scEta) < 0.77 || fabs(scEta) > 1.10 ) continue;
    //if( fabs(scEta) < 1.10 || fabs(scEta) > 1.56 ) continue;
    //if( fabs(scEta) < 1.56 || fabs(scEta) > 2.00 ) continue;
    //if( fabs(scEta) < 2.00 ) continue;
    
        
    if( seedLaserCorrection <= 1.) continue;   
    if( seedLaserAlpha < 1.5 ) continue;
    //if( timeStampHigh > 1303862400 ) continue;
    
    if( seedZside < 0 )
      if( (seedIx > 20 ) && (seedIx < 50) && (seedIy > 85) && (seedIy < 92) ) continue;
    if( seedZside == -1 )
      if( (seedIx > 35 ) && (seedIx < 55) && (seedIy > 80) && (seedIy < 87) ) continue;
    if( seedZside > 0 )
      if( (seedIx > 65 ) && (seedIx < 77) && (seedIy > 33) && (seedIy < 57) ) continue;
    if( seedZside > 0 )
      if( (seedIx > 75 ) && (seedIx < 93) && (seedIy > 18) && (seedIy < 37) ) continue;    
    
    //if ( runId < 163045 ) continue;
    //if ( runId >= 163232 ) continue;
    //if ( runId < 163232 ) continue;
    
    
    //*********************** CLUSTER CORR *****************************
    //if( (ientry%1 == 0) ) std::cout << "\n\n\nreading entry " << ientry << std::endl;    
    //Ediff -> Fill( ( (scCrackCorrection*fClusterCorrections(scERaw+esE,scEta,scPhiWidth/scEtaWidth,params))-scE)/scE );
    //Ediff_vsEta -> Fill( scEta, ( (esE+scCrackCorrection*fClusterCorrections(scERaw,scEta,scPhiWidth/scEtaWidth,params))-scE)/scE );
    //if( fabs(fClusterCorrections(scERaw,scEta,scPhiWidth/scEtaWidth,params)-scE) > 0.001 )
    //{
    //  std::cout << "\n\n" << std::endl;
    //  std::cout << "scEta = " << scEta << "   scE = " << scE << "   scERaw = " << scERaw << std::endl;
    //  std::cout << "scERaw_corr = " << fClusterCorrections(scERaw,scEta,scPhiWidth/scEtaWidth,params) << std::endl;
    //}
    //*********************** CLUSTER CORR *****************************
        
    isSavedEntries.at(ientry) = true;
    
    SorterLC dummy;
    dummy.laserCorr = avgLaserCorrection;
    dummy.entry = ientry;
    sortedEntries.push_back(dummy);
    
    nSavePts++; 
  }
  std::sort(sortedEntries.begin(),sortedEntries.end(),SorterLC());
  std::cout << "Data sorted in " << EBEE << " - " << nSavePts << " events" << std::endl;
  
  //TCanvas* c_diff = new TCanvas("c_diff","c_diff");
  //c_diff -> cd();
  //Ediff -> Draw();
  //
  //TCanvas* c_diff_vsEta = new TCanvas("c_diff_vsEta","c_diff");
  //c_diff_vsEta -> cd();
  //Ediff_vsEta -> Draw("colz");
  
  // bins with evtsPerPoint events per bin
  int nBins = std::max(1,int(nSavePts/evtsPerPoint));
  int nBinPts = int( nSavePts/nBins );
  int nBinTempPts = 0;
  
  std::vector<int> binEntryMax;
  binEntryMax.push_back(0);
  for(int iSaved = 0; iSaved < nSavePts; ++iSaved)
  {
    ++nBinTempPts;
    
    if( nBinTempPts == nBinPts )
    {
      binEntryMax.push_back( iSaved );      
      nBinTempPts = 0;
    }
  }
  binEntryMax.at(nBins) = nSavePts;
  
  std::cout << "nBins = " << nBins << std::endl;
  for(int bin = 0; bin < nBins; ++bin)
    std::cout << "bin: " << bin
              << "   entry min: " << setw(6) << binEntryMax.at(bin)
              << "   entry max: " << setw(6) << binEntryMax.at(bin+1)
              << "   events: "    << setw(6) << binEntryMax.at(bin+1)-binEntryMax.at(bin)
              << std::endl;
  TVirtualFitter::SetDefaultFitter("Fumili2");
  
  
  
  // histogram definition
  
  TH1F* h_EoP_spread;
  TH1F* h_EoC_spread;
  TH2F* h_LC_map = new TH2F("h_LC_map","",360,0.,360,170,-85,85);
  
  if ( strcmp(EBEE,"EB")==0 )
  {  
    h_EoP_spread = new TH1F("h_EoP_spread","",100,0.95,1.01);
    h_EoC_spread = new TH1F("h_EoC_spread","",100,0.95,1.01);    
  }
  else 
  {  
    h_EoP_spread = new TH1F("h_EoP_spread","",100,0.91,1.03);
    h_EoC_spread = new TH1F("h_EoC_spread","",100,0.91,1.03);    
  }

  h_EoP_spread -> SetLineColor(kRed+2);
  h_EoP_spread -> SetLineWidth(2);
  h_EoP_spread -> GetXaxis() -> SetTitle("Relative E/p scale");
  
  h_EoC_spread -> SetLineColor(kGreen+2);
  h_EoC_spread -> SetLineWidth(2);
  h_EoC_spread -> GetXaxis() -> SetTitle("Relative E/p scale");  
  
  
  TH1F** h_EoP = new TH1F*[nBins];
  TH1F** h_EoC = new TH1F*[nBins];
  TH1F** h_Las = new TH1F*[nBins];

  for(int i = 0; i < nBins; ++i)
  {
    char histoName[80];
    
    sprintf(histoName, "EoP_%d", i);
    h_EoP[i] = new TH1F(histoName, histoName, 1200, 0., 3.);
    h_EoP[i] -> SetFillColor(kRed+2);
    h_EoP[i] -> SetMarkerStyle(20);
    h_EoP[i] -> SetMarkerSize(0.7);
    h_EoP[i] -> SetMarkerColor(kRed+2); 
    h_EoP[i] -> SetLineColor(kRed+2); 
    
    sprintf(histoName, "EoC_%d", i);
    h_EoC[i] = new TH1F(histoName, histoName, 1200, 0., 3.);
    h_EoC[i] -> SetFillColor(kGreen+2);
    h_EoC[i] -> SetMarkerStyle(20);
    h_EoC[i] -> SetMarkerSize(0.7);
    h_EoC[i] -> SetMarkerColor(kGreen+2);
    h_EoC[i] -> SetLineColor(kGreen+2);
    
    sprintf(histoName, "Las_%d", i);
    h_Las[i] = new TH1F(histoName, histoName, 100, 0.5, 1.5);    
  }
  
  
  // data definition
  std::vector< std::vector<double>* > dataEoP;
  std::vector< std::vector<double>* > dataEoC;
  
  for (int jbin = 0; jbin< nBins; jbin++){
    dataEoP.push_back(new std::vector<double>);
    dataEoC.push_back(new std::vector<double>);
  }


  // function definition
  TF1** f_EoP = new TF1*[nBins];
  TF1** f_EoC = new TF1*[nBins];
  
  
    
  // loop on the saved and sorted events
  std::cout << std::endl;
  std::cout << "***** Fill and fit histograms *****" << std::endl;

  
  for(int ientry = 0; ientry < nEntries; ++ientry)
  {
    if( (ientry%10000 == 0) ) std::cout << "reading entry " << ientry << std::endl;
    
    if( isSavedEntries.at(ientry) == false ) continue;
    
    int iSaved = -1;
    for(iSaved = 0; iSaved < nSavePts; ++iSaved)
      if( ientry == sortedEntries[iSaved].entry ) break;
    
    int bin = -1;
    for(bin = 0; bin < nBins; ++bin)
      if( iSaved >= binEntryMax.at(bin) && iSaved < binEntryMax.at(bin+1) )
        break;
    
    //std::cout << "ientry = " << ientry << "   iSaved: " << iSaved << "   bin: " << bin << std::endl;
    
    ntu_DA->GetEntry(ientry);
    
    float scale = 1.;
    //scale = sqrt( pow(avgLaserCorrection,((1.52-0.7843)/1.52)-1.) );
    //scale = 1. / (0.1811 + 0.7843*avgLaserCorrection);
    
        
    //// fill the bins 
    (h_Las[bin]) -> Fill(avgLaserCorrection);
    (h_EoP[bin]) -> Fill(EoP/avgLaserCorrection);
    (h_EoC[bin]) -> Fill(EoP * scale);
    h_LC_map->Fill(iPhi,iEta,seedLaserCorrection);

    // fill te vectors data E/p 
    dataEoP[bin]->push_back(EoP/avgLaserCorrection);
    dataEoC[bin]->push_back(EoP);

  }
  

  // Define graph and histograms
  TGraphAsymmErrors* g_fit   = new TGraphAsymmErrors();
  TGraphAsymmErrors* g_c_fit   = new TGraphAsymmErrors();
  
  
  
  // define the fitting function
  // N.B. [0] * ( [1] * f( [1]*(x-[2]) ) )
  histoFunc* templateHistoFunc = new histoFunc(templateHisto);
   
  //templateFunc = new TF1("templateFunc", templateHistoFunc, 0., 5., 3, "histoFunc");
  //templateFunc -> SetParName(0,"Norm"); 
  //templateFunc -> SetParName(1,"Scale factor"); 
  //templateFunc -> SetLineWidth(1); 
  //templateFunc -> SetNpx(10000);
  //templateFunc -> SetParameter(0, 1 );
  //templateFunc -> SetParameter(1, 1);
  //templateFunc -> FixParameter(2, 0.);
  //templateFunc -> FixParameter(0, 1./templateFunc ->Integral(0.,5.) ); // normalized to 1. BUT will be renormalized to 1 at each iteration!
  
  TFitterMinuit* myfit = new TFitterMinuit(1);
  myfit->SetFCN(mylike);
  myfit->SetPrintLevel(-1);
  
  
  
  for(int i = 0; i < nBins; ++i)
  {
    h_EoP[i] -> Rebin(rebin*4);    
    h_EoC[i] -> Rebin(rebin*4);    
    
    
    //------------------------------------
    // Fill the graph for uncorrected data
    
    // fit uncorrected data
    mydata = dataEoP.at(i);
    
    myfit->Clear();
    myfit->SetParameter(0, "scale", 1.,0.0005,0.50,1.50);
    
    double arglist[2];
    arglist[0] = 10000; // Max number of function calls
    arglist[1] = 1e-5;  // Tolerance on likelihood ?????????

    int fStatus = myfit->ExecuteCommand("MIGRAD",arglist,2); 
    
    double amin,edm,errdef;
    int nvpar,nparx;
    myfit->GetStats(amin, edm, errdef, nvpar, nparx);
    double bestScale = myfit->GetParameter(0);
    double eee       = myfit->GetParError(0);
    
    char funcName[50];
    sprintf(funcName,"f_EoP_%d",i);
    f_EoP[i] = new TF1(funcName, crystalBallLowHigh, 0., 10., 8);
    
    f_EoP[i] -> SetParameter(0,1.);
    for(int parIt = 1; parIt < 7; ++parIt)
      f_EoP[i] -> SetParameter(parIt,templateFunc->GetParameter(parIt));
    f_EoP[i] -> SetParameter(7,bestScale);
    f_EoP[i] -> SetParameter(0,h_EoP[i]->GetEntries()*h_EoP[i]->GetBinWidth(0)/f_EoP[i]->Integral(0.,10.));
    
    f_EoP[i] -> SetLineWidth(1); 
    f_EoP[i] -> SetLineColor(kRed+2); 
    f_EoP[i] -> SetNpx(10000);
    
    // Fill the graph
    if( fStatus == 0 && eee > 0. )
    {
      g_fit -> SetPoint(i,  h_Las[i]->GetMean() , 1./bestScale);
      g_fit -> SetPointError(i, h_Las[i]->GetRMS(), h_Las[i]->GetRMS(), eee, eee);
      h_EoP_spread -> Fill(1./bestScale);
    }
    else
      std::cout << "Fitting uncorrected time bin: " << i << "   Fail status: " << fStatus << "   sigma: " << eee << endl;
    
    
    
    //----------------------------------
    // Fill the graph for corrected data

    // fit uncorrected data
    
    mydata = dataEoC.at(i);
    
    myfit->Clear();
    myfit->SetParameter(0, "scale", 1.,0.0005,0.50,1.50);
    
    arglist[0] = 10000; // Max number of function calls
    arglist[1] = 1e-5;  // Tolerance on likelihood ?????????

    fStatus = myfit->ExecuteCommand("MIGRAD",arglist,2); 
    
    myfit->GetStats(amin, edm, errdef, nvpar, nparx);
    bestScale = myfit->GetParameter(0);
    eee       = myfit->GetParError(0);    
    
    
    sprintf(funcName,"f_EoC_%d",i);
    f_EoC[i] = new TF1(funcName, crystalBallLowHigh, 0., 10., 8);
    
    f_EoC[i] -> SetParameter(0,1.);
    for(int parIt = 1; parIt < 7; ++parIt)
      f_EoC[i] -> SetParameter(parIt,templateFunc->GetParameter(parIt));
    f_EoC[i] -> SetParameter(7,bestScale);
    f_EoC[i] -> SetParameter(0,h_EoP[i]->GetEntries()*h_EoP[i]->GetBinWidth(0)/f_EoC[i]->Integral(0.,10.));

    
    f_EoC[i] -> SetLineWidth(1); 
    f_EoC[i] -> SetLineColor(kGreen+2); 
    f_EoC[i] -> SetNpx(10000);

    // fill the graph
    if( fStatus == 0 && eee > 0. )
    {        
      g_c_fit -> SetPoint(i,  h_Las[i]->GetMean() , 1./bestScale);
      g_c_fit -> SetPointError(i, h_Las[i]->GetRMS() , h_Las[i]->GetRMS() , eee, eee);
      h_EoC_spread -> Fill(1./bestScale);
    }
    else 
      std::cout << "Fitting corrected time bin: " << i << "   Fail status: " << fStatus << "   sigma: " << eee << endl;
    
    
    
  }
  
  TF1* pol0 = new TF1("pol0","pol0");
  pol0 -> SetLineColor(kGreen+2);
  pol0 -> SetLineWidth(3);
  pol0 -> SetLineStyle(2);
  g_c_fit -> Fit("pol0","Q+");
  
  
  // Drawings
  TPaveStats** s_EoP = new TPaveStats*[nBins];
  TPaveStats** s_EoC = new TPaveStats*[nBins];
  
  TCanvas *c1[100]; 
  for(int i = 0; i < nBins; ++i)
  {
    char canvasName[50];
    if (i%2==0) {
      sprintf(canvasName, "Fits-%0d", i/2); 
      c1[i/2] = new TCanvas(canvasName, canvasName);
      c1[i/2] -> Divide(2,1);
    }
    c1[i/2] -> cd (i%2+1);

    h_EoP[i] -> GetXaxis() -> SetTitle("E/p");  
    if( strcmp(EBEE,"EB") == 0 )
      h_EoP[i] -> GetXaxis() -> SetRangeUser(0.5,1.5);
    else
      h_EoP[i] -> GetXaxis() -> SetRangeUser(0.5,2.5);
    h_EoP[i] -> Draw("ep");
    gPad->Update();
    s_EoP[i]= (TPaveStats*)(h_EoP[i]->GetListOfFunctions()->FindObject("stats"));
    s_EoP[i]->SetTextColor(kRed+2);

    h_EoC[i] -> Draw("epsames");
    gPad->Update(); 
    s_EoC[i]= (TPaveStats*)(h_EoC[i]->GetListOfFunctions()->FindObject("stats"));
    s_EoC[i]->SetY1NDC(0.59); //new x start position
    s_EoC[i]->SetY2NDC(0.79); //new x end position
    s_EoC[i]->SetTextColor(kGreen+2);
    s_EoC[i]->Draw();
    
    f_EoP[i]->Draw("same");
    f_EoC[i]->Draw("same");    
  }

  /*  
  TCanvas *c2[100]; 
  for(int i = 0; i < nBins; ++i)
  {
    char canvasName[50];
    if (i%6==0) {
      sprintf(canvasName, "LaserCorr-%0d", i/6); 
      c2[i/6] = new TCanvas(canvasName, canvasName);
      c2[i/6] -> Divide(3,2);
    }
        
    c2[i/6] -> cd (i%6+1);
    h_Las[i] -> GetXaxis() -> SetTitle("laser correction");
    h_Las[i] -> GetXaxis() -> SetRangeUser(0.5,1.5); 
    h_Las[i] -> Draw("");
    gPad->Update();
    s_Las[i]= (TPaveStats*)(h_Las[i]->GetListOfFunctions()->FindObject("stats"));
    s_Las[i]->SetTextColor(kBlack);
  }
  */

  /*  
  TCanvas *cmap = new TCanvas("cmap","cmap");
  cmap->cd();
  gStyle->SetPalette(1);
  h_LC_map->Draw("colz");
  */
  
  // Final plots
  TCanvas* cplot = new TCanvas("gplot", "gplot",100,100,725,500);
  cplot->cd();

  TPad *cLeft  = new TPad("pad_0","pad_0",0.00,0.00,0.64,1.00);
  TPad *cRight = new TPad("pad_1","pad_1",0.64,0.00,1.00,1.00);

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
  
  // pad settings
  TH1F *hPad = (TH1F*)gPad->DrawFrame(lcMin,0.9,lcMax,1.05);
  hPad->GetXaxis()->SetTitle("Laser correction");
  hPad->GetYaxis()->SetTitle("Relative E/p scale"); 
  hPad->GetYaxis()->SetTitleOffset(tYoffset);
  hPad->GetXaxis()->SetLabelSize(labSize);
  hPad->GetXaxis()->SetTitleSize(labSize);
  hPad->GetYaxis()->SetLabelSize(labSize);
  hPad->GetYaxis()->SetTitleSize(labSize);
  if ( strcmp(EBEE,"EB")==0 )
  {  
    hPad -> SetMinimum(0.950);
    hPad -> SetMaximum(1.010);
  }
  else 
  {  
    hPad -> SetMinimum(0.910);
    hPad -> SetMaximum(1.030);
  }
    
  // draw trend plot
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
}
