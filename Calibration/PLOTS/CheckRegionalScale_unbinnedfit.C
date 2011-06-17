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
  
  

  // Check qualifiers
  if ( strcmp(EBEE,"EB")!=0 && strcmp(EBEE,"EE")!=0 )
  {
    std::cout << "CHK-STB Error: unknown partition " << EBEE << std::endl;
    std::cout << "CHK-STB Select either EB or EE ! " << std::endl;
    return;
  }
  
  int useEvenOdd = 0;  
 
  //Define regions for reference
  EBregionBuilder* regionTemplate = BuildEBRegion("inputREGION_template.txt","h_EBRegion_template");
  TH2F* h_EBRegionTemplate = regionTemplate->DrawEBRegion();

  // Define regions where you want to check the scale
  EBregionBuilder* region = BuildEBRegion("inputREGION.txt","h_EBRegion");
  TH2F* h_EBRegion = (TH2F*)region->DrawEBRegion();

    
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
  int isW, isZ;
  float EoP, scEta;
  int seedIeta, seedIphi, seedIx, seedIy, seedZside;
  int Ieta, Iphi;
  
  ntu_DA->SetBranchAddress("isW", &isW);
  ntu_DA->SetBranchAddress("isZ", &isZ);
  ntu_DA->SetBranchAddress("ele1_scEta", &scEta);
  ntu_DA->SetBranchAddress("ele1_EOverP", &EoP);
  ntu_DA->SetBranchAddress("ele1_seedIeta", &Ieta);
  ntu_DA->SetBranchAddress("ele1_seedIphi", &Iphi);
  ntu_DA->SetBranchAddress("ele1_seedIx", &seedIx);
  ntu_DA->SetBranchAddress("ele1_seedIy", &seedIy);
  ntu_DA->SetBranchAddress("ele1_seedZside", &seedZside);
  
  ntu_MC->SetBranchAddress("isW", &isW);
  ntu_MC->SetBranchAddress("isZ", &isZ);
  ntu_MC->SetBranchAddress("ele1_scEta", &scEta);
  ntu_MC->SetBranchAddress("ele1_EOverP", &EoP);
  ntu_MC->SetBranchAddress("ele1_seedIeta", &Ieta);
  ntu_MC->SetBranchAddress("ele1_seedIphi", &Iphi);
  
  
  std::map<int,TH1F*> href;
  
  // Build the reference from 'infile2'
  std::cout << std::endl;
  std::cout << "***** Build reference *****" << std::endl;
  
  char histo_name[40];
  templateHisto = new TH1F("templateHisto", "", 1200, 0., 5.);


  for(int ientry = 0; ientry < ntu_MC->GetEntries(); ++ientry)
    {
      
      // use only even events to build templates
      if ( useEvenOdd==1  && ientry%2!=0) continue; 

      if( (ientry%100000 == 0) ) std::cout << "reading MC entry " << ientry << std::endl;
      ntu_MC->GetEntry(ientry);
      if (strcmp(EBEE,"EB")==0 && fabs(scEta) > 1.45) continue;                       // barrel
      if (strcmp(EBEE,"EE")==0 && (fabs(scEta) < 1.47 || fabs(scEta)>2.7 )) continue; // endcap
      
      int regionId = regionTemplate->EBRegionId(Ieta,Iphi);
      
      // fill the reference histograms
      std::map<int,TH1F*>::const_iterator itr = href.find(regionId);
   
      if (itr == href.end())
	{ 
	  sprintf(histo_name,"template_region%02d",regionId);
	  href[regionId] = new TH1F(histo_name,"",1200, 0., 3.);
	  href[regionId]->Fill(EoP);
	}
      else href[regionId]->Fill(EoP);
    }
  
    
  std::cout << "References built for " << EBEE <<  std::endl;  
  for (std::map<int,TH1F*>::const_iterator itr = href.begin(); itr != href.end(); ++itr)
    {
      std::cout << itr->first << "    -- events :" << (itr->second) -> GetEntries() << std::endl;
    }
  
  
  // Loop and sort events
  std::cout << std::endl;
  std::cout << "***** Sort events and define bins *****" << std::endl;
 
  int nEntries = ntu_DA -> GetEntriesFast(); 
  int nSavePts = 0; 
  std::vector<bool> isSavedEntries(nEntries);
     
  for(int ientry = 0; ientry < nEntries; ++ientry)  {
    ntu_DA -> GetEntry(ientry);
    isSavedEntries.at(ientry) = false;
       
   // save only what is needed for the analysis!!!
   if (strcmp(EBEE,"EB")==0 && fabs(scEta) > 1.45) continue;                       // barrel
   if (strcmp(EBEE,"EE")==0 && (fabs(scEta) < 1.47 || fabs(scEta)>2.7 )) continue; // endcap
  
   //if( fabs(scEta) > 0.44 ) continue;
   //if( fabs(scEta) < 0.44 || fabs(scEta) > 0.77 ) continue;
   //if( fabs(scEta) < 0.77 || fabs(scEta) > 1.10 ) continue;
   //if( fabs(scEta) < 1.10 || fabs(scEta) > 1.56 ) continue;
   //if( fabs(scEta) < 1.56 || fabs(scEta) > 2.00 ) continue;
   
   //if( fabs(scEta) > 0.77 ) continue;
   //if( fabs(scEta) < 0.77 || fabs(scEta) > 1.4442 ) continue;
   
   //if( (seedIeta > 0 && (seedIphi >= 100 && seedIphi < 121) ) ||
   //    (seedIeta < 0 && (seedIphi >= 160 && seedIphi < 180) ) ||
   //    (seedIeta < 0 && (seedIphi >= 300 && seedIphi < 320) ) )
   
   isSavedEntries.at(ientry) = true;
     
   nSavePts++;
  }
  std::cout << "Data sorted in " << EBEE << " - " << nSavePts << " events" << std::endl;
  
 
  
  // histogram definition
  TH2F* h_scale_EB  = new TH2F("h_scale_EB","", h_EBRegion->GetNbinsY()/region->EBRegionPhiWidth(),1.,361., h_EBRegion->GetNbinsX()/region->EBRegionEtaWidth(), -85., 86.);
  TH1F* h_EoP_spread = new TH1F("h_EoP_spread","",100,0.9,1.1);
     
  std::cout << std::endl;
  std::cout << "***** Fill and fit histograms *****" << std::endl;
  
  std::map<int,TH1F*> h_EoP;
  std::map<int,int> indref;

  // data definition
  std::vector< std::vector<double>* > dataEoP;
  for (int jbin = 0; jbin< region->EBregionsNum(); jbin++){
    dataEoP.push_back(new std::vector<double>);
  }

  for(int ientry = 0; ientry < nEntries; ++ientry)
    {
      // use only odd events to fit
      if ( useEvenOdd==1  && ientry%2!=1) continue; 

      if( (ientry%100000 == 0) ) std::cout << "reading entry " << ientry << std::endl;
      
      if( isSavedEntries.at(ientry) == false ) continue;
         
      ntu_DA->GetEntry(ientry);
      
      // fill the data histograms
      int regionId = region->EBRegionId(Ieta,Iphi);
      
      // matching ref regions  
      int regionIdRef = regionTemplate->EBRegionId(Ieta,Iphi);
      std::map<int,int>::const_iterator itr2 = indref.find(regionId);
      if (itr2 == indref.end()) indref[regionId] = regionIdRef;


      std::map<int,TH1F*>::const_iterator itr = h_EoP.find(regionId);
      if (itr == h_EoP.end())
	{ 
	  sprintf(histo_name,"EoP_region%02d",regionId);
	  h_EoP[regionId] = new TH1F(histo_name,histo_name,1200, 0., 3.);
	  h_EoP[regionId] -> SetFillColor(kGreen+2);
	  h_EoP[regionId] -> SetFillStyle(3004);
	  h_EoP[regionId] -> SetMarkerStyle(20);
	  h_EoP[regionId] -> SetMarkerSize(0.7);
	  h_EoP[regionId] -> SetMarkerColor(kGreen+2);
	  h_EoP[regionId] -> SetLineColor(kGreen+2);
	  h_EoP[regionId] -> Fill(EoP);
	}
      else h_EoP[regionId]->Fill(EoP);
      
      // fill te vectors data E/p 
      if (EoP > 0.5 && EoP < 1.5 ) dataEoP[regionId]->push_back(EoP);
      //dataEoP[regionId]->push_back(EoP);
    }
  
  // Define grpah and histograms
  TGraphAsymmErrors* gregion   = new TGraphAsymmErrors();
  TGraphAsymmErrors* gphi      = new TGraphAsymmErrors();
  TGraphAsymmErrors* gphi_EBP  = new TGraphAsymmErrors();
  TGraphAsymmErrors* gphi_EBM  = new TGraphAsymmErrors();
  int np = 0;
  int nm = 0;
  int nGoodFit  = 0;
  int nBadFit   = 0;
  int nTot = 0;

  // rebinning histos
  int rebin = 4;
  if (strcmp(EBEE,"EB")==0 ) rebin = 4; 
  for (std::map<int,TH1F*>::const_iterator itr = href.begin(); itr != href.end(); ++itr)
    {
      (itr->second) -> Rebin(rebin*4);
      
    }

  // define the fitting functions from templates
  std::map<int,TF1*> f_EoP;
  
  // AND NOW ... FIT!
  TVirtualFitter::SetDefaultFitter("Minuit2");
 
  TFitterMinuit* myfit = new TFitterMinuit(1);
  myfit->SetPrintLevel(-1);
  double arglist[2];
    
  
  cout << h_EoP.size() << endl;
 

  for (std::map<int,TH1F*>::const_iterator data_itr = h_EoP.begin(); data_itr != h_EoP.end(); ++data_itr)
    {
            
      (data_itr->second) -> Rebin(rebin*4);
     
      int idata = data_itr->first; 
      if( idata%100==0) cout << "Fitting region " << idata << endl;
  
      mydata = dataEoP.at(idata);

      // match actual data region with ref region
      std::map<int,int>::const_iterator index_itr = indref.find(idata);
      int iref = index_itr->second; 

      // define the fitting function  
      templateHisto = (TH1F*)href[iref]->Clone("templateHisto");
      FitTemplate(); 

      myfit->SetFCN(mylike);
      myfit->Clear();
      arglist[0] = 10000; // Max number of function calls
      arglist[1] = 1e-2;  // Tolerance on likelihood ?????????
      double step = h_EoP[idata]->GetRMS()/sqrt(h_EoP[idata]->GetEntries());
      myfit->SetParameter(0, "scale", 1.,step,0.50,1.50);
      int fStatus = myfit->ExecuteCommand("MIGRAD",arglist,2); 
     
      //if Fit did not converge, try with different step size, up to a maximum of 10 trials
      int ntrials = 1;
      while ( fStatus!=0 && ntrials < 10) {
	myfit->Clear();
	myfit->SetParameter(0, "scale", h_EoP[idata]->GetBinCenter(h_EoP[idata]->GetMaximumBin()),step*(ntrials+1),0.,0.);
	fStatus = myfit->ExecuteCommand("MIGRAD",arglist,2); 
	//cout << ntrials << " " << step*(ntrials+1) << fStatus << endl;
	ntrials++;
      }
      
      double amin,edm,errdef;
      int nvpar,nparx;
      myfit->GetStats(amin, edm, errdef, nvpar, nparx);
      double bestScale = myfit->GetParameter(0);
      double eee       = myfit->GetParError(0);

      double nEntriesInRegion =  (data_itr->second) ->GetEntries();

      if (nEntriesInRegion==0) cout << idata << " --> empty histo" << endl;
      
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
      f_EoP[idata] -> SetLineColor(kRed+2);  
      f_EoP[idata] -> SetLineWidth(1); 
      f_EoP[idata] -> SetParName(0,"Norm"); 
      f_EoP[idata] -> SetParName(1,"Scale factor"); 
      f_EoP[idata] -> SetNpx(10000);
      
      //fill the graph
      double aveEta = region->EBRegionEta(idata);
      double avePhi = region->EBRegionPhi(idata);
      nTot++;
      if ( fStatus==0 )
	{
	  nGoodFit++;
	  gregion -> SetPoint(nGoodFit,  idata , 1./bestScale);
	  gregion -> SetPointError(nGoodFit, 0 , 0, eee, eee);
	  gphi    -> SetPoint(nGoodFit,  avePhi , 1./bestScale);
	  gphi    -> SetPointError(nGoodFit, 0 , 0, eee, eee);
	  h_scale_EB -> Fill(avePhi,aveEta, 1./bestScale );
      	  h_EoP_spread -> Fill(1./bestScale);
	  
	  if ( aveEta > 0. ) {
	    gphi_EBP    -> SetPoint(np,  avePhi  , 1./bestScale);
	    gphi_EBP    -> SetPointError(np, 0. , 0., eee, eee);
	    np++;
	  }
	  
	  if ( aveEta < 0. ) {
	    gphi_EBM    -> SetPoint(nm,  avePhi  , 1./bestScale);
	    gphi_EBM    -> SetPointError(nm, 0. , 0., eee, eee);
	    nm++;
	  }
	  
	}
      else
	{
	  
	  std::cout << "Fitting region : " << idata << "   Fail status: " << fStatus << "   scale: " << bestScale <<"   sigma: " << eee << endl;
	  h_scale_EB -> Fill(avePhi,aveEta, 1.);
	  nBadFit++;
	}
    }


  cout << "Fraction of CONVERGED fits: " << nGoodFit << "/"<< nTot << " = " << (float)nGoodFit/(float)nTot<<endl;
  cout << "Fraction of NOT CONVERGED fits: " << nBadFit << "/"<< nTot << " = " << (float)nBadFit/(float)nTot<<endl;

  
  /////  // Drawings

  TCanvas *creference[5000]; 
  for(int i = 0; i < min(30,regionTemplate->EBregionsNum()); ++i)
    {
    char canvasName[50];
    if (i%6==0) {
      sprintf(canvasName, "template-%0d", i/6); 
      creference[i/6] = new TCanvas(canvasName, canvasName);
      creference[i/6] -> Divide(3,2);
    }
        
    creference[i/6] -> cd (i%6+1);
    href[i] -> GetXaxis() -> SetTitle("E/p");
    href[i] -> GetXaxis() -> SetRangeUser(0.3,1.7); 
    href[i] -> Draw("");
    gPad->Update();
  }
  
  TCanvas *cfit[5000]; 
  for(int i = 0; i < min(30,region->EBregionsNum()); ++i)
    {
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
    }

  
