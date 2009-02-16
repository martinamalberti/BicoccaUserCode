import FWCore.ParameterSet.Config as cms

jetTagger = cms.EDProducer(
    "VBFJetTagger",
     jetInputTag = cms.InputTag("iterativeCone5CaloJets"),
     tagJetsName = cms.string("taggedJets"),
     otherJetsName = cms.string("otherJets"),
     jetEtaMax = cms.double(2.3), # GeV
     jetPtMin = cms.double(35.), # GeV
     gatherConeSize = cms.double(0.2),
     algoType = cms.int32(0) # 0 = findTagJets -> Mjj max     1 = findMaxPtJetsPair -> max Pt pair
    )

   
   
    