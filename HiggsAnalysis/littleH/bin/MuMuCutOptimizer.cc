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

  onlyTheBest = true;

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
  int passedCandidates = 0;
  int passedTriggers = 0;

  Float_t signal[15][15];
  Float_t backcont[15][15];
  Float_t ups1S[15][15];
  Float_t ups2S[15][15];
  Float_t ups3S[15][15];
  Float_t cut[15];
  Float_t cut1[15];
  Float_t cut2[15];
  Float_t cutgraph[15];
  Float_t cutgraphX[15];
  Float_t cutgraphY[15];
  Float_t sgraph[15];
  Float_t sth2[15][15];

  for(Int_t i=0; i<15; i++){
    for(Int_t k=0; k<15; k++){
      signal[i][k]=0;
      backcont[i][k]=0;
      ups1S[i][k]=0;
      ups2S[i][k]=0;
      ups3S[i][k]=0;
      sth2[i][k]=0;
    }
    cut[i]=0;
    cut1[i]=0;
    cut2[i]=0;
    cutgraph[i] = 0.;
    cutgraphX[i] = 0.;
    cutgraphY[i] = 0.;
    sgraph[i] = 0.;
  }
  
  Float_t ssquared = 0.;
  Float_t signifi_max = 0.;
  Float_t cut_max = 0.;      
  Float_t cut1_max = 0.;  
  Float_t cut2_max = 0.;
  Float_t i_index = 0.;
  Float_t k_index = 0.;
  
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

    if (filestring.Contains("SimpleTree_ppMuX"))
      weight = 4080.96;
    else if (filestring.Contains("SimpleTree_Upsilon1S"))
      weight = 70;
    else if (filestring.Contains("SimpleTree_Upsilon2S"))
      weight = 28;
    else if (filestring.Contains("SimpleTree_Upsilon3S")) 
      weight = 8;
    else if (filestring.Contains("SimpleTree_A0_8GEV"))
      weight = 0.12766; 
    else if (filestring.Contains("SimpleTree_A0_10GEV"))
      weight = 0.319149;
    
    // Find the best candidate (if needed)
    int myBest = 0;
    if (onlyTheBest) myBest = theBestQQ();

    for (int iqq=0; iqq<(*QQ_size)[0]; iqq++) {

      if ((*QQ_sign)[iqq] != 0) continue;//change here for opposite sign !=, for same sign ==

      if((*QQ_type)[iqq] == 0) continue;

      if (onlyTheBest && iqq != myBest) continue;

      passedCandidates++;
      
      //cut optimization: remember to comment the isolation cut!!
      	
      int thehptMu = QQ_lephpt->at(iqq);   if (thehptMu >= muons_glb_normChi2->size()) continue;
      int thelptMu = QQ_leplpt->at(iqq);    if (thelptMu >= muons_glb_normChi2->size()) continue;

      Float_t isoVarh = (muons_glb_tkIsoR03->at(thehptMu) + muons_glb_emIsoR03->at(thehptMu))/((TLorentzVector*)muons_glb_4mom->At(thehptMu))->Pt();
      Float_t isoVarl = (muons_glb_tkIsoR03->at(thelptMu) + muons_glb_emIsoR03->at(thelptMu))/((TLorentzVector*)muons_glb_4mom->At(thelptMu))->Pt();

      //isolation cut optimization      
//       for(Int_t i=0; i<15; i++){
// 	cut[i] = 0.05 + 0.01*i;
	
// 	if(isoVarl < cut[i] && isoVarh < cut[i]){
// 	  if(filestring.Contains("SimpleTree_A0_8GEV") || filestring.Contains("SimpleTree_A0_10GEV")  ) signal[i][0]+=weight;
// 	  else if(filestring.Contains("SimpleTree_ppMuX")) backcont[i][0]+=weight;
// 	  else if(filestring.Contains("SimpleTree_Upsilon1S")) ups1S[i][0]+=weight;
// 	  else if(filestring.Contains("SimpleTree_Upsilon2S")) ups2S[i][0]+=weight; 
// 	  else if(filestring.Contains("SimpleTree_Upsilon3S")) ups3S[i][0]+=weight;
// 	}
//       }

  //significance estimate for iso cut 
//   for(Int_t i=0; i<15; i++){
//     ssquared = signal[i][0]/sqrt(signal[i][0]+backcont[i][0]+ups1S[i][0]+ups2S[i][0]+ups3S[i][0]);
//     sgraph[i] = ssquared;
//     cutgraph[i] = cut[i];
//     if(ssquared > signifi_max){
//       signifi_max = ssquared;
//       cut_max = cut[i];
//     }
//   }
 
  //significance plot for iso  
//   TGraph *gr1 = new TGraph(15,cutgraph,sgraph);
//   gr1->GetXaxis()->SetTitle("iso cut (GeV/c2)");
//   gr1->GetYaxis()->SetTitle("stat. signif."); 

//   TCanvas tmp1;
//   tmp1.cd();
//   gr1->Draw("AC*");
//   tmp1.Print("statsigcut.gif");
     
      //vertex probability cut optimization
      for(Int_t j=0; j<15; j++){
	cut1[j] = 0.05 + j*0.025;
	for(Int_t k=0; k<15; k++){
	  cut2[k] = 0.8 + 0.2*k;
	  if(QQ_S3Dip -> at(iqq) < cut2[k] && QQ_probChi2 -> at(iqq) > cut1[j]){
	    if(filestring.Contains("SimpleTree_A0_8GEV") || filestring.Contains("SimpleTree_A0_10GEV")  ) signal[j][k]+=weight;
	    else if(filestring.Contains("SimpleTree_ppMuX")) backcont[j][k]+=weight;
	    else if(filestring.Contains("SimpleTree_Upsilon1S")) ups1S[j][k]+=weight;
	    else if(filestring.Contains("SimpleTree_Upsilon2S")) ups2S[j][k]+=weight; 
	    else if(filestring.Contains("SimpleTree_Upsilon3S")) ups3S[j][k]+=weight; 
	  }
	}
      }
    }
    
  }//end loop on events

  //significance estimate for chi2 cut
  for(Int_t i=0; i<15; i++){
    for(Int_t k=0; k<15; k++){ 
      ssquared = signal[i][k]/sqrt(signal[i][k]+backcont[i][k]+ups1S[i][k]+ups2S[i][k]+ups3S[i][k]);
      cout << i << " " << k << " " << cut1[i] <<" " << cut2[k] << " " << ssquared << endl;
      sth2[i][k] = ssquared;
      cutgraphX[i] = cut1[i]; 
      cutgraphY[i] = cut2[i];    
      if(ssquared > signifi_max){
	signifi_max = ssquared;
	cut1_max = cut1[i];
	cut2_max = cut2[k];
	i_index = i;
	k_index = k;
	cout << "i " << i_index << " k " << k_index << endl;

      }
    }
  }

