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


int main (int argc, char *argv[])
{ 
 
 bool debug_ = false;
 
 char* nameInput = new char [1000];
 char* nameOutput = new char [1000];
 
 sprintf(nameInput,"/tmp/amassiro/VBF_SimpleTree_H160_WW_2l.root");
 sprintf(nameOutput,"histoJet.root");
 
 if (argc >1){ 
  nameInput = argv[1] ; 
  if (argc >2) {
   nameOutput = argv[2] ;   
  }
 }
 
 
 std::cerr << "  nameInput = " << nameInput << std::endl;
 std::cerr << "  nameOutput = " << nameOutput << std::endl;

 
 int numentries = 0;
 int numentriesVBF = 0;
 int numVBFHWW2l2nu = 0;
 
 int numVBFHWWee2nu = 0;
 int numVBFHWWemu2nu = 0;
 int numVBFHWWmumu2nu = 0;
 
 int numVBFHWWlnuqq = 0;
 int numVBFHWWqqqq = 0;
  
 TFile outFile(nameOutput,"recreate");

 TTree tValues("tValues","tValues");
 tValues.Branch("numentries",&numentries,"numentries/I");
 tValues.Branch("numentriesVBF",&numentriesVBF,"numentriesVBF/I");
 tValues.Branch("numVBFHWW2l2nu",&numVBFHWW2l2nu,"numVBFHWW2l2nu/I");
 tValues.Branch("numVBFHWWee2nu",&numVBFHWWee2nu,"numVBFHWWee2nu/I");
 tValues.Branch("numVBFHWWemu2nu",&numVBFHWWemu2nu,"numVBFHWWemu2nu/I");
 tValues.Branch("numVBFHWWmumu2nu",&numVBFHWWmumu2nu,"numVBFHWWmumu2nu/I");
 tValues.Branch("numVBFHWWlnuqq",&numVBFHWWlnuqq,"numVBFHWWlnuqq/I");
 tValues.Branch("numVBFHWWqqqq",&numVBFHWWqqqq,"numVBFHWWqqqq/I");

 
 
 
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
 
 
 numentries = chain_H->GetEntries();
 for (int i=0; i<numentries; i++ ){//---- loop over entries 
  
  chain_H->GetEntry(i);

  if (IdEvent==123 || IdEvent==124){
   numentriesVBF++;
   
//    for (int kk=0; kk<genParticles->Size();kk++){
//     TParticle* myparticle_higgs = (TParticle*) genParticles->At(kk);
//     if (myparticle_higgs->GetPdgCode() == 25){
//      TParticle* myparticle_W_1 = (TParticle*) genParticles->At(myparticle_higgs->GetDaughter(0));
//      TParticle* myparticle_W_2 = (TParticle*) genParticles->At(myparticle_higgs->GetDaughter(1));
   //      
//      TParticle* myparticle_d_1_A = (TParticle*) genParticles->At(myparticle_W_1->GetDaughter(0));
//      TParticle* myparticle_d_1_B = (TParticle*) genParticles->At(myparticle_W_1->GetDaughter(1));
   // 
//      TParticle* myparticle_d_2_A = (TParticle*) genParticles->At(myparticle_W_2->GetDaughter(0));
//      TParticle* myparticle_d_2_B = (TParticle*) genParticles->At(myparticle_W_2->GetDaughter(1));
   // 
//      if (myparticle_d_1_A->GetPdgCode() < 7 && myparticle_d_1_B->GetPdgCode() < 7 && 
//          myparticle_d_2_A->GetPdgCode() < 7 && myparticle_d_2_B->GetPdgCode() < 7) numVBFHWWqqqq++;
   //       
//      if ((myparticle_d_1_A->GetPdgCode() < 7 && myparticle_d_1_B->GetPdgCode() < 7 && 
//           myparticle_d_2_A->GetPdgCode() > 7 && myparticle_d_2_B->GetPdgCode() > 7 ) || 
//           ((myparticle_d_1_A->GetPdgCode() > 7 && myparticle_d_1_B->GetPdgCode() > 7 && 
//           myparticle_d_2_A->GetPdgCode() < 7 && myparticle_d_2_B->GetPdgCode() < 7 ))) numVBFHWWlnuqq++;
   //       
//      if (myparticle_d_1_A->GetPdgCode() > 7 && myparticle_d_1_B->GetPdgCode() > 7 && 
//          myparticle_d_2_A->GetPdgCode() > 7 && myparticle_d_2_B->GetPdgCode() > 7) numVBFHWW2l2nu++;
//     }
//    }
   
   int pdg_1_A = 0;
   int pdg_1_B = 0;
   int pdg_2_A = 0;
   int pdg_2_B = 0;
   
//    std::cerr << "  " << std::endl;
   for (int kk=0; kk<genParticles->GetEntries();kk++){
    TParticle* myparticle_W = (TParticle*) genParticles->At(kk);
    if (myparticle_W->GetPdgCode() == 24 || myparticle_W->GetPdgCode() == -24){
//      std::cerr << " found W " << std::endl;
//      std::cerr << "    daughter 1 W  = " << myparticle_W->GetDaughter(0) << std::endl;
//      std::cerr << "    daughter 2 W  = " << myparticle_W->GetDaughter(1) << std::endl;
     if (!pdg_1_A) pdg_1_A = myparticle_W->GetDaughter(0);
     else if (!pdg_2_A) pdg_2_A = myparticle_W->GetDaughter(0);
     if (!pdg_1_B) pdg_1_B = myparticle_W->GetDaughter(1);
     else if (!pdg_2_B) pdg_2_B = myparticle_W->GetDaughter(1);
    }
   }
   
   if (pdg_1_A!=0 && pdg_1_B!=0 && pdg_2_A!=0 && pdg_2_B!=0){
    if (abs(pdg_1_A) < 7 && abs(pdg_1_B) < 7 && 
        abs(pdg_2_A) < 7 && abs(pdg_2_B) < 7) numVBFHWWqqqq++;
    if ((abs(pdg_1_A) < 7 && abs(pdg_1_B) < 7 && 
         abs(pdg_2_A) > 7 && abs(pdg_2_B) > 7 ) || 
         ((abs(pdg_1_A) > 7 && abs(pdg_1_B) > 7 && 
         abs(pdg_2_A) < 7 && abs(pdg_2_B) < 7 ))) numVBFHWWlnuqq++;
    if (abs(pdg_1_A) > 7 && abs(pdg_1_B) > 7 && 
        abs(pdg_2_A) > 7 && abs(pdg_2_B) > 7 ) numVBFHWW2l2nu++;
    
    if ((((abs(pdg_1_A) == 11 && abs(pdg_1_B) == 12) || (abs(pdg_1_A) == 12 && abs(pdg_1_B) == 11)) && 
          ((abs(pdg_2_A) == 13 && abs(pdg_2_B) == 14) || (abs(pdg_2_A) == 14 && abs(pdg_2_B) == 13))) ||
        (((abs(pdg_1_A) == 13 && abs(pdg_1_B) == 14) || (abs(pdg_1_A) == 14 && abs(pdg_1_B) == 13)) && 
           ((abs(pdg_2_A) == 11 && abs(pdg_2_B) == 12) || (abs(pdg_2_A) == 12 && abs(pdg_2_B) == 11)))
       ) numVBFHWWemu2nu++;
    
    if (((abs(pdg_1_A) == 11 && abs(pdg_1_B) == 12) || (abs(pdg_1_A) == 12 && abs(pdg_1_B) == 11)) && 
           ((abs(pdg_2_A) == 11 && abs(pdg_2_B) == 12) || (abs(pdg_2_A) == 12 && abs(pdg_2_B) == 11)))
       numVBFHWWee2nu++;    

    if (((abs(pdg_1_A) == 13 && abs(pdg_1_B) == 14) || (abs(pdg_1_A) == 14 && abs(pdg_1_B) == 13)) && 
          ((abs(pdg_2_A) == 13 && abs(pdg_2_B) == 14) || (abs(pdg_2_A) == 14 && abs(pdg_2_B) == 13)))
       numVBFHWWmumu2nu++;    
    
   }
   
  }
   
 }//---- end loop over entries 


 
 tValues.Fill();
 tValues.Write();
 
 outFile.Close();
  
 return 0 ;
}

