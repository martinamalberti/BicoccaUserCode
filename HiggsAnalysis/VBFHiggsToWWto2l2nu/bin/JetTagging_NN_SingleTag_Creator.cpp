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

//  ------------------------------------------------------------





//  ========== M A I N    P R O G R A M =========================

// ---- example: ./doIt_JetTag_NN.exe 0.4 /tmp/amassiro/VBF_SimpleTree_H170_WW_2l.root


int main (int argc, char *argv[])
{ 
 
 bool debug_ = false;
 
 double maxDR = 0.3;
 char* nameInput = new char [1000];
 char* nameOutput = new char [1000];
 
 sprintf(nameInput,"/tmp/amassiro/VBF_SimpleTree_H160_WW_2l.root");
 sprintf(nameOutput,"histoJet.root");
 
 if (argc >1){
  maxDR = atof (argv[1]) ;
  if (argc >2){ 
   nameInput = argv[2] ; 
   if (argc >3) {
    nameOutput = argv[3] ;   
   }
  }
 }
 
 
 std::cerr << "  maxDR = " << maxDR << std::endl;
 std::cerr << "  nameInput = " << nameInput << std::endl;
 std::cerr << "  nameOutput = " << nameOutput << std::endl;

 
   
 TFile outFile(nameOutput,"recreate");

//  int numJet;
 double px_jet;
 double py_jet;
 double pz_jet;
 double E_jet;
 double match; //---- -1 = no, 1 = yes
 
 TTree tNN("tNN","tNN");
//  tNN.Branch("numJet",&numJet,"numJet/I");
 tNN.Branch("px_jet",&px_jet,"px_jet/D");
 tNN.Branch("py_jet",&py_jet,"py_jet/D");
 tNN.Branch("pz_jet",&pz_jet,"pz_jet/D");
 tNN.Branch("E_jet",&E_jet,"E_jet/D");
 tNN.Branch("match",&match,"match/D");
 
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
 
 
 
 int numentries = chain_H->GetEntries();
 for (int i=0; i<numentries; i++ ){
  std::pair<int,int> genJetTagged_pair;
  genJetTagged_pair.first = -1;
  genJetTagged_pair.second = -1;
    
  std::pair<int,int> recoJetTagged_pair;
  recoJetTagged_pair.first = -1;
  recoJetTagged_pair.second = -1;
  
  std::pair<double,double> genJetRecoQuarkDR_pair;
  genJetRecoQuarkDR_pair.first = -1;
  genJetRecoQuarkDR_pair.second = -1;
  
  
  chain_H->GetEntry(i);
  
  ///---- check genJet and quarks from Higgs ----
  if (IdEvent==123 || IdEvent==124){ //---- only VBF H_WW events
   if (genParticles->GetEntries () > 7){//--- quite sure for VBF
    TParticle* myparticle_quark1 = (TParticle*) genParticles->At(6);
    TLorentzVector momentum_quark1;
    myparticle_quark1->Momentum(momentum_quark1);
    TParticle* myparticle_quark2 = (TParticle*) genParticles->At(7);
    TLorentzVector momentum_quark2;
    myparticle_quark2->Momentum(momentum_quark2);     

    double minDR_J1 = 1000;    
    double minDR_J2 = 1000;
    int numDR_J1 = -1; // id number "k" of the loop   
    int numDR_J2 = -1;
     
    for (int k=0; k<genJets->GetEntries (); k++ ){ //--- loop over genJet ----
     TLorentzVector* myparticle = (TLorentzVector*) genJets->At(k);
          
     double DR_J1 = ROOT::Math::VectorUtil::DeltaR (myparticle->BoostVector(), momentum_quark1.BoostVector()) ;
     double DR_J2 = ROOT::Math::VectorUtil::DeltaR (myparticle->BoostVector(), momentum_quark2.BoostVector()) ;
     if (DR_J1<minDR_J1) {
      numDR_J1 = k;
      minDR_J1 = DR_J1;
     }
     if (DR_J2<minDR_J2) {
      numDR_J2 = k;
      minDR_J2 = DR_J2;
     }
    } //--- end loop over genJet ----

    if (numDR_J1 != -1 && numDR_J2 != -1 && numDR_J2!=numDR_J1) {
     if (debug_){
      std::cerr << "match 2 genJets " << numDR_J1 << " and " << numDR_J2 << std::endl;
      std::cerr << "           DR = " << minDR_J1 << " and " << minDR_J2 << std::endl;
     }
     
     if (minDR_J1<maxDR && minDR_J2<maxDR){//---- DRmax
     
      TLorentzVector* myGenJet1 = (TLorentzVector*) genJets->At(numDR_J1);
      TLorentzVector* myGenJet2 = (TLorentzVector*) genJets->At(numDR_J2);
      
      double pt1 = myGenJet1->Pt();
      double pt2 = myGenJet2->Pt();
      
      if (pt1 > pt2){
       genJetTagged_pair.first = numDR_J1;
       genJetTagged_pair.second = numDR_J2;
      }
      else {
       genJetTagged_pair.first = numDR_J2;
       genJetTagged_pair.second = numDR_J1;
      }
     }//---- end DRmax
    }
    
    ///---- recoJet tagging with quark ----
    double minDRQuarkReco1 = 1000;
    double minDRQuarkReco2 = 1000;
    int numDRQuarkReco1 = -1;
    int numDRQuarkReco2 = -1;
    for (int l=0; l<otherJets->GetEntries (); l++ ){ //--- loop over otherJets ----
     TLorentzVector* myparticle = (TLorentzVector*) otherJets->At(l);
     double DRGenReco1 = ROOT::Math::VectorUtil::DeltaR (momentum_quark1.BoostVector(), myparticle->BoostVector()) ;
     double DRGenReco2 = ROOT::Math::VectorUtil::DeltaR (momentum_quark2.BoostVector(), myparticle->BoostVector()) ;
     if (DRGenReco1<minDRQuarkReco1) {
      numDRQuarkReco1 = l;
      minDRQuarkReco1 = DRGenReco1;
     }
     if (DRGenReco2<minDRQuarkReco2) {
      numDRQuarkReco2 = l;
      minDRQuarkReco2 = DRGenReco2;
     }
    } //--- end loop over otherJets ----
    if (numDRQuarkReco1 != -1 && numDRQuarkReco2 != -1 && numDRQuarkReco2!=numDRQuarkReco1) {
     genJetRecoQuarkDR_pair.first = minDRQuarkReco1;
     genJetRecoQuarkDR_pair.second = minDRQuarkReco2;
    }
    //---- end recoJet tagging with quark ----  
   }
  }//---- end only VBF H_WW events
  
  ///---- GenJetTagged && recoJets (otherJets) ----
  double minDRGenReco1 = 1000;
  double minDRGenReco2 = 1000;
  int numDRGenReco1 = -1;
  int numDRGenReco2 = -1;

  if (genJetTagged_pair.first!=-1 && genJetTagged_pair.second!=-1){
   TLorentzVector* genJetTagged1 = (TLorentzVector*) genJets->At(genJetTagged_pair.first);
   TLorentzVector* genJetTagged2 = (TLorentzVector*) genJets->At(genJetTagged_pair.second);

   for (int l=0; l<otherJets->GetEntries (); l++ ){ //--- loop over otherJets ----
    TLorentzVector* myparticle = (TLorentzVector*) otherJets->At(l);
   
    double DRGenReco1 = ROOT::Math::VectorUtil::DeltaR (genJetTagged1->BoostVector(), myparticle->BoostVector()) ;
    double DRGenReco2 = ROOT::Math::VectorUtil::DeltaR (genJetTagged2->BoostVector(), myparticle->BoostVector()) ;
    if (DRGenReco1<minDRGenReco1) {
     numDRGenReco1 = l;
     minDRGenReco1 = DRGenReco1;
    }
    if (DRGenReco2<minDRGenReco2) {
     numDRGenReco2 = l;
     minDRGenReco2 = DRGenReco2;
    }
   } //--- end loop over otherJets ----
  
   if (numDRGenReco1 != -1 && numDRGenReco2 != -1 && numDRGenReco2!=numDRGenReco1) {
    if (debug_){
     std::cerr << "match 2 genJets " << numDRGenReco1 << " and " << numDRGenReco2 << std::endl;
     std::cerr << "           DR = " << minDRGenReco1 << " and " << minDRGenReco2 << std::endl;
    }  
    TLorentzVector* myJet1 = (TLorentzVector*) otherJets->At(numDRGenReco1);
    TLorentzVector* myJet2 = (TLorentzVector*) otherJets->At(numDRGenReco2);
  
    if (minDRGenReco1<maxDR && minDRGenReco2<maxDR){//---- DRmax
     recoJetTagged_pair.first = numDRGenReco1;
     recoJetTagged_pair.second = numDRGenReco2;
    }//---- end DRmax
   }
  }
  
  ///---- recoJets check ----

  for (int l=0; l<otherJets->GetEntries (); l++ ){ //--- loop over otherJets ----
   if (IdEvent==123 || IdEvent==124){
    TLorentzVector* myparticle_1 = (TLorentzVector*) otherJets->At(l);
    if (l==recoJetTagged_pair.first || l==recoJetTagged_pair.second) match = 1;
    else match = -1;
    px_jet = myparticle_1->Px();
    py_jet = myparticle_1->Py();
    pz_jet = myparticle_1->Pz();    
    E_jet = myparticle_1->E();    
    tNN.Fill();
   }
  }
  ////-----------------------

 }
 
 tNN.Write();
 
 outFile.Close();
  
 return 0 ;
}

