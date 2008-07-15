#ifndef calcEcalDeposits_h
#define calcEcalDeposits_h

#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h"


std::vector<GlobalPoint> getHelixPoints (const edm::EventSetup& iSetup,
                                         const FreeTrajectoryState* innerState,
                                         const FreeTrajectoryState* outerState) ;

#endif 
