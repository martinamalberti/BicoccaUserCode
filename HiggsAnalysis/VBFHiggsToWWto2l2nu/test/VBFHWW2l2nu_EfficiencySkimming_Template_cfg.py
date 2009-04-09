#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

process = cms.Process("VBFHWW2l2nuSimpleTree")

process.load("Configuration.StandardSequences.Geometry_cff") # for CaloGeometryRecord

process.load('Configuration/StandardSequences/Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'

##########
# Source #
##########


process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

from HiggsAnalysis.VBFHiggsToWWto2l2nu.Data.SimpleTreeInput_NAME_cfi import *

process.source = cms.Source(
    "PoolSource",
    VBFHWW2l2nuSource,
    skipBadFiles = cms.untracked.bool(True),
    debugFlag = cms.untracked.bool(True),
    debugVebosity = cms.untracked.uint32(1)
   )

    
####################
## Events counter ##

process.EventsCounter = cms.EDAnalyzer(
    "VBFEventsCounter"
   )
    
process.EventsCounterSequence = cms.Sequence(
 process.EventsCounter
)

## end Events counter ##
########################


########
# Path #
########

process.pathEventsCounter = cms.Path(process.EventsCounterSequence)

##########
# Output #
##########

process.TFileService = cms.Service("TFileService", 
                                 fileName = cms.string("VBF_EventsCounting_NAME.root"),
                                 closeFileFast = cms.untracked.bool(True),
                                )

###########
# shedule #
###########


process.schedule = cms.Schedule(process.pathEventsCounter) 