//   TF1 pol0("pol0","pol0",0,1000);
//   pol0.SetLineColor(kGreen+2);
//   pol0.SetLineWidth(2);
//   pol0.SetLineStyle(2);
//   gregion -> Fit("pol0","QR");
  
  TCanvas *c = new TCanvas("c","c");
  c -> cd();
  gregion->SetMarkerStyle(20);
  gregion->SetMarkerSize(0.8);
  gregion->SetMarkerColor(kGreen+2);
  gregion -> Draw("ap");
  gregion -> GetHistogram()->GetXaxis() ->SetTitle("region index");
  gregion -> GetHistogram()->GetYaxis() ->SetTitle("scale");
  gregion -> GetHistogram()->GetYaxis() ->SetRangeUser(0.8,1.2);

  TCanvas *cc = new TCanvas("cc","cc");
  cc -> cd();
  gphi_EBP->SetMarkerStyle(20);
  gphi_EBP->SetMarkerSize(0.8);
  gphi_EBP->SetMarkerColor(kGreen+2);
  gphi_EBM->SetMarkerStyle(24);
  gphi_EBM->SetMarkerSize(0.8);
  gphi_EBM->SetMarkerColor(kGreen+2);
  gphi_EBP -> Draw("ap");
  gphi_EBP  -> GetHistogram()->GetXaxis() ->SetTitle("#phi");
  gphi_EBP  -> GetHistogram()->GetYaxis() ->SetTitle("scale");
  gphi_EBP  -> GetHistogram()->GetXaxis() ->SetRangeUser(0.,361);
  gphi_EBP  -> GetHistogram()->GetYaxis() ->SetRangeUser(0.8,1.2);
  gphi_EBM -> Draw("psame");

  TCanvas *cmap = new TCanvas("cmap","cmap");
  cmap -> cd();
  h_scale_EB->Draw("COLZ");
  h_scale_EB->GetXaxis() ->SetTitle("#phi");
  h_scale_EB->GetYaxis() ->SetTitle("#eta");
  h_scale_EB->GetZaxis() ->SetRangeUser(0.85,1.15);


  // Final plots
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
  s_EoP_spread->SetX1NDC(0.49); //new x start position
  s_EoP_spread->SetX2NDC(0.99); //new x end position
  s_EoP_spread->SetY1NDC(0.750); //new x start position
  s_EoP_spread->SetY2NDC(0.875); //new x end position
  s_EoP_spread ->SetOptStat(1110);
  s_EoP_spread ->SetTextColor(kGreen+2);
  s_EoP_spread ->SetTextSize(0.05);
  s_EoP_spread -> Draw("sames");

  
//   TCanvas *cTemplate = new TCanvas("cTemplate","cTemplate");
//   cTemplate -> cd();
//   h_EBRegionTemplate -> Draw("text colz");
  
//   TCanvas *cRegion = new TCanvas("cRegion","cRegion");
//   cRegion -> cd();
//   h_EBRegion ->Draw("colz");

  
   
  TFile* o = new TFile("plots/TTscale/PhiRingTemplateData/checkTTscale_PhiRingTemplateData.root","RECREATE");
  o -> cd();
  gregion      -> Write("gregion");
  gphi         -> Write("gphi");
  gphi_EBP     -> Write("gphi");
  gphi_EBM     -> Write("gphi");
  h_EoP_spread -> Write("h_EoP_spread");
  h_scale_EB   -> Write("h_scale_EB_map");

  o -> Close();


  
}
