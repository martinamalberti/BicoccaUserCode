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
ReReco=False
#makeSimpleNtuple(process,GlobalTag="GR_P_V32::All",ReReco=ReReco)
makeSimpleNtuple(process,GlobalTag="GR_R_52_V7::All",ReReco=ReReco)
#makeRecoTags(process)
#makeSqliteTags(process)



# source
process.source.fileNames = cms.untracked.vstring(
    #'rfio:/castor/cern.ch/cms/store/data/Run2012A/SingleElectron/AOD/PromptReco-v1/000/190/703/1C7611B0-6383-E111-B61A-001D09F2447F.root'
    'file:/tmp/malberti/1C7611B0-6383-E111-B61A-001D09F2447F.root'
    )

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

process.GlobalTag.toGet = cms.VPSet(
    #laser corrections
    cms.PSet(
        record = cms.string("EcalLaserAPDPNRatiosRcd"),
        tag = cms.string("EcalLaserAPDPNRatios_prompt"),
        connect =cms.untracked.string("frontier://PromptProd/CMS_COND_42X_ECAL_LAS")
        ),

    #alpha
    cms.PSet(
        record = cms.string("EcalLaserAlphasRcd"),
        tag = cms.string("EcalLaserAlphas_v2_prompt"),
        connect =cms.untracked.string("frontier://PromptProd/CMS_COND_43X_ECAL")
        ),
    
    # intercalibration constants
    cms.PSet(
        record = cms.string("EcalIntercalibConstantsRcd"),
        tag = cms.string("EcalIntercalibConstants_V1_express"),
        connect =cms.untracked.string("frontier://PromptProd/CMS_COND_31X_ECAL")
        )


    )

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
        #process.e
        )
