#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>

#include <iostream>
#include <iomanip>
#include <string.h>

#include <math.h>

#include "MakeElePlots.hh"

using namespace std; 
using namespace TMath;

MakeElePlots::MakeElePlots(TChain *tree) 
  : smallHBaseClass(tree) {

  onlyTheBest = true;

  /// SELECTION CUTS ///
 
  MIN_vtxprob_jpsi = 0.001;

  bookHistos();
}

void MakeElePlots::bookHistos()
{
  hC_EleLowPt = new hChain ("EleLowPt", "Soft Electron Pt", 50,0.,15., 2);
  hC_EleHighPt= new hChain ("EleHighPt", "Hard Electron Pt", 50,0.,15., 2);
  hC_ElePt    = new hChain ("ElePt", "Electron Pt", 50,0.,15., 2);
  hC_EleIsoTk = new hChain ("EleIsoTk", "Electron TkIso", 50,0.,1., 2);
  hC_EleIsoEm = new hChain ("EleIsoEm", "Electron EmIso", 50,0.,1., 2);
  hC_EleIsoHad= new hChain ("EleIsoHad","Electron HadIso",50,0.,1., 2);
  hC_EleId    = new hChain ("EleId","Electron Id",50,-1.,1., 2);

  hC_InvMass = new hChain ("InvMass", "ele-ele invariant mass", 100,3.2,12., 2);

  hC_QQprobChi2 = new hChain ("QQprobChi2", "QQ Vtx P(#Chi2^{2})", 50,0.,1., 2);
  hC_QQS3Dip = new hChain ("QQS3Dip", "QQ SV 3dIP Sign.", 50,0.,5., 2);
  hC_QQSlxy = new hChain ("QQSlxy", "QQ SV Transverse Sign.", 50,0.,5000., 2);
  hC_QQSTip = new hChain ("QQSTip", "QQ SV Transverse Sign.LEO", 50,0.,5., 2);
  
  h2C_EleVsElePt = new h2Chain ("EleVsElePt", "ele vs ele Pt", 50,0.,15.,50,0.,15., 2);
  
  return;
}


