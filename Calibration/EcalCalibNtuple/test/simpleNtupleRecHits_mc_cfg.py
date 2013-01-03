import FWCore.ParameterSet.Config as cms

from Calibration.EcalCalibNtuple.simpleNtupleRecHits_cff import *

process = cms.Process("SimpleNtupleRecHits")


# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)
process.MessageLogger.cerr.threshold = cms.untracked.string("DEBUG")
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

# simpleNtuple
makeSimpleNtupleRecHits(process,GlobalTag="START53_V11::All",runOverData=False)

# path
process.simpleNtupleRecHits_step = cms.Path(
    process.simpleNtupleRecHits
    )

# source
process.source.fileNames = cms.untracked.vstring(
    '/store/group/alca_ecalcalib/meridian/Prod/SingleNuE10-PU_S10_START53_V7A_ECALNZS/meridian/SingleNuE10-PU_S10_START53_V7A_ECALNZS/SingleNuE10-PU_S10_START53_V7A_ECALNZS/4ff8655539646d574110ad5f71f67bac/SingleNuE10_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_RAW2DIGI_L1Reco_RECO_PU_1000_1_S7S.root',
    '/store/group/alca_ecalcalib/meridian/Prod/SingleNuE10-PU_S10_START53_V7A_ECALNZS/meridian/SingleNuE10-PU_S10_START53_V7A_ECALNZS/SingleNuE10-PU_S10_START53_V7A_ECALNZS/4ff8655539646d574110ad5f71f67bac/SingleNuE10_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_RAW2DIGI_L1Reco_RECO_PU_1000_1_S7S.root',
    '/store/group/alca_ecalcalib/meridian/Prod/SingleNuE10-PU_S10_START53_V7A_ECALNZS/meridian/SingleNuE10-PU_S10_START53_V7A_ECALNZS/SingleNuE10-PU_S10_START53_V7A_ECALNZS/4ff8655539646d574110ad5f71f67bac/SingleNuE10_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_RAW2DIGI_L1Reco_RECO_PU_1001_1_FOz.root',
    '/store/group/alca_ecalcalib/meridian/Prod/SingleNuE10-PU_S10_START53_V7A_ECALNZS/meridian/SingleNuE10-PU_S10_START53_V7A_ECALNZS/SingleNuE10-PU_S10_START53_V7A_ECALNZS/4ff8655539646d574110ad5f71f67bac/SingleNuE10_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_RAW2DIGI_L1Reco_RECO_PU_1002_1_V4n.root',
    '/store/group/alca_ecalcalib/meridian/Prod/SingleNuE10-PU_S10_START53_V7A_ECALNZS/meridian/SingleNuE10-PU_S10_START53_V7A_ECALNZS/SingleNuE10-PU_S10_START53_V7A_ECALNZS/4ff8655539646d574110ad5f71f67bac/SingleNuE10_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_RAW2DIGI_L1Reco_RECO_PU_1003_1_zZk.root',
    '/store/group/alca_ecalcalib/meridian/Prod/SingleNuE10-PU_S10_START53_V7A_ECALNZS/meridian/SingleNuE10-PU_S10_START53_V7A_ECALNZS/SingleNuE10-PU_S10_START53_V7A_ECALNZS/4ff8655539646d574110ad5f71f67bac/SingleNuE10_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_RAW2DIGI_L1Reco_RECO_PU_1004_1_hqb.root',
    '/store/group/alca_ecalcalib/meridian/Prod/SingleNuE10-PU_S10_START53_V7A_ECALNZS/meridian/SingleNuE10-PU_S10_START53_V7A_ECALNZS/SingleNuE10-PU_S10_START53_V7A_ECALNZS/4ff8655539646d574110ad5f71f67bac/SingleNuE10_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_RAW2DIGI_L1Reco_RECO_PU_1005_1_DL1.root',
    '/store/group/alca_ecalcalib/meridian/Prod/SingleNuE10-PU_S10_START53_V7A_ECALNZS/meridian/SingleNuE10-PU_S10_START53_V7A_ECALNZS/SingleNuE10-PU_S10_START53_V7A_ECALNZS/4ff8655539646d574110ad5f71f67bac/SingleNuE10_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_RAW2DIGI_L1Reco_RECO_PU_1006_1_r9h.root',
    '/store/group/alca_ecalcalib/meridian/Prod/SingleNuE10-PU_S10_START53_V7A_ECALNZS/meridian/SingleNuE10-PU_S10_START53_V7A_ECALNZS/SingleNuE10-PU_S10_START53_V7A_ECALNZS/4ff8655539646d574110ad5f71f67bac/SingleNuE10_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_RAW2DIGI_L1Reco_RECO_PU_1007_1_i5g.root',
    '/store/group/alca_ecalcalib/meridian/Prod/SingleNuE10-PU_S10_START53_V7A_ECALNZS/meridian/SingleNuE10-PU_S10_START53_V7A_ECALNZS/SingleNuE10-PU_S10_START53_V7A_ECALNZS/4ff8655539646d574110ad5f71f67bac/SingleNuE10_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_RAW2DIGI_L1Reco_RECO_PU_1008_1_YeO.root',
    '/store/group/alca_ecalcalib/meridian/Prod/SingleNuE10-PU_S10_START53_V7A_ECALNZS/meridian/SingleNuE10-PU_S10_START53_V7A_ECALNZS/SingleNuE10-PU_S10_START53_V7A_ECALNZS/4ff8655539646d574110ad5f71f67bac/SingleNuE10_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_RAW2DIGI_L1Reco_RECO_PU_1009_1_HP8.root'
        '/store/group/alca_ecalcalib/meridian/Prod/SingleNuE10-PU_S10_START53_V7A_ECALNZS/meridian/SingleNuE10-PU_S10_START53_V7A_ECALNZS/SingleNuE10-PU_S10_START53_V7A_ECALNZS/4ff8655539646d574110ad5f71f67bac/SingleNuE10_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_RAW2DIGI_L1Reco_RECO_PU_1010_1_50L.root'
    )

process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(-1)
)

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("/tmp/abenagli/simpleNtuple.root")
    )


process.schedule = cms.Schedule(
    process.simpleNtupleRecHits_step
    )
