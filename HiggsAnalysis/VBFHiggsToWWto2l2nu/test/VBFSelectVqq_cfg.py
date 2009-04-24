#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

process = cms.Process("VBFSelectVqq")

process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFSelectVqq_cfi")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.load('Configuration/StandardSequences/Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'






# Source
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))

process.source = cms.Source(
    "PoolSource",
    debugFlag = cms.untracked.bool(True),
    debugVebosity = cms.untracked.uint32(1),
    fileNames = cms.untracked.vstring(
        'file:/tmp/abenagli/VBFHWW2l2nuTest_1.root',
        'file:/tmp/abenagli/VBFHWW2l2nuTest_2.root',
        'file:/tmp/abenagli/VBFHWW2l2nuTest_3.root',
        'file:/tmp/abenagli/VBFHWW2l2nuTest_4.root'
       	)
    )






# Dump MC information
process.VBFSelectVqq.fileName = "VBFSelectVqq"
process.VBFSelectVqq.mass = "500"
process.VBFSelectVqq.verbosity = False
process.VBFSelectVqq.eventsToPrint = 0

process.p0 = cms.Path(process.VBFSelectVqq)






process.schedule = cms.Schedule(process.p0)
