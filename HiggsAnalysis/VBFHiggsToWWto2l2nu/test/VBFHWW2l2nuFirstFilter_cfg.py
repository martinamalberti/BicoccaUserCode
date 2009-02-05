#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

process = cms.Process("VBFHWW2l2nuFirstFilter")



process.load('Configuration/StandardSequences/Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'


process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFPreselectionSequence_cff")


# Source
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.source = cms.Source(
    "PoolSource",
    debugFlag = cms.untracked.bool(True),
    debugVebosity = cms.untracked.uint32(1),
    fileNames = cms.untracked.vstring(
        'file:/tmp/amassiro/VBFHWW2l2nuTest_21.root'
#         'rfio:/castor/cern.ch/cms/store/path/filename.root'
        )
    )
    
process.out = cms.OutputModule("PoolOutputModule",
                               verbose = cms.untracked.bool(False),
                               fileName = cms.untracked.string('/tmp/amassiro/FirstFilter.root'),
                               outputCommands = cms.untracked.vstring('drop *',
                                                                      'keep *_muons_*_*',
                                                                      'keep *_globalMuons_*_*',
                                                                      'keep *_pixelMatchGsfElectrons_*_*',
                                                                      'keep *_isolatedMuons_*_*', 
                                                                      'keep *_selectedMuons_*_*'
                                                                      )
                               )



process.p = cms.Path ( process.VBFPreselectionSequence)

# Output
process.o = cms.EndPath ( process.out )
