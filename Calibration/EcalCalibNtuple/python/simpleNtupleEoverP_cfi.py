import FWCore.ParameterSet.Config as cms

simpleNtupleEoverP = cms.EDAnalyzer(
    "SimpleNtupleEoverP",
    recHitCollection_EB = cms.InputTag("alCaIsolatedElectrons","alcaBarrelHits"),
    recHitCollection_EE = cms.InputTag("alCaIsolatedElectrons","alcaEndcapHits"),
    EleTag              = cms.InputTag("electronRecalibSCAssociator"),
    PFMetTag          = cms.InputTag("pfMet"),
    verbosity_    = cms.untracked.bool(True)
    )
