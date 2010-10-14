import FWCore.ParameterSet.Config as cms



process = cms.Process("MiBiCommonNTUPLE")

process.TFileService = cms.Service(
    "TFileService", 
    fileName = cms.string("MiBiCommonNTUPLE.root"),
    closeFileFast = cms.untracked.bool(True)
)


process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring('file:./MiBiCommonPAT.root')
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )


# the MiBiNTUPLE
process.load("PhysicsTools.MiBiCommonPAT.SimpleNtuple_cfi")
process.MiBiCommonNTUPLE = process.SimpleNtuple.clone()
process.MiBiCommonNTUPLE.HLTTag = cms.InputTag("TriggerResults","","RECO")

#process.MiBiNTUPLE.saveMCPtHat           = cms.untracked.bool (False),
#process.MiBiNTUPLE.saveMCTTBar           = cms.untracked.bool (False),    
#process.MiBiNTUPLE.saveMCHiggs           = cms.untracked.bool (False),
#process.MiBiNTUPLE.saveMCHiggsWW         = cms.untracked.bool (False),
#process.MiBiNTUPLE.saveMCHiggsGammaGamma = cms.untracked.bool (False),
#process.MiBiNTUPLE.saveGenJet            = cms.untracked.bool (False),

# the MiBiNTUPLE path
process.MiBiCommonNTUPLEPath = cms.Path(process.MiBiCommonNTUPLE)


# the MiBiPAT path
cms.Schedule(process.MiBiCommonNTUPLEPath)





