#ifndef calcEcalDeposits_h
#define calcEcalDeposits_h

#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h"
#include "TrackingTools/TrackAssociator/interface/TrackAssociatorParameters.h"



std::vector<GlobalPoint> calcEcalDeposit (const edm::EventSetup& iSetup,
					  const FreeTrajectoryState* innerState,
					  const FreeTrajectoryState* outerState,
					  const TrackAssociatorParameters& parameters) ;

#endif 
