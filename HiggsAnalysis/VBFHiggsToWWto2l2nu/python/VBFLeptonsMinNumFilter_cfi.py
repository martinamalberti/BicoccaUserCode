import FWCore.ParameterSet.Config as cms

leptonsMinNumber = cms.EDFilter("VBFLeptonsMinNumFilter",
                       m_GSFInputTag = cms.InputTag("pixelMatchGsfElectrons"),
                       m_muInputTag = cms.InputTag("muons"),  
                       m_minLeptonsNum = cms.int32(1)
                      )
