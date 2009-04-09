#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

process = cms.Process("VBFHWW2l2nuSkim")



process.load('Configuration/StandardSequences/Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'

##########
# Source #
##########


process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))

process.source = cms.Source(
    "PoolSource",
    debugFlag = cms.untracked.bool(True),
    debugVebosity = cms.untracked.uint32(1),
    fileNames = cms.untracked.vstring(
#         'file:/tmp/amassiro/VBFHWW2l2nuTest_8.root'
           'rfio:/castor/cern.ch/user/a/amassiro/VBF/Dump/H130_WW_2l_Summer08_IDEAL_V9_v2/VBFHWW2l2nuTest_1.root'
#         'rfio:/castor/cern.ch/cms/store/path/filename.root'
        )
    )

   
process.muonFilter = cms.EDFilter("PtMinMuonCountFilter",
  src = cms.InputTag("muons"),
  minNumber = cms.uint32(1),
  ptMin = cms.double(5.0)
)

process.electronFilter = cms.EDFilter("PtMinPixelMatchGsfElectronCountFilter",
  src = cms.InputTag("pixelMatchGsfElectrons"),
  minNumber = cms.uint32(1),
  ptMin = cms.double(5.0) 
)


#####################
## Skim Efficiency ##

process.SkimEfficiency = cms.EDAnalyzer(
    "VBFSkimEfficiencyNtuple",
   GSFInputTag = cms.InputTag("pixelMatchGsfElectrons"),
   elePtMin = cms.double(5.0),
   muInputTag = cms.InputTag("muons"),
   muPtMin = cms.double(5.0), 
   minLeptonsNum = cms.int32(1)
   )
    
process.SkimEfficiencySequence = cms.Sequence(
 process.SkimEfficiency
)

## end Skim Efficiency ##
#########################


########
# Path #
########

process.countMu = cms.Path (process.muonFilter)
process.countEle = cms.Path (process.electronFilter)
#process.countMuAndEle= cms.Path (process.muonFilter + process.electronFilter)

process.SkimEfficiencyPath = cms.Path (process.SkimEfficiencySequence)


##########
# Output #
##########

from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFHWW2l2nuEventContent_cff import *

process.VBFHWW2l2nuOutputModule = cms.OutputModule(
    "PoolOutputModule",
    VBFHWW2l2nuEventContent,
    dataset = cms.untracked.PSet(dataTier = cms.untracked.string('USER')),
    fileName = cms.untracked.string('VBFHWW2l2nuTest.root'),
#     fileName = cms.untracked.string('/tmp/amassiro/VBFHWW2l2nuTest_12Feb09_nuovoFiltro_HWW130_test.root'),
   
    SelectEvents = cms.untracked.PSet(
                SelectEvents = cms.vstring('countMu','countEle')
#                 SelectEvents = cms.vstring('countMuAndEle')
    )
)


process.TFileService = cms.Service("TFileService", 
                                 fileName = cms.string("VBF_SkimEfficiency.root"),
                                 closeFileFast = cms.untracked.bool(True),
                                )


###########
# shedule #
###########


process.o = cms.EndPath ( process.VBFHWW2l2nuOutputModule )

process.schedule = cms.Schedule(process.SkimEfficiencyPath,process.countMu,process.countEle,process.o) 