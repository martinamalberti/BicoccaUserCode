#ifndef NtupleFactory_h
#define NtupleFactory_h

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include "TString.h"
#include "TClonesArray.h"
#include "TLorentzVector.h"
#include "TTree.h"
#include "TFile.h"
// #include "DataFormats/Math/interface/LorentzVector.h"
#include "Math/PtEtaPhiE4D.h"
#include "Math/PtEtaPhiM4D.h"
#include "Math/LorentzVector.h"


class NtupleFactory{
 
 protected:
  
 public:
  NtupleFactory(TString namefile);
  NtupleFactory(TTree* outTree_input);
  ~NtupleFactory();
  
//   void AddXYZTLorentzVector(TString name);
//   void FillXYZTLorentzVector(TString name,ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >* vect);

//   template <class T> void AddVector(TString name);
//   template <class T> void FillVector(TString name,T* vect);
    
  void AddTLorentzVector(TString name);
  void FillTLorentzVector(TString name,TLorentzVector* vect);
 
  void AddStdXYZTLorentzVector(TString name);
  void FillStdXYZTLorentzVector(TString name,ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >* vect);
  
//   void AddStdTLorentzVector(TString name);
//   void FillStdTLorentzVector(TString name,TLorentzVector* vect);

  void AddFloat(TString name);
  void FillFloat(TString name,float vect);
  
  void FillNtuple();
    
  void WriteNtuple(); 
  
  void DumpBranches();
  
 private:
  TLorentzVector myvector ;
  std::map <TString,TClonesArray*> ArrayContent_ ;
  std::map <TString,int> ArrayContent_num_ ;
  std::map <TString,std::vector<float>* > ArrayContentFloat_ ;

//   std::map <TString,TClonesArray*> ArrayContent_XYZT_ ;
//   std::map <TString,int> ArrayContent_XYZT_num_ ;
    
  std::map <TString,std::vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* > ArrayContent_StdXYZT_ ;
//   std::map <TString,std::vector<TLorentzVector>* > ArrayContent_Std_ ;
  
       
  TTree* outTree_;
  bool internalTree_;
  
  TFile* outFile_;
  TString fileName_ ;
  
};




// template <class T> void NtupleFactory::AddVector (TString name){
// }
// 
// template <class T> void NtupleFactory::FillVector(TString name,T* vect){
// }


#endif
