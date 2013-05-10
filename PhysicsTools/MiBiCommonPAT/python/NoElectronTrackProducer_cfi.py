import FWCore.ParameterSet.Config as cms

NoElectronTrackProducer = cms.EDProducer(
    "NoElectronTrackProducer",
    #input collection of tracks
    #
    #input collection of muons
    #
    #min pt for muons
    ptMin = cms.untracked.double(5.0) 
    
    )