void MakeElePlots::Loop(int entries) {

  if (fChain == 0) return;  
  
  int nentries ;
  if (entries < fChain->GetEntries() ) nentries = entries ;
  else nentries =  (int)fChain->GetEntries();

  // loop over events
  cout << "Number of entries = " << nentries << endl;

  // counters
  int totalEvents = 0;
  int passedCandidates = 0;
  int passedTriggers = 0;

  for (int jentry=0; jentry< nentries; jentry++) {
    
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    fChain->GetEntry(jentry);

    if (jentry%100000 == 0) cout << ">>> Processing event # " << jentry << endl;
    
    totalEvents++;

    //MC block
    GetMcInfo() ;
        
//     TRIGGER CUTS 
//     if (
//       !(*HLTBits_accept)[5] &&            // Ele10_LW_L1R
//       !(*HLTBits_accept)[6] &&            // DoubleEle5_SW_L1R
//       !(*HLTBits_accept)[7] &&            // DoublePhoton5_Jpsi_L1R
//       !(*HLTBits_accept)[8] &&            // DoublePhoton5_Upsilon_L1R
//       !(*HLTBits_accept)[9]               // DoublePhoton5_eeRes_L1R
//       ) continue;    


    //Fake Trigger
    bool TriggerBit = false ; 
    for (int iEle1 = 0; iEle1 < electrons->GetEntries(); iEle1++)
    {
    TLorentzVector Ele1_4Mom = *((TLorentzVector*)electrons->At(iEle1)) ;
    if ( Ele1_4Mom.Pt() > 3. )
    {
      for (int iEle2 = iEle1+1 ; iEle2 < electrons->GetEntries(); iEle2++)
      { 
      TLorentzVector Ele2_4Mom = *((TLorentzVector*) electrons->At(iEle2)) ;
      if ( Ele1_4Mom.Pt() > 3. && ( Ele1_4Mom + Ele2_4Mom ).M() > 6. ) TriggerBit = true ;
      }
    }
    }

    if ( !TriggerBit )  continue ; 

    passedTriggers++;
    
     // Find the best candidate (if needed)
    int myBest = 0;
    if (onlyTheBest) myBest = theBestQQ();
         
    for (int iqq=0; iqq<(*QQ_size)[0]; iqq++) {

      if ((*QQ_sign)[iqq] != 0) continue; //exclude same sign
//       if ((*QQ_sign)[iqq] == 0) continue; //exclude opposite sign

      if((*QQ_type)[iqq] != 0) continue;

      if (onlyTheBest && iqq != myBest) continue;

      passedCandidates++;
      
      const float invMass = ((TLorentzVector*)QQ_4mom->At(iqq))->M();

      // Fill histos
      TLorentzVector LowPtEle  = *((TLorentzVector*)electrons->At(QQ_leplpt->at(iqq)));
      TLorentzVector HighPtEle = *((TLorentzVector*)electrons->At(QQ_lephpt->at(iqq)));
      
      if( LowPtEle.Pt() > HighPtEle.Pt() )
      {
	TLorentzVector tempTL ;
	tempTL = LowPtEle ;
	LowPtEle = HighPtEle ;
	HighPtEle = tempTL ;
      }
      
      
      hC_EleLowPt -> Fill(1, LowPtEle.Pt());
      hC_EleHighPt-> Fill(1, HighPtEle.Pt());
      hC_ElePt    -> Fill(1, LowPtEle.Pt());
      hC_ElePt    -> Fill(1, HighPtEle.Pt());
      hC_EleIsoTk -> Fill(1, electrons_tkIso -> at(QQ_leplpt->at(iqq))/LowPtEle.Pt());
      hC_EleIsoTk -> Fill(1, electrons_tkIso -> at(QQ_lephpt->at(iqq))/HighPtEle.Pt());
      hC_EleIsoEm -> Fill(1, electrons_emIso -> at(QQ_leplpt->at(iqq))/LowPtEle.Pt());
      hC_EleIsoEm -> Fill(1, electrons_emIso -> at(QQ_lephpt->at(iqq))/HighPtEle.Pt());
      hC_EleIsoHad -> Fill(1, electrons_hadIso -> at(QQ_leplpt->at(iqq))/LowPtEle.Pt());
      hC_EleIsoHad -> Fill(1, electrons_hadIso -> at(QQ_lephpt->at(iqq))/HighPtEle.Pt());
      hC_EleId -> Fill(1, electrons_IdBDT -> at(QQ_lephpt->at(iqq)) );
      hC_EleId -> Fill(1, electrons_IdBDT -> at(QQ_leplpt->at(iqq)) );

      hC_InvMass->Fill(1, invMass) ;

      hC_QQprobChi2 -> Fill(1, QQ_probChi2 -> at(iqq));
      hC_QQS3Dip -> Fill(1, QQ_S3Dip -> at(iqq));
      hC_QQSlxy  -> Fill(1, QQ_lxy -> at(iqq) / QQ_lxyErr -> at(iqq) );
      hC_QQSTip  -> Fill(1, QQ_STip -> at(iqq) );
      

      h2C_EleVsElePt -> Fill(1, LowPtEle.Pt(), HighPtEle.Pt() );

    }
  }

  cout << "###############" << endl;
  cout << "Some statistics " << endl;
  cout << "Total number of processed events = " << totalEvents << endl;
  cout << "Total number of triggered events = " << passedTriggers << endl;
  cout << "Total number of selected  events = " << passedCandidates << endl;
  cout << "###############" << endl;

  return;
} // end of program

void MakeElePlots::saveHistos(TFile * f1)
{
  f1->cd();
  hC_EleLowPt -> Write(*f1);
  hC_EleHighPt-> Write(*f1);
  hC_ElePt    -> Write(*f1);
  hC_EleIsoTk -> Write(*f1);
  hC_EleIsoEm -> Write(*f1);
  hC_EleIsoHad-> Write(*f1);
  hC_EleId    -> Write(*f1);

  hC_InvMass  -> Write(*f1);

  hC_QQprobChi2-> Write(*f1);
  hC_QQS3Dip   -> Write(*f1);
  hC_QQSlxy    -> Write(*f1); 
  hC_QQSTip    -> Write(*f1); 

  h2C_EleVsElePt-> Write(*f1);
  f1->Close();

  return;
}


