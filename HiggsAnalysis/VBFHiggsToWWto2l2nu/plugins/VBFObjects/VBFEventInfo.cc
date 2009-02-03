#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFObjects/VBFEventInfo.h"


using namespace vbfhww2l;


VBFEventInfo::VBFEventInfo() :
  runId(0), eventId(0),
  kFac(-1), presel(false),
  nZvtx(-1)
{
  primV.SetCoordinates(0.0, 0.0, 0.0);
}


VBFEventInfo::~VBFEventInfo(){
}

void VBFEventInfo::Reset(){
  runId = 0;  
  eventId = 0;
  
  kFac = -1;
  presel = false;

  nZvtx = -1;

  primV.SetCoordinates(0.0, 0.0, 0.0);

  trigger.clear();
}


void VBFEventInfo::SetTrigger(std::string trig_name, unsigned int trig_val){

  trigger.insert(std::make_pair(trig_name, trig_val));  
}

void VBFEventInfo::PrintTrigger(){

  if(trigger.size()){

    std::cout << "trigger info\n";
    for(std::map<std::string, unsigned int>::iterator iter = trigger.begin();
	iter != trigger.end(); iter++)
      std::cout << iter->first << " :\t" << iter->second << "\n";
  }
  else
    std::cout << "NO trigger info found!!\n";    
}

std::vector<std::string> VBFEventInfo::GetTriggerNames(){

  std::vector<std::string> names;
  if(trigger.size())

    for(std::map<std::string, unsigned int>::iterator iter = trigger.begin();
	iter != trigger.end(); iter++)
      names.push_back(iter->first);
    
  return names;
}


