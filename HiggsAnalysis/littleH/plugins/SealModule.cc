#include <FWCore/PluginManager/interface/ModuleDef.h>
#include <FWCore/Framework/interface/MakerMacros.h>

#include "PhysicsTools/UtilAlgos/interface/ObjectSelector.h"
#include "PhysicsTools/UtilAlgos/interface/SortCollectionSelector.h"



//define this as a plug-in
DEFINE_SEAL_MODULE();

//-------------
// SimpleNtuple 
//-------------

#include "HiggsAnalysis/littleH/plugins/SimpleNtple.h"
DEFINE_ANOTHER_FWK_MODULE(SimpleNtple) ;

//-------------
// LeptonLipTipProducer 
//-------------

#include "HiggsAnalysis/littleH/plugins/LeptonTipLipProducer.h"
DEFINE_ANOTHER_FWK_MODULE(LeptonTipLipProducer) ;

//-------------
// Lepton3DipProducer 
//-------------

#include "HiggsAnalysis/littleH/plugins/Lepton3DipProducer.h"
DEFINE_ANOTHER_FWK_MODULE(Lepton3DipProducer) ;
