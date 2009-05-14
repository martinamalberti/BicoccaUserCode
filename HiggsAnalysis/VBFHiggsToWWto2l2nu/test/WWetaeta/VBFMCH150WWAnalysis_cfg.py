#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

process = cms.Process("VBFMCttbarAnalysis")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.load('Configuration/StandardSequences/Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'






# Source
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.source = cms.Source(
    "PoolSource",
    debugFlag = cms.untracked.bool(True),
    debugVebosity = cms.untracked.uint32(1),
    fileNames = cms.untracked.vstring(
       'rfio:/castor/cern.ch/user/a/amassiro/VBF/090430_Everything_Skimmed_4Cluster/H150_WW_2l_Summer08_IDEAL_V11_redigi_v1/VBFHWW2l2nuTest_1.root',
       'rfio:/castor/cern.ch/user/a/amassiro/VBF/090430_Everything_Skimmed_4Cluster/H150_WW_2l_Summer08_IDEAL_V11_redigi_v1/VBFHWW2l2nuTest_2.root',
       'rfio:/castor/cern.ch/user/a/amassiro/VBF/090430_Everything_Skimmed_4Cluster/H150_WW_2l_Summer08_IDEAL_V11_redigi_v1/VBFHWW2l2nuTest_3.root',
       'rfio:/castor/cern.ch/user/a/amassiro/VBF/090430_Everything_Skimmed_4Cluster/H150_WW_2l_Summer08_IDEAL_V11_redigi_v1/VBFHWW2l2nuTest_4.root',
       'rfio:/castor/cern.ch/user/a/amassiro/VBF/090430_Everything_Skimmed_4Cluster/H150_WW_2l_Summer08_IDEAL_V11_redigi_v1/VBFHWW2l2nuTest_5.root',
       'rfio:/castor/cern.ch/user/a/amassiro/VBF/090430_Everything_Skimmed_4Cluster/H150_WW_2l_Summer08_IDEAL_V11_redigi_v1/VBFHWW2l2nuTest_6.root',
       'rfio:/castor/cern.ch/user/a/amassiro/VBF/090430_Everything_Skimmed_4Cluster/H150_WW_2l_Summer08_IDEAL_V11_redigi_v1/VBFHWW2l2nuTest_7.root',
       'rfio:/castor/cern.ch/user/a/amassiro/VBF/090430_Everything_Skimmed_4Cluster/H150_WW_2l_Summer08_IDEAL_V11_redigi_v1/VBFHWW2l2nuTest_8.root',
       'rfio:/castor/cern.ch/user/a/amassiro/VBF/090430_Everything_Skimmed_4Cluster/H150_WW_2l_Summer08_IDEAL_V11_redigi_v1/VBFHWW2l2nuTest_9.root',
       'rfio:/castor/cern.ch/user/a/amassiro/VBF/090430_Everything_Skimmed_4Cluster/H150_WW_2l_Summer08_IDEAL_V11_redigi_v1/VBFHWW2l2nuTest_10.root'
       )
    )




# Dump MC information


process.VBFMCttbarAnalysis = cms.EDAnalyzer(
    "VBFMCttbarAnalysis",
    srcHepMCProduct = cms.InputTag("source"),
    srcGenParticles = cms.InputTag("genParticles"),
    srcGenMet = cms.InputTag("genMetNoNuBSM"),
    srcIC5GenJets = cms.InputTag("iterativeCone5GenJets"),
    srcIC5CaloJets = cms.InputTag("iterativeCone5CaloJets"),
    fileName = cms.untracked.string("VBFMCDump"),
    mass = cms.string("300"),
    verbosity = cms.untracked.bool(True),
    eventsToPrint = cms.untracked.int32(1)
    )


process.VBFMCttbarAnalysis.fileName = "VBFMCDumper_H150WW"
process.VBFMCttbarAnalysis.mass = "150"
process.VBFMCttbarAnalysis.verbosity = False
process.VBFMCttbarAnalysis.eventsToPrint = 0

process.p0 = cms.Path(process.VBFMCttbarAnalysis)

process.schedule = cms.Schedule(process.p0)
