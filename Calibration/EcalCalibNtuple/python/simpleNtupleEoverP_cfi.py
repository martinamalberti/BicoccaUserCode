import FWCore.ParameterSet.Config as cms

simpleNtupleEoverP = cms.EDAnalyzer(
    "SimpleNtupleEoverP",
    MCPileupTag = cms.InputTag ("addPileupInfo"),
    PVTag               = cms.InputTag("offlinePrimaryVerticesWithBS"),
    recHitCollection_EB = cms.InputTag("alCaIsolatedElectrons","alCaRecHitsEB","ALCARERECO"),
    recHitCollection_EE = cms.InputTag("alCaIsolatedElectrons","alCaRecHitsEE","ALCARERECO"),
    EleTag              = cms.InputTag("electronRecalibSCAssociator","","ALCARERECO"),
    PFMetTag          = cms.InputTag("pfMet"),
    MCtruthTag          = cms.InputTag("genParticles"),
    rhoTag          = cms.InputTag("kt6PFJetsForRhoCorrection","rho"),
    conversionsInputTag    = cms.InputTag("allConversions"),
#    jsonFileName = cms.string("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-196531_8TeV_PromptReco_Collisions12_JSON.txt"),
    jsonFileName = cms.string("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing/Cert_190456-196531_8TeV_29Jun2012ReReco_Collisions12_JSON.txt"),
    dataRun             = cms.string("noCorrection"),
    jsonFlag    = cms.untracked.bool(False),
    verbosity    = cms.untracked.bool(False),
    doWZSelection    = cms.untracked.bool(True),
    applyCorrections    = cms.untracked.bool(False),
    saveMCPU      = cms.untracked.bool(False),
    saveMCInfo = cms.untracked.bool(True),
    dataFlag      = cms.untracked.bool(True),
    saveRecHitMatrix  = cms.untracked.bool(False),
    saveFbrem = cms.untracked.bool(True)   # set Flase if running on AOD
    )
