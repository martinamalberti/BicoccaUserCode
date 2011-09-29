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
// Check here: http://root.cern.ch/root/html/TH1.html#TH1:Fit
// to "Access to the fit status"
//
// *************************************************** */

#ifdef _MAKECINT_
#pragma link C++ class vector<Sorter>+;
#endif

#include "histoFunc.h"

#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TF1.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"
#include "TPaveStats.h"
#include "TLegend.h"
#include "TTree.h"
#include "TVirtualFitter.h"
#include "TFitResultPtr.h"
#include "TFitResult.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <ctime>
#include <map>
#include <algorithm>
#include <math.h>
#include <vector>

float scaleCorr(float scEta=0, float scPhi=0){
  /*
  float corr[36][6]; 
  corr[0][0] = 1.00599;
  corr[1][0] = 0.985565;
  corr[2][0] = 0.997574;
  corr[3][0] = 1.00492;
  corr[4][0] = 0.988292;
  corr[5][0] = 0.995079;
  corr[6][0] = 0.99763;
  corr[7][0] = 0.993101;
  corr[8][0] = 0.986021;
  corr[9][0] = 0.993734;
  corr[10][0] = 0.991952;
  corr[11][0] = 0.983326;
  corr[12][0] = 0.995199;
  corr[13][0] = 0.978099;
  corr[14][0] = 0.989819;
  corr[15][0] = 0.998529;
  corr[16][0] = 0.985957;
  corr[17][0] = 0.996462;
  corr[18][0] = 0.99665;
  corr[19][0] = 1.00142;
  corr[20][0] = 0.998938;
  corr[21][0] = 1.00583;
  corr[22][0] = 1.00294;
  corr[23][0] = 0.998014;
  corr[24][0] = 0.996867;
  corr[25][0] = 0.992165;
  corr[26][0] = 0.988276;
  corr[27][0] = 1.00349;
  corr[28][0] = 0.994762;
  corr[29][0] = 0.995746;
  corr[30][0] = 1.00765;
  corr[31][0] = 0.994711;
  corr[32][0] = 0.994527;
  corr[33][0] = 1.00456;
  corr[34][0] = 0.993193;
  corr[35][0] = 0.994794;
  corr[0][1] = 1.00337;
  corr[1][1] = 0.985423;
  corr[2][1] = 0.998123;
  corr[3][1] = 0.999057;
  corr[4][1] = 0.99075;
  corr[5][1] = 0.990198;
  corr[6][1] = 0.997761;
  corr[7][1] = 0.996472;
  corr[8][1] = 0.988861;
  corr[9][1] = 0.995231;
  corr[10][1] = 0.992006;
  corr[11][1] = 0.985589;
  corr[12][1] = 0.997148;
  corr[13][1] = 0.981686;
  corr[14][1] = 0.985373;
  corr[15][1] = 0.998146;
  corr[16][1] = 0.987632;
  corr[17][1] = 0.99709;
  corr[18][1] = 0.995607;
  corr[19][1] = 0.994788;
  corr[20][1] = 0.993384;
  corr[21][1] = 1.01163;
  corr[22][1] = 0.99601;
  corr[23][1] = 0.99591;
  corr[24][1] = 0.994579;
  corr[25][1] = 0.993266;
  corr[26][1] = 0.997569;
  corr[27][1] = 0.995885;
  corr[28][1] = 0.991287;
  corr[29][1] = 0.995031;
  corr[30][1] = 1.00635;
  corr[31][1] = 0.99407;
  corr[32][1] = 0.98955;
  corr[33][1] = 0.996716;
  corr[34][1] = 0.995974;
  corr[35][1] = 0.994155;
  corr[0][2] = 0.99635;
  corr[1][2] = 0.985615;
  corr[2][2] = 0.995155;
  corr[3][2] = 0.997415;
  corr[4][2] = 0.990272;
  corr[5][2] = 0.99384;
  corr[6][2] = 0.99355;
  corr[7][2] = 0.989922;
  corr[8][2] = 0.988944;
  corr[9][2] = 0.9914;
  corr[10][2] = 0.99632;
  corr[11][2] = 0.990957;
  corr[12][2] = 0.993154;
  corr[13][2] = 0.984108;
  corr[14][2] = 0.992683;
  corr[15][2] = 0.990484;
  corr[16][2] = 0.985475;
  corr[17][2] = 0.997274;
  corr[18][2] = 0.996348;
  corr[19][2] = 0.992226;
  corr[20][2] = 0.988661;
  corr[21][2] = 0.997392;
  corr[22][2] = 1.00513;
  corr[23][2] = 1.00398;
  corr[24][2] = 1.00902;
  corr[25][2] = 0.994925;
  corr[26][2] = 0.996315;
  corr[27][2] = 0.994508;
  corr[28][2] = 0.989732;
  corr[29][2] = 0.99172;
  corr[30][2] = 0.997661;
  corr[31][2] = 0.993801;
  corr[32][2] = 0.986536;
  corr[33][2] = 1.00719;
  corr[34][2] = 0.991897;
  corr[35][2] = 0.995794;
  corr[0][3] = 1.01925;
  corr[1][3] = 0.997424;
  corr[2][3] = 0.99678;
  corr[3][3] = 1.;
  corr[4][3] = 0.995567;
  corr[5][3] = 0.997332;
  corr[6][3] = 0.996774;
  corr[7][3] = 1.01061;
  corr[8][3] = 0.995708;
  corr[9][3] = 1.01219;
  corr[10][3] = 0.996741;
  corr[11][3] = 1.0;
  corr[12][3] = 1.0;
  corr[13][3] = 1.00695;
  corr[14][3] = 0.995754;
  corr[15][3] = 0.995859;
  corr[16][3] = 0.996616;
  corr[17][3] = 1.01146;
  corr[18][3] = 1.00651;
  corr[19][3] = 1.0;
  corr[20][3] = 0.995784;
  corr[21][3] = 1.0;
  corr[22][3] = 0.996648;
  corr[23][3] = 0.997319;
  corr[24][3] = 1.0;
  corr[25][3] = 1.01237;
  corr[26][3] = 0.995478;
  corr[27][3] = 0.997373;
  corr[28][3] = 0.996082;
  corr[29][3] = 0.996913;
  corr[30][3] = 0.997237;
  corr[31][3] = 0.992638;
  corr[32][3] = 0.996131;
  corr[33][3] = 1.0;
  corr[34][3] = 0.996424;
  corr[35][3] = 1.0;
  corr[0][4] = 0.986333;
  corr[1][4] = 0.992293;
  corr[2][4] = 0.98021;
  corr[3][4] = 0.986665;
  corr[4][4] = 0.993835;
  corr[5][4] = 0.981929;
  corr[6][4] = 0.993076;
  corr[7][4] = 1.03176;
  corr[8][4] = 0.994579;
  corr[9][4] = 0.990108;
  corr[10][4] = 0.976878;
  corr[11][4] = 0.990985;
  corr[12][4] = 1.0;
  corr[13][4] = 1.0;
  corr[14][4] = 0.98918;
  corr[15][4] = 0.990142;
  corr[16][4] = 0.988273;
  corr[17][4] = 1.01263;
  corr[18][4] = 1.0;
  corr[19][4] = 0.975999;
  corr[20][4] = 0.975829;
  corr[21][4] = 0.97093;
  corr[22][4] = 1.0;
  corr[23][4] = 0.981232;
  corr[24][4] = 1.0;
  corr[25][4] = 0.985402;
  corr[26][4] = 1.0;
  corr[27][4] = 0.988598;
  corr[28][4] = 1.0;
  corr[29][4] = 0.986145;
  corr[30][4] = 0.985709;
  corr[31][4] = 0.97649;
  corr[32][4] = 0.993902;
  corr[33][4] = 0.991452;
  corr[34][4] = 0.987999;
  corr[35][4] = 1.0;
  corr[0][5] = 1.01097;
  corr[1][5] = 0.993854;
  corr[2][5] = 0.986381;
  corr[3][5] = 0.99383;
  corr[4][5] = 0.99767;
  corr[5][5] = 0.973056;
  corr[6][5] = 0.990839;
  corr[7][5] = 0.990236;
  corr[8][5] = 1.01418;
  corr[9][5] = 1.0155;
  corr[10][5] = 0.990692;
  corr[11][5] = 1.01029;
  corr[12][5] = 0.997804;
  corr[13][5] = 0.995758;
  corr[14][5] = 0.998479;
  corr[15][5] = 1.0;
  corr[16][5] = 0.998842;
  corr[17][5] = 0.99692;
  corr[18][5] = 0.997886;
  corr[19][5] = 0.994407;
  corr[20][5] = 0.981651;
  corr[21][5] = 0.993269;
  corr[22][5] = 0.987388;
  corr[23][5] = 0.98717;
  corr[24][5] = 0.988399;
  corr[25][5] = 0.986084;
  corr[26][5] = 1.0;
  corr[27][5] = 1.0; 
  corr[28][5] = 0.992863;
  corr[29][5] = 0.987841;
  corr[30][5] = 0.974353;
  corr[31][5] = 1.0; 
  corr[32][5] = 1.0; 
  corr[33][5] = 1.0;
  corr[34][5] = 0.994426;
  corr[35][5] = 0.994267;

  int mod=0; 
  if (fabs(scEta)>0.44) mod++; 
  if (fabs(scEta)>0.77) mod++;
  if (fabs(scEta)>1.10) mod++; 
  if (fabs(scEta)>1.49) mod++; 
  if (fabs(scEta)>2.00) mod++; 
    
  // SM:
  float phi = scPhi * 180./3.14159265359 +180.; 
  phi += 10.; 
  int iSM = phi/20;
  if (iSM>17) iSM=0;  
  if (scEta<0) iSM = iSM+18; 

  return corr[iSM][mod]; 
  */
  return 1; 
}

