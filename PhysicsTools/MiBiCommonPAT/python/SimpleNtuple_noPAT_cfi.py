import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.GeometryDB_cff import *

SimpleNtuple_noPAT = cms.EDAnalyzer(
    "SimpleNtuple_noPAT",
    
    #-------------------
    #### Input Tags ####

    MCPileupTag = cms.InputTag ("addPileupInfo"),
    
    TriggerEventTag = cms.InputTag("hltTriggerSummaryAOD"),
    TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
    PatTriggerEventTag = cms.InputTag("patTriggerEvent","","PAT"),
    
    PVTag = cms.InputTag("goodOfflinePrimaryVertices"),

    #### tracks ####
    TracksTag = cms.InputTag("generalTracks"),

    #### SCs ####
    EBSCTag = cms.InputTag ("correctedHybridSuperClusters") ,
    EESCTag = cms.InputTag ("correctedMulti5x5SuperClustersWithPreshower") ,

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
    TCMetTag = cms.InputTag("patMETsAK5TC"),
    PFMetTag = cms.InputTag("patMETsAK5PF"),
    
    #### jets ####
    MCtruthTag = cms.InputTag("genParticles"),
    
    MatchingConeTreshold = cms.double(0.05),
    ElectronPtCut        = cms.double(8.0),     
    MuonPtCut        = cms.double(8.0),     


    PDFWeightsTag = cms.InputTag ("pdfWeights:CT10") ,

    
    #--------------
    #### flags ####

    dataFlag       = cms.untracked.bool (True),
    saveHLT        = cms.untracked.bool (True),     
    saveBS         = cms.untracked.bool (True),
    savePV         = cms.untracked.bool (True),
    saveRho        = cms.untracked.bool (True),
    saveEleLessPV  = cms.untracked.bool (False), # default no revertex
    saveMuonLessPV = cms.untracked.bool (False), # default no revertex
    saveTrack      = cms.untracked.bool (False), # default no tracks
    saveSC         = cms.untracked.bool (False), #default no superclusters
    saveTau        = cms.untracked.bool (False), # default no tau saved
    saveMu         = cms.untracked.bool (True),
    saveEle        = cms.untracked.bool (True),
    saveMet        = cms.untracked.bool (True),
    saveJet        = cms.untracked.bool (True),
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
    
    savePDFWeights  = cms.untracked.bool (False), #default no 
   

   
    verbosity = cms.untracked.bool(False),
    eventType = cms.untracked.int32(0) 
)