bool MakeElePlots::accept_ele(const int ele_index) const
{
  TLorentzVector *ele_4mom = (TLorentzVector*)electrons->At(ele_index);

  if(
     //Check Normalized Iso Variable 
     (electrons_tkIso->at(ele_index)/ele_4mom->Pt() + electrons_hadIso->at(ele_index))/ele_4mom->Pt() < 0.04 //10000000
     //Check BDT Id Variable - see  CMS AN -2010/034
//    && electrons_IdBDT->at(ele_index) > -0.1 //0.4
     ) 
  {
    hC_ElePt->Fill(1, ele_4mom -> Pt()) ;
    return true;
  }
  return false;
}

int MakeElePlots::theBestQQ() const
{
  int theBest = -1;
  float thehighestPt = -1.;

  for (int iqq=0; iqq<QQ_size->at(0); iqq++) {

    if (QQ_sign->at(iqq) == 0 && QQ_type->at(iqq) == 0 ) { //Opposite Sign
//     if (QQ_sign->at(iqq) != 0 && QQ_type->at(iqq) == 0 ) { //Same Sign

      int thehptEle = QQ_lephpt->at(iqq);    if (thehptEle >= electrons_charge->size()) continue;
      int thelptEle = QQ_leplpt->at(iqq);    if (thelptEle >= electrons_charge->size()) continue;
          
      if( ((TLorentzVector*)electrons->At(QQ_lephpt->at(iqq))) -> Pt() > ((TLorentzVector*)electrons->At(QQ_leplpt->at(iqq))) -> Pt() )
      {
	int tempiEle = thelptEle ;
	thelptEle = thehptEle ;
	thehptEle = tempiEle ;
      }

      if (QQ_probChi2->at(iqq) > MIN_vtxprob_jpsi && accept_ele(thehptEle) && accept_ele(thelptEle)) 
      {
        //Select the QQtoEE candidate with the hardest low pt ele
        TLorentzVector *theElemom = (TLorentzVector*)electrons->At(thelptEle);
        if (theElemom->Perp() > thehighestPt) 
        {
        thehighestPt = theElemom->Perp();
        theBest = iqq;
        }
      }
    }
    
  }//Cycle on Onia Cand
  
  return theBest;
} 			       

void MakeElePlots::GetMcInfo() const
{
  TLorentzVector MC_ele4mom [2] ;
  std::vector<float> MC_ele_pt ;
  int MC_iOniaEle = 0 ;

  for (int iMC = 0; iMC < MC_pdgID->size(); iMC++)
  {
    if ( fabs(MC_pdgID->at(iMC)) == 11 && MC_iMother->at(iMC) > 0 ) //Select MC e+ e- which are not primary
    { 
    TLorentzVector *MC_ele = (TLorentzVector*)MC_particles4V->At(iMC);
    if ( MC_pdgID->at(MC_iMother->at(iMC)) == 553    || 
	MC_pdgID->at(MC_iMother->at(iMC)) == 100553 ||
	MC_pdgID->at(MC_iMother->at(iMC)) == 200553
      ) //Select ele coming out of Upsilons 
      {
      MC_ele_pt.push_back(MC_ele->Pt()) ;
      MC_ele4mom[MC_iOniaEle] = *MC_ele ;
      MC_iOniaEle++ ; 
      }
	    
    }
  }
  if ( MC_ele_pt.size() == 2 ) 
  {
    hC_ElePt->Fill(0,MC_ele_pt[0]) ;
    hC_ElePt->Fill(0,MC_ele_pt[1]) ;
    
    hC_InvMass->Fill(0, (MC_ele4mom[0] + MC_ele4mom[1]).M() ) ;
  }

}