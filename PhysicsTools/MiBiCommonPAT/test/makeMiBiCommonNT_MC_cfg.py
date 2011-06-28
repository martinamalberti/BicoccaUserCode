import FWCore.ParameterSet.Config as cms

from PhysicsTools.MiBiCommonPAT.makeMiBiCommonNT_cff import *

process = cms.Process("MiBiCommonNT")

# the MiBiNT: Choose the MC type!!!
# Possible choices: TTBar, Higgs, HiggsWW, HiggsGammaGamma, ZW, Other
makeMiBiCommonNT(process, GlobalTag="START42_V12::All", HLT='', MC=True, MCType='Other')
  
process.source.fileNames = cms.untracked.vstring(
    'file:/data2/amassiro/CMSSWRoot/WZTo3LNu_TuneZ2_7TeV_pythia6_tauola_U_S4_START42_V11-v1_AODSIM.root'
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.TFileService = cms.Service(
    "TFileService", 
    fileName = cms.string("MiBiCommonNT.root"),
    closeFileFast = cms.untracked.bool(True)
)

