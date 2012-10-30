// g++ -Wall -o drawRegionAlphaPlots `root-config --cflags --glibs` -L/gwteraw/cmssw/slc5_amd64_gcc462/external/gcc/4.6.2/lib64/ setTDRStyle.cc ntupleUtils.cc stabilityUtils.cc geometryUtils.cc drawRegionAlphaPlots.cpp

#include "setTDRStyle.h"
#include "ntupleUtils.h"
#include "stabilityUtils.h"
#include "geometryUtils.h"

#include <iostream>
#include <cstdlib>

#include "TFile.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TH2F.h"
#include "TLatex.h"

int t1 = 1325376001;   //  1 Jan 2012
int t2 = 1356134399;   // 31 Dec 2012

TH1F* templateHisto;
TF1* templateFunc;
std::vector<double>* mydata;



int main(int argc, char** argv)
{
  setTDRStyle();
  gStyle -> SetPadRightMargin(0.18);
  
  std::cout << "\n***************************************************************************************************************************" << std::endl;
  std::cout << "arcg: " << argc << std::endl;
  char* EBEE = argv[1];
  std::string regionType = std::string(argv[2]);
  std::string dayMin = "";
  std::string dayMax = "";
  std::string dayMinLabel = "";
  std::string dayMaxLabel = "";
  
  if(argc >= 4)
  {
    dayMin = std::string(argv[3])+" "+std::string(argv[4])+" "+std::string(argv[5]);
    dayMax = std::string(argv[6])+" "+std::string(argv[7])+" "+std::string(argv[8]);
    dayMinLabel = std::string(argv[3])+"_"+std::string(argv[4])+"_"+std::string(argv[5]);
    dayMaxLabel = std::string(argv[6])+"_"+std::string(argv[7])+"_"+std::string(argv[8]);
    
    t1 = dateToInt(dayMin);
    t2 = dateToInt(dayMax);
  }
  
  std::cout << "EBEE: "          << EBEE          << std::endl;
  std::cout << "regionType: "    << regionType    << std::endl;
  std::cout << "dayMin: "        << dayMin        << std::endl;
  std::cout << "dayMax: "        << dayMax        << std::endl;
  
  
  
  //-------------------
  // Define in/outfiles
  
  std::string folderName = std::string(EBEE) + "_1_1_2012_31_12_2012";
  TFile* inFile = TFile::Open((folderName+"/"+folderName+"_"+regionType+"_histos.root").c_str(),"READ");


  std::string alphaName = std::string(EBEE) + "_" + dayMinLabel + "_" + dayMaxLabel + "_"+regionType;
  TFile* outFile = TFile::Open((folderName+"/"+alphaName+"_alphaHistos.root").c_str(),"RECREATE");
  
  
  
  //--------------
  //define regions
  
  TBarrelRegions* ebRegion;
  TEndcapRegions* eeRegion;
  int nRegions = 0;
  
  if( strcmp(EBEE,"EB") == 0 )
  {
    ebRegion = new TBarrelRegions();
    nRegions = ebRegion->GetNRegions(regionType);
  }
  
  if( strcmp(EBEE,"EE") == 0 )
  {
    eeRegion = new TEndcapRegions();
    //nRegions = eeRegion->GetNRegions(regionType);
  }
  
  
  
  //-----------------
  // Build alpha plot
  
  std::map<int,TH1F*> h_EoP_alpha_spread;
  std::map<int,TH1F*> h_EoC_alpha_spread;
  
  std::map<int,TH2F*> h2_EoP_alpha;
  std::map<int,TH2F*> h2_EoC_alpha;
  
  std::map<int,TGraphErrors*> g_EoP_alpha_vsRegionId;
  std::map<int,TGraphErrors*> g_EoC_alpha_vsRegionId;
  
  std::map<int,TGraphErrors*> g_EoP_alpha;
  std::map<int,TGraphErrors*> g_EoC_alpha;
  
  if( strcmp(EBEE,"EB") == 0 )
  {
    h_EoP_alpha_spread[0] = new TH1F("h_EoP_alpha_spread_EB","",100,1.00,2.00);
    h_EoC_alpha_spread[0] = new TH1F("h_EoC_alpha_spread_EB","",100,1.00,2.00);
    
    g_EoP_alpha_vsRegionId[0] = new TGraphErrors();
    g_EoC_alpha_vsRegionId[0] = new TGraphErrors();
    
    h2_EoP_alpha[0] = new TH2F("h2_EoP_EB","",171,-85.,86.,360,1.,361.);
    h2_EoC_alpha[0] = new TH2F("h2_EoC_EB","",171,-85.,86.,360,1.,361.);
  }
  
  if( strcmp(EBEE,"EE") == 0 )
  {
    h2_EoP_alpha[-1] = new TH2F("h2_EoP_EEM","",100,1.,101.,100,1.,100.);
    h2_EoC_alpha[-1] = new TH2F("h2_EoC_EEM","",100,1.,101.,100,1.,100.);
    
    h2_EoP_alpha[1]  = new TH2F("h2_EoP_EEP","",100,1.,101.,100,1.,100.);
    h2_EoC_alpha[1]  = new TH2F("h2_EoC_EEP","",100,1.,101.,100,1.,100.);
  }
  
  
  
  std::map<int,float> alpha_EoP;
  std::map<int,float> alphaErr_EoP;
  std::map<int,float> alpha_EoC;
  std::map<int,float> alphaErr_EoC;
  
  for(int iRegion = 0; iRegion < nRegions; ++iRegion)
  {
    std::cout << ">>> region " << iRegion << " / " << nRegions << std::endl;
    
    
    char regionChar[50];
    sprintf(regionChar,"%d",iRegion);
    std::string regionString(regionChar);
    
    g_EoP_alpha[iRegion] = new TGraphErrors();
    g_EoC_alpha[iRegion] = new TGraphErrors();
    
    char graphName[50];
    
    sprintf(graphName,"region_%d/g_LT_%d",iRegion,iRegion);
    TGraphErrors* g_LT = (TGraphErrors*)( inFile->Get(graphName) );
    sprintf(graphName,"region_%d/g_fit_%d",iRegion,iRegion);
    TGraphAsymmErrors* g_fit = (TGraphAsymmErrors*)( inFile->Get(graphName) );
    sprintf(graphName,"region_%d/g_c_fit_%d",iRegion,iRegion);
    TGraphAsymmErrors* g_c_fit = (TGraphAsymmErrors*)( inFile->Get(graphName) );
    sprintf(graphName,"region_%d/g_c_fit_run_%d",iRegion,iRegion);
    TGraphAsymmErrors* g_c_fit_run = (TGraphAsymmErrors*)( inFile->Get(graphName) );
    
    double time,time2;
    double run;
    double LT,LTErr;
    double EoP,EoPErr;
    double EoC,EoCErr;
    
    int savedPoints = 0;
    for(int point = 0; point < g_LT->GetN(); ++point)
    {
      g_LT -> GetPoint(point,time,LT);
      LTErr = g_LT -> GetErrorY(point);
      
      bool found = false;
      for(int point2 = 0; point2 < g_fit->GetN(); ++point2)
      {
        g_fit       -> GetPoint(point2,time2,EoP);
        g_c_fit     -> GetPoint(point2,time2,EoC);
        g_c_fit_run -> GetPoint(point2,run, EoC);
        
        EoPErr = g_fit   -> GetErrorY(point);
        EoCErr = g_c_fit -> GetErrorY(point);
        
        if( time2 == time)
        {
          found = true;
          break;
        }
      }
      
      if( found == false ) continue;
      if( run == 201191. ) continue;
      if( EoP > 1. ) continue;
      if( (EoP <= 0.) || (EoC <= 0.) ) continue;
      if( (EoPErr > 0.01) || (EoCErr > 0.01) ) continue;
      if( (EoPErr < 0.0002) || (EoCErr < 0.0002) ) continue;
      if( (time < t1) || (time > t2) ) continue;
      
      //std::cout << ">>> getting point " << point << std::endl;
      //std::cout << ">>>>>> time: " << time
      //          << "   LT: "  << LT
      //          << "   EoP: " << EoP
      //          << "   EoC: " << EoC
      //          << std::endl;
      
      g_EoP_alpha[iRegion] -> SetPoint(savedPoints,LT,log(EoP));
      g_EoC_alpha[iRegion] -> SetPoint(savedPoints,LT,log(EoC));
      g_EoP_alpha[iRegion] -> SetPointError(savedPoints,LTErr,EoPErr/EoP);
      g_EoC_alpha[iRegion] -> SetPointError(savedPoints,LTErr,EoCErr/EoC);
      
      ++savedPoints;
    }
    
    g_EoP_alpha[iRegion] -> SetPoint(g_EoP_alpha[iRegion]->GetN(),-1.,0.);
    g_EoP_alpha[iRegion] -> SetPoint(g_EoP_alpha[iRegion]->GetN(), 1.,0.);
    g_EoC_alpha[iRegion] -> SetPoint(g_EoC_alpha[iRegion]->GetN(),-1.,0.);
    g_EoC_alpha[iRegion] -> SetPoint(g_EoC_alpha[iRegion]->GetN(), 1.,0.);
    
    
    
    //----------------
    // Draw alpha plot
    
    TCanvas* c1 = new TCanvas();
    c1 -> cd();
    c1 -> SetGridx();
    c1 -> SetGridy();
    
    g_EoP_alpha[iRegion] -> SetMarkerStyle(24);
    g_EoP_alpha[iRegion] -> SetMarkerSize(0.7);
    g_EoP_alpha[iRegion] -> SetMarkerColor(kRed+2);
    g_EoP_alpha[iRegion] -> SetLineColor(kRed+2);
    g_EoP_alpha[iRegion] -> GetXaxis() -> SetNdivisions(510);
    g_EoP_alpha[iRegion] -> GetXaxis() -> SetLabelSize(0.04);
    g_EoP_alpha[iRegion] -> GetYaxis() -> SetLabelSize(0.04);
    g_EoP_alpha[iRegion] -> GetXaxis() -> SetTitle("#LT log [ R/R_{0} ] #GT");
    g_EoP_alpha[iRegion] -> GetYaxis() -> SetTitle("log [ E/p relative scale ]");
    g_EoP_alpha[iRegion] -> GetYaxis() -> SetTitleOffset(1.10);
    if( strcmp(EBEE,"EB") == 0 )
    {
      g_EoP_alpha[iRegion] -> GetXaxis() -> SetRangeUser(-0.060,0.015);
      g_EoP_alpha[iRegion] -> GetYaxis() -> SetRangeUser(-0.080,0.020);
    }
    else
    {
      g_EoP_alpha[iRegion] -> GetXaxis() -> SetRangeUser(-0.300,0.075);
      g_EoP_alpha[iRegion] -> GetYaxis() -> SetRangeUser(-0.400,0.100);
    }
    
    g_EoC_alpha[iRegion] -> SetMarkerStyle(20);
    g_EoC_alpha[iRegion] -> SetMarkerSize(0.7);
    g_EoC_alpha[iRegion] -> SetMarkerColor(kGreen+2);
    g_EoC_alpha[iRegion] -> SetLineColor(kGreen+2);
    g_EoC_alpha[iRegion] -> GetXaxis() -> SetNdivisions(510);
    g_EoC_alpha[iRegion] -> GetXaxis() -> SetLabelSize(0.04);
    g_EoC_alpha[iRegion] -> GetYaxis() -> SetLabelSize(0.04);
    g_EoC_alpha[iRegion] -> GetXaxis() -> SetTitle("#LT log [ R/R_{0} ] #GT");
    g_EoC_alpha[iRegion] -> GetYaxis() -> SetTitle("log [ E/p relative scale ]");
    g_EoC_alpha[iRegion] -> GetYaxis() -> SetTitleOffset(1.10);
    if( strcmp(EBEE,"EB") == 0 )
    {
      g_EoC_alpha[iRegion] -> GetXaxis() -> SetRangeUser(-0.060,0.015);
      g_EoC_alpha[iRegion] -> GetYaxis() -> SetRangeUser(-0.080,0.020);
    }
    else
    {
      g_EoC_alpha[iRegion] -> GetXaxis() -> SetRangeUser(-0.300,0.075);
      g_EoC_alpha[iRegion] -> GetYaxis() -> SetRangeUser(-0.400,0.100);
    }
    
    TF1* f_EoP = new TF1("f_EoP","[0]+[1]*x",-0.500,0.);
    f_EoP -> SetLineColor(kRed+2);
    f_EoP -> SetLineStyle(7);
    f_EoP -> SetLineWidth(2);
    if( strcmp(EBEE,"EB") == 0 ) f_EoP -> SetParameters(0.,1.52);
    else                         f_EoP -> SetParameters(0.,1.16);
    if( strcmp(EBEE,"EB") == 0 ) g_EoP_alpha[iRegion] -> Fit("f_EoP","QNS+","",-0.050,-0.010);
    else                         g_EoP_alpha[iRegion] -> Fit("f_EoP","QNS+","",-0.300,-0.050);
    alpha_EoP[iRegion] = f_EoP->GetParameter(1);
    alphaErr_EoP[iRegion] = f_EoP->GetParError(1);
    
    
    
    TF1* f_EoC = new TF1("f_EoC","[0]+[1]*x",-0.500,0.);
    f_EoC -> SetLineColor(kGreen+2);
    f_EoC -> SetLineStyle(7);
    f_EoC -> SetLineWidth(2);
    f_EoC -> SetParameters(0.,0.);
    if( strcmp(EBEE,"EB") == 0 ) g_EoC_alpha[iRegion] -> Fit("f_EoC","QNS+","",-0.050,-0.010);
    else                         g_EoC_alpha[iRegion] -> Fit("f_EoC","QNS+","",-0.300,-0.050);
    if( strcmp(EBEE,"EB") == 0 ) alpha_EoC[iRegion] = 1.52 + f_EoC->GetParameter(1);
    else                         alpha_EoC[iRegion] = 1.16 + f_EoC->GetParameter(1);
    alphaErr_EoC[iRegion] = f_EoC->GetParError(1);
    
    char latexBuffer[250];
    
    sprintf(latexBuffer,"#alpha = %1.2f #pm %1.2f",f_EoP->GetParameter(1),f_EoP->GetParError(1));
    TLatex* latex_EoP = new TLatex(0.47,0.22,latexBuffer);
    latex_EoP -> SetNDC();
    latex_EoP -> SetTextFont(42);
    latex_EoP -> SetTextSize(0.05);
    latex_EoP -> SetTextColor(kRed+2);
    
    if( strcmp(EBEE,"EB") == 0 ) sprintf(latexBuffer,"#alpha = %1.2f #pm %1.2f",1.52+f_EoC->GetParameter(1),f_EoC->GetParError(1));
    else                         sprintf(latexBuffer,"#alpha = %1.2f #pm %1.2f",1.16+f_EoC->GetParameter(1),f_EoC->GetParError(1));
    TLatex* latex_EoC = new TLatex(0.47,0.32,latexBuffer);
    latex_EoC -> SetNDC();
    latex_EoC -> SetTextFont(42);
    latex_EoC -> SetTextSize(0.05);
    latex_EoC -> SetTextColor(kGreen+2);
    
    
    g_EoP_alpha[iRegion] -> Draw("AP");  
    g_EoC_alpha[iRegion] -> Draw("P,same");
    f_EoP -> Draw("same");
    f_EoC -> Draw("same");
    latex_EoP -> Draw("same");
    latex_EoC -> Draw("same");
    
    
    
    //----------------
    // Save alpha plot
    
    std::string alphaName2 = std::string(EBEE) + "_" + dayMinLabel + "_" + dayMaxLabel + "_"+regionType+"_alphaPlot_" + regionString;
    
    c1 -> Print((folderName+"/"+alphaName2+".pdf").c_str(),"pdf");
    c1 -> Print((folderName+"/"+alphaName2+".png").c_str(),"png");
    c1 -> SaveAs((folderName+"/"+alphaName2+".C").c_str());
    
    outFile -> cd();
    
    sprintf(graphName,"g_EoP_alpha_%d",iRegion);
    g_EoP_alpha[iRegion] -> Write(graphName);
    
    sprintf(graphName,"g_EoC_alpha_%d",iRegion);
    g_EoC_alpha[iRegion] -> Write(graphName);
  }
  
  
  
  if( strcmp(EBEE,"EB") == 0 )
  {
    g_EoP_alpha_vsRegionId[0] -> SetPoint(0,-100.,-1.);
    g_EoP_alpha_vsRegionId[0] -> SetPointError(0,0.,0.);
    
    g_EoC_alpha_vsRegionId[0] -> SetPoint(0,-100.,-1.);
    g_EoC_alpha_vsRegionId[0] -> SetPointError(0,0.,0.);
    
    for(int iRegion = 0; iRegion < nRegions; ++iRegion)
    {
      h_EoP_alpha_spread[0] -> Fill(alpha_EoP[iRegion]);
      h_EoC_alpha_spread[0] -> Fill(alpha_EoC[iRegion]);
      
      g_EoP_alpha_vsRegionId[0] -> SetPoint(iRegion+1,iRegion,alpha_EoP[iRegion]);
      g_EoP_alpha_vsRegionId[0] -> SetPointError(iRegion+1,0.,alphaErr_EoP[iRegion]);
      
      g_EoC_alpha_vsRegionId[0] -> SetPoint(iRegion+1,iRegion,alpha_EoC[iRegion]);
      g_EoC_alpha_vsRegionId[0] -> SetPointError(iRegion+1,0.,alphaErr_EoC[iRegion]);
    }
    
    g_EoP_alpha_vsRegionId[0] -> SetPoint(nRegions+1,nRegions+100,-1.);
    g_EoP_alpha_vsRegionId[0] -> SetPointError(nRegions+1,0.,0.);
    
    g_EoC_alpha_vsRegionId[0] -> SetPoint(nRegions+1,nRegions+100,-1.);
    g_EoC_alpha_vsRegionId[0] -> SetPointError(nRegions+1,0.,0.);
    
    
    for(int iBin = 1; iBin <= h2_EoP_alpha[0]->GetNbinsX(); ++iBin)
      for(int jBin = 1; jBin <= h2_EoP_alpha[0]->GetNbinsY(); ++jBin)
      {
        int iEta = int( h2_EoP_alpha[0]->GetXaxis()->GetBinLowEdge(iBin) );
        int iPhi = int( h2_EoP_alpha[0]->GetYaxis()->GetBinLowEdge(jBin) );
        int regionId = ebRegion -> GetRegionId(iEta,iPhi,regionType);
        
        h2_EoP_alpha[0] -> SetBinContent(iBin,jBin,alpha_EoP[regionId]);
        h2_EoC_alpha[0] -> SetBinContent(iBin,jBin,alpha_EoC[regionId]);
      }
  }
  
  
  
  //----------
  // Draw maps
  
  outFile -> cd();
  
  TCanvas* c1_spread = new TCanvas();
  c1_spread -> cd();
  c1_spread -> SetGridx();
  c1_spread -> SetGridy();
  
  if( strcmp(EBEE,"EB") == 0 )
  {
    h_EoP_alpha_spread[0] -> GetXaxis() -> SetTitle("#alpha from uncorrected E/p scale");
    h_EoP_alpha_spread[0] -> GetYaxis() -> SetTitle("entries");
    h_EoP_alpha_spread[0] -> GetXaxis() -> SetLabelSize(0.04);
    h_EoP_alpha_spread[0] -> GetYaxis() -> SetLabelSize(0.04);
    h_EoP_alpha_spread[0] -> GetXaxis() -> SetTitleSize(0.05);
    h_EoP_alpha_spread[0] -> GetYaxis() -> SetTitleSize(0.05);
    h_EoP_alpha_spread[0] -> Draw();
    h_EoP_alpha_spread[0] -> Write();
    
    char latexBuffer[250];
    
    sprintf(latexBuffer,"mean = %1.2e",h_EoP_alpha_spread[0]->GetMean());
    TLatex* latex_mean = new TLatex(0.17,0.85,latexBuffer);
    latex_mean -> SetNDC();
    latex_mean -> SetTextFont(42);
    latex_mean -> SetTextSize(0.05);
    latex_mean -> Draw("same");
    
    sprintf(latexBuffer," RMS = %1.2e",h_EoP_alpha_spread[0]->GetRMS());
    TLatex* latex_RMS = new TLatex(0.17,0.75,latexBuffer);
    latex_RMS -> SetNDC();
    latex_RMS -> SetTextFont(42);
    latex_RMS -> SetTextSize(0.05);
    latex_RMS -> Draw("same");
  }
  
  TCanvas* c1_vsRegionId = new TCanvas();
  c1_vsRegionId -> cd();
  c1_vsRegionId -> SetGridx();
  c1_vsRegionId -> SetGridy();
  
  if( strcmp(EBEE,"EB") == 0 )
  {
    g_EoP_alpha_vsRegionId[0] -> GetXaxis() -> SetTitle("region ID");
    g_EoP_alpha_vsRegionId[0] -> GetYaxis() -> SetTitle("#alpha from uncorrected E/p scale");
    g_EoP_alpha_vsRegionId[0] -> GetXaxis() -> SetRangeUser(-1,nRegions);
    g_EoP_alpha_vsRegionId[0] -> SetMinimum(1.00);
    g_EoP_alpha_vsRegionId[0] -> SetMaximum(2.00);
    g_EoP_alpha_vsRegionId[0] -> GetXaxis() -> SetLabelSize(0.04);
    g_EoP_alpha_vsRegionId[0] -> GetYaxis() -> SetLabelSize(0.04);
    g_EoP_alpha_vsRegionId[0] -> GetXaxis() -> SetTitleSize(0.05);
    g_EoP_alpha_vsRegionId[0] -> GetYaxis() -> SetTitleSize(0.05);
    g_EoP_alpha_vsRegionId[0] -> Draw("AP");
    g_EoP_alpha_vsRegionId[0] -> Write("gEB_EoP_alpha_vsRegionId");
  }
  
  TCanvas* c1_map = new TCanvas();
  c1_map -> cd();
  c1_map -> SetGridx();
  c1_map -> SetGridy();
    
  if( strcmp(EBEE,"EB") == 0 )
  {
    h2_EoP_alpha[0] -> GetXaxis() -> SetTitle("i#eta");
    h2_EoP_alpha[0] -> GetYaxis() -> SetTitle("i#phi");
    h2_EoP_alpha[0] -> GetZaxis() -> SetTitle("#alpha from uncorrected E/p scale");
    h2_EoP_alpha[0] -> GetZaxis() -> SetRangeUser(1.00,2.00);
    h2_EoP_alpha[0] -> GetZaxis() -> SetTitleOffset(1.25);
    h2_EoP_alpha[0] -> GetXaxis() -> SetLabelSize(0.04);
    h2_EoP_alpha[0] -> GetYaxis() -> SetLabelSize(0.04);
    h2_EoP_alpha[0] -> GetZaxis() -> SetLabelSize(0.04);
    h2_EoP_alpha[0] -> GetXaxis() -> SetTitleSize(0.05);
    h2_EoP_alpha[0] -> GetYaxis() -> SetTitleSize(0.05);
    h2_EoP_alpha[0] -> GetZaxis() -> SetTitleSize(0.05);
    h2_EoP_alpha[0] -> Draw("COLZ");
    h2_EoP_alpha[0] -> Write();
  }
  
  
  
  TCanvas* c2_spread = new TCanvas();
  c2_spread -> cd();
  c2_spread -> SetGridx();
  c2_spread -> SetGridy();
    
  if( strcmp(EBEE,"EB") == 0 )
  {
    h_EoC_alpha_spread[0] -> GetXaxis() -> SetTitle("#alpha from corrected E/p scale");
    h_EoC_alpha_spread[0] -> GetYaxis() -> SetTitle("entries");
    h_EoC_alpha_spread[0] -> GetXaxis() -> SetLabelSize(0.04);
    h_EoC_alpha_spread[0] -> GetYaxis() -> SetLabelSize(0.04);
    h_EoC_alpha_spread[0] -> GetXaxis() -> SetTitleSize(0.05);
    h_EoC_alpha_spread[0] -> GetYaxis() -> SetTitleSize(0.05);
    h_EoC_alpha_spread[0] -> Draw();
    h_EoC_alpha_spread[0] -> Write();
        
    char latexBuffer[250];
    
    sprintf(latexBuffer,"mean = %1.2e",h_EoC_alpha_spread[0]->GetMean());
    TLatex* latex_mean = new TLatex(0.17,0.85,latexBuffer);
    latex_mean -> SetNDC();
    latex_mean -> SetTextFont(42);
    latex_mean -> SetTextSize(0.05);
    latex_mean -> Draw("same");
    
    sprintf(latexBuffer," RMS = %1.2e",h_EoC_alpha_spread[0]->GetRMS());
    TLatex* latex_RMS = new TLatex(0.17,0.75,latexBuffer);
    latex_RMS -> SetNDC();
    latex_RMS -> SetTextFont(42);
    latex_RMS -> SetTextSize(0.05);
    latex_RMS -> Draw("same");
  }
  
  TCanvas* c2_vsRegionId = new TCanvas();
  c2_vsRegionId -> cd();
  c2_vsRegionId -> SetGridx();
  c2_vsRegionId -> SetGridy();
  
  if( strcmp(EBEE,"EB") == 0 )
  {
    g_EoC_alpha_vsRegionId[0] -> GetXaxis() -> SetTitle("region ID");
    g_EoC_alpha_vsRegionId[0] -> GetYaxis() -> SetTitle("#alpha from corrected E/p scale");
    g_EoC_alpha_vsRegionId[0] -> GetXaxis() -> SetRangeUser(-1,nRegions);
    g_EoC_alpha_vsRegionId[0] -> SetMinimum(1.00);
    g_EoC_alpha_vsRegionId[0] -> SetMaximum(2.00);
    g_EoC_alpha_vsRegionId[0] -> GetXaxis() -> SetLabelSize(0.04);
    g_EoC_alpha_vsRegionId[0] -> GetYaxis() -> SetLabelSize(0.04);
    g_EoC_alpha_vsRegionId[0] -> GetXaxis() -> SetTitleSize(0.05);
    g_EoC_alpha_vsRegionId[0] -> GetYaxis() -> SetTitleSize(0.05);
    g_EoC_alpha_vsRegionId[0] -> Draw("AP");
    g_EoC_alpha_vsRegionId[0] -> Write("gEB_EoC_alpha_vsRegionId");
  }
  
  TCanvas* c2_map = new TCanvas();
  c2_map -> cd();
  c2_map -> SetGridx();
  c2_map -> SetGridy();
    
  if( strcmp(EBEE,"EB") == 0 )
  {
    h2_EoC_alpha[0] -> GetXaxis() -> SetTitle("i#eta");
    h2_EoC_alpha[0] -> GetYaxis() -> SetTitle("i#phi");
    h2_EoC_alpha[0] -> GetZaxis() -> SetTitle("#alpha from corrected E/p scale");
    h2_EoC_alpha[0] -> GetZaxis() -> SetRangeUser(1.00,2.00);
    h2_EoC_alpha[0] -> GetZaxis() -> SetTitleOffset(1.25);
    h2_EoC_alpha[0] -> GetXaxis() -> SetLabelSize(0.04);
    h2_EoC_alpha[0] -> GetYaxis() -> SetLabelSize(0.04);
    h2_EoC_alpha[0] -> GetZaxis() -> SetLabelSize(0.04);
    h2_EoC_alpha[0] -> GetXaxis() -> SetTitleSize(0.05);
    h2_EoC_alpha[0] -> GetYaxis() -> SetTitleSize(0.05);
    h2_EoC_alpha[0] -> GetZaxis() -> SetTitleSize(0.05);
    h2_EoC_alpha[0] -> Draw("COLZ");
    h2_EoC_alpha[0] -> Write();
  }
  
  
  
  //--------------------
  // Save regional plots
  
  c1_spread -> Print((folderName+"/"+alphaName+"_EoP_alpha_spread.pdf").c_str(),"pdf");
  c1_spread -> Print((folderName+"/"+alphaName+"_EoP_alpha_spread.png").c_str(),"png");
  c1_spread -> SaveAs((folderName+"/"+alphaName+"_EoP_alpha_spread.C").c_str());
  
  c1_vsRegionId -> Print((folderName+"/"+alphaName+"_EoP_alpha_vsRegionId.pdf").c_str(),"pdf");
  c1_vsRegionId -> Print((folderName+"/"+alphaName+"_EoP_alpha_vsRegionId.png").c_str(),"png");
  c1_vsRegionId -> SaveAs((folderName+"/"+alphaName+"_EoP_alpha_vsRegionId.C").c_str());
  
  c1_map -> Print((folderName+"/"+alphaName+"_EoP_alpha_map.pdf").c_str(),"pdf");
  c1_map -> Print((folderName+"/"+alphaName+"_EoP_alpha_map.png").c_str(),"png");
  c1_map -> SaveAs((folderName+"/"+alphaName+"_EoP_alpha_map.C").c_str());
  
  
  
  c2_spread -> Print((folderName+"/"+alphaName+"_EoC_alpha_spread.pdf").c_str(),"pdf");
  c2_spread -> Print((folderName+"/"+alphaName+"_EoC_alpha_spread.png").c_str(),"png");
  c2_spread -> SaveAs((folderName+"/"+alphaName+"_EoC_alpha_spread.C").c_str());
  
  c2_vsRegionId -> Print((folderName+"/"+alphaName+"_EoC_alpha_vsRegionId.pdf").c_str(),"pdf");
  c2_vsRegionId -> Print((folderName+"/"+alphaName+"_EoC_alpha_vsRegionId.png").c_str(),"png");
  c2_vsRegionId -> SaveAs((folderName+"/"+alphaName+"_EoC_alpha_vsRegionId.C").c_str());
  
  c2_map -> Print((folderName+"/"+alphaName+"_EoC_alpha_map.pdf").c_str(),"pdf");
  c2_map -> Print((folderName+"/"+alphaName+"_EoC_alpha_map.png").c_str(),"png");
  c2_map -> SaveAs((folderName+"/"+alphaName+"_EoC_alpha_map.C").c_str());
}
