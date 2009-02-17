import FWCore.ParameterSet.Config as cms

channelFilterMC = cms.EDProducer(
    "VBFMCChannelFilter",
     MCtruthInputTag = cms.InputTag("genParticles"),
     channel = cms.int32(0) # 1 = uu, 2 = ee, 3 = ue, 0 = all
    )

   
   
    