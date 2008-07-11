#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

#include "TFile.h"
#include "TBranch.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "THStack.h"
#include "TLorentzVector.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"

//COMPILO c++ -o merge3 `root-config --cflags --libs --glibs` merge3.cpp 


//PG FIXME mancano i TClonesArray
//LM added CSA07 needed structure and branch
//LM added variables and branches for 'daskalakis' electron identification
struct dati 
{
   TLorentzVector * genMet4Momentum ; 
   TLorentzVector * recoMet4Momentum ; 
   double elePT[10] ;  
   double eleEta[10] ; 
   double elePhi[10] ;
   int    SCNum ; 
   double SCE[30] ;  
   double SCEta[30] ; 
   double SCPhi[30] ; 
   int    eleCharge[10] ; 
   double jetPT[50] ;  
   double jetEta[50] ; 
   double jetPhi[50] ;    
   int    jetFlav[50] ;    
   double jetPTMatch[10] ;  
   double jetEtaMatch[10] ; 
   double jetPhiMatch[10] ; 
   double SCEMatch[10] ;  
   double SCEtaMatch[10] ; 
   double SCPhiMatch[10] ; 
   double jetmaxPT[10] ;  
   double jetmaxEta[10] ; 
   double jetmaxPhi[10] ; 
   double EMjetCompon[10] ; 
   double bremFraction[10] ; 
   int    jetFlavour[10] ;  
   int    jetmaxFlavour[10] ;  
   int    rawBit[10] ;  
   int    ambiguityBit[10] ;  
   int    tkIsoBit[10] ;  
   //int    hadIsoBit[10] ; 
   int    eleIdBit[10] ;  
   int    eleIdLooseBit[10] ;  
   int    eleIdTightBit[10] ;  
   int    eleClass[10] ;  
   int    ptHat ;
   int    eleNum ;
   double eleDeltaPhi[10];
   double eleDeltaEta[10];
   double eleSigmaEtaEta[10];
   double eleHE[10];
   int eleIsBarrel[10];
   
   double ecalIsoValue[10];
   double hcalIsoValue[10];

   int MCTruthMatchBit[10];
   int pdgIdTruth[10];
   int pdgIdMother[10];
   double DelatRMatch[10];

/*   
   double transverse(double appoEta, double appoVar)
     {
     double theta = 2.* atan ( exp( - appoEta) ) ;
     double transverseVar = abs ( appoVar * sin(theta) ) ;
     return transverseVar;
     }
*/   
   struct CSA07Info {
     Int_t procId;
     Float_t ptHat;
     Float_t filterEff;
     Float_t weight;
     Int_t trigBits[90];
   } csa07Info ;  

//PG FIXME manca il lorentz vector
   TBranch * b_elePT ;  
   TBranch * b_eleEta ; 
   TBranch * b_elePhi ;
   TBranch * b_SCNum ; 
   TBranch * b_SCE ;  
   TBranch * b_SCEta ; 
   TBranch * b_SCPhi ; 
   TBranch * b_eleCharge ; 
   TBranch * b_jetPT ;  
   TBranch * b_jetEta ; 
   TBranch * b_jetPhi ;    
   TBranch * b_jetFlav ;    
   TBranch * b_jetPTMatch ;  
   TBranch * b_jetEtaMatch ; 
   TBranch * b_jetPhiMatch ; 
   TBranch * b_SCEMatch ;  
   TBranch * b_SCEtaMatch ; 
   TBranch * b_SCPhiMatch ; 
   TBranch * b_jetmaxPT ;  
   TBranch * b_jetmaxEta ; 
   TBranch * b_jetmaxPhi ; 
   TBranch * b_EMjetCompon ; 
   TBranch * b_bremFraction ; 
   TBranch * b_jetFlavour ;  
   TBranch * b_jetmaxFlavour ;  
   TBranch * b_rawBit ;  
   TBranch * b_ambiguityBit ;  
   TBranch * b_tkIsoBit ;  
   //TBranch * b_hadIsoBit ; 
   TBranch * b_eleIdBit ;  
   TBranch * b_eleIdLooseBit ;  
   TBranch * b_eleIdTightBit ;  
   TBranch * b_eleClass ;  
   TBranch * b_ptHat ;
   TBranch * b_eleNum ;  
   TBranch * b_CSA07B ;
   TBranch * b_eleHE ;
   TBranch * b_eleDeltaPhi ;
   TBranch * b_eleDeltaEta ;
   TBranch * b_eleSigmaEtaEta ;
   TBranch * b_eleIsBarrel ;
   TBranch * b_ecalIsoValue;
   TBranch * b_hcalIsoValue;
   TBranch * b_MCTruthMatchBit;
   TBranch * b_pdgIdTruth;
   TBranch * b_pdgIdMother;
   TBranch * b_DelatRMatch;

