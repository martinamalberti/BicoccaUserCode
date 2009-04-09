#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

process = cms.Process("VBFHWW2l2nuJetTaggingPreselections")

process.load("Configuration.StandardSequences.Geometry_cff") # for CaloGeometryRecord

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
        'file:/tmp/amassiro/VBFHWW2l2nuTest_1.root'
#            'rfio:/castor/cern.ch/user/a/amassiro/VBF/Dump/H130_WW_2l_Summer08_IDEAL_V9_v2/VBFHWW2l2nuTest_1.root'
#         'rfio:/castor/cern.ch/user/a/amassiro/VBF/090313_Everything_Skimmed_4Cluster/H170_WW_2l/VBFHWW2l2nuTest_1.root'


    
        )
    )

    
##############################   
## muon and electron filter ## 
   
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

process.EleSequence = cms.Sequence(
 process.electronFilter
)

process.MuSequence = cms.Sequence(
 process.muonFilter
)


## end muon and electron filter ## 
##################################


###########################
## Met and Jet corrector ##

#only iterativeCone5CaloJets
process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFJetAndMetCorrectorSequence_cff")

## end Met and Jet corrector ##
###############################


###################
## Jet cleaning  ##

process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFJetCleaner_cfi")
process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFJetCleanerRef_cfi")

# different type of jets
from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFJetCleaner_cfi import cleanedJets

process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFJetCleanerTemplatePFJet_cfi")
from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFJetCleanerTemplateCaloJet_cfi import cleanedJetTemplateCaloJet
process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFJetCleanerTemplateCaloJet_cfi")
from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFJetCleanerTemplatePFJet_cfi import cleanedJetTemplatePFJet

process.cleanedIterativeCone5CaloJets = cleanedJetTemplateCaloJet.clone(
   src = cms.InputTag("L2L3CorJetIcone5")
)

process.cleanedSisCone5CaloJets = cleanedJetTemplateCaloJet.clone(
   src = cms.InputTag("sisCone5CaloJets")
)

process.cleanedIterativeCone5PFJets = cleanedJetTemplatePFJet.clone(
   src = cms.InputTag("iterativeCone5PFJets")
)

process.cleanedSisCone5PFJets = cleanedJetTemplatePFJet.clone(
   src = cms.InputTag("sisCone5PFJets")
)

process.VBFJetCleaningSequence = cms.Sequence(
 process.cleanedIterativeCone5CaloJets
#  process.cleanedSisCone5CaloJets *
#  process.cleanedIterativeCone5PFJets *
#  process.cleanedSisCone5PFJets
)

## end Jet cleaning  ##
#######################

   

#################
## Jet tagger  ##

process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFJetTagger_cfi")
from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFJetTagger_cfi import jetTagger

process.taggingIterativeCone5CaloJets = jetTagger.clone(
   jetInputTag = cms.InputTag("cleanedIterativeCone5CaloJets"),
   tagJetsName = cms.string("taggedIterativeCone5CaloJets"),
   otherJetsName = cms.string("otherIterativeCone5CaloJets")   
)

process.taggingSisCone5CaloJets = jetTagger.clone(
   jetInputTag = cms.InputTag("cleanedSisCone5CaloJets"),
   tagJetsName = cms.string("taggedSisCone5CaloJets"),
   otherJetsName = cms.string("otherSisCone5CaloJets")   
)

process.taggingIterativeCone5PFJets = jetTagger.clone(
   jetInputTag = cms.InputTag("cleanedIterativeCone5PFJets"),
   tagJetsName = cms.string("taggedIterativeCone5PFJets"),
   otherJetsName = cms.string("otherIterativeCone5PFJets") 
)

process.taggingSisCone5PFJets = jetTagger.clone(
   jetInputTag = cms.InputTag("cleanedSisCone5PFJets"),
   tagJetsName = cms.string("taggedSisCone5PFJets"),
   otherJetsName = cms.string("otherSisCone5PFJets")    
)


process.VBFJetTaggingSequence = cms.Sequence(
 process.taggingIterativeCone5CaloJets
#  process.taggingSisCone5CaloJets *
#  process.taggingIterativeCone5PFJets *
#  process.taggingSisCone5PFJets
)

## end Jet tagger  ##
#####################




################################
## Event selector using jets  ##

process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFEventSelector_cfi")
from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFEventSelector_cfi import selectedEvents

process.selectedEventsIterativeCone5CaloJets = selectedEvents.clone(
   tagJetInputTag = cms.InputTag("taggingIterativeCone5CaloJets:taggedIterativeCone5CaloJets")
)

process.selectedEventsSisCone5CaloJets = selectedEvents.clone(
   tagJetInputTag = cms.InputTag("taggingSisCone5CaloJets:taggedSisCone5CaloJets")
)

process.selectedEventsIterativeCone5PFJets = selectedEvents.clone(
   tagJetInputTag = cms.InputTag("taggingIterativeCone5PFJets:taggedIterativeCone5PFJets")
)

process.selectedEventsSisCone5PFJets = selectedEvents.clone(
   tagJetInputTag = cms.InputTag("taggingSisCone5PFJets:taggedSisCone5PFJets")
)


