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


void fillEBMap (std::map<int,double> & EBxtlMap, //PG effective output
                double & pSubtract,              //PG effective output
                EBDetId EBmax,
                std::vector<EBDetId> & crystals,
                const EcalRecHitCollection * hitsCollection,
                std::map<int,int> xtalRegionId, //PG FIXME become a member of a class
                std::map<int,int> xtalPositionInRegion, //PG FIXME become a member of a class
                int EBNumberOfRegion) ;


//PG ------------------------------------------------------------------


int main (int argc, char** argv)
{
  std::vector<VEcalCalibBlock*> calibBlocks ;
  calibBlocks.push_back (new IMACalibBlock (10)) ;
  calibBlocks.push_back (new L3CalibBlock (10, 1)) ;

  //PG FIXME parameters to be read from outside
  EBregionBuilder EBregionsTool (-85, 85, 5, 0, 20, 5) ;

  //PG single blocks calibrators
  std::vector<VEcalCalibBlock *> EcalCalibBlocks ;

  std::string algorithm = "IMA" ;

  //PG loop over the regions set
  for (int region = 0 ; 
       region < EBregionsTool.EBregionsNum () ; 
       ++region)
    {   
      if (algorithm == "IMA")
        EcalCalibBlocks.push_back (
            new IMACalibBlock (EBregionsTool.xtalNumInRegion (region))
          ) ; 
      else if (algorithm == "L3")
        EcalCalibBlocks.push_back (
            new L3CalibBlock (EBregionsTool.xtalNumInRegion (region), 
                              1)
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

  TChain * chain = new TChain ("EcalCosmicsAnalysis") ;
  chain->Add (argv[1]) ;

  EcalCosmicsTreeContent treeVars ; 
  setBranchAddresses (chain, treeVars) ;

  int nEntries = chain->GetEntries () ;
  std::cout << "FOUND " << nEntries << " ENTRIES\n" ;        

  //PG loop over entries
  for (int entry = 0 ; entry < nEntries ; ++entry)
    {
      chain->GetEntry (entry) ;
      if (entry % 1000) std::cout << "reading entry " << entry << "\n" ; 

      //PG association between muons and superclusters
      //PG -------------------------------------------
      
      std::vector<ect::association> associations ;
      ect::fillAssocVector (associations, treeVars) ;
      ect::selectOnDR (associations, treeVars, 0.3) ;

      //PG loop on associations vector
      for (unsigned int i = 0 ; i < associations.size () ; ++i)
        { 
          //PG xtal - energy association map
          std::map<int,double> EBxtlMap ;
          
          int SCindex = associations.at (i).second ;
          
          //PG FIXME find the region ID
          
          double ESubtract = 0. ;
          
          //PG loop over crystals in SUPERcluster
          for (int XTLindex = treeVars.xtalIndexInSuperCluster[SCindex] ; 
               XTLindex < treeVars.xtalIndexInSuperCluster[SCindex] + 
                          treeVars.nXtalsInSuperCluster[SCindex] ; 
               ++XTLindex)
            {
              if (1) //PG FIXME if xtals do not fall in the right region
                {
                  EBxtlMap[treeVars.xtalHashedIndex[XTLindex]] = 
                    treeVars.xtalEnergy[XTLindex] ;
                }
              else
                {
                  ESubtract += treeVars.xtalEnergy[XTLindex] ;
                }  
            } //PG loop over crystals in SUPERcluster

          //PG get the matrix of crystals
          //PG FIXME as a vector of DetID to be understood
          
          //PG fill the map of energies
          //PG FIXME with the function to be modified
      
          //PG feed the calibration algorithm

          
        } //PG loop on associations vector


    } //PG loop over entries

  //PG extract the solution of the calibration  

  return 0 ;

}


//PG ------------------------------------------------------------------


//!Fills the map to be sent to the CalibBlock
//PG may be templated for EB/EE? FIXME
void fillEBMap (std::map<int,double> & EBxtlMap, //PG effective output
                double & pSubtract,              //PG effective output
                EBDetId EBmax,
                std::vector<EBDetId> & crystals,
                const EcalRecHitCollection * hitsCollection,
                std::map<int,int> xtalRegionId, //PG FIXME become a member of a class
                std::map<int,int> xtalPositionInRegion, //PG FIXME become a member of a class
                int EBNumberOfRegion)
{
  double dummy = 0. ;
  pSubtract = 0 ;
  //PG loop over xtals  
  for (std::vector<EBDetId>::const_iterator xtalIt = crystals.begin () ;
       xtalIt != crystals.end () ;
       ++xtalIt)
    {
      //PG find the energy corresponding to the DetId
      EcalRecHitCollection::const_iterator curr_recHit = hitsCollection->find (*xtalIt) ;
      if (curr_recHit == hitsCollection->end ()) continue;
      dummy = curr_recHit->energy () ;

//PG FIXME limits and calib coeffs to be set as members of a class,
//PG FIXME either expected to be applied somewhere else
//PG FIXME by an official module
//      if ( dummy > m_minEnergyPerCrystal && dummy < m_maxEnergyPerCrystal)
//           dummy *= m_recalibMap[det.rawId ()] ;     
//      else continue;

      if (xtalRegionId[xtalIt->rawId ()] == EBNumberOfRegion)
           EBxtlMap[xtalPositionInRegion[xtalIt->rawId ()]] = dummy ;
      else pSubtract += dummy;
    
    }//PG loop over xtals
}


