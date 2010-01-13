#include "HiggsAnalysis/VBFHiggsToVV/interface/NtupleFactory.h"


///--- Constructor ---
NtupleFactory::NtupleFactory(TString fileName){
//---- create output tree ----
internalTree_ = true;
fileName_ = fileName;
outFile_ = new TFile(fileName_,"RECREATE");
outTree_ = new TTree("SimpleTree","SimpleTree");
}

NtupleFactory::NtupleFactory(TTree* outTree_input){
//---- create output tree ----
 internalTree_ = false;
 outTree_ = outTree_input;
}

///--- Destructor ---
NtupleFactory::~NtupleFactory(){
 //---- destroy everything ----
 if (internalTree_){
  delete outTree_;
  delete outFile_;
  }
}


///--------------------------
///----- Add collection -----


void NtupleFactory::AddStdXYZTLorentzVector(TString name){
 if (ArrayContent_StdXYZT_.find (name) != ArrayContent_StdXYZT_.end ())
 {
  std::cerr << "ERROR : Array series " << name << " already existing, NOT replaced" << std::endl ;
  return ;                
 }
 std::vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* dummy = new std::vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > > ;
 ArrayContent_StdXYZT_[name] = dummy ;
 outTree_->Branch(name,"std::vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >",&(ArrayContent_StdXYZT_[name]));
 return ;                          
}

void NtupleFactory::AddStdXYZVector(TString name){
 if (ArrayContent_StdXYZ_.find (name) != ArrayContent_StdXYZ_.end ())
 {
  std::cerr << "ERROR : Array series " << name << " already existing, NOT replaced" << std::endl ;
  return ;                
 }
 std::vector<ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<double>,ROOT::Math::DefaultCoordinateSystemTag> >* dummy = new std::vector<ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<double>,ROOT::Math::DefaultCoordinateSystemTag> > ;
 ArrayContent_StdXYZ_[name] = dummy ;
 outTree_->Branch(name,"std::vector<ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<double>,ROOT::Math::DefaultCoordinateSystemTag> >",&(ArrayContent_StdXYZ_[name]));
 return ;                          
}

void NtupleFactory::AddFloat(TString name){
 if (ArrayContentFloat_.find (name) != ArrayContentFloat_.end ())
 {
  std::cerr << "ERROR : Array series " << name << " already existing, NOT replaced" << std::endl ;
  return ;                
 }
 std::vector<float>* dummy = new std::vector<float> ;
 ArrayContentFloat_[name] = dummy ;
 outTree_->Branch(name,"std::vector<float>",&(ArrayContentFloat_[name]));
 return ;                          
}

////--------------------------
///----- Fill collection -----

void NtupleFactory::FillStdXYZTLorentzVector(TString name,ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >* vect){
 if (ArrayContent_StdXYZT_.find (name) != ArrayContent_StdXYZT_.end ()){
  ArrayContent_StdXYZT_[name]->push_back(*vect);
 }
 else {
  std::cerr << "ERROR : Array series " << name << " not existing. Nothing done." << std::endl ;
  return ;        
 }
 return ;
}

void NtupleFactory::FillStdXYZVector(TString name,ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<double>,ROOT::Math::DefaultCoordinateSystemTag>* vect){
 if (ArrayContent_StdXYZ_.find (name) != ArrayContent_StdXYZ_.end ()){
  ArrayContent_StdXYZ_[name]->push_back(*vect);
 }
 else {
  std::cerr << "ERROR : Array series " << name << " not existing. Nothing done." << std::endl ;
  return ;        
 }
 return ;
}

void NtupleFactory::FillFloat(TString name,float vect){
 if (ArrayContentFloat_.find (name) != ArrayContentFloat_.end ()){
  ArrayContentFloat_[name]->push_back(vect);
 }
 else {
  std::cerr << "ERROR : Array series " << name << " not existing. Nothing done." << std::endl ;
  return ;        
 }
 return ;
}
 
 
///---- Write entry to TTree ----
void NtupleFactory::FillNtuple(){
 outTree_->Fill();
 for (std::map<TString,std::vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* >::iterator it=ArrayContent_StdXYZT_.begin() ; it != ArrayContent_StdXYZT_.end(); it++ ){
  ((*it).second)->clear();  
 }
 for (std::map<TString,std::vector<ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<double>,ROOT::Math::DefaultCoordinateSystemTag> >* >::iterator it=ArrayContent_StdXYZ_.begin() ; it != ArrayContent_StdXYZ_.end(); it++ ){
  ((*it).second)->clear();  
 }
 for (std::map<TString,std::vector<float>* >::iterator it=ArrayContentFloat_.begin() ; it != ArrayContentFloat_.end(); it++ ){
  ((*it).second)->clear();  
 }
}



///---- Write collection to file ----
void NtupleFactory::WriteNtuple(){
 //---- if using TFileService, not use this
 if (internalTree_) outFile_->Write();
 else std::cerr << "Service not available: using TFileService" << std::endl ;
}




///---- Write to a text file branches declaration ----
void NtupleFactory::DumpBranches(){
 //----- symply use MakeClass
 outTree_->MakeClass("out.txt");
}

