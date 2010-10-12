import FWCore.ParameterSet.Config as cms

SimpleNtuple = cms.EDAnalyzer(
    "SimpleNtuple",
    
    HLTTag = cms.InputTag("TriggerResults","","HLT"),
    
    PVTag = cms.InputTag("offlinePrimaryVerticesWithBS"),
    
    #### muons ####
    MuTag = cms.InputTag("patMuons"),
     
    #### electrons ####
    EleTag                = cms.InputTag("patElectrons"),
    TracksTag             = cms.InputTag("generalTracks"),
    EBRecHitCollectionTag = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
    EERecHitCollectionTag = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
    EleID_names           = cms.vstring('simpleEleId80relIso','simpleEleId85relIso','eidLoose','eidRobustLoose','eidTight','eidRobustTight'),
    
    #### jets ####
    JetTag     = cms.InputTag("patJets"),
    BTag_names = cms.vstring('trackCountingHighEffBJetTags','trackCountingHighPurBJetTags','simpleSecondaryVertexHighPurBJetTags','simpleSecondaryVertexHighEffBJetTags'),
    
    MetTag   = cms.InputTag("patMETs"),
    TCMetTag = cms.InputTag("patMETsTC"),         
    PFMetTag = cms.InputTag("patMETsPF"),         
    
    #### jets ####
    MCtruthTag = cms.InputTag("genParticles"),
    
    
    #### flags ####
    dataFlag              = cms.untracked.bool (True),
    saveHLT               = cms.untracked.bool (True),     
    savePV                = cms.untracked.bool (True),     
    saveMu                = cms.untracked.bool (True),
    saveEle               = cms.untracked.bool (True),
    saveJet               = cms.untracked.bool (True),
    saveMet               = cms.untracked.bool (True),
    saveMCPtHat           = cms.untracked.bool (False),
    saveMCTTBar           = cms.untracked.bool (False),    
    saveMCHiggs           = cms.untracked.bool (False),
    saveMCHiggsWW         = cms.untracked.bool (False),
    saveMCHiggsGammaGamma = cms.untracked.bool (False),
       
    verbosity = cms.untracked.bool(False),
    eventType = cms.untracked.int32(0) 
)
