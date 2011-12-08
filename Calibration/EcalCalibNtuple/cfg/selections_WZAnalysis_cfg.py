import FWCore.ParameterSet.Config as cms



process = cms.Process("WZAnalysis")

process.Input = cms.PSet(

    inputFileList = cms.string("cfg/list.txt"),
    
    #jsonFileName = cms.string("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/Reprocessing/Cert_136033-149442_7TeV_Dec22ReReco_Collisions10_JSON_v4.txt")
    #jsonFileName = cms.string("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/DCSOnly/json_DCSONLY.txt")
    jsonFileName = cms.string("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Prompt/Cert_160404-178078_7TeV_PromptReco_Collisions11_JSON.txt"),
    inputFlag_isCalib=cms.bool(True)
    )

process.Output = cms.PSet(
    outputRootFilePath = cms.string("/data1/rgerosa/"),
    outputRootFileName = cms.string("WToENu_TuneZ2_7TeV-pythia6_Summer11")
    )

process.Options = cms.PSet(
    entryMIN = cms.int32(0),
    entryMAX = cms.int32(-1),
    entryMODULO = cms.int32(10000),
    dataFlag = cms.int32(0),
    jsonFlag = cms.int32(0),
    crossSection = cms.double(1.),
    verbosity = cms.int32(0),
    )
