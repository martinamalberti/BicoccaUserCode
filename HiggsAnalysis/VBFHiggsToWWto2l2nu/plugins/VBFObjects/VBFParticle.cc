#include <iostream>

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFObjects/VBFParticle.h"


using namespace vbfhww2l;


VBFParticle::VBFParticle() :
  pdgId(0)
{
  v4.SetCoordinates(0.0, 0.0, 0.0, 0.0);
  vertex.SetCoordinates(0.0, 0.0, 0.0);

  intProp.clear();
  doubleProp.clear();
}

VBFParticle::~VBFParticle(){
}

void VBFParticle::Reset(){

  v4.SetCoordinates(0.0, 0.0, 0.0, 0.0);
  vertex.SetCoordinates(0.0, 0.0, 0.0);

  pdgId =0;

  intProp.clear();
  doubleProp.clear();
}

void VBFParticle::AddIntProp(std::string label, int val){

  intProp.insert(std::make_pair(label, val)); 
}

void VBFParticle::AddDoubleProp(std::string label, double val){

  doubleProp.insert(std::make_pair(label, val)); 
}


int VBFParticle::GetIntProp(std::string label){

  int prop = -999;
  std::map<std::string, int>::iterator iter = intProp.find(label);
  if(iter != intProp.end())
    prop = iter->second;

  return prop;
}

double VBFParticle::GetDoubleProp(std::string label){

  double prop = -999.;
  std::map<std::string, double>::iterator iter = doubleProp.find(label);
  if(iter != doubleProp.end())
    prop = iter->second;

  return prop;
}

void VBFParticle::PrintPropNames(){

  if(intProp.size()){

    std::cout << "lep intProperties \n";
    for(std::map<std::string, int>::iterator iter = intProp.begin();
	iter != intProp.end(); iter++)
      std::cout << iter->first << " :\t" << iter->second << "\n";
  }
  else
    std::cout << "NO intProperties found!!\n";


  if(doubleProp.size()){

    std::cout << "\nlep doubleProperties \n";
    for(std::map<std::string, double>::iterator iter = doubleProp.begin();
	iter != doubleProp.end(); iter++)
      std::cout << iter->first << " :\t" << iter->second << "\n";
  }
  else
    std::cout << "NO doubleProperties found!!\n";
}

std::vector<std::string> VBFParticle::GetIntPropNames(){

  std::vector<std::string> names;
  if(intProp.size())

    for(std::map<std::string, int>::iterator iter = intProp.begin();
	iter != intProp.end(); iter++)
      names.push_back(iter->first);
    
  return names;
}

std::vector<std::string> VBFParticle::GetDoublePropNames(){

  std::vector<std::string> names;
  if(doubleProp.size())

    for(std::map<std::string, double>::iterator iter = doubleProp.begin();
	iter != doubleProp.end(); iter++)
      names.push_back(iter->first);
    
  return names;
}
