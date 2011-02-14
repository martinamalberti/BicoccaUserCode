import FWCore.ParameterSet.Config as cms

process = cms.Process("CONVERSIONTREE")



##GENERAL
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

process.GlobalTag.globaltag = 'START39_V8::All'

process.load("Configuration.StandardSequences.Geometry_cff")
process.load('Configuration/StandardSequences/Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'


##SOURCE
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.source = cms.Source("PoolSource",
        skipEvents = cms.untracked.uint32(0),
        fileNames = cms.untracked.vstring(

    #run 132440 trgBit 40||41 on Skim2
    #'rfio:'

    #/MinimumBias/Commissioning10-PromptReco-v7/RECO
    #'/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/F4C92A98-163C-DF11-9788-0030487C7392.root'

    #/MinimumBias/Commissioning10-GOODCOLL-v7/RAW-RECO 132440
    #'/store/data/Commissioning10/MinimumBias/RAW-RECO/v7/000/132/440/FCCA95EA-4A3C-DF11-B108-00E0817918BF.root'

    #/store/mc/Summer09/MinBias/GEN-SIM-RECO/MC_31X_V3-v1/
    #'file:/tmp/deguio/C6F7D0C1-E881-DE11-BA55-00E08133CDA0.root'

    #'rfio:/castor/cern.ch/user/d/deguio/Dalitz/Beam09/900GeV/conversionsFiltered_1.root',
    #'rfio:/castor/cern.ch/user/d/deguio/Dalitz/Beam09/900GeV/conversionsFiltered_2.root'

    #/MinBias/Spring10-START3X_V26A_356ReReco-v1/GEN-SIM-RECO
    #'/store/mc/Spring10/MinBias/GEN-SIM-RECO/START3X_V26A_356ReReco-v1/0009/FEFC70B6-F53D-DF11-B57E-003048679150.root'

    #Spring10_MinBias_TuneD6T_7TeV-pythia6_GEN-SIM-RECO_START3X_V26B-v2
    #'file:/gwpool/users/deguio/DATASETS/Spring10_MinBias_TuneD6T_7TeV-pythia6_GEN-SIM-RECO_START3X_V26B-v2/FEFD6F35-BF68-DF11-A7E7-003048F0E81E.root'

    'file:/tmp/deguio/FEEC6FF3-890E-E011-8EAD-001A64789E60.root'
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

# filter on primary vertex
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
                                        saveMC = cms.untracked.bool(True),
                                        saveMCPi0 = cms.untracked.bool(True),
                                        #### photons ####
                                        PhotonTag    = cms.InputTag("photons"),
                                        EBRechitTag  = cms.InputTag("reducedEcalRecHitsEB","","RECO"),
                                        EERechitTag  = cms.InputTag("reducedEcalRecHitsEE","","RECO"),
                                        )



##OUTPUT
process.TFileService = cms.Service("TFileService",
    fileName = cms.string("conversionTreeFiltered_MC.root"),
    closeFileFast = cms.untracked.bool(True),
    )


#process.load("PhysicsTools.NtupleUtils.AllPassFilter_cfi")

# Counter1: All read events
#process.AllEvents = process.AllPassFilter.clone()
# Counter2: Good Vertex
#process.ConvFilter = process.AllPassFilter.clone()
# Counter3: Conversion Filter
#process.GoodVertex = process.AllPassFilter.clone()

process.p = cms.Path(
 #   process.AllEvents* # -> Counter
    
    process.noscraping*
    process.primaryVertexFilter*
 #   process.GoodVertex* # -> Counter

    process.conversionFilter*
 #   process.ConvFilter* # -> Counter

    #process.trackerOnlyConversionSequence*
    process.conversionTree
    )



