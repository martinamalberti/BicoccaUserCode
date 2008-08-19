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
  if (EBregionCheck (etaXtl,phiXtl)) return -1;
  int phifake = m_phiStartEB;
  if (m_phiStartEB>m_phiEndEB) phifake = m_phiStartEB - 360 ;
  int Nphi = (m_phiEndEB - phifake) / m_phiWidthEB ;
  int etaI = (etaXtl - m_etaStart) / m_etaWidth ;  
  int phiI = (phiXtl - m_phiStartEB) / m_phiWidthEB ; 
  int regionNumEB = phiI + Nphi * etaI ;
  return (int) regionNumEB;
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
  for (int eta = 0 ; eta < 170  ; ++eta)
    for (int phi = 0 ; phi < 360 ; ++phi)
       {
         reg = EBRegionId (eta,phi) ;
         m_xtalRegionId[EBDetId::unhashIndex (eta*360+phi).rawId ()] = reg ; 
         if (reg==-1) continue;
         m_xtalPositionInRegion[EBDetId::unhashIndex (eta*360+phi).rawId ()] = 
           m_regions.at (reg) ;
         ++m_regions.at (reg);
       }
}


//----------------------------------------


//! Tells if you are in the region to be calibrated
int 
EBregionBuilder::EBregionCheck (int eta, int phi) const 
 {
   if (eta < m_etaStart) return 1 ;
   if (eta >= m_etaEnd)   return 2 ;
   if (phi < m_phiStartEB) return 3 ;
   if (phi >= m_phiEndEB)   return 4 ;
   return 0 ;
 }


//--------------------------------------------


//!Shifts eta in other coordinates (from 0 to 170)
int EBregionBuilder::etaShifter (int etaOld) const
   {
     if (etaOld < 0) return etaOld + 85;
     else if (etaOld > 0) return etaOld + 84;
   }

