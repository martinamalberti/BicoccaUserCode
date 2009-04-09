#include <iostream>
#include <cmath>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>


#include "TTree.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TClonesArray.h"
#include "TString.h"
#include "TGraph.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TParticle.h"
#include "TH1.h"
#include "TH2.h"
#include "TText.h"
#include "TLegend.h"

#include <Math/VectorUtil.h>
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFUtils.h"

//---- usare questa ---
// #include "HiggsAnalysis/VBFHiggsToWWto2l2nu/bin/NNfunction_SinglePair_JetCorrection.h"
// #include "HiggsAnalysis/VBFHiggsToWWto2l2nu/bin/NNfunction_SinglePair_JetCorrection.cxx"

//---- o questa ----
// #include "HiggsAnalysis/VBFHiggsToWWto2l2nu/bin/NNfunction_SinglePair_NoCorrection.h"
// #include "HiggsAnalysis/VBFHiggsToWWto2l2nu/bin/NNfunction_SinglePair_NoCorrection.cxx"

//---- queste due sono vecchie ----
// #include "HiggsAnalysis/VBFHiggsToWWto2l2nu/bin/NNfunction_SinglePair_OneShot.h"
// #include "HiggsAnalysis/VBFHiggsToWWto2l2nu/bin/NNfunction_SinglePair_OneShot.cxx"

// #include "HiggsAnalysis/VBFHiggsToWWto2l2nu/bin/NNfunction_SinglePair.h"
// #include "HiggsAnalysis/VBFHiggsToWWto2l2nu/bin/NNfunction_SinglePair.cxx"

//---- questa è la nuova senza Mjj tag ----
// #include "HiggsAnalysis/VBFHiggsToWWto2l2nu/bin/NNfunction_SinglePair_JetCorrection_NoMjj.h"
// #include "HiggsAnalysis/VBFHiggsToWWto2l2nu/bin/NNfunction_SinglePair_JetCorrection_NoMjj.cxx"


//---- questa è la nuova senza Mjj tag e senza anche il DEta tag ----
// #include "HiggsAnalysis/VBFHiggsToWWto2l2nu/bin/NNfunction_SinglePair_NoCorrection_NoMjj_NoDeta.h"
// #include "HiggsAnalysis/VBFHiggsToWWto2l2nu/bin/NNfunction_SinglePair_NoCorrection_NoMjj_NoDeta.cxx"

// #include "HiggsAnalysis/VBFHiggsToWWto2l2nu/bin/NNfunction_SinglePair_JetCorrection_NoMjj_NoDeta.h"
// #include "HiggsAnalysis/VBFHiggsToWWto2l2nu/bin/NNfunction_SinglePair_JetCorrection_NoMjj_NoDeta.cxx"


//---- questa è la nuova senza Mjj tag e senza anche il DEta tag ma con Zeppenfeld ----
// #include "HiggsAnalysis/VBFHiggsToWWto2l2nu/bin/NNfunction_SinglePair_NoCorrection_NoMjj_NoDeta_Zeppenfeld.h"
// #include "HiggsAnalysis/VBFHiggsToWWto2l2nu/bin/NNfunction_SinglePair_NoCorrection_NoMjj_NoDeta_Zeppenfeld.cxx"


//---- questa è la nuova senza Mjj tag e senza anche il DEta tag ma con Pt sum ----
// #include "HiggsAnalysis/VBFHiggsToWWto2l2nu/bin/NNfunction_SinglePair_NoCorrection_NoMjj_NoDeta_ptSum.h"
// #include "HiggsAnalysis/VBFHiggsToWWto2l2nu/bin/NNfunction_SinglePair_NoCorrection_NoMjj_NoDeta_ptSum.cxx"

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/bin/NNfunction_SinglePair_JetCorrection_NoMjj_NoDeta_ptSum.h"
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/bin/NNfunction_SinglePair_JetCorrection_NoMjj_NoDeta_ptSum.cxx"


//  ------------------------------------------------------------




double findGenParticles (int IdEvent, TClonesArray* genParticles,
  TLorentzVector &momentum_quark1,
  TLorentzVector &momentum_quark2)
{
 double found = -1;
 if (IdEvent==123 || IdEvent==124){
  TParticle* myparticle_quark1 = (TParticle*) genParticles->At(6);
  myparticle_quark1->Momentum(momentum_quark1);
  TParticle* myparticle_quark2 = (TParticle*) genParticles->At(7);
  myparticle_quark2->Momentum(momentum_quark2);     
  found = 1; 
 }
 return found;
}




//  ========== M A I N    P R O G R A M =========================

// ---- example: ./doIt_JetTag.exe 0.4 /tmp/amassiro/VBF_SimpleTree_H170_WW_2l.root
// ---- example: JetTagging_SelectionOrderAnalysis_MyNN_SinglePair in.root out.root 40 5 500 0
// ---- example: JetTagging_SelectionOrderAnalysis_MyNN_SinglePair in.root out.root m_jetPtMin m_jetDEtaMin m_jetMjjMin m_prodEtaMax

