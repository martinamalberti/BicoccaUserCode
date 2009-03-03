import FWCore.ParameterSet.Config as cms

cleanedJets = cms.EDFilter("VBFJetCleaning",
   src = cms.InputTag("L2L3CorJetIcone5"),
   GSFInputTag = cms.InputTag("pixelMatchGsfElectrons"),
   maxDeltaR = cms.double(0.2),
   maxHEoverEmE = cms.double(0.2),
   minEleOJetEratio = cms.double(0.9)
)
