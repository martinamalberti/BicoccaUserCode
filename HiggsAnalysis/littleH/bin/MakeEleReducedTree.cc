#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>

#include <iostream>
#include <iomanip>
#include <string.h>

#include <math.h>

#include "MakeEleReducedTree.hh"

using namespace std; 
using namespace TMath;

MakeEleReducedTree::MakeEleReducedTree(TChain *tree) 
  : smallHBaseClass(tree) {

  onlyTheBest = true;

  bookTree();
}

void MakeEleReducedTree::bookTree()
{
  out_tree = new TTree("TMVA_tree","TMVA_tree");

  //High Pt ele
  out_tree->Branch("m_electronHpT_tkIso", &m_electronHpT_tkIso, "F");
  out_tree->Branch("m_electronHpT_emIso", &m_electronHpT_emIso, "F");
  out_tree->Branch("m_electronHpT_hadIso", &m_electronHpT_hadIso, "F");
  out_tree->Branch("m_electronHpT_IdBDT", &m_electronHpT_IdBDT, "F");
  out_tree->Branch("m_electronHpT_IdLoose", &m_electronHpT_IdLoose, "F");
  out_tree->Branch("m_electronHpT_IdTight", &m_electronHpT_IdTight, "F");
  out_tree->Branch("m_electronHpT_IdRobustLoose", &m_electronHpT_IdRobustLoose, "F");
  out_tree->Branch("m_electronHpT_IdRobustTight", &m_electronHpT_IdRobustTight, "F");
  out_tree->Branch("m_electronHpT_HoE", &m_electronHpT_HoE, "F");
  out_tree->Branch("m_electronHpT_dEtaSc", &m_electronHpT_dEtaSc, "F");
  out_tree->Branch("m_electronHpT_dPhiSc", &m_electronHpT_dPhiSc, "F");
  out_tree->Branch("m_electronHpT_SigiEtaiEtaSc", &m_electronHpT_SigiEtaiEtaSc, "F");
  out_tree->Branch("m_electronHpT_EoP", &m_electronHpT_EoP, "F");
  out_tree->Branch("m_electronHpT_Et", &m_electronHpT_Et, "F");
  out_tree->Branch("m_electronHpT_pAtVtx", &m_electronHpT_pAtVtx, "F");
  out_tree->Branch("m_electronHpT_ptAtVtx", &m_electronHpT_ptAtVtx, "F");

  //Low Pt ele
  out_tree->Branch("m_electronLpT_tkIso", &m_electronLpT_tkIso, "F");
  out_tree->Branch("m_electronLpT_emIso", &m_electronLpT_emIso, "F");
  out_tree->Branch("m_electronLpT_hadIso", &m_electronLpT_hadIso, "F");
  out_tree->Branch("m_electronLpT_IdBDT", &m_electronLpT_IdBDT, "F");
  out_tree->Branch("m_electronLpT_IdLoose", &m_electronLpT_IdLoose, "F");
  out_tree->Branch("m_electronLpT_IdTight", &m_electronLpT_IdTight, "F");
  out_tree->Branch("m_electronLpT_IdRobustLoose", &m_electronLpT_IdRobustLoose, "F");
  out_tree->Branch("m_electronLpT_IdRobustTight", &m_electronLpT_IdRobustTight, "F");
  out_tree->Branch("m_electronLpT_HoE", &m_electronLpT_HoE, "F");
  out_tree->Branch("m_electronLpT_dEtaSc", &m_electronLpT_dEtaSc, "F");
  out_tree->Branch("m_electronLpT_dPhiSc", &m_electronLpT_dPhiSc, "F");
  out_tree->Branch("m_electronLpT_SigiEtaiEtaSc", &m_electronLpT_SigiEtaiEtaSc, "F");
  out_tree->Branch("m_electronLpT_EoP", &m_electronLpT_EoP, "F");
  out_tree->Branch("m_electronLpT_Et", &m_electronLpT_Et, "F");
  out_tree->Branch("m_electronLpT_pAtVtx", &m_electronLpT_pAtVtx, "F");
  out_tree->Branch("m_electronLpT_ptAtVtx", &m_electronLpT_ptAtVtx, "F");

  out_tree->Branch("m_QQ_DeltaR", &m_QQ_DeltaR, "F");
  out_tree->Branch("m_QQ_cosTheta", &m_QQ_cosTheta, "F");
  out_tree->Branch("m_QQ_normChi2", &m_QQ_normChi2, "F");
  out_tree->Branch("m_QQ_probChi2", &m_QQ_probChi2, "F");
  out_tree->Branch("m_QQ_cosAlpha", &m_QQ_cosAlpha, "F");
  out_tree->Branch("m_QQ_STip", &m_QQ_STip, "F");
  out_tree->Branch("m_QQ_SLip", &m_QQ_SLip, "F");
  out_tree->Branch("m_QQ_S3Dip", &m_QQ_S3Dip, "F");

  return;
}


