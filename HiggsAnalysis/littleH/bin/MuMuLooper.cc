#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>

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
 
  MIN_nhits_trk = 12;
  MAX_normchi2_trk = 5.0;
  MAX_normchi2_glb = 20.0;
  MIN_nhits_pixel = 2;
  MAX_d0_trk = 2.0;
  MAX_dz_trk = 25.0;
  MIN_vtxprob_jpsi = 0.001;

  bookHistos();
}

void MuMuLooper::bookHistos()
{
  hInvMass = new TH1F("hInvMass","#mu-#mu invariant mass",100,3.2,12.);
  hIsoVar03_glb_TK = new TH1F("hIsoVar03_glb_TK", "isolation var03 on tk, glb", 100, 0., 20. );
  hIsoVar05_glb_TK = new TH1F("hIsoVar05_glb_TK", "isolation var05 on tk, glb", 100, 0., 20. );
  hIsoVar03_glb_ECAL = new TH1F("hIsoVar03_glb_ECAL", "isolation var03 on ecal, glb", 100, 0., 20. );
  hIsoVar05_glb_ECAL = new TH1F("hIsoVar05_glb_ECAL", "isolation var05 on ecal, glb", 100, 0., 20. );
  hIsoVar03_trk_TK = new TH1F("hIsoVar03_trk_TK", "isolation var03 on tk, trk", 100, 0., 20. );
  hIsoVar05_trk_TK = new TH1F("hIsoVar05_trk_TK", "isolation var05 on tk, trk", 100, 0., 20. );
  hIsoVar03_trk_ECAL = new TH1F("hIsoVar03_trk_ECAL", "isolation var03 on ecal, trk", 100, 0., 20. );
  hIsoVar05_trk_ECAL = new TH1F("hIsoVar05_trk_ECAL", "isolation var05 on ecal, trk", 100, 0., 20. );
  return;
}


void MuMuLooper::Loop() {

  if (fChain == 0) return;  
  int nentries = (int)fChain->GetEntries(); 

  // loop over events
  cout << "Number of entries = " << nentries << endl;

  // counters
  int totalEvents = 0;
  int passedCandidates = 0;
  int passedTriggers = 0;

  int MCcat = 0;

  for (int jentry=0; jentry< nentries; jentry++) {
    
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    fChain->GetEntry(jentry);

    if (jentry%100000 == 0) cout << ">>> Processing event # " << jentry << endl;
    
    totalEvents++;

    // TRIGGER CUTS 
    if (!(*HLTBits_accept)[0]) continue;    // SingleMu3

    passedTriggers++;

    //estrablish which kind of MC this is
    MCcat = -999;
    TString filestring(fChain->GetCurrentFile()->GetName());

    //FIXME to be defined
    /*
    if (filestring.Contains("promptJpsiMuMu")) {
       MCcat = 0;
       // weight = 0.1089;
       weight = 0.09176;
    } else if (filestring.Contains("BJpsiMuMu")) {
       MCcat = 1;
       // weight = 0.1226;
       weight = 0.0309;
    }
    */

     // Find the best candidate (if needed)
    int myBest = 0;
    if (onlyTheBest) myBest = theBestQQ();

    for (int iqq=0; iqq<(*QQ_size)[0]; iqq++) {

      if ((*QQ_sign)[iqq] != 0) continue;//change here for opposite sign !=, for same sign ==

      if((*QQ_type)[iqq] == 0) continue;

      if (onlyTheBest && iqq != myBest) continue;

	passedCandidates++;
	
	const float invMass = ((TLorentzVector*)QQ_4mom->At(iqq))->M();

        // Fill histos
	hInvMass->Fill(invMass);

    }
  }

  //  saveHistos(f1);

  cout << "###############" << endl;
  cout << "Some statistics " << endl;
  cout << "Total number of processed events = " << totalEvents << endl;
  cout << "Total number of triggered events = " << passedTriggers << endl;
  cout << "Total number of selected  events = " << passedCandidates << endl;
  cout << "###############" << endl;

  return;
} // end of program

void MuMuLooper::saveHistos(TFile * f1)
{
  //  TFile f1("out.root","RECREATE");
  f1->cd();
  hInvMass->Write();
  hIsoVar03_glb_TK->Write();
  hIsoVar05_glb_TK->Write();
  hIsoVar03_glb_ECAL->Write();
  hIsoVar05_glb_ECAL->Write();
  hIsoVar03_trk_TK->Write();
  hIsoVar05_trk_TK->Write();
  hIsoVar03_trk_ECAL->Write();
  hIsoVar05_trk_ECAL->Write();

  f1->Close();

  return;
}


bool MuMuLooper::accept_glb_mu(const int mu_index) const
{
  TLorentzVector *mu_4mom = (TLorentzVector*)muons_glb_4mom->At(mu_index);
  hIsoVar03_glb_TK->Fill(muons_glb_tkIsoR03->at(mu_index));
  hIsoVar05_glb_TK->Fill(muons_glb_tkIsoR05->at(mu_index));
  hIsoVar03_glb_ECAL->Fill(muons_glb_emIsoR03->at(mu_index));
  hIsoVar05_glb_ECAL->Fill(muons_glb_emIsoR05->at(mu_index));

  if(muons_glb_nhitstrack->at(mu_index) > MIN_nhits_trk     &&
     muons_glb_normChi2->at(mu_index)   < MAX_normchi2_glb  &&
     fabs(muons_glb_d0->at(mu_index))   < MAX_d0_trk        &&
     fabs(muons_glb_dz->at(mu_index))   < MAX_dz_trk        &&
     (((muons_glb_nhitsPixB->at(mu_index) + muons_glb_nhitsPixE->at(mu_index)) > MIN_nhits_pixel) ||
      ((muons_glb_nhitsPixB->at(mu_index) + muons_glb_nhitsPixE->at(mu_index)) > MIN_nhits_pixel-1 && muons_glb_nhitsPix1Hit->at(mu_index) == 1)) &&
     muons_glb_tkIsoR03->at(mu_index)/mu_4mom->Pt() < 0.2 &&
     muons_glb_emIsoR03->at(mu_index) < 0.6
     ) return true;

  return false;
}

