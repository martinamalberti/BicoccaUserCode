#ifndef calcEcalTraj_h
#define calcEcalTraj_h

#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/DetId/interface/DetId.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"

#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"

#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include <vector>

#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/EcalAlgo/interface/EcalBarrelGeometry.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "RecoCaloTools/Navigation/interface/CaloNavigator.h"


/** 
    Calculate the path length inside a single xtal, 
    given the trajectory as a colletion of GlobalPoints
*/
double getTrajLengthInXtal (const std::vector<GlobalPoint> & neckLace,
                            DetId xtal,
                            edm::ESHandle<CaloGeometry> & pGeometry) ;

/** 
    Calculate the path length inside a set of single xtals, 
    given the trajectory as a colletion of GlobalPoints
*/
double getTrajLengthInXtals (const std::vector<GlobalPoint> & neckLace,
                             const std::vector<DetId> & xtals,
                             edm::ESHandle<CaloGeometry> & pGeometry) ;

/** 
    Calculate the path length inside the set of single xtals contained in a basic cluster,
    given the trajectory as a colletion of GlobalPoints
*/
double getTrajLengthInCluster (const std::vector<GlobalPoint> & neckLace,
                               reco::BasicCluster & cluster,
                               edm::ESHandle<CaloGeometry> & pGeometry) ;


#endif
