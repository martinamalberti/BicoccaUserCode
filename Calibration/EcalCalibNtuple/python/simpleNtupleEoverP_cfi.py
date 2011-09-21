import FWCore.ParameterSet.Config as cms

simpleNtupleEoverP = cms.EDAnalyzer(
    "SimpleNtupleEoverP",
    recHitCollection_EB = cms.InputTag("alCaIsolatedElectrons","alcaBarrelHits"),
    recHitCollection_EE = cms.InputTag("alCaIsolatedElectrons","alcaEndcapHits"),
    EleTag              = cms.InputTag("gsfElectrons"),
    CALOMetTag          = cms.InputTag("met"),
    verbosity_    = cms.untracked.bool(True)
    )