void MakeEleReducedTree::Loop(int entries) {

  if (fChain == 0) return;  

  int nentries ;
  if (entries < fChain->GetEntries() ) nentries = entries ;
  else nentries =  (int)fChain->GetEntries();

  // loop over events
  cout << "Number of entries = " << nentries << endl;

  // counters
  int totalEvents = 0;
  int passedCandidates = 0;
  int passedLooseCandidates = 0;
  int passedTriggers = 0;

  for (int jentry=0; jentry< nentries; jentry++) {
    
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    fChain->GetEntry(jentry);

    if (jentry%100000 == 0) cout << ">>> Processing event # " << jentry << endl;
    
    totalEvents++;

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
      if ( electrons_charge->at(iEle1) != electrons_charge->at(iEle2) &&  Ele1_4Mom.Pt() > 3. && ( Ele1_4Mom + Ele2_4Mom ).M() > 6. ) TriggerBit = true ;
      }
    }
    }

    if ( !TriggerBit )  continue ; 

    passedTriggers++;
        
     // Find the best candidate (if needed) ::loose ele selection
    int myBest = 0;
    if (onlyTheBest) myBest = theBestQQ(true);
         
    for (int iqq=0; iqq<(*QQ_size)[0]; iqq++) {

      if ((*QQ_sign)[iqq] != 0) continue; //exclude same sign
//       if ((*QQ_sign)[iqq] == 0) continue; //exclude opposite sign

      if((*QQ_type)[iqq] != 0) continue;

      if (onlyTheBest && iqq != myBest) continue;

      passedLooseCandidates++;

      //Get Ele indexes
      int thehptEle = QQ_lephpt->at(iqq);   
      int thelptEle = QQ_leplpt->at(iqq);  
      
      //Check Ele Pt ordering
      if( ((TLorentzVector*)electrons->At(QQ_lephpt->at(iqq))) -> Pt() > ((TLorentzVector*)electrons->At(QQ_leplpt->at(iqq))) -> Pt() )
      {
	int tempiEle = thelptEle ;
	thelptEle = thehptEle ;
	thehptEle = tempiEle ;
      }
      
      TLorentzVector *HpTele_4mom = (TLorentzVector*)electrons->At(thehptEle);
      TLorentzVector *LpTele_4mom = (TLorentzVector*)electrons->At(thelptEle);
      
      //Fill HpT ele Variables
      m_electronHpT_tkIso = electrons_tkIso->at(thehptEle)/HpTele_4mom->Pt();
      m_electronHpT_emIso = electrons_emIso->at(thehptEle);
      m_electronHpT_hadIso= electrons_hadIso->at(thehptEle);
      m_electronHpT_IdBDT = electrons_IdBDT->at(thehptEle);
      m_electronHpT_IdLoose = electrons_IdLoose->at(thehptEle);
      m_electronHpT_IdTight = electrons_IdTight->at(thehptEle);
      m_electronHpT_IdRobustLoose = electrons_IdRobustLoose->at(thehptEle);
      m_electronHpT_IdRobustTight = electrons_IdRobustTight->at(thehptEle);
      m_electronHpT_HoE = electrons_HoE->at(thehptEle);
      m_electronHpT_dEtaSc = electrons_dEtaSc->at(thehptEle);
      m_electronHpT_dPhiSc = electrons_dPhiSc->at(thehptEle);
      m_electronHpT_SigiEtaiEtaSc = electrons_SigiEtaiEtaSc->at(thehptEle);
      m_electronHpT_EoP = electrons_EoP->at(thehptEle);
      m_electronHpT_Et = electrons_Et->at(thehptEle);
      m_electronHpT_pAtVtx = electrons_pAtVtx->at(thehptEle);
      m_electronHpT_ptAtVtx = electrons_ptAtVtx->at(thehptEle);

      //Fill LpT ele Variables
      m_electronLpT_tkIso = electrons_tkIso->at(thelptEle)/LpTele_4mom->Pt();
      m_electronLpT_emIso = electrons_emIso->at(thelptEle);
      m_electronLpT_hadIso= electrons_hadIso->at(thelptEle);
      m_electronLpT_IdBDT = electrons_IdBDT->at(thelptEle);
      m_electronLpT_IdLoose = electrons_IdLoose->at(thelptEle);
      m_electronLpT_IdTight = electrons_IdTight->at(thelptEle);
      m_electronLpT_IdRobustLoose = electrons_IdRobustLoose->at(thelptEle);
      m_electronLpT_IdRobustTight = electrons_IdRobustTight->at(thelptEle);
      m_electronLpT_HoE = electrons_HoE->at(thelptEle);
      m_electronLpT_dEtaSc = electrons_dEtaSc->at(thelptEle);
      m_electronLpT_dPhiSc = electrons_dPhiSc->at(thelptEle);
      m_electronLpT_SigiEtaiEtaSc = electrons_SigiEtaiEtaSc->at(thelptEle);
      m_electronLpT_EoP = electrons_EoP->at(thelptEle);
      m_electronLpT_Et = electrons_Et->at(thelptEle);
      m_electronLpT_pAtVtx = electrons_pAtVtx->at(thelptEle);
      m_electronLpT_ptAtVtx = electrons_ptAtVtx->at(thelptEle);
      
      m_QQ_DeltaR = QQ_DeltaR->at(iqq) ;
      m_QQ_cosTheta = QQ_cosTheta->at(iqq) ;
      m_QQ_normChi2 = QQ_normChi2->at(iqq) ;
      m_QQ_probChi2 = QQ_probChi2->at(iqq) ;
      m_QQ_cosAlpha = QQ_cosAlpha->at(iqq) ;
      m_QQ_STip = QQ_STip->at(iqq) ;
      m_QQ_SLip = QQ_SLip->at(iqq) ;
      m_QQ_S3Dip = QQ_S3Dip->at(iqq) ;

      out_tree->Fill();
      
    }
        
  }

  cout << "###############" << endl;
  cout << "Some statistics " << endl;
  cout << "Total number of processed events = " << totalEvents << endl;
  cout << "Total number of triggered events = " << passedTriggers << endl;
  cout << "Total number of selected loose events = " << passedLooseCandidates << endl;
  cout << "###############" << endl;

  return;
} // end of program