bool MuMuLooper::accept_trk_mu(const int mu_index) const
{
  TLorentzVector *mu_4mom = (TLorentzVector*)muons_trk_4mom->At(mu_index);
  hIsoVar03_trk_TK->Fill(muons_trk_tkIsoR03->at(mu_index));
  //  hIsoVar05_trk_TK->Fill(muons_trk_tkIsoR05->at(mu_index));
  hIsoVar03_trk_ECAL->Fill(muons_trk_emIsoR03->at(mu_index));
  //  hIsoVar05_trk_ECAL->Fill(muons_trk_emIsoR05->at(mu_index));

  if(muons_trk_nhitstrack->at(mu_index) > MIN_nhits_trk     &&
     muons_trk_normChi2->at(mu_index)   < MAX_normchi2_trk  &&
     fabs(muons_trk_d0->at(mu_index))   < MAX_d0_trk        &&
     fabs(muons_trk_dz->at(mu_index))   < MAX_dz_trk        &&
     ((muons_trk_nhitsPixB->at(mu_index) + muons_trk_nhitsPixE->at(mu_index)) > MIN_nhits_pixel) &&
     ((((int)muons_trk_PIDmask->at(mu_index)) & (int)pow(2,5))/(int)pow(2,5) > 0 || (((int)muons_trk_PIDmask->at(mu_index)) & (int)pow(2,8))/(int)pow(2,8) > 0) &&
     muons_trk_tkIsoR03->at(mu_index)/mu_4mom->Pt() < 0.2 &&
     muons_trk_emIsoR03->at(mu_index) < 0.6 
    ) return true;

  return false;
}


int MuMuLooper::theBestQQ() const
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

      // cout << "the highest " << ((TLorentzVector*)muons_glb_4mom->At(thehptMu))->Pt() << " and the lowest " << ((TLorentzVector*)muons_glb_4mom->At(thelptMu))->Pt() << endl;

      if (QQ_probChi2->at(iqq) > MIN_vtxprob_jpsi && accept_glb_mu(thehptMu) && accept_glb_mu(thelptMu)) return iqq;
    }
  }

  //GLB-TRK case
  for (int iqq=0; iqq<QQ_size->at(0); iqq++) {

    if (QQ_sign->at(iqq) == 0 && QQ_type->at(iqq) == 2 ) {//change here for opposite sign ==0, for same sign !=0
      
      int thehptMu = QQ_lephpt->at(iqq);   if (thehptMu >= muons_glb_normChi2->size()) continue;
      int thelptMu = QQ_leplpt->at(iqq);    if (thelptMu >= muons_trk_normChi2->size()) continue;

//       if( ((TLorentzVector*)muons_glb_4mom->At(QQ_lephpt->at(iqq))) -> Pt() < ((TLorentzVector*)muons_trk_4mom->At(QQ_leplpt->at(iqq))) -> Pt() )
//       {
//         int temp_iMu = thelptMu ;
//         thelptMu = thehptMu ;
//         thehptMu = temp_iMu ;
//       }
      
      if (QQ_probChi2->at(iqq) > MIN_vtxprob_jpsi && accept_glb_mu(thehptMu) && accept_trk_mu(thelptMu)) {
	
        TLorentzVector *theTrMumom = (TLorentzVector*)muons_trk_4mom->At(thelptMu);
        if (theTrMumom->Perp() > thehighestPt) {
	  thehighestPt = theTrMumom->Perp();
          theBest = iqq;
	}
      }
    }  
  }
  
  if (theBest >= 0) return theBest;

//   //TRK-TRK case
//   for (int iqq=0; iqq<QQ_size->at(0); iqq++) {

//     if (QQ_sign->at(iqq) == 0 && QQ_type->at(iqq) == 3 ) {//change here for opposite sign ==0, for same sign !=0
      
//        int thehptMu = QQ_lephpt->at(iqq);   if (thehptMu >= muons_trk_normChi2->size()) continue;
//        int thelptMu = QQ_leplpt->at(iqq);    if (thelptMu >= muons_trk_normChi2->size()) continue;
//       if( ((TLorentzVector*)muons_trk_4mom->At(QQ_lephpt->at(iqq))) -> Pt() < ((TLorentzVector*)muons_trk_4mom->At(QQ_leplpt->at(iqq))) -> Pt() )
//       {
//         int temp_iMu = thelptMu ;
//         thelptMu = thehptMu ;
//         thehptMu = temp_iMu ;
//       }

//       if (QQ_probChi2->at(iqq) > MIN_vtxprob_jpsi && accept_trk_mu(thehptMu) && accept_trk_mu(thelptMu)){
//         TLorentzVector *theTrMumom = (TLorentzVector*)muons_trk_4mom->At(thehptMu);
//         if (theTrMumom->Perp() > thehighestPt) {
// 	  thehighestPt = theTrMumom->Perp();
//           theBest = iqq;
// 	}
//       }
//     }    
//   }
  
//   if (theBest >= 0) return theBest;
  
  return theBest;
} 			       
