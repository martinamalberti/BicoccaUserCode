import FWCore.ParameterSet.Config as cms

from PhysicsTools.MiBiCommonPAT.makeMiBiCommonPATtoNT_cff import *

process = cms.Process("MiBiCommonNT")

# the MiBiNT: Choose the MC type!!!
# Possible choices: TTBar, Higgs, HiggsWW, HiggsGammaGamma, ZW, Other
makeMiBiCommonNT(process, GlobalTag="START41_V0::All", HLT='', MC=True, MCType='Other')
  
process.source.fileNames = cms.untracked.vstring(
#   'file:/data2/amassiro/CMSSWRoot/Latinos_ggH160toWW_to2L2Nu_PAT/ggToH160toWWto2L2Nu_7_1_QhP.root'
   'file:/data2/amassiro/CMSSWRoot/Latinos_DY_PAT/DYtoMuMu_31_1_RfT.root'
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.TFileService = cms.Service(
    "TFileService", 
    fileName = cms.string("MiBiCommonNT.root"),
    closeFileFast = cms.untracked.bool(True)
)

