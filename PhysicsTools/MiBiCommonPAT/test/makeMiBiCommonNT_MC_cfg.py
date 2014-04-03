import FWCore.ParameterSet.Config as cms

from PhysicsTools.MiBiCommonPAT.makeMiBiCommonNT_cff import *

process = cms.Process("MiBiCommonNT")

# the MiBiNT: Choose the MC type!!!
# Possible choices: TTBar, Higgs, HiggsWW, HiggsGammaGamma, ZW, Other
#makeMiBiCommonNT(process, GlobalTag="START53_V27::All", HLT='', MC=True, MCType='ZW') # MC 2012
#makeMiBiCommonNT(process, GlobalTag="START53_V7N::All", HLT='', MC=True, MCType='ZW') # run dep MC 2012
#makeMiBiCommonNT(process, GlobalTag="FT_R_53_LV3::All", HLT='', MC=True, MCType='ZW') # MC 2011
makeMiBiCommonNT(process, GlobalTag="POSTLS170_V3::All", HLT='', MC=True, MCType='ZW') # 70X mc

process.source.fileNames = cms.untracked.vstring(
    #'file:/data2/amassiro/CMSSWRoot/Summer12/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_AODSIM/AODSIM.root'
    '/store/relval/CMSSW_7_0_0_pre13/RelValZMM_13/GEN-SIM-RECO/POSTLS170_V3-v1/00000/BA62F041-F08F-E311-B52E-0025905A60DE.root',
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(20) )
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.TFileService = cms.Service(
    "TFileService", 
    fileName = cms.string("MiBiCommonNT.root"),
    closeFileFast = cms.untracked.bool(True)
)
