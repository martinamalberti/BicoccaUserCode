import FWCore.ParameterSet.Config as cms

NoLeptonTrackProducer = cms.EDProducer(
    "NoLeptonTrackProducer",
    #input collection of tracks
    #
    #input collection of muons
    #
    #min pt for muons
    ptMin = cms.untracked.double(5.0) 
    
    )
