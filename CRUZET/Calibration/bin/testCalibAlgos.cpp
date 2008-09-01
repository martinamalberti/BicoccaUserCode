#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"
#include "DataFormats/EgammaCandidates/interface/ElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"

//PG for the cluster-based calibration 
#include "Calibration/EcalCalibAlgos/interface/VEcalCalibBlock.h"
#include "Calibration/EcalCalibAlgos/interface/IMACalibBlock.h"
#include "Calibration/EcalCalibAlgos/interface/L3CalibBlock.h"

#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeContent.h"
#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeUtils.h"
#include "CRUZET/Calibration/interface/ClusterCalibTools.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <boost/foreach.hpp>


int findRegion (EcalCosmicsTreeContent treeVars,
                int SCindex,
                EBregionBuilder EBRegionsTool) ;


//PG ------------------------------------------------------------------



int main (int argc, char** argv)
{
  std::cerr << "Welcome to testCalibAlgos program" << std::endl;


  // read input parameters from config file
  if(!argv[1])
    {
      std::cout << ">>> No input config file given. Terminating program <<<" << std::endl;
      return -1;
    }

  std::string fileName(argv[1]);
  boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile(fileName);
  boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet() ;
  std::cout << parameterSet->dump () << std::endl ; //PG for testing

  edm::ParameterSet subPSetCalib =  
    parameterSet->getParameter<edm::ParameterSet> ("clusterCalib") ;
  int etaMin = subPSetCalib.getParameter<int> ("etaMin") ;
  int etaMax = subPSetCalib.getParameter<int> ("etaMax") ;
  int etaStep = subPSetCalib.getParameter<int> ("etaStep") ;
  int phiMin = subPSetCalib.getParameter<int> ("phiMin") ;
  int phiMax = subPSetCalib.getParameter<int> ("phiMax") ;
  int phiStep = subPSetCalib.getParameter<int> ("phiStep") ;
  std::string algorithm = subPSetCalib.getParameter<std::string> ("algorithm") ;

//  EBregionBuilder EBRegionsTool (-85, -1, 2, 1, 181, 2) ;
  EBregionBuilder EBRegionsTool (etaMin, etaMax, etaStep, phiMin, phiMax, phiStep) ;


  //PG single blocks calibrators
  std::vector<VEcalCalibBlock *> EcalCalibBlocks ;

  //PG loop over the regions set
  for (int region = 0 ;
       region < EBRegionsTool.EBregionsNum () ;
       ++region)
    {
      if (algorithm == "IMA")
        EcalCalibBlocks.push_back (
            new IMACalibBlock (EBRegionsTool.xtalNumInRegion (region))
          ) ;
      else if (algorithm == "L3")
        EcalCalibBlocks.push_back (
            new L3CalibBlock (EBRegionsTool.xtalNumInRegion (region), 1)
          ) ;
      else
        {
          edm::LogError ("building") << algorithm
                          << " is not a valid calibration algorithm" ;
          exit (1) ;
        }
    } //PG loop over the regions set



  //PG FIXME one has to read and save calibration coeffs
  //PG FIXME read from xml files or DB directly

  //PG FIXME would it be possible to do it for rings or crystals
  //PG FIXME in the same code?

  std::cout << ">>> testCalibAlgos::OpeningFiles::begin <<<" << std::endl;

  TChain * chain = new TChain ("EcalCosmicsAnalysis") ;

  edm::ParameterSet subPSetInput =  
    parameterSet->getParameter<edm::ParameterSet> ("inputNtuples") ;
  std::vector<std::string> inputFiles = 
   subPSetInput.getParameter<std::vector<std::string> > ("inputFiles") ;
  std::cout << "reading : " ;
  for (std::vector<std::string>::const_iterator listIt = inputFiles.begin () ;
       listIt != inputFiles.end () ;
       ++listIt)
    {
      std::cout << *listIt << " " ;
      chain->Add (listIt->c_str ()) ;
    }

  std::cout << ">>> testCalibAlgos::TreeBuilding::begin <<<" << std::endl;

  EcalCosmicsTreeContent treeVars ;
  setBranchAddresses (chain, treeVars) ;
  int nEntries = chain->GetEntries () ;
  std::cout << ">>> testCalibAlgos::Found " << nEntries << " entries\n" ;
  std::cout << ">>> testCalibAlgos::TreeBuilding::end <<<" << std::endl;



  //PG loop over entries
  for (int entry = 0 ; entry < nEntries ; ++entry)
    {
      chain->GetEntry (entry) ;
      if (entry%1000 == 0) std::cout << "reading entry " << entry << "\n" ;

      //PG association between muons and superclusters
      //PG -------------------------------------------

      std::vector<ect::association> associations ;
      ect::fillAssocVector (associations, treeVars) ;
      ect::selectOnDR (associations, treeVars, 0.3) ;

      //PG loop on associations vector
      for (unsigned int i = 0 ; i < associations.size () ; ++i)
        {
      int MUindex = associations.at (i).first ;
      int SCindex = associations.at (i).second ;


          //PG FIXME find the region ID
      int EBNumberOfRegion = findRegion(treeVars, SCindex, EBRegionsTool) ;
      if (EBNumberOfRegion == -1) continue ;


          //PG xtal - energy association map
          std::map<int,double> EBxtlMap ;
      double pSubtract ;
//      EBRegionsTool.fillEBMap (EBxtlMap, pSubtract,
//                   treeVars, SCindex,
//                   EBNumberOfRegion) ;

          //PG get the matrix of crystals
          //PG FIXME as a vector of DetID to be understood

          //PG fill the map of energies
          //PG FIXME with the function to be modified

          //PG feed the calibration algorithm



      EcalCalibBlocks.at (EBNumberOfRegion) -> Fill (EBxtlMap.begin() , EBxtlMap.end (),
                             treeVars.muonTkLengthInEcalDetail[MUindex],
                             pSubtract) ;




        } //PG loop on associations vector


    } //PG loop over entries

  //PG extract the solution of the calibration

return 0 ;

}


//PG ------------------------------------------------------------------









//!Find the region to which the SC belongs
int findRegion (EcalCosmicsTreeContent treeVars,
                int SCindex,
                EBregionBuilder EBRegionsTool)
{
  double dummyEnergy = 0. ;
  EBDetId dummyDetId ;

  //PG loop over xtals
  for (int XTLindex = treeVars.xtalIndexInSuperCluster[SCindex] ;
       XTLindex < treeVars.xtalIndexInSuperCluster[SCindex] +
     treeVars.nXtalsInSuperCluster[SCindex] ;
       ++XTLindex)
    {
      if (treeVars.xtalEnergy[XTLindex] > dummyEnergy)
    {
      dummyEnergy = treeVars.xtalEnergy[XTLindex] ;
      dummyDetId = EBDetId::unhashIndex (treeVars.xtalHashedIndex[XTLindex]) ;
    }
    }

  int ieta = dummyDetId.ieta () ;
  if (ieta < 0) ieta = ieta + 85;
  else if (ieta > 0) ieta = ieta + 84;
  int iphi = dummyDetId.iphi () ;

  // std::cout << "ene = " << dummyEnergy << "   ieta/iphi = " << ieta << "," << iphi
  //         << "    RegID = "<< EBRegionsTool.EBRegionId(ieta, iphi) << std::endl;

  return EBRegionsTool.EBRegionId(ieta, iphi);
}


