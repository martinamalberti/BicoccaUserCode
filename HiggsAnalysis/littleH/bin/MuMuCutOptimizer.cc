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
  MIN_PtTk = 3.;
  MIN_Pt = 0.;
  MIN_P = 0.;
  MAX_iso = 10.;
  MIN_Chi2 = 0.;
  n_step = 40;
  n_signal = 7;
}

void MuMuCutOptimizer::Loop(TString optVar) {

  if (fChain == 0) return;  
  int nentries = (int)fChain->GetEntries(); 

  // loop over events
  cout << "Number of entries = " << nentries << endl;

  // counters
  int totalEvents = 0;
  int passedTriggers = 0;
  
  Float_t signal[n_signal][n_step];
  Float_t m[n_signal];
  Float_t backcont[n_step];
  Float_t ups1S[n_step];
  Float_t ups2S[n_step];
  Float_t ups3S[n_step];
  Float_t cut[n_step];
  Float_t sgraph[n_signal][n_step];
  Float_t signifi_max[n_signal];
  Float_t cut_max[n_signal];
  Float_t ssquared[n_signal];  
 
  for(Int_t i=0; i<n_step; i++){
    for(Int_t j=0; j<n_signal; j++){
      m[j]=j+5;
      signal[j][i]=0;
      sgraph[j][i] = 0.;
      signifi_max[j]=0;
      cut_max[j]=0;
      ssquared[j]=0;
    }
    backcont[i]=0;
    ups1S[i]=0;
    ups2S[i]=0;
    ups3S[i]=0;
    cut[i]=0;
  }
  
  for (int jentry=0; jentry< nentries; jentry++) {
    
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    fChain->GetEntry(jentry);
    
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
    else if (filestring.Contains("A0_10.5GEV"))
      weight = 0.252525;
    
    for (int iqq=0; iqq<(*QQ_size)[0]; iqq++) {

      if ((*QQ_sign)[iqq] != 0) continue;

      if((*QQ_type)[iqq] == 0) continue;

      int thehptMu;
      int thelptMu;

      Float_t isoVarh; 
      Float_t isoVarl; 

      TLorentzVector *mu_4momH;
      TLorentzVector *mu_4momL;

      if((*QQ_type)[iqq] == 1){//GLB-GLB
	thehptMu = QQ_lephpt->at(iqq);   if (thehptMu >= muons_glb_normChi2->size()) continue;
	thelptMu = QQ_leplpt->at(iqq);   if (thelptMu >= muons_glb_normChi2->size()) continue;
	
	mu_4momH = (TLorentzVector*)muons_glb_4mom->At(thehptMu);
	mu_4momL = (TLorentzVector*)muons_glb_4mom->At(thelptMu);   

	isoVarh = (muons_glb_tkIsoR03->at(thehptMu) + muons_glb_emIsoR03->at(thehptMu))/((TLorentzVector*)muons_glb_4mom->At(thehptMu))->Pt();
	isoVarl = (muons_glb_tkIsoR03->at(thelptMu) + muons_glb_emIsoR03->at(thelptMu))/((TLorentzVector*)muons_glb_4mom->At(thelptMu))->Pt();      

	if(!(accept_glb_mu(thehptMu) && accept_glb_mu(thelptMu))) continue;	
      }

      if((*QQ_type)[iqq] == 2){//GLB-TRK
	
	thehptMu = QQ_lephpt->at(iqq);   if (thehptMu >= muons_glb_normChi2->size()) continue;
	thelptMu = QQ_leplpt->at(iqq);   if (thelptMu >= muons_trk_normChi2->size()) continue;
	
	mu_4momH = (TLorentzVector*)muons_glb_4mom->At(thehptMu);
	mu_4momL = (TLorentzVector*)muons_trk_4mom->At(thelptMu); 

	isoVarh = (muons_glb_tkIsoR03->at(thehptMu) + muons_glb_emIsoR03->at(thehptMu))/((TLorentzVector*)muons_glb_4mom->At(thehptMu))->Pt();
	isoVarl = (muons_trk_tkIsoR03->at(thelptMu) + muons_trk_emIsoR03->at(thelptMu))/((TLorentzVector*)muons_trk_4mom->At(thelptMu))->Pt();    

	if(!(accept_glb_mu(thehptMu) && accept_trk_mu(thelptMu))) continue;
      }
      
      if( fabs(mu_4momL->Eta()) > 2.4 && fabs(mu_4momH->Eta()) > 2.4 ) continue;     

      for(Int_t i=0; i<n_step; i++){

	if(optVar.Contains("PtTk") && (*QQ_type)[iqq] == 2 ){
	  cut[i] = 1. + 0.1*i;
	  if(!(mu_4momL->Pt() > cut[i])) continue;
 	}
	else if( optVar.Contains("PtGlb") && (*QQ_type)[iqq] == 2){
	  cut[i] = 1. + 0.1*i;
	  if(!(mu_4momH->Pt() > cut[i] && mu_4momL->Pt() > MIN_PtTk)) continue;
	}
	else if( optVar.Contains("PtGlb") && (*QQ_type)[iqq] == 1){
	  cut[i] = 1. + 0.1*i;
	  if(!(mu_4momL->Pt() > cut[i] && mu_4momH->Pt() > cut[i])) continue;
	}
 
	else if (optVar.Contains("PfwdTk")){
	  if(!(mu_4momL->Pt() > MIN_Pt && mu_4momH->Pt() > MIN_Pt)) continue;//previous cut implementation
	  cut[i] = 3. + 0.02*i;
	  if(!(mu_4momL->Rho() > cut[i] && fabs(mu_4momL->Eta()) > 1.1 && mu_4momH->Rho() > cut[i] && fabs(mu_4momH->Eta()) > 1.1)) continue;
	}
	else if (optVar.Contains("Iso")){
	  if(!(mu_4momL->Pt() > MIN_Pt && mu_4momH->Pt() > MIN_Pt &&
	       (mu_4momL->Rho() > MIN_P && fabs(mu_4momL->Eta()) > 1.1 && mu_4momH->Rho() > MIN_P && fabs(mu_4momH->Eta()) > 1.1)) ) continue;//previous cut implementation
	  cut[i] = 0.05 + 0.01*i;
	  if(!(isoVarh < cut[i] && isoVarl < cut[i])) continue;
	}
	else if(optVar.Contains("Chi2")){
	  if(!(mu_4momL->Pt() > MIN_Pt && mu_4momH->Pt() > MIN_Pt &&
	       (mu_4momL->Rho() > MIN_P && fabs(mu_4momL->Eta()) > 1.1 && mu_4momH->Rho() > MIN_P && fabs(mu_4momH->Eta()) > 1.1) &&
	       (isoVarh < MAX_iso && isoVarl < MAX_iso)) ) continue;//previous cut implementation
	  cut[i] = 0.05 + 0.01*i;	
	  if(!(QQ_probChi2->at(iqq) > cut[i])) continue;
	}
	else if(optVar.Contains("S3Dip")){
	  if(!(mu_4momL->Pt() > MIN_Pt && mu_4momH->Pt() > MIN_Pt &&
	       (mu_4momL->Rho() > MIN_P && fabs(mu_4momL->Eta()) > 1.1 && mu_4momH->Rho() > MIN_P && fabs(mu_4momH->Eta()) > 1.1) &&
	       (isoVarh < MAX_iso && isoVarl < MAX_iso)) &&
	     (QQ_probChi2->at(iqq) > MIN_Chi2 )) continue;//previous cut implementation	
	  cut[i] = 0.5 + 0.05*i;	
	  if(!(QQ_S3Dip->at(iqq) < cut[i])) continue;
	}

	if(filestring.Contains("5GEV") ) signal[0][i]+=weight;
	if(filestring.Contains("6GEV") ) signal[1][i]+=weight;
	if(filestring.Contains("7GEV") ) signal[2][i]+=weight;
	if(filestring.Contains("8GEV") ) signal[3][i]+=weight;
	if(filestring.Contains("9GEV") ) signal[4][i]+=weight;
	if(filestring.Contains("10GEV") ) signal[5][i]+=weight;
	if(filestring.Contains("10.5GEV") ) signal[6][i]+=weight;

	else if(filestring.Contains("ppMuX")) backcont[i]+=weight;
	else if(filestring.Contains("Upsilon1S")) ups1S[i]+=weight;
	else if(filestring.Contains("Upsilon2S")) ups2S[i]+=weight; 
	else if(filestring.Contains("Upsilon3S")) ups3S[i]+=weight;
	
      }
    }
  }
  
  for(Int_t i=0; i<n_step; i++){
    for(Int_t j=0; j<n_signal; j++){
      ssquared[j] = signal[j][i]/sqrt(signal[j][i]+backcont[i]+ups1S[i]+ups2S[i]+ups3S[i]);
      sgraph[j][i] = ssquared[j];
      if(ssquared[j] > signifi_max[j]){
	signifi_max[j] = ssquared[j];
	cout << signifi_max[j] << " " << j << endl;
	cut_max[j] = cut[i];
      }
    }
  }  
  
  TGraph *gr1 = new TGraph(n_step,cut,sgraph[0]);
  gr1->GetXaxis()->SetTitle(optVar+" cut");
  gr1->GetYaxis()->SetTitle("stat. signif."); 

  TGraph *gr2 = new TGraph(n_step,cut,sgraph[1]);
  gr2->GetXaxis()->SetTitle(optVar+" cut");
  gr2->GetYaxis()->SetTitle("stat. signif."); 

  TGraph *gr3 = new TGraph(n_step,cut,sgraph[2]);
  gr3->GetXaxis()->SetTitle(optVar+" cut");
  gr3->GetYaxis()->SetTitle("stat. signif."); 

  TGraph *gr4 = new TGraph(n_step,cut,sgraph[3]);
  gr4->GetXaxis()->SetTitle(optVar+" cut");
  gr4->GetYaxis()->SetTitle("stat. signif."); 

  TGraph *gr5 = new TGraph(n_step,cut,sgraph[4]);
  gr5->GetXaxis()->SetTitle(optVar+" cut");
  gr5->GetYaxis()->SetTitle("stat. signif."); 

  TGraph *gr6 = new TGraph(n_step,cut,sgraph[5]);
  gr6->GetXaxis()->SetTitle(optVar+" cut");
  gr6->GetYaxis()->SetTitle("stat. signif."); 

  TGraph *gr7 = new TGraph(n_step,cut,sgraph[6]);
  gr7->GetXaxis()->SetTitle(optVar+" cut");
  gr7->GetYaxis()->SetTitle("stat. signif."); 

  TGraph *gr8 = new TGraph(n_signal,m,cut_max);
  gr8->GetXaxis()->SetTitle("a0 Mass");
  gr8->GetYaxis()->SetTitle("cut on "+optVar);

  TCanvas tmp1;
  tmp1.cd();
  gr1->Draw("AC*");
  tmp1.Print("statsigcut"+optVar+"5GeV.gif");
  gr2->Draw("AC*");
  tmp1.Print("statsigcut"+optVar+"6GeV.gif");
  gr3->Draw("AC*");
  tmp1.Print("statsigcut"+optVar+"7GeV.gif");
  gr4->Draw("AC*");
  tmp1.Print("statsigcut"+optVar+"8GeV.gif");
  gr5->Draw("AC*");
  tmp1.Print("statsigcut"+optVar+"9GeV.gif");
  gr6->Draw("AC*");
  tmp1.Print("statsigcut"+optVar+"10GeV.gif");
  gr7->Draw("AC*");
  tmp1.Print("statsigcut"+optVar+"105GeV.gif");

  gr8->Draw("AC*");
  tmp1.Print("statsigcut"+optVar+"VSmass.gif");
  
  cout << "###############" << endl;
  cout << "Some statistics " << endl;
  cout << "Total number of processed events = " << totalEvents << endl;
  cout << "Total number of triggered events = " << passedTriggers << endl;
  cout << "###############" << endl;

  cout << "######### Optimization cuts ########" << endl;
  for(Int_t j=0; j<n_signal; j++){
    cout << "The best cut for " << optVar << " is " << cut_max[j] << " for " << m[j] << " GeV" << endl;
    cout << "Statistical significance is " << signifi_max[j] << endl;

  }

  return;
  } // end of program

