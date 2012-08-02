import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.GeometryDB_cff import *

SimpleNtupleGen = cms.EDAnalyzer(
    "SimpleNtupleGen",
    
    #-------------------
    #### Input Tags ####
    
    GenParticlesTag = cms.InputTag("genParticles"),
    GenJetTag       = cms.InputTag("ak5GenJets"),
    GenMetTag       = cms.InputTag("genMetTrue"),
    
    
    #--------------
    #### flags ####
    saveMCPtHat            = cms.untracked.bool(True),
    saveGenEle             = cms.untracked.bool(True),
    saveGenMu              = cms.untracked.bool(True),
    saveGenTau             = cms.untracked.bool(True),
    saveGenTauJ            = cms.untracked.bool(True),
    saveGenMet             = cms.untracked.bool(True),
    saveGenJet             = cms.untracked.bool(True),
    saveGenTTBar           = cms.untracked.bool(False),
    saveGenHiggs           = cms.untracked.bool(False),
    saveGenHiggsWW         = cms.untracked.bool(False),
    saveGenHiggsGammaGamma = cms.untracked.bool(False),
	    
    eventType      = cms.untracked.int32(0),
    
    verbosity = cms.untracked.bool(False)
)
