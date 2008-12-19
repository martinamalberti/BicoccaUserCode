import FWCore.ParameterSet.Config as cms

process = cms.Process("trivialAnalysisTest")
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/gwtera5/users/govoni/Phantom/test/phamom_test3.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.TFileService = cms.Service ("TFileService", 
    fileName = cms.string ("histo.root")
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.MessageLogger = cms.Service("MessageLogger")

#process.tagJets = cms.EDProducer ("VBFJetTagger",
#    jetInputTag  = cms.InputTag ("iterativeCone5CaloJets"),
#    tagJetsName = cms.string ("tagJets"),
#    jetEtaMax = cms.double (5) ,
#    jetPtMin = cms.double (15), # GeV
#    gatherConeSize = cms.double (0.1)
#)

process.kinematics = cms.EDAnalyzer("VBFKinematics",
    eleIDInputTag = cms.InputTag("electronId"),
    GSFInputTag = cms.InputTag("pixelMatchGsfElectrons"),
    muInputTag = cms.InputTag("muons"),
    jetTagsInputTag = cms.InputTag("tagJets","tagJets"),
    metInputTag = cms.InputTag("met")
)

process.reading = cms.Path(process.kinematics)
#process.reading = cms.Path(process.tagJets, process.kinematics)

