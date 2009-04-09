import FWCore.ParameterSet.Config as cms

cleanedJetTemplatePFJet = cms.EDFilter("VBFJetCleaningTemplatePFJet",
   src = cms.InputTag("iterativeCone5PFJets"),
   GSFInputTag = cms.InputTag("pixelMatchGsfElectrons"),
   maxDeltaR = cms.double(0.2),
   maxHEoverEmE = cms.double(0.2),
   minEleOJetEratio = cms.double(0.9)
)