int main (int argc, char *argv[])
{ 
  
 char* nameInput = new char [1000];
 char* nameOutput = new char [1000];
 
 sprintf(nameInput,"/tmp/amassiro/VBF_SimpleTree_H160_WW_2l.root");
 sprintf(nameOutput,"histoJet.root");
 
 double m_jetPtMin = 1.;
 double m_jetDEtaMin = 2.;
 double m_jetMjjMin = 10.;
 double m_prodEtaMax = 1000.;
   
 
 if (argc >1){ 
  nameInput = argv[1] ; 
  if (argc >2) {
   nameOutput = argv[2] ;
   if (argc >3) {
    m_jetPtMin = atof(argv[3]) ;
    if (argc >4) {
     m_jetDEtaMin = atof(argv[4]) ;
     if (argc >5) {
      m_jetMjjMin = atof(argv[5]) ;
      if (argc >6) {
       m_prodEtaMax = atof(argv[6]) ;
      }
     }
    }
   }
  }
 }
 
 std::cerr << "  nameInput = " << nameInput << std::endl;
 std::cerr << "  nameOutput = " << nameOutput << std::endl;
 std::cerr << "  m_jetPtMin = " << m_jetPtMin << std::endl;
 std::cerr << "  m_jetDEtaMin = " << m_jetDEtaMin << std::endl;
 std::cerr << "  m_jetMjjMin = " << m_jetMjjMin << std::endl;
 std::cerr << "  m_prodEtaMax = " << m_prodEtaMax << std::endl;

 
 TFile outFile(nameOutput,"recreate");

 double M_Z;
 
 double DR_NN_A_1;
 double DR_NN_A_2;
 double DR_NN_B_1;
 double DR_NN_B_2;
 double DEta_NN;
 double Mjj_NN;
 double Pt_NN_1;
 double Pt_NN_2;
 double Eta_NN_1;
 double Eta_NN_2;
 double Ze_NN_I;
 double Ze_NN_II;
 
 double DR_PT_A_1;
 double DR_PT_A_2;
 double DR_PT_B_1;
 double DR_PT_B_2;
 double DEta_PT;
 double Mjj_PT;
 double Pt_PT_1;
 double Pt_PT_2;
 double Eta_PT_1;
 double Eta_PT_2;
 double Ze_PT_I;
 double Ze_PT_II;

 double DR_Mjj_A_1;
 double DR_Mjj_A_2;
 double DR_Mjj_B_1;
 double DR_Mjj_B_2;
 double DEta_Mjj;
 double Mjj_Mjj;
 double Pt_Mjj_1;
 double Pt_Mjj_2;
 double Eta_Mjj_1;
 double Eta_Mjj_2;
 double Ze_Mjj_I;
 double Ze_Mjj_II;

 double Flag_VBF;
 
 TTree m_Tree("m_Tree","m_Tree");
 
 m_Tree.Branch("M_Z",&M_Z,"M_Z/D");
 m_Tree.Branch("DR_NN_A_1",&DR_NN_A_1,"DR_NN_A_1/D");
 m_Tree.Branch("DR_NN_A_2",&DR_NN_A_2,"DR_NN_A_2/D");
 m_Tree.Branch("DR_NN_B_1",&DR_NN_B_1,"DR_NN_B_1/D");
 m_Tree.Branch("DR_NN_B_2",&DR_NN_B_2,"DR_NN_B_2/D");
 m_Tree.Branch("DEta_NN",&DEta_NN,"DEta_NN/D"); 
 m_Tree.Branch("Mjj_NN",&Mjj_NN,"Mjj_NN/D"); 
 m_Tree.Branch("Pt_NN_1",&Pt_NN_1,"Pt_NN_1/D"); 
 m_Tree.Branch("Pt_NN_2",&Pt_NN_2,"Pt_NN_2/D"); 
 m_Tree.Branch("Eta_NN_1",&Eta_NN_1,"Eta_NN_1/D"); 
 m_Tree.Branch("Eta_NN_2",&Eta_NN_2,"Eta_NN_2/D"); 
 m_Tree.Branch("Ze_NN_I",&Ze_NN_I,"Ze_NN_I/D"); 
 m_Tree.Branch("Ze_NN_II",&Ze_NN_II,"Ze_NN_II/D"); 
 
 m_Tree.Branch("DR_PT_A_1",&DR_PT_A_1,"DR_PT_A_1/D");
 m_Tree.Branch("DR_PT_A_2",&DR_PT_A_2,"DR_PT_A_2/D");
 m_Tree.Branch("DR_PT_B_1",&DR_PT_B_1,"DR_PT_B_1/D");
 m_Tree.Branch("DR_PT_B_2",&DR_PT_B_2,"DR_PT_B_2/D");
 m_Tree.Branch("DEta_PT",&DEta_PT,"DEta_PT/D"); 
 m_Tree.Branch("Mjj_PT",&Mjj_PT,"Mjj_PT/D"); 
 m_Tree.Branch("Pt_PT_1",&Pt_PT_1,"Pt_PT_1/D"); 
 m_Tree.Branch("Pt_PT_2",&Pt_PT_2,"Pt_PT_2/D"); 
 m_Tree.Branch("Eta_PT_1",&Eta_PT_1,"Eta_PT_1/D"); 
 m_Tree.Branch("Eta_PT_2",&Eta_PT_2,"Eta_PT_2/D"); 
 m_Tree.Branch("Ze_PT_I",&Ze_PT_I,"Ze_PT_I/D"); 
 m_Tree.Branch("Ze_PT_II",&Ze_PT_II,"Ze_PT_II/D");
 
 m_Tree.Branch("DR_Mjj_A_1",&DR_Mjj_A_1,"DR_Mjj_A_1/D");
 m_Tree.Branch("DR_Mjj_A_2",&DR_Mjj_A_2,"DR_Mjj_A_2/D");
 m_Tree.Branch("DR_Mjj_B_1",&DR_Mjj_B_1,"DR_Mjj_B_1/D");
 m_Tree.Branch("DR_Mjj_B_2",&DR_Mjj_B_2,"DR_Mjj_B_2/D");
 m_Tree.Branch("DEta_Mjj",&DEta_Mjj,"DEta_Mjj/D"); 
 m_Tree.Branch("Mjj_Mjj",&Mjj_Mjj,"Mjj_Mjj/D"); 
 m_Tree.Branch("Pt_Mjj_1",&Pt_Mjj_1,"Pt_Mjj_1/D"); 
 m_Tree.Branch("Pt_Mjj_2",&Pt_Mjj_2,"Pt_Mjj_2/D"); 
 m_Tree.Branch("Eta_Mjj_1",&Eta_Mjj_1,"Eta_Mjj_1/D"); 
 m_Tree.Branch("Eta_Mjj_2",&Eta_Mjj_2,"Eta_Mjj_2/D"); 
 m_Tree.Branch("Ze_Mjj_I",&Ze_Mjj_I,"Ze_Mjj_I/D"); 
 m_Tree.Branch("Ze_Mjj_II",&Ze_Mjj_II,"Ze_Mjj_II/D");
 
 m_Tree.Branch("Flag_VBF",&Flag_VBF,"Flag_VBF/D");
 
 
 
 
 
 TTree m_Tree_BeforeMInvZ("m_Tree_BeforeMInvZ","m_Tree_BeforeMInvZ");
 
 m_Tree_BeforeMInvZ.Branch("M_Z",&M_Z,"M_Z/D");
 m_Tree_BeforeMInvZ.Branch("DR_NN_A_1",&DR_NN_A_1,"DR_NN_A_1/D");
 m_Tree_BeforeMInvZ.Branch("DR_NN_A_2",&DR_NN_A_2,"DR_NN_A_2/D");
 m_Tree_BeforeMInvZ.Branch("DR_NN_B_1",&DR_NN_B_1,"DR_NN_B_1/D");
 m_Tree_BeforeMInvZ.Branch("DR_NN_B_2",&DR_NN_B_2,"DR_NN_B_2/D");
 m_Tree_BeforeMInvZ.Branch("DEta_NN",&DEta_NN,"DEta_NN/D"); 
 m_Tree_BeforeMInvZ.Branch("Mjj_NN",&Mjj_NN,"Mjj_NN/D"); 
 m_Tree_BeforeMInvZ.Branch("Pt_NN_1",&Pt_NN_1,"Pt_NN_1/D"); 
 m_Tree_BeforeMInvZ.Branch("Pt_NN_2",&Pt_NN_2,"Pt_NN_2/D"); 
 m_Tree_BeforeMInvZ.Branch("Eta_NN_1",&Eta_NN_1,"Eta_NN_1/D"); 
 m_Tree_BeforeMInvZ.Branch("Eta_NN_2",&Eta_NN_2,"Eta_NN_2/D"); 
 m_Tree_BeforeMInvZ.Branch("Ze_NN_I",&Ze_NN_I,"Ze_NN_I/D"); 
 m_Tree_BeforeMInvZ.Branch("Ze_NN_II",&Ze_NN_II,"Ze_NN_II/D"); 
 
 m_Tree_BeforeMInvZ.Branch("DR_PT_A_1",&DR_PT_A_1,"DR_PT_A_1/D");
 m_Tree_BeforeMInvZ.Branch("DR_PT_A_2",&DR_PT_A_2,"DR_PT_A_2/D");
 m_Tree_BeforeMInvZ.Branch("DR_PT_B_1",&DR_PT_B_1,"DR_PT_B_1/D");
 m_Tree_BeforeMInvZ.Branch("DR_PT_B_2",&DR_PT_B_2,"DR_PT_B_2/D");
 m_Tree_BeforeMInvZ.Branch("DEta_PT",&DEta_PT,"DEta_PT/D"); 
 m_Tree_BeforeMInvZ.Branch("Mjj_PT",&Mjj_PT,"Mjj_PT/D"); 
 m_Tree_BeforeMInvZ.Branch("Pt_PT_1",&Pt_PT_1,"Pt_PT_1/D"); 
 m_Tree_BeforeMInvZ.Branch("Pt_PT_2",&Pt_PT_2,"Pt_PT_2/D"); 
 m_Tree_BeforeMInvZ.Branch("Eta_PT_1",&Eta_PT_1,"Eta_PT_1/D"); 
 m_Tree_BeforeMInvZ.Branch("Eta_PT_2",&Eta_PT_2,"Eta_PT_2/D"); 
 m_Tree_BeforeMInvZ.Branch("Ze_PT_I",&Ze_PT_I,"Ze_PT_I/D"); 
 m_Tree_BeforeMInvZ.Branch("Ze_PT_II",&Ze_PT_II,"Ze_PT_II/D");
 
 m_Tree_BeforeMInvZ.Branch("DR_Mjj_A_1",&DR_Mjj_A_1,"DR_Mjj_A_1/D");
 m_Tree_BeforeMInvZ.Branch("DR_Mjj_A_2",&DR_Mjj_A_2,"DR_Mjj_A_2/D");
 m_Tree_BeforeMInvZ.Branch("DR_Mjj_B_1",&DR_Mjj_B_1,"DR_Mjj_B_1/D");
 m_Tree_BeforeMInvZ.Branch("DR_Mjj_B_2",&DR_Mjj_B_2,"DR_Mjj_B_2/D");
 m_Tree_BeforeMInvZ.Branch("DEta_Mjj",&DEta_Mjj,"DEta_Mjj/D"); 
 m_Tree_BeforeMInvZ.Branch("Mjj_Mjj",&Mjj_Mjj,"Mjj_Mjj/D"); 
 m_Tree_BeforeMInvZ.Branch("Pt_Mjj_1",&Pt_Mjj_1,"Pt_Mjj_1/D"); 
 m_Tree_BeforeMInvZ.Branch("Pt_Mjj_2",&Pt_Mjj_2,"Pt_Mjj_2/D"); 
 m_Tree_BeforeMInvZ.Branch("Eta_Mjj_1",&Eta_Mjj_1,"Eta_Mjj_1/D"); 
 m_Tree_BeforeMInvZ.Branch("Eta_Mjj_2",&Eta_Mjj_2,"Eta_Mjj_2/D"); 
 m_Tree_BeforeMInvZ.Branch("Ze_Mjj_I",&Ze_Mjj_I,"Ze_Mjj_I/D"); 
 m_Tree_BeforeMInvZ.Branch("Ze_Mjj_II",&Ze_Mjj_II,"Ze_Mjj_II/D");
 
 m_Tree_BeforeMInvZ.Branch("Flag_VBF",&Flag_VBF,"Flag_VBF/D");
 
 
 
  
 TChain * chain_H = new TChain ("ntpla/VBFSimpleTree") ;
 chain_H->Add (nameInput);
 
 TClonesArray * genJets = new TClonesArray ("TLorentzVector") ;
 chain_H->SetBranchAddress ("genJets", &genJets) ;
 
 TClonesArray * otherJets = new TClonesArray ("TLorentzVector") ;
 chain_H->SetBranchAddress ("otherJets", &otherJets) ;
 
 TClonesArray * genParticles = new TClonesArray ("TParticle") ;
 chain_H->SetBranchAddress ("genParticles", &genParticles) ;
 
 int IdEvent;
 chain_H->SetBranchAddress ("IdEvent", &IdEvent) ;
 
 double pair_DEta;
 double pair_Mjj;
 double pair_pt1;
 double pair_pt2;
 double pair_eta1;
 double pair_eta2;
 int pair_numJet;
 double pair_etaMean;
 double pair_SumPtOthers;
 double pair_MjjOthers;
 
 
 int numentries = chain_H->GetEntries();
 for (int i=0; i<numentries; i++ ){//---- loop over entries 
   
  chain_H->GetEntry(i);
  
  M_Z = -99.;
    
  DR_NN_A_1 = -99.;
  DR_NN_A_2 = -99.;
  DR_NN_B_1 = -99.;
  DR_NN_B_2 = -99.;
  DEta_NN = -99.;
  Mjj_NN = -99.;
  Pt_NN_1 = -99.;
  Pt_NN_2 = -99.;
  Eta_NN_1 = -99.;
  Eta_NN_2 = -99.;
  Ze_NN_I = -99.;
  Ze_NN_II = -99.;

  DR_PT_A_1 = -99.;
  DR_PT_A_2 = -99.;
  DR_PT_B_1 = -99.;
  DR_PT_B_2 = -99.;
  DEta_PT = -99.;
  Mjj_PT = -99.;
  Pt_PT_1 = -99.;
  Pt_PT_2 = -99.;
  Eta_PT_1 = -99.;
  Eta_PT_2 = -99.;
  Ze_PT_I = -99.;
  Ze_PT_II = -99.;

  DR_Mjj_A_1 = -99.;
  DR_Mjj_A_2 = -99.;
  DR_Mjj_B_1 = -99.;
  DR_Mjj_B_2 = -99.;
  DEta_Mjj = -99.;
  Mjj_Mjj = -99.;
  Pt_Mjj_1 = -99.;
  Pt_Mjj_2 = -99.;
  Eta_Mjj_1 = -99.;
  Eta_Mjj_2 = -99.;
  Ze_Mjj_I = -99.;
  Ze_Mjj_II = -99.;

  TLorentzVector momentum_quarkA;
  TLorentzVector momentum_quarkB;
  
  Flag_VBF = findGenParticles(IdEvent,genParticles,momentum_quarkA,momentum_quarkB);
  
  
  std::vector< TLorentzVector > vect_recoJet_PtSorting;
  for (int l=0; l<otherJets->GetEntries (); l++ ){
   TLorentzVector* myparticle = (TLorentzVector*) otherJets->At(l);
   vect_recoJet_PtSorting.push_back(*myparticle);
  }
  
  if (otherJets->GetEntries () >= 5) {
   std::vector<TLorentzVector> shortVector (vect_recoJet_PtSorting.begin (), vect_recoJet_PtSorting.begin () + 5) ;
   shortVector.swap (vect_recoJet_PtSorting) ;
  }

    
  ///---- filter jet M_Z ----
  int forbidden_Jet_1 = -1;
  int forbidden_Jet_2 = -1;
  double M_Z_real = 91.2; //GeV
  double M_Z_reco = -99.; //GeV
  for (int l=0; l<vect_recoJet_PtSorting.size (); l++ ){ //--- loop over otherJets ----
   TLorentzVector jet_1 = (TLorentzVector) vect_recoJet_PtSorting.at(l);
   for (int m=l+1; m<vect_recoJet_PtSorting.size (); m++ ){ //--- loop over otherJets ----
    TLorentzVector jet_2 = (TLorentzVector) vect_recoJet_PtSorting.at(m);
    double InvMass = (jet_1 + jet_2).M();
    if (fabs(InvMass - M_Z_real) < fabs(M_Z_reco - M_Z_real)){
     forbidden_Jet_1 = l;
     forbidden_Jet_2 = m;
     M_Z_reco = InvMass;
     M_Z = M_Z_reco;
    }
   }
  }
  
  TLorentzVector jet_I;
  TLorentzVector jet_II;
  if (M_Z_reco != -99.){
   jet_I = (TLorentzVector) vect_recoJet_PtSorting.at(forbidden_Jet_1);
   jet_II = (TLorentzVector) vect_recoJet_PtSorting.at(forbidden_Jet_2);
  }

  
  
  ///---- Mjj ----
   std::pair<int,int> tagJets_Mjj;
   tagJets_Mjj.first = -1;
   tagJets_Mjj.second = -1;
   double maxInvMass = 0. ;
   for (int l=0; l<vect_recoJet_PtSorting.size (); l++ ){ //--- loop over otherJets ----
    TLorentzVector myparticle_1 = (TLorentzVector) vect_recoJet_PtSorting.at(l);
    if (l == forbidden_Jet_1 || l == forbidden_Jet_2) continue;
    if (myparticle_1.Pt () < m_jetPtMin) continue ;    
    for (int m=l+1; m<vect_recoJet_PtSorting.size (); m++ ){ //--- loop over otherJets ----
     TLorentzVector myparticle_2 = (TLorentzVector) vect_recoJet_PtSorting.at(m);
     if (m == forbidden_Jet_1 || m == forbidden_Jet_2) continue;
     if (myparticle_2.Pt () < m_jetPtMin) continue ;    
     double InvMass = (myparticle_1 + myparticle_2).M();
     if (InvMass > maxInvMass){
      //---- Check if DEta and Mjj selections are fullfilled
      float etaA = myparticle_1.Eta();
      float etaB= myparticle_2.Eta();
      float DEta = fabs(etaA - etaB);
      float Mjj = (myparticle_1 + myparticle_2).M();
      float prodEta = etaA * etaB;
      if (DEta<m_jetDEtaMin || Mjj<m_jetMjjMin || prodEta>m_prodEtaMax) continue;
       
      maxInvMass = InvMass;
     //---- pt order
      if (myparticle_1.Pt() > myparticle_2.Pt()){
       tagJets_Mjj.first = l;
       tagJets_Mjj.second = m;
      }
      else {
       tagJets_Mjj.first = m;
       tagJets_Mjj.second = l;      
      }
     }
    }
   }
  

   if (tagJets_Mjj.first != -1 && tagJets_Mjj.second != -1){
    TLorentzVector myRecoJet_1 = (TLorentzVector) vect_recoJet_PtSorting.at(tagJets_Mjj.first);   
    TLorentzVector myRecoJet_2 = (TLorentzVector) vect_recoJet_PtSorting.at(tagJets_Mjj.second);
   
    Eta_Mjj_1 = myRecoJet_1.Eta();
    Eta_Mjj_2 = myRecoJet_2.Eta();
    DEta_Mjj = fabs(Eta_Mjj_1 - Eta_Mjj_2);
    Mjj_Mjj = (myRecoJet_1 + myRecoJet_2).M();
    Pt_Mjj_1 = myRecoJet_1.Pt();
    Pt_Mjj_2 = myRecoJet_2.Pt();
    double meanEta = ( Eta_Mjj_1 + Eta_Mjj_2 ) /2.;
    Ze_Mjj_I = (jet_I.Eta() - meanEta) / DEta_Mjj;
    Ze_Mjj_II = (jet_II.Eta() - meanEta) / DEta_Mjj;
      
    if (Flag_VBF==1) {
     DR_Mjj_A_1 = ROOT::Math::VectorUtil::DeltaR (myRecoJet_1.Vect(), momentum_quarkA.Vect()) ;
     DR_Mjj_A_2 = ROOT::Math::VectorUtil::DeltaR (myRecoJet_2.Vect(), momentum_quarkA.Vect()) ;
     DR_Mjj_B_1 = ROOT::Math::VectorUtil::DeltaR (myRecoJet_1.Vect(), momentum_quarkB.Vect()) ;
     DR_Mjj_B_2 = ROOT::Math::VectorUtil::DeltaR (myRecoJet_2.Vect(), momentum_quarkB.Vect()) ;
    }    
   }
   
   
   
  
  ///---- Pt ----
   std::pair<int,int> tagJets_PT;
   tagJets_PT.first = -1;
   tagJets_PT.second = -1;
   double ptMax1 = -1. ;
   double ptMax2 = -1. ;
   int maxNum1 = -1;
   int maxNum2 = -1;

   for (int l=0; l<vect_recoJet_PtSorting.size (); l++ ){ //--- loop over otherJets ----
    TLorentzVector myparticle_1 = (TLorentzVector) vect_recoJet_PtSorting.at(l);
    if (l == forbidden_Jet_1 || l == forbidden_Jet_2) continue;
    if (maxNum1!=-1){
    //---- Check if DEta and Mjj selections are fullfilled

     float etaA = myparticle_1.Eta();
     TLorentzVector myparticle_1_temp = (TLorentzVector) vect_recoJet_PtSorting.at(maxNum1);
     float etaB= myparticle_1_temp.Eta();
     float DEta = fabs(etaA - etaB);
     float Mjj = (myparticle_1 + myparticle_1_temp).M();
     float prodEta = etaA * etaB;
     if (DEta<m_jetDEtaMin || Mjj<m_jetMjjMin || prodEta>m_prodEtaMax) continue;
    }
    
    if (myparticle_1.Pt () < m_jetPtMin) continue ;    
    
    if (myparticle_1.Pt() > ptMax2)
    {
     if (myparticle_1.Pt() > ptMax1){
      maxNum2 = maxNum1;
      ptMax2 = ptMax1;
      maxNum1 = l;
      ptMax1 = myparticle_1.Pt() ;
     } 
     else {
      maxNum2 = l;
      ptMax2 = myparticle_1.Pt() ;
     }
    }
   }
   

   tagJets_PT.first = maxNum1;
   tagJets_PT.second = maxNum2;
   
   if (tagJets_PT.first != -1 && tagJets_PT.second != -1){
    TLorentzVector myRecoJet_1 = (TLorentzVector) vect_recoJet_PtSorting.at(tagJets_PT.first);   
    TLorentzVector myRecoJet_2 = (TLorentzVector) vect_recoJet_PtSorting.at(tagJets_PT.second);
   
    Eta_PT_1 = myRecoJet_1.Eta();
    Eta_PT_2 = myRecoJet_2.Eta();
    DEta_PT = fabs(Eta_PT_1 - Eta_PT_2);
    Mjj_PT = (myRecoJet_1 + myRecoJet_2).M();
    Pt_PT_1 = myRecoJet_1.Pt();
    Pt_PT_2 = myRecoJet_2.Pt();
    double meanEta = ( Eta_PT_1 + Eta_PT_2 ) /2.;
    Ze_PT_I = (jet_I.Eta() - meanEta) / DEta_PT;
    Ze_PT_II = (jet_II.Eta() - meanEta) / DEta_PT;

    if (Flag_VBF==1) {
     DR_PT_A_1 = ROOT::Math::VectorUtil::DeltaR (myRecoJet_1.Vect(), momentum_quarkA.Vect()) ;
     DR_PT_A_2 = ROOT::Math::VectorUtil::DeltaR (myRecoJet_2.Vect(), momentum_quarkA.Vect()) ;
     DR_PT_B_1 = ROOT::Math::VectorUtil::DeltaR (myRecoJet_1.Vect(), momentum_quarkB.Vect()) ;
     DR_PT_B_2 = ROOT::Math::VectorUtil::DeltaR (myRecoJet_2.Vect(), momentum_quarkB.Vect()) ;
    }    
   }
 
   
   
    ///---- NN ----

   std::pair<int,int> tagJets_NN;
   tagJets_NN.first = -1;
   tagJets_NN.second = -1;
   double NNMax = -2 ;
   int maxNNNum1 = -1;
   int maxNNNum2 = -1;


   
//    sort( vect_recoJet_PtSorting.begin(), vect_recoJet_PtSorting.end(),vbfhww2l::PtSortingTLorentzVector() );

      
   NNfunction myNN;
   
   
    ///---- make pair ----

   int counter = 0;
   pair_numJet = otherJets->GetEntries ();
   for (int l=0; l<(int) vect_recoJet_PtSorting.size(); l++ ){
    if (l == forbidden_Jet_1 || l == forbidden_Jet_2) continue; 
    for (int m=l+1; m<(int) vect_recoJet_PtSorting.size(); m++ ){
     if (m == forbidden_Jet_1 || m == forbidden_Jet_2) continue;
     pair_DEta = fabs(vect_recoJet_PtSorting.at(l).Eta() - vect_recoJet_PtSorting.at(m).Eta());
     TLorentzVector sumPair = vect_recoJet_PtSorting.at(l) + vect_recoJet_PtSorting.at(m);
     pair_Mjj = sumPair.M();
     pair_pt1 = vect_recoJet_PtSorting.at(l).Pt();
     pair_pt2 = vect_recoJet_PtSorting.at(m).Pt();
     pair_eta1 = vect_recoJet_PtSorting.at(l).Eta();
     pair_eta2 = vect_recoJet_PtSorting.at(m).Eta();  
     pair_etaMean = pair_eta1;
     pair_etaMean+= pair_eta2;   
     TLorentzVector sumOthers(0,0,0,0);
     for (int k=0; k<(int) vect_recoJet_PtSorting.size(); k++ ){
      if (k!=l && k!=m){
       TLorentzVector addOther = vect_recoJet_PtSorting.at(k);
       sumOthers = sumOthers + addOther;
       pair_etaMean+= addOther.Eta();
      }
     }
     pair_SumPtOthers = sumOthers.Pt();
     pair_MjjOthers = sumOthers.M();     
     
//      double flagNN = myNN.Value(0,pair_DEta,pair_Mjj,pair_pt1,pair_pt2,pair_numJet,pair_SumPtOthers,pair_MjjOthers);
     
//      double flagNN = myNN.Value(0,pair_DEta,pair_pt1,pair_pt2,pair_numJet,pair_SumPtOthers,pair_MjjOthers);
     
//      double flagNN = myNN.Value(0,pair_pt1,pair_pt2,pair_numJet,pair_SumPtOthers,pair_MjjOthers);
     
//      double flagNN = myNN.Value(0,pair_pt1,pair_pt2,pair_numJet,pair_SumPtOthers,pair_MjjOthers,(pair_eta1-pair_etaMean)/pair_DEta,(pair_eta2-pair_etaMean)/pair_DEta);
     
     double flagNN = myNN.Value(0,pair_pt1+pair_pt2,pair_pt1,pair_pt2,pair_numJet,pair_SumPtOthers,pair_MjjOthers);
     
     if (flagNN > NNMax)
     {
      //---- Check if DEta and Mjj selections are fullfilled
      float etaA = vect_recoJet_PtSorting.at(l).Eta();
      float etaB= vect_recoJet_PtSorting.at(m).Eta();
      float DEta = fabs(etaA - etaB);
      float Mjj = (vect_recoJet_PtSorting.at(l) + vect_recoJet_PtSorting.at(m)).M();
      float prodEta = etaA * etaB;
      if (DEta<m_jetDEtaMin || Mjj<m_jetMjjMin || prodEta>m_prodEtaMax) continue;
      if (pair_pt1 < m_jetPtMin || pair_pt2 < m_jetPtMin) continue ;

      if (pair_pt1 > pair_pt2){
       maxNNNum1 = l;
       maxNNNum2 = m;
       NNMax = flagNN ;
      }
      else{
       maxNNNum1 = m;
       maxNNNum2 = l;
       NNMax = flagNN ;
      }
     }
     counter++;
    }
   }
     
   tagJets_NN.first = maxNNNum1;
   tagJets_NN.second = maxNNNum2;
   
   
   if (tagJets_NN.first != -1 && tagJets_NN.second != -1){
    TLorentzVector myRecoJet_1 = (TLorentzVector) vect_recoJet_PtSorting.at(tagJets_NN.first);   
    TLorentzVector myRecoJet_2 = (TLorentzVector) vect_recoJet_PtSorting.at(tagJets_NN.second);
    
    Eta_NN_1 = myRecoJet_1.Eta();
    Eta_NN_2 = myRecoJet_2.Eta();
    DEta_NN = fabs(Eta_NN_1 - Eta_NN_2);
    Mjj_NN = (myRecoJet_1 + myRecoJet_2).M();
    Pt_NN_1 = myRecoJet_1.Pt();
    Pt_NN_2 = myRecoJet_2.Pt();
    double meanEta = ( Eta_NN_1 + Eta_NN_2 ) /2.;
    Ze_NN_I = (jet_I.Eta() - meanEta) / DEta_NN;
    Ze_NN_II = (jet_II.Eta() - meanEta) / DEta_NN;

    
    if (Flag_VBF==1) { 
     DR_NN_A_1 = ROOT::Math::VectorUtil::DeltaR (myRecoJet_1.Vect(), momentum_quarkA.Vect()) ;
     DR_NN_A_2 = ROOT::Math::VectorUtil::DeltaR (myRecoJet_2.Vect(), momentum_quarkA.Vect()) ;
     DR_NN_B_1 = ROOT::Math::VectorUtil::DeltaR (myRecoJet_1.Vect(), momentum_quarkB.Vect()) ;
     DR_NN_B_2 = ROOT::Math::VectorUtil::DeltaR (myRecoJet_2.Vect(), momentum_quarkB.Vect()) ;
    }    
   }
 
   m_Tree_BeforeMInvZ.Fill();
   
   
   ////////////////////////////////////////////////
   /////----- without Minv Z preselection ----/////
   
   M_Z = -99.;
    
   DR_NN_A_1 = -99.;
   DR_NN_A_2 = -99.;
   DR_NN_B_1 = -99.;
   DR_NN_B_2 = -99.;
   DEta_NN = -99.;
   Mjj_NN = -99.;
   Pt_NN_1 = -99.;
   Pt_NN_2 = -99.;
   Eta_NN_1 = -99.;
   Eta_NN_2 = -99.;
   Ze_NN_I = -99.;
   Ze_NN_II = -99.;

   DR_PT_A_1 = -99.;
   DR_PT_A_2 = -99.;
   DR_PT_B_1 = -99.;
   DR_PT_B_2 = -99.;
   DEta_PT = -99.;
   Mjj_PT = -99.;
   Pt_PT_1 = -99.;
   Pt_PT_2 = -99.;
   Eta_PT_1 = -99.;
   Eta_PT_2 = -99.;
   Ze_PT_I = -99.;
   Ze_PT_II = -99.;

   DR_Mjj_A_1 = -99.;
   DR_Mjj_A_2 = -99.;
   DR_Mjj_B_1 = -99.;
   DR_Mjj_B_2 = -99.;
   DEta_Mjj = -99.;
   Mjj_Mjj = -99.;
   Pt_Mjj_1 = -99.;
   Pt_Mjj_2 = -99.;
   Eta_Mjj_1 = -99.;
   Eta_Mjj_2 = -99.;
   Ze_Mjj_I = -99.;
   Ze_Mjj_II = -99.;
   
    ///---- Mjj ----
   tagJets_Mjj.first = -1;
   tagJets_Mjj.second = -1;
   maxInvMass = 0. ;
   for (int l=0; l<vect_recoJet_PtSorting.size (); l++ ){ //--- loop over otherJets ----
    TLorentzVector myparticle_1 = (TLorentzVector) vect_recoJet_PtSorting.at(l);
    if (myparticle_1.Pt () < m_jetPtMin) continue ;    
    for (int m=l+1; m<vect_recoJet_PtSorting.size (); m++ ){ //--- loop over otherJets ----
     TLorentzVector myparticle_2 = (TLorentzVector) vect_recoJet_PtSorting.at(m);
     if (myparticle_2.Pt () < m_jetPtMin) continue ;    
     double InvMass = (myparticle_1 + myparticle_2).M();
     if (InvMass > maxInvMass){
      //---- Check if DEta and Mjj selections are fullfilled
      float etaA = myparticle_1.Eta();
      float etaB= myparticle_2.Eta();
      float DEta = fabs(etaA - etaB);
      float Mjj = (myparticle_1 + myparticle_2).M();
      float prodEta = etaA * etaB;
      if (DEta<m_jetDEtaMin || Mjj<m_jetMjjMin || prodEta>m_prodEtaMax) continue;
       
      maxInvMass = InvMass;
     //---- pt order
      if (myparticle_1.Pt() > myparticle_2.Pt()){
       tagJets_Mjj.first = l;
       tagJets_Mjj.second = m;
      }
      else {
       tagJets_Mjj.first = m;
       tagJets_Mjj.second = l;      
      }
     }
    }
   }
  

   if (tagJets_Mjj.first != -1 && tagJets_Mjj.second != -1){
    TLorentzVector myRecoJet_1 = (TLorentzVector) vect_recoJet_PtSorting.at(tagJets_Mjj.first);   
    TLorentzVector myRecoJet_2 = (TLorentzVector) vect_recoJet_PtSorting.at(tagJets_Mjj.second);
    
    Eta_Mjj_1 = myRecoJet_1.Eta();
    Eta_Mjj_2 = myRecoJet_2.Eta();
    DEta_Mjj = fabs(Eta_Mjj_1 - Eta_Mjj_2);
    Mjj_Mjj = (myRecoJet_1 + myRecoJet_2).M();
    Pt_Mjj_1 = myRecoJet_1.Pt();
    Pt_Mjj_2 = myRecoJet_2.Pt();

    if (Flag_VBF==1) {
     DR_Mjj_A_1 = ROOT::Math::VectorUtil::DeltaR (myRecoJet_1.Vect(), momentum_quarkA.Vect()) ;
     DR_Mjj_A_2 = ROOT::Math::VectorUtil::DeltaR (myRecoJet_2.Vect(), momentum_quarkA.Vect()) ;
     DR_Mjj_B_1 = ROOT::Math::VectorUtil::DeltaR (myRecoJet_1.Vect(), momentum_quarkB.Vect()) ;
     DR_Mjj_B_2 = ROOT::Math::VectorUtil::DeltaR (myRecoJet_2.Vect(), momentum_quarkB.Vect()) ;
    }    
   }
   
   
   
  
  ///---- Pt ----
   
   tagJets_PT.first = -1;
   tagJets_PT.second = -1;
   ptMax1 = -1. ;
   ptMax2 = -1. ;
   maxNum1 = -1;
   maxNum2 = -1;

   for (int l=0; l<vect_recoJet_PtSorting.size (); l++ ){ //--- loop over otherJets ----
    TLorentzVector myparticle_1 = (TLorentzVector) vect_recoJet_PtSorting.at(l);
    if (maxNum1!=-1){
    //---- Check if DEta and Mjj selections are fullfilled

     float etaA = myparticle_1.Eta();
     TLorentzVector myparticle_1_temp = (TLorentzVector) vect_recoJet_PtSorting.at(maxNum1);
     float etaB= myparticle_1_temp.Eta();
     float DEta = fabs(etaA - etaB);
     float Mjj = (myparticle_1 + myparticle_1_temp).M();
     float prodEta = etaA * etaB;
     if (DEta<m_jetDEtaMin || Mjj<m_jetMjjMin || prodEta>m_prodEtaMax) continue;
    }
    
    if (myparticle_1.Pt () < m_jetPtMin) continue ;    
    
    if (myparticle_1.Pt() > ptMax2)
    {
     if (myparticle_1.Pt() > ptMax1){
      maxNum2 = maxNum1;
      ptMax2 = ptMax1;
      maxNum1 = l;
      ptMax1 = myparticle_1.Pt() ;
     } 
     else {
      maxNum2 = l;
      ptMax2 = myparticle_1.Pt() ;
     }
    }
   }
   

   tagJets_PT.first = maxNum1;
   tagJets_PT.second = maxNum2;
   
   if (tagJets_PT.first != -1 && tagJets_PT.second != -1){
    TLorentzVector myRecoJet_1 = (TLorentzVector) vect_recoJet_PtSorting.at(tagJets_PT.first);   
    TLorentzVector myRecoJet_2 = (TLorentzVector) vect_recoJet_PtSorting.at(tagJets_PT.second);
   
    Eta_PT_1 = myRecoJet_1.Eta();
    Eta_PT_2 = myRecoJet_2.Eta();
    DEta_PT = fabs(Eta_PT_1 - Eta_PT_2);
    Mjj_PT = (myRecoJet_1 + myRecoJet_2).M();
    Pt_PT_1 = myRecoJet_1.Pt();
    Pt_PT_2 = myRecoJet_2.Pt();
    
    if (Flag_VBF==1) {
     DR_PT_A_1 = ROOT::Math::VectorUtil::DeltaR (myRecoJet_1.Vect(), momentum_quarkA.Vect()) ;
     DR_PT_A_2 = ROOT::Math::VectorUtil::DeltaR (myRecoJet_2.Vect(), momentum_quarkA.Vect()) ;
     DR_PT_B_1 = ROOT::Math::VectorUtil::DeltaR (myRecoJet_1.Vect(), momentum_quarkB.Vect()) ;
     DR_PT_B_2 = ROOT::Math::VectorUtil::DeltaR (myRecoJet_2.Vect(), momentum_quarkB.Vect()) ;
    }    
   }
 
   
   
   
    ///---- NN ----

   tagJets_NN.first = -1;
   tagJets_NN.second = -1;
   NNMax = -2 ;
   maxNNNum1 = -1;
   maxNNNum2 = -1;

//    std::vector< TLorentzVector > vect_recoJet_PtSorting;
//    for (int l=0; l<otherJets->GetEntries (); l++ ){
//     TLorentzVector* myparticle = (TLorentzVector*) otherJets->At(l);
//     vect_recoJet_PtSorting.push_back(*myparticle);
//    }
   
//    sort( vect_recoJet_PtSorting.begin(), vect_recoJet_PtSorting.end(),vbfhww2l::PtSortingTLorentzVector() );

      
//    NNfunction myNN;
   
   
    ///---- make pair ----

   counter = 0;
   pair_numJet = otherJets->GetEntries ();
   for (int l=0; l<(int) vect_recoJet_PtSorting.size(); l++ ){ 
    for (int m=l+1; m<(int) vect_recoJet_PtSorting.size(); m++ ){
     pair_DEta = fabs(vect_recoJet_PtSorting.at(l).Eta() - vect_recoJet_PtSorting.at(m).Eta());
     TLorentzVector sumPair = vect_recoJet_PtSorting.at(l) + vect_recoJet_PtSorting.at(m);
     pair_Mjj = sumPair.M();
     pair_pt1 = vect_recoJet_PtSorting.at(l).Pt();
     pair_pt2 = vect_recoJet_PtSorting.at(m).Pt();
     pair_eta1 = vect_recoJet_PtSorting.at(l).Eta();
     pair_eta2 = vect_recoJet_PtSorting.at(m).Eta();  
     pair_etaMean = pair_eta1;
     pair_etaMean+= pair_eta2;   
     TLorentzVector sumOthers(0,0,0,0);
     for (int k=0; k<(int) vect_recoJet_PtSorting.size(); k++ ){
      if (k!=l && k!=m){
       TLorentzVector addOther = vect_recoJet_PtSorting.at(k);
       sumOthers = sumOthers + addOther;
       pair_etaMean+= addOther.Eta();
      }
     }
     pair_SumPtOthers = sumOthers.Pt();
     pair_MjjOthers = sumOthers.M();     
     
//      double flagNN = myNN.Value(0,pair_DEta,pair_Mjj,pair_pt1,pair_pt2,pair_numJet,pair_SumPtOthers,pair_MjjOthers);
     
//      double flagNN = myNN.Value(0,pair_DEta,pair_pt1,pair_pt2,pair_numJet,pair_SumPtOthers,pair_MjjOthers);
     
//      double flagNN = myNN.Value(0,pair_pt1,pair_pt2,pair_numJet,pair_SumPtOthers,pair_MjjOthers);
     
//      double flagNN = myNN.Value(0,pair_pt1,pair_pt2,pair_numJet,pair_SumPtOthers,pair_MjjOthers,(pair_eta1-pair_etaMean)/pair_DEta,(pair_eta2-pair_etaMean)/pair_DEta);
     
     double flagNN = myNN.Value(0,pair_pt1+pair_pt2,pair_pt1,pair_pt2,pair_numJet,pair_SumPtOthers,pair_MjjOthers);
     
     if (flagNN > NNMax)
     {
      //---- Check if DEta and Mjj selections are fullfilled
      float etaA = vect_recoJet_PtSorting.at(l).Eta();
      float etaB= vect_recoJet_PtSorting.at(m).Eta();
      float DEta = fabs(etaA - etaB);
      float Mjj = (vect_recoJet_PtSorting.at(l) + vect_recoJet_PtSorting.at(m)).M();
      float prodEta = etaA * etaB;
      if (DEta<m_jetDEtaMin || Mjj<m_jetMjjMin || prodEta>m_prodEtaMax) continue;
      if (pair_pt1 < m_jetPtMin || pair_pt2 < m_jetPtMin) continue ;

      if (pair_pt1 > pair_pt2){
       maxNNNum1 = l;
       maxNNNum2 = m;
       NNMax = flagNN ;
      }
      else{
       maxNNNum1 = m;
       maxNNNum2 = l;
       NNMax = flagNN ;
      }
     }
     counter++;
    }
   }
     
   tagJets_NN.first = maxNNNum1;
   tagJets_NN.second = maxNNNum2;
   
   
   if (tagJets_NN.first != -1 && tagJets_NN.second != -1){
    TLorentzVector myRecoJet_1 = (TLorentzVector) vect_recoJet_PtSorting.at(tagJets_NN.first);   
    TLorentzVector myRecoJet_2 = (TLorentzVector) vect_recoJet_PtSorting.at(tagJets_NN.second);
    
    Eta_NN_1 = myRecoJet_1.Eta();
    Eta_NN_2 = myRecoJet_2.Eta();
    DEta_NN = fabs(Eta_NN_1 - Eta_NN_2);
    Mjj_NN = (myRecoJet_1 + myRecoJet_2).M();
    Pt_NN_1 = myRecoJet_1.Pt();
    Pt_NN_2 = myRecoJet_2.Pt();

    if (Flag_VBF==1) { 
     DR_NN_A_1 = ROOT::Math::VectorUtil::DeltaR (myRecoJet_1.Vect(), momentum_quarkA.Vect()) ;
     DR_NN_A_2 = ROOT::Math::VectorUtil::DeltaR (myRecoJet_2.Vect(), momentum_quarkA.Vect()) ;
     DR_NN_B_1 = ROOT::Math::VectorUtil::DeltaR (myRecoJet_1.Vect(), momentum_quarkB.Vect()) ;
     DR_NN_B_2 = ROOT::Math::VectorUtil::DeltaR (myRecoJet_2.Vect(), momentum_quarkB.Vect()) ;
    }    
   }
   
   m_Tree.Fill();
   
 }//---- end loop over entries 


 
 
 m_Tree.Write();
 m_Tree_BeforeMInvZ.Write();
 
 outFile.Close();
 
 return 0 ;
}

