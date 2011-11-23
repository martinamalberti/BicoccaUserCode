import FWCore.ParameterSet.Config as cms

from PhysicsTools.MiBiCommonPAT.makeMiBiCommonNT_noPAT_cff import *

process = cms.Process("MiBiCommonNT")

# the MiBiNT: Choose the MC type!!!
# Possible choices: TTBar, Higgs, HiggsWW, HiggsGammaGamma, ZW, Other
makeMiBiCommonNT_noPAT(process, GlobalTag="START42_V13::All", HLT='', MC=True, MCType='Other')

process.source.fileNames = cms.untracked.vstring(
    #'file:/data1/abenagli/MC_WW_TuneZ2_7TeV_pythia6_tauola_yumiceva_PAT.root'
    #'file:/data1/abenagli/MC_WW_TuneZ2_7TeV_pythia6_tauola_uvnd_PAT.root'
    #'file:/data1/abenagli/MC_ggHWWlvjj_TuneZ2_7TeV_pythia6_tauola_common_PAT.root'
    'file:/data1/abenagli/MC_ggHWWlvjj_TuneZ2_7TeV_pythia6_tauola_yumicevanew_PAT.root'
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.TFileService = cms.Service(
    "TFileService", 
    fileName = cms.string("MiBiCommonNT.root"),
    closeFileFast = cms.untracked.bool(True)
)
