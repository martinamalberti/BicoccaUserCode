//------------

#include "../histoFunc.h"
#include "./ntupleUtils.h"
#include "./ntupleUtils.cc"
#include "./ClusterCalibTools.h"
#include "./ClusterCalibTools.cc"

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
#include "TFitterMinuit.h"
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

void CheckRegionalScale_unbinnedfit(Char_t* EBEE = 0){
  
  // Set style options
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetOptTitle(0); 
  gStyle->SetOptStat(1110); 
  gStyle->SetOptFit(1); 
  gStyle->SetPalette(1); 
  
  int useEvenOdd = 0;  
  
  // Check qualifiers
  if ( strcmp(EBEE,"EB")!=0 && strcmp(EBEE,"EE")!=0 )
  {
    std::cout << "CHK-STB Error: unknown partition " << EBEE << std::endl;
    std::cout << "CHK-STB Select either EB or EE ! " << std::endl;
    return;
  }
  
 
  //Define regions for reference
  EBregionBuilder* regionTemplate = BuildEBRegion("inputREGION_template.txt","h_EBRegion_template");
  TH2F* h_EBRegionTemplate = regionTemplate->DrawEBRegion();

  // Define regions where you want to check the scale
  EBregionBuilder* region = BuildEBRegion("inputREGION.txt","h_EBRegion");
  TH2F* h_EBRegion = (TH2F*)region->DrawEBRegion();

 
  // Get trees
  std::cout << std::endl;
  
  // Data to fit
  TChain* ntu_DA = new TChain("ntu");
  FillChain(ntu_DA,"inputDATA.txt");
  std::cout << "     DATA: " << std::setw(8) << ntu_DA->GetEntries() << " entries" << std::endl;
  
  // Reference
  TChain* ntu_MC = new TChain("ntu");
  FillChain(ntu_MC,"inputMC.txt");
  std::cout << "REFERENCE: " << std::setw(8) << ntu_MC->GetEntries() << " entries" << std::endl;
  
  if (ntu_DA->GetEntries() == 0 || ntu_MC->GetEntries() == 0 )
    {
      std::cout << "CHK-STB Error: At least one file is empty" << std::endl; 
      return;
    }
    
  
  // Set branch addresses
  int isW;
  float EoP, scEta;
  int Ieta, Iphi;
  
  ntu_DA->SetBranchAddress("isW", &isW);
  ntu_DA->SetBranchAddress("ele1_scEta", &scEta);
  ntu_DA->SetBranchAddress("ele1_EOverP", &EoP);
  ntu_DA->SetBranchAddress("ele1_seedIeta", &Ieta);
  ntu_DA->SetBranchAddress("ele1_seedIphi", &Iphi);

  ntu_MC->SetBranchAddress("isW", &isW);
  ntu_MC->SetBranchAddress("ele1_scEta", &scEta);
  ntu_MC->SetBranchAddress("ele1_EOverP", &EoP);
  ntu_MC->SetBranchAddress("ele1_seedIeta", &Ieta);
  ntu_MC->SetBranchAddress("ele1_seedIphi", &Iphi);

  
  // Build the reference from 'infile2'
  std::cout << std::endl;
  std::cout << "***** Build reference *****" << std::endl;

  std::map<int,TH1F*> href;
  std::map<int,TF1*> fref;
  char obj_name[100];
  for(int ientry = 0; ientry < ntu_MC->GetEntries(); ++ientry)
    {
      // use only even events to build templates
      if ( useEvenOdd==1  && ientry%2!=0) continue; 

      if( (ientry%100000 == 0) ) std::cout << "reading MC entry " << ientry << std::endl;
      ntu_MC->GetEntry(ientry);
      if (strcmp(EBEE,"EB")==0 && fabs(scEta) > 1.45) continue;                       // barrel
      if (strcmp(EBEE,"EE")==0 && (fabs(scEta) < 1.47 || fabs(scEta)>2.7 )) continue; // endcap
      
      // define the region and fill the reference histograms
      int regionId = regionTemplate->EBRegionId(Ieta,Iphi);
      if (regionId==-1) continue;
      std::map<int,TH1F*>::const_iterator itr = href.find(regionId);
   
      if (itr == href.end())
	{ 
	  sprintf(obj_name,"obj_region%02d",regionId);
	  href[regionId] = new TH1F(obj_name,"",1200, 0., 3.);
	  href[regionId]->Fill(EoP);
	}
      else href[regionId]->Fill(EoP);
    }
 

    
  std::cout << "References built for " << EBEE <<  std::endl;  
  for (std::map<int,TH1F*>::const_iterator itr = href.begin(); itr != href.end(); ++itr)
    {
      std::cout << itr->first << "    -- events :" << (itr->second) -> GetEntries() << std::endl;
    }
  
      
  // histograms definition
  double *xbins = new double;
  double *ybins = new double;
  int nx = 0;
  for (int ii = 1; ii < 362; ii++){
    if ( (ii % region->EBRegionPhiWidth()) == 1 ) { 
      xbins[nx]=ii; 
      nx++;
    }
  }

  int ny = 0;
  for (int jj = -85; jj < 87; jj++){
    if ( (jj == -85) || (jj == 86))  {
      ybins[ny]=jj;
      ny++;
    }
    if (abs(jj) % region->EBRegionEtaWidth() == 1) {
      ybins[ny]=jj;
      ny++;
    }
  }

  TH2F* h_scale_EB  = new TH2F("h_scale_EB","",nx-1, xbins, ny-1, ybins);
  TH2F* h_error_EB  = new TH2F("h_error_EB","",nx-1, xbins, ny-1, ybins );
  TH2F* h_occupancy_EB  = new TH2F("h_occupancy_EB","",nx-1, xbins, ny-1, ybins );

  TH1F* h_EoP_spread = new TH1F("h_EoP_spread","",100,0.9,1.1);
  TH1F* h_EoP_spread_centralregion = new TH1F("h_EoP_spread_centralregion","",100,0.9,1.1);
  TH1F* h_EoP_spread_outerregion = new TH1F("h_EoP_spread_outerregion","",100,0.9,1.1);

  TH1F* h_EoP_error = new TH1F("h_EoP_error","",1000,0.,0.25);
     
  std::map<int,TH1F*> h_EoP;
  std::map<int,int> indref;

  // data definition
  std::vector< std::vector<double>* > dataEoP;
  for (int jbin = 0; jbin< region->EBregionsNum(); jbin++){
    dataEoP.push_back(new std::vector<double>);
  }

  std::cout << std::endl;
  std::cout << "***** Fill histograms *****" << std::endl;
  
  // Loop over events
  int nEntries = ntu_DA -> GetEntriesFast(); 
  for(int ientry = 0; ientry < nEntries; ++ientry)
    {
      // use only odd events to fit
      if ( useEvenOdd==1  && ientry%2!=1) continue; 
      
      if( (ientry%100000 == 0) ) std::cout << "reading entry " << ientry << std::endl;
      
      ntu_DA->GetEntry(ientry);
      
      // save only what is needed for the analysis!!!
      if (strcmp(EBEE,"EB")==0 && fabs(scEta) > 1.45) continue;                       // barrel
      if (strcmp(EBEE,"EE")==0 && (fabs(scEta) < 1.47 || fabs(scEta)>2.7 )) continue; // endcap
 
      // fill the data histograms
      int regionId = region->EBRegionId(Ieta,Iphi);
      if ( regionId == -1) continue;
      
      // matching ref regions
      int regionIdRef = regionTemplate->EBRegionId(Ieta,Iphi);
      if ( regionIdRef == -1) continue;

      std::map<int,int>::const_iterator itr2 = indref.find(regionId);
      if (itr2 == indref.end()) indref[regionId] = regionIdRef;
                  
      std::map<int,TH1F*>::const_iterator itr = h_EoP.find(regionId);
      if (itr == h_EoP.end())
	{ 
	  sprintf(obj_name,"EoP_region%02d",regionId);
	  h_EoP[regionId] = new TH1F(obj_name,obj_name,1200, 0., 3.);
	  h_EoP[regionId] -> SetFillColor(kGreen+2);
	  h_EoP[regionId] -> SetFillStyle(3004);
	  h_EoP[regionId] -> SetMarkerStyle(20);
	  h_EoP[regionId] -> SetMarkerSize(0.7);
	  h_EoP[regionId] -> SetMarkerColor(kGreen+2);
	  h_EoP[regionId] -> SetLineColor(kGreen+2);
	  h_EoP[regionId] -> Fill(EoP);
	}
      else h_EoP[regionId]->Fill(EoP);
      
      
      // fill the vectors data E/p 
      if ( EoP > 0.5 && EoP < 1.5 ) {
	dataEoP[regionId]->push_back(EoP);
	if (Ieta>0) h_occupancy_EB -> Fill( Iphi, Ieta);
 	if (Ieta<0) h_occupancy_EB -> Fill( Iphi, Ieta-1); // ????
      }
      
    }
  

  // Define graphs and histograms
  TGraphAsymmErrors* gregion   = new TGraphAsymmErrors();

  int nEtaRegions = regionTemplate->EBregionsNum();
  TGraphAsymmErrors* gphi[nEtaRegions]; 
  vector<int> np;
  for (int ir = 0; ir < nEtaRegions; ir++){
    char gname[100], gtitle[100];
    sprintf(gname,"gphi_etaRegion%d",ir);
    sprintf(gtitle,"<#eta> = %d",(int)regionTemplate->EBRegionEta(ir));
    gphi[ir] = new TGraphAsymmErrors();
    gphi[ir]->SetName(gname);
    gphi[ir]->SetTitle(gtitle);
    np.push_back(0);
  }
  
  int nGoodFit  = 0;
  int nBadFit   = 0;
  int nTot = 0;
  
  // rebinning histos
  int rebin = 4;
    
  // define the fitting functions from templates
  templateHisto = new TH1F("templateHisto", "", 1200, 0., 5.);
  
  for (unsigned int ii = 0; ii < href.size();ii++){
    href[ii] -> Rebin(rebin);
    templateHisto = (TH1F*)href[ii]->Clone("templateHisto");
    FitTemplate();       
    fref[ii] = new TF1("fref", templateFunc,0,10,8);
    for(int parIt = 0; parIt < 8; ++parIt)
      fref[ii] -> SetParameter(parIt,templateFunc->GetParameter(parIt));
  }

    
  // AND NOW ... FIT!
  std::cout << std::endl;
  std::cout << "***** Start fitting ... *****" << std::endl;  

  std::map<int,TF1*> f_EoP;

  TVirtualFitter::SetDefaultFitter("Minuit2");
  TFitterMinuit* myfit = new TFitterMinuit(1);
  myfit->SetPrintLevel(-1);
  double arglist[2];

  for (std::map<int,TH1F*>::const_iterator data_itr = h_EoP.begin(); data_itr != h_EoP.end(); ++data_itr)
    {
      
      (data_itr->second) -> Rebin(rebin*4);
      
      int idata = data_itr->first; 
      
      if( idata%100==0) 
	cout << "Fitting region " << idata << endl;
      
      mydata = dataEoP.at(idata);
      
      // match actual data region with ref region
      std::map<int,int>::const_iterator index_itr = indref.find(idata);
      int iref = index_itr->second; 
      
      // define the fitting function  
      templateFunc = fref[iref];
      
      myfit->SetFCN(mylike);
      myfit->Clear();
      arglist[0] = 10000; // Max number of function calls
      arglist[1] = 1e-2;  // Tolerance on likelihood ?????????
      double step = h_EoP[idata]->GetRMS()/sqrt(h_EoP[idata]->GetEntries());
      myfit->SetParameter(0, "scale", 1.,step,0.5,1.5);
      int fStatus = myfit->ExecuteCommand("MIGRAD",arglist,2); 
      
      //if Fit did not converge, try with different step size, up to a maximum of 10 trials
      int ntrials = 1;
      while ( fStatus!=0 && ntrials < 10) {
	myfit->Clear();
	myfit->SetParameter(0, "scale", h_EoP[idata]->GetBinCenter(h_EoP[idata]->GetMaximumBin()),step*(ntrials+1),0.5,1.5);
	fStatus = myfit->ExecuteCommand("MIGRAD",arglist,2); 
	//cout << ntrials << " " << step*(ntrials+1) << fStatus << endl;
	ntrials++;
      }
      
      double amin,edm,errdef;
      int nvpar,nparx;
      myfit->GetStats(amin, edm, errdef, nvpar, nparx);
      double bestScale = myfit->GetParameter(0);
      double eee       = myfit->GetParError(0);

      double nEntriesInRegion =  h_EoP[idata] ->GetEntries();
      double xNorm = h_EoP[idata]->GetMaximum();
           
      char funcName[50];
      sprintf(funcName,"f_EoP_%d", idata );
      f_EoP[idata] =  new TF1(funcName,templateFunc,0.,10.,8);
      f_EoP[idata] -> SetParameter(0,xNorm);
      f_EoP[idata] -> SetParameter(1,templateFunc->GetParameter(1));
      f_EoP[idata] -> SetParameter(2,templateFunc->GetParameter(2));
      f_EoP[idata] -> SetParameter(3,templateFunc->GetParameter(3));
      f_EoP[idata] -> SetParameter(4,templateFunc->GetParameter(4));
      f_EoP[idata] -> SetParameter(5,templateFunc->GetParameter(5));
      f_EoP[idata] -> SetParameter(6,templateFunc->GetParameter(6));
      f_EoP[idata] -> SetParameter(7,bestScale);
      f_EoP[idata] -> SetParError(7,eee);
      f_EoP[idata] -> SetLineColor(kRed+2);  
      f_EoP[idata] -> SetLineWidth(1); 
      f_EoP[idata] -> SetParName(0,"Norm"); 
      f_EoP[idata] -> SetParName(1,"Scale factor"); 
      f_EoP[idata] -> SetNpx(10000);
      
      //fill the graphs
      double aveEta = region->EBRegionEta(idata);
      double avePhi = region->EBRegionPhi(idata);
      nTot++;
      if ( fStatus==0 )
	{
	  nGoodFit++;
	  gregion      -> SetPoint(nGoodFit,  idata , bestScale);
	  gregion      -> SetPointError(nGoodFit, 0 , 0, eee, eee);
      	  h_EoP_spread -> Fill(bestScale);
	  if ( fabs(aveEta) <= 40 ) h_EoP_spread_centralregion -> Fill(bestScale);
	  if ( fabs(aveEta) > 40  ) h_EoP_spread_outerregion   -> Fill(bestScale);

	  h_EoP_error  -> Fill(eee);
	  h_scale_EB   -> Fill(avePhi,aveEta, bestScale );
	  h_error_EB   -> Fill(avePhi,aveEta, eee );

	  gphi[iref] -> SetPoint(np[iref],  avePhi  , bestScale);
 	  gphi[iref] -> SetPointError(np[iref], 0. , 0., eee, eee);
 	  np[iref]++;
	}
      else
	{
	  std::cout << "Fitting region : " << idata << "   Fail status: " << fStatus 
		    << "   scale: "        << bestScale <<"   sigma: " << eee << endl;
	  h_scale_EB -> Fill(avePhi,aveEta, 1.);
	  nBadFit++;
	}
        
    }
  

  cout << "Fraction of CONVERGED fits: " << nGoodFit << "/"<< nTot << " = " << (float)nGoodFit/(float)nTot<<endl;
  cout << "Fraction of NOT CONVERGED fits: " << nBadFit << "/"<< nTot << " = " << (float)nBadFit/(float)nTot<<endl;

  
  // Drawings

  TCanvas *creference[5000]; 
  for(int i = 0; i < min(60,regionTemplate->EBregionsNum()); ++i){
    char canvasName[50];
    if (i%6==0) {
      sprintf(canvasName, "template-%0d", i/6); 
      creference[i/6] = new TCanvas(canvasName, canvasName);
      creference[i/6] -> Divide(3,2);
    }
    
    creference[i/6] -> cd (i%6+1);
    href[i] -> SetFillColor(kCyan+2);
    href[i] -> GetXaxis() -> SetTitle("E/p");
    href[i] -> GetXaxis() -> SetRangeUser(0.3,1.7); 
    href[i] -> Draw("histos");
    fref[i] -> SetLineColor(2);
    fref[i] -> SetLineWidth(2);
    href[i] -> Fit(fref[i],"QRS");
    gPad->Update();
  }
  
  
  TCanvas *cfit[5000]; 
  for(int i = 0; i < min(30,region->EBregionsNum()); ++i){
    char canvasName[50];
    if (i%6==0) {
      sprintf(canvasName, "fit-%0d", i/6); 
      cfit[i/6] = new TCanvas(canvasName, canvasName);
      cfit[i/6] -> Divide(3,2);
    }
    
    cfit[i/6] -> cd (i%6+1);
    h_EoP[i] -> GetXaxis() -> SetTitle("E/p");
    h_EoP[i] -> GetXaxis() -> SetRangeUser(0.3,1.7); 
    h_EoP[i] -> Draw("e1");
    f_EoP[i] -> Draw("sames+");
    char ll[100];
    sprintf(ll,"scale = %.3f +/- %.3f",f_EoP[i] -> GetParameter(7),f_EoP[i] -> GetParError(7));
    TLatex *latex = new TLatex(0.2,0.8,ll);
    latex->SetNDC();
    latex->SetTextFont(42);
    latex->SetTextSize(0.05);
    latex->Draw("same");
  }


  TCanvas *c = new TCanvas("c","c");
  c -> cd();
  c->SetGridx();
  c->SetGridy();
  gregion->SetMarkerStyle(20);
  gregion->SetMarkerSize(0.8);
  gregion->SetMarkerColor(kGreen+2);
  gregion -> Draw("ap");
  gregion -> GetHistogram()->GetXaxis() ->SetTitle("region index");
  gregion -> GetHistogram()->GetYaxis() ->SetTitle("scale");
  gregion -> GetHistogram()->GetYaxis() ->SetRangeUser(0.9,1.1);

  TLegend * leg = new TLegend(0.69, 0.69, 0.89, 0.89);
  leg->SetFillColor(0);

  TCanvas *cc = new TCanvas("cc","cc");
  cc -> cd();
  cc->SetGridx();
  cc->SetGridy();
  int mycolor ;
  for ( int i = 0; i < nEtaRegions ; i++){
    gphi[i]->SetMarkerStyle(20);
    gphi[i]->SetMarkerSize(0.8);
    if (i==9) {
      mycolor = 1; 
      gphi[i]->SetMarkerStyle(24);
    }
    else { mycolor = i+1; }
    gphi[i]->SetMarkerColor(mycolor);
    gphi[i]->SetLineColor(mycolor);
    if (i==0) {
      gphi[i] -> Draw("ap");
      leg->AddEntry(gphi[i], gphi[i]->GetTitle(), "PL");
    }
    if (i!=0 && i%(nEtaRegions/5)==0)  {
      //if ( i!=0 )  {
      gphi[i] -> Draw("psame");
      leg->AddEntry(gphi[i], gphi[i]->GetTitle(), "PL");
      leg->Draw("same");
    }
    gphi[i] -> GetHistogram()->GetXaxis() ->SetTitle("#phi");
    gphi[i] -> GetHistogram()->GetYaxis() ->SetTitle("scale");
    gphi[i] -> GetHistogram()->GetXaxis() ->SetRangeUser(0.,361);
    gphi[i] -> GetHistogram()->GetYaxis() ->SetRangeUser(0.95,1.05);
  }

  TCanvas *cmap = new TCanvas("cmap","cmap");
  cmap -> cd();
  cmap->SetLeftMargin(0.1); 
  cmap->SetRightMargin(0.15); 
  h_scale_EB->Draw("COLZ");
  h_scale_EB->GetXaxis() ->SetTitle("#phi");
  h_scale_EB->GetYaxis() ->SetTitle("#eta");
  h_scale_EB->GetZaxis() ->SetRangeUser(0.95,1.05);

  TCanvas *cmapErr = new TCanvas("cmapErr","cmapErr");
  cmapErr->SetLeftMargin(0.1); 
  cmapErr->SetRightMargin(0.15); 
  cmapErr -> cd();
  h_error_EB->Draw("COLZ");
  h_error_EB->GetXaxis() ->SetTitle("#phi");
  h_error_EB->GetYaxis() ->SetTitle("#eta");
  h_error_EB->GetZaxis() ->SetRangeUser(0.,2*h_EoP_error->GetRMS());
  
  TCanvas *cOcc = new TCanvas("cOcc","cOcc");
  cOcc->SetLeftMargin(0.1); 
  cOcc->SetRightMargin(0.15); 
  cOcc -> cd();
  h_occupancy_EB->Draw("COLZ");
  h_occupancy_EB->GetXaxis() ->SetTitle("#phi");
  h_occupancy_EB->GetYaxis() ->SetTitle("#eta");
  

  TCanvas* cspread_centralregion = new TCanvas("cspread_centralregion", "cspread_centralregion",500,500);
  cspread_centralregion->cd();
  h_EoP_spread_centralregion -> SetFillStyle(3001);
  h_EoP_spread_centralregion -> SetLineWidth(3001);
  h_EoP_spread_centralregion -> SetFillColor(kGreen+2);
  h_EoP_spread_centralregion -> SetLineColor(kGreen+2);
  h_EoP_spread_centralregion -> GetXaxis() -> SetTitle("Relative E/p scale");
  h_EoP_spread_centralregion -> Draw("hs");
  //h_EoP_spread_centralregion   -> Fit("gaus");
  gPad->Update();

  TPaveStats *s_EoP_spread_centralregion = (TPaveStats*)(h_EoP_spread_centralregion->GetListOfFunctions()->FindObject("stats"));
  s_EoP_spread_centralregion -> SetX1NDC(0.55); //new x start position
  s_EoP_spread_centralregion -> SetX2NDC(0.89); //new x end position
  s_EoP_spread_centralregion -> SetY1NDC(0.750); //new x start position
  s_EoP_spread_centralregion -> SetY2NDC(0.85); //new x end position
  s_EoP_spread_centralregion -> SetOptStat(1110);
  s_EoP_spread_centralregion -> SetTextColor(kGreen+2);
  s_EoP_spread_centralregion -> SetTextSize(0.03);
  s_EoP_spread_centralregion -> Draw("sames");

  TCanvas* cspread_outerregion = new TCanvas("cspread_outerregion", "cspread_outerregion",500,500);
  cspread_outerregion->cd();
  h_EoP_spread_outerregion -> SetFillStyle(3001);
  h_EoP_spread_outerregion -> SetLineWidth(3001);
  h_EoP_spread_outerregion -> SetFillColor(kGreen+2);
  h_EoP_spread_outerregion -> SetLineColor(kGreen+2);
  h_EoP_spread_outerregion -> GetXaxis() -> SetTitle("Relative E/p scale");
  h_EoP_spread_outerregion -> Draw("hs");
  //h_EoP_spread_outerregion -> Fit("gaus");
  gPad->Update();

  TPaveStats *s_EoP_spread_outerregion = (TPaveStats*)(h_EoP_spread_outerregion->GetListOfFunctions()->FindObject("stats"));
  s_EoP_spread_outerregion -> SetX1NDC(0.55); //new x start position
  s_EoP_spread_outerregion -> SetX2NDC(0.89); //new x end position
  s_EoP_spread_outerregion -> SetY1NDC(0.750); //new x start position
  s_EoP_spread_outerregion -> SetY2NDC(0.85); //new x end position
  s_EoP_spread_outerregion -> SetOptStat(1110);
  s_EoP_spread_outerregion -> SetTextColor(kGreen+2);
  s_EoP_spread_outerregion -> SetTextSize(0.03);
  s_EoP_spread_outerregion -> Draw("sames");

  TCanvas* cspread = new TCanvas("cspread", "cspread",500,500);
  cspread->cd();
  h_EoP_spread -> SetFillStyle(3001);
  h_EoP_spread -> SetLineWidth(3001);
  h_EoP_spread -> SetFillColor(kGreen+2);
  h_EoP_spread -> SetLineColor(kGreen+2);
  h_EoP_spread -> GetXaxis() -> SetTitle("Relative E/p scale");
  h_EoP_spread -> Draw("hs");
  gPad->Update();

  TPaveStats *s_EoP_spread = (TPaveStats*)(h_EoP_spread->GetListOfFunctions()->FindObject("stats"));
  s_EoP_spread -> SetX1NDC(0.55); //new x start position
  s_EoP_spread -> SetX2NDC(0.89); //new x end position
  s_EoP_spread -> SetY1NDC(0.750); //new x start position
  s_EoP_spread -> SetY2NDC(0.85); //new x end position
  s_EoP_spread -> SetOptStat(1110);
  s_EoP_spread -> SetTextColor(kGreen+2);
  s_EoP_spread -> SetTextSize(0.03);
  s_EoP_spread -> Draw("sames");

  TCanvas* cErr = new TCanvas("cErr", "cErr",500,500);
  cErr->cd();
  h_EoP_error -> SetFillStyle(3001);
  h_EoP_error -> SetLineWidth(3001);
  h_EoP_error -> SetFillColor(kGreen+2);
  h_EoP_error -> SetLineColor(kGreen+2);
  h_EoP_error -> GetXaxis() -> SetTitle("error");
  h_EoP_error -> GetXaxis() -> SetRangeUser(0.,10*h_EoP_error->GetRMS());
  h_EoP_error -> Draw("hs");
  gPad->Update();

  TPaveStats *s_scale_error = (TPaveStats*)(h_EoP_error->GetListOfFunctions()->FindObject("stats"));
  s_scale_error->SetX1NDC(0.55); //new x start position
  s_scale_error->SetX2NDC(0.89); //new x end position
  s_scale_error->SetY1NDC(0.750); //new x start position
  s_scale_error->SetY2NDC(0.875); //new x end position
  s_scale_error ->SetOptStat(1110);
  s_scale_error ->SetTextColor(kGreen+2);
  s_scale_error ->SetTextSize(0.03);
  s_scale_error -> Draw("sames");

  
  TCanvas *cTemplate = new TCanvas("cTemplate","cTemplate");
  cTemplate -> cd();
  h_EBRegionTemplate -> Draw("colz");
  
  TCanvas *cRegion = new TCanvas("cRegion","cRegion");
  cRegion -> cd();
  h_EBRegion ->Draw("colz");



  // save plots in a root file  
  //std::string dirname("./plots/Regions20x20/May10ReReco_2011Laser_Bon_V20110616_weightedAverage/");
  //std::string dirname("./plots/Regions20x20/May10ReReco/");

  std::string dirname("./plots/TTscale/May10ReReco_plus_PromptReco_2011Laser_Bon_V20110616_weightedAverage/");
  //std::string dirname("./plots/TTscale/May10ReReco_plus_PromptReco_2011Laser_Bon_V20110616_weightedAverage_TemplateOddEvents/");
  
  std::string outfilename("checkSMscale.root");

  TFile* o = new TFile((dirname+outfilename).c_str(),"RECREATE");
  
  o -> cd();
  
  gregion      -> Write("gregion");
  for (int i=0; i< nEtaRegions; i++){
    gphi[i]     -> Write();
  }

  h_EoP_spread               -> Write("h_EoP_spread");
  h_EoP_spread_centralregion -> Write("h_EoP_spread_centralregion");
  h_EoP_spread_outerregion   -> Write("h_EoP_spread_outerregion");
  h_EoP_error                -> Write("h_EoP_error");
  
  h_scale_EB       -> Write("h_scale_EB_map");
  h_error_EB       -> Write("h_error_EB_map");
  h_occupancy_EB   -> Write("h_occupancy_EB");
  o -> Close();
 
  
  // print plots
  std::string plotname;

  creference[0] -> cd();
  plotname = "exampleReferences.gif";
  creference[0] -> Print((dirname+plotname).c_str());

  cfit[0] ->cd(); 
  plotname = "exampleFits.gif";
  cfit[0] -> Print((dirname+plotname).c_str());
  
  c->cd();
  plotname = "RelativeScale_vs_regionIndex.gif";
  c -> Print((dirname+plotname).c_str());
 
  cc->cd();
  plotname="RelativeScale_vs_phi.gif";
  cc->Print((dirname+plotname).c_str());

  cspread->cd();
  plotname="RelativeScale_spread.gif";
  cspread->Print((dirname+plotname).c_str()); 

  cspread_centralregion->cd();
  plotname="RelativeScale_spread_centralBarrel.gif";
  cspread_centralregion->Print((dirname+plotname).c_str()); 

  cspread_outerregion->cd();
  plotname="RelativeScale_spread_outerBarrel.gif";
  cspread_outerregion->Print((dirname+plotname).c_str()); 

  cErr->cd();
  plotname="RelativeScale_error.gif";
  cErr->Print((dirname+plotname).c_str()); 

  gStyle->SetOptStat(0);
  cmap->cd();
  plotname="RelativeScale_map.gif";
  cmap->Print((dirname+plotname).c_str());

  cmapErr->cd();
  plotname="RelativeScale_errorsmap.gif";
  cmapErr->Print((dirname+plotname).c_str());
  
  cOcc->cd();
  plotname="occupancy_map.gif";
  cOcc->Print((dirname+plotname).c_str());

}
