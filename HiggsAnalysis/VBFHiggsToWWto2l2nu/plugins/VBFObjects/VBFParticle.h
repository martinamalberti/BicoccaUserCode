#ifndef __VBFParticle_H__
#define __VBFParticle_H__


#include <map>
#include <vector>
#include <string>

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFObjects/VBFDefs.h"


namespace vbfhww2l{

  class VBFParticle{

  public:

    VBFParticle();
    ~VBFParticle();

    void Reset();

    void   AddIntProp(std::string label, int val);
    void   AddDoubleProp(std::string label, double val);
    int    GetIntProp(std::string label);
    double GetDoubleProp(std::string label);
    void   PrintPropNames();
    std::vector<std::string> GetIntPropNames();
    std::vector<std::string> GetDoublePropNames();

  
    LorentzVector v4;
    Point vertex;
    int pdgId;

    std::map<std::string, int>    intProp;
    std::map<std::string, double> doubleProp;

  };
}

typedef std::vector<vbfhww2l::VBFParticle> VBFParticleCollection ;

#endif
