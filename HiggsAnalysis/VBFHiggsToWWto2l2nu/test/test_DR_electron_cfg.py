#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

process = cms.Process("VBFElectronAmbiguityResolverAnalyzer")

process.load("Configuration.StandardSequences.Geometry_cff") # for CaloGeometryRecord

process.load('Configuration/StandardSequences/Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'

##########
# Source #
##########


process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

from HiggsAnalysis.VBFHiggsToWWto2l2nu.Data.SimpleTreeInput_H160_WW_2l_redigi_cfi import *

process.source = cms.Source(
    "PoolSource",
    VBFHWW2l2nuSource,
#     fileNames = cms.untracked.vstring('file:/tmp/amassiro/VBF_SkimEfficiency_9.root'),
    skipBadFiles = cms.untracked.bool(True),
    debugFlag = cms.untracked.bool(True),
    debugVebosity = cms.untracked.uint32(1)
   )

    
  
######################################
## VBFEleAmbiguityResolver sequence ##

process.EleAmbiguityResolution = cms.EDFilter("VBFEleAmbiguityResolution",
   src = cms.InputTag("pixelMatchGsfElectrons"),
)
  
process.EleAmbiguityResolutionSequence = cms.Sequence(
 process.EleAmbiguityResolution
)

## end VBFEleAmbiguityResolver sequence ##
##########################################

    
##########################
## SimpleTree sequence ##


process.ntple = cms.EDAnalyzer(
    "VBFElectronAmbiguityResolverAnalyzer",
#     GSFInputTag = cms.InputTag("EleAmbiguityResolution")
    GSFInputTag = cms.InputTag("pixelMatchGsfElectrons") 
    )
    
process.ntpleSequence = cms.Sequence(
  process.ntple
)

## end SimpleTree sequence ##
#############################


########
# Path #
########

process.pathNtuple = cms.Path(
#       process.EleAmbiguityResolutionSequence *
      process.ntpleSequence
      )

##########
# Output #
##########

process.TFileService = cms.Service("TFileService", 
                                 fileName = cms.string("/tmp/amassiro/prova.root"),
                                 closeFileFast = cms.untracked.bool(True),
                                )

###########
# shedule #
###########


process.schedule = cms.Schedule(process.pathNtuple) 