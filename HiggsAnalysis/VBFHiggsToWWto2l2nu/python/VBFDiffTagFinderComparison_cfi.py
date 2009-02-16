import FWCore.ParameterSet.Config as cms

jetTagger = cms.EDProducer(
    "VBFDiffTagFinderComparison",
     jetInputTag = cms.InputTag("taggedJets"),
     MCtruthInputTag = cms.string("genParticles"),
     algoType = cms.int32(1) # 0 = findTagJets -> Mjj max     1 = findMaxPtJetsPair -> max Pt pair
    )

   
   
    