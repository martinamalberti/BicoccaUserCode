#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

//#include "FakeRate/EffAnalysis/plugins/FakeRateMcFilter.h"
//#include "FakeRate/EffAnalysis/plugins/FakeRateMcEventTypeFilter.h"
//#include "FakeRate/EffAnalysis/plugins/FakeRateMcAnalyzer.h"
//#include "FakeRate/EffAnalysis/plugins/FakeRateFilter.h"
#include "FakeRate/EffAnalysis/plugins/FakeRateAmbiguityResolve.h"
#include "FakeRate/EffAnalysis/plugins/FakeRateMiniTreeProducer.h"
//#include "FakeRate/EffAnalysis/plugins/FakeRatePreSelection.h"
//#include "FakeRate/EffAnalysis/plugins/FakeRatePrimaryVertex.h"
//#include "FakeRate/EffAnalysis/plugins/FakeRateIsolation.h"
#include "FakeRate/EffAnalysis/plugins/FakeRateTkIsolation.h"
#include "FakeRate/EffAnalysis/plugins/FakeRateEWKTkIsolation.h"
#include "FakeRate/EffAnalysis/plugins/FakeRateHadIsolation.h"
#include "FakeRate/EffAnalysis/plugins/FakeRateEleId.h"
//#include "FakeRate/EffAnalysis/plugins/FakeRateKineTreeProducer.h"

#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"

#include "FakeRate/EffAnalysis/plugins/RobertoSelector.h"
#include "PhysicsTools/UtilAlgos/interface/ObjectSelector.h"
#include "PhysicsTools/UtilAlgos/interface/SortCollectionSelector.h"
#include "PhysicsTools/Utilities/interface/PtComparator.h"
#include "PhysicsTools/UtilAlgos/interface/PtMinSelector.h"
#include "PhysicsTools/UtilAlgos/interface/SingleElementCollectionSelector.h"
#include "PhysicsTools/UtilAlgos/interface/ObjectCountFilter.h"
#include "PhysicsTools/UtilAlgos/interface/SingleObjectSelector.h"
//#include "PhysicsTools/UtilAlgos/interface/SingleObjectRefVectorSelector.h"
#include "DataFormats/Common/interface/RefVector.h"

//typedef RobertoSelector<FakeRateAmbiguityResolve> AmbResolver ;
typedef ObjectSelector<FakeRateAmbiguityResolve> AmbResolver ;
typedef ObjectSelector<
          FakeRateAmbiguityResolve, 
          edm::RefVector<reco::PixelMatchGsfElectronCollection> 
         > AmbResolverRef ;

typedef ObjectSelector<FakeRateTkIsolation> TkIso;
typedef ObjectSelector<
          FakeRateTkIsolation, 
          edm::RefVector<reco::PixelMatchGsfElectronCollection> 
         > TkIsoRef ;

typedef ObjectSelector<FakeRateEWKTkIsolation> EWKTkIso;
typedef ObjectSelector<
          FakeRateEWKTkIsolation, 
          edm::RefVector<reco::PixelMatchGsfElectronCollection> 
         > EWKTkIsoRef ;

typedef RobertoSelector<FakeRateHadIsolation> HadIso;
typedef RobertoSelector<
          FakeRateHadIsolation, 
          edm::RefVector<reco::PixelMatchGsfElectronCollection> 
         > HadIsoRef ;

typedef ObjectSelector<FakeRateEleId> EleId;
typedef ObjectSelector<
          FakeRateEleId, 
          edm::RefVector<reco::PixelMatchGsfElectronCollection> 
         > EleIdRef ;

DEFINE_SEAL_MODULE();

DEFINE_ANOTHER_FWK_MODULE(AmbResolver);
DEFINE_ANOTHER_FWK_MODULE(AmbResolverRef);

DEFINE_ANOTHER_FWK_MODULE(FakeRateMiniTreeProducer);

DEFINE_ANOTHER_FWK_MODULE(TkIso);
DEFINE_ANOTHER_FWK_MODULE(TkIsoRef);

DEFINE_ANOTHER_FWK_MODULE(EWKTkIso);
DEFINE_ANOTHER_FWK_MODULE(EWKTkIsoRef);

DEFINE_ANOTHER_FWK_MODULE(HadIso);
DEFINE_ANOTHER_FWK_MODULE(HadIsoRef);

DEFINE_ANOTHER_FWK_MODULE(EleId);
DEFINE_ANOTHER_FWK_MODULE(EleIdRef);

