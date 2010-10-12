import FWCore.ParameterSet.Config as cms

process = cms.Process("MiBiCommonPAT")

from PhysicsTools.MiBiCommonPAT.makeMiBiCommonPAT_cff import *

makeMiBiCommonPAT(process, GlobalTag="START38_V12::All", MC=True, HLT="REDIGI38X", Filter=True, SavePAT=True)

process.source.fileNames = cms.untracked.vstring(
  'file:/gwtera5/users/data/NTUPLES/VBF/WENu_38X.root'
)

process.TFileService = cms.Service(
    "TFileService", 
    fileName = cms.string("Counters.root"),
    closeFileFast = cms.untracked.bool(True)
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

