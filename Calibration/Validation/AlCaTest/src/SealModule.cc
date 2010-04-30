#include "FWCore/Framework/interface/MakerMacros.h"

#include "Validation/AlCaTest/interface/AlCaElectronsTest.h"
DEFINE_FWK_MODULE (AlCaElectronsTest) ;

#include "Validation/AlCaTest/interface/AlCaRecHitsTest.h"
DEFINE_FWK_MODULE (AlCaRecHitsTest) ;

#include "Validation/AlCaTest/interface/AlCaSuperClustersTest.h"
DEFINE_FWK_MODULE (AlCaSuperClustersTest) ;

#include "Validation/AlCaTest/interface/AlCaL1Efficiencies.h"
DEFINE_FWK_MODULE (AlCaL1Efficiencies) ;

#include "Validation/AlCaTest/interface/AlCaHLTEfficiencies.h"
DEFINE_FWK_MODULE (AlCaHLTEfficiencies) ;

#include "Validation/AlCaTest/interface/SelectionsTest.h"
DEFINE_FWK_MODULE (SelectionsTest) ;
