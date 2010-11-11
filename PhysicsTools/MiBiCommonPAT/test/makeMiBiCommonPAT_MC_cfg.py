import FWCore.ParameterSet.Config as cms

from PhysicsTools.MiBiCommonPAT.makeMiBiCommonPAT_cff import *



process = cms.Process("MiBiCommonPAT")



makeMiBiCommonPAT(process, GlobalTag="START38_V12::All", MC=True, Filter=True, SavePAT=True)

process.source.fileNames = cms.untracked.vstring(
  'file:/tmp/dimatteo/963B8D02-12E7-DF11-A8E0-003048D3E452.root'
  #'file:/gwpool/users/amassiro/VBF/Releases/CMSSW_3_8_3/src/PhysicsTools/MiBiCommonPAT/669A4128-43D0-DF11-AE93-001A92810ACE.root'  

)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))

process.out.fileName = cms.untracked.string("file:/tmp/dimatteo/MiBiCommonPAT.root")


