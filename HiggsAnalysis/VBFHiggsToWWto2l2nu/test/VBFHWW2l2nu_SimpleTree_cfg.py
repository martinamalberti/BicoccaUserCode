#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

process = cms.Process("VBFHWW2l2nuSImpleTree")

process.load("Configuration.StandardSequences.Geometry_cff") # for CaloGeometryRecord

process.load('Configuration/StandardSequences/Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'

##########
# Source #
##########


process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

from HiggsAnalysis.VBFHiggsToWWto2l2nu.Data.SimpleTreeInput_H160_WW_2l_cfi import *

process.source = cms.Source(
    "PoolSource",
    VBFHWW2l2nuSource,
    debugFlag = cms.untracked.bool(True),
    debugVebosity = cms.untracked.uint32(1)
   )

    
   
##########################
## SimpleTree sequence ##

process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.SimpleNtple_cfi")

process.ntpleSequence = cms.Sequence(
 process.ntpla
)

## end SimpleTree sequence ##
#####################################




########
# Path #
########

process.pathNtuple = cms.Path(process.ntpleSequence)

##########
# Output #
##########

process.TFileService = cms.Service("TFileService", 
                                 fileName = cms.string("VBF_SimpleTree.root"),
                                 closeFileFast = cms.untracked.bool(True),
                                )



###########
# shedule #
###########


process.schedule = cms.Schedule(process.pathNtuple) 