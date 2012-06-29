import FWCore.ParameterSet.Config as cms

from Calibration.EcalCalibNtuple.simpleNtuple_cff import *
from Calibration.EcalCalibNtuple.recoTags_cff import *

process = cms.Process("SimpleNtuple")



# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)
process.MessageLogger.cerr.threshold = cms.untracked.string("DEBUG")
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))

ReReco=False

# simpleNtuple module call
makeSimpleNtuple(process,GlobalTag="FT_R_52_V8::All",ReReco=ReReco)
	
# Set reco laser, IC ,ADCtoGeV .... tags
#makeRecoTags(process)

#makeSqliteTags(process)

# source
process.source.fileNames = cms.untracked.vstring(
#    'file:/media/DATA/CMSSWRoot/DATA2012/Run2012B_DoubleElectron_RAW-RECO_PromptSkim-v1/B4507E93-C0A4-E111-B958-0030486790C0.root'
    'file:/media/DATA/SANDBOX/NTUPLES/3CD47EAB-7283-E111-B30B-001D09F253D4_AOD_2012.root'
    )

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(100)
)

# out
if not ReReco:
    process.TFileService = cms.Service(
        "TFileService",
        fileName = cms.string("simpleNtuple.root")
        )

    #process.out = cms.OutputModule(
    #    "PoolOutputModule",
    #    fileName = cms.untracked.string('file:SimplePATple.root'),
    #    outputCommands = cms.untracked.vstring()
    #    )

if ReReco:
    process.TFileService = cms.Service(
        "TFileService",
        fileName = cms.string("simpleNtupleReReco.root")
        )

    #process.out = cms.OutputModule(
    #    "PoolOutputModule",
    #    fileName = cms.untracked.string('simplePATpleReReco.root'),
    #    outputCommands = cms.untracked.vstring('keep *_*_*_*')
    #    )
    #process.e = cms.EndPath(process.out)



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
