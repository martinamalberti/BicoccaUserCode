import FWCore.ParameterSet.Config as cms



process = cms.Process("new_WZAnalysis")

process.Input = cms.PSet(

    inputFileList = cms.string("cfg/list.txt"),
    
    #jsonFileName = cms.string("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/Reprocessing/Cert_136033-149442_7TeV_Dec22ReReco_Collisions10_JSON_v4.txt")
    #jsonFileName = cms.string("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/DCSOnly/json_DCSONLY.txt")
    jsonFileName = cms.string("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Prompt/Cert_160404-178078_7TeV_PromptReco_Collisions11_JSON.txt"),
    inputFlag_isCalib=cms.bool(True)
    )

process.Output = cms.PSet(
    outputRootFilePath = cms.string("/data2/calibrator/NTUPLES/Run2011A/WZAnalysis/"),
    outputRootFileName = cms.string("WZAnalysis_SingleElectron_Run2011A-05Jul2011ReReco-ECAL-v1_new")
    )

process.Options = cms.PSet(
    entryMIN = cms.int32(0),
    entryMAX = cms.int32(100000),
    entryMODULO = cms.int32(1000),
    dataFlag = cms.int32(1),
    jsonFlag = cms.int32(1),
    crossSection = cms.double(1.),
    verbosity = cms.int32(0),
    )