process.VBFSelectingEventsSequence = cms.Sequence(
 process.selectedEventsIterativeCone5CaloJets
#  process.selectedEventsSisCone5CaloJets *
#  process.selectedEventsIterativeCone5PFJets *
#  process.selectedEventsSisCone5PFJets
)

## end Event selector using jets  ##
####################################







#######################
## Central Jet Veto  ##

process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFCentralJetVetoFilter_cfi")
from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFCentralJetVetoFilter_cfi import centralJetVeto

process.CentralJetVetoIterativeCone5CaloJets = centralJetVeto.clone(
   tagJetInputTag = cms.InputTag("taggingIterativeCone5CaloJets:taggedIterativeCone5CaloJets"),
   jetInputTag = cms.InputTag("taggingIterativeCone5CaloJets:otherIterativeCone5CaloJets")
)

process.CentralJetVetoSisCone5CaloJets = centralJetVeto.clone(
   tagJetInputTag = cms.InputTag("taggingSisCone5CaloJets:taggedSisCone5CaloJets"),
   jetInputTag = cms.InputTag("taggingSisCone5CaloJets:otherSisCone5CaloJets")
)

process.CentralJetVetoIterativeCone5PFJets = centralJetVeto.clone(
   tagJetInputTag = cms.InputTag("taggingIterativeCone5PFJets:taggedIterativeCone5PFJets"),
   jetInputTag = cms.InputTag("taggingIterativeCone5PFJets:otherIterativeCone5PFJets")
)

process.CentralJetVetoSisCone5PFJets = centralJetVeto.clone(
   tagJetInputTag = cms.InputTag("taggingSisCone5PFJets:taggedSisCone5PFJets"),
   jetInputTag = cms.InputTag("taggingSisCone5PFJets:otherSisCone5PFJets")
)

process.VBFCentralJetVetoSequence = cms.Sequence(
 process.CentralJetVetoIterativeCone5CaloJets 
#  process.CentralJetVetoSisCone5CaloJets *
#  process.CentralJetVetoIterativeCone5PFJets *
#  process.CentralJetVetoSisCone5PFJets
)

## end Central Jet Veto  ##
###########################


#############################################################
## VBFDiffTagFinderComparison Simple analyzer for TagJets  ##

process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFDiffTagFinderComparison_cfi")
from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFDiffTagFinderComparison_cfi import jetDiffTagFinder

process.jetDiffTagFinderIterativeCone5CaloJets = jetDiffTagFinder.clone(
   jetInputTag = cms.InputTag("taggingIterativeCone5CaloJets:taggedIterativeCone5CaloJets")
)

process.VBFJetDiffTagFinderSequence = cms.Sequence(
 process.jetDiffTagFinderIterativeCone5CaloJets
)


## end VBFDiffTagFinderComparison Simple analyzer for TagJets  ##
#################################################################


##################
## VBF sequence ##

process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFMCJetTagger_cfi")

process.VBFSequence = cms.Sequence(
 process.jetMETCorrectionsSequence *    ### Jet corrector and Met corrector
 process.VBFJetCleaningSequence *       ### jet Cleaning
 process.VBFJetTaggingSequence *        ### jet Tagging
 process.VBFSelectingEventsSequence *   ### select events using jet information
 process.VBFCentralJetVetoSequence *    ### CJV = Central Jet Veto
#  process.jetMCTagger *
 process.VBFJetDiffTagFinderSequence    ### simple analyzer for purity
)

## end VBF sequence ##
######################


#####################
# MC channel filter #

process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFMCChannelFilter_cfi")

process.channelFilterMCSequence = cms.Sequence(
 process.channelFilterMC
)

# end MC channel filter #
#########################


########
# Path #
########


process.pathVBF = cms.Path(process.VBFSequence)
process.pathEle = cms.Path(process.EleSequence)
process.pathMu = cms.Path(process.MuSequence)


##########
# Output #
##########

from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFHWW2l2nuEventContent_cff import *

process.VBFHWW2l2nuOutputModule = cms.OutputModule(
    "PoolOutputModule",
    VBFHWW2l2nuEventContent,
    dataset = cms.untracked.PSet(dataTier = cms.untracked.string('USER')),
    fileName = cms.untracked.string('/tmp/amassiro/VBF_todayTest.root'),
   
    SelectEvents = cms.untracked.PSet(
#                   SelectEvents = cms.vstring('pathEle','pathMu')
    )
)

process.VBFHWW2l2nuOutputModule.outputCommands.append(
         'keep *_*_*_VBFHWW2l2nuJetTaggingPreselections'
)


process.TFileService = cms.Service("TFileService", 
                                 fileName = cms.string("/tmp/amassiro/VBF_tree_todayTest.root"),
                                 closeFileFast = cms.untracked.bool(True),
                                )

process.o = cms.EndPath ( process.VBFHWW2l2nuOutputModule )




###########
# shedule #
###########

process.schedule = cms.Schedule(process.pathVBF,process.o) # exec only 
# process.schedule = cms.Schedule(process.pathEle,process.pathMu,process.pathVBF,process.o) # exec only 