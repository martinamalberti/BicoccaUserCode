import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.GeometryDB_cff import *

SimpleNtuple = cms.EDAnalyzer(
    "SimpleNtuple",
    
    #-------------------
    #### Input Tags ####

    MCPileupTag = cms.InputTag ("addPileupInfo"),
    
    TriggerEventTag = cms.InputTag("hltTriggerSummaryAOD"),
    TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
    
    PVTag = cms.InputTag("offlinePrimaryVertices"),

    #### tracks ####
    TracksTag = cms.InputTag("generalTracks"),

    #### muons ####
    TauTag = cms.InputTag("patTaus"),
    
    #### muons ####
    MuTag = cms.InputTag("patMuons"),
     
    #### electrons ####
    EleTag                = cms.InputTag("patElectrons"),
    EleID_names           = cms.vstring(
#        'simpleEleId95relIso',
#        'simpleEleId90relIso',
#        'simpleEleId85relIso',
#        'simpleEleId80relIso',
#        'simpleEleId70relIso',
#        'simpleEleId60relIso',
#        'simpleEleId95cIso',
#        'simpleEleId90cIso',
#        'simpleEleId85cIso',
#        'simpleEleId80cIso',
#        'simpleEleId70cIso',
#        'simpleEleId60cIso'
        ),
    
    #### photons ####
    PhotonTag             = cms.InputTag("patPhotons"),
    EBRechitTag  = cms.InputTag("reducedEcalRecHitsEB"),
    EERechitTag  = cms.InputTag("reducedEcalRecHitsEE"),
    
    
    #### jets ####
    JetTag     = cms.InputTag("patJets"),
    BTag_names = cms.vstring('trackCountingHighEffBJetTags','trackCountingHighPurBJetTags','simpleSecondaryVertexHighPurBJetTags','simpleSecondaryVertexHighEffBJetTags'),
    
    MetTag   = cms.InputTag("patMETs"),
    TCMetTag = cms.InputTag("patMETsTC"),         
    PFMetTag = cms.InputTag("patMETsPF"),         
    
    #### jets ####
    MCtruthTag = cms.InputTag("genParticles"),
    
    MatchingConeTreshold = cms.double(0.05),
    ElectronPtCut        = cms.double(8.0),     
    MuonPtCut        = cms.double(8.0),     
    
    #--------------
    #### flags ####

    dataFlag       = cms.untracked.bool (True),
    saveHLT        = cms.untracked.bool (True),     
    saveBS         = cms.untracked.bool (True),
    savePV         = cms.untracked.bool (True),
    saveEleLessPV  = cms.untracked.bool (False), # default no revertex
    saveMuonLessPV = cms.untracked.bool (False), # default no revertex
    saveTrack      = cms.untracked.bool (False), # default no tracks
    saveTau        = cms.untracked.bool (False), # default no tau saved
    saveMu         = cms.untracked.bool (True),
    saveEle        = cms.untracked.bool (True),
    saveMet        = cms.untracked.bool (True),
    saveJet        = cms.untracked.bool (True),
    savePhoton     = cms.untracked.bool (True),
    saveHCALNoise  = cms.untracked.bool (True),
    
    saveMCPU              = cms.untracked.bool (False),
    saveMCPtHat           = cms.untracked.bool (False),
    saveMCTTBar           = cms.untracked.bool (False),    
    saveMCHiggs           = cms.untracked.bool (False),
    saveMCHiggsWW         = cms.untracked.bool (False),
    saveMCHiggsGammaGamma = cms.untracked.bool (False),
    saveMCZW              = cms.untracked.bool (False),
    saveGenJet            = cms.untracked.bool (False),
    saveProcessId         = cms.untracked.bool (False),
    savePhotonsMother     = cms.untracked.bool (False),
       
    verbosity = cms.untracked.bool(False),
    eventType = cms.untracked.int32(0) 
)
