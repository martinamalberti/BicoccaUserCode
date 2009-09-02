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
#include "TProfile2D.h"
#include "TFile.h"

//MF read CFG files includes5
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <boost/foreach.hpp>


#include "CRUZET/NTuplesTest/interface/NTuplesUtils.h"

//! main program
int main (int argc, char** argv)
{

  std::string outputRootName = "matchDistance.root" ;
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

  TH1F matchDR ("matchDR","matchDR",100,0,1) ;

  TProfile2D matchDRMap ("matchDRMap","matchDRMap",360,-3.1416,3.1416,170,-1.5,1.5) ;
  TProfile matchDRvsEtaSopra("matchDRvsEtaSopra","matchDRvsEtaSopra", 170,-1.5,1.5); 
  TProfile matchDRvsEtaSotto("matchDRvsEtaSotto","matchDRvsEtaSotto", 170,-1.5,1.5);       
  TProfile matchDEtavsEtaSopra("matchDEtavsEtaSopra","matchDEtavsEtaSopra", 170,-1.5,1.5); 
  TProfile matchDEtavsEtaSotto("matchDEtavsEtaSotto","matchDEtavsEtaSotto", 170,-1.5,1.5);         
  TProfile matchDRvsPhi("matchDRvsPhi","matchDRvsPhi", 360,-3.1416,3.1416);        

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
	  if(treeVars.superClusterPhi[SCindex]>0) 
	  			{
	  		        matchDRvsEtaSopra.Fill (treeVars.superClusterEta[SCindex],
                                deltaR) ;
			        matchDEtavsEtaSopra.Fill (treeVars.superClusterEta[SCindex],
                                (muonTkAtEcal.first.eta () - treeVars.superClusterEta[SCindex])) ;
			   	} 
	  if(treeVars.superClusterPhi[SCindex]<=0) 
	                        {
	 	 		matchDEtavsEtaSotto.Fill (treeVars.superClusterEta[SCindex],
                                (muonTkAtEcal.first.eta () - treeVars.superClusterEta[SCindex])) ; 
			        }
	  matchDRvsPhi.Fill (treeVars.superClusterPhi[SCindex],
                           deltaR) ;
        }
    } //PG loop over entries

  TFile saving ("matchOutput.root","recreate") ;
  saving.cd () ;  
  matchDR.Write () ;
  matchDRvsEtaSotto.Write();
  matchDRvsEtaSopra.Write();
  matchDEtavsEtaSotto.Write();
  matchDEtavsEtaSopra.Write();

  matchDRvsPhi.Write();

  matchDRMap.Write ("matchDRMap") ;
  saving.Close () ;

  return 0 ;
}


