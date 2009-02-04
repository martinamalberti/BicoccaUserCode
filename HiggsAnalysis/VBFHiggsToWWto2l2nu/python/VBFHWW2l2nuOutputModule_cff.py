import FWCore.ParameterSet.Config as cms



from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFHWW2l2nuEventContent_cff import *



VBFHWW2l2nuOutputModule = cms.OutputModule(
    "PoolOutputModule",
    VBFHWW2l2nuEventContent,
    dataset = cms.untracked.PSet(dataTier = cms.untracked.string('USER')),
    fileName = cms.untracked.string('VBFHWW2l2nuTest.root')
)
