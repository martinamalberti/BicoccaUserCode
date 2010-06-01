// per compilare  c++ `root-config --libs --cflags --glibs` NewThinking.BKP.cc -o NewThinking_bkp

//shapeFunctions
#include "EcalAnalysis/SpikeStudies/bin/ShapeFunctions.cc"
#include "EcalAnalysis/SpikeStudies/interface/EcalTreeContent.h"

//CMSSW utils
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"

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

  TApplication* theApp = new TApplication("example",&argc, argv);

  // load ntuple
  TChain *chain = new TChain ("myanalysis/EcalAnalysisTree","EcalAnalysisTree") ;
  //FIXME  EcalTreeContent treeVars;
  //  setBranchAddresses(chain, treeVars);  
  chain->Add("/gwpool/users/deguio/NTUPLES/SpikesCommissioning2010_GOODCOLL_May6_MERGED.root");   //dal run 134520: 7TeV phase 2

  int nEntries = (int)chain->GetEntries() ;
  cout << "FOUND " << nEntries  << " ENTRIES\n" << endl;;
  
 
  //ecalVariables variables
  unsigned int runId;
  unsigned int eventId;
  unsigned int eventNaiveId;
 
  int nEcalRecHits;
  float ecalRecHitType[100];
  float ecalRecHitEnergy[100];
  float ecalRecHitIEta[100];
  float ecalRecHitIPhi[100];
  float ecalRecHitTime[100];
  int ecalRecHitRawId[100];
  float ecalRecHitChi2[100];
  float ecalRecHitOutOfTimeChi2[100];
  int ecalRecHitRecoFlag[100];
  float ecalRecHitMatrixFlag[100][5][5];
  float ecalRecHitMatrix[100][5][5];
  float ecalRecHitR9[100];
  float ecalRecHitS4oS1[100];
  int ecalDigis[100][10];
  int ecalGainId[100][10];
  int techL1Bit[64];
  int algoL1Bit[128];

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
  chain -> SetBranchAddress("ecalRecHitRawId",           ecalRecHitRawId);
  chain -> SetBranchAddress("ecalRecHitChi2",            ecalRecHitChi2);
  chain -> SetBranchAddress("ecalRecHitOutOfTimeChi2",   ecalRecHitOutOfTimeChi2);
  chain -> SetBranchAddress("ecalRecHitRecoFlag",        ecalRecHitRecoFlag);
  chain -> SetBranchAddress("ecalRecHitMatrixFlag",      ecalRecHitMatrixFlag);
  chain -> SetBranchAddress("ecalRecHitMatrix",          ecalRecHitMatrix);
  chain -> SetBranchAddress("ecalRecHitR9",              ecalRecHitR9);
  chain -> SetBranchAddress("ecalRecHitS4oS1",           ecalRecHitS4oS1);
  chain -> SetBranchAddress("ecalDigis",                 ecalDigis);
  chain -> SetBranchAddress("ecalGainId",                ecalGainId);
  chain -> SetBranchAddress("techL1Bit",                 techL1Bit);
  chain -> SetBranchAddress("algoL1Bit",                 algoL1Bit);

  
  char outfileName[100];
  sprintf(outfileName,"6May_fase_0ns.root");
  
  TFile* saving = new TFile (outfileName,"recreate") ;
  saving->mkdir("shapes");
  saving->cd ("shapes") ;

  TGraphErrors* gsample;

  char gname[100];
  char gnameCanvas[100];
  char gtitle[100];
  int ievt=0;  
  float EcalPhase;
  float tau = 43.;

  TH1F* spikeTime = new TH1F("spikeTime","spikeTime",100,-20.,20.);
  TH1F* goodTime = new TH1F("goodTime","goodTime",100,-20.,20);
  TH1F* spikeR9_goodDisc_Time = new TH1F("spikeR9_goodDisc_Time","spikeR9_goodDisc_Time",100,-20.,20);
  TH1F* goodR9_spikeDisc_Time = new TH1F("goodR9_spikeDisc_Time","goodR9_spikeDisc_Time",100,-20.,20);

  TH1F* deltaChi2 = new TH1F("deltaChi2","deltaChi2", 500, -50., 50.);
  TH1F* L_disc_spi = new TH1F("L_disc_spi","L_disc_spi", 200,0.,1.1);
  TH1F* L_disc_sci = new TH1F("L_disc_sci","L_disc_sci", 200,0.,1.1);

  TH2F* L_disc_vsTime_sci = new TH2F("L_disc_vsTime_sci","L_disc_vsTime_sci", 500,-50.,50.,110,0.,1.1);
  TH2F* L_disc_vsR9 = new TH2F("L_disc_vsR9","L_disc_vsR9", 110,0.,1.1,200,-0.5,1.5);
  TH2F* L_disc_vsTime_spi = new TH2F("L_disc_vsTime_spi","L_disc_vsTime_spi", 500,-50.,50.,110,0.,1.1);

  TH1F* sigmaIEtaIEtaDistr = new TH1F("sigmaIEtaIEtaDistr","sigmaIEtaIEtaDistr",10000.,0.,1000.);
  
  TGraph* eff = new TGraph;

  TGraph* effChi2;
  TGraph* effChi2OutOfTime;

  int nSpiTot = 0;
  int nSciTot = 0;
  int nSpiLdiscCut[100];
  int nSciLdiscCut[100];
  int nSpiChi2Cut[100];
  int nSciChi2Cut[100];
  int nSpiChi2OutOfTimeCut[100];
  int nSciChi2OutOfTimeCut[100];
  
  for (int count = 0; count < 100; ++count)
    {
      nSpiLdiscCut[count] = 0;
      nSciLdiscCut[count] = 0;
      nSpiChi2Cut[count] = 0;
      nSciChi2Cut[count] = 0;
      nSpiChi2OutOfTimeCut[count] = 0;
      nSciChi2OutOfTimeCut[count] = 0;
    }
  

  //loop over the events
  //for (int entry = 0 ; entry < nEntries; ++entry) {
  for (int entry = 0 ; entry < 150000; ++entry) {
    
    chain->GetEntry(entry) ;
    if (entry%10000==0) cout << "    Analyzing entry " << entry << endl;
    
    //only 0ns phase
    if (runId > 134520) continue;
    
    for (int ihit =0 ; ihit < nEcalRecHits; ihit++)
      {
	if (ecalRecHitEnergy[ihit] < 3. ) continue;      // E>3 GeV
	//if (ecalRecHitRecoFlag[ihit] == 2) continue;     // not outOfTime
	//if (fabs(ecalRecHitTime[ihit]) > 4.) continue;     // not outOfTime
	if (ecalRecHitType[ihit] != 0) continue;         // solo barrel
	
	bool goodcluster = true;  //tutti i cristalli intorno accesi e buoni!
	double e_5x5 = 0.;
	double meanDEta = 0;
	double meanDPhi = 0;
	for (int xx = 0; xx < 5; ++xx)
	  {
	    for (int yy = 0; yy < 5; ++yy)
	      {
		if((ecalRecHitMatrixFlag[ihit][xx][yy] > 2 && ecalRecHitMatrixFlag[ihit][xx][yy] != 10) || ecalRecHitMatrixFlag[ihit][xx][yy] == -9999 )
		  { 
		    goodcluster = false;
		    break;
		  }
		e_5x5 += ecalRecHitMatrix[ihit][xx][yy];
		meanDEta += ecalRecHitMatrix[ihit][xx][yy] * fabs(xx - 2);
		meanDPhi += ecalRecHitMatrix[ihit][xx][yy] * fabs(yy - 2);
	      }
	    if(goodcluster == false)
	      break;
	  }

	meanDEta /= e_5x5;
	meanDPhi /= e_5x5;
	
	// check gain switch
	bool gainSwitch = false;
	for (int isample = 0; isample < 10 ; isample++){
	  if ((isample > 0 && ecalGainId[ihit][isample] != ecalGainId[ihit][isample-1]) || ecalGainId[ihit][isample] !=1 ) 
	    {
	      gainSwitch = true;
	      break;
	    }
	}
	
	// select on beam activity and gain and goodquality
	if ((techL1Bit[40]+techL1Bit[41])==0) continue;
	if (gainSwitch) continue;    // reject pulses beyond G12
	if (goodcluster == false) continue;

	//if all good -> cluster shape eval	
	double numeratorIEtaIEta = 0;
	double numeratorIEtaIPhi = 0;
	double numeratorIPhiIPhi = 0;
	double denominator     = 0;
	for (int xx = 0; xx < 5; ++xx)
	  for (int yy = 0; yy < 5; ++yy)
	    {
	      double dEta = fabs(xx - 2) - meanDEta;

	      //double w = std::max(0.0, w0 + log(ecalRecHitMatrix[ihit][xx][yy] / e_5x5));
	      double w = std::max(0.0, log(ecalRecHitMatrix[ihit][xx][yy] / e_5x5));

	      denominator += w;
	      numeratorIEtaIEta += w * dEta*dEta;
	    }

	double covIEtaIEta = 0.01745*0.01745 * numeratorIEtaIEta / denominator;
	double sigmaIEtaIEta = sqrt(covIEtaIEta);

	sigmaIEtaIEtaDistr->Fill(sigmaIEtaIEta);

	// graphs
	gsample = new TGraphErrors();
	sprintf(gtitle,"shape_Run%d_Evt%d",runId,eventId );
	sprintf(gname,"shape_%d",ievt);
	gsample->SetName(gname);
	gsample->SetTitle(gtitle);
	
	float ped = (ecalDigis[ihit][0] + ecalDigis[ihit][1] + ecalDigis[ihit][2])/3;
	
	int imax = 0;
	int maxAmpl = 0;
	int ithr = 0;
	for (int isample = 0; isample < 10 ; isample++)
	  {
	    // cerca il primo sample sopra soglia per definire il range di FIT
	    if (ecalDigis[ihit][isample] >= ped+7.)
	      ithr = isample;
	    
	    if (ecalDigis[ihit][isample] >= maxAmpl) 
	      {
		imax = isample;
		maxAmpl = ecalDigis[ihit][isample] ;
	      }
	    gsample->SetPoint(isample, double(isample)*25.,ecalDigis[ihit][isample]);      
	    gsample->SetPointError(isample,0.,0.9);      
	  }
	
	
	//tengo solo se il primo sopra soglia e' il quarto o il quinto
	//if (ithr > 4 || ithr < 3) continue;
	
	// fit functions	    
	//Ecal Shape with FLOATING phase TB shape
	//TF1 *fpulseShape = new TF1("fpulseShape",pulseShape,ithr*25-5.,240.,3);
	TF1 *fpulseShape = new TF1("fpulseShape",pulseShape,0.,240.,3);
	fpulseShape->SetLineColor(3);
	if (ecalRecHitS4oS1[ihit]>0.95) fpulseShape->SetLineColor(2);
	fpulseShape->SetParName(0,"Ped");
	fpulseShape->SetParName(1,"A");
	fpulseShape->SetParName(2,"T0");
	fpulseShape->FixParameter(0,ped); // ped
	fpulseShape->SetParameter(1,maxAmpl - ped); // ampl
	fpulseShape->SetParameter(2,imax*25.); // T0
	//fpulseShape->SetParLimits(2,2*25., 7*25.); // T0 lim
	fpulseShape->SetParLimits(2,0*25., 10*25.); // T0 lim
	gsample->Fit("fpulseShape","QSR+") ;
	
	// electonic pulse
	TF1 *fSinglePulse = new TF1("fSinglePulse",abnormalShape,0.,240.,4);
	//TF1 *fSinglePulse = new TF1("fSinglePulse",abnormalShape,ithr*25-5.,240.,4);
	//TF1 *fSinglePulse = new TF1("fSinglePulse",spikeShape,ithr*25-5.,230.,4);  //marti - alessio
	fSinglePulse->SetLineColor(4);
	fSinglePulse->SetParName(0,"Ped");
	fSinglePulse->SetParName(1,"A");
	fSinglePulse->SetParName(2,"T0");
	fSinglePulse->SetParName(3,"tau");
	fSinglePulse->FixParameter(0,ped); // ped
	fSinglePulse->SetParameter(1,maxAmpl - ped);  // amp
	fSinglePulse->SetParameter(2,imax*25.- tau); // T0
	fSinglePulse->FixParameter(3,tau); // tau  = 40.5
	gsample->Fit("fSinglePulse","QSR+")    ;
	
	//minimo taglio di qualita' sul fit
	float bloodi_sci = TMath::Prob(fpulseShape->GetChisquare(),fpulseShape->GetNDF());
	float bloodi_spi = TMath::Prob(fSinglePulse->GetChisquare(),fSinglePulse->GetNDF());
	
	if(bloodi_sci <= 0.00 && bloodi_spi <= 0.00) continue;
	
	//provo a discriminare
	//chi2
	float chi2sci = fpulseShape->GetChisquare()/fpulseShape->GetNDF();
	float chi2spi = fSinglePulse->GetChisquare()/fSinglePulse->GetNDF();
	float deltachi2 = chi2spi-chi2sci; //if deltaChi2<0 -> spike
	
	
	
	//likelihood
	float Ldisc = 1./(1.+exp(-deltachi2/2.));
	
	if(bloodi_sci <= 0.00) Ldisc = -0.2;
	if(bloodi_spi <= 0.00) Ldisc = 1.2;
	
	
	deltaChi2->Fill(deltachi2);
	L_disc_vsR9->Fill(ecalRecHitR9[ihit],Ldisc);
	
	
	if(ecalRecHitS4oS1[ihit] > 0.95) 
	  {
	    ++nSpiTot;
	    L_disc_spi->Fill(Ldisc);
	    L_disc_vsTime_spi->Fill(ecalRecHitTime[ihit],Ldisc);
	  }
	else if (ecalRecHitS4oS1[ihit] < 0.80)
	  {
	    if (fabs(ecalRecHitTime[ihit]) > 4.) continue;     // not outOfTime FIXME
	    ++nSciTot;
	    L_disc_sci->Fill(Ldisc);
	    L_disc_vsTime_sci->Fill(ecalRecHitTime[ihit],Ldisc);
	  }
	
	for (int nn = 0; nn < 100; ++nn)
	  {
	    float discCut = (float)(nn+1)/100.;
	    if(Ldisc > discCut && ecalRecHitS4oS1[ihit] > 0.95) ++nSpiLdiscCut[nn];
	    if(Ldisc > discCut && ecalRecHitS4oS1[ihit] < 0.80) ++nSciLdiscCut[nn];
	    
	    float chi2Cut = (float)(nn+1) * 65/100.;
	    
	    if(ecalRecHitChi2[ihit] < chi2Cut && ecalRecHitS4oS1[ihit] > 0.95) ++nSpiChi2Cut[nn];
	    if(ecalRecHitOutOfTimeChi2[ihit] < chi2Cut && ecalRecHitS4oS1[ihit] > 0.95) ++nSpiChi2OutOfTimeCut[nn];
	    
	    
	    if(ecalRecHitChi2[ihit] < chi2Cut && ecalRecHitS4oS1[ihit] < 0.80) ++nSciChi2Cut[nn];
	    if(ecalRecHitOutOfTimeChi2[ihit] < chi2Cut && ecalRecHitS4oS1[ihit] > 0.80) ++nSciChi2OutOfTimeCut[nn];
	  }
	
	
	ievt++;
	
	delete gsample;
	delete fpulseShape;
	delete fSinglePulse;
	
      }//recHits
  }//event
  effChi2 = new TGraph();
  effChi2OutOfTime = new TGraph();
  
  //eff plots
  for (int nn = 0; nn < 100; ++nn)
    {
      eff->SetPoint(nn, (float)nSciLdiscCut[nn]/(float)nSciTot, 1. - (float)nSpiLdiscCut[nn]/(float)nSpiTot);
      
      effChi2->SetPoint(nn, (float)nSciChi2Cut[nn]/(float)nSciTot, 1. - (float)nSpiChi2Cut[nn]/(float)nSpiTot);
      effChi2OutOfTime->SetPoint(nn, (float)nSciChi2OutOfTimeCut[nn]/(float)nSciTot, 1. - (float)nSpiChi2OutOfTimeCut[nn]/(float)nSpiTot);
      
    }
  
  TCanvas* quadranti = new TCanvas("quadranti","quadranti");
  quadranti->Divide(2,2);
  
  quadranti->cd(1);
  goodTime->GetXaxis()->SetTitle("Time (ns)");
  goodTime->Draw();

  quadranti->cd(2);
  spikeR9_goodDisc_Time->GetXaxis()->SetTitle("Time (ns)");
  spikeR9_goodDisc_Time->Draw();

  quadranti->cd(3);
  goodR9_spikeDisc_Time->GetXaxis()->SetTitle("Time (ns)");
  goodR9_spikeDisc_Time->Draw();

  quadranti->cd(4);
  spikeTime->GetXaxis()->SetTitle("Time (ns)");
  spikeTime->Draw();

  goodTime->Write();
  spikeR9_goodDisc_Time->Write();
  goodR9_spikeDisc_Time->Write();
  spikeTime->Write();

  TCanvas* c1 = new TCanvas("L_disc_vsR9","L_disc_vsR9");
  L_disc_vsR9->GetXaxis()->SetTitle("R9");
  L_disc_vsR9->GetYaxis()->SetTitle("disc = #frac{1}{1 + L1/L2}");
  L_disc_vsR9->Draw("colz");

  TCanvas* c2 = new TCanvas("L_disc","L_disc");
  c2->SetLogy();
  c2->SetGridy();
  L_disc_spi->GetXaxis()->SetTitle("disc = #frac{1}{1 + L1/L2}");
  L_disc_spi->SetLineColor(2);
  //L_disc_sci->SetFillColor(2);  
  L_disc_spi->DrawNormalized();
  L_disc_sci->SetLineColor(3);
  //L_disc_sci->SetFillColor(3);  
  L_disc_sci->DrawNormalized("same");

  deltaChi2->Write();
  L_disc_vsR9->Write();
  L_disc_spi->Write();
  L_disc_sci->Write();


  saving->cd();
  effChi2->Write();
  effChi2OutOfTime->Write();

  sigmaIEtaIEtaDistr->Draw();
  sigmaIEtaIEtaDistr->Write();

  saving->Close () ;
  theApp->Run();
}



