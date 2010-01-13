#include "HeavyFlavorAnalysis/Onia2MuMu/interface/NtupleFactory.h"


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


void NtupleFactory::AddTLorentzVector(TString name){
 if (ArrayContent_.find (name) != ArrayContent_.end ())
 {
  std::cerr << "ERROR : Array series " << name << " already existing, NOT replaced" << std::endl ;
  return ;                
 }
 TClonesArray* dummy = new TClonesArray ("TLorentzVector");
 ArrayContent_[name] = dummy ;
 outTree_->Branch (name, "TClonesArray", &(ArrayContent_[name]), 256000,0);
 ArrayContent_num_[name] = 0;
 return ;                          
}

// void NtupleFactory::AddXYZTLorentzVector(TString name){
//  if (ArrayContent_XYZT_.find (name) != ArrayContent_XYZT_.end ())
//  {
//   std::cerr << "ERROR : Array series " << name << " already existing, NOT replaced" << std::endl ;
//   return ;                
//  }
//  TClonesArray* dummy = new TClonesArray ("ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >");
//  ArrayContent_XYZT_[name] = dummy ;
//  outTree_->Branch (name, "TClonesArray", &(ArrayContent_XYZT_[name]), 256000,0);
//  ArrayContent_XYZT_num_[name] = 0;
//  return ;                          
// }

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

// void NtupleFactory::AddStdTLorentzVector(TString name){
//  if (ArrayContent_Std_.find (name) != ArrayContent_Std_.end ())
//  {
//   std::cerr << "ERROR : Array series " << name << " already existing, NOT replaced" << std::endl ;
//   return ;                
//  }
//  std::vector<TLorentzVector>* dummy = new std::vector<TLorentzVector> ;
//  ArrayContent_Std_[name] = dummy ;
//  outTree_->Branch(name,"std::vector<TLorentzVector>",&(ArrayContent_Std_[name]));
//  return ;                          
// }

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

void NtupleFactory::FillTLorentzVector(TString name,TLorentzVector* vect){
 if (ArrayContent_.find (name) != ArrayContent_.end ()){
  int counter = ArrayContent_num_[name];
  new ((*(ArrayContent_[name]))[counter]) TLorentzVector (*vect);
  ArrayContent_num_[name] += 1;
 }
 else {
  std::cerr << "ERROR : Array series " << name << " not existing. Nothing done." << std::endl ;
  return ;        
 }
 return ;
}
 
// void NtupleFactory::FillXYZTLorentzVector(TString name,ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >* vect){
//  if (ArrayContent_XYZT_.find (name) != ArrayContent_XYZT_.end ()){
//   int counter = ArrayContent_XYZT_num_[name];
//   new ((*(ArrayContent_XYZT_[name]))[counter]) ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > (*vect);
//   ArrayContent_XYZT_num_[name] += 1;
//  }
//  else {
//   std::cerr << "ERROR : Array series " << name << " not existing. Nothing done." << std::endl ;
//   return ;        
//  }
//  return ;
// }

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

// void NtupleFactory::FillStdTLorentzVector(TString name,TLorentzVector* vect){
//  if (ArrayContent_Std_.find (name) != ArrayContent_Std_.end ()){
//   ArrayContent_Std_[name]->push_back(*vect);
//  }
//  else {
//   std::cerr << "ERROR : Array series " << name << " not existing. Nothing done." << std::endl ;
//   return ;        
//  }
//  return ;
// }

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
 for (std::map<TString,TClonesArray*>::iterator it=ArrayContent_.begin() ; it != ArrayContent_.end(); it++ ){
  ((*it).second)->Clear(); 
  ArrayContent_num_[(*it).first] = 0;
 }
//  for (std::map<TString,TClonesArray*>::iterator it=ArrayContent_XYZT_.begin() ; it != ArrayContent_XYZT_.end(); it++ ){
//   ((*it).second)->Clear(); 
//   ArrayContent_XYZT_num_[(*it).first] = 0;
//  }
 for (std::map<TString,std::vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* >::iterator it=ArrayContent_StdXYZT_.begin() ; it != ArrayContent_StdXYZT_.end(); it++ ){
  ((*it).second)->clear();  
 }
//  for (std::map<TString,std::vector<TLorentzVector>* >::iterator it=ArrayContent_Std_.begin() ; it != ArrayContent_Std_.end(); it++ ){
//   ((*it).second)->clear();  
//  }
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

