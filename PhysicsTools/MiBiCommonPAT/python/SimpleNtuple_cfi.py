import FWCore.ParameterSet.Config as cms

SimpleNtuple = cms.EDAnalyzer(
    "SimpleNtuple",
    
    #-------------------
    #### Input Tags ####
    
    HLTTag = cms.InputTag("TriggerResults","","HLT"),
    
    PVTag = cms.InputTag("offlinePrimaryVerticesWithBS"),
    
    #### muons ####
    MuTag = cms.InputTag("selectedPatMuons"),
     
    #### electrons ####
    EleTag                = cms.InputTag("selectedPatElectrons"),
    EleID_names           = cms.vstring('eidLoose','eidRobustLoose','eidTight','eidRobustTight'),
    
    #### photons ####
    PhotonTag             = cms.InputTag("selectedPatPhotons"),

    #### jets ####
    JetTag     = cms.InputTag("selectedPatJets"),
    BTag_names = cms.vstring('trackCountingHighEffBJetTags','trackCountingHighPurBJetTags','simpleSecondaryVertexHighPurBJetTags','simpleSecondaryVertexHighEffBJetTags'),
    
    MetTag   = cms.InputTag("patMETs"),
    TCMetTag = cms.InputTag("patMETsTC"),         
    PFMetTag = cms.InputTag("patMETsPF"),         
    
    #### jets ####
    MCtruthTag = cms.InputTag("genParticles"),
    
    
    
    #--------------
    #### flags ####

    dataFlag      = cms.untracked.bool (True),
    saveHLT       = cms.untracked.bool (True),     
    saveBS        = cms.untracked.bool (True),
    savePV        = cms.untracked.bool (True),     
    saveMu        = cms.untracked.bool (True),
    saveEle       = cms.untracked.bool (True),
    saveMet       = cms.untracked.bool (True),
    saveJet       = cms.untracked.bool (True),
    saveHCALNoise = cms.untracked.bool (False),
    
    saveMCPtHat           = cms.untracked.bool (False),
    saveMCTTBar           = cms.untracked.bool (False),    
    saveMCHiggs           = cms.untracked.bool (False),
    saveMCHiggsWW         = cms.untracked.bool (False),
    saveMCHiggsGammaGamma = cms.untracked.bool (False),
    saveGenJet            = cms.untracked.bool (False),
       
    verbosity = cms.untracked.bool(False),
    eventType = cms.untracked.int32(0) 
)
