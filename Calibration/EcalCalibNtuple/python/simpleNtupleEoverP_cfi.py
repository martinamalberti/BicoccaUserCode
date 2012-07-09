import FWCore.ParameterSet.Config as cms

simpleNtupleEoverP = cms.EDAnalyzer(
    "SimpleNtupleEoverP",
    MCPileupTag = cms.InputTag ("addPileupInfo"),
    PVTag               = cms.InputTag("offlinePrimaryVerticesWithBS"),
    recHitCollection_EB = cms.InputTag("alCaIsolatedElectrons","alCaRecHitsEB","ALCARERECO"),
    recHitCollection_EE = cms.InputTag("alCaIsolatedElectrons","alCaRecHitsEE","ALCARERECO"),
    EleTag              = cms.InputTag("electronRecalibSCAssociator","","ALCARERECO"),
    PFMetTag            = cms.InputTag("pfMet","","RECO"),
    rhoTag              = cms.InputTag("kt6PFJetsForRhoCorrection","rho"),
    jsonFileName        = cms.string("/afs/cern.ch/user/r/ric/public/Golden190456-193557PlusLaserRecoverable.json"),
    dataRun             = cms.string("2011A"),
    jsonFlag            = cms.untracked.bool(False),	      
    verbosity           = cms.untracked.bool(True),
    doWZSelection       = cms.untracked.bool(True),
    applyCorrections    = cms.untracked.bool(False),
    dataFlag            =  cms.untracked.bool(True),
    saveMCPU      = cms.untracked.bool(False)
   
    )
