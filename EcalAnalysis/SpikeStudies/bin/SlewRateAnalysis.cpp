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
  int ecalRecHitRawId[3300];
  float ecalRecHitChi2[3300];
  float ecalRecHitOutOfTimeChi2[3300];
  int ecalRecHitRecoFlag[3300];
  float ecalRecHitMatrixFlag[3300][5][5];
  float ecalRecHitMatrix[3300][5][5];
  float ecalRecHitR9[3300];
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
  chain -> SetBranchAddress("ecalRecHitR9",              ecalRecHitR9);
  chain -> SetBranchAddress("ecalRecHitS4oS1",           ecalRecHitS4oS1);
  chain -> SetBranchAddress("ecalDigis",                 ecalDigis);
  chain -> SetBranchAddress("ecalGainId",                ecalGainId);
   
   
  // output file
  char outfileName[100];
  sprintf(outfileName,"HistosSlewRate.root");
  cout << "Saving histograms on file " << outfileName  <<endl;

  TFile saving (outfileName,"recreate") ;
  

  // histos 
  TH1F *hSlewRate = new TH1F("hSlewRate","hSlewRate", 5000, 0, 5000); 
  hSlewRate ->SetFillStyle(1);
  hSlewRate ->SetFillColor(kRed);
  hSlewRate ->GetXaxis()->SetTitle("Slew Rate (ADC/ns)");

  TH2F *hAmplitude = new TH2F("hAmplitude","hAmplitude", 10000, 0, 10000, 5000, 0, 5000); 
  hAmplitude -> SetMarkerColor(kRed);
  hAmplitude -> SetMarkerStyle(20);
  hAmplitude -> SetMarkerSize(0.3);
  hAmplitude -> GetXaxis() -> SetTitle("Expected Amplitude (ADC)");
  hAmplitude -> GetYaxis() -> SetTitle("Observed Amplitude (ADC)");

  TGraphErrors* gsample;
  char gname[100];
  char gnameCanvas[100];
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
      if (gainSwitch != true) continue;
      
      //if (ecalGainId[ihit][imax]!=3 ) continue;

      cout << "MAX sample = "<< imax << "   MAX amplitude = "<< A[ihit][imax] << "   MAX gain Id =" << ecalGainId[ihit][imax]<< endl;
      cout << "MAX-1 sample = "<< imax-1 << "   MAX-1 amplitude = "<< A[ihit][imax-1] << "   MAX-1 gain Id =" << ecalGainId[ihit][imax-1] << endl;
      cout << endl;   
      
     
      float slewRate = A[ihit][imax-1];
      hSlewRate -> Fill(slewRate); 

      // graphs
      gsample = new TGraphErrors();
      sprintf(gtitle,"shape_Run%d_Evt%d",runId,eventId);
      sprintf(gname,"shape_%d",ievt);
      gsample->SetName(gname);
      gsample->SetTitle(gtitle);
      
      for (int isample = 0; isample < 10 ; isample++)
      	{
      	  gsample->SetPoint(isample, double(isample)*25., A[ihit][isample]);      
      	  gsample->SetPointError(isample,0.,0.9);      
      	}


      // fit functions            
      //Ecal Shape with FLOATING phase TB shape
      TF1 *fpulseShape = new TF1("fpulseShape", pulseShape,0.,240.,3);
      fpulseShape->SetLineColor(4);
      fpulseShape->SetParName(0,"Ped");
      fpulseShape->SetParName(1,"A");
      fpulseShape->SetParName(2,"T0");
      fpulseShape->SetParameter(0,0); // ped
      fpulseShape->SetParameter(1,A[ihit][imax]); // ampl
      fpulseShape->SetParameter(2,imax*25.); // T0
      gsample->RemovePoint(imax-1);
      gsample->Fit("fpulseShape","QSR+") ;
      gsample->SetPoint(imax-1,(imax-1)*25.,A[ihit][imax-1]);
      gsample->SetPoint(imax,(imax)*25.,A[ihit][imax]);
      ievt++;
      
      if (ievt < 100) {
	gsample->Write();
      }

      float measuredAmpl = A[ihit][imax-1] ;
      float expectedAmpl = A[ihit][imax] * fpulseShape->Eval((imax-1)*25.)/fpulseShape->Eval(imax*25.);
      //cout << fpulseShape->Eval(imax*25.) << "   " << fpulseShape->Eval(imax*25.-25.) << endl;
      //cout  << "Measured amplitude = " << measuredAmpl << "     Expected amplitude = " << expectedAmpl << endl ;
      //cout << endl;
      hAmplitude ->Fill(expectedAmpl,measuredAmpl);
      
        


    }// end loop over recHits
    

    
  }//end loop over events
 
 
  chain->Delete();

  saving.cd () ;

  hSlewRate  -> Write();
  hAmplitude ->Write();

  saving.Close () ;
  
  return 0;
  
  
}




