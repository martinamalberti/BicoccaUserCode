#include <FWCore/PluginManager/interface/ModuleDef.h>
#include <FWCore/Framework/interface/MakerMacros.h>


#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SortCollectionSelector.h"



//define this as a plug-in
DEFINE_SEAL_MODULE();

//-------------
// SimpleNtuple 
//-------------

#include "HiggsAnalysis/TTBarAnalysis/plugins/SimpleNtpleTTBar.h"
DEFINE_ANOTHER_FWK_MODULE(SimpleNtpleTTBar) ;

#include "HiggsAnalysis/TTBarAnalysis/plugins/SimpleNtpleWJets.h"
DEFINE_ANOTHER_FWK_MODULE(SimpleNtpleWJets) ;

#include "HiggsAnalysis/TTBarAnalysis/plugins/NtupleMC.h"
DEFINE_ANOTHER_FWK_MODULE(NtupleMC) ;