void MakeEleReducedTree::saveTree(TFile * f1)
{
  f1->cd();
  out_tree->Write();
  f1->Close();
  return;
}


bool MakeEleReducedTree::accept_ele(const int ele_index) const
{
  TLorentzVector *ele_4mom = (TLorentzVector*)electrons->At(ele_index);

  if(
     //Check Normalized Iso Variable 
//      (electrons_tkIso->at(ele_index)/ele_4mom->Pt() + electrons_hadIso->at(ele_index)) < 0.4 //0.4
     //Check BDT Id Variable - see  CMS AN -2010/034
//    && electrons_IdBDT->at(ele_index) > -0.2 //0.4
//    && ((int)electrons_flag_mask->at(ele_index) % 2) != 0 //only PF ele 
    fabs(ele_4mom ->Eta()) < 1.45 || ( fabs(ele_4mom ->Eta()) > 1.6 && fabs(ele_4mom ->Eta()) < 2.4)
//    && fabs(electrons_EoP ->at(ele_index) - 1) < WIN_EoP 
     ) 
  {
//     std::cout << "lll " << (int)electrons_flag_mask->at(ele_index) % 2  << std::endl ;
    return true;
  }
//   std::cout << "lll2 " << (int)electrons_flag_mask->at(ele_index) % 2  << std::endl ;
  return false;
}

int MakeEleReducedTree::theBestQQ(const bool select_ele) const
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
      
      bool select_EE = false ;
      if ( accept_ele(thehptEle) && accept_ele(thelptEle) || !select_ele ) select_EE = true ;
      //Select the QQtoEE candidate with the hardest low pt ele
      TLorentzVector *theElemom = (TLorentzVector*)electrons->At(thelptEle);
      if (theElemom->Perp() > thehighestPt) 
      {
      thehighestPt = theElemom->Perp();
      theBest = iqq;
      }
    }
    
  }//Cycle on Onia Cand
  
  return theBest;
} 			       