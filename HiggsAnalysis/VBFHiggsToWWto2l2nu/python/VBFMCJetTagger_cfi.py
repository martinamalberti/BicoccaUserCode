import FWCore.ParameterSet.Config as cms

jetMCTagger = cms.EDProducer("VBFMCJetTagger",
     MCParticlesInputTag = cms.InputTag("genParticles"),
     MCtagJetsName = cms.string("VBFMCTagJet")
    )

   
   