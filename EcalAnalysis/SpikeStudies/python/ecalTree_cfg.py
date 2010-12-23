import FWCore.ParameterSet.Config as cms

process = cms.Process("ECALANALYSIS")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(10000) #10000


# Geometry
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
process.load("Geometry.CaloEventSetup.CaloGeometry_cff")

# Global Tag
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")

process.GlobalTag.globaltag = 'GR_R_38X_V15::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(500) )

process.source = cms.Source("PoolSource",
    skipEvents = cms.untracked.uint32(0),
    fileNames = cms.untracked.vstring(

           'file:/media/amassiro/deguio/Datasets/Electron_Run2010B-WZEG-Nov4Skim_v1_RAW-RECO.root'

        ## '/store/data/Run2010B/Electron/RAW-RECO/WZEG-Nov4Skim_v1/0167/F8EAC093-42EF-DF11-95E5-001A92971BB4.root',
        ## '/store/data/Run2010B/Electron/RAW-RECO/WZEG-Nov4Skim_v1/0167/F4A18D82-42EF-DF11-BE24-001A92810AE6.root',
        ## '/store/data/Run2010B/Electron/RAW-RECO/WZEG-Nov4Skim_v1/0167/F43A3376-42EF-DF11-9A6E-00261894393D.root',
        ## '/store/data/Run2010B/Electron/RAW-RECO/WZEG-Nov4Skim_v1/0167/F4324792-42EF-DF11-86A6-001A92810AD8.root',
        ## '/store/data/Run2010B/Electron/RAW-RECO/WZEG-Nov4Skim_v1/0167/F2A11CA0-42EF-DF11-8B77-0018F3D096DE.root',
        ## '/store/data/Run2010B/Electron/RAW-RECO/WZEG-Nov4Skim_v1/0167/F0D1AF84-42EF-DF11-AB70-001A92811736.root',
        ## '/store/data/Run2010B/Electron/RAW-RECO/WZEG-Nov4Skim_v1/0167/F0C94C8B-42EF-DF11-980F-001A928116BE.root',
        ## '/store/data/Run2010B/Electron/RAW-RECO/WZEG-Nov4Skim_v1/0167/F02C4D78-42EF-DF11-A868-003048678FA6.root',
        ## '/store/data/Run2010B/Electron/RAW-RECO/WZEG-Nov4Skim_v1/0167/EE5BC18B-42EF-DF11-AB9F-00248C55CC9D.root',
        ## '/store/data/Run2010B/Electron/RAW-RECO/WZEG-Nov4Skim_v1/0167/EE10208C-42EF-DF11-BC6C-0018F3D09710.root',
        ## '/store/data/Run2010B/Electron/RAW-RECO/WZEG-Nov4Skim_v1/0167/EABB5979-42EF-DF11-A4DC-001A92971B7E.root'            
    )
)

process.myanalysis = cms.EDAnalyzer('EcalTree',
                                    ebRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
                                    ebDigiCollection = cms.InputTag("ecalDigis","ebDigis"),
                                    #ebClusterCollection = cms.InputTag("hybridSuperClusters","hybridBarrelBasicClusters","RECO"),
                                    #ebClusterCollection = cms.InputTag("multi5x5BasicClusters","multi5x5BarrelBasicClusters","EGammaCommissioning"),

                                    eeRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
                                    eeDigiCollection = cms.InputTag("ecalDigis","eeDigis"),
                                    #eeClusterCollection = cms.InputTag("hybridSuperClusters","hybridEndcapBasicClusters","RECO"),
                                    #eeClusterCollection = cms.InputTag("multi5x5BasicClusters","multi5x5BarrelBasicClusters","EGammaCommissioning"),

                                    GsfEleTag = cms.InputTag("gsfElectrons"),
                                    L1InputTag =  cms.InputTag("gtDigis"),
                                    ak5CaloJets =  cms.InputTag("ak5CaloJets"),
                                    
                                    MetTag          = cms.InputTag("corMetGlobalMuons"),
                                    Type1MetTag     = cms.InputTag("VBFType1SisCone5CaloMet"),
                                    TcMetTag        = cms.InputTag("tcMet"),
                                    PFMetTag        = cms.InputTag("pfMet"),         

                                    minRecHitEnergy = cms.untracked.double(5.)  #5GeV
                                   
)


##SKIMMING

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


##OUTPUT
process.TFileService = cms.Service("TFileService",
    fileName = cms.string("EcalTree.root")
)

process.p = cms.Path(

    process.noscraping*
    process.primaryVertexFilter*
        
    process.ecalDigis*
    process.myanalysis
    )
