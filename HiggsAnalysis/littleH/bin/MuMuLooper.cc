#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TGraph.h>

#include <iostream>
#include <iomanip>
#include <string.h>

#include <math.h>

#include "MuMuLooper.hh"

using namespace std;


MuMuLooper::MuMuLooper(TChain *tree) 
  : smallHBaseClass(tree) {

  onlyTheBest = true;

  /// SELECTION CUTS ///
  MAX_normchi2_glb = 20.0;
  MIN_vtxprob = 0.001;
  MAX_S3Dip = 2.;
  MAX_muisol = 0.2;//0.11;
  MIN_muP = 4.;
  MIN_pt = 2.;

  bookHistos();
}

void MuMuLooper::bookHistos()
{
  tInvMass = new TTree("tInvMass","#mu-#mu invariant mass tree");
  tInvMass->Branch("invMass",&invMass,"invMass");
  hInvMass = new TH1F("hInvMass","#mu-#mu invariant mass",1000,4.,14.);

  hQQProbChi2 = new TH1F("hQQProbChi2","#chi^2 prob", 1000, 0., 1.);
  hQQS3Dip = new TH1F("hQQS3Dip", "", 100, 0., 5.);
  hQQEta = new TH1F("hQQEta", "Eta", 100, -5.,5.);
  hQQPt = new TH1F("hQQPt", "Pt", 100, 0., 50.);
  hMuEtaTk = new TH1F("hMuEtaTk","Muon Eta Tk", 100, -5., 5.);
  hMuPtTk = new TH1F("hMuPtTk","Muon Pt Tk", 100, 0., 50.);
  hMuEtaGlb = new TH1F("hMuEtaGlb","Muon Eta Glb", 100, -5., 5.);
  hMuPtGlb = new TH1F("hMuPtGlb","Muon Pt Glb", 100, 0., 50.);

  hMuPGlb = new TH1F("hMuPGlb","Muon P Glb", 100, 0., 50.);
  hMuPTk = new TH1F("hMuPTk","Muon P Tk", 100, 0., 50.);

  return;
}

