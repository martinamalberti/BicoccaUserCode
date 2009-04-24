#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

process = cms.Process("VBFMCDumper")

process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFMCDumper_cfi")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.load('Configuration/StandardSequences/Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'






# Source
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(5000))

process.source = cms.Source(
    "PoolSource",
    debugFlag = cms.untracked.bool(True),
    debugVebosity = cms.untracked.uint32(1),
    fileNames = cms.untracked.vstring(
        'file:/tmp/abenagli/VBFHWW2l2nuTest_1.root'
        #'file:/tmp/abenagli/VBFHWW2l2nuTest_11.root',
        #'file:/tmp/abenagli/VBFHWW2l2nuTest_13.root',
        #'file:/tmp/abenagli/VBFHWW2l2nuTest_15.root'
       	)
    )






# Dump MC information
process.VBFMCDumper.fileName = "VBFMCDumper"
process.VBFMCDumper.mass = "500"
process.VBFMCDumper.verbosity = False
process.VBFMCDumper.eventsToPrint = 0

process.p0 = cms.Path(process.VBFMCDumper)






process.schedule = cms.Schedule(process.p0)
