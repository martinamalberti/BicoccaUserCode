import FWCore.ParameterSet.Config as cms

process = cms.Process("DumpJEC")



process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "START42_V13::All"

process.source = cms.Source("EmptySource")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.load('PhysicsTools.MiBiCommonPAT.DumpJEC_cfi')
process.DumpJECPath = cms.Path(process.DumpJEC)