//significance plot for prob
  
  TFile *f = new TFile("output.root", "RECREATE");
  TH2F *h2 = new TH2F("h2","3D vs chi2",15,cutgraphX,15,cutgraphY);
  for(Int_t i=0; i<15; i++){
    for (Int_t k=0; k<15; k++){
      cout << cutgraphX[i] << " " << cutgraphY[k] << endl; 
      h2->SetBinContent(i,k,sth2[i][k]);
    }
  }
  
  f->cd();
  h2->Draw();
  h2->Write();
  f->Close();

  cout << "###############" << endl;
  cout << "Some statistics " << endl;
  cout << "Total number of processed events = " << totalEvents << endl;
  cout << "Total number of triggered events = " << passedTriggers << endl;
  cout << "Total number of selected  events = " << passedCandidates << endl;
  cout << "###############" << endl;

  cout << "######### Optimization cuts ########" << endl;
  cout << "Iso cut is " << cut_max << endl;
  cout << "Chi2 prob cut is " << cut1_max << endl;
  cout << "S2Dip cut is " << cut2_max << endl;

  cout << "Statistical significance is " << ssquared << endl;

  return;
} // end of program

bool MuMuCutOptimizer::accept_glb_mu(const int mu_index) const
{
  TLorentzVector *mu_4mom = (TLorentzVector*)muons_glb_4mom->At(mu_index);

  if(muons_glb_nhitstrack->at(mu_index) > MIN_nhits_trk     &&
     muons_glb_normChi2->at(mu_index)   < MAX_normchi2_glb  &&
     fabs(muons_glb_d0->at(mu_index))   < MAX_d0_trk        &&
     fabs(muons_glb_dz->at(mu_index))   < MAX_dz_trk        &&
     (((muons_glb_nhitsPixB->at(mu_index) + muons_glb_nhitsPixE->at(mu_index)) > MIN_nhits_pixel) ||
      ((muons_glb_nhitsPixB->at(mu_index) + muons_glb_nhitsPixE->at(mu_index)) > MIN_nhits_pixel-1 && muons_glb_nhitsPix1Hit->at(mu_index) == 1)) &&
     (muons_glb_tkIsoR03->at(mu_index)+muons_glb_emIsoR03->at(mu_index))/mu_4mom->Pt() < 0.11)    
    return true;

  return false;
}

