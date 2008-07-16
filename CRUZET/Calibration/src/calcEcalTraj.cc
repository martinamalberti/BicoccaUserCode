#include "CRUZET/Calibration/interface/calcEcalTraj.h"


double getTrajLengthInXtal (const std::vector<GlobalPoint> & neckLace,
                            DetId xtal,
                            edm::ESHandle<CaloGeometry> & pGeometry)
{
  if (neckLace.size () < 2) return 0 ;
  double length = 0. ;
  std::vector<GlobalPoint>::const_iterator start = neckLace.begin () ;
  ++start ;
  //PG loop over steps
  for (std::vector<GlobalPoint>::const_iterator nlIt = start ;
       nlIt != neckLace.end () ;
       ++nlIt)
    {
      if (pGeometry->getSubdetectorGeometry (xtal)->getGeometry (xtal)->inside (*nlIt))  
        {
          length += (*nlIt - *(nlIt-1)).mag () ;
        }
    } //PG loop over steps
  length -= (*start - *neckLace.begin ()).mag ()/2. ;
  return length ;
}                            


// ---------------------------------------------------------------------------------------


double getTrajLengthInXtals (const std::vector<GlobalPoint> & neckLace,
                             const std::vector<DetId> & xtals,
                             edm::ESHandle<CaloGeometry> & pGeometry)
{

  double length = 0. ;
  
  //PG loop over xtals of the cluster
  for (std::vector<DetId>::const_iterator detitr = xtals.begin () ; 
      detitr != xtals.end () ; 
      ++detitr)
    {
      length += getTrajLengthInXtal (neckLace, *detitr, pGeometry) ;
    }//PG loop over xtals of the cluster

  return length ;
}


// ---------------------------------------------------------------------------------------


double getTrajLengthInCluster (const std::vector<GlobalPoint> & neckLace,
                               const reco::BasicCluster & cluster,
                               edm::ESHandle<CaloGeometry> & pGeometry) 
{
  //get these from the cluster
  std::vector<DetId> clusterDetIds = cluster.getHitsByDetId () ; 
  return getTrajLengthInXtals (neckLace, clusterDetIds, pGeometry) ;
}