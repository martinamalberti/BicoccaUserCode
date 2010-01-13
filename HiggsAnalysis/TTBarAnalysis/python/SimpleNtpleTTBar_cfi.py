import FWCore.ParameterSet.Config as cms

SimpleNtpleTTBar = cms.EDAnalyzer(
    "SimpleNtpleTTBar",
     MuTag = cms.InputTag("muons"),
     EleTag = cms.InputTag("pixelMatchGsfElectrons"),
     TracksTag = cms.InputTag("generalTracks"),
     JetTag = cms.InputTag("sisCone5CaloJets"),
     flag_JetBTag = cms.untracked.bool(True),
     JetBTag = cms.untracked.InputTag("newJetBProbabilityBJetTags"),
     correctedJetTag = cms.InputTag("dummy"),
     MetTag = cms.InputTag("met"),         
     MCtruthTag = cms.InputTag("genParticles"),
     genJetTag = cms.InputTag("sisCone5GenJets"),
     genMetTag = cms.InputTag("genMet"),
     verbosity = cms.untracked.bool(True),
     eventType = cms.untracked.int32(0), 
)