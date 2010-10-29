#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>

#include <iostream>
#include <iomanip>
#include <string.h>

#include <math.h>

#include "EleEleLooper.hh"

using namespace std; 
using namespace TMath;

EleEleLooper::EleEleLooper(TChain *tree) 
  : smallHBaseClass(tree) {

  /// SELECTION CUTS ///
  n_steps = 14 ;
 
  //Onia
  MAX_QQ_S3Dip = 3.;
  MIN_probChi2 = 0.2;
  
  //Electrons

  //Opt1: ok
  
//   MAX_tkIso   = 0.2 ;
//   MAX_emIso = 0.7 ;
//   MAX_hadIso  = 0.01 ;

//   WIN_EoP     = 0.20 ;
//   WIN_EseedOP = 0.25 ;
//   WIN_dEtaSc = 0.09 ;
//   WIN_dPhiSc = 0.04 ;
//   MAX_fbrem  = 0.2 ;
//   MAX_HoE = 0.05 ; 

//   //Opt2: better
//   MAX_tkIso   = 0.05 ;
//   MAX_emIso = 0.2 ;
//   MAX_hadIso  = 0.01 ;
//   
//   WIN_EoP     = 0.20 ;
//   WIN_EseedOP = 0.25 ;
//   WIN_dEtaSc = 0.01 ;
//   WIN_dPhiSc = 0.25 ;
//   MAX_fbrem  = 0.3 ;
//   MAX_HoE = 0.05 ; 

//   //Opt3: Opt2 + EB only - ECal drivennes : works for data
//   MAX_tkIso   = 0.05 ;
//   MAX_emIso = 0.2 ;
//   MAX_hadIso  = 0.01 ;
//   
//   WIN_EoP     = 0.20 ;
//   WIN_EseedOP = 0.25 ;
//   WIN_dEtaSc = 0.01 ;
//   WIN_dPhiSc = 0.06 ; //0.25
//   MAX_fbrem  = 0.3 ;
//   MAX_HoE = 0.05 ; 

  //Opt3: Opt2 + EB only - ECal drivennes : works for data
  MAX_tkIso   = 0.05 ;
  MAX_emIso = 0.2 ;
  MAX_hadIso  = 0.01 ;
  
  WIN_EoP     = 0.20 ;
  WIN_EseedOP = 0.25 ;
  WIN_dEtaSc = 0.02 ;
  WIN_dPhiSc = 0.06 ; //0.25
  MAX_fbrem  = 0.3 ;
  MAX_HoE = 0.05 ; 

//   //Opt4: Opt2 + EB only - ECal drivennes : MC still problematic
//   MAX_tkIso   = 0.05 ;
//   MAX_emIso = 0.2 ;
//   MAX_hadIso  = 0.01 ;
//   
//   WIN_EoP     = 0.20 ;
//   WIN_EseedOP = 0.25 ;
//   WIN_dEtaSc = 0.01 ;
//   WIN_dPhiSc = 0.04 ; //0.25
//   MAX_fbrem  = 0.3 ;
//   MAX_HoE = 0.05 ; 

  bookHistos();
}

void EleEleLooper::bookHistos()
{
  hC_InvMass_OS = new hChain ("InvMass_OS", "ele-ele invariant Mass_OS", 200,4,12., n_steps+1);
  hC_InvMass_SS = new hChain ("InvMass_SS", "ele-ele invariant Mass_SS", 200,4,12., n_steps+1);
  hC_InvMass_DF = new hChain ("InvMass_DF", "ele-ele invariant Mass_DF", 200,4,12., n_steps+1);
  hC_InvMassSC_OS = new hChain ("InvMassSC_OS", "ele-ele SC invariant Mass_OS", 200,4,12., n_steps+1);
  hC_InvMassSC_SS = new hChain ("InvMassSC_SS", "ele-ele SC invariant Mass_SS", 200,4,12., n_steps+1);
  
  h_StepSummary = new TH1F   ("h_StepSummary", "h_StepSummary", n_steps+1, -0.5, n_steps + 1 - 0.5);

  return;
}