bool MuMuCutOptimizer::accept_glb_mu(const int mu_index) const
{
  TLorentzVector *mu_4mom = (TLorentzVector*)muons_glb_4mom->At(mu_index);
  if(muons_glb_nhitstrack->at(mu_index) > MIN_nhits_trk     &&
     muons_glb_normChi2->at(mu_index)   < MAX_normchi2_glb  &&
     fabs(muons_glb_d0->at(mu_index))   < MAX_d0_trk        &&
     fabs(muons_glb_dz->at(mu_index))   < MAX_dz_trk        &&
     ( ( (muons_glb_nhitsPixB->at(mu_index) + muons_glb_nhitsPixE->at(mu_index)) > MIN_nhits_pixel) ||
       ( (muons_glb_nhitsPixB->at(mu_index) + muons_glb_nhitsPixE->at(mu_index)) > MIN_nhits_pixel-1 && muons_glb_nhitsPix1Hit->at(mu_index) == 1) ) )     
    return true;

  return false;
}

bool MuMuCutOptimizer::accept_trk_mu(const int mu_index) const
{
  if(muons_trk_nhitstrack->at(mu_index) > MIN_nhits_trk     &&
     muons_trk_normChi2->at(mu_index)   < MAX_normchi2_trk  &&
     fabs(muons_trk_d0->at(mu_index))   < MAX_d0_trk        &&
     fabs(muons_trk_dz->at(mu_index))   < MAX_dz_trk        &&
     ((muons_trk_nhitsPixB->at(mu_index) + muons_trk_nhitsPixE->at(mu_index)) > MIN_nhits_pixel) &&
     ((((int)muons_trk_PIDmask->at(mu_index)) & (int)pow(2,5))/(int)pow(2,5) > 0 || (((int)muons_trk_PIDmask->at(mu_index)) & (int)pow(2,8))/(int)pow(2,8) > 0))
    return true;
  
  return false;
}
