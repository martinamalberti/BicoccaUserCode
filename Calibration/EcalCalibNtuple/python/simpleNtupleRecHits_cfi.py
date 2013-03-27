import FWCore.ParameterSet.Config as cms

simpleNtupleRecHits = cms.EDAnalyzer(
    "SimpleNtupleRecHits",
    MCPileupTag = cms.InputTag("addPileupInfo"),
    PVTag               = cms.InputTag("offlinePrimaryVerticesWithBS"),
    recHitCollection_EB = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
    recHitCollection_EE = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
    digiCollection_EB   = cms.InputTag("ecalDigis","ebDigis"),
    digiCollection_EE   = cms.InputTag("ecalDigis","eeDigis"),
    rhoTag          = cms.InputTag("kt6PFJetsForRhoCorrection","rho"),
    dataFlag      = cms.untracked.bool(True)
    )
