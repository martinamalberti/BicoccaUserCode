//COMPILO c++ -o doIt_JetTag.exe -lm `root-config --cflags --libs --glibs` -l EG doIt_JetTag.cpp 

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


int main (int argc, char *argv[])
{ 
 
 bool debug_ = false;
 
 double maxDR = 0.3;
  
 if (argc >1){
  maxDR = atof (argv[1]) ; 
 }
 
 
 std::cerr << "  maxDR = " << maxDR << std::endl;
 
 
 TFile outFile("histJet.root","recreate");

 TH1F hTagGenJetPtMax("hTagGenJetPtMax","Tagged GenJet pt of the most energetic jet",1000,0,1000);
 TH1F hTagGenJetPtMin("hTagGenJetPtMin","Tagged GenJet pt of the least energetic jet",1000,0,1000);
 TH1F hTagGenJetPt("hTagGenJetPt","Tagged GenJet pt",1000,0,1000);
 
 TH1F hGenJetPt("hGenJetPt","GenJet pt",1000,0,1000);
 TH1F hGenJetPtVBFHWW("hGenJetPtVBFHWW","GenJet pt of all jets in a VBF Higgs WW event",1000,0,1000);

 TH1F hTagJetPtMax("hTagJetPtMax","Tagged Jet pt of the most energetic jet",1000,0,1000);
 TH1F hTagJetPtMin("hTagJetPtMin","Tagged Jet pt of the least energetic jet",1000,0,1000);
 TH1F hTagJetPt("hTagJetPt","Tagged Jet pt",1000,0,1000);
 
 TH1F hRatioTagJetPtRecoGen("hRatioTagJetPtRecoGen","Ratio Jet pt reco / gen",100,0,2);
 
 
 
 TChain * chain_H = new TChain ("ntpla/VBFSimpleTree") ;
//  chain_H->Add ("/tmp/amassiro/VBF_SimpleTree_20Feb.root");
 chain_H->Add ("/tmp/amassiro/VBF_SimpleTree_H160_WW_2l.root");
 
 
 
  
 TClonesArray * genJets = new TClonesArray ("TLorentzVector") ;
 chain_H->SetBranchAddress ("genJets", &genJets) ;
 
 TClonesArray * otherJets = new TClonesArray ("TLorentzVector") ;
 chain_H->SetBranchAddress ("otherJets", &otherJets) ;
 
 TClonesArray * genParticles = new TClonesArray ("TParticle") ;
 chain_H->SetBranchAddress ("genParticles", &genParticles) ;
 
 int IdEvent;
 chain_H->SetBranchAddress ("IdEvent", &IdEvent) ;
 
 
 //---- vecto of genJet tagged with MC truth with quarks from Higgs 
 //----- default <-1,-1>
 std::vector<std::pair<int,int> > genJetTagged; //---- first.pt > second.pt
 
 
 
 
 int numentryies = chain_H->GetEntries();
 for (int i=0; i<numentryies; i++ ){
  std::pair<int,int> genJetTagged_pair;
  genJetTagged_pair.first = -1;
  genJetTagged_pair.second = -1;
  
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
     if (minDR_J1<maxDR && minDR_J1<maxDR){//---- DRmax
     
      TLorentzVector* myGenJet1 = (TLorentzVector*) genJets->At(numDR_J1);
      TLorentzVector* myGenJet2 = (TLorentzVector*) genJets->At(numDR_J2);
      
      double pt1 = myGenJet1->Pt();
      double pt2 = myGenJet2->Pt();
      
      if (pt1 > pt2){
       hTagGenJetPtMin.Fill(pt2);
       hTagGenJetPtMax.Fill(pt1);
       genJetTagged_pair.first = numDR_J1;
       genJetTagged_pair.second = numDR_J2;
      }
      else {
       hTagGenJetPtMin.Fill(pt1);
       hTagGenJetPtMax.Fill(pt2);
       genJetTagged_pair.first = numDR_J2;
       genJetTagged_pair.second = numDR_J1;
      }
      hTagGenJetPt.Fill(pt1);
      hTagGenJetPt.Fill(pt2);
     }//---- end DRmax
    }
   }
  }//---- end only VBF H_WW events
  genJetTagged.push_back(genJetTagged_pair);
  
  
  for (int k=0; k<genJets->GetEntries (); k++ ){ //--- loop over genJet ----
   TLorentzVector* myparticle = (TLorentzVector*) genJets->At(k);
   hGenJetPt.Fill(myparticle->Pt());
   if (IdEvent==123 || IdEvent==124) hGenJetPtVBFHWW.Fill(myparticle->Pt());
  } //--- end loop over genJet ----

  
  
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
    if (minDRGenReco1<maxDR && minDRGenReco2<maxDR){//---- DRmax
     
     
     TLorentzVector* myJet1 = (TLorentzVector*) otherJets->At(numDRGenReco1);
     TLorentzVector* myJet2 = (TLorentzVector*) otherJets->At(numDRGenReco2);
      
     double pt1 = myJet1->Pt();
     double pt2 = myJet2->Pt();
      
     if (pt1 > pt2){
      hTagJetPtMin.Fill(pt2);
      hTagJetPtMax.Fill(pt1);
     }
     else {
      hTagJetPtMin.Fill(pt1);
      hTagJetPtMax.Fill(pt2);
     }
     hTagJetPt.Fill(pt1);
     hTagJetPt.Fill(pt2);
     if (genJetTagged1->Pt()!=0) hRatioTagJetPtRecoGen.Fill(pt1 / genJetTagged1->Pt());
     if (genJetTagged2->Pt()!=0) hRatioTagJetPtRecoGen.Fill(pt2 / genJetTagged2->Pt());
    
    }//---- end DRmax
   }
  }

 }
 
 
  
   
  hTagGenJetPtMin.Write();
  hTagGenJetPtMax.Write();
  hTagGenJetPt.Write();
  hGenJetPt.Write();
  hGenJetPtVBFHWW.Write();
 
  hTagJetPtMax.Write();
  hTagJetPtMin.Write();
  hTagJetPt.Write();
  hRatioTagJetPtRecoGen.Write();
 
  outFile.Close();
  
  return 0 ;
 }

