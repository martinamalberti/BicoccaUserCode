import FWCore.ParameterSet.Config as cms

simpleNtupleEoverP = cms.EDAnalyzer(
    "SimpleNtupleEoverP",
    MCPileupTag = cms.InputTag ("addPileupInfo"),
    PVTag               =  cms.InputTag("offlinePrimaryVerticesWithBS"),
    recHitCollection_EB =  cms.InputTag("ecalRecHit","EcalRecHitsEB","RECO"),
    recHitCollection_EE =  cms.InputTag("ecalRecHit","EcalRecHitsEE","RECO"),
    EleTag              =  cms.InputTag("gsfElectrons"),
    rhoTag              =  cms.InputTag("kt6PFJets","rho"),
    SRFlagCollection_EB =  cms.InputTag("ecalDigis"),
    SRFlagCollection_EE =  cms.InputTag("ecalDigis"),
    PFMetTag            =  cms.InputTag("pfMet"),
    MCtruthTag          =  cms.InputTag("genParticles"),
    conversionsInputTag =  cms.InputTag("allConversions"),
    jsonFileName = cms.string("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing/Cert_190456-196531_8TeV_29Jun2012ReReco_Collisions12_JSON.txt"),
    dataRun             = cms.string("noCorrection"),
    jsonFlag    = cms.untracked.bool(False),
    verbosity    = cms.untracked.bool(False),
    doWZSelection    = cms.untracked.bool(True),
    applyCorrections    = cms.untracked.bool(False), #always false
    saveMCPU      = cms.untracked.bool(False),
    saveMCInfo = cms.untracked.bool(False),
    dataFlag      = cms.untracked.bool(True),
    saveRecHitMatrix  = cms.untracked.bool(False),
    saveFbrem = cms.untracked.bool(False)   # set False if running on AOD
    )
