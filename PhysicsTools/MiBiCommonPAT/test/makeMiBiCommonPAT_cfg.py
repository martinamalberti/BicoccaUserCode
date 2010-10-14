import FWCore.ParameterSet.Config as cms

from PhysicsTools.MiBiCommonPAT.makeMiBiCommonPAT_cff import *



process = cms.Process("MiBiCommonPAT")


makeMiBiCommonPAT(process, GlobalTag="START38_V12::All", MC=False, HLT="REDIGI38X", Filter=True, SavePAT=True)


process.TFileService = cms.Service(
    "TFileService", 
    fileName = cms.string("MiBiCommonNTUPLE.root"),
    closeFileFast = cms.untracked.bool(True)
)


process.source.fileNames = cms.untracked.vstring(
  'file:/gwtera5/users/data/NTUPLES/VBF/WENu_38X.root'
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )



