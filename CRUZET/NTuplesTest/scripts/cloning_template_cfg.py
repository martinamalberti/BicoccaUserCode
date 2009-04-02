import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")
process.inputNtuples = cms.PSet(
    maxEvents = cms.int32(-1),
    inputFiles = cms.vstring(
    LISTOFFILES
    )
)
process.outputNtuple = cms.PSet(
    ntupleName = cms.string('/tmp/cloned_RUNNUMBER.root')
)

