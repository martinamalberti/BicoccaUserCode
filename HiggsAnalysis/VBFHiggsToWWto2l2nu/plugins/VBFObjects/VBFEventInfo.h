#ifndef __VBFEventInfo_H__
#define __VBFEventInfo_H__

#include <map>
#include <vector>
#include <string>

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFObjects/VBFDefs.h"


namespace vbfhww2l{

  class VBFEventInfo{

  public: // we keep all data members public

    VBFEventInfo();
    ~VBFEventInfo();

    void Reset();
       
    void SetTrigger(std::string trig_name, unsigned int trig_val);
    void PrintTrigger();
    std::vector<std::string> GetTriggerNames();
    
    
    // general event information
    unsigned int runId;
    unsigned int eventId;
    
    double kFac;
    bool presel;

    // reconstructed objects
    int   nZvtx;                   // number of vertices

    Point primV;                   // primary vertex

    // trigger
    std::map<std::string, unsigned int> trigger; 
  }; 
}
#endif
