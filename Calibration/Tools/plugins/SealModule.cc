//#include "PluginManager/ModuleDef.h" 
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "Calibration/Tools/interface/PhiRangeSelector.h"
#include "Calibration/Tools/interface/IMASelector.h"
#include "PhysicsTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"
#include "PhysicsTools/RecoAlgos/interface/PixelMatchGsfElectronSelector.h"

DEFINE_SEAL_MODULE();


namespace reco {
  namespace modules {
    typedef SingleObjectSelector<reco::PixelMatchGsfElectronCollection,
                                 ::PhiRangeSelector> PhiRangeSelector; 
    DEFINE_ANOTHER_FWK_MODULE(PhiRangeSelector);

    typedef SingleObjectSelector<reco::PixelMatchGsfElectronCollection,
                                 ::IMASelector> IMASelector; 
    DEFINE_ANOTHER_FWK_MODULE(IMASelector);

  }
}
