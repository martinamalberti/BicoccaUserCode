
import FWCore.ParameterSet.Config as cms

from PhysicsTools.MiBiCommonPAT.makeMiBiCommonNT_cff import *

process = cms.Process("MiBiCommonNT")

# the MiBiNT
makeMiBiCommonNT(process, GlobalTag="GR_R_52_V7::All", HLT='', MC=False, MCType='Other')
#makeMiBiCommonNT(process, GlobalTag="GR_P_V32::All", HLT='', MC=False, MCType='Other')
  
process.source.fileNames = cms.untracked.vstring(
     '/store/data/Run2012A/MuEG/RECO/PromptReco-v1/000/190/702/8EAD0A55-5383-E111-BEA9-001D09F295A1.root'
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))

process.TFileService = cms.Service(
    "TFileService", 
    fileName = cms.string("MiBiCommonNT.root"),
    closeFileFast = cms.untracked.bool(True)
)

