#ifndef VBFNtupleUtils_h
#define VBFNtupleUtils_h

#include "HiggsAnalysis/VBFHiggsToWWTolnujj/interface/VBFNtupleContent.h"

#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#include "TVector3.h"
#include "TLorentzVector.h"
#include "TFile.h"






double SelectJets(std::vector<int>& it, VBFNtupleContent& treeVars, const std::string& method,
                  const double& etMin = 15., const std::vector<int>* blacklist = 0);


#endif
