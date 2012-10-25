#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"

#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "CalibCalorimetry/EcalLaserAnalyzer/interface/MEEBGeom.h"
#include "CalibCalorimetry/EcalLaserAnalyzer/interface/MEEEGeom.h"

#include <iostream>
#include <vector>
#include <map>
#include <utility>

#include "TChain.h"
#include "TF1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"



int main(int argc, char** argv)
{
  TFile* fout = new TFile("defineRegions.root","RECREATE");

  TH2F* EBmap = new TH2F("EBmap","EBmap",171,-85,86,360,1,361);

  for(int ieta=-85; ieta<86; ++ieta)
    for(int iphi=1; iphi<361; ++iphi)
      {
	if( !EBDetId::validDetId(ieta,iphi) ) continue;
	EBDetId myEB(ieta,iphi);

	int lmmod = MEEBGeom::lmmod(myEB.ieta(),myEB.iphi());
	int sm    = MEEBGeom::sm(myEB.ieta(),myEB.iphi());
	int tt    = MEEBGeom::tt(myEB.ieta(),myEB.iphi());

	sm = sm-1;
	tt = tt-1;
	lmmod = lmmod - 1;
	std::cout << "(" << myEB.ieta() << ", " << myEB.iphi() << ") " << sm << " " << tt+(sm*68) << " " << lmmod+(sm*9) << std::endl;	

	EBmap -> Fill(myEB.ieta(),myEB.iphi(),lmmod+(sm*9));


      }

  EBmap -> Write();

  fout -> Close();
  
  return 0;
}
