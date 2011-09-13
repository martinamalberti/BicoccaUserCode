import FWCore.ParameterSet.Config as cms

process = cms.Process("WZAnalysisNoLC")

process.Input = cms.PSet(

    inputFileList = cms.string("cfg/list.txt"),
    jsonFileName = cms.string("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Prompt/Cert_160404-173692_7TeV_PromptReco_Collisions11_JSON.txt")
    )

process.Output = cms.PSet(
    outputRootFilePath = cms.string("/data1/dimatteo/Calibration/Ntuples/Run2011A/WZAnalysisNoLC/"),
    outputRootFileName = cms.string("WZAnalysis_SingleElectron_Run2011A-WElectron-PromptReco-v6")
    #outputRootFileName = cms.string("TEST")
    )
process.Options = cms.PSet(
    entryMIN = cms.int32(0),
    entryMAX = cms.int32(-1),
    entryMODULO = cms.int32(100000),
    jsonFlag = cms.int32(1),
    verbosity = cms.int32(0),
    )