void MuMuLooper::Loop(string filename, string flag) {

  if (fChain == 0) return;  
  int nentries = (int)fChain->GetEntries(); 

  //  loop over events
  cout << "Number of entries = " << nentries << endl;

  //  counters
  int totalEvents = 0;
  int passedCandidates = 0;
  int passedTriggers = 0;

  bool accept;

  //===========================
  // === load the HLT tree ====
  //===========================
  std::vector<int> *HLTwasrun=0;
  std::vector<int> *HLTaccept=0;
  std::vector<int> *HLTerror=0;

  TChain *chainHLT = new TChain ("TriggerResults/HLTree") ;

  chainHLT -> SetBranchAddress("HLTwasrun", &HLTwasrun);
  chainHLT -> SetBranchAddress("HLTaccept", &HLTaccept);
  chainHLT -> SetBranchAddress("HLTerror" , &HLTerror);
  chainHLT -> Add(filename.c_str());

   for (int jentry=0; jentry< nentries; jentry++) {
    
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    fChain->GetEntry(jentry);
    chainHLT->GetEntry(jentry);

    if (jentry%100000 == 0) cout << ">>> Processing event # " << jentry << endl;

    totalEvents++;

    if( flag == "MC")  accept = HLTaccept->at(0);
    else
      if ((*nRun)[0] < 147116)  accept = HLTaccept->at(0);
      else accept = HLTaccept->at(1);

    if(accept == 0) continue;

    passedTriggers++;

    TString filestring(fChain->GetCurrentFile()->GetName());

//     if (filestring.Contains("QCD"))
//       weight = (35.9*40689600)/22642229;//these numbers are for QCD in 397
//     //48440000000*0.00084//532840//(1000*406896)/2886523; //these are tests for QCD in 397
//     else if (filestring.Contains("Y1S_Simple"))
//       weight = (35.9*99900*0.143)/(0.087*2323818);
//     else if (filestring.Contains("Y2S_Simple"))
//       weight = (35.9*75400*0.080)/(0.093*1148581);
//     else if (filestring.Contains("Y3S_Simple")) 
//       weight = (35.9*2850*0.57)/(0.122*944625);
//     else if (filestring.Contains("Data"))
//       weight = 1.;
    
//     if (filestring.Contains("QCD_Simple"))
//       weight = (35.9*40689600)/22642229;//these numbers are for QCD in 397
//     //          weight = (35.9*40689600)/42827261;//these numbers are for QCD in 362
//     else if (filestring.Contains("Y1S_Simple"))
//       weight = (35.9*99900*0.143)/2323818;
//     else if (filestring.Contains("Y2S_Simple"))
//       weight = (35.9*75400*0.080)/1148581;
//     else if (filestring.Contains("Y3S_Simple")) 
//       weight = (35.9*2850*0.57)/944625;
//     else if (filestring.Contains("Data"))
//       weight = 1.;
    
    if (filestring.Contains("QCD_Simple"))
      weight = (35.9*40689600)/42827261;//these numbers are for QCD in 362
    else if (filestring.Contains("Y1S_Simple"))
      weight = (35.9*7370)/2323818;
    else if (filestring.Contains("Y2S_Simple"))
      weight = (35.9*1900)/1148581;
    else if (filestring.Contains("Y3S_Simple"))
      weight = (35.9*1020)/944625;
    else if (filestring.Contains("5GEV"))
      weight = (35.9*15.)/26100;
    else if (filestring.Contains("6GEV"))
      weight = (35.9*15.)/27000;
    else if (filestring.Contains("7GEV"))
      weight = (35.9*15.)/26400;
    else if (filestring.Contains("8GEV"))
      weight = (35.9*15.)/30000;
    else if (filestring.Contains("9GEV"))
      weight = (35.9*15.)/29100;
    else if (filestring.Contains("Data"))
      weight = 1.;


    int myBest = 0;
    //    cout << jentry << endl;
    if (onlyTheBest) myBest = theBestQQ();
    
    for (int iqq=0; iqq<(*QQ_size)[0]; iqq++) {
      
      if ((*QQ_sign)[iqq] != 0) continue;//change here for opposite sign !=, for same sign ==
      
      if((*QQ_type)[iqq] == 0) continue;
      //      if( fabs(((TLorentzVector*)QQ_4mom->At(iqq))->Eta()) < 2.){
      invMass = ((TLorentzVector*)QQ_4mom->At(iqq))->M();
      //      if(invMass <= 8.5){
      //      }//end of if mass < 8.5
      
      if (onlyTheBest && iqq != myBest) continue;
      
      passedCandidates++;
      
      // Fill histos
      hQQProbChi2->Fill(QQ_probChi2 -> at(iqq),weight);
      hQQS3Dip -> Fill(QQ_S3Dip -> at(iqq),weight);
      hQQEta->Fill(((TLorentzVector*)QQ_4mom->At(iqq))->Eta(),weight);
      hQQPt->Fill(((TLorentzVector*)QQ_4mom->At(iqq))->Pt(),weight);
      
      tInvMass->Fill();
      hInvMass->Fill(invMass, weight);
      //      }//end of if eta
    }
    
   }//end loop on events
   
   cout << "###############" << endl;
  cout << "Some statistics " << endl;
  cout << "Total number of processed events = " << totalEvents << endl;
  cout << "Total number of triggered events = " << passedTriggers << endl;
  cout << "Total number of selected  events = " << passedCandidates << endl;
  cout << "###############" << endl;
  cout << "Total number of events scaled = " << hInvMass->Integral() << endl;

  return;
} // end of program

void MuMuLooper::saveHistos(TFile * f1)
{
  f1->cd();

  f1->Write();
  f1->Close();

  return;
}


bool MuMuLooper::accept_glb_mu(const int mu_index) const
{
  TLorentzVector *mu_4mom = (TLorentzVector*)muons_glb_4mom->At(mu_index);
  
  if(muons_glb_normChi2->at(mu_index)   < MAX_normchi2_glb  && mu_4mom->Pt() > MIN_pt && mu_4mom->Eta() <= 2.4 &&
     (muons_glb_tkIsoR03->at(mu_index)+muons_glb_emIsoR03->at(mu_index)+muons_glb_hadIsoR03->at(mu_index))/mu_4mom->Pt() < MAX_muisol){
    hMuPtGlb->Fill(mu_4mom->Pt(),weight);
    hMuEtaGlb->Fill(mu_4mom->Eta(),weight); 
    if(fabs(mu_4mom->Eta()) > 1.3){
      hMuPGlb->Fill(mu_4mom->Rho(),weight); 
    }
    return true;
  }
  
  return false;
}

