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
#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"

//COMPILO c++ -o merge3 `root-config --cflags --libs --glibs` merge3.cpp 

struct dati 
{
  Double_t  elePT[10];
  Double_t  eleEta[10];
  Double_t  elePhi[10];
  Int_t     eleCharge[10];
  Double_t  jetPT[10];
  Double_t  jetEta[10];
  Double_t  jetPhi[10];
  Double_t  jetmaxPT[10];
  Double_t  jetmaxEta[10];
  Double_t  jetmaxPhi[10];
  Double_t  EMjetCompon[10];
  Int_t     jetFlavour[10];
  Int_t     jetmaxFlavour[10];
  Int_t     rawBit[10];
  Int_t     ambiguityBit[10];
  Int_t     tkIsoBit[10];
  Int_t     hadIsoBit[10];
  Int_t     eleIdBit[10];
  Int_t     eleIdLooseBit[10];
  Int_t     eleIdTightBit[10];
  Int_t     eleClass[10];
  Int_t     ptHat;
  Int_t     eleNum;

  // List of branches
  TBranch * b_elePT;   //!
  TBranch * b_eleEta;   //!
  TBranch * b_elePhi;   //!
  TBranch * b_eleCharge;   //!
  TBranch * b_jetPT;   //!
  TBranch * b_jetEta;   //!
  TBranch * b_jetPhi;   //!
  TBranch * b_jetmaxPT;   //!
  TBranch * b_jetmaxEta;   //!
  TBranch * b_jetmaxPhi;   //!
  TBranch * b_EMjetCompon;   //!
  TBranch * b_jetFlavour;   //!
  TBranch * b_jetmaxFlavour;   //!
  TBranch * b_rawBit;   //!
  TBranch * b_ambiguityBit;   //!
  TBranch * b_tkIsoBit;   //!
  TBranch * b_hadIsoBit;   //!
  TBranch * b_eleIdBit;   //!
  TBranch * b_eleIdLooseBit;   //!
  TBranch * b_eleIdTightBit;   //!
  TBranch * b_eleClass;   //!
  TBranch * b_ptHat;   //!
  TBranch * b_eleNum;   //!

  dati (TTree * fChain) 
    {
      fChain->SetMakeClass(1);

      fChain->SetBranchAddress ("elePT" ,elePT) ;
      fChain->SetBranchAddress ("eleEta" ,eleEta) ;
      fChain->SetBranchAddress ("elePhi" ,elePhi) ;
      fChain->SetBranchAddress ("eleCharge" ,eleCharge) ;
      fChain->SetBranchAddress ("jetPT" ,jetPT) ;
      fChain->SetBranchAddress ("jetEta" ,jetEta) ;
      fChain->SetBranchAddress ("jetPhi" ,jetPhi) ;
      fChain->SetBranchAddress ("jetmaxPT" ,jetmaxPT) ;
      fChain->SetBranchAddress ("jetmaxEta" ,jetmaxEta) ;
      fChain->SetBranchAddress ("jetmaxPhi" ,jetmaxPhi) ;
      fChain->SetBranchAddress ("EMjetCompon" ,EMjetCompon) ;
      fChain->SetBranchAddress ("jetFlavour" ,jetFlavour) ;
      fChain->SetBranchAddress ("jetmaxFlavour" ,jetmaxFlavour) ;
      fChain->SetBranchAddress ("rawBit" ,rawBit) ;
      fChain->SetBranchAddress ("ambiguityBit" ,ambiguityBit) ;
      fChain->SetBranchAddress ("tkIsoBit" ,tkIsoBit) ;
      fChain->SetBranchAddress ("hadIsoBit" ,hadIsoBit) ;
      fChain->SetBranchAddress ("eleIdBit" ,eleIdBit) ;
      fChain->SetBranchAddress ("eleIdLooseBit" ,eleIdLooseBit) ;
      fChain->SetBranchAddress ("eleIdTightBit" ,eleIdTightBit) ;
      fChain->SetBranchAddress ("eleClass" ,eleClass) ;
      fChain->SetBranchAddress ("ptHat" ,&ptHat) ;
      fChain->SetBranchAddress ("eleNum" ,&eleNum) ;

      b_elePT = fChain->GetBranch("elePT");
      b_eleEta = fChain->GetBranch("eleEta");
      b_elePhi = fChain->GetBranch("elePhi");
      b_eleCharge = fChain->GetBranch("eleCharge");
      b_jetPT = fChain->GetBranch("jetPT");
      b_jetEta = fChain->GetBranch("jetEta");
      b_jetPhi = fChain->GetBranch("jetPhi");
      b_jetmaxPT = fChain->GetBranch("jetmaxPT");
      b_jetmaxEta = fChain->GetBranch("jetmaxEta");
      b_jetmaxPhi = fChain->GetBranch("jetmaxPhi");
      b_EMjetCompon = fChain->GetBranch("EMjetCompon");
      b_jetFlavour = fChain->GetBranch("jetFlavour");
      b_jetmaxFlavour = fChain->GetBranch("jetmaxFlavour");
      b_rawBit = fChain->GetBranch("rawBit");
      b_ambiguityBit = fChain->GetBranch("ambiguityBit");
      b_tkIsoBit = fChain->GetBranch("tkIsoBit");
      b_hadIsoBit = fChain->GetBranch("hadIsoBit");
      b_eleIdBit = fChain->GetBranch("eleIdBit");
      b_eleIdLooseBit = fChain->GetBranch("eleIdLooseBit");
      b_eleIdTightBit = fChain->GetBranch("eleIdTightBit");
      b_eleClass = fChain->GetBranch("eleClass");
      b_ptHat = fChain->GetBranch("ptHat");
      b_eleNum = fChain->GetBranch("eleNum");   
    }     
} ;


