#include <iostream>
#include <string>

//PG CMSSW includes
#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeContent.h"
#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeUtils.h"
//#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <boost/foreach.hpp>

//MF read CFG files includes5
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <boost/foreach.hpp>

//PG root includes
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"

//! main program
int main (int argc, char** argv)

{
  std::string fileName (argv[1]) ;
  boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile (fileName) ;
  boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet () ;
  std::cout << parameterSet->dump () << std::endl ; //PG for testing

//  edm::ParameterSet subPSetSelections =  parameterSet->getParameter<edm::ParameterSet> ("Selections") ;
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

  std::string outputRootName = "SCdistr.root" ;

  TH2F SCdistr ("SCdistr","SCdistr",360,-3.1416,3.1416,170,-1.5,1.5) ;
  TH2F SCradiusMap ("SCradiusMap","SCradiusMap",360,-3.1416,3.1416,170,-1.5,1.5) ;
  TH1F SCradius("SCradius","SCradius",900,1.10,2.) ;
  double radius;

  int nEntries = chain->GetEntries () ;
  std::cout << "FOUND " << nEntries << " ENTRIES\n" ;    

  //PG loop over entries
  for (int entry = 0 ; entry < nEntries ; ++entry)
    {
      chain->GetEntry (entry) ;
      if (entry % 100000 == 0) std::cout << "reading entry " << entry << std::endl ;

      //PG loop on superclusters
      for (int SCindex = 0 ; SCindex <= treeVars.nSuperClusters ; ++SCindex)
        {
	  radius = sqrt (
             (treeVars.superClusterX[SCindex]*treeVars.superClusterX[SCindex]) +
             (treeVars.superClusterY[SCindex]*treeVars.superClusterY[SCindex]  )  
          ) ;
          SCdistr.Fill (treeVars.superClusterPhi[SCindex],
                        treeVars.superClusterEta[SCindex]) ;
	  SCradiusMap.Fill (treeVars.superClusterPhi[SCindex],
                        treeVars.superClusterEta[SCindex],
			radius) ;
	  SCradius.Fill(radius);
	              
        } //PG loop on superclusters
    } //PG loop over entries

  TFile saving (outputRootName.c_str (),"recreate") ;
  saving.cd () ;  
  SCdistr.Write () ;
  SCradius.Write();
  SCradiusMap.Write();
  saving.Close () ;

  return 0 ;
}


