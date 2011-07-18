import FWCore.ParameterSet.Config as cms



process = cms.Process("WZAnalysis")

process.Input = cms.PSet(

    inputFileList = cms.string("LISTFILES"),
    jsonFileName = cms.string("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/DCSOnly/json_DCSONLY.txt")
    )

process.Output = cms.PSet(
    outputRootFilePath = cms.string("OUTPUTSAVEPATH/OUTPUTSAVEFOLDER"),
    outputRootFileName = cms.string("OUTPUTFILENAME_runRUN_newSelection")
    )

process.Options = cms.PSet(
    entryMIN = cms.int32(0),
    entryMAX = cms.int32(-1),
    entryMODULO = cms.int32(100000),
    dataFlag = cms.int32(1),
    jsonFlag = cms.int32(1),
    crossSection = cms.double(1.),
    verbosity = cms.int32(0),
    )
