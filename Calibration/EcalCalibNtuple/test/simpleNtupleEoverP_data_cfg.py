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
#makeSimpleNtuple(process,GlobalTag="GR_R_42_V21::All")
#makeSimpleNtuple(process,GlobalTag="GR_R_42_V20::All")
makeSimpleNtuple(process,GlobalTag="GR_P_V22::All")

#makeRecoTags(process)  #commented for 05Jul ReReco
#makeSqliteTags(process)



# source
process.source.fileNames = cms.untracked.vstring(
    'file:/tmp/dimatteo/ALCARECO_meridian.root'
    )

process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(-1)
)

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("simpleNtuple.root")
    )


process.schedule = cms.Schedule(
    process.simpleNtuple_step
    )