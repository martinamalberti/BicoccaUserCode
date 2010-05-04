import FWCore.ParameterSet.Config as cms

SimpleNtple = cms.EDAnalyzer("SimpleNtple",
     JetTag                  = cms.InputTag("pfJets"),
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
     endcapClusterCollection = cms.InputTag("multi5x5SuperClustersWithPreshower"), # w/ projective preshower energy
     beamSpotTag             = cms.InputTag("offlineBeamSpot"),
     triggerEventTag         = cms.string("hltTriggerSummaryAOD"),
     triggerResultsTag       = cms.string("TriggerResults"),
     HLTprocessName8e29      = cms.string("HLT"),
     #HLTprocessName8e29      = cms.string("HLT8E29"),
     HLTprocessName1e31      = cms.string("HLT"),

     saveJets   = cms.untracked.bool (True) ,
     saveVtx    = cms.untracked.bool (True) ,
     saveMu     = cms.untracked.bool (True) ,
     saveTracks = cms.untracked.bool (True) , 
     saveEle    = cms.untracked.bool (True) ,
     saveMC     = cms.untracked.bool (False) ,
     saveSC     = cms.untracked.bool (False) ,
     saveTrigger     = cms.untracked.bool (True) ,
     saveBeamSpot    = cms.untracked.bool (True) ,
     saveOniaCand    = cms.untracked.bool (True) ,
     
     storeWSOnia     = cms.untracked.bool (True) ,
     beamSpotFlag    = cms.untracked.bool (True) ,
     oniaMaxCat    = cms.untracked.int32(2) ,
    
     Chi2OniaVtxCut= cms.untracked.double(0.05), #0.01      
     OniaMassCut= cms.untracked.double(2.0), #3.2
     OniaS3DipCut= cms.untracked.double(4.), # 4.
     
     treeName = cms.untracked.string ("SimpleTree") ,
     verbosity = cms.untracked.bool(False),
     eventType = cms.untracked.int32(0), # 1 for signal, 0 for bkg
     
     GsfParameters = cms.PSet(           # Parameters are described here https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideGaussianSumFitter
      maxDistance = cms.double(0.01), #0.01
      maxNbrOfIterations = cms.int32(10),
      limitComponents = cms.bool(False), #True
      smoothTracks = cms.bool(False),
      GsfMergerParameters = cms.PSet(
        maxNbrComponents= cms.int32(4),
        merger = cms.string("CloseComponentsMerger"),
        distance = cms.string("KullbackLeiblerDistance")
          )
      ),
     
#      GsfParameters = cms.PSet(
#         maxshift = cms.double(0.0001),
#         maxstep = cms.int32(30),
#         maxlpshift = cms.double(0.1),
#         weightthreshold = cms.double(0.001),
#         limitComponents = cms.bool(True),
#         smoothTracks = cms.bool(True),
#         GsfMergerParameters = cms.PSet(
#                 maxNbrComponents= cms.int32(4),
#                 merger = cms.string("CloseComponentsMerger"),
#                 distance = cms.string("KullbackLeiblerDistance")
#          )
#     ),
  
)
