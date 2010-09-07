import FWCore.ParameterSet.Config as cms

SimpleNtple = cms.EDAnalyzer("SimpleNtple",
     Onia2MuMuTag            = cms.InputTag("onia2MuMuPatTrkTrk"),
     Onia2EleEleTag          = cms.InputTag("onia2EleElePat"),
     MuTag                   = cms.InputTag("patMuons"),
     EleTag                  = cms.InputTag("patElectrons"),  # pixelMatchGsfElectrons
     TracksTag               = cms.InputTag("generalTracks"),
     PrimaryVertexTag        = cms.InputTag("offlinePrimaryVertices"),
     beamSpotTag             = cms.InputTag("offlineBeamSpot"),
     MCtruthTag              = cms.InputTag("genLeptons"),
     eleIDCut_LooseInputTag  = cms.string("eidLoose"),
     eleIDCut_RLooseInputTag = cms.string("eidRobustLoose"),
     eleIDCut_TightInputTag  = cms.string("eidTight"), 
     eleIDCut_RTightInputTag = cms.string("eidRobustTight"),
     triggerResultsTag       = cms.string("TriggerResults"),
     HLTprocessName          = cms.string("HLT8E29"),

     saveVtx    = cms.untracked.bool (True) ,
     saveMu     = cms.untracked.bool (True) ,
     saveTracks = cms.untracked.bool (True) , 
     saveEle    = cms.untracked.bool (True) ,
     saveMC     = cms.untracked.bool (True) ,
     saveTrigger     = cms.untracked.bool (True) ,
     saveBeamSpot    = cms.untracked.bool (True) ,
     saveOniaCand    = cms.untracked.bool (True) ,
     
     storeWSOnia     = cms.untracked.bool (True) ,
     beamSpotFlag    = cms.untracked.bool (True) ,
     oniaMaxCat    = cms.untracked.int32(2) ,
         
     treeName = cms.untracked.string ("OniaTree") ,
     verbosity = cms.untracked.bool(False),
     eventType = cms.untracked.int32(0), # 2 for data, 1 for signal, 0 for bkg
  
)
