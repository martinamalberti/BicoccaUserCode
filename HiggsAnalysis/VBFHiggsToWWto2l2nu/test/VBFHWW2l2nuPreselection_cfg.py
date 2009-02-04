#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

process = cms.Process("VBFHWW2l2nuPreselection")



process.load('Configuration/StandardSequences/Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFPreselectionSequence_cff")
process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFOutputModule_cff")



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



# Do the preselection
process.p = cms.Path ( process.VBFPreselectionSequence )




# Output
process.o = cms.EndPath ( process.VBFOutputModule )
