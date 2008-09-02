//#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/eventsetupdata_registration_macro.h"

#include "EtaAnalyzer.h"
#include "EtaAnalyzerWithMC.h"

DEFINE_SEAL_MODULE();

#include "PhysicsTools/UtilAlgos/interface/Merger.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
typedef Merger<reco::SuperClusterCollection> EgammaSuperClusterMerger;
DEFINE_FWK_MODULE( EgammaSuperClusterMerger );
DEFINE_ANOTHER_FWK_MODULE(EtaAnalyzer);
DEFINE_ANOTHER_FWK_MODULE(EtaAnalyzerWithMC);
