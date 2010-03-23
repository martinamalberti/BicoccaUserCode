#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_SEAL_MODULE();

#include "Validation/AlCaTest/interface/AlCaElectronsTest.h"
DEFINE_ANOTHER_FWK_MODULE (AlCaElectronsTest) ;

#include "Validation/AlCaTest/interface/AlCaSuperClustersTest.h"
DEFINE_ANOTHER_FWK_MODULE (AlCaSuperClustersTest) ;

#include "Validation/AlCaTest/interface/SelectionsTest.h"
DEFINE_ANOTHER_FWK_MODULE (SelectionsTest) ;
