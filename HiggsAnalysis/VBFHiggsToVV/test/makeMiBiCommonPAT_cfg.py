import FWCore.ParameterSet.Config as cms

process = cms.Process("MiBiCommonPAT")

from HiggsAnalysis.VBFHiggsToVV.makeMiBiCommonPAT_cff import *

makeMiBiCommonPAT(process, GlobalTag="START38_V12::All", MC=True, HLT="REDIGI38X", Filter=True, SavePAT=True)

process.source.fileNames = cms.untracked.vstring(
  'file:/tmp/dimatteo/Fall10_WToENu_TuneZ2_7TeV-pythia6_AODSIM_START38_V12-v1.root'
)

process.TFileService = cms.Service("TFileService", 
      fileName = cms.string("Counters.root"),
      closeFileFast = cms.untracked.bool(True)
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

