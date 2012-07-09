import FWCore.ParameterSet.Config as cms

from Calibration.EcalCalibNtuple.simpleNtupleEoverP_cff import *
from Calibration.EcalCalibNtuple.recoTags_cff import *

process = cms.Process("SimpleNtupleEoverP")


# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)
process.MessageLogger.cerr.threshold = cms.untracked.string("DEBUG")
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))


# simpleNtuple
makeSimpleNtuple(process,GlobalTag="GR_R_42_V24::All")

makeRecoTags(process)  #commented for 05Jul ReReco
#makeSqliteTags(process)

process.simpleNtupleEoverP.dataFlag = cms.untracked.bool(isDATA);
process.simpleNtupleEoverP.dataRun = cms.string("dataRUN");



# source
process.source.fileNames = cms.untracked.vstring(
    LISTOFFILES
    )

process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(-1)
)

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("OUTPUTFILENAME.root")
    )


process.schedule = cms.Schedule(
    process.simpleNtuple_step
    )
