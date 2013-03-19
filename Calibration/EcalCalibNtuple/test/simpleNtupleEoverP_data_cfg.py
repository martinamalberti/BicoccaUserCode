import FWCore.ParameterSet.Config as cms

from Calibration.EcalCalibNtuple.simpleNtupleEoverP_cff import *
from Calibration.EcalCalibNtuple.recoTags_cff import *

process = cms.Process("SimpleNtupleEoverP")

# flags
GlobalTag = "FT_R_53_V18::All" #22Jan2013
#GlobalTag = "FT_R_53_V21::All" #26Feb2013
runOverSandbox   = False
runOverAlcaReco  = False
runOverData      = True
saveRecHitMatrix = True
isRecoFormat     = True

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)
process.MessageLogger.cerr.threshold = cms.untracked.string("DEBUG")
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

# simpleNtuple
makeSimpleNtuple(process,GlobalTag=GlobalTag,runOverSandbox=runOverSandbox,runOverAlcaReco=runOverAlcaReco,runOverData=runOverData)

#makeRecoTags(process)
#makeSqliteTags(process)

# path
if not saveRecHitMatrix:
    process.simpleNtuple_step = cms.Path(
        process.simpleNtupleEoverP
        )

if saveRecHitMatrix:

    if isRecoFormat:
        process.simpleNtuple_step = cms.Path(
            process.ecalDigis *
            process.simpleNtupleEoverP
            )
    
    if not isRecoFormat:
        process.simpleNtuple_step = cms.Path(
            process.ecalDigis *
            process.ecalPreshowerDigis *
            process.hcalDigis *
            process.calolocalreco *
            process.simpleNtupleEoverP
            )        
    
    process.simpleNtupleEoverP.saveFbrem        = cms.untracked.bool(True)
    process.simpleNtupleEoverP.saveRecHitMatrix = cms.untracked.bool(True)
    process.simpleNtupleEoverP.recHitCollection_EB = cms.InputTag("ecalRecHit","EcalRecHitsEB")
    process.simpleNtupleEoverP.recHitCollection_EE = cms.InputTag("ecalRecHit","EcalRecHitsEE")

# source
process.source.fileNames = cms.untracked.vstring(
    '/store/data/Run2012A/DoubleElectron/RAW-RECO/ZElectron-22Jan2013-v1/20000/E20DBB2F-C567-E211-84D7-00259059642A.root'
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
