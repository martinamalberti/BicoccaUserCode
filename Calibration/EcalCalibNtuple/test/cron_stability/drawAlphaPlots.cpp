// g++ -Wall -o drawAlphaPlots `root-config --cflags --glibs` setTDRStyle.cc ntupleUtils.cc stabilityUtils.cc drawAlphaPlots.cpp

#include "setTDRStyle.h"
#include "ntupleUtils.h"
#include "stabilityUtils.h"

#include <iostream>
#include <cstdlib>

#include "TFile.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TLatex.h"

int t1 = 1325376001;   //  1 Jan 2012
int t2 = 1356134399;   // 31 Dec 2012

TH1F* templateHisto;
TF1* templateFunc;
std::vector<double>* mydata;



int main(int argc, char** argv)
{
  setTDRStyle();
  gStyle -> SetPadRightMargin(0.13);
  
  std::cout << "\n***************************************************************************************************************************" << std::endl;
  std::cout << "arcg: " << argc << std::endl;
  char* EBEE = argv[1];
  std::string dayMin = "";
  std::string dayMax = "";
  std::string dayMinLabel = "";
  std::string dayMaxLabel = "";
  float absEtaMin = -1.;
  float absEtaMax = -1.;
    
  if(argc >= 3)
  {
    dayMin = std::string(argv[2])+" "+std::string(argv[3])+" "+std::string(argv[4]);
    dayMax = std::string(argv[5])+" "+std::string(argv[6])+" "+std::string(argv[7]);
    dayMinLabel = std::string(argv[2])+"_"+std::string(argv[3])+"_"+std::string(argv[4]);
    dayMaxLabel = std::string(argv[5])+"_"+std::string(argv[6])+"_"+std::string(argv[7]);
    
    t1 = dateToInt(dayMin);
    t2 = dateToInt(dayMax);
  }    
  if(argc >= 9)
  {
    absEtaMin = atof(argv[8]);
    absEtaMax = atof(argv[9]);
  }
    
  std::cout << "EBEE: "          << EBEE          << std::endl;
  std::cout << "dayMin: "        << dayMin        << std::endl;
  std::cout << "dayMax: "        << dayMax        << std::endl;
  std::cout << "absEtaMin: "     << absEtaMin     << std::endl;
  std::cout << "absEtaMax: "     << absEtaMax     << std::endl;
  
  
  
  //-------------------
  // Define in/outfiles

  std::string folderName = std::string(EBEE) + "_1_1_2012_31_12_2012";
  if( (absEtaMin != -1.) && (absEtaMax != -1.) )
  {
    char absEtaBuffer[50];
    sprintf(absEtaBuffer,"_%.2f-%.2f",absEtaMin,absEtaMax);
    folderName += std::string(absEtaBuffer);
  }
  TFile* inFile = TFile::Open((folderName+"/"+folderName+"_histos.root").c_str(),"READ");
  
  
  
  //-----------------
  // Build alpha plot
  
  TGraphErrors*      g_LT        = (TGraphErrors*)     ( inFile->Get("g_LT") );
  TGraphAsymmErrors* g_fit       = (TGraphAsymmErrors*)( inFile->Get("g_fit") );
  TGraphAsymmErrors* g_c_fit     = (TGraphAsymmErrors*)( inFile->Get("g_c_fit") );
  TGraphAsymmErrors* g_c_fit_run = (TGraphAsymmErrors*)( inFile->Get("g_c_fit_run") );
  
  TGraphErrors* g_EoP_alpha = new TGraphErrors();
  TGraphErrors* g_EoC_alpha = new TGraphErrors();
  
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
    if( (EoPErr < 0.0001) || (EoCErr < 0.0001) ) continue;
    if( (time < t1) || (time > t2) ) continue;
    
    //std::cout << ">>> getting point " << point << std::endl;
    //std::cout << ">>>>>> time: " << time
    //          << "   LT: "  << LT
    //          << "   EoP: " << EoP
    //          << "   EoC: " << EoC
    //          << std::endl;
    
    g_EoP_alpha -> SetPoint(savedPoints,LT,log(EoP));
    g_EoC_alpha -> SetPoint(savedPoints,LT,log(EoC));
    g_EoP_alpha -> SetPointError(savedPoints,LTErr,EoPErr/EoP);
    g_EoC_alpha -> SetPointError(savedPoints,LTErr,EoCErr/EoC);
    
    ++savedPoints;
  }
  
  g_EoP_alpha -> SetPoint(g_EoP_alpha->GetN(),-1.,0.);
  g_EoP_alpha -> SetPoint(g_EoP_alpha->GetN(), 1.,0.);
  g_EoC_alpha -> SetPoint(g_EoC_alpha->GetN(),-1.,0.);
  g_EoC_alpha -> SetPoint(g_EoC_alpha->GetN(), 1.,0.);
  
  
  
  //----------------
  // Draw alpha plot
  
  TCanvas* c1 = new TCanvas();
  c1 -> cd();
  c1 -> SetGridx();
  c1 -> SetGridy();
  
  g_EoP_alpha -> SetMarkerStyle(24);
  g_EoP_alpha -> SetMarkerSize(0.7);
  g_EoP_alpha -> SetMarkerColor(kRed+2);
  g_EoP_alpha -> SetLineColor(kRed+2);
  g_EoP_alpha -> GetXaxis() -> SetNdivisions(510);
  g_EoP_alpha -> GetXaxis() -> SetLabelSize(0.04);
  g_EoP_alpha -> GetYaxis() -> SetLabelSize(0.04);
  g_EoP_alpha -> GetXaxis() -> SetTitle("#LT log [ R/R_{0} ] #GT");
  g_EoP_alpha -> GetYaxis() -> SetTitle("log [ E/p relative scale ]");
  g_EoP_alpha -> GetYaxis() -> SetTitleOffset(1.10);
  if( strcmp(EBEE,"EB") == 0 )
  {
    g_EoP_alpha -> GetXaxis() -> SetRangeUser(-0.060,0.015);
    g_EoP_alpha -> GetYaxis() -> SetRangeUser(-0.080,0.020);
  }
  else
  {
    g_EoP_alpha -> GetXaxis() -> SetRangeUser(-0.300,0.075);
    g_EoP_alpha -> GetYaxis() -> SetRangeUser(-0.400,0.100);
  }
  
  g_EoC_alpha -> SetMarkerStyle(20);
  g_EoC_alpha -> SetMarkerSize(0.7);
  g_EoC_alpha -> SetMarkerColor(kGreen+2);
  g_EoC_alpha -> SetLineColor(kGreen+2);
  g_EoC_alpha -> GetXaxis() -> SetNdivisions(510);
  g_EoC_alpha -> GetXaxis() -> SetLabelSize(0.04);
  g_EoC_alpha -> GetYaxis() -> SetLabelSize(0.04);
  g_EoC_alpha -> GetXaxis() -> SetTitle("#LT log [ R/R_{0} ] #GT");
  g_EoC_alpha -> GetYaxis() -> SetTitle("log [ E/p relative scale ]");
  g_EoC_alpha -> GetYaxis() -> SetTitleOffset(1.10);
  if( strcmp(EBEE,"EB") == 0 )
  {
    g_EoC_alpha -> GetXaxis() -> SetRangeUser(-0.060,0.015);
    g_EoC_alpha -> GetYaxis() -> SetRangeUser(-0.080,0.020);
  }
  else
  {
    g_EoC_alpha -> GetXaxis() -> SetRangeUser(-0.300,0.075);
    g_EoC_alpha -> GetYaxis() -> SetRangeUser(-0.400,0.100);
  }
  
  TF1* f_EoP = new TF1("f_EoP","[0]+[1]*x",-0.500,0.);
  f_EoP -> SetLineColor(kRed+2);
  f_EoP -> SetLineStyle(7);
  f_EoP -> SetLineWidth(2);
  if( strcmp(EBEE,"EB") == 0 ) f_EoP -> SetParameters(0.,1.52);
  else                         f_EoP -> SetParameters(0.,1.16);
  if( strcmp(EBEE,"EB") == 0 ) g_EoP_alpha -> Fit("f_EoP","QNS+","",-0.050,-0.010);
  else                         g_EoP_alpha -> Fit("f_EoP","QNS+","",-0.300,-0.050);
  
  TF1* f_EoC = new TF1("f_EoC","[0]+[1]*x",-0.500,0.);
  f_EoC -> SetLineColor(kGreen+2);
  f_EoC -> SetLineStyle(7);
  f_EoC -> SetLineWidth(2);
  f_EoC -> SetParameters(0.,0.);
  if( strcmp(EBEE,"EB") == 0 ) g_EoC_alpha -> Fit("f_EoC","QNS+","",-0.050,-0.010);
  else                         g_EoC_alpha -> Fit("f_EoC","QNS+","",-0.300,-0.050);
  
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
    
  g_EoP_alpha -> Draw("AP");  
  g_EoC_alpha -> Draw("P,same");
  f_EoP -> Draw("same");
  f_EoC -> Draw("same");
  latex_EoP -> Draw("same");
  latex_EoC -> Draw("same");
  
  
  
  //----------------
  // Save alpha plot
  
  std::string alphaName = std::string(EBEE) + "_" + dayMinLabel + "_" + dayMaxLabel + "_alphaPlot";
  
  TFile* outFile = TFile::Open((folderName+"/"+alphaName+"_alphaHistos.root").c_str(),"RECREATE");
  outFile -> cd();
  
  c1 -> Print((folderName+"/"+alphaName+".pdf").c_str(),"pdf");
  c1 -> Print((folderName+"/"+alphaName+".png").c_str(),"png");
  c1 -> SaveAs((folderName+"/"+alphaName+".C").c_str());
  
  g_EoP_alpha -> Write("g_EoP_alpha");
  g_EoC_alpha -> Write("g_EoC_alpha");
}
