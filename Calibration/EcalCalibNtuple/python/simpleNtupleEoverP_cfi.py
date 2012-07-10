import FWCore.ParameterSet.Config as cms

simpleNtupleEoverP = cms.EDAnalyzer(
    "SimpleNtupleEoverP",
    MCPileupTag = cms.InputTag ("addPileupInfo"),
    PVTag               = cms.InputTag("offlinePrimaryVerticesWithBS"),
    recHitCollection_EB = cms.InputTag("alCaIsolatedElectrons","alCaRecHitsEB","ALCARERECO"),
    recHitCollection_EE = cms.InputTag("alCaIsolatedElectrons","alCaRecHitsEE","ALCARERECO"),
    EleTag              = cms.InputTag("electronRecalibSCAssociator","","ALCARERECO"),
    PFMetTag          = cms.InputTag("pfMet"),
    rhoTag          = cms.InputTag("kt6PFJetsForRhoCorrection","rho"),
    conversionsInputTag    = cms.InputTag("allConversions"),
    jsonFileName = cms.string("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-196531_8TeV_PromptReco_Collisions12_JSON.txt"),
    dataRun             = cms.string("2012A"),
    jsonFlag    = cms.untracked.bool(True),     
    verbosity    = cms.untracked.bool(True),
    doWZSelection    = cms.untracked.bool(True),
    applyCorrections    = cms.untracked.bool(False),
    saveMCPU      = cms.untracked.bool(False),
    dataFlag      = cms.untracked.bool(True)
    )