bool MuMuLooper::accept_trk_mu(const int mu_index) const
{
  TLorentzVector *mu_4mom = (TLorentzVector*)muons_trk_4mom->At(mu_index);

  //   if(fabs(mu_4mom->Eta()) > 1.3 && mu_4mom->Rho() < MIN_muP) return false;
  
  if(((((int)muons_trk_PIDmask->at(mu_index)) & (int)pow(2,5))/(int)pow(2,5) > 0 || (((int)muons_trk_PIDmask->at(mu_index)) & (int)pow(2,8))/(int)pow(2,8) > 0) && mu_4mom->Pt() > MIN_pt && mu_4mom->Eta()<=2.4 && (muons_trk_tkIsoR03->at(mu_index)+muons_trk_emIsoR03->at(mu_index)+muons_trk_hadIsoR03->at(mu_index))/mu_4mom->Pt() < MAX_muisol){
    hMuPtTk->Fill(mu_4mom->Pt(),weight);
    hMuEtaTk->Fill(mu_4mom->Eta(),weight);
    if(fabs(mu_4mom->Eta()) > 1.3){
      hMuPTk->Fill(mu_4mom->Rho(),weight); 
    }
    return true;
  }
  
  return false;
}

int MuMuLooper::theBestQQ() const
{
  int theBest = -1;
  float thehighestChi2 = -1.;
  float thehighestPt = -1.;

  for (int iqq=0; iqq<QQ_size->at(0); iqq++) {
    //    cout << QQ_size->at(0) << " candidates" << endl;    
    if (QQ_sign->at(iqq) == 0 && QQ_type->at(iqq) == 1 ) {//change here for opposite sign ==0, for same sign !=0 

      int thehptMu = QQ_lepone->at(iqq);    if (thehptMu >= muons_glb_charge->size()) continue;
      int thelptMu = QQ_leptwo->at(iqq);    if (thelptMu >= muons_glb_charge->size()) continue;
      
      if( ((TLorentzVector*)muons_glb_4mom->At(QQ_lepone->at(iqq))) -> Pt() < ((TLorentzVector*)muons_glb_4mom->At(QQ_leptwo->at(iqq))) -> Pt() )
	{
	  int tempiMu = thelptMu ;
	  thelptMu = thehptMu ;
	  thehptMu = tempiMu ;
	}
      
      if ( QQ_probChi2->at(iqq) > MIN_vtxprob && QQ_S3Dip->at(iqq) < MAX_S3Dip && accept_glb_mu(thehptMu) && accept_glb_mu(thelptMu)){
	 Float_t theVtxChi2Prob = QQ_probChi2->at(iqq);
	 if (theVtxChi2Prob > thehighestChi2){
	   thehighestChi2 = theVtxChi2Prob;
	   theBest = iqq;
	 }
      }
      //return iqq;
    }
  }
  return theBest;
  
  //GLB-TRK case
  for (int iqq=0; iqq<QQ_size->at(0); iqq++) {

     if (QQ_sign->at(iqq) == 0 && QQ_type->at(iqq) == 2 ) {//change here for opposite sign ==0, for same sign !=0
      
       int thehptMu = QQ_lepone->at(iqq);    if (thehptMu >= muons_glb_charge->size()) continue;
       int thelptMu = QQ_leptwo->at(iqq);    if (thelptMu >= muons_trk_charge->size()) continue; 

       if( QQ_probChi2->at(iqq) > MIN_vtxprob && QQ_S3Dip->at(iqq) < MAX_S3Dip && accept_glb_mu(thehptMu) && accept_trk_mu(thelptMu)) {
	 
	 Float_t theVtxChi2Prob = QQ_probChi2->at(iqq);
	 if (theVtxChi2Prob > thehighestChi2){
	   thehighestChi2 = theVtxChi2Prob;
	   theBest = iqq;
	 }
//         TLorentzVector *theTrMumom = (TLorentzVector*)muons_trk_4mom->At(thelptMu);
//         if (theTrMumom->Perp() > thehighestPt) {
// 	  thehighestPt = theTrMumom->Perp();
//           theBest = iqq;
// 	}

//	 return iqq;//ATTENZIONE: scommentare questo significa che non stiamo facendo il best candidate!!
       }
     }
  }
   
   if (theBest >= 0) return theBest;
   return theBest;
} 			       
