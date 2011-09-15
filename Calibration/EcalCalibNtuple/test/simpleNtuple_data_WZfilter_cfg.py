import FWCore.ParameterSet.Config as cms

from Calibration.EcalCalibNtuple.simpleNtuple_cff import *
from Calibration.EcalCalibNtuple.recoTags_cff import *

process = cms.Process("NOLC")
#process = cms.Process("SimpleNtuple")



# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1)
process.MessageLogger.cerr.threshold = cms.untracked.string("DEBUG")
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))



# simpleNtuple
ReReco=True
makeSimpleNtuple(process,GlobalTag="GR_R_42_V20::All",ReReco=ReReco)

makeRecoTags(process)  #commented for 05Jul ReReco
#makeSqliteTags(process)



# source
process.source.fileNames = cms.untracked.vstring(
    #Run2011A - /SingleElectron/Run2011A-PromptReco-v1/*
    #'file:/data_CMS/cms/abenagli/DATA_SingleElectron_412_RAW.root'
    #'file:/data_CMS/cms/abenagli/DATA_SingleElectron_412_RECO.root'
    #'file:/data_CMS/cms/abenagli/DATA_SingleElectron_412_AOD.root'
    #'/store/data/Run2011A/SingleElectron/AOD/May3ReReco-v1/0000/E8B7D65A-E075-E011-982B-00261894394D.root'
    #'file:/data2/Datasets/DATA_SingleElectron_Run2011A-WElectron-May10ReReco_RAW-RECO.root'
    'file:/data1/abenagli/DATA_SingleElectron_Run2011A-WElectron-May10ReReco_RAW-RECO.root'
    #'file:/tmp/deguio/provaRERECO.root'
    )

process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(1000)
)

# out : commenting the output ntuple since we save data in edm format
#if not ReReco:
    #process.TFileService = cms.Service(
        #"TFileService",
        #fileName = cms.string("simpleNtuple.root")
        #)

#if ReReco:
    #process.TFileService = cms.Service(
        #"TFileService",
        #fileName = cms.string("simpleNtupleReReco.root")
        #)



#============ WZfilter ============
process.load("DPGAnalysis/Skims/WZinterestingEventFilter_cfi") 

# Take WP 80 cuts from here:https://twiki.cern.ch/twiki/bin/view/CMS/SimpleCutBasedEleID
process.WZInterestingEventSelector.missHitsCut = cms.int32(0)
process.WZInterestingEventSelector.eb_trIsoCut = cms.double(0.09)
process.WZInterestingEventSelector.eb_ecalIsoCut = cms.double(0.07)
process.WZInterestingEventSelector.eb_hcalIsoCut = cms.double(0.1)
process.WZInterestingEventSelector.eb_hoeCut = cms.double(0.04)
process.WZInterestingEventSelector.eb_seeCut = cms.double(0.01)
process.WZInterestingEventSelector.ee_trIsoCut = cms.double(0.04)
process.WZInterestingEventSelector.ee_ecalIsoCut = cms.double(0.05)
process.WZInterestingEventSelector.ee_hcalIsoCut = cms.double(0.025)
process.WZInterestingEventSelector.ee_hoeCut = cms.double(0.025)
process.WZInterestingEventSelector.ee_seeCut = cms.double(0.03)
process.WZInterestingEventSelector.metCut = cms.double(25.)
process.WZInterestingEventSelector.invMassCut = cms.double(70.)
process.WZfilter = cms.Path(process.WZInterestingEventSelector)
#============ WZfilter END============
process.load("Configuration/EventContent/EventContent_cff")

process.out = cms.OutputModule(
    "PoolOutputModule",
    outputCommands = process.RECOEventContent.outputCommands,
    
#    outputCommands = cms.untracked.vstring('drop *_*_*_*',
#                                           'keep *_*_*_RERECO',
#                                           'keep *_*_*_HLT',
#                                           'drop FEDRawDataCollection_*_*_HLT',
#                                           'drop *_*pat*_*_*',
#                                           'drop *_*selectedPat*_*_*'
#                                           ),
                                           
    fileName =  cms.untracked.string('recoNoLc.root'),
    )

process.out.outputCommands.extend(['drop *_*_*_RECO'])
process.e = cms.EndPath(process.out)


 
# schedule
if not ReReco:
    process.schedule = cms.Schedule(
        process.WZfilter,
        #process.simpleNtuple_step
        )

if ReReco:
    process.schedule = cms.Schedule(
        process.WZfilter,
        process.raw2digi_step,
        process.L1Reco_step,
        process.reconstruction_step,
        process.endjob_step,
        #process.simpleNtuple_step,
        process.e
        )


