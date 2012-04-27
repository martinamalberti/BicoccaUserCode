import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.GeometryDB_cff import *

#from PhysicsTools.MiBiCommonPAT.offlinePrimaryVerticesParameters_cfi import *

SimpleNtuple = cms.EDAnalyzer(
    "SimpleNtuple",
    
    #-------------------
    #### Input Tags ####

    MCPileupTag = cms.InputTag ("addPileupInfo"),
    
    TriggerEventTag = cms.InputTag("hltTriggerSummaryAOD"),
    TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),

    #### primary vertices
    PVTag = cms.InputTag("offlinePrimaryVerticesWithBS"),
    NoMuonPVTag = cms.InputTag("offlinePrimaryVerticesNoMu","WithBS"),

    #### tracks ####
    TracksTag = cms.InputTag("generalTracks"),
    NoMuonTracksTag = cms.InputTag("NoMuonTrackProducer"),

    #### muons ####
    TauTag = cms.InputTag("patTaus"),
    
    #### muons ####
    MuTag = cms.InputTag("patMuons"),
     
    #### electrons ####
    EleTag                = cms.InputTag("patElectrons"),
    EleID_names           = cms.vstring(
        #'simpleEleId95relIso',
        #'simpleEleId90relIso',
        #'simpleEleId85relIso',
        #'simpleEleId80relIso',
        #'simpleEleId70relIso',
        #'simpleEleId60relIso',
        #'simpleEleId95cIso',
        #'simpleEleId90cIso',
        #'simpleEleId85cIso',
        #'simpleEleId80cIso',
        #'simpleEleId70cIso',
        #'simpleEleId60cIso'
        ),
    
    #### photons ####
    PhotonTag    = cms.InputTag("patPhotons"),
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
    saveRho        = cms.untracked.bool (True),
    saveEleLessPV  = cms.untracked.bool (False), # default no revertex
    saveMuonLessPV = cms.untracked.bool (True), # default no revertex
    saveTrack      = cms.untracked.bool (True), # default no tracks
    saveTau        = cms.untracked.bool (False), # default no tau saved
    saveMu         = cms.untracked.bool (True),
    saveEle        = cms.untracked.bool (False),
    saveMet        = cms.untracked.bool (False),
    saveJet        = cms.untracked.bool (False),
    savePhoton     = cms.untracked.bool (False),
    saveHCALNoise  = cms.untracked.bool (False),
    
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
    eventType = cms.untracked.int32(0),


    #check that these parameters correspond to what is used in the actual RECO
    #vertexParameters = offlinePrimaryVerticesParameters
        
)
