//per compilare
//c++  `root-config --libs --cflags --glibs` -lMinuit2 CheckRegionalScaleEB.cpp -o CheckRegionalScaleEB

//------------

#include "../histoFunc.h"
#include "./ntupleUtils.h"
#include "./ntupleUtils.cc"
#include "./ClusterCalibTools.h"
#include "./ClusterCalibTools.cc"

#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"
#include "TPaveStats.h"
#include "TLegend.h"
#include "TChain.h"
#include "TVirtualFitter.h"
#include "TFitterMinuit.h"
#include "TLatex.h"
#include "TMath.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <ctime>
#include <map>
#include <string>
#include <algorithm>
#include <math.h>
#include <vector>

TH1F* templateHisto;
TF1* templateFunc;
std::vector<double>* mydata;

int SMId(int eta, int phi)
{
  int id = -1;
  if (eta>0)
    id = phi%20 + 1;
  else 
    id = -(phi%20 + 1) ;

  return id;
} 


//-------------------------------------------------------------

int main (int argc, char** argv)
{

  int useEvenOdd = 0;  

  float eopmin = 0.5;
  float eopmax = 1.5;

 
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
      return 0;
    }
    
  
  //------ Set branch addresses
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

  
  //------ Build the references from 'infile2'
  std::cout << std::endl;
  std::cout << "***** Build reference *****" << std::endl;

  std::vector< std::vector<double>* > refEoP;
  for (int jbin = 0; jbin< regionTemplate->EBregionsNum(); jbin++){
    refEoP.push_back(new std::vector<double>);
  }

  std::map<int,TH1F*> href;
  char obj_name[100];
  for(int ientry = 0; ientry < ntu_MC->GetEntries(); ++ientry)
    {
      // use only even events to build templates
      if ( useEvenOdd==1  && ientry%2!=0) continue; 
      
      if( (ientry%100000 == 0) ) std::cout << "reading MC entry " << ientry << std::endl;
      ntu_MC->GetEntry(ientry);
      if (fabs(scEta) > 1.45) continue;                       // barrel
      
      // define the region and fill the reference histograms
      int regionId = regionTemplate->EBRegionId(Ieta,Iphi);
      if (regionId==-1) continue;
      std::map<int,TH1F*>::const_iterator itr = href.find(regionId);
      
      if (itr == href.end())
	{ 
	  sprintf(obj_name,"href_region%02d",regionId);
	  href[regionId] = new TH1F(obj_name,"",1200, 0., 3.);
	  href[regionId]->Fill(EoP);
	}
      else href[regionId]->Fill(EoP);
  
      if ( EoP > eopmin && EoP < eopmax ) {
	refEoP[regionId]->push_back(EoP);
      }
    }
  
  std::cout << "References built for EB :" <<  std::endl;  
  for (std::map<int,TH1F*>::const_iterator itr = href.begin(); itr != href.end(); ++itr)
    {
      std::cout << itr->first << "    -- events :" << (itr->second) -> GetEntries() << std::endl;
    }




  //------ DATA to fit from 'infile1'

  std::cout << std::endl;
  std::cout << "***** Fill histograms for DATA *****" << std::endl;

  std::map<int,TH1F*> h_EoP;
  std::map<int,int> indref;

  std::vector< std::vector<double>* > dataEoP;
  for (int jbin = 0; jbin< region->EBregionsNum(); jbin++){
    dataEoP.push_back(new std::vector<double>);
  }

  
  double* xbins = new double[360];
  double* ybins = new double[172];
  
  int nx = 0;
  for (int ii = 1; ii < 362; ii++){
    if ( region->EBRegionPhiWidth()!=1 && (ii % region->EBRegionPhiWidth()) == 1 ) { 
      xbins[nx]=ii; 
      nx++;
    }
    if (region->EBRegionPhiWidth()==1 ) {
      xbins[nx]=ii; 
      nx++;
    }
  }

  int ny = 0;
  for (int jj = -85; jj < 87; jj++){
    if ( (jj == -85) || (jj == 86) || region->EBRegionEtaWidth()== 1)  {
      ybins[ny]=jj;
      ny++;
    }
    if (region->EBRegionEtaWidth()!= 1 && abs(jj) % region->EBRegionEtaWidth() == 1) {
      ybins[ny]=jj;
      ny++;
    }
  }
  TH2F* h_occupancy_EB  = new TH2F("h_occupancy_EB","",nx-1, xbins, ny-1, ybins );
  
  // Loop over events
  int nEntries = ntu_DA -> GetEntriesFast(); 
  for(int ientry = 0; ientry < nEntries; ++ientry)
    {
      // use only odd events to fit
      if ( useEvenOdd==1  && ientry%2!=1) continue; 
      
      if( (ientry%100000 == 0) ) std::cout << "reading entry " << ientry << std::endl;
      
      ntu_DA->GetEntry(ientry);
      
      // save only what is needed for the analysis!!!
      if (fabs(scEta) > 1.45) continue;                       // barrel
   
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
      if ( EoP > eopmin && EoP < eopmax ) {
	dataEoP[regionId]->push_back(EoP);
	if (Ieta>0) h_occupancy_EB -> Fill( Iphi, Ieta);
 	if (Ieta<0) h_occupancy_EB -> Fill( Iphi, Ieta-1); // ????
      }
    }
  


  
  //------ Define graphs and histograms


  
  TH2F*     h_scale_EB  = new TH2F("h_scale_EB","",nx-1, xbins, ny-1, ybins);
  TH2F*     h_error_EB  = new TH2F("h_error_EB","",nx-1, xbins, ny-1, ybins );
  TProfile* p_scale_EB  = new TProfile("p_scale_EB","",ny-1, ybins, 0, 2);
  TProfile* p_error_EB  = new TProfile("p_error_EB","",ny-1, ybins, 0, 2);

  TH1F* h_EoP_spread = new TH1F("h_EoP_spread","",100,0.9,1.1);
  TH1F* h_EoP_spread_centralregion = new TH1F("h_EoP_spread_centralregion","",100,0.9,1.1);
  TH1F* h_EoP_spread_outerregion = new TH1F("h_EoP_spread_outerregion","",100,0.9,1.1);

  TH1F* h_EoP_spread_TBSM = new TH1F("h_EoP_spread_TBSM","",100,0.9,1.1);
  TH1F* h_EoP_spread_centralregion_TBSM = new TH1F("h_EoP_spread_centralregion_TBSM","",100,0.9,1.1);
  TH1F* h_EoP_spread_outerregion_TBSM = new TH1F("h_EoP_spread_outerregion_TBSM","",100,0.9,1.1);

  TH1F* h_EoP_error = new TH1F("h_EoP_error","",1000,0.,0.25);

  TGraphAsymmErrors* gregion   = new TGraphAsymmErrors();

  int nEtaRegions = regionTemplate->EBregionsNum();
  TGraphAsymmErrors* gphi[nEtaRegions]; 
  std::vector<int> np;
  np.clear();
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
  
  TVirtualFitter::SetDefaultFitter("Minuit2");
  TFitterMinuit* myfit = new TFitterMinuit(1);
  myfit->SetPrintLevel(-1);
  double arglist[2];

  // rebinning histos
  int rebin = 4;
    
  //------ define the fitting functions from templates
  templateHisto = new TH1F("templateHisto", "", 1200, 0., 5.);
  std::map<int,TF1*> fref;
  std::map<int,double> refScale;

  for (unsigned int ii = 0; ii < href.size();ii++){
    href[ii] -> Rebin(rebin);
    templateHisto = (TH1F*)href[ii]->Clone("templateHisto");
    FitTemplate();       
    sprintf(obj_name,"fref_%d", ii );
    fref[ii] = new TF1(obj_name, templateFunc,0,10,8);
    fref[ii] -> SetNpx(10000);
    fref[ii] -> SetLineColor(2);
    fref[ii] -> SetLineWidth(1);
    for(int parIt = 0; parIt < 8; ++parIt){
      fref[ii] -> FixParameter(parIt,templateFunc->GetParameter(parIt));
    }
     
    //-----        
    mydata = refEoP.at(ii);
    templateFunc = fref[ii];
    myfit->SetFCN(mylike);
    myfit->Clear();
    arglist[0] = 10000; // Max number of function calls
    arglist[1] = 1e-5;  // Tolerance on likelihood ?????????
    double step = href[ii]->GetRMS()/sqrt(href[ii]->GetEntries());
    myfit->SetParameter(0, "scale", 1.,step,0.,0);
    int fStatus = myfit->ExecuteCommand("MIGRAD",arglist,2); 
    refScale[ii]= myfit->GetParameter(0);
  }

    
  //------ FIT DATA
  std::cout << std::endl;
  std::cout << "***** Start fitting ... *****" << std::endl;  

  std::map<int,TF1*> f_EoP;

  for (std::map<int,TH1F*>::const_iterator data_itr = h_EoP.begin(); data_itr != h_EoP.end(); ++data_itr)
    {
      (data_itr->second) -> Rebin(rebin*4);
      
      int idata = data_itr->first; 
      
      if( idata%100==0) 
	std::cout << "Fitting region " << idata << std::endl;
      
      mydata = dataEoP.at(idata);
      
      // match actual data region with ref region
      std::map<int,int>::const_iterator index_itr = indref.find(idata);
      int iref = index_itr->second; 

      // define the fitting function  
      templateFunc = fref[iref];
      
      myfit->SetFCN(mylike);
      myfit->Clear();
      arglist[0] = 10000; // Max number of function calls
      arglist[1] = 1e-5;  // Tolerance on likelihood ?????????
      double step = h_EoP[idata]->GetRMS()/sqrt(h_EoP[idata]->GetEntries());
      myfit->SetParameter(0, "scale", 1.,step,0.,0);
      int fStatus = myfit->ExecuteCommand("MIGRAD",arglist,2); 
 
      //if Fit did not converge, try with different step size, up to a maximum of 10 trials
      int ntrials = 1;
      while ( fStatus!=0 && ntrials < 10) {
	myfit->Clear();
	myfit->SetParameter(0, "scale", h_EoP[idata]->GetBinCenter(h_EoP[idata]->GetMaximumBin()),step*(ntrials+1),0.,0);
	fStatus = myfit->ExecuteCommand("MIGRAD",arglist,2); 
	ntrials++;
      }
      
      double amin,edm,errdef;
      int nvpar,nparx;
      myfit->GetStats(amin, edm, errdef, nvpar, nparx);
      double bestScale = myfit->GetParameter(0);
      bestScale/=refScale[iref]; 
      double eee       = myfit->GetParError(0);

      double xNorm = h_EoP[idata]->GetMaximum();
                 
      char funcName[50];
      sprintf(funcName,"f_EoP_%d", idata );
      f_EoP[idata] =  new TF1(funcName,templateFunc,0.,10.,8);
      f_EoP[idata] -> SetParameter(0,xNorm);
      f_EoP[idata] -> FixParameter(1,templateFunc->GetParameter(1));
      f_EoP[idata] -> FixParameter(2,templateFunc->GetParameter(2));
      f_EoP[idata] -> FixParameter(3,templateFunc->GetParameter(3));
      f_EoP[idata] -> FixParameter(4,templateFunc->GetParameter(4));
      f_EoP[idata] -> FixParameter(5,templateFunc->GetParameter(5));
      f_EoP[idata] -> FixParameter(6,templateFunc->GetParameter(6));
      f_EoP[idata] -> FixParameter(7,bestScale);
      f_EoP[idata] -> SetParError(7,eee);
      f_EoP[idata] -> SetLineColor(kRed+2);  
      f_EoP[idata] -> SetLineWidth(1); 
      f_EoP[idata] -> SetParName(0,"Norm"); 
      f_EoP[idata] -> SetParName(1,"Scale factor"); 
      f_EoP[idata] -> SetNpx(10000);
      h_EoP[idata] -> Fit(f_EoP[idata],"QSR" );
      f_EoP[idata] -> SetParError(7,eee);

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

	  // central or outer region
	  if ( fabs(aveEta) <= 40 ) h_EoP_spread_centralregion -> Fill(bestScale);
	  if ( fabs(aveEta) > 40  ) h_EoP_spread_outerregion   -> Fill(bestScale);

	  // modules exposed to TB
	  int smid = SMId((int)aveEta, (int)avePhi);
	  if ( smid == 1  || smid == 2 || smid == 10 || 
	       smid == 11 || smid == 15 || 
	       smid == -1 || smid == -2 ||
	       smid == -3 || smid == -6) {
	    
	    h_EoP_spread_TBSM -> Fill(bestScale);
	    if ( fabs(aveEta) <= 40 ) h_EoP_spread_centralregion_TBSM -> Fill(bestScale);
	    if ( fabs(aveEta) > 40  ) h_EoP_spread_outerregion_TBSM   -> Fill(bestScale);

	  }
	  
	  h_EoP_error  -> Fill(eee);
	  h_scale_EB   -> Fill(avePhi,aveEta, bestScale );
	  h_error_EB   -> Fill(avePhi,aveEta, eee );
	  p_scale_EB   -> Fill(aveEta, bestScale );
	  p_error_EB   -> Fill(aveEta, eee );

	  gphi[iref] -> SetPoint(np[iref],  avePhi  , bestScale);
 	  gphi[iref] -> SetPointError(np[iref], 0. , 0., eee, eee);
 	  np[iref]++;

	  
	}
      else
	{
	  std::cout << "Fitting region : " << idata << "   Fail status: " << fStatus 
		    << "   scale: "        << bestScale <<"   sigma: " << eee << std::endl;
	  h_scale_EB -> Fill(avePhi,aveEta, 1.);
	  nBadFit++;
	}
        
    }
 

  std::cout << "Fraction of CONVERGED fits: " << nGoodFit << "/"<< nTot << " = " << (float)nGoodFit/(float)nTot<<std::endl;
  std::cout << "Fraction of NOT CONVERGED fits: " << nBadFit << "/"<< nTot << " = " << (float)nBadFit/(float)nTot<<std::endl;


  delete  xbins;
  delete  ybins;

  // save plots in a root file  
  //std::string dirname("./plots/TTscale/May10ReReco_plus_PromptReco_2011Laser_Bon_V20110616_weightedAverage/");
  //std::string dirname("./plots/TTscale/May10ReReco_plus_PromptReco_2011Laser_Bon_V20110616_weightedAverage_TemplateOddEvents/");
  std::string dirname("./plots/XtalScale/May10ReReco_plus_PromptReco_2011Laser_Bon_V20110616_weightedAverage/");

  std::string outfilename("checkScale.root");
  
  
  TFile* out = new TFile((dirname+outfilename).c_str(),"RECREATE");

  TDirectory *data      =   out -> mkdir("data","data");
  data->cd();
  for (std::map<int,TH1F*>::const_iterator itr = h_EoP.begin(); itr != h_EoP.end(); ++itr){
    h_EoP[itr->first] -> Write(); 
    f_EoP[itr->first] -> Write(); 
  }
   
  out -> cd();
  TDirectory *reference =   out -> mkdir("reference","reference");
  reference->cd();
  for (std::map<int,TH1F*>::const_iterator itr = href.begin(); itr != href.end(); ++itr){
    //href[itr->first] -> Fit(fref[itr->first],"QRS"); 
    href[itr->first] -> Write(); 
    fref[itr->first] -> Write(); 
  }
  
  out -> cd();
  for (int i=0; i< nEtaRegions; i++){
    gphi[i]     -> Write();
  }

  gregion      -> Write("gregion");
  
  h_EoP_spread               -> Write("h_EoP_spread");
  h_EoP_spread_centralregion -> Write("h_EoP_spread_centralregion");
  h_EoP_spread_outerregion   -> Write("h_EoP_spread_outerregion");

  h_EoP_error      -> Write("h_EoP_error");
  
  h_scale_EB       -> Write("h_scale_map");
  h_error_EB       -> Write("h_error_map");
  p_error_EB       -> Write("h_error_profile_vs_ieta");
  p_scale_EB       -> Write("h_scale_profile_vs_ieta");
  h_occupancy_EB   -> Write("h_occupancy");

  out -> Close();

  return 0;
  
}
