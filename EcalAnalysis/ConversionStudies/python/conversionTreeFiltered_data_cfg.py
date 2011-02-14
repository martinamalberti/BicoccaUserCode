import FWCore.ParameterSet.Config as cms

process = cms.Process("CONVERSIONTREE")



##GENERAL
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

process.GlobalTag.globaltag = 'GR_R_38X_V15::All'

process.load("Configuration.StandardSequences.Geometry_cff")
process.load('Configuration/StandardSequences/Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'


##SOURCE
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(2000) )
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
    
process.source = cms.Source("PoolSource",
        skipEvents = cms.untracked.uint32(0),
        fileNames = cms.untracked.vstring(

    #Spring10_MinBias_TuneD6T_7TeV-pythia6_GEN-SIM-RECO_START3X_V26B-v2
    #'file:/gwpool/users/deguio/DATASETS/Spring10_MinBias_TuneD6T_7TeV-pythia6_GEN-SIM-RECO_START3X_V26B-v2/FEFD6F35-BF68-DF11-A7E7-003048F0E81E.root'

    #'/store/data/Run2010B/Jet/RECO/PromptReco-v2/000/149/442/E46CB2D6-49E6-DF11-AAF1-0030487CD7EE.root'
    'file:/tmp/deguio/4AB1C58E-730F-E011-AA63-002354EF3BE4.root'
    )
)


#SKIMMING

# filter on PhysDeclared bit
process.skimming = cms.EDFilter("PhysDecl",
                                applyfilter = cms.untracked.bool(True),
                                debugOn = cms.untracked.bool(False),
                                HLTriggerResults = cms.InputTag("TriggerResults","","HLT")
                                )

# filter on bit 40 || 41 nad !(bit36 || bit37 || bit38 || bit39)
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('(40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')

#Good Vertex Filter (see GOODCOLL skim)
process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
                                           vertexCollection = cms.InputTag('offlinePrimaryVertices'),
                                           minimumNDOF = cms.uint32(4) ,
                                           maxAbsZ = cms.double(24),
                                           maxd0 = cms.double(2)
                                           )

# FilterOutScraping
process.noscraping = cms.EDFilter("FilterOutScraping",
                                  applyfilter = cms.untracked.bool(True),
                                  debugOn = cms.untracked.bool(False),
                                  numtrack = cms.untracked.uint32(10),
                                  thresh = cms.untracked.double(0.25)
                                  )


process.goodvertex=cms.Sequence(process.primaryVertexFilter*process.noscraping)
process.goodcollisions=cms.Sequence(process.hltLevel1GTSeed*process.goodvertex)


# Filter on conversions number per event
process.conversionFilter = cms.EDFilter('conversionFilter')

process.conversionTree = cms.EDAnalyzer('conversionTree',
                                        TriggerEventTag = cms.InputTag("hltTriggerSummaryAOD"),
                                        TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
                                        PVTag = cms.InputTag("offlinePrimaryVertices"),
                                        MCtruthTag = cms.InputTag("genParticles"),
                                        #### photons ####
                                        PhotonTag    = cms.InputTag("photons"),
                                        EBRechitTag  = cms.InputTag("reducedEcalRecHitsEB","","RECO"),
                                        EERechitTag  = cms.InputTag("reducedEcalRecHitsEE","","RECO"),
                                        )



##OUTPUT
process.TFileService = cms.Service("TFileService",
    fileName = cms.string("conversionTreeFiltered_DATA.root"),
    closeFileFast = cms.untracked.bool(True),
    )

process.load("PhysicsTools.NtupleUtils.AllPassFilter_cfi")

# Counter1: All read events
#process.AllEvents = process.AllPassFilter.clone()
# Counter2: Good Vertex
#process.ConvFilter = process.AllPassFilter.clone()
# Counter3: Conversion Filter
#process.GoodVertex = process.AllPassFilter.clone()

process.p = cms.Path(
    #process.AllEvents * # -> Counter
    
    process.goodvertex*
    #process.GoodVertex* # -> Counter
    
    process.conversionFilter*
    #process.ConvFilter* # -> Counter
    
    #process.trackerOnlyConversionSequence*  #ancora necessario in 38X?
    process.conversionTree
    )



