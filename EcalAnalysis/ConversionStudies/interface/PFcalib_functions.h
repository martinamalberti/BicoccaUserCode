
#ifndef PFCALIBFUNC_H
#define PFCALIBFUNC_H


#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cmath>
#include <boost/lexical_cast.hpp>

#include "TTree.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile2D.h"
#include "TFile.h"
#include "TVector3.h"
#include "TLorentzVector.h"

#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloTopology/interface/CaloSubdetectorTopology.h"
#include "PhysicsTools/NtupleUtils/interface/treeReader.h"
#include "PhysicsTools/NtupleUtils/interface/hFactory.h"
#include "PhysicsTools/NtupleUtils/interface/hChain.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"

#define mElec 0.000511
#define mPi0  0.1349766
#define PI    3.141592654




template <class particle>
std::pair<int,double> min_dR(const particle& part, const std::vector<ROOT::Math::XYZTVector>& MCPhoton, const std::vector<float>& MCPhotonMother)
{
  int iteratore = -1;
  double dR_min = 1000.;

  for(unsigned int  MCPhoItr = 0; MCPhoItr < MCPhoton.size(); ++MCPhoItr)
    {
      if(MCPhotonMother[MCPhoItr] != 111) continue;
      double dEtaPMC = fabs(part.eta() - MCPhoton[MCPhoItr].Eta());
      double dPhiPMC = fabs(part.phi() - MCPhoton[MCPhoItr].Phi());
      if (dPhiPMC > 2*PI) dPhiPMC -= 2*PI;
      if (dPhiPMC > PI ) dPhiPMC = 2*PI - dPhiPMC;
      
      if(sqrt(dPhiPMC*dPhiPMC + dEtaPMC*dEtaPMC) < dR_min)
	{
	  dR_min = sqrt(dPhiPMC*dPhiPMC + dEtaPMC*dEtaPMC);
	  iteratore = MCPhoItr;
	}
    }
  std::pair<int,double> matched(iteratore,dR_min);
  return(matched);
}


void checkBin(const int& index, std::map<int,TH1F*>& plotMap, std::string& name, const int& nBins, const double& xMin, const double& xMax)
{
  std::map<int,TH1F*>::const_iterator mapItr = plotMap.find(index);
  if(mapItr == plotMap.end())
    {
      name.append("_");
      name += boost::lexical_cast<std::string>(index);
      plotMap[index] = (new TH1F(name.c_str(), name.c_str(), nBins, xMin, xMax));
    }
  return;
}



#endif
