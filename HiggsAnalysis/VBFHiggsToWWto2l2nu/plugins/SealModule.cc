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

#include "TParticle.h"


//define this as a plug-in
DEFINE_SEAL_MODULE () ;


#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFEventsCounter.h"
DEFINE_ANOTHER_FWK_MODULE (VBFEventsCounter) ;

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFSkimEfficiencyNtuple.h"
DEFINE_ANOTHER_FWK_MODULE (VBFSkimEfficiencyNtuple) ;

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFTagsMaxPt.h"
DEFINE_ANOTHER_FWK_MODULE (VBFTagsMaxPt) ;

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFJetNumFilter.h"
DEFINE_ANOTHER_FWK_MODULE (VBFJetNumFilter) ;

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFDiffTagFinderComparison.h"
DEFINE_ANOTHER_FWK_MODULE (VBFDiffTagFinderComparison) ;

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFMCProcessFilter.h"
DEFINE_ANOTHER_FWK_MODULE (VBFMCProcessFilter) ; 

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFMCChannelFilter.h"
DEFINE_ANOTHER_FWK_MODULE (VBFMCChannelFilter) ; 

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFProcessFilter.h"
DEFINE_ANOTHER_FWK_MODULE (VBFProcessFilter) ; 

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFLeptFilter.h"
DEFINE_ANOTHER_FWK_MODULE (VBFLeptFilter) ; 

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFElePtFilter.h"
DEFINE_ANOTHER_FWK_MODULE (VBFElePtFilter) ; 

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFLeptonsNumFilter.h"
DEFINE_ANOTHER_FWK_MODULE (VBFLeptonsNumFilter) ; 

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFEleNumFilter.h"
DEFINE_ANOTHER_FWK_MODULE (VBFEleNumFilter) ; 

// #include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFJetNumFilter.h"
// DEFINE_ANOTHER_FWK_MODULE (VBFJetNumFilter) ;

// #include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFReadEvent.h"
// DEFINE_ANOTHER_FWK_MODULE (VBFReadEvent) ; 

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFJetCleanerPlots.h"
DEFINE_ANOTHER_FWK_MODULE (VBFJetCleanerPlots) ;

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFJetTagger.h"
DEFINE_ANOTHER_FWK_MODULE (VBFJetTagger) ; 

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFUtils.h"
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFJetTaggerAfterCuts.h"
typedef VBFJetTaggerAfterCuts<vbfhww2l::MaxPtSorting> VBFJetTaggerMaxPt ;
DEFINE_ANOTHER_FWK_MODULE (VBFJetTaggerMaxPt) ; 

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFMCJetTagger.h"
DEFINE_ANOTHER_FWK_MODULE (VBFMCJetTagger) ; 

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFMCFlagEvent.h"
DEFINE_ANOTHER_FWK_MODULE (VBFMCFlagEvent) ; 

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFSimpleJetTagger.h"
typedef ObjectSelector<VBFSimpleJetTagger> VBFJetTagSelector ;
DEFINE_ANOTHER_FWK_MODULE (VBFJetTagSelector) ;

typedef ObjectSelector<
          VBFSimpleJetTagger, 
          edm::RefVector<reco::CaloJetCollection> 
         > VBFJetTagSelectorRef ;
DEFINE_ANOTHER_FWK_MODULE (VBFJetTagSelectorRef) ;

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFJetCleaner.h"
typedef ObjectSelector<
          VBFJetCleaner
         > VBFJetCleaning ;
DEFINE_ANOTHER_FWK_MODULE (VBFJetCleaning) ;

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFJetCleanerTemplate.h"
typedef ObjectSelector<VBFJetCleanerTemplate<reco::CaloJetCollection> > VBFJetCleaningTemplateCaloJet ;
typedef ObjectSelector<VBFJetCleanerTemplate<reco::PFJetCollection> > VBFJetCleaningTemplatePFJet ;
DEFINE_ANOTHER_FWK_MODULE (VBFJetCleaningTemplateCaloJet) ;
DEFINE_ANOTHER_FWK_MODULE (VBFJetCleaningTemplatePFJet) ;

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFJetEtaPtSelector.h"
typedef ObjectSelector<VBFJetEtaPtSelector> VBFJetEtaPtSelecting ;
DEFINE_ANOTHER_FWK_MODULE (VBFJetEtaPtSelecting) ;

typedef ObjectSelector<
          VBFJetCleaner, 
          edm::RefVector<reco::CaloJetCollection> 
         > VBFJetCleaningRef ;
DEFINE_ANOTHER_FWK_MODULE (VBFJetCleaningRef) ;

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFJetVetoFilter.h"
DEFINE_ANOTHER_FWK_MODULE (VBFJetVetoFilter) ; 

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFCentralJetVetoFilter.h"
DEFINE_ANOTHER_FWK_MODULE (VBFCentralJetVetoFilter) ; 

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFTrackCountVetoFilter.h"
DEFINE_ANOTHER_FWK_MODULE (VBFTrackCountVetoFilter) ; 

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFTrivialAnalysis.h"
DEFINE_ANOTHER_FWK_MODULE (VBFTrivialAnalysis) ;

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFEleIsolationStudy.h"
DEFINE_ANOTHER_FWK_MODULE (VBFEleIsolationStudy) ;

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFEleIsolationDump.h"
DEFINE_ANOTHER_FWK_MODULE (VBFEleIsolationDump) ;

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFplots.h"
DEFINE_ANOTHER_FWK_MODULE (VBFplots) ; 

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFElePlots.h"
DEFINE_ANOTHER_FWK_MODULE (VBFElePlots) ; 

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFEleSelectionsStudy.h"
DEFINE_ANOTHER_FWK_MODULE (VBFEleSelectionsStudy) ; 

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFLeptPlots.h"
DEFINE_ANOTHER_FWK_MODULE (VBFLeptPlots) ; 

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFTestTagJets.h"
DEFINE_ANOTHER_FWK_MODULE (VBFTestTagJets) ; 

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFMetCorrector.h"
DEFINE_ANOTHER_FWK_MODULE (VBFMetCorrector) ; 

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFEleIDMeter.h"
DEFINE_ANOTHER_FWK_MODULE (VBFEleIDMeter) ; 

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFDisplay.h"
DEFINE_ANOTHER_FWK_MODULE (VBFDisplay) ; 

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFElectronIsolator.h"
typedef ObjectSelector<VBFElectronIsolator> VBFElectronIsolation ;
DEFINE_ANOTHER_FWK_MODULE (VBFElectronIsolation) ;

