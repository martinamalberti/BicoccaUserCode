import FWCore.ParameterSet.Config as cms

from PhysicsTools.MiBiCommonPAT.makeMiBiCommonPAT_cff import *



process = cms.Process("MiBiCommonPAT")

makeMiBiCommonPAT(process, GlobalTag="START38_V12::All", MC=True, HLT="REDIGI38X", Filter=True, SavePAT=True)


process.TFileService = cms.Service(
    "TFileService", 
    fileName = cms.string("MiBiCommonNTUPLE.root"),
    closeFileFast = cms.untracked.bool(True)
)


process.source.fileNames = cms.untracked.vstring(
  'file:/gwtera5/users/data/NTUPLES/VBF/WENu_38X.root'
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )


# the MiBiNTUPLE
process.load("PhysicsTools.MiBiCommonPAT.SimpleNtuple_cfi")
process.MiBiNTUPLE = process.SimpleNtuple.clone()
process.MiBiNTUPLE.HLTTag = cms.InputTag("TriggerResults","","REDIGI38X")

# the MiBiNTUPLE path
process.MiBiNTUPLEPath = cms.Path(process.MiBiNTUPLE)
    
# the MiBiPAT path
cms.Schedule(process.MiBiCommonPAT,process.MiBiNTUPLEPath)





