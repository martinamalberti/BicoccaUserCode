#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

process = cms.Process("VBFHWW2l2nuFirstFilter")

process.load("Configuration.StandardSequences.Geometry_cff") # for CaloGeometryRecord

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

    'file:/tmp/amassiro/VBFHWW2l2nuTest_12Feb09_nuovoFiltro.root'
#     'file:/tmp/amassiro/VBFHWW2l2nuTest_1.root'
 
#         'file:/tmp/amassiro/VBFHWW2l2nuTest_24.root'
#           'file:/tmp/amassiro/VBFHWW2l2nuTest_6Feb09.root'
#         'file:/tmp/amassiro/VBFHWW2l2nuTest_21.root'
#         'file:/tmp/amassiro/relval_Zee_CMSSW_2_1_7.root'
#         'rfio:/castor/cern.ch/cms/store/path/filename.root'






        )
    )
    
process.out = cms.OutputModule("PoolOutputModule",
                               verbose = cms.untracked.bool(False),
                               fileName = cms.untracked.string('/tmp/amassiro/VBFHWW2l2nuTest_Result.root'),
                               outputCommands = cms.untracked.vstring('drop *',
                                                                      'keep *_muons_*_*',
                                                                      'keep *_globalMuons_*_*',
                                                                      'keep *_pixelMatchGsfElectrons_*_*',
                                                                      'keep *_isolatedMuons_*_*', 
                                                                      'keep *_*_*_VBFHWW2l2nuFirstFilter',  
                                                                      'keep *_selectedMuons_*_*'
                                                                      )

                              
                               )

process.TFileService = cms.Service("TFileService", 
                                 fileName = cms.string("/tmp/amassiro/VBFHWW2l2nuTest_Tree_nuovoFiltro.root"),
                                 closeFileFast = cms.untracked.bool(True)
                                )



process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFElectronIsolationSequence_cff")
process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFElectronIDSequence_cff")

process.VBFFirstFilterTree = cms.EDAnalyzer("VBFFirstFilterTreeProducer",
     verbosity = cms.bool(True),
     verbosity_mc = cms.bool(True),
 
     srcElectrons = cms.InputTag("pixelMatchGsfElectrons"),
     srcMuons = cms.InputTag("muons"),         
     srcJets = cms.InputTag("iterativeCone5CaloJets"),

     eleTkIso = cms.InputTag("electronTrackIsolationScone"),
     eleEcalIso = cms.InputTag("electronEcalRecHitIsolationScone"),
     eleHcalIso = cms.InputTag("electronHcalDepth1TowerIsolationScone")     
)
                                          
process.VBFFirstFilterTreeSeq = cms.Sequence(
 process.VBFElectronIdSequence * 
 process.VBFElectronIsolationSequence *
 process.VBFFirstFilterTree
)
                               
process.p = cms.Path (process.VBFFirstFilterTreeSeq)

# Output
process.o = cms.EndPath ( process.out )
