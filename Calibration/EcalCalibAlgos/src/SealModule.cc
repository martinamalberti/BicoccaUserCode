#include "FWCore/PluginManager/interface/ModuleDef.h" 
#include "FWCore/Framework/interface/LooperFactory.h" 
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "Calibration/EcalCalibAlgos/interface/InvMatrixLooper.h"
#include "Calibration/EcalCalibAlgos/interface/InvRingCalib.h"

// #include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
// #include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"
// #include "PhysicsTools/UtilAlgos/interface/SingleObjectSelector.h"
// #include "PhysicsTools/RecoAlgos/interface/PixelMatchGsfElectronSelector.h"

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_LOOPER(InvMatrixLooper);
DEFINE_ANOTHER_FWK_LOOPER(InvRingCalib);

// namespace reco {
//   namespace modules {
//     typedef SingleObjectSelector<reco::PixelMatchGsfElectronCollection,
//                                  ::PhiRangeSelector> PhiRangeSelector; 
//     DEFINE_ANOTHER_FWK_MODULE(PhiRangeSelector);
// 
//     typedef SingleObjectSelector<reco::PixelMatchGsfElectronCollection,
//                                  ::IMASelector> IMASelector; 
//     DEFINE_ANOTHER_FWK_MODULE(IMASelector);
// 
//   }
// }
