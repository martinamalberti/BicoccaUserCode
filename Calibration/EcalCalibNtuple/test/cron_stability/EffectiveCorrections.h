#ifndef EffectiveCorrections_h
#define EffectiveCorrections_h

#include "geometryUtils.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "TFile.h"
#include "TGraphAsymmErrors.h"



class EffectiveCorrections
{
 public:
  
  //!ctor
  EffectiveCorrections(char* EBEE, const std::string& corrType, const std::string& regionType, const std::string& inFileName);
  
  //!dtor
  ~EffectiveCorrections();
  
  //!methods
  double GetEffectiveCorrection(const int& time, const int& regionId);
  
  
 private:
  
  char* EBEE_;
  std::string corrType_;
  std::string regionType_;
  TFile* inFile_;
  
  std::map<int,std::vector<double> > corrFactor_;
  std::map<int,std::vector<double> > minTime_;
  std::map<int,std::vector<double> > maxTime_;
};

#endif
