import FWCore.ParameterSet.Config as cms

process = cms.Process("MiBiCommonNTUPLE")


process.TFileService = cms.Service(
    "TFileService", 
    fileName = cms.string("MiBiCommonNTUPLE.root"),
    closeFileFast = cms.untracked.bool(True)
)
 
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:MiBiCommonPAT.root')
)


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )


# the MiBiNTUPLE
process.load("PhysicsTools.MiBiCommonPAT.SimpleNtuple_cfi")
process.MiBiNTUPLE = process.SimpleNtuple.clone()
process.MiBiNTUPLE.HLTTag = cms.InputTag("TriggerResults","","HLT")


# the MiBiNTUPLE path
process.MiBiNTUPLEPath = cms.Path(process.MiBiNTUPLE)
    





