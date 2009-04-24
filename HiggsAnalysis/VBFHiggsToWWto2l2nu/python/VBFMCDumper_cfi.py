import FWCore.ParameterSet.Config as cms

VBFMCDumper = cms.EDAnalyzer(
    "VBFMCDumper",
    srcHepMCProduct = cms.InputTag("source"),
    srcGenParticles = cms.InputTag("genParticles"),
    srcGenMet = cms.InputTag("genMetNoNuBSM"),
    srcIC5GenJets = cms.InputTag("iterativeCone5GenJets"),
    srcIC5CaloJets = cms.InputTag("iterativeCone5CaloJets"),
    fileName = cms.untracked.string("VBFMCDump"),
    mass = cms.string("300"),
    verbosity = cms.untracked.bool(True),
    eventsToPrint = cms.untracked.int32(1)
    )
