#include <iostream>

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFObjects/VBFSampleInfo.h"


using namespace vbfhww2l;


VBFSampleInfo::VBFSampleInfo(){

  cross_section =0.00;
  total_events  =0;
  name          ="";

  used_cuts.clear();
}


VBFSampleInfo::~VBFSampleInfo(){
}


void VBFSampleInfo::AddCut(std::string label, double val){

  used_cuts.insert(std::make_pair(label, val));
}


void VBFSampleInfo::PrintCuts(){

  if(used_cuts.size())
    for(std::map<std::string, double>::iterator iter = used_cuts.begin();
	iter != used_cuts.end(); iter++)
      std::cout << iter->first << " :\t" << iter->second << "\n";
  else
    std::cout << "NO cuts found!!\n";  
}

