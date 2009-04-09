#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

process = cms.Process("VBFHWW2l2nuFirstFilter")

process.load("Configuration.StandardSequences.Geometry_cff") # for CaloGeometryRecord

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

#              'file:/tmp/amassiro/VBFHWW2l2nuTest_10.root'
 
#         'file:/tmp/amassiro/VBFHWW2l2nuTest_24.root'
#           'file:/tmp/amassiro/VBFHWW2l2nuTest_6Feb09.root'
#         'file:/tmp/amassiro/VBFHWW2l2nuTest_21.root'
#         'file:/tmp/amassiro/relval_Zee_CMSSW_2_1_7.root'
#         'rfio:/castor/cern.ch/cms/store/path/filename.root'



'rfio:/castor/cern.ch/user/a/amassiro/VBF/Skimmed/H130_WW_2l_Summer08_IDEAL_V9_v2/VBFHWW2l2nuTest_1.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Skimmed/H130_WW_2l_Summer08_IDEAL_V9_v2/VBFHWW2l2nuTest_10.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Skimmed/H130_WW_2l_Summer08_IDEAL_V9_v2/VBFHWW2l2nuTest_11.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Skimmed/H130_WW_2l_Summer08_IDEAL_V9_v2/VBFHWW2l2nuTest_12.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Skimmed/H130_WW_2l_Summer08_IDEAL_V9_v2/VBFHWW2l2nuTest_13.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Skimmed/H130_WW_2l_Summer08_IDEAL_V9_v2/VBFHWW2l2nuTest_14.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Skimmed/H130_WW_2l_Summer08_IDEAL_V9_v2/VBFHWW2l2nuTest_15.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Skimmed/H130_WW_2l_Summer08_IDEAL_V9_v2/VBFHWW2l2nuTest_16.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Skimmed/H130_WW_2l_Summer08_IDEAL_V9_v2/VBFHWW2l2nuTest_17.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Skimmed/H130_WW_2l_Summer08_IDEAL_V9_v2/VBFHWW2l2nuTest_18.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Skimmed/H130_WW_2l_Summer08_IDEAL_V9_v2/VBFHWW2l2nuTest_19.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Skimmed/H130_WW_2l_Summer08_IDEAL_V9_v2/VBFHWW2l2nuTest_2.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Skimmed/H130_WW_2l_Summer08_IDEAL_V9_v2/VBFHWW2l2nuTest_20.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Skimmed/H130_WW_2l_Summer08_IDEAL_V9_v2/VBFHWW2l2nuTest_21.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Skimmed/H130_WW_2l_Summer08_IDEAL_V9_v2/VBFHWW2l2nuTest_22.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Skimmed/H130_WW_2l_Summer08_IDEAL_V9_v2/VBFHWW2l2nuTest_23.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Skimmed/H130_WW_2l_Summer08_IDEAL_V9_v2/VBFHWW2l2nuTest_24.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Skimmed/H130_WW_2l_Summer08_IDEAL_V9_v2/VBFHWW2l2nuTest_3.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Skimmed/H130_WW_2l_Summer08_IDEAL_V9_v2/VBFHWW2l2nuTest_4.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Skimmed/H130_WW_2l_Summer08_IDEAL_V9_v2/VBFHWW2l2nuTest_5.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Skimmed/H130_WW_2l_Summer08_IDEAL_V9_v2/VBFHWW2l2nuTest_6.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Skimmed/H130_WW_2l_Summer08_IDEAL_V9_v2/VBFHWW2l2nuTest_7.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Skimmed/H130_WW_2l_Summer08_IDEAL_V9_v2/VBFHWW2l2nuTest_8.root',
'rfio:/castor/cern.ch/user/a/amassiro/VBF/Skimmed/H130_WW_2l_Summer08_IDEAL_V9_v2/VBFHWW2l2nuTest_9.root'



        )
    )
    
process.out = cms.OutputModule("PoolOutputModule",
                               verbose = cms.untracked.bool(False),
                               fileName = cms.untracked.string('FirstFilter_H130_WW_2l_Summer08_IDEAL_V9_v2.root'),
                               outputCommands = cms.untracked.vstring('drop *',
                                                                      'keep *_muons_*_*',
                                                                      'keep *_globalMuons_*_*',
                                                                      'keep *_pixelMatchGsfElectrons_*_*',
                                                                      'keep *_isolatedMuons_*_*', 
                                                                      'keep *_*_*_VBFHWW2l2nuFirstFilter',  
                                                                      'keep *_selectedMuons_*_*'
                                                                      )
                               )



process.p = cms.Path ( process.VBFPreselectionSequence)

# Output
process.o = cms.EndPath ( process.out )