bool MuMuCutOptimizer::accept_trk_mu(const int mu_index) const
{
  TLorentzVector *mu_4mom = (TLorentzVector*)muons_trk_4mom->At(mu_index);

  if(muons_trk_nhitstrack->at(mu_index) > MIN_nhits_trk     &&
     muons_trk_normChi2->at(mu_index)   < MAX_normchi2_trk  &&
     fabs(muons_trk_d0->at(mu_index))   < MAX_d0_trk        &&
     fabs(muons_trk_dz->at(mu_index))   < MAX_dz_trk        &&
     ((muons_trk_nhitsPixB->at(mu_index) + muons_trk_nhitsPixE->at(mu_index)) > MIN_nhits_pixel) &&
     ((((int)muons_trk_PIDmask->at(mu_index)) & (int)pow(2,5))/(int)pow(2,5) > 0 || (((int)muons_trk_PIDmask->at(mu_index)) & (int)pow(2,8))/(int)pow(2,8) > 0) &&
     (muons_trk_tkIsoR03->at(mu_index)+muons_trk_emIsoR03->at(mu_index))/mu_4mom->Pt() < 0.11)
    return true;
  
  return false;
}

int MuMuCutOptimizer::theBestQQ() const
{
  int theBest = -1;
  float thehighestPt = -1.;

  //GLB-GLB case 
  for (int iqq=0; iqq<QQ_size->at(0); iqq++) {

    if (QQ_sign->at(iqq) == 0 && QQ_type->at(iqq) == 1 ) {//change here for opposite sign ==0, for same sign !=0

      int thehptMu = QQ_lephpt->at(iqq);   if (thehptMu >= muons_glb_normChi2->size()) continue;
      int thelptMu = QQ_leplpt->at(iqq);    if (thelptMu >= muons_glb_normChi2->size()) continue;

      if( ((TLorentzVector*)muons_glb_4mom->At(QQ_lephpt->at(iqq))) -> Pt() < ((TLorentzVector*)muons_glb_4mom->At(QQ_leplpt->at(iqq))) -> Pt() )
      {
        int temp_iMu = thelptMu ;
        thelptMu = thehptMu ;
        thehptMu = temp_iMu ;
      }

      if (accept_glb_mu(thehptMu) && accept_glb_mu(thelptMu)) return iqq;
      else if (accept_glb_mu(thehptMu) && accept_glb_mu(thelptMu)) return true;
    }
  }

  //GLB-TRK case
  for (int iqq=0; iqq<QQ_size->at(0); iqq++) {

    if (QQ_sign->at(iqq) == 0 && QQ_type->at(iqq) == 2 ) {//change here for opposite sign ==0, for same sign !=0
      
      int thehptMu = QQ_lephpt->at(iqq);   if (thehptMu >= muons_glb_normChi2->size()) continue;
      int thelptMu = QQ_leplpt->at(iqq);    if (thelptMu >= muons_trk_normChi2->size()) continue;

      if (accept_glb_mu(thehptMu) && accept_trk_mu(thelptMu)) {
	
        TLorentzVector *theTrMumom = (TLorentzVector*)muons_trk_4mom->At(thelptMu);
        if (theTrMumom->Perp() > thehighestPt) {
	  thehighestPt = theTrMumom->Perp();
          theBest = iqq;
	}
      }
    }  
  }
  
  if (theBest >= 0) return theBest;
  
  return theBest;
} 			       
