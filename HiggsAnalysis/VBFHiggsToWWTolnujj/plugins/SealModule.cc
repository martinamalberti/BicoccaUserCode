#include <FWCore/PluginManager/interface/ModuleDef.h>
#include <FWCore/Framework/interface/MakerMacros.h>

#include "PhysicsTools/UtilAlgos/interface/ObjectSelector.h"
#include "PhysicsTools/UtilAlgos/interface/SortCollectionSelector.h"



//define this as a plug-in
DEFINE_SEAL_MODULE();






//-------------------------------------------------
// VBFNtuple
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToWWTolnujj/plugins/VBFNtuple.h"
typedef VBFNtuple<reco::CaloJetCollection> VBFNtupleCaloJets;
typedef VBFNtuple<reco::PFJetCollection> VBFNtuplePFJets;
DEFINE_ANOTHER_FWK_MODULE(VBFNtupleCaloJets);
DEFINE_ANOTHER_FWK_MODULE(VBFNtuplePFJets);
