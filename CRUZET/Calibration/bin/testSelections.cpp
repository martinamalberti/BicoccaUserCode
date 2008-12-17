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
#include "Calibration/Tools/interface/calibXMLwriter.h"

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

#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TLorentzVector.h"


int main (int argc, char** argv)
{
  std::cerr << "Welcome to testSelections program" << std::endl;


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

  TChain * chain = new TChain ("EcalCosmicsAnalysis") ;

  edm::ParameterSet subPSetInput =  
    parameterSet->getParameter<edm::ParameterSet> ("inputNtuples") ;
  std::vector<std::string> inputFiles = 
   subPSetInput.getParameter<std::vector<std::string> > ("inputFiles") ;
  int maxEvents = subPSetInput.getParameter<int> ("maxEvents") ;
  std::cout << "reading : " ;
  for (std::vector<std::string>::const_iterator listIt = inputFiles.begin () ;
       listIt != inputFiles.end () ;
       ++listIt)
    {
      std::cout << *listIt << " " << std::endl ;
      chain->Add (listIt->c_str ()) ;
    }

  EcalCosmicsTreeContent treeVars ;
  setBranchAddresses (chain, treeVars) ;
  int nEntries = chain->GetEntries () ;
  if (maxEvents < 0) maxEvents = nEntries ;
  std::cout << ">>> testSelections::Found " << nEntries << " entries\n" ;
  std::cout << ">>> testSelections::TreeBuilding::end <<<" << std::endl;


      //PG loop over entries
      for (int entry = 0 ; entry < maxEvents ; ++entry)
        {
          chain->GetEntry (entry) ;
          if (entry%10000 == 0) std::cout << "reading entry " << entry << "\n" ;
      
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
        
              //PG selections
              //PG ----------
                            
              TVector3 SC0_pos (treeVars.superClusterX[SCindex], 
                                treeVars.superClusterY[SCindex], 
                                treeVars.superClusterZ[SCindex]) ; 
    	       
              TVector3 MuonDir (treeVars.muonPx[MUindex], 
                                treeVars.muonPy[MUindex], 
                                treeVars.muonPz[MUindex]) ;
                    
              double angle = MuonDir.Angle (SC0_pos) ;
              if( angle > 3.1415/2. ) angle = 3.1415 - angle; // angle belongs to [0:90]

              int MaxXTLindex = findMaxXtalInSC (treeVars, SCindex) ;
              if (MaxXTLindex < 0) continue ;
              if (treeVars.xtalEnergy[MaxXTLindex] < 0.) continue ;
            
              //PG geometry
              //PG --------
                            
              //PG find the region ID
//              int EBNumberOfRegion = findRegion (treeVars, SCindex, EBRegionsTool) ;

              EBDetId dummyDetId = EBDetId::unhashIndex (treeVars.xtalHashedIndex[MaxXTLindex]) ;
              int ieta = dummyDetId.ieta () ;
              int iphi = dummyDetId.iphi () ;

              std::cout << "cluster num in SC: " 
                        << treeVars.nClustersInSuperCluster[SCindex] 
                        << std::endl ;
                        
              EBDetId det = EBDetId::unhashIndex (
                  treeVars.xtalHashedIndex[findMaxXtalInSC (treeVars, SCindex)]
                ) ;
              std::cout << "confronto_maxXtalInSC (" 
                        << det.ieta () << "," 
                        << det.iphi () << ") " ;
              
              EBDetId det2 (treeVars.clusterMaxId[findMaxClusterInSC (treeVars, SCindex)]) ;
              std::cout << "<-> (" << det2.ieta () << "," << det2.iphi () << ")" << std::endl ;
 
 
                                            
              std::cout << "maxClusterIndex: " << findMaxClusterInSC (treeVars, SCindex) << std::endl ;
            } //PG loop on associations vector
      
        } //PG loop over entries
      

  return 0 ;

}

