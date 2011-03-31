import FWCore.ParameterSet.Config as cms

simpleNtuple = cms.EDAnalyzer(
    "SimpleNtuple",
    L1Tag               = cms.InputTag("gtDigis"),
    HLTTag              = cms.InputTag("TriggerResults"),
    PVTag               = cms.InputTag("offlinePrimaryVertices"),
    #recHitCollection_EB = cms.InputTag("reducedEcalRecHitsEB"),
    #recHitCollection_EE = cms.InputTag("reducedEcalRecHitsEB"),
    recHitCollection_EB = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
    recHitCollection_EE = cms.InputTag("ecalRecHit","EcalRecHitsEE"),    
    EleTag              = cms.InputTag("selectedPatElectrons"),
    MuTag               = cms.InputTag("selectedPatMuons"),
    JetTag              = cms.InputTag("selectedPatJets"),
    CALOMetTag          = cms.InputTag("patMETs"),
    TCMetTag            = cms.InputTag("patMETsTC"),
    PFMetTag            = cms.InputTag("patMETsPF"),
    eleId_names         = cms.vstring('eidLoose','eidTight','eidRobustLoose','eidRobustTight'),
    saveL1        = cms.untracked.bool(True),
    saveHLT       = cms.untracked.bool(True),
    saveEle       = cms.untracked.bool(True),
    saveEleShape  = cms.untracked.bool(True),
    saveMu        = cms.untracked.bool(True),
    saveJet       = cms.untracked.bool(True),
    saveCALOMet   = cms.untracked.bool(True),
    saveTCMet     = cms.untracked.bool(True),
    savePFMet     = cms.untracked.bool(True),
    verbosity_    = cms.untracked.bool(False)
    )
