#include "CRUZET/Calibration/interface/ClusterCalibTools.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"


EBregionBuilder::EBregionBuilder (int etaStart, int etaEnd, int etaWidth,
                                  int phiStartEB, int phiEndEB, int phiWidthEB):
  m_etaWidth (etaWidth),  
  m_phiWidthEB (phiWidthEB),
  m_etaStart (etaShifter (etaStart)),  
  m_etaEnd (etaShifter (etaEnd)),
  m_phiStartEB (phiStartEB),
  m_phiEndEB (phiEndEB)
{
  EBRegionDefinition () ;
}                                  


//----------------------------------------


int 
EBregionBuilder::EBRegionId (int etaXtl, int phiXtl) const 
{
  if (EBregionCheck (etaXtl, phiXtl)) return -1 ;
  int etaLocal = etaShifter (etaXtl) ;
  int phifake = m_phiStartEB;
  if (m_phiStartEB > m_phiEndEB) phifake = m_phiStartEB - 360 ;
  int Nphi = (m_phiEndEB - phifake) / m_phiWidthEB ;
  int etaI = (etaLocal - m_etaStart) / m_etaWidth ;  
  int phiI = (phiXtl - m_phiStartEB) / m_phiWidthEB ; 
  int regionNumEB = phiI + Nphi * etaI ;
  return static_cast<int> (regionNumEB) ;
}


//----------------------------------------


//!DS number of regions in EB
int 
EBregionBuilder::EBregionsNum () const 
{
  int phi = m_phiStartEB ;
  if (m_phiStartEB > m_phiEndEB) phi = m_phiStartEB - 360 ;

  return ( (m_etaEnd - m_etaStart) / m_etaWidth) * 
         ( (m_phiEndEB - phi) / m_phiWidthEB) ; 
}


//----------------------------------------


//!DS EB Region Definition
void 
EBregionBuilder::EBRegionDefinition ()
{
  int reg = -1 ;
  for (int it = 0 ; it < EBregionsNum () ; ++it) m_regions.push_back (0) ;   
  for (int eta = -85 ; eta < 86  ; ++eta)
    for (int phi = 0 ; phi < 360 ; ++phi) 
       {
         if (eta == 0) continue ;
         reg = EBRegionId (eta,phi+1) ;
         int etaLocal = etaShifter (eta) ;
         m_xtalRegionId[EBDetId::unhashIndex (etaLocal*360+phi).rawId ()] = reg ; 
         if (reg == -1) continue;
         m_xtalPositionInRegion[EBDetId::unhashIndex (etaLocal*360+phi).rawId ()] = 
           m_regions.at (reg) ;
         ++m_regions.at (reg) ;
       }
}


//----------------------------------------


//! Tells if you are in the region to be calibrated
int 
EBregionBuilder::EBregionCheck (int eta, int phi) const 
 {
   int etaLocal = etaShifter (eta) ;
   if (etaLocal < m_etaStart) return 1 ;
   if (etaLocal >= m_etaEnd) return 2 ;
   if (phi < m_phiStartEB) return 3 ;
   if (phi >= m_phiEndEB) return 4 ;
   return 0 ;
 }


//--------------------------------------------


//!Fills the map to be sent to the IMACalibBlock
void 
EBregionBuilder::fillEBMap (std::map<int,double> & EBxtlMap,
                            double & pSubtract,
                            const std::map<int, double> & SCComponentsMap,
                            int EBNumberOfRegion)
{
  double dummy;
  pSubtract = 0. ;

  //PG loop over crystals in the reconstruction cluster  
  for (std::map<int,double>::const_iterator xtalsIt = SCComponentsMap.begin () ;
       xtalsIt != SCComponentsMap.end () ;
       ++xtalsIt)
     {
       //PG FIXME assumed to be valid
       //PG FIXME assumed to be in EB 
       EBDetId det = EBDetId::unhashIndex (xtalsIt->first) ;

       dummy = xtalsIt->second ;

       if (m_xtalRegionId[det.rawId ()] == EBNumberOfRegion)
            EBxtlMap[m_xtalPositionInRegion[det.rawId ()]] = dummy ;
       else pSubtract += dummy ;
   
     } //PG loop over crystals in the reconstruction cluster

  return ;
  //PG FIXME qui bisognera' inserire il ctrl per non essere troppo lontano, il bordo insomma
}


