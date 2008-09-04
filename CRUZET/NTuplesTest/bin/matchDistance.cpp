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
#include "TProfile2D.h"
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

  TH1F matchDR ("matchDR","matchDR",100,0,1) ;
  TProfile2D matchDRMap ("matchDRMap","matchDRMap",360,-3.1416,3.1416,170,-1.5,1.5) ;
           
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

      std::vector<ect::association> associations ;
      ect::fillAssocVector (associations, treeVars) ;
      ect::selectOnDR (associations, treeVars, 0.3) ;

      //PG loop on associations vector
      for (unsigned int i = 0 ; 
           i < associations.size () ; 
           ++i)
        {
          int MUindex = associations.at (i).first  ;
          int SCindex = associations.at (i).second ;

          const GlobalPoint muonInnerHit (treeVars.muonInnerHitX[MUindex],
                                          treeVars.muonInnerHitY[MUindex],
                                          treeVars.muonInnerHitZ[MUindex]);
          const GlobalPoint muonOuterHit (treeVars.muonOuterHitX[MUindex],
                                          treeVars.muonOuterHitY[MUindex],
                                          treeVars.muonOuterHitZ[MUindex]);
    
          std::pair<GlobalPoint, GlobalPoint> muonTkAtEcal = 
                ect::EtaPhiMuonAtEcal(muonInnerHit, muonOuterHit) ;

          if (muonTkAtEcal.first.perp () < 0.001) continue ;
          double deltaR = sqrt (
            (muonTkAtEcal.first.phi () - treeVars.superClusterPhi[SCindex]) *
            (muonTkAtEcal.first.phi () - treeVars.superClusterPhi[SCindex]) +
            (muonTkAtEcal.first.eta () - treeVars.superClusterEta[SCindex]) *
            (muonTkAtEcal.first.eta () - treeVars.superClusterEta[SCindex])            
          ) ;
          matchDR.Fill (deltaR) ;
          matchDRMap.Fill (treeVars.superClusterPhi[SCindex],
                           treeVars.superClusterEta[SCindex],
                           deltaR) ;
        }
    } //PG loop over entries

  TFile saving (outputRootName.c_str (),"recreate") ;
  saving.cd () ;  
  matchDR.Write () ;
  matchDRMap.Write ("matchDRMap") ;
  saving.Close () ;

  return 0 ;
}


