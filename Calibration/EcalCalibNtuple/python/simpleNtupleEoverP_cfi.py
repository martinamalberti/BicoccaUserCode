import FWCore.ParameterSet.Config as cms

simpleNtupleEoverP = cms.EDAnalyzer(
    "SimpleNtupleEoverP",
    MCPileupTag = cms.InputTag ("addPileupInfo"),
    PVTag               = cms.InputTag("offlinePrimaryVerticesWithBS"),
    recHitCollection_EB = cms.InputTag("alCaIsolatedElectrons","alCaRecHitsEB","ALCARECO"),
    recHitCollection_EE = cms.InputTag("alCaIsolatedElectrons","alCaRecHitsEE","ALCARECO"),
    EleTag              = cms.InputTag("gsfElectrons"),
    PFMetTag          = cms.InputTag("pfMet"),
    rhoTag          = cms.InputTag("kt6PFJetsForRhoCorrection","rho"),
    conversionsInputTag    = cms.InputTag("allConversions"),
    jsonFileName = cms.string("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-195016_8TeV_PromptReco_Collisions12_JSON_v3.txt"),
    jsonFlag    = cms.untracked.bool(False),     
    verbosity    = cms.untracked.bool(True),
    doWZSelection    = cms.untracked.bool(True),
    applyCorrections    = cms.untracked.bool(False),
    saveMCPU      = cms.untracked.bool(True),
    )
