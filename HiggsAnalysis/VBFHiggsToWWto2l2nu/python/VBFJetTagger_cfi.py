import FWCore.ParameterSet.Config as cms

jetTagger = cms.EDProducer(
    "VBFJetTagger",
     jetInputTag = cms.InputTag("cleanedJets"),
     tagJetsName = cms.string("taggedJets"),
     otherJetsName = cms.string("otherJets"),
     jetEtaMax = cms.double(5.),
     jetPtMin = cms.double(15.), # GeV
     gatherConeSize = cms.double(0.5),
     jetDEtaMin = cms.untracked.double(0),
     jetMjjMin = cms.untracked.double(10),     
     algoType = cms.int32(1) # 0 = findTagJets -> Mjj max     1 = findMaxPtJetsPair -> max Pt pair 2 = findMaxNNJetsPair_PtOrdered
    )

   
   
    