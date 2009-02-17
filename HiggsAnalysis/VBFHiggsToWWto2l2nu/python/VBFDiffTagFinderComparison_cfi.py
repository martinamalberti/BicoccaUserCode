import FWCore.ParameterSet.Config as cms

jetDiffTagFinder = cms.EDProducer(
    "VBFDiffTagFinderComparison",
     jetInputTag = cms.InputTag("jetTagger:taggedJets"),
     MCtruthInputTag = cms.InputTag("genParticles")
    )

   
   
    