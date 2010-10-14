import FWCore.ParameterSet.Config as cms

from PhysicsTools.MiBiCommonPAT.makeMiBiCommonPAT_cff import *



process = cms.Process("MiBiCommonPAT")

makeMiBiCommonPAT(process, GlobalTag="START38_V12::All", MC=False, HLT="RECO", Filter=True, SavePAT=True)


process.TFileService = cms.Service(
    "TFileService", 
    fileName = cms.string("MiBiCommonNTUPLE.root"),
    closeFileFast = cms.untracked.bool(True)
)


process.source.fileNames = cms.untracked.vstring(
#  'file:/gwtera5/users/data/NTUPLES/VBF/WENu_38X.root'
  'file:/gwpool/users/amassiro/VBF/Releases/CMSSW_3_8_3/src/PhysicsTools/MiBiCommonPAT/669A4128-43D0-DF11-AE93-001A92810ACE.root'
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )


# the MiBiNTUPLE
process.load("PhysicsTools.MiBiCommonPAT.SimpleNtuple_cfi")
process.MiBiNTUPLE = process.SimpleNtuple.clone()
process.MiBiNTUPLE.HLTTag = cms.InputTag("TriggerResults","","RECO")

#process.MiBiNTUPLE.saveMCPtHat           = cms.untracked.bool (False),
#process.MiBiNTUPLE.saveMCTTBar           = cms.untracked.bool (False),    
#process.MiBiNTUPLE.saveMCHiggs           = cms.untracked.bool (False),
#process.MiBiNTUPLE.saveMCHiggsWW         = cms.untracked.bool (False),
#process.MiBiNTUPLE.saveMCHiggsGammaGamma = cms.untracked.bool (False),
#process.MiBiNTUPLE.saveGenJet            = cms.untracked.bool (False),

# the MiBiNTUPLE path
process.MiBiNTUPLEPath = cms.Path(process.MiBiNTUPLE)
    
# the MiBiPAT path
cms.Schedule(process.MiBiCommonPAT)
#,process.MiBiNTUPLEPath)





