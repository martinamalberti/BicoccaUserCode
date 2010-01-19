import FWCore.ParameterSet.Config as cms



LeptonTipLipProducer = cms.EDProducer(
    "LeptonTipLipProducer",
    srcVertices  = cms.InputTag("offlinePrimaryVertices"),
    srcElectrons = cms.InputTag("gsfElectrons"),
    srcMuons     = cms.InputTag("muons")
    )



