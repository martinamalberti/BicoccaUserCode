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


//  ------------------------------------------------------------





//  ========== M A I N    P R O G R A M =========================

// ---- example: ./doIt_JetTag.exe 0.4 /tmp/amassiro/VBF_SimpleTree_H170_WW_2l.root


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

 
 int numentries = 0;
 int numentriesVBF = 0;
 int numTagRecoPtMax = 0;
 int numTagRecoMjjMax = 0;
  
 TFile outFile(nameOutput,"recreate");
 

 TTree tTest("tTest","tTest");
 int sizeVectPairQGJet;
 int sizeVectPairQRJet;
 int sVectPairQGRJet;
 int sizeotherJets;

 
 tTest.Branch("sizeotherJets",&sizeotherJets,"sizeotherJets/I");
 tTest.Branch("sizeVectPairQGJet",&sizeVectPairQGJet,"sizeVectPairQGJet/I");
 tTest.Branch("sizeVectPairQRJet",&sizeVectPairQRJet,"sizeVectPairQRJet/I");
 tTest.Branch("sVectPairQGRJet",&sVectPairQGRJet,"sVectPairQGRJet/I");
  
 
 double* eta_jet_1 = new double (6);
 double* eta_jet_2 = new double (6);
 double* pt_jet_1 = new double (6);
 double* pt_jet_2 = new double (6);
 double* M_jet_jet = new double (6);
 double* match_Tag = new double (6); //---- -1 = no, 1 = yes
 double match_VBF; //---- -1 = no, 1 = yes
 int numJet;
 
 
 //---- Tree definition ----
 TTree tNN("tNN","tNN");
 
 tNN.Branch("numJet",&numJet,"numJet/I");
 
 char name[100];
 char what[100];
 
 for (int i=0; i<6; i++){
  sprintf(name,"eta_jet_1_%d",i);
  sprintf(what,"eta_jet_1_%d/D",i);
  tNN.Branch(name,&eta_jet_1[i],what);
  sprintf(name,"eta_jet_2_%d",i);
  sprintf(what,"eta_jet_2_%d/D",i);
  tNN.Branch(name,&eta_jet_2[i],what);
  
  sprintf(name,"pt_jet_1_%d",i);
  sprintf(what,"pt_jet_1_%d/D",i);
  tNN.Branch(name,&pt_jet_1[i],what);
  sprintf(name,"pt_jet_2_%d",i);
  sprintf(what,"pt_jet_2_%d/D",i);
  tNN.Branch(name,&pt_jet_2[i],what);
 
  sprintf(name,"M_jet_jet_%d",i);
  sprintf(what,"M_jet_jet_%d/D",i);
  tNN.Branch(name,&M_jet_jet[i],what);
  
  sprintf(name,"match_Tag_%d",i);
  sprintf(what,"match_Tag_%d/D",i);
  tNN.Branch(name,&match_Tag[i],what);
  
 }
 
 tNN.Branch("match_VBF",&match_VBF,"match_VBF/D");

  
 //---- end Tree definition ----
 
 
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
 
 
 //---- vecto of genJet tagged with MC truth with quarks from Higgs 
 //----- default <-1,-1>
 std::vector<std::pair<int,int> > genJetTagged; //---- first.pt > second.pt
 
 //---- vecto of recoJet tagged with GenJet
 std::vector<std::pair<int,int> > recoJetTagged; //---- first.pt > second.pt
 
 
 
 numentries = chain_H->GetEntries();
 for (int i=0; i<numentries; i++ ){//---- loop over entries 
  
  
  //---- initialize ----
  for (int ii=0; ii<6;ii++){
   eta_jet_1[ii] = 0;
   pt_jet_1[ii] = 0;
   eta_jet_2[ii] = 0;
   pt_jet_2[ii] = 0;
   M_jet_jet[ii] = 0;
   match_Tag[ii] = 0;
  }
  
  std::pair<int,int> genJetTagged_pair;
  genJetTagged_pair.first = -1;
  genJetTagged_pair.second = -1;
  std::pair<double,double> genJetRecoQuarkDR_pair; //--- first and second are w.r.t. quark 6 and 7 NOT pt
  genJetRecoQuarkDR_pair.first = -1;
  genJetRecoQuarkDR_pair.second = -1;
    
  std::pair<int,int> recoJetTagged_pair;
  recoJetTagged_pair.first = -1;
  recoJetTagged_pair.second = -1;
  
  
  
  std::vector<std::pair<std::vector<TLorentzVector>::const_iterator,std::vector<TLorentzVector>::const_iterator> > Vect_PairQuarkGenJet;
  std::vector<std::pair<std::vector<TLorentzVector>::const_iterator,std::vector<TLorentzVector>::const_iterator> > Vect_PairQuarkGenJetRecoJet;   
  std::vector<std::pair<std::vector<TLorentzVector>::const_iterator,std::vector<TLorentzVector>::const_iterator> > Vect_PairQuarkRecoJet;   

  
  chain_H->GetEntry(i);
  
  sizeVectPairQGJet = -1;
  sizeVectPairQRJet = -1;
  sVectPairQGRJet = -1;
  sizeotherJets = -1;


  
  numJet = otherJets->GetEntries ();
  sizeotherJets = otherJets->GetEntries ();
  
  if (IdEvent==123 || IdEvent==124){ //---- only VBF H_WW events
   numentriesVBF++;

   if (genParticles->GetEntries () > 7){//--- quite sure for VBF
    TParticle* myparticle_quark1 = (TParticle*) genParticles->At(6);
    TLorentzVector momentum_quark1;
    myparticle_quark1->Momentum(momentum_quark1);
    TParticle* myparticle_quark2 = (TParticle*) genParticles->At(7);
    TLorentzVector momentum_quark2;
    myparticle_quark2->Momentum(momentum_quark2);     


    ///---- check genJet and quarks from Higgs ----

    double minDR_J1 = 1000;    
    double minDR_J2 = 1000; 
    int numDR_J1 = -1;
    int numDR_J2 = -1;
         
    for (int k=0; k<genJets->GetEntries (); k++ ){ //--- loop over genJet ----
     TLorentzVector* myparticle = (TLorentzVector*) genJets->At(k);
     
     std::pair<TLorentzVector*,TLorentzVector*> PairQuarkGenJet_1(myparticle,&momentum_quark1);
     Vect_PairQuarkGenJet.push_back(PairQuarkGenJet_1);
     
     std::pair<TLorentzVector*,TLorentzVector*> PairQuarkGenJet_2(myparticle,&momentum_quark2);
     Vect_PairQuarkGenJet.push_back(PairQuarkGenJet_2);
    } //--- end loop over genJet ----
    
    sort( Vect_PairQuarkGenJet.begin(), Vect_PairQuarkGenJet.end(),vbfhww2l::DeltaRSortingTLorentzVector() );
    unique( Vect_PairQuarkGenJet.begin(), Vect_PairQuarkGenJet.end(),vbfhww2l::DeltaRSameFirstOrSecondTLorentzVector() );
//     unique( Vect_PairQuarkGenJet.begin(), Vect_PairQuarkGenJet.end(),vbfhww2l::DeltaRSameFirstTLorentzVector() );
//     unique( Vect_PairQuarkGenJet.begin(), Vect_PairQuarkGenJet.end(),vbfhww2l::DeltaRSameSecondTLorentzVector() );
//     unique( Vect_PairQuarkGenJet.begin(), Vect_PairQuarkGenJet.end(),vbfhww2l::DeltaRSameFirstTLorentzVector() );
//     unique( Vect_PairQuarkGenJet.begin(), Vect_PairQuarkGenJet.end(),vbfhww2l::DeltaRSameSecondTLorentzVector() );
    
    sizeVectPairQGJet = Vect_PairQuarkGenJet.size();
    
    if (Vect_PairQuarkGenJet.size()>2){
     minDR_J1 = ROOT::Math::VectorUtil::DeltaR(Vect_PairQuarkGenJet.at(0).first->BoostVector(),Vect_PairQuarkGenJet.at(0).second->BoostVector());
     minDR_J2 = ROOT::Math::VectorUtil::DeltaR(Vect_PairQuarkGenJet.at(1).first->BoostVector(),Vect_PairQuarkGenJet.at(1).second->BoostVector());       
    }
           
 
   
    ///---- recoJet tagging with quark ----
    double minDRQuarkReco1 = 1000;
    double minDRQuarkReco2 = 1000;
    int numDRQuarkReco1 = -1;
    int numDRQuarkReco2 = -1;
    for (int l=0; l<otherJets->GetEntries (); l++ ){ //--- loop over otherJets ----
     TLorentzVector* myparticle = (TLorentzVector*) otherJets->At(l);
   
     std::pair<TLorentzVector*,TLorentzVector*> PairQuarkRecoJet_1(myparticle,&momentum_quark1);
     Vect_PairQuarkRecoJet.push_back(PairQuarkRecoJet_1);
     
     std::pair<TLorentzVector*,TLorentzVector*> PairQuarkRecoJet_2(myparticle,&momentum_quark2);
     Vect_PairQuarkRecoJet.push_back(PairQuarkRecoJet_2);
    } //--- end loop over otherJets ----
     
    sort( Vect_PairQuarkRecoJet.begin(), Vect_PairQuarkRecoJet.end(),vbfhww2l::DeltaRSortingTLorentzVector() );
    unique( Vect_PairQuarkRecoJet.begin(), Vect_PairQuarkRecoJet.end(),vbfhww2l::DeltaRSameFirstOrSecondTLorentzVector() );
//     unique( Vect_PairQuarkRecoJet.begin(), Vect_PairQuarkRecoJet.end(),vbfhww2l::DeltaRSameFirstTLorentzVector() );
//     unique( Vect_PairQuarkRecoJet.begin(), Vect_PairQuarkRecoJet.end(),vbfhww2l::DeltaRSameSecondTLorentzVector() );
//     unique( Vect_PairQuarkRecoJet.begin(), Vect_PairQuarkRecoJet.end(),vbfhww2l::DeltaRSameFirstTLorentzVector() );
//     unique( Vect_PairQuarkRecoJet.begin(), Vect_PairQuarkRecoJet.end(),vbfhww2l::DeltaRSameSecondTLorentzVector() );
    
    sizeVectPairQRJet = Vect_PairQuarkRecoJet.size();
    
    if (Vect_PairQuarkRecoJet.size()>2){
     minDRQuarkReco1 = ROOT::Math::VectorUtil::DeltaR(Vect_PairQuarkRecoJet.at(0).first->BoostVector(),Vect_PairQuarkRecoJet.at(0).second->BoostVector());
     minDRQuarkReco2 = ROOT::Math::VectorUtil::DeltaR(Vect_PairQuarkRecoJet.at(1).first->BoostVector(),Vect_PairQuarkRecoJet.at(1).second->BoostVector());       
    }
   
    //---- histograms ----

    if (minDR_J1<maxDR && minDR_J2<maxDR){//---- DRmax
     genJetRecoQuarkDR_pair.first = minDRQuarkReco1;
     genJetRecoQuarkDR_pair.second = minDRQuarkReco2;
    }
    
    
    
     ///---- GenJetTagged && recoJets (otherJets) ----
    double minDRGenReco1 = 1000;
    double minDRGenReco2 = 1000;
    double DRQuarkGenReco1 = 1000;
    double DRQuarkGenReco2 = 1000;
    int numDRGenReco1 = -1;
    int numDRGenReco2 = -1;

    if (Vect_PairQuarkGenJet.size()>2){ //---- only if 2 match Quark-GenJet
     TLorentzVector myGenJet_1 = *(Vect_PairQuarkGenJet.at(0).first);
     TLorentzVector myGenJet_2 = *(Vect_PairQuarkGenJet.at(1).first);
     
     for (int l=0; l<otherJets->GetEntries (); l++ ){ //--- loop over otherJets ----
      TLorentzVector* myparticle = (TLorentzVector*) otherJets->At(l);

      
      std::pair<TLorentzVector*,TLorentzVector*> PairQuarkGenJetRecoJet_1(myparticle,&myGenJet_1);
      Vect_PairQuarkGenJetRecoJet.push_back(PairQuarkGenJetRecoJet_1);
     
      std::pair<TLorentzVector*,TLorentzVector*> PairQuarkGenJetRecoJet_2(myparticle,&myGenJet_2);
      Vect_PairQuarkGenJetRecoJet.push_back(PairQuarkGenJetRecoJet_2);
     } //--- end loop over otherJets ----
     
     sort( Vect_PairQuarkGenJetRecoJet.begin(), Vect_PairQuarkGenJetRecoJet.end(),vbfhww2l::DeltaRSortingTLorentzVector() );
     unique( Vect_PairQuarkGenJetRecoJet.begin(), Vect_PairQuarkGenJetRecoJet.end(),vbfhww2l::DeltaRSameFirstOrSecondTLorentzVector() );
//      unique( Vect_PairQuarkGenJetRecoJet.begin(), Vect_PairQuarkGenJetRecoJet.end(),vbfhww2l::DeltaRSameFirstTLorentzVector() );
//      unique( Vect_PairQuarkGenJetRecoJet.begin(), Vect_PairQuarkGenJetRecoJet.end(),vbfhww2l::DeltaRSameSecondTLorentzVector() );
//      unique( Vect_PairQuarkGenJetRecoJet.begin(), Vect_PairQuarkGenJetRecoJet.end(),vbfhww2l::DeltaRSameFirstTLorentzVector() );
//      unique( Vect_PairQuarkGenJetRecoJet.begin(), Vect_PairQuarkGenJetRecoJet.end(),vbfhww2l::DeltaRSameSecondTLorentzVector() );
    
     sVectPairQGRJet = Vect_PairQuarkGenJetRecoJet.size();
     
     if (Vect_PairQuarkGenJetRecoJet.size()>2){
      minDRGenReco1 = ROOT::Math::VectorUtil::DeltaR(Vect_PairQuarkGenJetRecoJet.at(0).first->BoostVector(),Vect_PairQuarkGenJetRecoJet.at(0).second->BoostVector());
      minDRGenReco2 = ROOT::Math::VectorUtil::DeltaR(Vect_PairQuarkGenJetRecoJet.at(1).first->BoostVector(),Vect_PairQuarkGenJetRecoJet.at(1).second->BoostVector());
      
      DRQuarkGenReco1 = std::min(ROOT::Math::VectorUtil::DeltaR (momentum_quark1.BoostVector(), Vect_PairQuarkGenJetRecoJet.at(0).first->BoostVector()),ROOT::Math::VectorUtil::DeltaR (momentum_quark2.BoostVector(), Vect_PairQuarkGenJetRecoJet.at(0).first->BoostVector())) ;
      DRQuarkGenReco2 = std::min(ROOT::Math::VectorUtil::DeltaR (momentum_quark1.BoostVector(), Vect_PairQuarkGenJetRecoJet.at(1).first->BoostVector()),ROOT::Math::VectorUtil::DeltaR (momentum_quark2.BoostVector(), Vect_PairQuarkGenJetRecoJet.at(1).first->BoostVector()));
     }
    }
    
   }//--- end quite sure for VBF
  }//---- end only VBF H_WW events
  

    

 
 ///-----------------------
 ///---- reco analysis ---- 
 ///-----------------------

  match_VBF = 0; //---- default
  
  if (IdEvent==123 || IdEvent==124) { //--- if VBF
   
   match_VBF = 1.;
   
   int TagReco_1 = -1;
   int TagReco_2 = -1;
   
 
   
  
  ///---- Pt ----
   std::pair<int,int> tagJets_Pt;
   tagJets_Pt.first = -1;
   tagJets_Pt.second = -1;
   double ptMax1 = 0. ;
   double ptMax2 = 0. ;
   int maxNum1 = -1;
   int maxNum2 = -1;

   std::vector< TLorentzVector > vect_recoJet_PtSorting;
   for (int l=0; l<otherJets->GetEntries (); l++ ){ //--- loop over otherJets ----
    TLorentzVector* myparticle = (TLorentzVector*) otherJets->At(l);
    vect_recoJet_PtSorting.push_back(*myparticle);
   }
   
//    sort( vect_recoJet_PtSorting.begin(), vect_recoJet_PtSorting.end(),vbfhww2l::PtSorting<TLorentzVector> );
   sort( vect_recoJet_PtSorting.begin(), vect_recoJet_PtSorting.end(),vbfhww2l::PtSortingTLorentzVector() );
   
   
     ///---- find number of selected reco ----
   for (int l=0; l<vect_recoJet_PtSorting.size (); l++ ){ //--- loop over otherJets ----
    TLorentzVector myRecoJet = vect_recoJet_PtSorting.at(l);
//     if (ROOT::Math::VectorUtil::DeltaR(Vect_PairQuarkGenJetRecoJet.at(0).first->BoostVector(),myRecoJet.BoostVector()) < 0.00001) TagReco_1 = l;
//     if (ROOT::Math::VectorUtil::DeltaR(Vect_PairQuarkGenJetRecoJet.at(1).first->BoostVector(),myRecoJet.BoostVector()) < 0.00001) TagReco_2 = l;
    
    if (ROOT::Math::VectorUtil::DeltaR(Vect_PairQuarkRecoJet.at(0).first->BoostVector(),myRecoJet.BoostVector()) < 0.00001) TagReco_1 = l;
    if (ROOT::Math::VectorUtil::DeltaR(Vect_PairQuarkRecoJet.at(1).first->BoostVector(),myRecoJet.BoostVector()) < 0.00001) TagReco_2 = l;
    
    
   }
   if (TagReco_1 == TagReco_2) {
    std::cerr << "Error: TagReco_1 == TagReco_2 ----- Entry: " << i << " -> " << TagReco_1 << " = " << TagReco_2 << " num RecoJet = " << numJet << " = " << vect_recoJet_PtSorting.size () << " GenJet matched = " << Vect_PairQuarkGenJet.size() << " RecoJet matched = " << Vect_PairQuarkGenJetRecoJet.size() << " GenJet == " << genJets->GetEntries () << " --- 0 Pt = " << Vect_PairQuarkGenJetRecoJet.at(0).first->BoostVector().Pt() << " 1 Pt = " << Vect_PairQuarkGenJetRecoJet.at(1).first->BoostVector().Pt() << std::endl;
    
    TParticle* myparticle_quark1 = (TParticle*) genParticles->At(6);
    TLorentzVector momentum_quark1;
    myparticle_quark1->Momentum(momentum_quark1);
    TParticle* myparticle_quark2 = (TParticle*) genParticles->At(7);
    TLorentzVector momentum_quark2;
    myparticle_quark2->Momentum(momentum_quark2);     
   
    std::cerr << "    eta Q 1 = " << momentum_quark1.Eta() << "    eta Q 2 = " << momentum_quark2.Eta() << std::endl;
   }
    
   TLorentzVector myTagRecoJet_1 = vect_recoJet_PtSorting.at(TagReco_1);   
   TLorentzVector myTagRecoJet_2 = vect_recoJet_PtSorting.at(TagReco_2);   
    
   if (myTagRecoJet_1.Pt() < myTagRecoJet_2.Pt()){
    int TagReco_temp = TagReco_1;
    TagReco_1 = TagReco_2;
    TagReco_2 = TagReco_temp;
   }

   

   ///---- make pair ----
   int counter = 0;
   for (int l=0; l<std::min((int) vect_recoJet_PtSorting.size(),4); l++ ){ 
    for (int m=l+1; m<std::min((int) vect_recoJet_PtSorting.size(),4); m++ ){
     eta_jet_1[counter] = vect_recoJet_PtSorting.at(l).Eta();
     pt_jet_1[counter] = vect_recoJet_PtSorting.at(l).Pt();
     eta_jet_2[counter] = vect_recoJet_PtSorting.at(m).Eta();
     pt_jet_2[counter] = vect_recoJet_PtSorting.at(m).Pt();
     M_jet_jet[counter] = (vect_recoJet_PtSorting.at(m) + vect_recoJet_PtSorting.at(m)).M();
     if ((l == TagReco_1) && (m == TagReco_2))
     {
      match_Tag[counter] = 1.;
     }
     else {
      match_Tag[counter] = -1.;
     }
     counter++;
    }
   }
      
   
  } //--- end if VBF
  ////-----------------------


  
  tTest.Fill();
  tNN.Fill();
 }//---- end loop over entries 

 
 tNN.Write();
 tTest.Write();
 
 outFile.Close();
  
 return 0 ;
}

