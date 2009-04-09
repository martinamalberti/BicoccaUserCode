import FWCore.ParameterSet.Config as cms

selectedEvents = cms.EDFilter("VBFEventSelector",
      tagJetInputTag = cms.InputTag("jetTagger:taggedJets"),
      jetDeltaEtaMin = cms.double(5),
      jetPtMin = cms.double(15),
      MjjMin = cms.double(40)
     )
