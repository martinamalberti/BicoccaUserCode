import FWCore.ParameterSet.Config as cms

process = cms.Process("ECALANALYSIS")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)


# Geometry
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
process.load("Geometry.CaloEventSetup.CaloGeometry_cff")

# Global Tag
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")

#process.GlobalTag.globaltag = 'GR_R_35X_V7A::All'
process.GlobalTag.globaltag = 'GR_R_36X_V12::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )

process.source = cms.Source("PoolSource",
    skipEvents = cms.untracked.uint32(0),
    fileNames = cms.untracked.vstring(

    #/EG/Run2010A-PromptReco-v4/RECO
    '/store/data/Run2010A/EG/RECO/v4/000/139/529/2E54C0AB-1589-DF11-8B63-001D09F23A20.root',
    '/store/data/Run2010A/EG/RECO/v4/000/139/466/54D5B3CF-B688-DF11-A0FF-000423D94E70.root',
    '/store/data/Run2010A/EG/RECO/v4/000/139/459/ECA0E034-D088-DF11-B34E-0030487A18A4.root',
    '/store/data/Run2010A/EG/RECO/v4/000/139/459/CA9AF9F7-CB88-DF11-BACD-001D09F29524.root',
    '/store/data/Run2010A/EG/RECO/v4/000/139/459/8C531733-D088-DF11-B6D2-0030487C6090.root',
    '/store/data/Run2010A/EG/RECO/v4/000/139/458/F22FB620-BB88-DF11-94A5-001617C3B79A.root',
    '/store/data/Run2010A/EG/RECO/v4/000/139/458/DEC78624-C288-DF11-8CA9-003048F118DE.root',
    '/store/data/Run2010A/EG/RECO/v4/000/139/458/DC447D23-BB88-DF11-9EBB-001617C3B70E.root',
    '/store/data/Run2010A/EG/RECO/v4/000/139/458/B891155D-C588-DF11-B89C-003048F024FE.root'
    

    #/MinimumBias/Commissioning10-ECALRECHIT-May27thSkim_v5/RAW-RECO
    #'/store/data/Commissioning10/MinimumBias/RAW-RECO/ECALRECHIT-May27thSkim_v5/0023/5C35650D-C672-DF11-A649-0018F3D0960A.root',
    #'/store/data/Commissioning10/MinimumBias/RAW-RECO/ECALRECHIT-May27thSkim_v5/0023/F4B02E12-C672-DF11-B457-001A928116C2.root',
    #'/store/data/Commissioning10/MinimumBias/RAW-RECO/ECALRECHIT-May27thSkim_v5/0023/FE382A98-C672-DF11-971F-00261894392B.root'

            
    )
)

process.myanalysis = cms.EDAnalyzer('EcalTree',
                                    ebRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
                                    ebClusterCollection = cms.InputTag("hybridSuperClusters","hybridBarrelBasicClusters","RECO"),
                                    #ebClusterCollection = cms.InputTag("multi5x5BasicClusters","multi5x5BarrelBasicClusters","EGammaCommissioning"),
                                    ebDigiCollection = cms.InputTag("ecalDigis","ebDigis"),
                                    L1InputTag =  cms.InputTag("gtDigis"),
                                    ak5CaloJets =  cms.InputTag("ak5CaloJets"),
                                    
                                    MetTag          = cms.InputTag("corMetGlobalMuons"),
                                    Type1MetTag     = cms.InputTag("VBFType1SisCone5CaloMet"),
                                    TcMetTag        = cms.InputTag("tcMet"),
                                    PFMetTag        = cms.InputTag("pfMet"),         

                                    minRecHitEnergy = cms.untracked.double(3.)
                                   
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

#Good Vertex Filter (see GOODCOLL skim)
process.primaryVertexFilter = cms.EDFilter("VertexSelector",
                                           src = cms.InputTag("offlinePrimaryVertices"),
                                           cut = cms.string("!isFake && ndof > 4 && abs(z) <= 15 && position.Rho <= 2"),
                                           filter = cms.bool(True)
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
    fileName = cms.string("SpikesCommissioning_May27thReReco_Run2010A-ECALRECHIT.root")
)

process.p = cms.Path(

    process.skimming*
    process.goodcollisions*
        
    process.ecalDigis*
    process.myanalysis
    )
