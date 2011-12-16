import FWCore.ParameterSet.Config as cms



process = cms.Process("WZAnalysis")

process.Input = cms.PSet(

    inputFileList = cms.string("cfg/list.txt"),
    
    jsonFileName = cms.string("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Prompt/Cert_160404-180252_7TeV_PromptReco_Collisions11_JSON.txt"),
    inputFlag_isCalib = cms.bool(False)
    )

process.Output = cms.PSet(
    outputRootFilePath = cms.string("/data2/calibrator/NTUPLES/Run2011A/WZAnalysis/"),
    outputRootFileName = cms.string("WZAnalysis_SingleElectron_Run2011A-WElectron-May10ReReco-v1")
    )

process.Options = cms.PSet(
    entryMIN = cms.int32(0),
    entryMAX = cms.int32(-1),
    entryMODULO = cms.int32(10000),
    dataFlag = cms.int32(1),
    jsonFlag = cms.int32(0),
    crossSection = cms.double(1.),
    verbosity = cms.int32(0),
    )
