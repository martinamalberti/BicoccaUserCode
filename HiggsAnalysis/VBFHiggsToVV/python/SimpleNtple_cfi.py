import FWCore.ParameterSet.Config as cms

SimpleNtple = cms.EDAnalyzer(
    "SimpleNtple",
    
     HLTTag           = cms.InputTag("TriggerResults","","HLT"),
     
     PVTag           = cms.InputTag("offlinePrimaryVertices"),

#### muons ####
     MuTag           = cms.InputTag("muons"),
     
#### electrons ####
     EleTag          = cms.InputTag("gsfElectrons"),
     eleID_names     = cms.vstring('simpleEleId80relIso','simpleEleId85relIso','eidLoose','eidRobustLoose','eidTight','eidRobustTight'),
     TracksTag       = cms.InputTag("generalTracks"),

#### jets ####
     JetTag          = cms.InputTag("sisCone5CaloJets"),

     MetTag          = cms.InputTag("corMetGlobalMuons"),
     Type1MetTag     = cms.InputTag("VBFType1SisCone5CaloMet"),
     TcMetTag        = cms.InputTag("tcMet"),         
     PFMetTag        = cms.InputTag("pfMet"),         

     MCtruthTag      = cms.InputTag("genParticles"),

     BTag_names      = cms.vstring('trackCountingHighEffBJetTags','trackCountingHighPurBJetTags','simpleSecondaryVertexHighPurBJetTags','simpleSecondaryVertexHighEffBJetTags'),

#### flags ####
     saveHLT          = cms.untracked.bool (True),     
     savePV           = cms.untracked.bool (True),     
     saveMu           = cms.untracked.bool (True),
     saveEle          = cms.untracked.bool (True),
     saveJet          = cms.untracked.bool (True),
     saveMet          = cms.untracked.bool (True),
     saveMCPtHat      = cms.untracked.bool (True),
     saveHiggs      = cms.untracked.bool (True),
     saveTTBar        = cms.untracked.bool (False),    
        
     verbosity = cms.untracked.bool(False),
     eventType = cms.untracked.int32(0) 
)
