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
#include "Math/Vector3D.h"
#include "Math/Vector3Dfwd.h"

class NtupleFactory{
  
 protected:
  
 public:
  NtupleFactory(TString namefile);
  NtupleFactory(TTree* outTree_input);
  ~NtupleFactory();
      
  void AddStdXYZTLorentzVector(TString name);
  void FillStdXYZTLorentzVector(TString name,ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >* vect);

  void AddStdXYZVector(TString name);
  void FillStdXYZVector(TString name,ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<double>,ROOT::Math::DefaultCoordinateSystemTag>* vect);
      
  void AddFloat(TString name);
  void FillFloat(TString name,float vect);
  
  void FillNtuple();
    
  void WriteNtuple(); 
  
  void DumpBranches();
  
 private:
  TLorentzVector myvector ;
  std::map <TString,std::vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* > ArrayContent_StdXYZT_ ;
  std::map <TString,std::vector<ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<double>,ROOT::Math::DefaultCoordinateSystemTag> >* > ArrayContent_StdXYZ_ ;
  std::map <TString,std::vector<float>* > ArrayContentFloat_ ;
  
       
  TTree* outTree_;
  bool internalTree_;
  
  TFile* outFile_;
  TString fileName_ ;
  
};



#endif
