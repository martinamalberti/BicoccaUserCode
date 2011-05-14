import FWCore.ParameterSet.Config as cms

from PhysicsTools.MiBiCommonPAT.makeMiBiCommonNT_cff import *

process = cms.Process("MiBiCommonNT")

# the MiBiNT
makeMiBiCommonNT(process, GlobalTag="FT_R_311_V4A::All", HLT='', MC=False, MCType='Other')
  
process.source.fileNames = cms.untracked.vstring(
    #May10 su cmsmi5
    'file:/data2/Datasets/SingleElectron_Run2011A-May10ReReco-v1_AOD.root'

)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.TFileService = cms.Service(
    "TFileService", 
    fileName = cms.string("MiBiCommonNT.root"),
    closeFileFast = cms.untracked.bool(True)
)

