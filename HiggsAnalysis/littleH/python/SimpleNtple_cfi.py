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
     barrelClusterCollection = cms.InputTag("hybridSuperClusters"), # w/o clustering corrections
#     barrelClusterCollection = cms.InputTag("correctedHybridSuperClusters"), # w/ clustering corrections
#     endcapClusterCollection = cms.InputTag("multi5x5SuperClusters", "multi5x5EndcapSuperClusters"), # w/o preshower energy
     endcapClusterCollection = cms.InputTag("multi5x5SuperClustersWithPreshower"), # w/ projective preshower energy
     triggerEventTag         = cms.string("hltTriggerSummaryAOD"),
     triggerResultsTag       = cms.string("TriggerResults"),
     HLTprocessName8e29      = cms.string("HLT8E29"),
     HLTprocessName1e31      = cms.string("HLT"),  

     saveVtx     = cms.untracked.bool (True) ,
     saveMu      = cms.untracked.bool (True) ,
     saveTracks  = cms.untracked.bool (True) , 
     saveEle     = cms.untracked.bool (True) ,
     saveMC      = cms.untracked.bool (True) ,
     saveSC      = cms.untracked.bool (False) ,
     saveTrigger = cms.untracked.bool (False) ,

     treeName = cms.untracked.string ("SimpleTree") ,
     verbosity = cms.untracked.bool(False),
     eventType = cms.untracked.int32(0), # 1 for signal, 0 for bkg
)