void trisScaleModules(Char_t* infile1 =0,
		      Char_t* infile2 =0)

{
  // Set style options
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetOptTitle(0); 
  gStyle->SetOptStat(1110); 
  gStyle->SetOptFit(1); 

  // Check input filenames and qualifiers
  if (!infile1 || !infile2) {
    std::cout << "CHK-STB Error: Input files not specified !" << std::endl;
    return;
  }
  TFile *f = TFile::Open(infile1);  // data
  TFile *m = TFile::Open(infile2);  // data or mc
  
  
  // Get trees
  std::cout << std::endl;
  
  TTree* ntu_DA = (TTree*)(f->Get("ntu"));
  std::cout << "     DATA: " << std::setw(8) << ntu_DA->GetEntries() << " entries in " << infile1 << std::endl;
  
  TTree* ntu_MC = (TTree*)(m->Get("ntu"));
  std::cout << "REFERENCE: " << std::setw(8) << ntu_MC->GetEntries() << " entries in " << infile2 << std::endl;
  
  if (ntu_DA->GetEntries()==0 || ntu_MC->GetEntries()==0) {
    std::cout << "CHK-STB Error: At least one file is empty" << std::endl; 
    return;
  }
  
  // Set branch addresses
  int runId;
  int isW, isZ;
  float seedLaserAlpha;
  float avgLaserCorrection, EoP, scEta, scPhi, fbrem;
  
 
  ntu_DA->SetBranchAddress("runId", &runId);  
  ntu_DA->SetBranchAddress("isW", &isW);
  //ntu_DA->SetBranchAddress("isZ", &isZ);
  ntu_DA->SetBranchAddress("ele1_scLaserCorr", &avgLaserCorrection);
  ntu_DA->SetBranchAddress("ele1_seedLaserAlpha", &seedLaserAlpha);
  ntu_DA->SetBranchAddress("ele1_scEta", &scEta);
  ntu_DA->SetBranchAddress("ele1_scPhi", &scPhi);
  ntu_DA->SetBranchAddress("ele1_EOverP", &EoP);
  ntu_DA->SetBranchAddress("ele1_fbrem", fbrem); 
 

  // for 2010
  /*  
  ntu_DA->SetBranchAddress("avgLaserCorrection1", &avgLaserCorrection);
  ntu_DA->SetBranchAddress("seedLaserAlpha1", &seedLaserAlpha);
  ntu_DA->SetBranchAddress("scEta1", &scEta);
  ntu_DA->SetBranchAddress("scPhi1", &scPhi);
  ntu_DA->SetBranchAddress("EoP1", &EoP);
  */

  // for 2010 or reference (may need to re-make the ntuple
  /*  ntu_MC->SetBranchAddress("isW", &isW);
  ntu_MC->SetBranchAddress("isZ", &isZ);
  ntu_MC->SetBranchAddress("scEta1", &scEta);
  ntu_MC->SetBranchAddress("EoP1", &EoP);
  ntu_MC->SetBranchAddress("fbrem1", &fbrem);
  */
  ntu_MC->SetBranchAddress("runId", &runId);  
  ntu_MC->SetBranchAddress("isW", &isW);
  //ntu_DA->SetBranchAddress("isZ", &isZ);
  ntu_MC->SetBranchAddress("ele1_scLaserCorr", &avgLaserCorrection);
  ntu_MC->SetBranchAddress("ele1_seedLaserAlpha", &seedLaserAlpha);
  ntu_MC->SetBranchAddress("ele1_scEta", &scEta);
  ntu_MC->SetBranchAddress("ele1_scPhi", &scPhi);
  ntu_MC->SetBranchAddress("ele1_EOverP", &EoP);
  ntu_MC->SetBranchAddress("ele1_fbrem", fbrem); 
  
  // Build the references from 'infile2'
  std::cout << std::endl;
  std::cout << "***** Build references *****" << std::endl;
  
  TH1F* h_template = new TH1F("template", "", 1200, 0., 3.);
  TH1F* h_Ref[6]; 
  for (int i=0;i<6;i++){
    char refName[24];
    sprintf (refName,"Mod %d",i+1); 
    h_Ref[i] = new TH1F(refName,"",1200,0.,3.); 
  }

  for(int ientry = 0; ientry < ntu_MC->GetEntries(); ++ientry) {
    if( (ientry%100000 == 0) ) std::cout << "reading MC entry " << ientry << std::endl;
    ntu_MC->GetEntry(ientry);
    if (fabs(scEta)>1.44 && fabs(scEta)<1.566 ) continue; //Endcap event!
    if (fabs(scEta)>2.50 ) continue; //Endcap event!
    if (fbrem>0.5) continue;


    int i=0; 
    if (fabs(scEta)>0.44) i++; 
    if (fabs(scEta)>0.77) i++;
    if (fabs(scEta)>1.10) i++; 
    if (fabs(scEta)>1.50) i++; 
    if (fabs(scEta)>2.00) i++;

    h_Ref[i] -> Fill(EoP);
    h_template -> Fill(EoP); 
  }
  
  for (int i=0;i<6;i++) {
    std::cout << "Reference built for Mod " << i+1 << " - " << h_Ref[i]->GetEntries() << " events" << std::endl;
  }
    
  // histogram definition
  TVirtualFitter::SetDefaultFitter("Fumili2");
  TH1F* h_EoC[36][6];
  TH1F* h_EoC_spread[6];
  TH1F* h_EB_spread = new TH1F("h_EB_spread","",50,0.9,1.1);  
  h_EB_spread -> GetXaxis() -> SetTitle("Relative E/p scale");  
  TH1F* h_EE_spread = new TH1F("h_EE_spread","",50,0.9,1.1);
  h_EE_spread -> GetXaxis() -> SetTitle("Relative E/p scale");  
    
  for(int mod=0; mod<6; mod++) {
    char histoName[80];
    sprintf(histoName,"Spread for Mod %d",mod+1); 
    h_EoC_spread[mod] = new TH1F(histoName,"",50,0.9,1.1);
    h_EoC_spread[mod] -> SetFillStyle(3002);
    h_EoC_spread[mod] -> SetFillColor(mod+2);
    h_EoC_spread[mod] -> GetXaxis() -> SetTitle("Relative E/p scale");  
  
    for(int iSM = 0; iSM < 36; iSM++) {
         
      sprintf(histoName, "EoC_%02d_%d", iSM, mod);
      h_EoC[iSM][mod] = new TH1F(histoName, histoName, 1200, 0., 3.);
      h_EoC[iSM][mod] -> SetFillColor(kGreen+2);
      h_EoC[iSM][mod] -> SetFillStyle(3004);
      h_EoC[iSM][mod] -> SetMarkerStyle(7);
      h_EoC[iSM][mod] -> SetMarkerColor(kGreen+2);     
    }
  }
    
  std::cout << std::endl;
  std::cout << "***** Fill and fit histograms *****" << std::endl;
  
  int nEntries = ntu_DA->GetEntries(); 
  for(int ientry = 0; ientry < nEntries; ++ientry)
  {
    if( (ientry%10000 == 0) ) std::cout << "reading entry " << ientry << std::endl;    
    ntu_DA->GetEntry(ientry);

    if (fabs(scEta)>1.44 && fabs(scEta)<1.566 ) continue; //Endcap event!
    if (fabs(scEta)>2.50 ) continue; //Endcap event!
    if (fbrem>0.5) continue;
    
    //    float scale = 1.;
    //    scale = sqrt( pow(avgLaserCorrection,((1.52-0.197)/1.52)-1.) );
    //    scale = 1. / (0.1811 + 0.7843*avgLaserCorrection);

    // module:
    int mod=0; 
    if (fabs(scEta)>0.44) mod++; 
    if (fabs(scEta)>0.77) mod++;
    if (fabs(scEta)>1.10) mod++; 
    if (fabs(scEta)>1.50) mod++; 
    if (fabs(scEta)>2.00) mod++; 
    
    // SM:
    float phi = scPhi * 180./3.14159265359 +180.; 
    phi += 10.; 
    int iSM = phi/20;
    if (iSM>17) iSM=0;  
    if (scEta<0) iSM = iSM+18; 

    // if( (ientry%10000 == 0) ) std:: cout << "Filling:  " << iSM << " " << mod << " " << EoP << endl;
    h_EoC[iSM][mod] -> Fill(EoP * scaleCorr(scEta,scPhi) ); 

  }

  int rebin = 4;
  
  // Make fits and plots  
  TCanvas *c1[36]; 
  for (int iSM=0;iSM<36;iSM++){
    char canvasName[50];
    sprintf(canvasName, "Fits for SM %0d", iSM); 
    c1[iSM] = new TCanvas(canvasName, canvasName,150,150,500,500);
    c1[iSM]->Divide(3,2); 
    for (int mod=0;mod<6;mod++) { 
      h_EoC[iSM][mod] -> Rebin(rebin*2);    
      c1[iSM]->cd(mod+1); 
      h_EoC[iSM][mod] -> GetXaxis() -> SetTitle("E/p");  
      h_EoC[iSM][mod] -> GetXaxis() -> SetRangeUser(0.5,1.5); 
      h_EoC[iSM][mod]->Draw("e"); 
    }
  }


  //  h_template->Rebin(rebin*2); 
  for (int mod=0;mod<6;mod++){
    h_Ref[mod] -> Rebin(rebin*2);
    //    h_template->Draw();
    for (int iSM=0;iSM<36;iSM++){
  
      // define the fitting function
      // N.B. [0] * ( [1] * f( [1]*(x-[2]) ) )
      //histoFunc* templateHistoFunc = new histoFunc(h_template);
      histoFunc* templateHistoFunc = new histoFunc(h_Ref[mod]);
      
      TF1* templateFunc = new TF1("templateFunc", templateHistoFunc, 0.2, 1.4, 3, "histoFunc");
      templateFunc -> SetParName(0,"Norm"); 
      templateFunc -> SetParName(1,"Scale factor"); 
      templateFunc -> SetLineWidth(1); 
      templateFunc -> SetNpx(10000);
      
      double xNorm = h_EoC[iSM][mod]->GetEntries()/h_Ref[mod]->GetEntries() *
	h_EoC[iSM][mod]->GetBinWidth(1)/h_Ref[mod]->GetBinWidth(1); 
      
      templateFunc -> FixParameter(0, xNorm);
      templateFunc -> SetParameter(1, 0.99);
      templateFunc -> FixParameter(2, 0.);
      
      //      std::cout << "***** Fitting Bin: " << iSM << endl; 
      
      templateFunc -> SetLineColor(kRed+2); 
      TFitResultPtr r = h_EoC[iSM][mod] -> Fit("templateFunc", "QERLS+");
      int fStatus = r;
      double precision = r->Edm(); 
      
      // Fill the graph for uncorrected data

      cout << "corr["<<iSM<<"]["<<mod<<"] = " << templateFunc->GetParameter(1) << endl;
      
      if (fStatus ==0) { 
	  h_EoC_spread[mod] -> Fill(1./templateFunc->GetParameter(1));
	  if (mod<4) h_EB_spread -> Fill(1./templateFunc->GetParameter(1));
	  else h_EE_spread -> Fill(1./templateFunc->GetParameter(1));
      }
      else {
	//	  h_EoC_spread[mod] -> Fill(1./templateFunc->GetParameter(1));
	  double eee= templateFunc->GetParError(1); 
	  cout << 
	    "  Fail:   " << fStatus << " on SM " << iSM << " mod " << mod <<
	    ", Scale = " <<  1./templateFunc->GetParameter(1) << 
	    " +/- "      << eee << 
	    ", edm = "   << precision << endl; 
      }
    }
  }


  // summarry plots
  TCanvas* ccc = new TCanvas("g_fit", "g_fit",100,100,700,500);
  ccc->Divide(2,1);
  ccc->cd(1);
  h_EB_spread->Draw(); 

  //  THStack hs("hs","");
  //  for (int mod=0;mod<4;mod++){
  //    hs.Add(h_EoC_spread[mod]); 
  //  }
  //  hs.Draw();
  ccc->cd(2);
  h_EE_spread->Draw(); 
  //  THStack he("he","");
  //  for (int mod=4;mod<6;mod++){
  //    he.Add(h_EoC_spread[mod]); 
  //  }
  //  he.Draw();
}
