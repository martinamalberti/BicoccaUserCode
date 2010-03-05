#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>

#include <iostream>
#include <iomanip>
#include <string.h>

#include <math.h>

#include "EleEleLooper_TMVA.hh"

using namespace std; 
using namespace TMath;

EleEleLooper_TMVA::EleEleLooper_TMVA(TChain *tree) 
  : smallHBaseClass(tree) {

  onlyTheBest = true;
  
  /// SELECTION CUTS ///
  MVA_cut_val = 0.0572 ;
  WIN_EoP = 0.05;

  bookHistos();
}

void EleEleLooper_TMVA::bookHistos()
{
  TMVAreader = new TMVA::Reader( "!Color:!Silent" );
  
  TMVAreader->AddVariable("m_QQ_DeltaR" , &m_QQ_DeltaR);
  TMVAreader->AddVariable("m_QQ_probChi2" , &m_QQ_probChi2);
  TMVAreader->AddVariable("m_QQ_STip" , &m_QQ_STip);
  TMVAreader->AddVariable("m_QQ_SLip" , &m_QQ_SLip);
  TMVAreader->AddVariable("m_QQ_S3Dip" , &m_QQ_S3Dip);

  TMVAreader->AddVariable("m_electronHpT_tkIso" , &m_electronHpT_tkIso);
  TMVAreader->AddVariable("m_electronHpT_emIso" , &m_electronHpT_emIso);
  TMVAreader->AddVariable("m_electronHpT_hadIso" , &m_electronHpT_hadIso);
  TMVAreader->AddVariable("m_electronHpT_IdBDT" , &m_electronHpT_IdBDT);
  TMVAreader->AddVariable("m_electronHpT_EoP" , &m_electronHpT_EoP);

  TMVAreader->AddVariable("m_electronHpT_HoE" , &m_electronHpT_HoE);
  TMVAreader->AddVariable("m_electronHpT_dEtaSc" , &m_electronHpT_dEtaSc);
  TMVAreader->AddVariable("m_electronHpT_dPhiSc" , &m_electronHpT_dPhiSc);
  TMVAreader->AddVariable("m_electronHpT_SigiEtaiEtaSc" , &m_electronHpT_SigiEtaiEtaSc);
  TMVAreader->AddVariable("m_electronHpT_Et" , &m_electronHpT_Et);
  TMVAreader->AddVariable("m_electronHpT_pAtVtx" , &m_electronHpT_pAtVtx);
  TMVAreader->AddVariable("m_electronHpT_ptAtVtx" , &m_electronHpT_ptAtVtx);

  TMVAreader->AddVariable("m_electronLpT_tkIso" , &m_electronLpT_tkIso);
  TMVAreader->AddVariable("m_electronLpT_emIso" , &m_electronLpT_emIso);
  TMVAreader->AddVariable("m_electronLpT_hadIso" , &m_electronLpT_hadIso);
  TMVAreader->AddVariable("m_electronLpT_IdBDT" , &m_electronLpT_IdBDT);
  TMVAreader->AddVariable("m_electronLpT_EoP" , &m_electronLpT_EoP);

  TMVAreader->AddVariable("m_electronLpT_HoE" , &m_electronLpT_HoE);
  TMVAreader->AddVariable("m_electronLpT_dEtaSc" , &m_electronLpT_dEtaSc);
  TMVAreader->AddVariable("m_electronLpT_dPhiSc" , &m_electronLpT_dPhiSc);
  TMVAreader->AddVariable("m_electronLpT_SigiEtaiEtaSc" , &m_electronLpT_SigiEtaiEtaSc);
  TMVAreader->AddVariable("m_electronLpT_Et" , &m_electronLpT_Et);
  TMVAreader->AddVariable("m_electronLpT_pAtVtx" , &m_electronLpT_pAtVtx);
  TMVAreader->AddVariable("m_electronLpT_ptAtVtx" , &m_electronLpT_ptAtVtx);
  
  methodName = "BDT method";
  weightfile = "./Macros/weights/TMVAnalysis_BDT.weights.xml";
  TMVAreader->BookMVA( methodName, weightfile );

  
  hC_InvMass = new hChain ("InvMass", "ele-ele invariant mass", 100,3.2,12., 3);
  return;
}