  dati (TTree * fChain) 
    {
      fChain->SetMakeClass(1);

      fChain->SetBranchAddress ("elePT" ,elePT) ;
      fChain->SetBranchAddress ("eleEta" ,eleEta) ;
      fChain->SetBranchAddress ("elePhi" ,elePhi) ;
      fChain->SetBranchAddress ("SCNum" ,&SCNum) ;
      fChain->SetBranchAddress ("SCE" ,SCE) ;  
      fChain->SetBranchAddress ("SCEta" ,SCEta) ; 
      fChain->SetBranchAddress ("SCPhi" ,SCPhi) ; 
      fChain->SetBranchAddress ("eleCharge" ,eleCharge) ;
      fChain->SetBranchAddress ("jetPT" ,jetPT) ;
      fChain->SetBranchAddress ("jetEta" ,jetEta) ;
      fChain->SetBranchAddress ("jetPhi" ,jetPhi) ;
      fChain->SetBranchAddress ("jetFlav" ,jetFlav) ;    
      fChain->SetBranchAddress ("jetPTMatch" ,jetPTMatch) ;  
      fChain->SetBranchAddress ("jetEtaMatch" ,jetEtaMatch) ; 
      fChain->SetBranchAddress ("jetPhiMatch" ,jetPhiMatch) ; 
      fChain->SetBranchAddress ("SCEMatch" ,SCEMatch) ;  
      fChain->SetBranchAddress ("SCEtaMatch" ,SCEtaMatch) ; 
      fChain->SetBranchAddress ("SCPhiMatch" ,SCPhiMatch) ; 
      fChain->SetBranchAddress ("jetmaxPT" ,jetmaxPT) ;
      fChain->SetBranchAddress ("jetmaxEta" ,jetmaxEta) ;
      fChain->SetBranchAddress ("jetmaxPhi" ,jetmaxPhi) ;
      fChain->SetBranchAddress ("EMjetCompon" ,EMjetCompon) ;
      fChain->SetBranchAddress ("bremFraction" ,bremFraction) ; 
      fChain->SetBranchAddress ("jetFlavour" ,jetFlavour) ;
      fChain->SetBranchAddress ("jetmaxFlavour" ,jetmaxFlavour) ;
      fChain->SetBranchAddress ("rawBit" ,rawBit) ;
      fChain->SetBranchAddress ("ambiguityBit" ,ambiguityBit) ;
      fChain->SetBranchAddress ("tkIsoBit" ,tkIsoBit) ;
      //fChain->SetBranchAddress ("hadIsoBit" ,hadIsoBit) ;
      fChain->SetBranchAddress ("eleIdBit" ,eleIdBit) ;
      fChain->SetBranchAddress ("eleIdLooseBit" ,eleIdLooseBit) ;
      fChain->SetBranchAddress ("eleIdTightBit" ,eleIdTightBit) ;
      fChain->SetBranchAddress ("eleClass" ,eleClass) ;
      fChain->SetBranchAddress ("ptHat" ,&ptHat) ;
      fChain->SetBranchAddress ("eleNum" ,&eleNum) ;

      fChain->SetBranchAddress ("CSA07B" ,&csa07Info) ; //questo va con o senza ampersand& ?? forse e' per questo che alcune cose non le leggiavamo
      fChain->SetBranchAddress ("eleHE" ,eleHE) ;
      fChain->SetBranchAddress ("eleDeltaPhi" ,eleDeltaPhi) ;
      fChain->SetBranchAddress ("eleDeltaEta" ,eleDeltaEta) ;
      fChain->SetBranchAddress ("eleSigmaEtaEta" ,eleSigmaEtaEta) ;
      fChain->SetBranchAddress ("eleIsBarrel" ,eleIsBarrel) ;
      fChain->SetBranchAddress ("ecalIsoValue"  ,ecalIsoValue) ;
      fChain->SetBranchAddress ("hcalIsoValue"  ,hcalIsoValue) ;
      fChain->SetBranchAddress ("MCTruthMatchBit", MCTruthMatchBit);
      fChain->SetBranchAddress ("pdgIdTruth", pdgIdTruth);
      fChain->SetBranchAddress ("pdgIdMother", pdgIdMother);
      fChain->SetBranchAddress ("DelatRMatch", DelatRMatch);


      b_elePT = fChain->GetBranch("elePT");
      b_eleEta = fChain->GetBranch("eleEta");
      b_elePhi = fChain->GetBranch("elePhi");
      b_SCNum = fChain->GetBranch("SCNum");  
      b_SCE = fChain->GetBranch("SCE");  
      b_SCEta = fChain->GetBranch("SCEta");
      b_SCPhi = fChain->GetBranch("SCPhi");  
      b_eleCharge = fChain->GetBranch("eleCharge");
      b_jetPT = fChain->GetBranch("jetPT");
      b_jetEta = fChain->GetBranch("jetEta");
      b_jetPhi = fChain->GetBranch("jetPhi");
      b_jetFlav = fChain->GetBranch("jetFlav"); 
      b_jetPTMatch = fChain->GetBranch("jetPTMatch"); 
      b_jetEtaMatch = fChain->GetBranch("jetEtaMatch"); 
      b_jetPhiMatch = fChain->GetBranch("jetPhiMatch"); 
      b_SCEMatch = fChain->GetBranch("SCEMatch"); 
      b_SCEtaMatch = fChain->GetBranch("SCEtaMatch"); 
      b_SCPhiMatch = fChain->GetBranch("SCPhiMatch"); 
      b_jetmaxPT = fChain->GetBranch("jetmaxPT");
      b_jetmaxEta = fChain->GetBranch("jetmaxEta");
      b_jetmaxPhi = fChain->GetBranch("jetmaxPhi");
      b_EMjetCompon = fChain->GetBranch("EMjetCompon");
      b_bremFraction = fChain->GetBranch("bremFraction") ;  
      b_jetFlavour = fChain->GetBranch("jetFlavour");
      b_jetmaxFlavour = fChain->GetBranch("jetmaxFlavour");
      b_rawBit = fChain->GetBranch("rawBit");
      b_ambiguityBit = fChain->GetBranch("ambiguityBit");
      b_tkIsoBit = fChain->GetBranch("tkIsoBit");
      //b_hadIsoBit = fChain->GetBranch("hadIsoBit");
      b_eleIdBit = fChain->GetBranch("eleIdBit");
      b_eleIdLooseBit = fChain->GetBranch("eleIdLooseBit");
      b_eleIdTightBit = fChain->GetBranch("eleIdTightBit");
      b_eleClass = fChain->GetBranch("eleClass");
      b_ptHat = fChain->GetBranch("ptHat");
      b_eleNum = fChain->GetBranch("eleNum");
      b_CSA07B = fChain->GetBranch("CSA07B");
      b_eleHE = fChain->GetBranch("eleHE");   
      b_eleDeltaPhi = fChain->GetBranch("eleDeltaPhi");   
      b_eleDeltaEta = fChain->GetBranch("eleDeltaEta");   
      b_eleSigmaEtaEta = fChain->GetBranch("eleSigmaEtaEta");   
      b_eleIsBarrel = fChain->GetBranch("eleIsBarrel");   
      b_ecalIsoValue = fChain->GetBranch("ecalIsoValue");
      b_hcalIsoValue = fChain->GetBranch("hcalIsoValue");
      b_MCTruthMatchBit = fChain->GetBranch("MCTruthMatchBit");
      b_pdgIdTruth = fChain->GetBranch("pdgIdTruth");
      b_pdgIdMother = fChain->GetBranch("pdgIdMother");
      b_DelatRMatch = fChain->GetBranch("DelatRMatch");
  
    }     

} ;



