#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "HiggsAnalysis/TauAnalysis/interface/smallAnalysis.h"
#include "HiggsAnalysis/TauAnalysis/interface/ditausAnalysis.h"
#include "HiggsAnalysis/TauAnalysis/interface/ditausAnalysisMC.h"

#include "HiggsAnalysis/TauAnalysis/interface/hlt_optimizer.h"


//define this as a plug-in
DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(smallAnalysis);
DEFINE_ANOTHER_FWK_MODULE(ditausAnalysis);
DEFINE_ANOTHER_FWK_MODULE(ditausAnalysisMC);
DEFINE_ANOTHER_FWK_MODULE(hlt_optimizer);