void EleEleLooper::Loop(string filename) {

  if (fChain == 0) return;  
  int nentries =  (int)fChain->GetEntries(); 

  int totalEvents = 0;
  
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

  // loop over events
  cout << "Number of entries = " << nentries << endl;
  
  for (int jentry=0; jentry< /*300000*/nentries; jentry++) {
    

    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    fChain->GetEntry(jentry);
    chainHLT->GetEntry(jentry);
    
    cout << "accept " << HLTaccept->at(0) << std::endl;
        
    if (jentry%100000 == 0) cout << ">>> Processing event # " << jentry << endl;
    
    totalEvents++;
        
    //Trigger eeRes 2->15
    bool HLT=false;
    for (int iqq=0; iqq<(*QQ_size)[0]; iqq++) {
      
      if((*QQ_type)[iqq] != 0) continue;

      int index1 = QQ_lepone -> at(iqq);
      int index2 = QQ_leptwo -> at(iqq);
      TLorentzVector *Ele1_4mom = (TLorentzVector*)electrons->At(index1);
      TLorentzVector *Ele2_4mom = (TLorentzVector*)electrons->At(index2);
      TLorentzVector Onia4Mom = *Ele1_4mom + *Ele2_4mom;
      if ( electrons_Et -> at(index1) && electrons_Et -> at(index2) && Onia4Mom.M() > 2 && Onia4Mom.M() < 15 ) HLT = true;
      
    }
    if ( !HLT ) continue; //Skip events that don't fire HLT_eeRes4...
        
    // Find the best candidate (if needed) ::no ele selection
    int myBest = 0;
    float QQ_EoP_Best = 100000000.;
    bool hasSelected = false;
    float invMassBestSC;
         
    for (int iqq=0; iqq<(*QQ_size)[0]; iqq++) {
      
      int i_step = 0;
  
      //consider just ele-ele resonances
      if((*QQ_type)[iqq] != 0) continue;
      
      const float invMass = ((TLorentzVector*)QQ_4mom->At(iqq))->M();
      
      //Step 0 : All Di Ele
      h_StepSummary -> Fill(i_step);
      if ((*QQ_sign)[iqq] == 0) {
        hC_InvMass_OS -> Fill(i_step,invMass);
        hC_InvMass_DF -> Fill(i_step,invMass,1);
      }
      if ((*QQ_sign)[iqq] != 0) {
        hC_InvMass_SS -> Fill(i_step,invMass);
        hC_InvMass_DF -> Fill(i_step,invMass,-1);
      }
      
            
      //Step 1: Cuts on Onia IP
      i_step++;
      if ( QQ_S3Dip -> at(iqq) > MAX_QQ_S3Dip ) continue;
      if ((*QQ_sign)[iqq] == 0) {
        hC_InvMass_OS -> Fill(i_step,invMass);
        hC_InvMass_DF -> Fill(i_step,invMass,1);
      }
      if ((*QQ_sign)[iqq] != 0) {
        hC_InvMass_SS -> Fill(i_step,invMass);
        hC_InvMass_DF -> Fill(i_step,invMass,-1);
      }
      h_StepSummary -> Fill(i_step);
      
            
      //Step 2: Cuts on Onia Vtx Prob
      i_step++;
      if ( QQ_probChi2 -> at(iqq) < MIN_probChi2 ) continue;
      h_StepSummary -> Fill(i_step);
      if ((*QQ_sign)[iqq] == 0) {
        hC_InvMass_OS -> Fill(i_step,invMass);
        hC_InvMass_DF -> Fill(i_step,invMass,1);
      }
      if ((*QQ_sign)[iqq] != 0) {
        hC_InvMass_SS -> Fill(i_step,invMass);
        hC_InvMass_DF -> Fill(i_step,invMass,-1);
      }
      
      ///Get Ele Indexes and 4 vec
      int index_ele1 = QQ_lepone -> at(iqq);
      int index_ele2 = QQ_leptwo -> at(iqq);
      TLorentzVector *ele1_4mom = (TLorentzVector*)electrons->At(index_ele1);
      TLorentzVector *ele2_4mom = (TLorentzVector*)electrons->At(index_ele2);
      //Build the ele 4 vectors assuming mass 0 and using just SC energy
      TLorentzVector *ele1_SC4mom = (TLorentzVector*)electrons->At(index_ele1);
      TLorentzVector *ele2_SC4mom = (TLorentzVector*)electrons->At(index_ele2);
      ele1_SC4mom -> SetE(electrons_E->at(index_ele1)) ; 
      ele2_SC4mom -> SetE(electrons_E->at(index_ele2)) ;
      //Now rescale the momentum to the energy value 
      ele1_SC4mom -> SetRho(electrons_E->at(index_ele1)) ; 
      ele2_SC4mom -> SetRho(electrons_E->at(index_ele2)) ; 
      float invMassSC = (*ele1_SC4mom + *ele2_SC4mom).M();
      

      //Step 3: Cuts on Ele Acceptance
      i_step++;
      if ( (fabs(ele1_4mom->Eta()) <  1.566 && fabs(ele1_4mom->Eta()) >  1.4442) || fabs(ele1_4mom->Eta()) > 2.5 ) continue ;
      if ( (fabs(ele2_4mom->Eta()) <  1.566 && fabs(ele2_4mom->Eta()) >  1.4442) || fabs(ele2_4mom->Eta()) > 2.5 ) continue ;
      //Barrel only
      if ( fabs(ele1_4mom->Eta()) >  1.4442 ) continue ;
      if ( fabs(ele2_4mom->Eta()) >  1.4442 ) continue ;
      h_StepSummary -> Fill(i_step);
      if ((*QQ_sign)[iqq] == 0) {
        hC_InvMass_OS -> Fill(i_step,invMass);
        hC_InvMass_DF -> Fill(i_step,invMass,1);
      }
      if ((*QQ_sign)[iqq] != 0) {
        hC_InvMass_SS -> Fill(i_step,invMass);
        hC_InvMass_DF -> Fill(i_step,invMass,-1);
      }
      

      //Step 4: Cuts on Ele TkIso
      i_step++;
      if ( electrons_tkIso->at(index_ele1)/ele1_4mom->Pt() > MAX_tkIso ) continue ;
      if ( electrons_tkIso->at(index_ele2)/ele2_4mom->Pt() > MAX_tkIso ) continue ;
      h_StepSummary -> Fill(i_step);
      if ((*QQ_sign)[iqq] == 0) {
        hC_InvMass_OS -> Fill(i_step,invMass);
        hC_InvMass_DF -> Fill(i_step,invMass,1);
      }
      if ((*QQ_sign)[iqq] != 0) {
        hC_InvMass_SS -> Fill(i_step,invMass);
        hC_InvMass_DF -> Fill(i_step,invMass,-1);
      }

      //Step 5: Cuts on Ele EmIso
      i_step++;
      if ( electrons_emIso->at(index_ele1)/ele1_4mom->Pt() > MAX_emIso ) continue ;
      if ( electrons_emIso->at(index_ele2)/ele2_4mom->Pt() > MAX_emIso ) continue ;
      h_StepSummary -> Fill(i_step);
      if ((*QQ_sign)[iqq] == 0) {
        hC_InvMass_OS -> Fill(i_step,invMass);
        hC_InvMass_DF -> Fill(i_step,invMass,1);
      }
      if ((*QQ_sign)[iqq] != 0) {
        hC_InvMass_SS -> Fill(i_step,invMass);
        hC_InvMass_DF -> Fill(i_step,invMass,-1);
      }
            
      //Step 6: Cuts on Ele EmIso
      i_step++;
      if ( electrons_emIso->at(index_ele1)/ele1_4mom->Pt() > MAX_emIso ) continue ;
      if ( electrons_emIso->at(index_ele2)/ele2_4mom->Pt() > MAX_emIso ) continue ;
      h_StepSummary -> Fill(i_step);
      if ((*QQ_sign)[iqq] == 0) {
        hC_InvMass_OS -> Fill(i_step,invMass);
        hC_InvMass_DF -> Fill(i_step,invMass,1);
      }
      if ((*QQ_sign)[iqq] != 0) {
        hC_InvMass_SS -> Fill(i_step,invMass);
        hC_InvMass_DF -> Fill(i_step,invMass,-1);
      }
      
      //Step 7: Cuts on Ele HadIso
      i_step++;
      if ( electrons_hadIso->at(index_ele1)/ele1_4mom->Pt() > MAX_hadIso ) continue ;
      if ( electrons_hadIso->at(index_ele2)/ele2_4mom->Pt() > MAX_hadIso ) continue ;
      h_StepSummary -> Fill(i_step);
      if ((*QQ_sign)[iqq] == 0) {
        hC_InvMass_OS -> Fill(i_step,invMass);
        hC_InvMass_DF -> Fill(i_step,invMass,1);
      }
      if ((*QQ_sign)[iqq] != 0) {
        hC_InvMass_SS -> Fill(i_step,invMass);
        hC_InvMass_DF -> Fill(i_step,invMass,-1);
      }
      
      //Step 8: Cuts on EoP
      i_step++;
      if ( fabs(electrons_EoP ->at(index_ele1) - 1) > WIN_EoP ) continue ;
      if ( fabs(electrons_EoP ->at(index_ele2) - 1) > WIN_EoP ) continue ;
      h_StepSummary -> Fill(i_step);
      if ((*QQ_sign)[iqq] == 0) {
        hC_InvMass_OS -> Fill(i_step,invMass);
        hC_InvMass_DF -> Fill(i_step,invMass,1);
      }
      if ((*QQ_sign)[iqq] != 0) {
        hC_InvMass_SS -> Fill(i_step,invMass);
        hC_InvMass_DF -> Fill(i_step,invMass,-1);
      }

      //Step 9: Cuts on dEtaTkScl
      i_step++;
      if ( fabs(electrons_dEtaSc->at(index_ele1)) > WIN_dEtaSc ) continue ;
      if ( fabs(electrons_dEtaSc->at(index_ele2)) > WIN_dEtaSc ) continue ;
      h_StepSummary -> Fill(i_step);
      if ((*QQ_sign)[iqq] == 0) {
        hC_InvMass_OS -> Fill(i_step,invMass);
        hC_InvMass_DF -> Fill(i_step,invMass,1);
      }
      if ((*QQ_sign)[iqq] != 0) {
        hC_InvMass_SS -> Fill(i_step,invMass);
        hC_InvMass_DF -> Fill(i_step,invMass,-1);
      }
      
      //Step 10: Cuts on dPhiTkScl
      i_step++;
      if ( fabs(electrons_dPhiSc->at(index_ele1)) > WIN_dPhiSc ) continue ;
      if ( fabs(electrons_dPhiSc->at(index_ele2)) > WIN_dPhiSc ) continue ;
      h_StepSummary -> Fill(i_step);
      if ((*QQ_sign)[iqq] == 0) {
        hC_InvMass_OS -> Fill(i_step,invMass);
        hC_InvMass_DF -> Fill(i_step,invMass,1);
      }
      if ((*QQ_sign)[iqq] != 0) {
        hC_InvMass_SS -> Fill(i_step,invMass);
        hC_InvMass_DF -> Fill(i_step,invMass,-1);
      }

      //Step 11: require zero lost hits (conversion rejection + track quality)
      i_step++;
      if ( electrons_nLostHits->at(index_ele1) > 0.5 ) continue;
      if ( electrons_nLostHits->at(index_ele2) > 0.5 ) continue;
      h_StepSummary -> Fill(i_step);
      if ((*QQ_sign)[iqq] == 0) {
        hC_InvMass_OS -> Fill(i_step,invMass);
        hC_InvMass_DF -> Fill(i_step,invMass,1);
      }
      if ((*QQ_sign)[iqq] != 0) {
        hC_InvMass_SS -> Fill(i_step,invMass);
        hC_InvMass_DF -> Fill(i_step,invMass,-1);
      }
      
      //Step 12: require ECAL drivenness
      i_step++;
//       if ( (int)electrons_flag_mask->at(index_ele1)  < 1.5 ) continue;
//       if ( (int)electrons_flag_mask->at(index_ele2)  < 1.5 ) continue;
      h_StepSummary -> Fill(i_step);
      if ((*QQ_sign)[iqq] == 0) {
        hC_InvMass_OS -> Fill(i_step,invMass);
        hC_InvMass_DF -> Fill(i_step,invMass,1);
      }
      if ((*QQ_sign)[iqq] != 0) {
        hC_InvMass_SS -> Fill(i_step,invMass);
        hC_InvMass_DF -> Fill(i_step,invMass,-1);
      }
      
      //Step 13: Harder Cuts on EoP
      i_step++;
      if ( fabs(electrons_EoP ->at(index_ele1) - 1) > 0.5 ) continue ;
      if ( fabs(electrons_EoP ->at(index_ele2) - 1) > 0.5 ) continue ;
      h_StepSummary -> Fill(i_step);
      if ((*QQ_sign)[iqq] == 0) {
        hC_InvMass_OS -> Fill(i_step,invMass);
        hC_InvMass_DF -> Fill(i_step,invMass,1);
      }
      if ((*QQ_sign)[iqq] != 0) {
        hC_InvMass_SS -> Fill(i_step,invMass);
        hC_InvMass_DF -> Fill(i_step,invMass,-1);
      }
      
      //Step 14: resolve ambiguity - choose the cand with the best E1/p1 + E2/p2
      if ( fabs(electrons_EoP ->at(index_ele1) - 1) + fabs(electrons_EoP ->at(index_ele2) - 1) < QQ_EoP_Best ) {
        QQ_EoP_Best = fabs(electrons_EoP ->at(index_ele1) - 1) + fabs(electrons_EoP ->at(index_ele2) - 1);        
        myBest = iqq;
        invMassBestSC = invMassSC;
      }
      
      hasSelected = true;
     
    }
    
    if (!hasSelected) continue;
    const float invMassBest = ((TLorentzVector*)QQ_4mom->At(myBest))->M();
      
    //Step 14: resolve ambiguity - choose the cand with the best E1/p1 + E2/p2
    h_StepSummary -> Fill(n_steps);
    if ((*QQ_sign)[myBest] == 0) {
      hC_InvMass_OS -> Fill(n_steps,invMassBest);
      hC_InvMass_DF -> Fill(n_steps,invMassBest,1);
      hC_InvMassSC_OS -> Fill(n_steps,invMassBestSC);
    }
    if ((*QQ_sign)[myBest] != 0) {
      hC_InvMass_SS -> Fill(n_steps,invMassBest);
      hC_InvMass_DF -> Fill(n_steps,invMassBest,-1);
      hC_InvMassSC_SS -> Fill(n_steps,invMassBestSC);
    }

     
    


  }

  cout << "###############" << endl;
  cout << "Some statistics " << endl;
  cout << "Total number of processed events = " << totalEvents << endl;
  cout << "###############" << endl;

  return;
} // end of program

