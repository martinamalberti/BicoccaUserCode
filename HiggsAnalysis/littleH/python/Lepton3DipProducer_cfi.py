import FWCore.ParameterSet.Config as cms



Lepton3DipProducer = cms.EDProducer(
    "Lepton3DipProducer",
    srcVertices  = cms.InputTag("offlinePrimaryVertices"),
    srcElectrons = cms.InputTag("gsfElectrons"),
    srcMuons     = cms.InputTag("muons")
    )