void EleEleLooper_TMVA::Loop(int entries) {

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
      
      m_QQ_DeltaR = QQ_DeltaR->at(iqq) ;
      m_QQ_probChi2 = QQ_probChi2->at(iqq) ;
      m_QQ_STip = QQ_STip->at(iqq) ;
      m_QQ_SLip = QQ_SLip->at(iqq) ;
      m_QQ_S3Dip = QQ_S3Dip->at(iqq) ;
      
      //Fill HpT ele Variables
      m_electronHpT_tkIso = electrons_tkIso->at(thehptEle)/HpTele_4mom->Pt();
      m_electronHpT_emIso = electrons_emIso->at(thehptEle);
      m_electronHpT_hadIso= electrons_hadIso->at(thehptEle);
      m_electronHpT_IdBDT = electrons_IdBDT->at(thehptEle);
      m_electronHpT_EoP = electrons_EoP->at(thehptEle);
      m_electronHpT_HoE = electrons_HoE->at(thehptEle);
      m_electronHpT_dEtaSc = electrons_dEtaSc->at(thehptEle);
      m_electronHpT_dPhiSc = electrons_dPhiSc->at(thehptEle);
      m_electronHpT_SigiEtaiEtaSc = electrons_SigiEtaiEtaSc->at(thehptEle);
      m_electronHpT_Et = electrons_Et->at(thehptEle);
      m_electronHpT_pAtVtx = electrons_pAtVtx->at(thehptEle);
      m_electronHpT_ptAtVtx = electrons_ptAtVtx->at(thehptEle);

      //Fill LpT ele Variables
      m_electronLpT_tkIso = electrons_tkIso->at(thelptEle)/LpTele_4mom->Pt();
      m_electronLpT_emIso = electrons_emIso->at(thelptEle);
      m_electronLpT_hadIso= electrons_hadIso->at(thelptEle);
      m_electronLpT_IdBDT = electrons_IdBDT->at(thelptEle);
      m_electronLpT_EoP = electrons_EoP->at(thelptEle);
      m_electronLpT_HoE = electrons_HoE->at(thelptEle);
      m_electronLpT_dEtaSc = electrons_dEtaSc->at(thelptEle);
      m_electronLpT_dPhiSc = electrons_dPhiSc->at(thelptEle);
      m_electronLpT_SigiEtaiEtaSc = electrons_SigiEtaiEtaSc->at(thelptEle);
      m_electronLpT_Et = electrons_Et->at(thelptEle);
      m_electronLpT_pAtVtx = electrons_pAtVtx->at(thelptEle);
      m_electronLpT_ptAtVtx = electrons_ptAtVtx->at(thelptEle);

      float invMass = ((TLorentzVector*)QQ_4mom->At(iqq))->M();

      // Fill histos
      hC_InvMass->Fill(1, invMass) ;

      // Use TMVA
      float MVA_val = TMVAreader->EvaluateMVA(methodName);
      if ( MVA_val < MVA_cut_val ) continue ;

      //squeeze gaussians
      if ( fabs(electrons_EoP ->at(thehptEle) - 1.) > WIN_EoP ) continue ; 
      if ( fabs(electrons_EoP ->at(thelptEle) - 1.) > WIN_EoP ) continue ; 

      // Fill histos
      hC_InvMass->Fill(2, invMass) ;
      
      passedCandidates++ ;

    }
        
  }

  cout << "###############" << endl;
  cout << "Some statistics " << endl;
  cout << "Total number of processed events = " << totalEvents << endl;
  cout << "Total number of triggered events = " << passedTriggers << endl;
  cout << "Total number of selected loose events = " << passedLooseCandidates << endl;
  cout << "Total number of selected events = " << passedCandidates << endl;
  cout << "###############" << endl;

  return;
} // end of program

void EleEleLooper_TMVA::saveHistos(TFile * f1)
{
  f1->cd();
  hC_InvMass-> Write(*f1);
  f1->Close();

  return;
}


bool EleEleLooper_TMVA::accept_ele(const int ele_index) const
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

int EleEleLooper_TMVA::theBestQQ(const bool select_ele) const
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