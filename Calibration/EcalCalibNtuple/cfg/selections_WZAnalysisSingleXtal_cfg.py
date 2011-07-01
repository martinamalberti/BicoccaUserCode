import FWCore.ParameterSet.Config as cms



process = cms.Process("WZAnalysisSingleXtal")

process.Input = cms.PSet(

    inputFileList = cms.string("cfg/list.txt"),
    
    #jsonFileName = cms.string("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/Reprocessing/Cert_136033-149442_7TeV_Dec22ReReco_Collisions10_JSON_v4.txt")
    #jsonFileName = cms.string("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Prompt/Cert_160404-163869_7TeV_PromptReco_Collisions11_JSON.txt")
    jsonFileName = cms.string("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Reprocessing/Cert_160404-163869_7TeV_May10ReReco_Collisions11_JSON.txt")
    #jsonFileName = cms.string("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/DCSOnly/json_DCSONLY.txt")
    )

process.Output = cms.PSet(
    outputRootFilePath = cms.string("/data1/dimatteo/Calibration/Ntuples/Run2011A/WZAnalysisSingleXtal/"),

    #outputRootFileName = cms.string("WZAnalysis_EGElectron_Run2010AB-Apr21ReReco-v1")
    #outputRootFileName = cms.string("WZAnalysis_SingleElectron_Run2011A-May10ReReco-v1_160329-163869")
    outputRootFileName = cms.string("WZAnalysis_SingleElectron_Run2011A-WElectron-May10ReReco-v1")
    #outputRootFileName = cms.string("WZAnalysis_SingleElectron_Run2011A-PromptReco-v4_165071-165980")
    )

process.Options = cms.PSet(
    entryMIN = cms.int32(0),
    entryMAX = cms.int32(-1),
    entryMODULO = cms.int32(100000),
    jsonFlag = cms.int32(1),
    verbosity = cms.int32(0),
    )
