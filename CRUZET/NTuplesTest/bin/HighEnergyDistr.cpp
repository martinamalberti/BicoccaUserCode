#include <iostream>
#include <string>

//PG CMSSW includes
#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeContent.h"
#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeUtils.h"
//#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

//PG root includes
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TProfile2D.h"
#include "TLorentzVector.h"
#include "TApplication.h"

//MF read CFG files includes
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <boost/foreach.hpp>


#include "CRUZET/Calibration/interface/CRUtils.h"

//#include "CRUZET/Calibration/interface/AssociationUtils.h"

int main (int argc, char** argv)
{

  std::string fileName (argv[1]) ;
  boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile (fileName) ;
  boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet () ;
  std::cout << parameterSet->dump () << std::endl ; //PG for testing
  
  edm::ParameterSet subPSetSelections =  parameterSet->getParameter<edm::ParameterSet> ("selections") ;
  edm::ParameterSet subPSetInput =  
    parameterSet->getParameter<edm::ParameterSet> ("inputNtuples") ;
  std::vector<std::string> inputFiles = 
   subPSetInput.getParameter<std::vector<std::string> > ("inputFiles") ;
  std::cout << "reading : " ;

  TChain *chain = new TChain ("EcalCosmicsAnalysis") ;
  EcalCosmicsTreeContent treeVars ; 
  setBranchAddresses (chain, treeVars) ;

  for (std::vector<std::string>::const_iterator listIt = inputFiles.begin () ;
       listIt != inputFiles.end () ;
       ++listIt)
    {
      std::cout << *listIt << " " << std::endl ;
      chain->Add (listIt->c_str ()) ;
    }

  int nEntries = chain->GetEntries () ;
  std::cout << "FOUND " << nEntries << " ENTRIES\n" ;    

  TH2F OccupancyXtals ("OccupancyXtals","OccupancyXtals",360,1.,360.,172,-86.,86.) ;     
  TH2F IntegratedEnergy ("IntegratedEnergy","IntegratedEnergy",360,1.,360.,172,-86.,86.) ;     
  TH1F spectrum ("spectrum","spectrum",100,0,100) ;     

  double singleXtalMinEnergy = 1. ; //PG GeV
  double XtalEnergyMax = 0 ;		
  double XtalEnergyMin = 0 ;
  
  //PG loop over entries
  for (int entry = 0 ; entry < nEntries ; ++entry)
    {
      chain->GetEntry (entry) ;

      if (entry % 10000 == 0) std::cout << "reading entry " << entry << std::endl ;

	  for (int XTLindex = 0 ;
           XTLindex < treeVars.nXtals ;
           ++XTLindex)
        {
            EBDetId dummy = EBDetId::unhashIndex (treeVars.xtalHashedIndex[XTLindex]) ;
            if (treeVars.xtalEnergy[XTLindex] < singleXtalMinEnergy) continue ;            
            OccupancyXtals.Fill (dummy.iphi (), dummy.ieta ()) ;
            IntegratedEnergy.Fill (dummy.iphi (), dummy.ieta (),treeVars.xtalEnergy[XTLindex]) ;
            spectrum.Fill (treeVars.xtalEnergy[XTLindex]) ;
        }
    } //PG loop over entries

  TFile saving ("HighEnergyXtals.root" ,"recreate") ;
  saving.cd () ;  
  OccupancyXtals.Write () ;
  IntegratedEnergy.Write () ;
  spectrum.Write () ;
  saving.Close () ;
  
  return 0 ;
}


