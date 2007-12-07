//PG non sono sicuro che siano gli include giusti
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "PhysicsTools/UtilAlgos/interface/ObjectSelector.h"
#include "PhysicsTools/UtilAlgos/interface/SortCollectionSelector.h"
#include "PhysicsTools/Utilities/interface/PtComparator.h"
#include "PhysicsTools/UtilAlgos/interface/PtMinSelector.h"
#include "PhysicsTools/UtilAlgos/interface/SingleElementCollectionSelector.h"
#include "PhysicsTools/UtilAlgos/interface/ObjectCountFilter.h"
#include "PhysicsTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "PhysicsTools/UtilAlgos/interface/SingleObjectRefVectorSelector.h"
#include "DataFormats/Common/interface/RefVector.h"

//define this as a plug-in
DEFINE_SEAL_MODULE () ;

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFHiggsToWW2e.h"
DEFINE_ANOTHER_FWK_MODULE (VBFHiggsToWW2e) ;

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFMCProcessFilter.h"
DEFINE_ANOTHER_FWK_MODULE (VBFMCProcessFilter) ; 

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFProcessFilter.h"
DEFINE_ANOTHER_FWK_MODULE (VBFProcessFilter) ; 

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFReadEvent.h"
DEFINE_ANOTHER_FWK_MODULE (VBFReadEvent) ; 

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFJetTagger.h"
typedef ObjectSelector<VBFJetTagger> VBFJetTagSelector ;
DEFINE_ANOTHER_FWK_MODULE(VBFJetTagSelector);

typedef ObjectSelector<
          VBFJetTagger, 
          edm::RefVector<reco::CaloJetCollection> 
         > VBFJetTagSelectorRef ;
DEFINE_ANOTHER_FWK_MODULE(VBFJetTagSelectorRef);
