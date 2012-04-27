import FWCore.ParameterSet.Config as cms

from PhysicsTools.MiBiCommonPAT.makeMiBiCommonNT_cff import *

process = cms.Process("MiBiCommonNT")

# the MiBiNT: Choose the MC type!!!
# Possible choices: TTBar, Higgs, HiggsWW, HiggsGammaGamma, ZW, Other
makeMiBiCommonNT(process, GlobalTag="START52_V9::All", HLT='', MC=True, MCType='ZW')
  
process.source.fileNames = cms.untracked.vstring(
    #'file:/data2/amassiro/CMSSWRoot/Summer12/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_AODSIM/AODSIM.root'
     'file:/tmp/malberti/AODSIM.root'
        )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))

process.TFileService = cms.Service(
    "TFileService", 
    fileName = cms.string("MiBiCommonNT.root"),
    closeFileFast = cms.untracked.bool(True)
)
