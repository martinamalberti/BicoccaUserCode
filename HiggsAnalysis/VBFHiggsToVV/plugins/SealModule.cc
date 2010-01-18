#include <FWCore/PluginManager/interface/ModuleDef.h>
#include <FWCore/Framework/interface/MakerMacros.h>

#include "PhysicsTools/UtilAlgos/interface/ObjectSelector.h"
#include "PhysicsTools/UtilAlgos/interface/SortCollectionSelector.h"



//define this as a plug-in
DEFINE_SEAL_MODULE();






//-------------------------------------------------
// VBFElectron/Muon/Lepton/Jet/MetDistributions
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFElectronDistributions.h"
DEFINE_ANOTHER_FWK_MODULE(VBFElectronDistributions);
#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFMuonDistributions.h"
DEFINE_ANOTHER_FWK_MODULE(VBFMuonDistributions);
#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFLeptonDistributions.h"
DEFINE_ANOTHER_FWK_MODULE(VBFLeptonDistributions);
#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFMetDistributions.h"
DEFINE_ANOTHER_FWK_MODULE(VBFMetDistributions);

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFJetDistributions.h"
typedef VBFJetDistributions<reco::CaloJetCollection> VBFCaloJetDistributions;
typedef VBFJetDistributions<reco::PFJetCollection> VBFPFJetDistributions;
DEFINE_ANOTHER_FWK_MODULE(VBFCaloJetDistributions);
DEFINE_ANOTHER_FWK_MODULE(VBFPFJetDistributions);






//-------------------------------------------------
// VBFElectronAmbiguityResolver
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFElectronAmbiguityResolver.h"
typedef ObjectSelector<VBFElectronAmbiguityResolver> VBFElectronAmbiguityResolution;
typedef ObjectSelector<VBFElectronAmbiguityResolver, edm::RefVector<reco::GsfElectronCollection> > VBFElectronAmbiguityResolutionRef;
DEFINE_ANOTHER_FWK_MODULE(VBFElectronAmbiguityResolution);
DEFINE_ANOTHER_FWK_MODULE(VBFElectronAmbiguityResolutionRef);


//-------------------------------------------------
// VBFElectronSelector
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFElectronSelector.h"
typedef ObjectSelector<VBFElectronSelector> VBFElectronSelection;
typedef ObjectSelector<VBFElectronSelector, edm::RefVector<reco::GsfElectronCollection> > VBFElectronSelectionRef;
DEFINE_ANOTHER_FWK_MODULE(VBFElectronSelection);
DEFINE_ANOTHER_FWK_MODULE(VBFElectronSelectionRef);


//-------------------------------------------------
// VBFElectronIsolator
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFElectronIsolator.h"
typedef ObjectSelector<VBFElectronIsolator> VBFElectronIsolation;
typedef ObjectSelector<VBFElectronIsolator, edm::RefVector<reco::GsfElectronCollection> > VBFElectronIsolationRef;
DEFINE_ANOTHER_FWK_MODULE(VBFElectronIsolation);
DEFINE_ANOTHER_FWK_MODULE(VBFElectronIsolationRef);


//-------------------------------------------------
// VBFElectronIdSelector
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFElectronIdSelector.h"
typedef ObjectSelector<VBFElectronIdSelector> VBFElectronIdSelection;
typedef ObjectSelector<VBFElectronIdSelector, edm::RefVector<reco::GsfElectronCollection> > VBFElectronIdSelectionRef;
DEFINE_ANOTHER_FWK_MODULE(VBFElectronIdSelection);
DEFINE_ANOTHER_FWK_MODULE(VBFElectronIdSelectionRef);






//-------------------------------------------------
// VBFMuonSelector
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFMuonSelector.h"
typedef ObjectSelector<VBFMuonSelector> VBFMuonSelection;
typedef ObjectSelector<VBFMuonSelector, edm::RefVector<reco::MuonCollection> > VBFMuonSelectionRef;
DEFINE_ANOTHER_FWK_MODULE(VBFMuonSelection);
DEFINE_ANOTHER_FWK_MODULE(VBFMuonSelectionRef);


//-------------------------------------------------
// VBFMuonIsolator
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFMuonIsolator.h"
typedef ObjectSelector<VBFMuonIsolator> VBFMuonIsolation;
typedef ObjectSelector<VBFMuonIsolator, edm::RefVector<reco::MuonCollection> > VBFMuonIsolationRef;
DEFINE_ANOTHER_FWK_MODULE(VBFMuonIsolation);
DEFINE_ANOTHER_FWK_MODULE(VBFMuonIsolationRef);






