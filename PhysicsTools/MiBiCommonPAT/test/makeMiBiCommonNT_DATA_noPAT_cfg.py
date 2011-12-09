import FWCore.ParameterSet.Config as cms

from PhysicsTools.MiBiCommonPAT.makeMiBiCommonNT_noPAT_cff import *

process = cms.Process("MiBiCommonNT")

# the MiBiNT: Choose the MC type!!!
# Possible choices: TTBar, Higgs, HiggsWW, HiggsGammaGamma, ZW, Other
makeMiBiCommonNT_noPAT(process, GlobalTag="GR_R_42_V19::All", HLT='', MC=False, MCType='Other')

process.source.fileNames = cms.untracked.vstring(
    #'file:/gwteraz/users/benaglia/pat_42x_fall11_SQWaT_PAT_42X_2011A_May10ReReco-v1_v1.root'
    'file:/gwteraz/users/benaglia/pat_42x_fall11_SQWaT_PAT_42X_2011A-PromptReco-v6_v2.root'
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.TFileService = cms.Service(
    "TFileService", 
    fileName = cms.string("MiBiCommonNT.root"),
    closeFileFast = cms.untracked.bool(True)
)
