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

//  new IMACalibBlock (m_regions.at (region))
//  new L3CalibBlock (m_regions.at (region), eventWeight)

  //PG build the regions to be calibrated
  //PG FIXME fill  xtalRegionId, xtalPositionInRegion
    
  int nEntries = -1 ;

  //PG loop over entries
  for (int entry = 0 ; entry < nEntries ; ++entry)
    {
       //PG get the matrix of crystals
       //PG FIXME as a vector of DetID
       
       //PG fill the map of energies
       //PG FIXME with the function
       
       //PG feed the calibration algorithm
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
