import FWCore.ParameterSet.Config as cms

NoMuonTrackProducer = cms.EDProducer(
    "NoMuonTrackProducer",
    #input collection of tracks
    #
    #input collection of muons
    #
    #min pt for muons
    ptMin = cms.untracked.double(8.0) 
    
    )
