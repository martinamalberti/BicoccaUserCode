#ifndef G__DICTIONARY
#define G__DICTIONARY


#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFObjects/VBFParticle.h"
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFObjects/VBFEventInfo.h"
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFObjects/VBFMCEvent.h"
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFObjects/VBFSampleInfo.h"


namespace {
  namespace {
    
    vbfhww2l::VBFParticle  p;
    vbfhww2l::VBFEventInfo  ei;
    vbfhww2l::VBFMCEvent    mc;
    vbfhww2l::VBFSampleInfo si;

    VBFParticleCollection pc;

    Point point;
    LorentzVector lv;
    
    std::map<std::string, unsigned int> tr;
    std::map<std::string, int>    iP;
    std::map<std::string, double> dP;

    std::string s;


  }
}

#endif
 
