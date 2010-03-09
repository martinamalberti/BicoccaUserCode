#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TGraph.h>
#include <TH2F.h>

#include <iostream>
#include <iomanip>
#include <string.h>

#include <math.h>

#include "MuMuCutOptimizer.hh"

using namespace std;


MuMuCutOptimizer::MuMuCutOptimizer(TChain *tree) 
  : smallHBaseClass(tree) {

  /// SELECTION CUTS ///
 
  MIN_nhits_trk = 12;
  MAX_normchi2_trk = 5.0;
  MAX_normchi2_glb = 20.0;
  MIN_nhits_pixel = 2;
  MAX_d0_trk = 2.0;
  MAX_dz_trk = 25.0;
}

void MuMuCutOptimizer::Loop() {

  if (fChain == 0) return;  
  int nentries = (int)fChain->GetEntries(); 

  // loop over events
  cout << "Number of entries = " << nentries << endl;

  // counters
  int totalEvents = 0;
  int passedTriggers = 0;

  Float_t signal[100];
  Float_t backcont[100];
  Float_t ups1S[100];
  Float_t ups2S[100];
  Float_t ups3S[100];
  Float_t cut[100];
  Float_t cutgraph[100];
  Float_t sgraph[100];

  for(Int_t i=0; i<100; i++){
    signal[i]=0;
    backcont[i]=0;
    ups1S[i]=0;
    ups2S[i]=0;
    ups3S[i]=0;
    sgraph[i] = 0.;
    cut[i]=0;
    cutgraph[i] = 0.;
  }
  
  Float_t ssquared = 0.;
  Float_t signifi_max = 0.;
  Float_t cut_max = 0.;      
  
  for (int jentry=0; jentry< nentries; jentry++) {
    
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    fChain->GetEntry(jentry);

    ssquared = 0.;
    signifi_max = 0.;
    cut_max = 0.;      
    
   if (jentry%100000 == 0) cout << ">>> Processing event # " << jentry << endl;
    
    totalEvents++;

    // TRIGGER CUTS 
    if (!(*HLTBits_accept)[0]) continue;    // SingleMu3

    passedTriggers++;

    TString filestring(fChain->GetCurrentFile()->GetName());

    //FIXME to be defined

    if (filestring.Contains("_ppMuX"))
      weight = 4080.96;
    else if (filestring.Contains("Upsilon1S"))
      weight = 70;
    else if (filestring.Contains("Upsilon2S"))
      weight = 28;
    else if (filestring.Contains("Upsilon3S")) 
      weight = 8;
    else if (filestring.Contains("A0_5GEV"))
      weight = 0.166667;
    else if (filestring.Contains("A0_6GEV"))
      weight = 0.163265;
    else if (filestring.Contains("A0_7GEV"))
      weight = 0.172043;
    else if (filestring.Contains("A0_8GEV"))
      weight = 0.177419;
    else if (filestring.Contains("A0_9GEV"))
      weight = 0.239362;
    else if (filestring.Contains("A0_10GEV"))
      weight = 0.421348;
    else if (filestring.Contains("A0_105GEV"))
      weight = 0.252525;
    
    for (int iqq=0; iqq<(*QQ_size)[0]; iqq++) {

      if ((*QQ_sign)[iqq] != 0) continue;//change here for opposite sign !=, for same sign ==

      if((*QQ_type)[iqq] == 0) continue;

      int thehptMu = QQ_lephpt->at(iqq);   if (thehptMu >= muons_trk_normChi2->size()) continue;
      int thelptMu = QQ_leplpt->at(iqq);    if (thelptMu >= muons_trk_normChi2->size()) continue;
      
//       Float_t isoVarh = (muons_glb_tkIsoR03->at(thehptMu) + muons_glb_emIsoR03->at(thehptMu))/((TLorentzVector*)muons_glb_4mom->At(thehptMu))->Pt();
//       Float_t isoVarl = (muons_glb_tkIsoR03->at(thelptMu) + muons_glb_emIsoR03->at(thelptMu))/((TLorentzVector*)muons_glb_4mom->At(thelptMu))->Pt();

      TLorentzVector *mu_4momH = (TLorentzVector*)muons_trk_4mom->At(thehptMu);
      TLorentzVector *mu_4momL = (TLorentzVector*)muons_trk_4mom->At(thelptMu);   
   
      for(Int_t i=0; i<10; i++){
	//cut[i] = 3. + 0.02*i;
// 	cut[i] = 0.05 + 0.01*i;	//chi2
// 	if(QQ_probChi2->at(iqq) > cut[i]){

	//  if(isoVarh < cut[i] && isoVarl < cut[i]){

 	cut[i] = 0.5 + 0.05*i;	
 	if(QQ_S3Dip->at(iqq) < cut[i]){
// 	cut[i] = 1. + 0.1*i;
// 	if(mu_4momL->Pt() > cut[i] && mu_4momH->Pt() > cut[i]){
	  if(filestring.Contains("8GEV") ) signal[i]+=weight;// || filestring.Contains("10GEV") 
	  else if(filestring.Contains("ppMuX")) backcont[i]+=weight;
	  else if(filestring.Contains("Upsilon1S")) ups1S[i]+=weight;
	  else if(filestring.Contains("Upsilon2S")) ups2S[i]+=weight; 
	  else if(filestring.Contains("Upsilon3S")) ups3S[i]+=weight;
	  else signal[i]+=0;
	}
      }
    }
  }
  
  for(Int_t i=0; i<10; i++){
    ssquared = signal[i]/sqrt(signal[i]+backcont[i]+ups1S[i]+ups2S[i]+ups3S[i]);
    sgraph[i] = ssquared;
    if(ssquared > signifi_max){
      signifi_max = ssquared;
      cout << signifi_max << endl;
      cut_max = cut[i];
    }
  }  
  
  TGraph *gr1 = new TGraph(10,cut,sgraph);
  gr1->GetXaxis()->SetTitle("S3Dip cut");
  gr1->GetYaxis()->SetTitle("stat. signif."); 
  
  TCanvas tmp1;
  tmp1.cd();
  gr1->Draw("AC*");
  tmp1.Print("statsigcutS3DipZoom.gif");
  
  
  cout << "###############" << endl;
  cout << "Some statistics " << endl;
  cout << "Total number of processed events = " << totalEvents << endl;
  cout << "Total number of triggered events = " << passedTriggers << endl;
  cout << "###############" << endl;

  cout << "######### Optimization cuts ########" << endl;
  cout << "The best cut is " << cut_max << endl;

  cout << "Statistical significance is " << signifi_max << endl;

  return;
  } // end of program