//-------------------------------------------------                                                                                             
// VBFLeptonTipLipProducer                                                                                                                      
//-------------------------------------------------                                                                                             

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFLeptonTipLipProducer.h"
DEFINE_ANOTHER_FWK_MODULE(VBFLeptonTipLipProducer);






//-------------------------------------------------
// VBFJetCleaner
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFJetCleaner.h"
typedef ObjectSelector<VBFJetCleaner<reco::CaloJetCollection> > VBFCaloJetCleaning;
typedef ObjectSelector<VBFJetCleaner<reco::CaloJetCollection>, edm::RefVector<reco::CaloJetCollection> > VBFCaloJetCleaningRef;
typedef ObjectSelector<VBFJetCleaner<reco::PFJetCollection> > VBFPFJetCleaning;
typedef ObjectSelector<VBFJetCleaner<reco::PFJetCollection>, edm::RefVector<reco::PFJetCollection> > VBFPFJetCleaningRef;
DEFINE_ANOTHER_FWK_MODULE(VBFCaloJetCleaning);
DEFINE_ANOTHER_FWK_MODULE(VBFCaloJetCleaningRef);
DEFINE_ANOTHER_FWK_MODULE(VBFPFJetCleaning);
DEFINE_ANOTHER_FWK_MODULE(VBFPFJetCleaningRef);


//-------------------------------------------------
// VBFJetSelector
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFJetSelector.h"
typedef ObjectSelector<VBFJetSelector<reco::CaloJetCollection> > VBFCaloJetSelection;
typedef ObjectSelector<VBFJetSelector<reco::CaloJetCollection>, edm::RefVector<reco::CaloJetCollection> > VBFCaloJetSelectionRef;
typedef ObjectSelector<VBFJetSelector<reco::PFJetCollection> > VBFPFJetSelection;
typedef ObjectSelector<VBFJetSelector<reco::PFJetCollection>, edm::RefVector<reco::PFJetCollection> > VBFPFJetSelectionRef;
DEFINE_ANOTHER_FWK_MODULE(VBFCaloJetSelection);
DEFINE_ANOTHER_FWK_MODULE(VBFCaloJetSelectionRef);
DEFINE_ANOTHER_FWK_MODULE(VBFPFJetSelection);
DEFINE_ANOTHER_FWK_MODULE(VBFPFJetSelectionRef);






//-------------------------------------------------
// VBFPtMinLeptonCountFilter / VBFEtMinJetCountFilter
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFPtMinLeptonCountFilter.h"
DEFINE_ANOTHER_FWK_MODULE(VBFPtMinLeptonCountFilter);

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFEtMinJetCountFilter.h"
typedef VBFEtMinJetCountFilter<reco::CaloJetCollection> VBFEtMinCaloJetCountFilter;
typedef VBFEtMinJetCountFilter<reco::PFJetCollection>   VBFEtMinPFJetCountFilter;
DEFINE_ANOTHER_FWK_MODULE(VBFEtMinCaloJetCountFilter);
DEFINE_ANOTHER_FWK_MODULE(VBFEtMinPFJetCountFilter);

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFFwdJetCountFilter.h"
typedef VBFFwdJetCountFilter<reco::CaloJetCollection> VBFFwdCaloJetCountFilter;
typedef VBFFwdJetCountFilter<reco::PFJetCollection>   VBFFwdPFJetCountFilter;
DEFINE_ANOTHER_FWK_MODULE(VBFFwdCaloJetCountFilter);
DEFINE_ANOTHER_FWK_MODULE(VBFFwdPFJetCountFilter);



//-------------
// SimpleNtuple 
//-------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/SimpleNtple.h"
DEFINE_ANOTHER_FWK_MODULE(SimpleNtple) ;










//----------------------------------
//----------- MonteCarlo -----------
//----------------------------------

//-------------------------
// MCDecayModeFilter
//-------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/MCDecayModeFilter.h"
DEFINE_ANOTHER_FWK_MODULE(MCDecayModeFilter);



//-------------------------
// MCPtMinLeptonCountFilter 
//-------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/MCPtMinLeptonCountFilter.h"
DEFINE_ANOTHER_FWK_MODULE(MCPtMinLeptonCountFilter);


//----------------------
// MCFwdJetsPreselFilter 
//----------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/MCFwdJetsPreselFilter.h"
DEFINE_ANOTHER_FWK_MODULE(MCFwdJetsPreselFilter) ;







/*
#include "HiggsAnalysis/VBFHiggsToVV/plugins/FwdJetsPreselFilter.h"
DEFINE_ANOTHER_FWK_MODULE(FwdJetsPreselFilter) ;

#include "HiggsAnalysis/VBFHiggsToVV/plugins/LepMinNumFilter.h"
DEFINE_ANOTHER_FWK_MODULE(LepMinNumFilter) ;*/
