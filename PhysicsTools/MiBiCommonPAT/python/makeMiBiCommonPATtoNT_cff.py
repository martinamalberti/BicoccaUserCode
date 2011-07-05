import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.metTools import *
from PhysicsTools.PatAlgos.tools.tauTools import *
from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.tools.coreTools import *
from PhysicsTools.PatAlgos.tools.pfTools import *

from PhysicsTools.PatAlgos.selectionLayer1.leptonCountFilter_cfi import *
from PhysicsTools.PatAlgos.selectionLayer1.photonCountFilter_cfi import *
from PhysicsTools.PatAlgos.selectionLayer1.electronCountFilter_cfi import *
from PhysicsTools.PatAlgos.selectionLayer1.jetCountFilter_cfi import *

def makeMiBiCommonNT(process, GlobalTag, HLT='HLT', MC=False, MCType='Other'):

    # Setup the process
    process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
    process.load("FWCore.MessageService.MessageLogger_cfi")
    process.MessageLogger.cerr.FwkReport.reportEvery = 1000
    process.load('Configuration.StandardSequences.GeometryDB_cff')
    process.load("Configuration.StandardSequences.MagneticField_cff")
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
    process.GlobalTag.globaltag = GlobalTag
    

    # Source
    process.source = cms.Source(
        "PoolSource",
        fileNames = cms.untracked.vstring()
    )
    
    # Out
    process.out = cms.OutputModule(
        "PoolOutputModule",
        outputCommands = cms.untracked.vstring(),
        fileName = cms.untracked.string('file:./MiBiCommonPAT.root'),
    )
    #process.e = cms.EndPath(process.out)
    
    
    process.load("PhysicsTools.NtupleUtils.AllPassFilter_cfi")

    #--------------------------
    # Counter1: All read events
    process.AllEvents = process.AllPassFilter.clone()
    
    #------------------
    #Load PAT sequences
    process.load("PhysicsTools.PatAlgos.patSequences_cff")
    process.load("PhysicsTools.PatAlgos.tools.pfTools")
    
    # the MiBiPAT path
    process.MiBiCommonPAT = cms.Sequence(
        process.AllEvents # -> Counter
        )
    

    
    #----------------------
    # Lepton/Photon Filters
    process.load('PhysicsTools.PatAlgos.selectionLayer1.leptonCountFilter_cfi')
    process.LeptonsFilter = countPatLeptons.clone(
      electronSource = cms.InputTag("boostedElectrons"),
      muonSource     = cms.InputTag("boostedMuons"),
      minNumber      = cms.uint32(1)
     )

    
    #------------
    # Jet Filters    
    process.JetFilterAK5PF = countPatJets.clone(
      src = cms.InputTag("slimPatJetsTriggerMatch"),
      minNumber      = cms.uint32(2)
    )
    
    
    #-----------
    # Sequences & Other counters
    process.LeptonsFilterEvents = process.AllPassFilter.clone()
    process.JetFilterAK5PFEvents = process.AllPassFilter.clone()
    
    process.OneLeptonTwoJetsAK5PFSeq = cms.Sequence(
        process.LeptonsFilter*
        process.LeptonsFilterEvents*
        process.JetFilterAK5PF*
        process.JetFilterAK5PFEvents
        )
    
    # the MiBiNTUPLE
    process.load("PhysicsTools.MiBiCommonPAT.SimpleNtuplePATtoNT_cfi")
    process.MiBiCommonNT = process.SimpleNtuplePATtoNT.clone()
    process.MiBiCommonNT.saveMCPU              = cms.untracked.bool (MC)
    process.MiBiCommonNT.saveProcessId         = cms.untracked.bool (MC)
    process.MiBiCommonNT.savePhotonsMother     = cms.untracked.bool (MC)
    process.MiBiCommonNT.saveGenJet            = cms.untracked.bool (MC)
    process.MiBiCommonNT.saveMCPtHat           = cms.untracked.bool (MC)

    process.MiBiCommonNT.saveMCTTBar           = cms.untracked.bool (MC)    
    process.MiBiCommonNT.saveMCHiggs           = cms.untracked.bool (MC)
    process.MiBiCommonNT.saveMCHiggsWW         = cms.untracked.bool (MC)
    process.MiBiCommonNT.saveMCHiggsGammaGamma = cms.untracked.bool (MC)
    process.MiBiCommonNT.saveMCZW              = cms.untracked.bool (MC)
    
    if MCType == 'TTBar':
        process.MiBiCommonNT.saveMCTTBar = cms.untracked.bool (True)
    if MCType == 'Higgs':
        process.MiBiCommonNT.saveMCHiggs = cms.untracked.bool (True)
    if MCType == 'HiggsWW':
        process.MiBiCommonNT.saveMCHiggs   = cms.untracked.bool (True)
        process.MiBiCommonNT.saveMCHiggsWW = cms.untracked.bool (True)
    if MCType == 'HiggsGammaGamma':
        process.MiBiCommonNT.saveMCHiggs           = cms.untracked.bool (True)
        process.MiBiCommonNT.saveMCHiggsGammaGamma = cms.untracked.bool (True)                
    if MCType == 'ZW':
        process.MiBiCommonNT.saveMCZW              = cms.untracked.bool (True)

    if not MC:
        process.MiBiCommonNT.TriggerResultsTag     = cms.InputTag("TriggerResults","","HLT")


    process.MiBiCommonNTOneLeptonTwoJetsAK5PF = process.MiBiCommonNT.clone()
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.JetTag    = cms.InputTag("slimPatJetsTriggerMatch")
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.EleTag    = cms.InputTag("boostedElectrons")
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.MuTag     = cms.InputTag("boostedMuons")    
    
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.MetTag     = cms.InputTag("met")    
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.TCMetTag   = cms.InputTag("tcMet")    
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.PFMetTag   = cms.InputTag("pfMet")    

    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.saveHLT        = cms.untracked.bool (True)     
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.saveBS         = cms.untracked.bool (True)
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.savePV         = cms.untracked.bool (True)
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.saveRho        = cms.untracked.bool (True)
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.saveEleLessPV  = cms.untracked.bool (False) # default no revertex
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.saveMuonLessPV = cms.untracked.bool (False) # default no revertex
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.saveTrack      = cms.untracked.bool (False) # default no tracks
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.saveTau        = cms.untracked.bool (False) # default no tau process.MiBiCommonNTOneLeptonTwoJetsAK5PF.saved
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.saveMu         = cms.untracked.bool (True)
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.saveEle        = cms.untracked.bool (True)
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.saveMet        = cms.untracked.bool (True)
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.saveJet        = cms.untracked.bool (True)
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.savePhoton     = cms.untracked.bool (False)
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.saveHCALNoise  = cms.untracked.bool (False)
    
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.saveMCPU              = cms.untracked.bool (MC)
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.saveMCPtHat           = cms.untracked.bool (MC)
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.saveMCTTBar           = cms.untracked.bool (False)    
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.saveMCHiggs           = cms.untracked.bool (MC)
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.saveMCHiggsWW         = cms.untracked.bool (MC)
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.saveMCHiggsGammaGamma = cms.untracked.bool (False)
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.saveMCZW              = cms.untracked.bool (False)
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.saveProcessId         = cms.untracked.bool (MC)
    process.MiBiCommonNTOneLeptonTwoJetsAK5PF.savePhotonsMother     = cms.untracked.bool (False)

    # VBF paths
    process.MiBiPathAK5PF = cms.Path(process.MiBiCommonPAT*process.OneLeptonTwoJetsAK5PFSeq*process.MiBiCommonNTOneLeptonTwoJetsAK5PF)
