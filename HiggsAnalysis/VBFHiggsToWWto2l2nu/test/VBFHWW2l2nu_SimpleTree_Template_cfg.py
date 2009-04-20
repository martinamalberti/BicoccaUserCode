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

# from HiggsAnalysis.VBFHiggsToWWto2l2nu.Data.SimpleTreeInput_NAME_cfi import *

process.source = cms.Source(
    "PoolSource",
    VBFHWW2l2nuSource,
#     fileNames = cms.untracked.vstring('file:/tmp/amassiro/VBF_SkimEfficiency_9.root'),
    skipBadFiles = cms.untracked.bool(True),
    debugFlag = cms.untracked.bool(True),
    debugVebosity = cms.untracked.uint32(1)
   )

    
###################
## Jet cleaning  ##

process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFJetCleanerTemplateCaloJet_cfi")
process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFJetCleanerTemplatePFJet_cfi")
from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFJetCleanerTemplateCaloJet_cfi import cleanedJetTemplateCaloJet
from HiggsAnalysis.VBFHiggsToWWto2l2nu.VBFJetCleanerTemplatePFJet_cfi import cleanedJetTemplatePFJet

process.cleanedIterativeCone5CaloJets = cleanedJetTemplateCaloJet.clone(
   src = cms.InputTag("iterativeCone5CaloJets")
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

process.jetCleaningSequence = cms.Sequence(
 process.cleanedIterativeCone5CaloJets +
 process.cleanedSisCone5CaloJets       +
 process.cleanedIterativeCone5PFJets   +
 process.cleanedSisCone5PFJets 
)


## end Jet cleaning  ##
#######################

   
#########################
## Jet corrector L2+L3 ##

# from JetMETCorrections.Configuration.L2L3Corrections_Summer08_cff import *

process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer08Redigi_cff")
from JetMETCorrections.Configuration.L2L3Corrections_Summer08Redigi_cff import *


# es_prefer_L2L3JetCorrectorIC5Calo = cms.ESPrefer("JetCorrectionServiceChain","L2L3JetCorrectorIC5Calo")
# process.prefer(L2L3JetCorrectorIC5Calo)

# es_prefer_L2L3JetCorrectorSC5Calo = cms.ESPrefer("JetCorrectionServiceChain","L2L3JetCorrectorSC5Calo")
# es_prefer_L2L3JetCorrectorIC5PF = cms.ESPrefer("JetCorrectionServiceChain","L2L3JetCorrectorIC5PF")
# es_prefer_L2L3JetCorrectorSC5PF = cms.ESPrefer("JetCorrectionServiceChain","L2L3JetCorrectorSC5PF")

process.prefer("L2L3JetCorrectorIC5Calo")


process.L2L3CorJetIterativeCone5CaloJets = L2L3CorJetIC5Calo.clone(
   src = cms.InputTag("cleanedIterativeCone5CaloJets"),
   correctors = cms.vstring('L2L3JetCorrectorIC5Calo') 
)

process.L2L3CorJetSisCone5CaloJets = L2L3CorJetSC5Calo.clone(
   src = cms.InputTag("cleanedSisCone5CaloJets"),
   correctors = cms.vstring('L2L3JetCorrectorSC5Calo') 
)

process.L2L3CorJetIterativeCone5PFJets = L2L3CorJetIC5PF.clone(
   src = cms.InputTag("cleanedIterativeCone5PFJets"),
   correctors = cms.vstring('L2L3JetCorrectorIC5PF') 
)

process.L2L3CorJetSisCone5PFJets = L2L3CorJetSC5PF.clone(
   src = cms.InputTag("cleanedSisCone5PFJets"),
   correctors = cms.vstring('L2L3JetCorrectorSC5PF') 
)

process.jetMETCorrectionsSequence = cms.Sequence(
 process.L2L3CorJetIterativeCone5CaloJets +
 process.L2L3CorJetSisCone5CaloJets       +
 process.L2L3CorJetIterativeCone5PFJets   +
 process.L2L3CorJetSisCone5PFJets 
)

## end Jet corrector L2+L3 ##
#############################




#########################
## Jet corrector JPT ##
### https://twiki.cern.ch/twiki/bin/view/CMS/JetPlusTracksCorrections
### http://cmslxr.fnal.gov/lxr/source/JetMETCorrections/JetPlusTrack/test/JetAnalysis_cfg.py
### http://cmslxr.fnal.gov/lxr/source/JetMETCorrections/Configuration/python/JetPlusTrackCorrections_cff.py

process.load("Configuration.StandardSequences.MagneticField_cff")

process.load("JetMETCorrections.Configuration.JetPlusTrackCorrections_cff")
process.load("JetMETCorrections.Configuration.ZSPJetCorrections219_cff")

process.ZSPJetCorJetIcone5.src = cms.InputTag("cleanedIterativeCone5CaloJets") 

process.jetJPTCorrectionsSequence = cms.Sequence(
 process.ZSPJetCorrections *
 process.JetPlusTrackCorrections
)

## end Jet corrector JPT ##
#############################












##########################
## SimpleTree sequence ##

process.load("HiggsAnalysis.VBFHiggsToWWto2l2nu.SimpleNtple_cfi")


process.ntpla.JetTag = cms.InputTag("iterativeCone5CaloJets")

process.ntpla.bool_JetTagIterativeCone5CaloJets = cms.untracked.bool(True)
process.ntpla.bool_JetTagSisCone5CaloJets = cms.untracked.bool(True)
process.ntpla.bool_JetTagIterativeCone5PFJets = cms.untracked.bool(True)
process.ntpla.bool_JetTagSisCone5PFJets = cms.untracked.bool(True)

process.ntpla.JetTagIterativeCone5CaloJets = cms.InputTag("cleanedIterativeCone5CaloJets")
process.ntpla.JetTagSisCone5CaloJets = cms.InputTag("cleanedSisCone5CaloJets")
process.ntpla.JetTagIterativeCone5PFJets = cms.InputTag("cleanedIterativeCone5PFJets")
process.ntpla.JetTagSisCone5PFJets = cms.InputTag("cleanedSisCone5PFJets")



process.ntpla.bool_JetTagIterativeCone5CaloJets_L2L3 = cms.untracked.bool(True)
process.ntpla.bool_JetTagSisCone5CaloJets_L2L3 = cms.untracked.bool(True)
process.ntpla.bool_JetTagIterativeCone5PFJets_L2L3 = cms.untracked.bool(True)
process.ntpla.bool_JetTagSisCone5PFJets_L2L3 = cms.untracked.bool(True)

process.ntpla.JetTagIterativeCone5CaloJets_L2L3 = cms.InputTag("L2L3CorJetIterativeCone5CaloJets")
process.ntpla.JetTagSisCone5CaloJets_L2L3 = cms.InputTag("L2L3CorJetSisCone5CaloJets")
process.ntpla.JetTagIterativeCone5PFJets_L2L3 = cms.InputTag("L2L3CorJetIterativeCone5PFJets")
process.ntpla.JetTagSisCone5PFJets_L2L3 = cms.InputTag("L2L3CorJetSisCone5PFJets")


process.ntpla.bool_JetTagIterativeCone5CaloJets_JPT = cms.untracked.bool(True)
process.ntpla.bool_JetTagSisCone5CaloJets_JPT = cms.untracked.bool(False)
process.ntpla.bool_JetTagIterativeCone5PFJets_JPT = cms.untracked.bool(False)
process.ntpla.bool_JetTagSisCone5PFJets_JPT = cms.untracked.bool(False)

process.ntpla.JetTagIterativeCone5CaloJets_JPT = cms.InputTag("JetPlusTrackZSPCorJetIcone5")
### defined here: http://cmslxr.fnal.gov/lxr/source/JetMETCorrections/Configuration/python/JetPlusTrackCorrections_cff.py



process.ntpleSequence = cms.Sequence(
 process.jetCleaningSequence *        # Jet cleaning
 process.jetMETCorrectionsSequence *  # Jet corrector and Met corrector
 process.jetJPTCorrectionsSequence *  # Jet corrector and Met corrector JPT
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