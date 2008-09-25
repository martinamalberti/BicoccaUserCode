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
#include "TProfile2D.h"
#include "TVector3.h"


//! get the time average among crystals of the SC
double getAverageTime (const EcalCosmicsTreeContent & treeVars, int SCindex) ;




//! main program
int main (int argc, char** argv)
{
  std::string outputRootName = "timingPlots.root" ;

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

  TH1F muonsAngle ("MuonsAngle","DOWN muon angle vs UP muon",10000,-2,2) ;
  TH1F SCtime ("SCtime","ave SC timing",100,-2,2) ;
  TProfile2D timeMap ("timeMap","ave of ave SC timing map",360,-3.1416,3.1416,170,-1.5,1.5) ;
  TH1F deltaT ("deltaT","diff of ave SC timing",1000,-5,5) ;
  TProfile2D deltaTMapUp ("deltaTMapUp","ave delta T map for upper muons",360,-3.1416,3.1416,170,-1.5,1.5) ;
  TProfile2D deltaTMapDown ("deltaTMapDown","ave delta T map for lower muons",360,-3.1416,3.1416,170,-1.5,1.5) ;
  TProfile2D negativeDeltaTMapUp ("negativeDeltaTMapUp","ave negative delta T map for upper muons",360,-3.1416,3.1416,170,-1.5,1.5) ;
  TProfile2D negativeDeltaTMapDown ("negativeDeltaTMapDown","ave negative delta T map for lower muons",360,-3.1416,3.1416,170,-1.5,1.5) ;

//  TH1F assocNum ("assocNum","number of associated SC muons found", 30,0,30) ;

  int nEntries = chain->GetEntries () ;
  std::cout << "FOUND " << nEntries << " ENTRIES\n" ;    

  //PG loop over entries
  for (int entry = 0 ; entry < nEntries ; ++entry)
    {
      chain->GetEntry (entry) ;
      if (entry % 100000 == 0) std::cout << "reading entry " << entry << std::endl ;
      std::vector<ect::association> associations ;
      ect::fillAssocVector (associations, treeVars) ;
      ect::selectOnDR (associations, treeVars, 0.3) ;

      if (associations.size () != 2) continue ;

      int SCindexUP = associations.at (0).second ;     
      int SCindexDOWN = associations.at (1).second ;     
      int MUindexUP = associations.at (0).first ;     
      int MUindexDOWN = associations.at (1).first ;     
      //PG if both on the same side, discard
      if (treeVars.superClusterPhi[SCindexUP] * treeVars.superClusterPhi[SCindexDOWN] >= 0) continue ;    
      if (treeVars.superClusterPhi[SCindexUP] < 0) 
        {
          std::swap (SCindexUP, SCindexDOWN) ;
          std::swap (MUindexUP, MUindexDOWN) ;
        }
      double timeUP = getAverageTime (treeVars, SCindexUP) ;
      double timeDOWN = getAverageTime (treeVars, SCindexDOWN) ;

      //PG angle between the two muons
      //PG ---------------------------

      TVector3 MuonDirUP (treeVars.muonMomentumX[MUindexUP], 
                          treeVars.muonMomentumY[MUindexUP], 
                          treeVars.muonMomentumZ[MUindexUP]) ;

      TVector3 MuonDirDOWN (treeVars.muonMomentumX[MUindexDOWN], 
                            treeVars.muonMomentumY[MUindexDOWN], 
                            treeVars.muonMomentumZ[MUindexDOWN]) ;

      double MUangle = MuonDirUP.Angle (MuonDirDOWN) ;
      muonsAngle.Fill (MUangle) ;
      if (MUangle > 0.1) continue ;
      if (treeVars.superClusterPhi[SCindexUP] < 1.2 ||
          treeVars.superClusterPhi[SCindexUP] > 1.8) continue ;
      if (treeVars.superClusterEta[SCindexUP] < -1 ||
          treeVars.superClusterEta[SCindexUP] > 1) continue ;

      SCtime.Fill (timeUP) ;
      SCtime.Fill (timeDOWN) ;
      timeMap.Fill (treeVars.superClusterPhi[SCindexUP],treeVars.superClusterEta[SCindexUP],timeUP) ;
      timeMap.Fill (treeVars.superClusterPhi[SCindexDOWN],treeVars.superClusterEta[SCindexDOWN],timeDOWN) ;

      double deltaTime =  timeDOWN - timeUP ;

      deltaT.Fill (deltaTime) ;
      deltaTMapUp.Fill (treeVars.superClusterPhi[SCindexUP],treeVars.superClusterEta[SCindexUP],deltaTime) ;
      deltaTMapDown.Fill (treeVars.superClusterPhi[SCindexDOWN],treeVars.superClusterEta[SCindexDOWN],deltaTime) ;
      if (deltaTime < 0)
        {
          negativeDeltaTMapUp.Fill (treeVars.superClusterPhi[SCindexUP],treeVars.superClusterEta[SCindexUP],deltaTime) ;
          negativeDeltaTMapDown.Fill (treeVars.superClusterPhi[SCindexDOWN],treeVars.superClusterEta[SCindexDOWN],deltaTime) ;
        }


    } //PG loop over entries

  TFile saving (outputRootName.c_str (),"recreate") ;
  saving.cd () ;  

  muonsAngle.Write () ;      
  SCtime.Write () ;      
  timeMap.Write () ;   
  deltaT.Write () ;
  deltaTMapUp.Write () ;          
  deltaTMapDown.Write () ;        
  negativeDeltaTMapUp.Write () ;  
  negativeDeltaTMapDown.Write () ;

  saving.Close () ;

  return 0 ;
}


// --------------------------------------------------------------------------------------------


//! get the time average among crystals of the SC
double 
getAverageTime (const EcalCosmicsTreeContent & treeVars, 
                int SCindex)
{
  double time = 0. ;
  for (int XTLindex = treeVars.xtalIndexInSuperCluster[SCindex] ;
       XTLindex < treeVars.xtalIndexInSuperCluster[SCindex] +
                  treeVars.nXtalsInSuperCluster[SCindex] ;
       ++XTLindex)
    {
      time += treeVars.xtalTime[XTLindex] ;  
    }
  time /= static_cast<double> (treeVars.nXtalsInSuperCluster[SCindex]) ;  
  return time ;
}
