import FWCore.ParameterSet.Config as cms

from PhysicsTools.MiBiCommonPAT.makeMiBiCommonPAT_cff import *



process = cms.Process("MiBiCommonPAT")



makeMiBiCommonPAT(process, GlobalTag="GR_R_38X_V15::All", MC=False, Filter=True, SavePAT=True)

process.source.fileNames = cms.untracked.vstring(
  'file:/tmp/dimatteo/D8700674-35CB-DF11-8C60-0024E85A3ED8.root'
  #'file:/gwpool/users/amassiro/VBF/Releases/CMSSW_3_8_3/src/PhysicsTools/MiBiCommonPAT/669A4128-43D0-DF11-AE93-001A92810ACE.root'  

)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))

process.out.fileName = cms.untracked.string("file:/tmp/dimatteo/MiBiCommonPAT.root")


