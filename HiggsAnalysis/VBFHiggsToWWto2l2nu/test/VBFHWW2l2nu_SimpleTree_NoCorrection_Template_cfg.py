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

    
###################
## Jet cleaning  ##

process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFJetCleaner_cfi")
process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFJetCleanerRef_cfi")


from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFJetCleaner_cfi import cleanedJets

process.cleanedJets = cleanedJets.clone(
   src = cms.InputTag("iterativeCone5CaloJets")
)


from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFJetCleanerRef_cfi import cleanedJetsRef

process.cleanedJetsRef = cleanedJetsRef.clone(
   src = cms.InputTag("iterativeCone5CaloJets")
)


## end Jet cleaning  ##
#######################


##########################
## SimpleTree sequence ##

process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.SimpleNtple_cfi")

# process.ntpla.JetTagSisCone5CaloJets = cms.InputTag("cleanedSisCone5CaloJets")
# process.ntpla.JetTagSisCone5PFJets = cms.InputTag("cleanedSisCone5PFJets")
# process.ntpla.JetTagIterativeCone5PFJets = cms.InputTag("cleanedIterativeCone5PFJets")

process.ntpleSequence = cms.Sequence(
 process.cleanedJets *                # Jet cleaning
 process.cleanedJetsRef *             # Jet cleaning ref creation
 process.ntpla                        # ntuple crator -> all particles/jets/met, reco/gen 
)

## end SimpleTree sequence ##
#############################




########
# Path #
########

process.pathNtuple = cms.Path(process.ntpleSequence)

##########
# Output #
##########

process.TFileService = cms.Service("TFileService", 
                                 fileName = cms.string("VBF_SimpleTree_NAME.root"),
                                 closeFileFast = cms.untracked.bool(True),
                                )

###########
# shedule #
###########


process.schedule = cms.Schedule(process.pathNtuple) 