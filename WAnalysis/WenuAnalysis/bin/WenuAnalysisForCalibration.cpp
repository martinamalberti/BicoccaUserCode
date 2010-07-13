#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

//---- CMSSW includes
#include "DataFormats/Math/interface/LorentzVector.h"
#include "WAnalysis/WenuAnalysis/interface/WenuTreeContent.h"

//---- root includes
#include "TH1.h"
#include "TH2.h"

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#define PI 3.14159265
#define TWOPI 6.28318530

using namespace std;

float deltaPhi(float phi1,float phi2) {
 float deltaphi = fabs(phi1-phi2);
 if (deltaphi > TWOPI) deltaphi -= TWOPI;
 if (deltaphi > PI ) deltaphi = TWOPI - deltaphi;
 return deltaphi;
}

//! main program
int main (int argc, char** argv)
{
 
  int ff = atoi(argv[1]);
  

  // load the tree
  TChain *chain = new TChain ("myanalysis/WprimeAnalysisTree") ;
  WenuTreeContent treeVars ;
  setBranchAddresses (chain, treeVars) ;
  
  if ( ff == 0 ){
    chain->Add("/tmp/malberti/treeWenuMC.root");
  }

  if (ff == 1){
    chain->Add("/tmp/malberti/treeWenu_Commissioning10-SD_EG-v9.root");
    chain->Add("/tmp/malberti/treeWenu_EG-Run2010A-PromptReco-v1.root");
    chain->Add("/tmp/malberti/treeWenu_EG-Run2010A-PromptReco-v2.root");
  }

  int nEntries = chain->GetEntries () ;
  std::cout << "FOUND " << nEntries << " ENTRIES\n" ;
  
  double w = 1.;
  
 

  //INITIALIZING HISTOGRAMS
  TH1F *hNGoodElectrons = new TH1F("hNGoodElectrons","hNGoodElectrons",5,0,5);
 
  TH1F *hSumEt = new TH1F("hSumEt","hSumEt",500,0,500);
  TH1F *hmet_o_SumEt = new TH1F("hmet_o_SumEt","hmet_o_SumEt",100,0,10);
  TH2F *hmet_vs_metOverSumEt = new TH2F("hmet_vs_metOverSumEt","hmet_vs_metOverSumEt",100,0,10,100,0,100);

  TH1F *het    = new TH1F("het","het",25,2,102);
  TH1F *hetEB  = new TH1F("hetEB","hetEB",25,2,102);
  TH1F *hetEE  = new TH1F("hetEE","hetEE",25,2,102);

  TH1F *hmet   = new TH1F("hmet","hmet",25,2,102);
  TH1F *hmetEB = new TH1F("hmetEB","hmetEB",25,2,102);
  TH1F *hmetEE = new TH1F("hmetEE","hmetEE",25,2,102);
 
  TH1F *hmt    = new TH1F("hmt","hmt",50,0,200);
  TH1F *hmtEB  = new TH1F("hmtEB","hmtEB",50,0,200);
  TH1F *hmtEE  = new TH1F("hmtEE","hmtEE",50,0,200);

  TH1F *heop   = new TH1F("heop","heop",50,0,2);
  TH1F *heopEB = new TH1F("heopEB","heopEB",50,0,2);
  TH1F *heopEE = new TH1F("heopEE","heopEE",50,0,2);

  TH1F *hetscaled[200];
  TH1F *hetEBscaled[200];
  TH1F *hetEEscaled[200];
  TH1F *hmtEBscaled[200];
  TH1F *hmtEEscaled[200];
  TH1F *heopEBscaled[200];
  TH1F *heopEEscaled[200];

  char hname[100];
  for (int i = 0; i < 200 ; i++){
  
    sprintf(hname,"hetscaled_%d",i);
    hetscaled[i] = new TH1F(hname,hname,25,2,102);

    sprintf(hname,"hetEBscaled_%d",i);
    hetEBscaled[i] = new TH1F(hname,hname,25,2,102);

    sprintf(hname,"hetEEscaled_%d",i);
    hetEEscaled[i] = new TH1F(hname,hname,25,2,102);

    sprintf(hname,"hmtEBscaled_%d",i);
    hmtEBscaled[i] = new TH1F(hname,hname,50,0,200);

    sprintf(hname,"hmtEEscaled_%d",i);
    hmtEEscaled[i] = new TH1F(hname,hname,50,0,200);

    sprintf(hname,"heopEBscaled_%d",i);
    heopEBscaled[i] = new TH1F(hname,hname,50,0,2);

    sprintf(hname,"heopEEscaled_%d",i);
    heopEEscaled[i] = new TH1F(hname,hname,50,0,2);
  
  }

//   float EtaCutEB    = 1.4442;
//   float EtaCutEE    = 1.560;
  float EtaCutEB    = 1.5;
  float EtaCutEE    = 1.5;
  float EtaMax      = 3.0;

  int nWele = 0;


  // loop over entries
  for (int entry = 0; entry < nEntries; ++entry) {
    
    chain->GetEntry (entry) ;
    if(entry%100000 == 0) std::cout << "event n. " << entry << std::endl;
       
    // HLT selection
    //if (treeVars.HLT_Photon10_L1R==0) continue;
   
        
    int nGoodElectrons = 0;
    int chosenEle = 0;

    //start loop over electron candidates
    for (int i=0; i<treeVars.nElectrons; i++){
     
      //float et  = treeVars.eleEt[i] ;
      float eta = treeVars.eleEta[i] ;
      
      // select electrons in ECAL fiducial volume
      if ( fabs(eta)> EtaCutEB && fabs(eta)< EtaCutEE ) continue;
      if ( fabs(eta)> EtaMax ) continue;
      
      
      float pt = sqrt(treeVars.elePx[i]*treeVars.elePx[i]+
		       treeVars.elePy[i]*treeVars.elePy[i]);
      float p  = sqrt(treeVars.elePx[i]*treeVars.elePx[i]+
		       treeVars.elePy[i]*treeVars.elePy[i]+
		       treeVars.elePz[i]*treeVars.elePz[i]);
      
      float et = treeVars.eleEt[i];
      
      float met  = treeVars.caloMet;
            
      float cphi = (treeVars.elePx[i]*treeVars.caloMex 
		     + treeVars.elePy[i]*treeVars.caloMey )/(met*pt);
      
      float mt   = sqrt(2*et*met*(1-cphi));
      
      float dphi = deltaPhi(treeVars.caloMetPhi,treeVars.elePhi[i]);
      
      float sumEt = 0;
      for (int j=0; j<treeVars.nJets; j++){
	float deta = treeVars.jetEta[j] -  treeVars.eleEta[i];
	float dphi = treeVars.jetPhi[j] -  treeVars.elePhi[i];
	float dR = sqrt(deta*deta+dphi*dphi);
	if ( dR < 0.4 && treeVars.jetPt[j]>15) continue;
	sumEt+=treeVars.jetPt[j];
      }

      hmet_vs_metOverSumEt ->Fill(met/sumEt,met);
      hSumEt->Fill(sumEt,w);
      hmet_o_SumEt->Fill(met/sumEt,w);

 //      if ( et < 18.)              continue ;
//       if ( treeVars.eleId[i] != 15)              continue ;

//       cout << "Run : " << treeVars.runId  
// 	   << "  lumi : " << treeVars.lumiId   
// 	   << "  evt  : " << treeVars.eventId 
// 	   << "  eleEt : " << et 
// 	   << "  met : " <<  treeVars.caloMet
// 	   << "  sumEt : " <<  sumEt
// 	   << "  met/sumEt : " << (met/sumEt) 
// 	   << "  dphi : " << dphi 
	 
// 	   << endl;

      // apply selections : Jim
      //if ( et < 18.)              continue ;
      if ( met < 18. )           continue;
      if ( dphi < 0.75)          continue;    
      if ( sumEt!=0 && (met/sumEt) < 0.5 )   continue;
      if ( met < (6 - 10.*(met/sumEt - 2))) continue;
      

      // electron ID
      if ( treeVars.eleId[i] != 15 ) continue;
            
      nGoodElectrons++;
      chosenEle = i;
      
    }// end loop over ele cand
    


    hNGoodElectrons->Fill( nGoodElectrons,w);
    //if ( nGoodElectrons != 1 ) continue;
    if ( nGoodElectrons == 0 ) continue;
    
    float pt = sqrt(treeVars.elePx[chosenEle]*treeVars.elePx[chosenEle]+
		     treeVars.elePy[chosenEle]*treeVars.elePy[chosenEle]);
    float p  = sqrt(treeVars.elePx[chosenEle]*treeVars.elePx[chosenEle]+
		     treeVars.elePy[chosenEle]*treeVars.elePy[chosenEle]+
		     treeVars.elePz[chosenEle]*treeVars.elePz[chosenEle]);
    
    float et = treeVars.eleEt[chosenEle];
    
    float met  = treeVars.caloMet;
    
    float cphi = (treeVars.elePx[chosenEle]*treeVars.caloMex 
		   + treeVars.elePy[chosenEle]*treeVars.caloMey )/(met*pt);

    float mt   = sqrt(2*et*met*(1-cphi));
    
    float dphi = deltaPhi(treeVars.caloMetPhi,treeVars.elePhi[chosenEle]);

    float sumEt = 0;
    for (int j=0; j<treeVars.nJets; j++){
      float deta = treeVars.jetEta[j] -  treeVars.eleEta[chosenEle];
      float dphi = treeVars.jetPhi[j] -  treeVars.elePhi[chosenEle];
      float dR = sqrt(deta*deta+dphi*dphi);
      if ( dR < 0.4 && treeVars.jetPt[j]>15) continue;
      sumEt+=treeVars.jetPt[j];
    }
    
    


    if ( et > 18.) {
      nWele++;
//       cout << "Run : " << treeVars.runId  
// 	   << "  lumi : " << treeVars.lumiId   
// 	   << "  evt  : " << treeVars.eventId 
// 	   << "  eleEt : " << et 
// 	   << "  eleId : " <<  treeVars.eleId[chosenEle] 
// 	   << "   eta : " << treeVars.eleEta[chosenEle] 
// 	   << "   phi : " << treeVars.elePhi[chosenEle] 
// 	   << endl;
      
      // fill histos EB+EE
      het  ->Fill(et,w);
      hmet ->Fill(met,w);
      hmt  ->Fill(mt,w);
      heop ->Fill(treeVars.eleE[chosenEle]/p ,w);

      // fill histos EB
      if (fabs(treeVars.eleEta[chosenEle]) < EtaCutEB){
	hetEB  -> Fill(et,w);
	hmtEB  -> Fill(mt,w);
	hmetEB -> Fill(met,w);
	heopEB ->Fill(treeVars.eleE[chosenEle]/p ,w);
      }

      // fill histos EE
      if (fabs(treeVars.eleEta[chosenEle]) > EtaCutEE && fabs(treeVars.eleEta[chosenEle]) < EtaMax ){
	hetEE  -> Fill(et,w);
	hmtEE  -> Fill(mt,w);
	hmetEE -> Fill(met,w);
	heopEE ->Fill(treeVars.eleE[chosenEle]/p ,w);
      }
    }
    
    

    // scaled distr.
    for (int i = 0; i< 200; i++){
      float alpha = 1. + 0.001*(i-100);
      // EB
      if (fabs(treeVars.eleEta[chosenEle]) < EtaCutEB && (alpha*et) > 18.){
	hetscaled[i]   ->Fill(alpha*et);
	hetEBscaled[i] ->Fill(alpha*et);
	hmtEBscaled[i] ->Fill(alpha*mt); 
	heopEBscaled[i]->Fill(alpha*treeVars.eleE[chosenEle]/p ,w);
      }
      
      // EE
      if ( fabs(treeVars.eleEta[chosenEle]) > EtaCutEE && fabs(treeVars.eleEta[chosenEle]) < EtaMax && (alpha*et) > 18.){
	hetscaled[i]   ->Fill(alpha*et);
	hetEEscaled[i] ->Fill(alpha*et);
	hmtEEscaled[i] ->Fill(alpha*mt); 
	heopEEscaled[i]->Fill(alpha*treeVars.eleE[chosenEle]/p ,w);
      }
      
    }
      
    

    
    
  } // end loop over entries
  
  cout << "Found " << nWele << " electrons from W" << endl;

  // FILE to save histos
  TFile *fout;
  if (ff == 0){
    fout  = new TFile("wenuHistos_MC.root","recreate");
  }

  if (ff == 1) {
    fout  = new TFile("wenuHistos_DATA.root","recreate");
  }

  fout->cd();

  hNGoodElectrons->Write();
  het    -> Write();
  hmet   -> Write();
  hmt    -> Write();
  hetEB  -> Write();
  hmetEB -> Write();
  hmtEB  -> Write();
  hetEE  -> Write();
  hmetEE -> Write();
  hmtEE  -> Write();
  
  heop   -> Write();
  heopEB -> Write();
  heopEE -> Write();

  
  hSumEt-> Write();
  hmet_o_SumEt->Write();
  hmet_vs_metOverSumEt ->Write();

  // if MC , save also scale d histograms
  if ( ff == 0 ){
    for (int i = 0; i < 200 ; i++){ 
      hetscaled[i]  ->Write();
      hetEBscaled[i]->Write();
      hetEEscaled[i]->Write();
      hmtEBscaled[i]->Write();
      hmtEEscaled[i]->Write();
      heopEBscaled[i]->Write();
      heopEEscaled[i]->Write();
    }
  }







}
