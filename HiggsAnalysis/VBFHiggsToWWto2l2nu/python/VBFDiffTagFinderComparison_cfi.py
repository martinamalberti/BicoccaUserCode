import FWCore.ParameterSet.Config as cms

jetDiffTagFinder = cms.EDProducer(
    "VBFDiffTagFinderComparison",
     jetInputTag = cms.InputTag("jetTagger:taggedJets"),
     MCtruthInputTag = cms.InputTag("genParticles"),
     algoType = cms.int32(1) # 0 = findTagJets -> Mjj max     1 = findMaxPtJetsPair -> max Pt pair
    )

   
   
    