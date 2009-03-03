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

 TTree tValues("tValues","tValues");
 tValues.Branch("numentries",&numentries,"numentries/I");
 tValues.Branch("numentriesVBF",&numentriesVBF,"numentriesVBF/I");
 tValues.Branch("numTagRecoPtMax",&numTagRecoPtMax,"numTagRecoPtMax/I");
 tValues.Branch("numTagRecoMjjMax",&numTagRecoMjjMax,"numTagRecoMjjMax/I");
 
 TH2F hMjjPt("hMjjPt","reco synoptic table Mjj vs PtMax",2,0,2,2,0,2);
 
 TH1F hRecoJetNum("hRecoJetNum","Number of RecoJet ",100,0,100);
 
 TH1F hGenJetTagDR("hGenJetTagDR","GenJet tagged #Delta R",1000,0,2);
 TH1F hJetTagDR("hJetTagDR","Jet tagged #Delta R",1000,0,2);
 TH1F hRecoJetQuarkTagDR("hRecoJetQuarkTagDR","recoJet and Quark tagged #Delta R",1000,0,2);
 
 TH1F hTagGenJetPtMax("hTagGenJetPtMax","Tagged GenJet pt of the most energetic jet",1000,0,1000);
 TH1F hTagGenJetPtMin("hTagGenJetPtMin","Tagged GenJet pt of the least energetic jet",1000,0,1000);
 TH1F hTagGenJetPt("hTagGenJetPt","Tagged GenJet pt",1000,0,1000);
 
 TH1F hGenJetPt("hGenJetPt","GenJet pt",1000,0,1000);
 TH1F hGenJetPtVBFHWW("hGenJetPtVBFHWW","GenJet pt of all jets in a VBF Higgs WW event",1000,0,1000);

 TH1F hGenJetMjj_Tag("hGenJetMjj_Tag","Tagged GenJet Mjj",10000,-10,2000); 
 TH1F hGenJetMjj("hGenJetMjj","GenJet Mjj",10000,-10,2000);
 TH1F hRecoJetMjj_Tag("hRecoJetMjj_Tag","Tagged recoJet Mjj",10000,-10,2000); 
 TH1F hRecoJetMjj("hRecoJetMjj","recoJet Mjj",10000,-10,2000); 
 TH1F hRecoJetMjj_VBF("hRecoJetMjj_VBF","recoJet Mjj VBF event",10000,-10,2000); 
 
 TH1F hTagGenJetMj("hTagGenJetMj","Tagged GenJet M",10000,-10,2000); 
 TH1F hGenJetMj("hGenJetMj","GenJet M Jet",10000,-10,2000);
 TH1F hRecoJetMj("hRecoJetMj","recoJet M Jet",10000,-10,2000);
 TH1F hRecoJetMj_VBF("hRecoJetMj_VBF","recoJet M Jet VBF",10000,-10,2000);
 TH1F hRecoJetMj_Tag("hRecoJetMj_Tag","recoJet Tagged M Jet",10000,-10,2000);
 
 
 TH1F hGenJetEta("hGenJetEta","GenJet eta",1000,-10,10);
 TH1F hGenJetEtaVBFHWW("hGenJetEtaVBFHWW","GenJet eta of all jets in a VBF Higgs WW event",1000,-10,10);
 TH1F hGenJetTagEtaVBFHWW("hGenJetTagEtaVBFHWW","GenJet eta of tag jets in a VBF Higgs WW event",1000,-10,10);
 TH2F hGenJetTagEtaVBFHWW_2D("hGenJetTagEtaVBFHWW_2D","GenJet eta-eta of tag jets in a VBF Higgs WW event",200,-10,10,200,-10,10);
 TH2F hQuarkTagEtaVBFHWW_2D("hQuarkTagEtaVBFHWW_2D","Quark eta-eta of tag quarks in a VBF Higgs WW event",200,-10,10,200,-10,10);
 TH1F hGenJetTagDeltaEtaVBFHWW("hGenJetTagDeltaEtaVBFHWW","GenJet #Delta #eta of tag jets in a VBF Higgs WW event",1000,0,20);
 TH1F hJetTagDeltaEtaVBFHWW("hJetTagDeltaEtaVBFHWW","recoJet #Delta #eta of tag jets in a VBF Higgs WW event",1000,0,20);
 
 TH1F hGenJetZeppenfeld("hGenJetZeppenfeld","GenJet zeppenfeld variable",1000,-10,10);
 TH1F hTagJetZeppenfeld("hTagJetZeppenfeld","recoJet zeppenfeld variable of tagged gets",1000,-10,10);
 
   
   
 TH1F hTagJetPtMax("hTagJetPtMax","Tagged Jet pt of the most energetic jet",1000,0,1000);
 TH1F hTagJetPtMin("hTagJetPtMin","Tagged Jet pt of the least energetic jet",1000,0,1000);
 TH1F hTagJetPt("hTagJetPt","Tagged Jet pt",1000,0,1000);
 
 TH1F hRatioTagJetPtRecoGen("hRatioTagJetPtRecoGen","Ratio Jet pt reco / gen",100,0,2);
 
 
 
 
 TH2F hDRGenJetAndRecoJetQuarkTagging_1_2D("hDRGenJetAndRecoJetQuarkTagging_1_2D","#Delta R Quark-GenJet and Quark-RecoJet. Quark 1",1000,0,4,1000,0,4);
 TH2F hDRGenJetAndRecoJetQuarkTagging_2_2D("hDRGenJetAndRecoJetQuarkTagging_2_2D","#Delta R Quark-GenJet and Quark-RecoJet. Quark 2",1000,0,4,1000,0,4);
 TH2F hDRRecoGenQuarkAndRecoQuarkTagging_1_2D("hDRRecoGenQuarkAndRecoQuarkTagging_1_2D","#Delta R Quark-RecoJet and Quark-(GenJet)-RecoJet. Quark 1",1000,0,4,1000,0,4);
 TH2F hDRRecoGenQuarkAndRecoQuarkTagging_2_2D("hDRRecoGenQuarkAndRecoQuarkTagging_2_2D","#Delta R Quark-RecoJet and Quark-(GenJet)-RecoJet. Quark 2",1000,0,4,1000,0,4);
 
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
   hRecoJetNum.Fill(otherJets->GetEntries ());
   
   if (genParticles->GetEntries () > 7){//--- quite sure for VBF
    TParticle* myparticle_quark1 = (TParticle*) genParticles->At(6);
    TLorentzVector momentum_quark1;
    myparticle_quark1->Momentum(momentum_quark1);
    TParticle* myparticle_quark2 = (TParticle*) genParticles->At(7);
    TLorentzVector momentum_quark2;
    myparticle_quark2->Momentum(momentum_quark2);     

    hQuarkTagEtaVBFHWW_2D.Fill(momentum_quark1.Eta(),momentum_quark2.Eta());
      

      

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
//     unique( Vect_PairQuarkGenJet.begin(), Vect_PairQuarkGenJet.end(),vbfhww2l::DeltaRSameFirstTLorentzVector() );
//     unique( Vect_PairQuarkGenJet.begin(), Vect_PairQuarkGenJet.end(),vbfhww2l::DeltaRSameSecondTLorentzVector() );
    unique( Vect_PairQuarkGenJet.begin(), Vect_PairQuarkGenJet.end(),vbfhww2l::DeltaRSameFirstOrSecondTLorentzVector() );
    
    
    if (Vect_PairQuarkGenJet.size()>2){
     minDR_J1 = ROOT::Math::VectorUtil::DeltaR(Vect_PairQuarkGenJet.at(0).first->BoostVector(),Vect_PairQuarkGenJet.at(0).second->BoostVector());
     minDR_J2 = ROOT::Math::VectorUtil::DeltaR(Vect_PairQuarkGenJet.at(1).first->BoostVector(),Vect_PairQuarkGenJet.at(1).second->BoostVector());       
    }
       
 
    //---- histograms ----   
    hGenJetTagDR.Fill(minDR_J1);
    hGenJetTagDR.Fill(minDR_J2);
 
    if (minDR_J1<maxDR && minDR_J2<maxDR){//---- DRmax
     
     double pt1 = Vect_PairQuarkGenJet.at(0).first->Pt();
     double pt2 = Vect_PairQuarkGenJet.at(1).first->Pt();
      
     if (pt1 > pt2){
      hTagGenJetPtMin.Fill(pt2);
      hTagGenJetPtMax.Fill(pt1);
     }
     else {
      hTagGenJetPtMin.Fill(pt1);
      hTagGenJetPtMax.Fill(pt2);
     }
     hTagGenJetPt.Fill(pt1);
     hTagGenJetPt.Fill(pt2);
      
      
     hGenJetMjj_Tag.Fill((*(Vect_PairQuarkGenJet.at(0).first) + *(Vect_PairQuarkGenJet.at(1).first)).M());
     hTagGenJetMj.Fill((*(Vect_PairQuarkGenJet.at(0).first)).M());
     hTagGenJetMj.Fill((*(Vect_PairQuarkGenJet.at(1).first)).M());
      
     hGenJetTagEtaVBFHWW.Fill(Vect_PairQuarkGenJet.at(0).first->Eta());
     hGenJetTagEtaVBFHWW.Fill(Vect_PairQuarkGenJet.at(1).first->Eta());
     hGenJetTagEtaVBFHWW_2D.Fill(Vect_PairQuarkGenJet.at(0).first->Eta(),Vect_PairQuarkGenJet.at(1).first->Eta());
      
     hGenJetTagDeltaEtaVBFHWW.Fill(fabs(Vect_PairQuarkGenJet.at(0).first->Eta()-Vect_PairQuarkGenJet.at(1).first->Eta()));
        
     hGenJetZeppenfeld.Fill(Vect_PairQuarkGenJet.at(0).first->Eta() - (Vect_PairQuarkGenJet.at(0).first->Eta() + Vect_PairQuarkGenJet.at(1).first->Eta())/2.);
     hGenJetZeppenfeld.Fill(Vect_PairQuarkGenJet.at(1).first->Eta() - (Vect_PairQuarkGenJet.at(0).first->Eta() + Vect_PairQuarkGenJet.at(1).first->Eta())/2.);
      
    }//---- end DRmax
    
    
    
    
    
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
//     unique( Vect_PairQuarkRecoJet.begin(), Vect_PairQuarkRecoJet.end(),vbfhww2l::DeltaRSameFirstTLorentzVector() );
//     unique( Vect_PairQuarkRecoJet.begin(), Vect_PairQuarkRecoJet.end(),vbfhww2l::DeltaRSameSecondTLorentzVector() );
    unique( Vect_PairQuarkRecoJet.begin(), Vect_PairQuarkRecoJet.end(),vbfhww2l::DeltaRSameFirstOrSecondTLorentzVector() );
    
    if (Vect_PairQuarkRecoJet.size()>2){
     minDRQuarkReco1 = ROOT::Math::VectorUtil::DeltaR(Vect_PairQuarkRecoJet.at(0).first->BoostVector(),Vect_PairQuarkRecoJet.at(0).second->BoostVector());
     minDRQuarkReco2 = ROOT::Math::VectorUtil::DeltaR(Vect_PairQuarkRecoJet.at(1).first->BoostVector(),Vect_PairQuarkRecoJet.at(1).second->BoostVector());       
    }
   
    //---- histograms ----

    if (minDR_J1<maxDR && minDR_J2<maxDR){//---- DRmax
     hDRGenJetAndRecoJetQuarkTagging_1_2D.Fill(minDRQuarkReco1,minDR_J1);
     hDRGenJetAndRecoJetQuarkTagging_2_2D.Fill(minDRQuarkReco2,minDR_J2);
     hRecoJetQuarkTagDR.Fill(minDRQuarkReco1);
     hRecoJetQuarkTagDR.Fill(minDRQuarkReco2);     
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
//      unique( Vect_PairQuarkGenJetRecoJet.begin(), Vect_PairQuarkGenJetRecoJet.end(),vbfhww2l::DeltaRSameFirstTLorentzVector() );
//      unique( Vect_PairQuarkGenJetRecoJet.begin(), Vect_PairQuarkGenJetRecoJet.end(),vbfhww2l::DeltaRSameSecondTLorentzVector() );
     unique( Vect_PairQuarkGenJetRecoJet.begin(), Vect_PairQuarkGenJetRecoJet.end(),vbfhww2l::DeltaRSameFirstOrSecondTLorentzVector() );
     
     if (Vect_PairQuarkGenJetRecoJet.size()>2){
      minDRGenReco1 = ROOT::Math::VectorUtil::DeltaR(Vect_PairQuarkGenJetRecoJet.at(0).first->BoostVector(),Vect_PairQuarkGenJetRecoJet.at(0).second->BoostVector());
      minDRGenReco2 = ROOT::Math::VectorUtil::DeltaR(Vect_PairQuarkGenJetRecoJet.at(1).first->BoostVector(),Vect_PairQuarkGenJetRecoJet.at(1).second->BoostVector());
      
      DRQuarkGenReco1 = std::min(ROOT::Math::VectorUtil::DeltaR (momentum_quark1.BoostVector(), Vect_PairQuarkGenJetRecoJet.at(0).first->BoostVector()),ROOT::Math::VectorUtil::DeltaR (momentum_quark2.BoostVector(), Vect_PairQuarkGenJetRecoJet.at(0).first->BoostVector())) ;
      DRQuarkGenReco2 = std::min(ROOT::Math::VectorUtil::DeltaR (momentum_quark1.BoostVector(), Vect_PairQuarkGenJetRecoJet.at(1).first->BoostVector()),ROOT::Math::VectorUtil::DeltaR (momentum_quark2.BoostVector(), Vect_PairQuarkGenJetRecoJet.at(1).first->BoostVector()));
     }
    
    //---- histograms ----
     hJetTagDR.Fill(minDRGenReco1);
     hJetTagDR.Fill(minDRGenReco2);
    
  
     if (minDRGenReco1<maxDR && minDRGenReco2<maxDR){//---- DRmax
      
      double pt1 = Vect_PairQuarkGenJetRecoJet.at(0).first->Pt();
      double pt2 = Vect_PairQuarkGenJetRecoJet.at(1).first->Pt();
      
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
      if (Vect_PairQuarkGenJetRecoJet.at(0).first->Pt()!=0) hRatioTagJetPtRecoGen.Fill(pt1 / Vect_PairQuarkGenJetRecoJet.at(0).first->Pt());
      if (Vect_PairQuarkGenJetRecoJet.at(1).first->Pt()!=0) hRatioTagJetPtRecoGen.Fill(pt2 / Vect_PairQuarkGenJetRecoJet.at(1).first->Pt());
     
      hRecoJetMj_Tag.Fill(Vect_PairQuarkGenJetRecoJet.at(0).first->M());
      hRecoJetMj_Tag.Fill(Vect_PairQuarkGenJetRecoJet.at(1).first->M());
    
      hTagJetZeppenfeld.Fill(Vect_PairQuarkGenJetRecoJet.at(1).first->Eta() - (Vect_PairQuarkGenJetRecoJet.at(0).first->Eta() + Vect_PairQuarkGenJetRecoJet.at(1).first->Eta())/2.);
      hTagJetZeppenfeld.Fill(Vect_PairQuarkGenJetRecoJet.at(0).first->Eta() - (Vect_PairQuarkGenJetRecoJet.at(0).first->Eta() + Vect_PairQuarkGenJetRecoJet.at(1).first->Eta())/2.);
     
      hJetTagDeltaEtaVBFHWW.Fill(fabs(Vect_PairQuarkGenJetRecoJet.at(0).first->Eta() - Vect_PairQuarkGenJetRecoJet.at(1).first->Eta()));

      hRecoJetMjj_Tag.Fill((*(Vect_PairQuarkGenJetRecoJet.at(0).first) + *(Vect_PairQuarkGenJetRecoJet.at(1).first)).M());
     }
     
     if (Vect_PairQuarkRecoJet.size()>2){
      hDRRecoGenQuarkAndRecoQuarkTagging_1_2D.Fill(minDRQuarkReco1,DRQuarkGenReco1);
      hDRRecoGenQuarkAndRecoQuarkTagging_2_2D.Fill(minDRQuarkReco2,DRQuarkGenReco2);
     }
    }
    
    
    
    
    
    
   }//--- end quite sure for VBF
  }//---- end only VBF H_WW events
  

    
  ///---- genJet distributions ----
  for (int k=0; k<genJets->GetEntries (); k++ ){ //--- loop over genJet ----
   TLorentzVector* myparticle = (TLorentzVector*) genJets->At(k);
   hGenJetPt.Fill(myparticle->Pt());
   hGenJetMj.Fill(myparticle->M());
   for (int l=k+1; l<genJets->GetEntries (); l++ ){ //--- loop over genJet ----
    TLorentzVector* myparticle_2 = (TLorentzVector*) genJets->At(l);
    hGenJetMjj.Fill((*myparticle + *myparticle_2).M());
   }
   if (IdEvent==123 || IdEvent==124) {
    hGenJetPtVBFHWW.Fill(myparticle->Pt());
    hGenJetEtaVBFHWW.Fill(myparticle->Eta());
   }
   hGenJetEta.Fill(myparticle->Eta());
  } //--- end loop over genJet ----

  ///---- recoJets plots ----

  for (int l=0; l<otherJets->GetEntries (); l++ ){ //--- loop over otherJets ----
   TLorentzVector* myparticle_1 = (TLorentzVector*) otherJets->At(l);
   hRecoJetMj.Fill(myparticle_1->M());
   if (IdEvent==123 || IdEvent==124) hRecoJetMj_VBF.Fill(myparticle_1->M());
   for (int m=l+1; m<otherJets->GetEntries (); m++ ){ //--- loop over otherJets ----
    TLorentzVector* myparticle_2 = (TLorentzVector*) otherJets->At(m);
    hRecoJetMjj.Fill((*myparticle_1 + *myparticle_2).M());
    if (IdEvent==123 || IdEvent==124) hRecoJetMjj_VBF.Fill((*myparticle_1 + *myparticle_2).M());
   }
  }
 
 ///-----------------------
 ///---- reco analysis ---- 
 ///-----------------------

  if (IdEvent==123 || IdEvent==124) { //--- if VBF
   int TagReco_1 = -1;
   int TagReco_2 = -1;
   
   ///---- find number of selected reco ----
   if (Vect_PairQuarkGenJetRecoJet.size()>2){
    for (int l=0; l<otherJets->GetEntries (); l++ ){ //--- loop over otherJets ----
     TLorentzVector* myRecoJet = (TLorentzVector*) otherJets->At(l);   
//      if (ROOT::Math::VectorUtil::DeltaR(Vect_PairQuarkGenJetRecoJet.at(0).first->BoostVector(),myRecoJet->BoostVector()) < 0.00001) TagReco_1 = l;
//      if (ROOT::Math::VectorUtil::DeltaR(Vect_PairQuarkGenJetRecoJet.at(1).first->BoostVector(),myRecoJet->BoostVector()) < 0.00001) TagReco_2 = l;
     
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
   
   
  ///---- Mjj ----
   std::pair<int,int> tagJets_Mjj;
   tagJets_Mjj.first = -1;
   tagJets_Mjj.second = -1;
   double maxInvMass = 0. ;
   for (int l=0; l<otherJets->GetEntries (); l++ ){ //--- loop over otherJets ----
    TLorentzVector* myparticle_1 = (TLorentzVector*) otherJets->At(l);
    for (int m=l+1; m<otherJets->GetEntries (); m++ ){ //--- loop over otherJets ----
     TLorentzVector* myparticle_2 = (TLorentzVector*) otherJets->At(m);
     double InvMass = (*myparticle_1 + *myparticle_2).M();
     if (InvMass > maxInvMass){
      maxInvMass = InvMass;
     //---- pt order
      if (myparticle_1->Pt() > myparticle_2->Pt()){
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
    TLorentzVector* myRecoJet_1 = (TLorentzVector*) otherJets->At(tagJets_Mjj.first);   
    TLorentzVector* myRecoJet_2 = (TLorentzVector*) otherJets->At(tagJets_Mjj.second);
    if ((std::min(ROOT::Math::VectorUtil::DeltaR(Vect_PairQuarkGenJetRecoJet.at(0).first->BoostVector(),myRecoJet_1->BoostVector()),ROOT::Math::VectorUtil::DeltaR(Vect_PairQuarkGenJetRecoJet.at(1).first->BoostVector(),myRecoJet_1->BoostVector()))< 0.01) && (std::min(ROOT::Math::VectorUtil::DeltaR(Vect_PairQuarkGenJetRecoJet.at(0).first->BoostVector(),myRecoJet_2->BoostVector()),ROOT::Math::VectorUtil::DeltaR(Vect_PairQuarkGenJetRecoJet.at(1).first->BoostVector(),myRecoJet_2->BoostVector()))< 0.01)){
     numTagRecoMjjMax++;
    }
   }
   
   
  
  ///---- Pt ----
   std::pair<int,int> tagJets_Pt;
   tagJets_Pt.first = -1;
   tagJets_Pt.second = -1;
   double ptMax1 = 0. ;
   double ptMax2 = 0. ;
   int maxNum1 = -1;
   int maxNum2 = -1;

// //    for (int l=0; l<otherJets->GetEntries (); l++ ){ //--- loop over otherJets ----
// //     TLorentzVector* myparticle_1 = (TLorentzVector*) otherJets->At(l);
// //     if (myparticle_1->Pt() > ptMax2)
// //     {
// //      if (myparticle_1->Pt() > ptMax1){
// //       maxNum2 = maxNum1;
// //       ptMax2 = ptMax1;
// //       maxNum1 = l;
// //       ptMax1 = myparticle_1->Pt() ;
// //      } 
// //      else {
// //       maxNum2 = l;
// //       ptMax2 = myparticle_1->Pt() ;
// //      }
// //     }
// //    }
   
   for (int l=0; l<otherJets->GetEntries (); l++ ){ //--- loop over otherJets ----
    TLorentzVector* myparticle_1 = (TLorentzVector*) otherJets->At(l);
    if (myparticle_1->Pt() > ptMax1)
    {
     maxNum1 = l;
     maxNum2 = maxNum1;
     ptMax2 = ptMax1;
     ptMax1 = myparticle_1->Pt() ;
    } 
    else if (myparticle_1->Pt() > ptMax2) 
    {
     maxNum2 = l;
     ptMax2 = myparticle_1->Pt() ;
    }
   }

   tagJets_Pt.first = maxNum1;
   tagJets_Pt.second = maxNum2;
   
   if (tagJets_Pt.first != -1 && tagJets_Pt.second != -1){
    TLorentzVector* myRecoJet_1 = (TLorentzVector*) otherJets->At(maxNum1);   
    TLorentzVector* myRecoJet_2 = (TLorentzVector*) otherJets->At(maxNum2);
    if (tagJets_Pt.first != -1 && tagJets_Pt.second != -1 && (std::min(ROOT::Math::VectorUtil::DeltaR(Vect_PairQuarkGenJetRecoJet.at(0).first->BoostVector(),myRecoJet_1->BoostVector()),ROOT::Math::VectorUtil::DeltaR(Vect_PairQuarkGenJetRecoJet.at(1).first->BoostVector(),myRecoJet_1->BoostVector()))< 0.01) && (std::min(ROOT::Math::VectorUtil::DeltaR(Vect_PairQuarkGenJetRecoJet.at(0).first->BoostVector(),myRecoJet_2->BoostVector()),ROOT::Math::VectorUtil::DeltaR(Vect_PairQuarkGenJetRecoJet.at(1).first->BoostVector(),myRecoJet_2->BoostVector()))< 0.01)){
     numTagRecoPtMax++;
    }
   }
  
   
   if ((((
          (tagJets_Pt.first == TagReco_1) 
          && (tagJets_Pt.second == TagReco_2))
          && ((tagJets_Mjj.first == TagReco_1)
          && (tagJets_Mjj.second == TagReco_2))))
          && (tagJets_Mjj.first != -1 && tagJets_Mjj.second != -1 && tagJets_Pt.first != -1 && tagJets_Pt.second != -1)) hMjjPt.Fill(1.,1.);
   
   if ((((
          (tagJets_Pt.first == TagReco_1)
          && (tagJets_Pt.second == TagReco_2))
          && !((tagJets_Mjj.first == TagReco_1)
          && (tagJets_Mjj.second == TagReco_2))))
          && (tagJets_Mjj.first != -1 && tagJets_Mjj.second != -1 && tagJets_Pt.first != -1 && tagJets_Pt.second != -1)) hMjjPt.Fill(1.,0.);

   if (((!(
           (tagJets_Pt.first == TagReco_1)
           && (tagJets_Pt.second == TagReco_2))
           && ((tagJets_Mjj.first == TagReco_1)
           && (tagJets_Mjj.second == TagReco_2))))
           && (tagJets_Mjj.first != -1 && tagJets_Mjj.second != -1 && tagJets_Pt.first != -1 && tagJets_Pt.second != -1)) hMjjPt.Fill(0.,1.);
   
   if (((!(
           (tagJets_Pt.first == TagReco_1)
           && (tagJets_Pt.second == TagReco_2))
           && !((tagJets_Mjj.first == TagReco_1)
           && (tagJets_Mjj.second == TagReco_2))))
           && (tagJets_Mjj.first != -1 && tagJets_Mjj.second != -1 && tagJets_Pt.first != -1 && tagJets_Pt.second != -1)) hMjjPt.Fill(0.,0.);
   
  } //--- end if VBF
  ////-----------------------

 }//---- end loop over entries 

 
 
 hMjjPt.Write();
 
 hGenJetTagDR.Write();
 hJetTagDR.Write();  
 hRecoJetQuarkTagDR.Write();
 
 hTagGenJetPtMin.Write();
 hTagGenJetPtMax.Write();
 hTagGenJetPt.Write();
 hGenJetPt.Write();
 hGenJetPtVBFHWW.Write();
  
 hGenJetMjj_Tag.Write();
 hGenJetMjj.Write();
 hRecoJetMjj_Tag.Write();
 hRecoJetMjj.Write();
 hRecoJetMjj_VBF.Write();
 
 hGenJetMj.Write();
 hTagGenJetMj.Write();
 hRecoJetMj.Write();
 hRecoJetMj_Tag.Write();
 hRecoJetMj_VBF.Write();
   
 hGenJetEta.Write();
 hGenJetEtaVBFHWW.Write();
 hGenJetTagEtaVBFHWW.Write();
 hGenJetTagEtaVBFHWW_2D.Write();
 hQuarkTagEtaVBFHWW_2D.Write();
 hGenJetTagDeltaEtaVBFHWW.Write();
 hJetTagDeltaEtaVBFHWW.Write();
 
 hGenJetZeppenfeld.Write();
 hTagJetZeppenfeld.Write();
 
 hTagJetPtMax.Write();
 hTagJetPtMin.Write();
 hTagJetPt.Write();
 hRatioTagJetPtRecoGen.Write();
 
 hDRGenJetAndRecoJetQuarkTagging_1_2D.Write();
 hDRGenJetAndRecoJetQuarkTagging_2_2D.Write();
 hDRRecoGenQuarkAndRecoQuarkTagging_1_2D.Write();
 hDRRecoGenQuarkAndRecoQuarkTagging_2_2D.Write();
 
 tValues.Fill();
 tValues.Write();
 
 hRecoJetNum.Write();
 
 outFile.Close();
  
 return 0 ;
}

