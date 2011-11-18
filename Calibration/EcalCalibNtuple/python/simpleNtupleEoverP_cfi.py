import FWCore.ParameterSet.Config as cms

simpleNtupleEoverP = cms.EDAnalyzer(
    "SimpleNtupleEoverP",
    recHitCollection_EB = cms.InputTag("alCaIsolatedElectrons","alcaBarrelHits"),
    recHitCollection_EE = cms.InputTag("alCaIsolatedElectrons","alcaEndcapHits"),
    EleTag              = cms.InputTag("electronRecalibSCAssociator"),
    PFMetTag          = cms.InputTag("pfMet"),
    rhoTag          = cms.InputTag("kt6PFJetsForRhoCorrection","rho"),
    verbosity    = cms.untracked.bool(True),
    doTighterSel    = cms.untracked.bool(True)
    )
