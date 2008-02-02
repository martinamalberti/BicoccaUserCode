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
//#include "PhysicsTools/UtilAlgos/interface/SingleObjectRefVectorSelector.h"
#include "DataFormats/Common/interface/RefVector.h"

//define this as a plug-in
DEFINE_SEAL_MODULE () ;

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFHiggsToWW2e.h"
DEFINE_ANOTHER_FWK_MODULE (VBFHiggsToWW2e) ;

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFMCProcessFilter.h"
DEFINE_ANOTHER_FWK_MODULE (VBFMCProcessFilter) ; 

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFMCChannelFilter.h"
DEFINE_ANOTHER_FWK_MODULE (VBFMCChannelFilter) ; 

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFProcessFilter.h"
DEFINE_ANOTHER_FWK_MODULE (VBFProcessFilter) ; 

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFLeptonsNumFilter.h"
DEFINE_ANOTHER_FWK_MODULE (VBFLeptonsNumFilter) ; 

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFReadEvent.h"
DEFINE_ANOTHER_FWK_MODULE (VBFReadEvent) ; 

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFJetTagger.h"
DEFINE_ANOTHER_FWK_MODULE (VBFJetTagger) ; 

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFMCJetTagger.h"
DEFINE_ANOTHER_FWK_MODULE (VBFMCJetTagger) ; 

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFMCFlagEvent.h"
DEFINE_ANOTHER_FWK_MODULE (VBFMCFlagEvent) ; 

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFSimpleJetTagger.h"
typedef ObjectSelector<VBFSimpleJetTagger> VBFJetTagSelector ;
DEFINE_ANOTHER_FWK_MODULE (VBFJetTagSelector) ;

typedef ObjectSelector<
          VBFSimpleJetTagger, 
          edm::RefVector<reco::CaloJetCollection> 
         > VBFJetTagSelectorRef ;
DEFINE_ANOTHER_FWK_MODULE (VBFJetTagSelectorRef) ;

typedef ObjectSelector<
          VBFSimpleJetTagger, 
          edm::RefVector<reco::CaloJetCollection> 
         > VBFJetTagSelectorRef ;
DEFINE_ANOTHER_FWK_MODULE (VBFJetTagSelectorRef) ;

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFJetCleaner.h"
typedef ObjectSelector<VBFJetCleaner> VBFJetCleaning ;
DEFINE_ANOTHER_FWK_MODULE (VBFJetCleaning) ;

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFJetEtaPtSelector.h"
typedef ObjectSelector<VBFJetEtaPtSelector> VBFJetEtaPtSelecting ;
DEFINE_ANOTHER_FWK_MODULE (VBFJetEtaPtSelecting) ;

typedef ObjectSelector<
          VBFJetCleaner, 
          edm::RefVector<reco::CaloJetCollection> 
         > VBFJetCleaningRef ;
DEFINE_ANOTHER_FWK_MODULE (VBFJetCleaningRef) ;

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFJetVetoFilter.h"
DEFINE_ANOTHER_FWK_MODULE (VBFJetVetoFilter) ; 

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFTrivialAnalysis.h"
DEFINE_ANOTHER_FWK_MODULE (VBFTrivialAnalysis) ;

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFplots.h"
DEFINE_ANOTHER_FWK_MODULE (VBFplots) ; 

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFTestTagJets.h"
DEFINE_ANOTHER_FWK_MODULE (VBFTestTagJets) ; 

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFEleIDMeter.h"
DEFINE_ANOTHER_FWK_MODULE (VBFEleIDMeter) ; 

//#include "HiggsAnalysis/HiggsToZZ4e/plugins/HZZ4eTkIsolation.h"
//typedef ObjectSelector<
//          HZZ4eTkIsolation, 
//          edm::RefVector<reco::PixelMatchGsfElectronCollection> 
//         > TkIsoRef ;
//DEFINE_ANOTHER_FWK_MODULE (TkIsoRef) ;