typedef ObjectSelector<
          VBFElectronIsolator, 
          edm::RefVector<reco::PixelMatchGsfElectronCollection> 
        > VBFElectronIsolationRef ;
DEFINE_ANOTHER_FWK_MODULE (VBFElectronIsolationRef) ;

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFElectronHadIsolator.h"
typedef ObjectSelector<VBFElectronHadIsolator> VBFElectronHadIsolation ;
DEFINE_ANOTHER_FWK_MODULE (VBFElectronHadIsolation) ;

typedef ObjectSelector<
          VBFElectronHadIsolator, 
          edm::RefVector<reco::PixelMatchGsfElectronCollection> 
        > VBFElectronHadIsolationRef ;
DEFINE_ANOTHER_FWK_MODULE (VBFElectronHadIsolationRef) ;

#include "PhysicsTools/UtilAlgos/interface/EtMinSelector.h"
#include "PhysicsTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CorrMETData.h"
#include "DataFormats/METReco/interface/SpecificCaloMETData.h"

typedef SingleObjectSelector<
          reco::CaloMETCollection,
          EtMinSelector
        > EtMinMETSelector ;

DEFINE_ANOTHER_FWK_MODULE (EtMinMETSelector) ;



//---- New ----


#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFEventSelector.h"
DEFINE_ANOTHER_FWK_MODULE (VBFEventSelector) ; 


#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFLeptonsMinNumFilter.h"
DEFINE_ANOTHER_FWK_MODULE (VBFLeptonsMinNumFilter) ; 


#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFMuonIsolator.h"
typedef ObjectSelector<VBFMuonIsolator> VBFMuonIsolation;
typedef ObjectSelector<VBFMuonIsolator, edm::RefVector<reco::MuonCollection> > VBFMuonIsolationRef;
DEFINE_ANOTHER_FWK_MODULE (VBFMuonIsolation);
DEFINE_ANOTHER_FWK_MODULE (VBFMuonIsolationRef);


#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFMuonSelector.h"
typedef ObjectSelector<VBFMuonSelector> VBFMuonSelection;
typedef ObjectSelector<VBFMuonSelector, edm::RefVector<reco::MuonCollection> > VBFMuonSelectionRef;
DEFINE_ANOTHER_FWK_MODULE (VBFMuonSelection);
DEFINE_ANOTHER_FWK_MODULE (VBFMuonSelectionRef);


#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFEleIDSelector.h"
typedef ObjectSelector<VBFEleIDSelector> VBFEleIDSelection;
typedef ObjectSelector<VBFEleIDSelector, edm::RefVector<reco::PixelMatchGsfElectronCollection> > VBFEleIDSelectionRef;
DEFINE_ANOTHER_FWK_MODULE (VBFEleIDSelection);
DEFINE_ANOTHER_FWK_MODULE (VBFEleIDSelectionRef);



#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFEleHETIsolator.h"
typedef ObjectSelector<VBFEleHETIsolator> VBFEleHETIsolation;
typedef ObjectSelector<VBFEleHETIsolator, edm::RefVector<reco::PixelMatchGsfElectronCollection> > VBFEleHETIsolationRef;
DEFINE_ANOTHER_FWK_MODULE (VBFEleHETIsolation);
DEFINE_ANOTHER_FWK_MODULE (VBFEleHETIsolationRef);


//-------------

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/SimpleNtple.h"
DEFINE_ANOTHER_FWK_MODULE (SimpleNtple) ;

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFTreeProducer.h"
DEFINE_ANOTHER_FWK_MODULE (VBFTreeProducer);

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFMCEventProducer.h"
DEFINE_ANOTHER_FWK_MODULE (VBFMCEventProducer);

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFEvInfoProducer.h"
DEFINE_ANOTHER_FWK_MODULE (VBFEvInfoProducer);

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFMuonProducer.h"
DEFINE_ANOTHER_FWK_MODULE (VBFMuonProducer);

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFElectronProducer.h"
DEFINE_ANOTHER_FWK_MODULE (VBFElectronProducer);

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFMetProducer.h"
DEFINE_ANOTHER_FWK_MODULE (VBFMetProducer);

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFJetProducer.h"
DEFINE_ANOTHER_FWK_MODULE (VBFJetProducer);

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFTrackProducer.h"
DEFINE_ANOTHER_FWK_MODULE (VBFTrackProducer);


//---- Tree producer ----

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFFirstFilterTreeProducer.h"
DEFINE_ANOTHER_FWK_MODULE (VBFFirstFilterTreeProducer);

