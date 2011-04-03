import FWCore.ParameterSet.Config as cms

from Calibration.EcalCalibNtuple.simpleNtuple_cff import *
from Calibration.EcalCalibNtuple.recoTags_cff import *

process = cms.Process("SimpleNtuple")



# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1)
process.MessageLogger.cerr.threshold = cms.untracked.string("DEBUG")
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))



# simpleNtuple
ReReco=True
makeSimpleNtuple(process,GlobalTag="GR_R_311_V2::All",ReReco=ReReco)
makeRecoTags(process)



# source
process.source.fileNames = cms.untracked.vstring(
    #Run2011A - /SingleElectron/Run2011A-PromptReco-v1/RECO
    'file:/data_CMS/cms/abenagli/DATA_SingleElectron_412_RECO.root'
    )

process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(1000)
)

# out
if not ReReco:
    process.TFileService = cms.Service(
        "TFileService",
        fileName = cms.string("simpleNtuple.root")
        )

    process.out = cms.OutputModule(
        "PoolOutputModule",
        fileName = cms.untracked.string('file:SimplePATple.root'),
        outputCommands = cms.untracked.vstring()
        )

if ReReco:
    process.TFileService = cms.Service(
        "TFileService",
        fileName = cms.string("simpleNtupleReReco.root")
        )

    process.out = cms.OutputModule(
        "PoolOutputModule",
        fileName = cms.untracked.string('file:SimplePATpleReReco.root'),
        outputCommands = cms.untracked.vstring()
        )



# schedule

if not ReReco:
    process.schedule = cms.Schedule(
        process.simpleNtuple_step
        )

if ReReco:
    process.schedule = cms.Schedule(
        process.raw2digi_step,
        process.L1Reco_step,
        process.reconstruction_step,
        process.endjob_step,
        process.simpleNtuple_step
        )