void EleEleLooper::saveHistos(TFile * f1)
{

  f1->cd();
  hC_InvMass_OS -> Write(*f1);
  hC_InvMass_SS -> Write(*f1);
  hC_InvMass_DF -> Write(*f1);
  
  hC_InvMassSC_OS -> Write(*f1);
  hC_InvMassSC_SS -> Write(*f1);

  h_StepSummary -> Write();

  f1->Close();

  return;
}

bool EleEleLooper::accept_ele(const int ele_index) const
{
  TLorentzVector *ele_4mom = (TLorentzVector*)electrons->At(ele_index);

  if(
     //Check Normalized Iso Variable 
     //Check BDT Id Variable - see  CMS AN -2010/034
//    && electrons_IdBDT->at(ele_index) > MIN_EIdBDT // -0.2
//    && electrons_SigiEtaiEtaSc ->at(ele_index)  < MAX_SEtaEta
     electrons_tkIso->at(ele_index)/ele_4mom->Pt() < MAX_tkIso
     && electrons_hadIso->at(ele_index)/ele_4mom->Pt() < MAX_hadIso 
     && ( fabs(ele_4mom ->Eta()) < 1.442 || fabs(ele_4mom ->Eta()) > 1.56 ) //Exclude crack electrons 
     && ( fabs(ele_4mom ->Eta()) < 1.442 ) //Barrel Only
     && electrons_HoE ->at(ele_index)  < MAX_HoE

     && electrons_emIso->at(ele_index)/ele_4mom->Pt() < MAX_emIso //0.4
     && fabs(electrons_EoP ->at(ele_index) - 1) < WIN_EoP
     && fabs(electrons_dEtaSc->at(ele_index)) < WIN_dEtaSc
     && fabs(electrons_dPhiSc->at(ele_index)) < WIN_dPhiSc
     && electrons_fbrem->at(ele_index) < MAX_fbrem
     && fabs(electrons_EseedOP ->at(ele_index) - 1) < WIN_EseedOP
  
     && electrons_nLostHits->at(ele_index) < 0.5 //require zero lost hits   
     && (int)electrons_flag_mask->at(ele_index)  > 1.5 //require ecal drivenness 
    ) {
//     std::cout << "lll " << (int)electrons_flag_mask->at(ele_index) % 2  << std::endl ;
    return true;
  }
//   std::cout << "lll2 " << (int)electrons_flag_mask->at(ele_index) % 2  << std::endl ;
  return false;
}

