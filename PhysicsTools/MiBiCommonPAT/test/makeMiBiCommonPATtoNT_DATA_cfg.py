import FWCore.ParameterSet.Config as cms

from PhysicsTools.MiBiCommonPAT.makeMiBiCommonPATtoNT_cff import *

process = cms.Process("MiBiCommonNT")

# the MiBiNT: Choose the MC type!!!
# Possible choices: TTBar, Higgs, HiggsWW, HiggsGammaGamma, ZW, Other
#  makeMiBiCommonNT(process, GlobalTag="GR_R_42_V12::All", HLT='', MC=False, MCType='Other')
makeMiBiCommonNT(process, GlobalTag="GR_R_42_V19::All", HLT='', MC=False, MCType='Other')
#  makeMiBiCommonNT(process, GlobalTag="GR_R_41_V0::All", HLT='', MC=False, MCType='Other')  

process.source.fileNames = cms.untracked.vstring(
#   'file:/data2/amassiro/CMSSWRoot/Latinos_DATA_PAT/DoubleMuon2011A_27_1_GPa.root'
   'file:/data2/amassiro/CMSSWRoot/Latinos_DATA_PAT/DoubleElectron2011AReRecoMay10_80_1_40B.root'
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.TFileService = cms.Service(
    "TFileService", 
    fileName = cms.string("MiBiCommonNT.root"),
    closeFileFast = cms.untracked.bool(True)
)

