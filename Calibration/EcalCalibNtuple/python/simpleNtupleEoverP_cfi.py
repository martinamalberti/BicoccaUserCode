import FWCore.ParameterSet.Config as cms

simpleNtupleEoverP = cms.EDAnalyzer(
    "SimpleNtupleEoverP",
    MCPileupTag         = cms.InputTag("addPileupInfo"),
    PVTag               = cms.InputTag("offlinePrimaryVerticesWithBS"),
    recHitCollection_EB = cms.InputTag("reducedEcalRecHitsEB"),
    recHitCollection_EE = cms.InputTag("reducedEcalRecHitsEE"),
    SRFlagCollection_EB = cms.InputTag("ecalDigis"),
    SRFlagCollection_EE = cms.InputTag("ecalDigis"),
    digiCollection_EB   = cms.InputTag("ecalDigis"),
    digiCollection_EE   = cms.InputTag("ecalDigis"),
    EleTag              = cms.InputTag("gsfElectrons"),
    PFMetTag            = cms.InputTag("pfMet"),
    MCtruthTag          = cms.InputTag("genParticles"),
    rhoTag              = cms.InputTag("kt6PFJets","rho"),
    conversionsInputTag = cms.InputTag("allConversions"),
    jsonFileName        = cms.string("dummy.txt"),
    dataRun             = cms.string("noCorrection"),
    jsonFlag            = cms.untracked.bool(False),
    verbosity           = cms.untracked.bool(False),
    doWZSelection       = cms.untracked.bool(True),
    applyCorrections    = cms.untracked.bool(False),
    saveMCPU            = cms.untracked.bool(False),
    saveMCInfo          = cms.untracked.bool(False),
    dataFlag            = cms.untracked.bool(True),
    saveRecHitMatrix    = cms.untracked.bool(False),
    saveFbrem           = cms.untracked.bool(False) # set False if running on AOD
    )
