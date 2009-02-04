#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

process = cms.Process("VBFHWW2l2nuSkim")



process.load('Configuration/StandardSequences/Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFHWW2l2nuOutputModule_cff")



# Source
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.source = cms.Source(
    "PoolSource",
    debugFlag = cms.untracked.bool(True),
    debugVebosity = cms.untracked.uint32(1),
    fileNames = cms.untracked.vstring(

        'rfio:/castor/cern.ch/cms/store/path/filename.root'
        )
    )




# Output
process.o = cms.EndPath ( process.VBFHWW2l2nuOutputModule )
