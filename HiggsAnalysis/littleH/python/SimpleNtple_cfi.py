import FWCore.ParameterSet.Config as cms

SimpleNtple = cms.EDAnalyzer("SimpleNtple",
     MuTag                   = cms.InputTag("muons"),
     EleTag                  = cms.InputTag("gsfElectrons"),  # pixelMatchGsfElectrons
     TracksTag               = cms.InputTag("generalTracks"),
     PrimaryVertexTag        = cms.InputTag("offlinePrimaryVertices"),
     MCtruthTag              = cms.InputTag("genParticles"),
     eleIDCut_LooseInputTag  = cms.InputTag("eidLoose"),
     eleIDCut_RLooseInputTag = cms.InputTag("eidRobustLoose"),
     eleIDCut_TightInputTag  = cms.InputTag("eidTight"), 
     eleIDCut_RTightInputTag = cms.InputTag("eidRobustTight"),
     barrelClusterCollection = cms.InputTag("hybridSuperClusters"),
     endcapClusterCollection = cms.InputTag("FIXME"),

     saveVtx    = cms.untracked.bool (True) ,
     saveMu     = cms.untracked.bool (True) ,
     saveTracks = cms.untracked.bool (True) , 
     saveEle    = cms.untracked.bool (True) ,
     saveMC     = cms.untracked.bool (True) ,
     saveSC     = cms.untracked.bool (False) ,

     treeName = cms.untracked.string ("SimpleTree") ,
     verbosity = cms.untracked.bool(False),
     eventType = cms.untracked.int32(0), # 1 for signal, 0 for bkg
)