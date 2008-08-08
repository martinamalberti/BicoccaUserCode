#include "CRUZET/Calibration/interface/calcEcalDeposits.h"
#include "TrackingTools/TrackAssociator/interface/CachedTrajectory.h"
#include "TrackingTools/TrackAssociator/interface/DetIdAssociator.h"
#include "TrackingTools/Records/interface/DetIdAssociatorRecord.h"
#include "TrackPropagation/SteppingHelixPropagator/interface/SteppingHelixStateInfo.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "TrackPropagation/SteppingHelixPropagator/interface/SteppingHelixPropagator.h"



std::vector<GlobalPoint> calcEcalDeposit (const edm::EventSetup& iSetup,
					  const FreeTrajectoryState* innerState,
					  const FreeTrajectoryState* outerState,
					  const TrackAssociatorParameters& parameters)
{
  SteppingHelixStateInfo trackOrigin (*innerState) ;
  
  CachedTrajectory neckLace ;
  neckLace.setStateAtIP (trackOrigin) ;

  
  // setup propagator
  edm::ESHandle<MagneticField> bField;
  iSetup.get<IdealMagneticFieldRecord>().get(bField);
  SteppingHelixPropagator* prop  = new SteppingHelixPropagator(&*bField,anyDirection);
  prop->setMaterialMode(false);
  prop->applyRadX0Correction(true);
  // prop->setDebug(true); // tmp
  neckLace.setPropagator(prop);

  neckLace.reset_trajectory () ;
  
  
  //dove vengono gi chiamati?
  edm::ESHandle<DetIdAssociator> ecalDetIdAssociator ;
  edm::ESHandle<DetIdAssociator> hoDetIdAssociator ;
  edm::ESHandle<DetIdAssociator> muonDetIdAssociator;
  iSetup.get<DetIdAssociatorRecord> ().get ("EcalDetIdAssociator", ecalDetIdAssociator) ;
  iSetup.get<DetIdAssociatorRecord>().get("HODetIdAssociator", hoDetIdAssociator);
  iSetup.get<DetIdAssociatorRecord>().get("MuonDetIdAssociator", muonDetIdAssociator);

  
  //PG set some params   
  double HOmaxR = hoDetIdAssociator->volume().maxR();
  double HOmaxZ = hoDetIdAssociator->volume().maxZ();
  double minR = ecalDetIdAssociator->volume ().minR () ;
  double minZ = ecalDetIdAssociator->volume ().minZ () ;
  neckLace.setMaxHORadius (HOmaxR) ;
  neckLace.setMaxHOLength (HOmaxZ*2.) ;
  neckLace.setMinDetectorRadius (minR) ;
  neckLace.setMinDetectorLength (minZ*2.) ;
  double maxR (0) ;
  double maxZ (0) ;
  
  if (parameters.useMuon) {
    maxR = muonDetIdAssociator->volume ().maxR () ;
    maxZ = muonDetIdAssociator->volume ().maxZ () ;
    neckLace.setMaxDetectorRadius (maxR) ;
    neckLace.setMaxDetectorLength (maxZ*2.) ;
  }
  else {
    maxR = HOmaxR ;
    maxZ = HOmaxZ ;
    neckLace.setMaxDetectorRadius (HOmaxR) ;
    neckLace.setMaxDetectorLength (HOmaxZ*2.) ;
  }
  

  //info.setCaloGeometry (theCaloGeometry_) ;
  
  //double HOmaxR = -1. ; //PG FIXME
  //double HOmaxZ = -1. ; //PG FIXME  
  // If track extras exist and outerState is before HO maximum, then use outerState
  if (outerState) {
    if (outerState->position ().perp ()<HOmaxR && fabs (outerState->position ().z ())<HOmaxZ) {
      LogTrace ("TrackAssociator") << "Using outerState as trackOrigin at Rho=" << outerState->position ().perp ()
				   << "  Z=" << outerState->position ().z () << "\n" ;
      trackOrigin = SteppingHelixStateInfo (*outerState) ;
    }
    else if (innerState) {
      LogTrace ("TrackAssociator") << "Using innerState as trackOrigin at Rho=" << innerState->position ().perp ()
				   << "  Z=" << innerState->position ().z () << "\n" ;
      trackOrigin = SteppingHelixStateInfo (*innerState) ;
    }
  }
  
  // propagateAll fills the cached trajectory with the array of points along the helix.
  // if failed, we should return a failure value 
  
  if ( ! neckLace.propagateAll (trackOrigin) ) 
    {
      std::cerr << "===>>> FALLIMENTO <<<===" << std::endl;
      return std::vector<GlobalPoint> () ;
    }
  
  // get trajectory in ECAL
  neckLace.findEcalTrajectory ( ecalDetIdAssociator->volume () ) ;
  
  const std::vector<SteppingHelixStateInfo>& complicatePoints = neckLace.getEcalTrajectory () ;
    
  std::vector<GlobalPoint> simplePoints ;
  for (std::vector<SteppingHelixStateInfo>::const_iterator cpIt = complicatePoints.begin () ;
       cpIt != complicatePoints.end () ;
       ++cpIt)
    {
      GlobalPoint dummy (cpIt->position ()) ;
      simplePoints.push_back (dummy) ;
    }
  
  return simplePoints ;
}


