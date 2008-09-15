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

//PG root includes
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"

//! main program
int main (int argc, char** argv)
{
  
  std::string outputRootName = "effCalc.root" ;
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

  TH2F SCMuons ("SCMuons","SCMuons",10,0,10,10,0,10) ;
           
  int nEntries = chain->GetEntries () ;
  std::cout << "FOUND " << nEntries << " ENTRIES\n" ;    

  //PG loop over entries
  for (int entry = 0 ; entry < nEntries ; ++entry)
    {
      chain->GetEntry (entry) ;
      SCMuons.Fill (treeVars.nSuperClusters,treeVars.nRecoMuons) ;             
    } //PG loop over entries

  TFile saving (outputRootName.c_str (),"recreate") ;
  saving.cd () ;  
  SCMuons.Write () ;
  saving.Close () ;

  return 0 ;
}


