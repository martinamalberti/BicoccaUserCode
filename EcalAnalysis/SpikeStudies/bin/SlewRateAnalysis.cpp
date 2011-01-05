// per compilare  c++ `root-config --libs --cflags --glibs` SlewRateAnalysis.cpp -o SlewRateAnalysis

//shapeFunctions
#include "EcalAnalysis/SpikeStudies/bin/ShapeFunctions.cc"

// ROOT includes
#include "TROOT.h"
#include "TApplication.h"
#include "TFile.h"
#include "TF1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TTree.h"
#include "TChain.h"
#include "TRandom.h"
#include "TString.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TLeaf.h"
#include "TNtuple.h"
#include "TMath.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <cmath>
#include <map>
#include <vector>

using namespace std;
 

int main(int argc, char** argv) {
  
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);

  // load ntuple
  TChain *chain = new TChain ("myanalysis/EcalAnalysisTree","EcalAnalysisTree") ;

  chain->Add("/tmp/malberti/EcalTree_EG_Run2010A-WZEG-Nov4Skim_v1_RAW-RECO.root");  
  chain->Add("/tmp/malberti/EcalTree_Electron_Run2010B-WZEG-Nov4Skim_v1_RAW-RECO.root");  

  int nEntries = (int)chain->GetEntries() ;
  cout << "FOUND " << nEntries  << " ENTRIES\n" << endl;;

 
  //ecalVariables variables
  unsigned int runId;
  unsigned int eventId;
  unsigned int eventNaiveId;
 
  int nEcalRecHits;
  float ecalRecHitType[3300];
  float ecalRecHitEnergy[3300];
  float ecalRecHitIEta[3300];
  float ecalRecHitIPhi[3300];
  float ecalRecHitTime[3300];
  float ecalRecHitChi2[3300];
  int ecalRecHitRecoFlag[3300];
  float ecalRecHitMatrixFlag[3300][5][5];
  float ecalRecHitMatrix[3300][5][5];
  float ecalRecHitS4oS1[3300];
  int ecalDigis[3300][10];
  int ecalGainId[3300][10];

  // Set branch addresses.
  chain -> SetBranchAddress("runId",                     &runId);
  chain -> SetBranchAddress("eventId",                   &eventId);
  chain -> SetBranchAddress("eventNaiveId",              &eventNaiveId);
  chain -> SetBranchAddress("nEcalRecHits",              &nEcalRecHits);
  chain -> SetBranchAddress("ecalRecHitType",            ecalRecHitType);
  chain -> SetBranchAddress("ecalRecHitEnergy",          ecalRecHitEnergy);
  chain -> SetBranchAddress("ecalRecHitIEta",            ecalRecHitIEta);
  chain -> SetBranchAddress("ecalRecHitIPhi",            ecalRecHitIPhi);
  chain -> SetBranchAddress("ecalRecHitTime",            ecalRecHitTime);
  chain -> SetBranchAddress("ecalRecHitChi2",            ecalRecHitChi2);
  chain -> SetBranchAddress("ecalRecHitRecoFlag",        ecalRecHitRecoFlag);
  chain -> SetBranchAddress("ecalRecHitMatrixFlag",      ecalRecHitMatrixFlag);
  chain -> SetBranchAddress("ecalRecHitMatrix",          ecalRecHitMatrix);
  chain -> SetBranchAddress("ecalRecHitS4oS1",           ecalRecHitS4oS1);
  chain -> SetBranchAddress("ecalDigis",                 ecalDigis);
  chain -> SetBranchAddress("ecalGainId",                ecalGainId);
   
   
  // output file
  char outfileName[100];
  sprintf(outfileName,"HistosSlewRate.root");
  cout << "Saving histograms on file " << outfileName  <<endl;

  TFile saving (outfileName,"recreate") ;
  

  // histos 
  TH1F *hGainSwitch = new TH1F("hGainSwitch","hGainSwitch",10,-5,5);
  hGainSwitch->GetXaxis()->SetTitle("Gain(max) - Gain(max-1)");

  TH1F *hSlewRate = new TH1F("hSlewRate","hSlewRate", 5000, 0, 5000); 
  hSlewRate ->SetFillStyle(1);
  hSlewRate ->SetFillColor(kRed);
  hSlewRate ->GetXaxis()->SetTitle("Slew Rate (ADC/ns)");

  TH2F *hAmplitude = new TH2F("hAmplitude","hAmplitude", 1000, 0, 10000, 500, 0, 5000); 
  hAmplitude -> SetMarkerColor(kRed);
  hAmplitude -> SetMarkerStyle(20);
  hAmplitude -> SetMarkerSize(0.35);
  hAmplitude -> GetXaxis() -> SetTitle("Expected Amplitude (ADC)");
  hAmplitude -> GetYaxis() -> SetTitle("Observed Amplitude (ADC)");

  TH2F *hAmplitude2 = new TH2F("hAmplitude2","hAmplitude2", 1000, 0, 10000, 500, 0, 5000); 
  hAmplitude2 -> SetMarkerColor(kBlue);
  hAmplitude2 -> SetMarkerStyle(20);
  hAmplitude2 -> SetMarkerSize(0.35);
  hAmplitude2 -> GetXaxis() -> SetTitle("Expected Amplitude (ADC)");
  hAmplitude2 -> GetYaxis() -> SetTitle("Observed Amplitude (ADC)");


  TH2F *hAmpl_vs_Time = new TH2F("hAmpl_vs_Time","hAmpl_vs_Time", 800, -100, 100, 1000, 0, 10000); 
  hAmpl_vs_Time -> SetMarkerColor(kRed);
  hAmpl_vs_Time -> SetMarkerStyle(20);
  hAmpl_vs_Time -> SetMarkerSize(0.3);
  hAmpl_vs_Time -> GetXaxis() -> SetTitle("Time (ns)");
  hAmpl_vs_Time -> GetYaxis() -> SetTitle("Expected Amplitude (ADC)");

  TH2F *hRatio_vs_Time = new TH2F("hRatio_vs_Time","hRatio_vs_Time", 800, -100, 100, 200, 0, 2); 
  hRatio_vs_Time -> SetMarkerColor(kRed);
  hRatio_vs_Time -> SetMarkerStyle(20);
  hRatio_vs_Time -> SetMarkerSize(0.3);
  hRatio_vs_Time -> GetXaxis() -> SetTitle("Time (ns)");
  hRatio_vs_Time -> GetYaxis() -> SetTitle("Ratio");

  TH1F *hr1 = new TH1F("hr1","hr1",200,0,2);
  TH1F *hr2 = new TH1F("hr2","hr2",200,0,2);

  TH1F *hTfit1 = new TH1F("hTfit1","hTfit1 (expected Amplitude < 3000 ADC)",400,-50,50);
  hTfit1 -> SetFillStyle(3001);
  hTfit1 -> SetFillColor(kRed+2);
  hTfit1 -> GetXaxis() -> SetTitle("T_{fit} (ns)");

  TH1F *hTfit2 = new TH1F("hTfit2","hTfit2 (expected Amplitude > 3000 ADC)",400,-50,50);
  hTfit2 -> SetFillStyle(3001);
  hTfit2 -> SetFillColor(kBlue+2);
  hTfit2 -> GetXaxis() -> SetTitle("T_{fit} (ns)");

  TH1F *hAmax1 = new TH1F("hAmax1","hTAmax1 (expected Amplitude < 3000 ADC)",1000,0,10000);
  hAmax1 -> SetFillStyle(3001);
  hAmax1 -> SetFillColor(kRed+2);
  hAmax1 -> GetXaxis() -> SetTitle("A_{max} (ADC counts)");

  TH1F *hAmax2 = new TH1F("hAmax2","hTAmax2 (expected Amplitude > 3000 ADC)",1000,0,10000);
  hAmax2 -> SetFillStyle(3001);
  hAmax2 -> SetFillColor(kBlue+2);
  hAmax2 -> GetXaxis() -> SetTitle("A_{max} (ADC counts)");


  TGraphErrors* gsample;
  char gname[100];
  char gtitle[100];
  int ievt=0;  

  float A[3300][10];
  float G[3] = {1.,2.,12.}; // gain ratios

  //loop over the events
  for (int entry = 0 ; entry < nEntries; ++entry) {
      
    chain -> GetEntry(entry) ;
    if (entry%10000==0) cout << "    Analyzing entry " << entry << endl;
        
    for (int ihit =0 ; ihit < nEcalRecHits; ihit++) {
      
      // barrel only
      if ( ecalRecHitType[ihit]!=0) continue; 

      float energy = ecalRecHitEnergy[ihit];
      float eta = ecalRecHitIEta[ihit]*0.0175;
      float theta = 2. * atan(exp(-eta));
      float et = energy*sin(theta); 
      
      //if ( energy < 100. ) continue;      // E>10 GeV
      
      //cerco il sample max
      float ped = (ecalDigis[ihit][0] + ecalDigis[ihit][1] + ecalDigis[ihit][2])/3;
      int imax = 0;
      int maxAmpl = 0;
      for (int isample = 0; isample < 10 ; isample++)
	{
	  if (ecalGainId[ihit][isample]!=0) A[ihit][isample] = (ecalDigis[ihit][isample] - ped) * G[ ecalGainId[ihit][isample] - 1]; 
	  
	  if ( A[ihit][isample] >= maxAmpl) 
	    {
	      imax = isample;
	      maxAmpl = A[ihit][isample] ;
	    }
	} // end loop over 10 samples
      


      // check gain switch
      bool gainSwitch = false;
      if (ecalGainId[ihit][imax-1]==1 && ecalGainId[ihit][imax] != ecalGainId[ihit][imax-1])   gainSwitch = true;
      
      //Seleziono eventi con gain switch
      if (!gainSwitch) continue;
      hGainSwitch -> Fill( ecalGainId[ihit][imax]-ecalGainId[ihit][imax-1]);

      //cout << "MAX sample = "<< imax << "   MAX amplitude = "<< A[ihit][imax] << "   MAX gain Id =" << ecalGainId[ihit][imax]<< endl;
      //cout << "MAX-1 sample = "<< imax-1 << "   MAX-1 amplitude = "<< A[ihit][imax-1] << "   MAX-1 gain Id =" << ecalGainId[ihit][imax-1] << endl;
      //cout << endl;   
      
     
      float slewRate = A[ihit][imax-1];
      hSlewRate -> Fill(slewRate); 

      // graphs
      gsample = new TGraphErrors();
      sprintf(gtitle,"shape_Run%d_Evt%d",runId,eventId);
      sprintf(gname,"shape_%d",ievt);
      gsample->SetName(gname);
      gsample->SetTitle(gtitle);
      gsample->GetXaxis()->SetTitle("Time (ns)");
      gsample->GetYaxis()->SetTitle("ADC counts");
      
      for (int isample = 0; isample < 10 ; isample++)
      	{
      	  gsample->SetPoint(isample, double(isample)*25., A[ihit][isample]);      
      	  gsample->SetPointError(isample,0.,0.9);      
      	}



      
      // fit function            
      TF1 *fpulseShape = new TF1("fpulseShape", pulseShape,0.,240.,3);
      fpulseShape->SetLineColor(kBlue);
      fpulseShape->SetParName(0,"Ped");
      fpulseShape->SetParName(1,"A");
      fpulseShape->SetParName(2,"T0");
      fpulseShape->FixParameter(0,0); // ped
      fpulseShape->SetParameter(1,A[ihit][imax]); // ampl
      fpulseShape->SetParameter(2,imax*25.); // T0
      gsample->RemovePoint(imax-1);
      gsample->Fit("fpulseShape","QSR+") ;
      //re-set dei punti, altrimenti root non li plotta!
      for (int isample = 0; isample < 10 ; isample++)
      	{
      	  gsample->SetPoint(isample, double(isample)*25., A[ihit][isample]);      
      	  gsample->SetPointError(isample,0.,0.9);      
      	}
      ievt++;
      

      float measuredAmpl = A[ihit][imax-1] ;
      float expectedAmpl = A[ihit][imax] * fpulseShape->Eval((imax-1)*25.)/fpulseShape->Eval(imax*25.);
                  
      hAmplitude  -> Fill(expectedAmpl,measuredAmpl);
      hAmplitude2 -> Fill( A[ihit][imax] * 0.774,measuredAmpl);
 

      float t0 = fpulseShape->GetParameter(2);
      
      hAmpl_vs_Time -> Fill(t0-125.,expectedAmpl);
      hRatio_vs_Time -> Fill(t0-125., fpulseShape->Eval((imax-1)*25.)/fpulseShape->Eval(imax*25.));
      
      if (expectedAmpl > 3000) {
	hr2->Fill( fpulseShape->Eval((imax-1)*25.)/fpulseShape->Eval(imax*25.));
	hTfit2->Fill(t0-125.);
	hAmax2->Fill(A[ihit][imax]);
      }
      else {
	hr1->Fill( fpulseShape->Eval((imax-1)*25.)/fpulseShape->Eval(imax*25.));
	hTfit1->Fill(t0-125.);
	hAmax1->Fill(A[ihit][imax]);
      }

      if ( expectedAmpl < 3000 )
	//if (ievt<10)
	gsample->Write();

    } // end loop over recHits
    
  }//end loop over events
 
 
  chain->Delete();

  saving.cd () ;

  hGainSwitch ->Write();
  hSlewRate   -> Write();
  hAmplitude  ->Write();
  hAmplitude2 ->Write();
  hAmpl_vs_Time ->Write();
  hRatio_vs_Time ->Write();
  hTfit1->Write();
  hTfit2->Write();
  hAmax1->Write();
  hAmax2->Write();

  hr1->Write();
  hr2->Write();

  saving.Close () ;
  
  return 0;
  
  
}




