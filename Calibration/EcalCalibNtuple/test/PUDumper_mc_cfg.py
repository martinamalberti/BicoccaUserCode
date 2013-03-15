import FWCore.ParameterSet.Config as cms

process = cms.Process("PUDumper")


# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)
process.MessageLogger.cerr.threshold = cms.untracked.string("DEBUG")
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

# PUDumper
process.load("PhysicsTools.NtupleUtils.PUDumper_cfi")

# path
process.PUDumper_step = cms.Path(
    process.PUDumper
    )

# source                                                                                                                                                                       
process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring()
    )

process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(-1)
)

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("PUDumper.root")
    )


process.schedule = cms.Schedule(
    process.PUDumper_step
    )
