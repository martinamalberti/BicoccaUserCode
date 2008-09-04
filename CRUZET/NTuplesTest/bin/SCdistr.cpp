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

//! main program
int main (int argc, char** argv)
{
  
  std::string outputRootName = "changeme.root" ;
  std::string testName = "-o";
  if (argc>2){
      if (argv[1] == testName) {
        outputRootName = argv[2] ;
      }
  }
  
  std::string testHelp = "--help";
  if (argc==2){
      if (argv[1] == testHelp) {
          std::cout << "Help" << std::endl ;
          std::cout << " --help : display help" << std::endl ;
          std::cout << " -o : output root file name (eg histograms.root)" << std::endl ;
          std::cout << " name of input file : list name of input files ntuples" << std::endl ;     
          exit(1);
      }
  }
    
  if (argc < 2)
    {
      std::cerr << "ERROR : ntuple name missing" << std::endl ;
      exit (1) ;
    }

  TH2F SCdistr ("SCdistr","SCdistr",360,-3.1416,3.1416,170,-1.5,1.5) ;
           
  // Tree construction
  TChain * chain = new TChain ("EcalCosmicsAnalysis") ;
  
  if (argv[1] == testName) {
      for (int i=3; i< (argc); i++) chain->Add (argv[i]) ;
  }
  else{
      for (int i=1; i< (argc); i++) chain->Add (argv[i]) ;
  }
        
  //   chain->Add (argv[1]) ;

  EcalCosmicsTreeContent treeVars ; 
  setBranchAddresses (chain, treeVars) ;

  int nEntries = chain->GetEntries () ;
  std::cout << "FOUND " << nEntries << " ENTRIES\n" ;    

  //PG loop over entries
  for (int entry = 0 ; entry < nEntries ; ++entry)
    {
      chain->GetEntry (entry) ;

      //PG loop on superclusters
      for (int SCindex = 0 ; 
           SCindex < treeVars.nSuperClusters ; 
           ++SCindex)
        {
          SCdistr.Fill (treeVars.superClusterPhi[SCindex],
                        treeVars.superClusterEta[SCindex]) ;             
        } //PG loop on superclusters
    } //PG loop over entries

  TFile saving (outputRootName.c_str (),"recreate") ;
  saving.cd () ;  
  SCdistr.Write () ;
  saving.Close () ;

  return 0 ;
}


