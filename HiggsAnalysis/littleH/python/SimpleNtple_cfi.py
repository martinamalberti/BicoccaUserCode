import FWCore.ParameterSet.Config as cms

SimpleNtple = cms.EDAnalyzer(
    "SimpleNtple",
     MuTag = cms.InputTag("muons"),
     EleTag = cms.InputTag("gsfElectrons"),  # pixelMatchGsfElectrons
     TracksTag = cms.InputTag("generalTracks"),
#      MCtruthTag = cms.InputTag("genParticles"),
     verbosity = cms.untracked.bool(False),
     eventType = cms.untracked.int32(0), 
     eleIDCut_LooseInputTag = cms.InputTag("eidLoose"),
     eleIDCut_RLooseInputTag = cms.InputTag("eidRobustLoose"),
     eleIDCut_TightInputTag = cms.InputTag("eidTight"), 
     eleIDCut_RTightInputTag = cms.InputTag("eidRobustTight"),
)