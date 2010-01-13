#include <FWCore/PluginManager/interface/ModuleDef.h>
#include <FWCore/Framework/interface/MakerMacros.h>

#include "PhysicsTools/UtilAlgos/interface/ObjectSelector.h"
#include "PhysicsTools/UtilAlgos/interface/SortCollectionSelector.h"



//define this as a plug-in
DEFINE_SEAL_MODULE();

//-------------
// SimpleNtuple 
//-------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/SimpleNtple.h"
DEFINE_ANOTHER_FWK_MODULE(SimpleNtple) ;
