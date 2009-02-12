import FWCore.ParameterSet.Config as cms

leptonsMinNumber = cms.EDFilter("VBFLeptonsMinNumFilter",
                       GSFInputTag = cms.InputTag("pixelMatchGsfElectrons"),
                       muInputTag = cms.InputTag("muons"),  
                       minLeptonsNum = cms.int32(1)
                      )
