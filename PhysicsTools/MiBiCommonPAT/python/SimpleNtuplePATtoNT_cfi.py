import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.GeometryDB_cff import *

SimpleNtuplePATtoNT = cms.EDAnalyzer(
    "SimpleNtuplePATtoNT",
    
    #-------------------
    #### Input Tags ####

    MCPileupTag = cms.InputTag ("addPileupInfo"),
    
    TriggerEventTag = cms.InputTag("hltTriggerSummaryAOD"),
    TriggerResultsTag = cms.InputTag("TriggerResults","","RECO"),
    #TriggerResultsTag = cms.InputTag("TriggerResults","","REDIGI311X"),
    
    PVTag = cms.InputTag("goodPrimaryVertices"),

    theBeamSpotTag = cms.InputTag("offlineBeamSpot"),

    #### tracks ####
    TracksTag = cms.InputTag("generalTracks"),

    #### muons ####
    TauTag = cms.InputTag("patTaus"),
    
    #### muons ####
    MuTag = cms.InputTag("patMuons"),
     
    #### electrons ####
    EleTag                = cms.InputTag("patElectrons"),
    EleID_names           = cms.vstring(
    #   "eidVBTFRel95",
    #   "eidVBTFRel80",
       "cicHyperTight1",
       "cicHyperTight1HWW",
       "cicHyperTight1MC",
       "cicHyperTight2",
       "cicHyperTight2HWW",
       "cicHyperTight2MC",
       "cicHyperTight3",
       "cicHyperTight3HWW",
       "cicHyperTight3MC",
       "cicLoose",
       "cicLooseHWW",
       "cicLooseMC",
       "cicMedium",
       "cicMediumHWW",
       "cicMediumMC",
       "cicSuperTight",
       "cicSuperTightHWW",
       "cicSuperTightMC",
       "cicTight",
       "cicTightHWW",
       "cicTightMC",
       "cicVeryLoose",
       "cicVeryLooseHWW",
       "cicVeryLooseMC",
       "egammaIDLikelihood",
       "eidLoose",
       "eidRobustHighEnergy",
       "eidRobustLoose",
       "eidRobustTight",
       "eidTight",
       "vbtf11WP60",
       "vbtf11WP70",
       "vbtf11WP80",
       "vbtf11WP85",
       "vbtf11WP90",
       "vbtf11WP95"

     ),

    
    #### photons ####
    PhotonTag    = cms.InputTag("patPhotons"),
    EBRechitTag  = cms.InputTag("reducedEcalRecHitsEB"),
    EERechitTag  = cms.InputTag("reducedEcalRecHitsEE"),
    
    #### jets ####
    JetTag     = cms.InputTag("patJets"),
    BTag_names = cms.vstring("trackCountingHighEffBJetTags","trackCountingHighPurBJetTags","simpleSecondaryVertexHighPurBJetTags","simpleSecondaryVertexHighEffBJetTags"),
    
    MetTag   = cms.InputTag("recoMETs"),
    TCMetTag = cms.InputTag("recoMETsTC"),         
    PFMetTag = cms.InputTag("recoMETsPF"),         
    chCandsTag= cms.InputTag("reducedPFCands"),
    ### jets ####
    MCtruthTag = cms.InputTag("prunedGen"),
    
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
    saveMuonLessPV = cms.untracked.bool (False), # default no revertex
    saveTrack      = cms.untracked.bool (False), # default no tracks
    saveTau        = cms.untracked.bool (False), # default no tau saved
    saveMu         = cms.untracked.bool (True),
    saveEle        = cms.untracked.bool (True),
    saveMet        = cms.untracked.bool (True),
    saveJet        = cms.untracked.bool (True),
    savePhoton     = cms.untracked.bool (True),
    saveHCALNoise  = cms.untracked.bool (True),
    
    saveMCPU              = cms.untracked.bool (True),
    saveMCPtHat           = cms.untracked.bool (True),
    saveMCTTBar           = cms.untracked.bool (True),    
    saveMCHiggs           = cms.untracked.bool (True),
    saveMCHiggsWW         = cms.untracked.bool (True),
    saveMCHiggsGammaGamma = cms.untracked.bool (False),
    saveMCZW              = cms.untracked.bool (True),
    saveGenJet            = cms.untracked.bool (False),
    saveProcessId         = cms.untracked.bool (True),
    savePhotonsMother     = cms.untracked.bool (False),
       
    verbosity = cms.untracked.bool(False),
    eventType = cms.untracked.int32(0) 
)
