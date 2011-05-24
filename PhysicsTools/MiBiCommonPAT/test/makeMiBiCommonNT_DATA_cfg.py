import FWCore.ParameterSet.Config as cms

from PhysicsTools.MiBiCommonPAT.makeMiBiCommonNT_cff import *

process = cms.Process("MiBiCommonNT")

# the MiBiNT
makeMiBiCommonNT(process, GlobalTag="GR_R_42_V12::All", HLT='', MC=False, MCType='Other')
  
process.source.fileNames = cms.untracked.vstring(
    #May10 su cmsmi5
  'file:/data2/amassiro/CMSSWRoot/SingleElectron_Run2011A-PromptReco-v4_AOD/165/364/085AF159-C384-E011-ACE0-001617DC1F70.root'

)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.TFileService = cms.Service(
    "TFileService", 
    fileName = cms.string("MiBiCommonNT.root"),
    closeFileFast = cms.untracked.bool(True)
)

