#ifndef __VBFSAMPLE_INFO__
#define __VBFSAMPLE_INFO__


#include <map>
#include <string>


namespace vbfhww2l{

  class VBFSampleInfo{

  public:
	
    VBFSampleInfo();
    ~VBFSampleInfo();

    void AddCut(std::string label, double val);
    void PrintCuts();
	
    double cross_section;
    int    total_events;
    std::string name;

    std::map<std::string, double> used_cuts;
  };
}

#endif
