import FWCore.ParameterSet.Config as cms

jetCleaner = cms.EDFilter ("VBFJetCleaning",
 src = cms.InputTag("iterativeCone5CaloJets"),
 GSFInputTag = cms.InputTag ("pixelMatchGsfElectrons"),
 maxDeltaR = cms.double (0.5),
 maxHEoverEmE = cms.double (0.2),
 minEleOJetEratio = cms.double (0)
)