//--------------------------------------------


//!Shifts eta in other coordinates (from 0 to 170)
int EBregionBuilder::etaShifter (int etaOld) const
   {
     if (etaOld < 0) return etaOld + 85;
     else if (etaOld > 0) return etaOld + 84;
   }



//--------------------------------------------


//! Shifts phi in other coordinates (from 0 to 359)
//! not used
int EBregionBuilder::phiShifter (int phiOld) const
   {
     return phiOld - 1 ;
   }


//PG ------------------------------------------------------------------


int
findMaxXtalInSC (const EcalCosmicsTreeContent & treeVars,
                 int SCindex)
{
  double dummyEnergy = 0. ;
  int maxXtalIndex = -1 ;

  //PG loop over xtals in supercluster
  for (int XTLindex = treeVars.xtalIndexInSuperCluster[SCindex] ;
       XTLindex < treeVars.xtalIndexInSuperCluster[SCindex] +
                  treeVars.nXtalsInSuperCluster[SCindex] ;
       ++XTLindex)
    {
      if (treeVars.xtalEnergy[XTLindex] > dummyEnergy)
        {
          dummyEnergy = treeVars.xtalEnergy[XTLindex] ;
          maxXtalIndex = XTLindex ;
        }
    } //PG loop over xtals in supercluster

  return maxXtalIndex ;
}




//PG ------------------------------------------------------------------


int
findMaxClusterInSC (const EcalCosmicsTreeContent & treeVars,
                    int SCindex)
{
  double dummyEnergy = 0. ;
  int maxClusterIndex = -1 ;

  //PG loop over xtals in supercluster
  for (int CLUSTERindex = treeVars.clusterIndexInSuperCluster[SCindex] ;
       CLUSTERindex < treeVars.clusterIndexInSuperCluster[SCindex] +
                      treeVars.nClustersInSuperCluster[SCindex] ;
       ++CLUSTERindex)
    {
      if (treeVars.clusterEnergy[CLUSTERindex] > dummyEnergy)
        {
          dummyEnergy = treeVars.clusterEnergy[CLUSTERindex] ;
          maxClusterIndex = CLUSTERindex ;
        }
    } //PG loop over xtals in supercluster

  return maxClusterIndex ;
}


//PG ------------------------------------------------------------------


std::pair <int,int>
findMaxXtalsInSC (const EcalCosmicsTreeContent & treeVars,
                  int SCindex)
{
  double dummyEnergy = 0. ;
  int maxXtalIndex = -1 ;
  double dummySecondEnergy = 0. ;
  int secondMaxXtalIndex = -1 ;

  //PG loop over xtals in supercluster
  for (int XTLindex = treeVars.xtalIndexInSuperCluster[SCindex] ;
       XTLindex < treeVars.xtalIndexInSuperCluster[SCindex] +
                  treeVars.nXtalsInSuperCluster[SCindex] ;
       ++XTLindex)
    {
      if (treeVars.xtalEnergy[XTLindex] > dummyEnergy)
        {
          dummySecondEnergy = dummyEnergy ;
          secondMaxXtalIndex = maxXtalIndex ;
          dummyEnergy = treeVars.xtalEnergy[XTLindex] ;
          maxXtalIndex = XTLindex ;
        } else {
          dummySecondEnergy = treeVars.xtalEnergy[XTLindex] ;
          secondMaxXtalIndex = XTLindex ;
        }
    } //PG loop over xtals in supercluster

  return std::pair <int,int> (maxXtalIndex,secondMaxXtalIndex) ;
}


//PG ------------------------------------------------------------------


double
calcLengthInSC (const EcalCosmicsTreeContent & treeVars,
                int SCindex)
{
  double length = 0. ;

  //PG loop over xtals in supercluster
  for (int XTLindex = treeVars.xtalIndexInSuperCluster[SCindex] ;
       XTLindex < treeVars.xtalIndexInSuperCluster[SCindex] +
                  treeVars.nXtalsInSuperCluster[SCindex] ;
       ++XTLindex)
    {
      length += treeVars.xtalTkLength[XTLindex] ;
    } //PG loop over xtals in supercluster

  return length ;
}


