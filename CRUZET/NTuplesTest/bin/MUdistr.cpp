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
  std::string outputRootName = "OutputMUdistr.root" ;

  std::string fileName (argv[1]) ;
  boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile (fileName) ;
  boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet () ;
  std::cout << parameterSet->dump () << std::endl ; //PG for testing
  
  edm::ParameterSet subPSetSelections =  parameterSet->getParameter<edm::ParameterSet> ("Selections") ;
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
  
  
  
  TH2F matchSCdistr ("matchSCdistr","matchSCdistr",360,-3.1416,3.1416,170,-1.5,1.5) ;
  TH2F MUdistr ("MUdistr","MUdistr",360,-3.1416,3.1416,170,-1.5,1.5) ;
        
  //   chain->Add (argv[1]) ;


  int nEntries = chain->GetEntries () ;
  std::cout << "FOUND " << nEntries << " ENTRIES\n" ;    

  //PG loop over entries
  for (int entry = 0 ; entry < nEntries ; ++entry)
    {
      chain->GetEntry (entry) ;

      for (int MUindex = 0 ; MUindex < treeVars.nRecoMuons ; ++MUindex)
        {
          const GlobalPoint muonInnerHit (treeVars.muonInnerHitX[MUindex],
                                          treeVars.muonInnerHitY[MUindex],
                                          treeVars.muonInnerHitZ[MUindex]);
          const GlobalPoint muonOuterHit (treeVars.muonOuterHitX[MUindex],
                                          treeVars.muonOuterHitY[MUindex],
                                          treeVars.muonOuterHitZ[MUindex]);
    
          std::pair<GlobalPoint, GlobalPoint> muonTkAtEcal = 
                ect::EtaPhiMuonAtEcal(muonInnerHit, muonOuterHit) ;

          if (muonTkAtEcal.first.perp () < 0.001) continue ;
          MUdistr.Fill (muonTkAtEcal.first.phi (),
                        muonTkAtEcal.first.eta ()) ;
        }


    } //PG loop over entries

  TFile saving (outputRootName.c_str (),"recreate") ;
  saving.cd () ;  
  matchSCdistr.Write () ;
  MUdistr.Write () ;
  saving.Close () ;

  return 0 ;
}


