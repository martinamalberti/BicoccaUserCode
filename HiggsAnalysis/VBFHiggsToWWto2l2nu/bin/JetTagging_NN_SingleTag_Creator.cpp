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

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/bin/NNfunction.h"
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/bin/NNfunction.cxx"


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
 int numTagRecoNNMax = 0;
  
 TFile outFile(nameOutput,"recreate");

 TTree tValues("tValues","tValues");
 tValues.Branch("numentries",&numentries,"numentries/I");
 tValues.Branch("numentriesVBF",&numentriesVBF,"numentriesVBF/I");
 tValues.Branch("numTagRecoPtMax",&numTagRecoPtMax,"numTagRecoPtMax/I");
 tValues.Branch("numTagRecoMjjMax",&numTagRecoMjjMax,"numTagRecoMjjMax/I");
 tValues.Branch("numTagRecoNNMax",&numTagRecoNNMax,"numTagRecoNNMax/I");

 int flagTagRecoPtMax = 0;
 int flagTagRecoMjjMax = 0;
 int flagTagRecoNNMax = 0;

 TTree tTag("tTag","tTag");
 tTag.Branch("flagTagRecoPtMax",&flagTagRecoPtMax,"flagTagRecoPtMax/I");
 tTag.Branch("flagTagRecoMjjMax",&flagTagRecoMjjMax,"flagTagRecoMjjMax/I");
 tTag.Branch("flagTagRecoNNMax",&flagTagRecoNNMax,"flagTagRecoNNMax/I");

 
 
 double px_jet;
 double py_jet;
 double pz_jet;
 double E_jet;
 double match; //---- -1 = no, 1 = yes
 
 TTree tNN("tNN","tNN");
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
 
 
 //---- vecto of genJet tagged with MC truth with quarks from Higgs 
 //----- default <-1,-1>
 std::vector<std::pair<int,int> > genJetTagged; //---- first.pt > second.pt
 
 //---- vecto of recoJet tagged with GenJet
 std::vector<std::pair<int,int> > recoJetTagged; //---- first.pt > second.pt
 
 
 
 numentries = chain_H->GetEntries();
 for (int i=0; i<numentries; i++ ){//---- loop over entries 
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
  
  int TagReco_1 = -1;
  int TagReco_2 = -1;

  if (IdEvent==123 || IdEvent==124) { //--- if VBF
   
   ///---- find number of selected reco ----
   if (Vect_PairQuarkGenJetRecoJet.size()>2){
    for (int l=0; l<otherJets->GetEntries (); l++ ){ //--- loop over otherJets ----
     TLorentzVector* myRecoJet = (TLorentzVector*) otherJets->At(l);   
     if (ROOT::Math::VectorUtil::DeltaR(Vect_PairQuarkRecoJet.at(0).first->BoostVector(),myRecoJet->BoostVector()) < 0.00001) TagReco_1 = l;
     if (ROOT::Math::VectorUtil::DeltaR(Vect_PairQuarkRecoJet.at(1).first->BoostVector(),myRecoJet->BoostVector()) < 0.00001) TagReco_2 = l;
    
    }
    if (TagReco_1 == TagReco_2) {
     std::cerr << "Error: TagReco_1 == TagReco_2 ----- Entry: " << i << " -> " << TagReco_1 << " = " << TagReco_2 << std::endl;
    }
    
    TLorentzVector* myTagRecoJet_1 = (TLorentzVector*) otherJets->At(TagReco_1);   
    TLorentzVector* myTagRecoJet_2 = (TLorentzVector*) otherJets->At(TagReco_2);   
    
    if (myTagRecoJet_1->Pt() < myTagRecoJet_2->Pt()){
     int TagReco_temp = TagReco_1;
     TagReco_1 = TagReco_2;
     TagReco_2 = TagReco_temp;
    }
   }
  }


  for (int l=0; l<otherJets->GetEntries (); l++ ){ //--- loop over otherJets ----
   if (IdEvent==123 || IdEvent==124){
    TLorentzVector* myparticle_1 = (TLorentzVector*) otherJets->At(l);
    if ((l == TagReco_1) || (l == TagReco_2)) match = 1;
    else match = -1;
    px_jet = myparticle_1->Px();
    py_jet = myparticle_1->Py();
    pz_jet = myparticle_1->Pz();    
    E_jet = myparticle_1->E();    
    tNN.Fill();
   }
  } 
  tTag.Fill();
  ////-----------------------
 }//---- end loop over entries 


 
 tValues.Fill();
 tValues.Write();
 

 tTag.Write();
 tNN.Write();
 outFile.Close();
  
 return 0 ;
}