//PG ------------------------------------------------------------------


double
calcVetoSingle (const EcalCosmicsTreeContent & treeVars,
                int SCindex,
                int maxXtalIndex)
{
  double vetoEnergy = 0. ;

  int maxIeta = treeVars.xtalHashedIndex[maxXtalIndex] / 360 ;
  int maxIphi = treeVars.xtalHashedIndex[maxXtalIndex] % 360 ;

  //PG loop over xtals in supercluster
  for (int XTLindex = treeVars.xtalIndexInSuperCluster[SCindex] ;
       XTLindex < treeVars.xtalIndexInSuperCluster[SCindex] +
                  treeVars.nXtalsInSuperCluster[SCindex] ;
       ++XTLindex)
    {
      //PG not the central one
      if (XTLindex == maxXtalIndex) continue ;
      //PG only the border ones
      int xtalIeta = treeVars.xtalHashedIndex[XTLindex] / 360 ;
      int xtalIphi = treeVars.xtalHashedIndex[XTLindex] % 360 ;
      if (abs (xtalIeta - maxIeta) > 1 || 
          abs (xtalIphi - maxIphi) > 1) continue ;      
      vetoEnergy += treeVars.xtalEnergy[XTLindex] ;
    } //PG loop over xtals in supercluster

  return vetoEnergy ;
}


//PG ------------------------------------------------------------------


double
calcVetoDouble (const EcalCosmicsTreeContent & treeVars,
                int SCindex,
                std::pair<int,int> maxXtals)
{
  double vetoEnergy = 0. ;

  if (checkCouple (treeVars, maxXtals)) return -1. ;

  int maxIeta1 = treeVars.xtalHashedIndex[maxXtals.first] / 360 ;
  int maxIphi1 = treeVars.xtalHashedIndex[maxXtals.first] % 360 ;
  int maxIeta2 = treeVars.xtalHashedIndex[maxXtals.second] / 360 ;
  int maxIphi2 = treeVars.xtalHashedIndex[maxXtals.second] % 360 ;

  //PG loop over xtals in supercluster
  for (int XTLindex = treeVars.xtalIndexInSuperCluster[SCindex] ;
       XTLindex < treeVars.xtalIndexInSuperCluster[SCindex] +
                  treeVars.nXtalsInSuperCluster[SCindex] ;
       ++XTLindex)
    {
      //PG not the central one
      if (XTLindex == maxXtals.first ||
          XTLindex == maxXtals.second) continue ;
      //PG only the border ones
      
      //PG FIXME to be done
      
      int xtalIeta = treeVars.xtalHashedIndex[XTLindex] / 360 ;
      int xtalIphi = treeVars.xtalHashedIndex[XTLindex] % 360 ;
      if ( (abs (xtalIeta - maxIeta1) > 1 || 
            abs (xtalIphi - maxIphi1) > 1) &&
           (abs (xtalIeta - maxIeta2) > 1 || 
            abs (xtalIphi - maxIphi2) > 1) ) 
        continue ;      
      vetoEnergy += treeVars.xtalEnergy[XTLindex] ;
    } //PG loop over xtals in supercluster

  return vetoEnergy ;
}


//PG ------------------------------------------------------------------


double
checkCouple (const EcalCosmicsTreeContent & treeVars,
             std::pair<int,int> maxXtals)
{
  int maxIeta1 = treeVars.xtalHashedIndex[maxXtals.first] / 360 ;
  int maxIphi1 = treeVars.xtalHashedIndex[maxXtals.first] % 360 ;
  int maxIeta2 = treeVars.xtalHashedIndex[maxXtals.second] / 360 ;
  int maxIphi2 = treeVars.xtalHashedIndex[maxXtals.second] % 360 ;

  if (abs (maxIeta1 - maxIeta2) > 1 ||
      abs (maxIphi1 - maxIphi2) > 1) return 0 ;
  return 1 ;
}

