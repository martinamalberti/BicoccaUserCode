import FWCore.ParameterSet.Config as cms

cleanedJetsRef = cms.EDFilter("VBFJetCleaningRef",
                           src = cms.InputTag("iterativeCone5CaloJets"),
                           GSFInputTag = cms.InputTag("pixelMatchGsfElectrons"),
                           maxDeltaR = cms.double(0.2),
                           maxHEoverEmE = cms.double(0.2),
                           minEleOJetEratio = cms.double(0.9)
                           )
