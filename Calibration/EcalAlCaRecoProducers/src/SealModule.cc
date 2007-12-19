#include "PluginManager/ModuleDef.h" 
#include "FWCore/Framework/interface/LooperFactory.h" 
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "Calibration/EcalAlCaRecoProducers/interface/AlCaElectronsProducer.h"
#include "Calibration/EcalAlCaRecoProducers/interface/AlCaElectronsTest.h"
#include "Calibration/EcalAlCaRecoProducers/interface/AlCaElectronsTestCalib.h"
#include "Calibration/EcalAlCaRecoProducers/interface/PhiRangeSelector.h"
#include "Calibration/EcalAlCaRecoProducers/interface/IMASelector.h"
#include "Calibration/EcalAlCaRecoProducers/interface/AlCaPhiSymRecHitsProducer.h"
#include "Calibration/EcalAlCaRecoProducers/interface/AlCaPi0RecHitsProducer.h"
#include "Calibration/EcalAlCaRecoProducers/interface/AlCaPi0BasicClusterRecHitsProducer.h"
#include "Calibration/EcalAlCaRecoProducers/interface/TBInvMatrixLooper.h"
#include "Calibration/EcalAlCaRecoProducers/interface/InvRingCalib.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"
#include "PhysicsTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "PhysicsTools/RecoAlgos/interface/PixelMatchGsfElectronSelector.h"
DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(AlCaElectronsProducer);
DEFINE_ANOTHER_FWK_MODULE(AlCaElectronsTest);
DEFINE_ANOTHER_FWK_MODULE(AlCaElectronsTestCalib);
DEFINE_ANOTHER_FWK_MODULE(AlCaPhiSymRecHitsProducer);
DEFINE_ANOTHER_FWK_MODULE(AlCaPi0RecHitsProducer);
DEFINE_ANOTHER_FWK_MODULE(AlCaPi0BasicClusterRecHitsProducer);

DEFINE_ANOTHER_FWK_LOOPER(TBInvMatrixLooper);
DEFINE_ANOTHER_FWK_LOOPER(InvRingCalib);

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
