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
#process.GlobalTag.globaltag = 'GR10_P_V4::All'
process.GlobalTag.globaltag = 'GR_R_35X_V7A::All'


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(3000) )

process.source = cms.Source("PoolSource",
    skipEvents = cms.untracked.uint32(0),
    fileNames = cms.untracked.vstring(
    
    #/MinimumBias/Commissioning10-GOODCOLL-v8/RAW-RECO
    #'/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/133/532/EC93873A-D74B-DF11-A1B9-00E08179185D.root',
    #'/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/133/531/D6E1CE68-ED4B-DF11-A676-003048D45F84.root'
    #'/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/133/529/223C34BD-EC4B-DF11-9CA6-003048D476D4.root',
    #'/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/133/526/B28AEED6-E94B-DF11-9124-00E08178C103.root',
    #'/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/133/521/C0EFDC20-024C-DF11-A82A-00E08178C155.root',
    #'/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/133/518/C620697E-B94B-DF11-A125-003048D46090.root',
    #'/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/133/516/CA61DFDB-E14B-DF11-9193-003048D476B0.root'
    
    #/MinimumBias/Commissioning10-Apr20Skim_GOODCOLL-v1/RAW-RECO

        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/FE4698E5-174E-DF11-874A-002354EF3BD0.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/FC05D135-184E-DF11-A0EF-003048679076.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/F27F730F-184E-DF11-8ABB-0030486792BA.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/EECC5D0E-184E-DF11-8C9A-00248C0BE005.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/E2E06661-184E-DF11-ACD5-00261894389E.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/D8FB05E4-174E-DF11-835A-002618943919.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/C623360C-184E-DF11-934C-0030486792B8.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/C4FA3235-184E-DF11-87D4-00261894388F.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/B4BBF00C-184E-DF11-9AF3-002618943849.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/B2E31A0D-184E-DF11-871C-003048678ED4.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/9ADB85E5-174E-DF11-93CF-002618943985.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/86315E61-184E-DF11-B547-002354EF3BE2.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/7EBAAFE4-174E-DF11-9695-00261894388D.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/76074C35-184E-DF11-99C8-0026189438A9.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/74BA60E3-174E-DF11-8728-003048679048.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/74A7440D-184E-DF11-838C-0026189438A5.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/72E74735-184E-DF11-906E-002618943908.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/725AACE5-174E-DF11-988F-00261894394D.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/723B2068-184E-DF11-BF15-003048678B8E.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/7040F611-184E-DF11-8608-0030486792BA.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/60037A35-184E-DF11-B06E-0026189438E9.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/3C30120D-184E-DF11-9966-002354EF3BD2.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/3A66C935-184E-DF11-B686-002618943849.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/300ADFE5-174E-DF11-9374-0030486790B8.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/26D2533F-184E-DF11-B998-0030486792BA.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/26322135-184E-DF11-BE50-00248C0BE018.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/2249B835-184E-DF11-AB18-00304867929E.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/02126D1E-184E-DF11-9390-00261894396F.root'
            
    )
)
## process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange(
##     '132440:157-132440:378',
##     '132596:382-132596:382',
##     '132596:447-132596:447',
##     '132598:174-132598:176',
##     '132599:1-132599:379',
##     '132599:381-132599:437',
##     '132601:1-132601:207',
##     '132601:209-132601:259',
##     '132601:261-132601:1107',
##     '132602:1-132602:70',
##     '132605:1-132605:444',
##     '132605:446-132605:522',
##     '132605:526-132605:622',
##     '132605:624-132605:814',
##     '132605:816-132605:829',
##     '132605:831-132605:867',
##     '132605:896-132605:942',
##     '132606:1-132606:26',
##     '132656:1-132656:111',
##     '132658:1-132658:51',
##     '132658:56-132658:120',
##     '132658:127-132658:148',
##     '132659:1-132659:76',
##     '132661:1-132661:116',
##     '132662:1-132662:9',
##     '132662:25-132662:74',
##     '132716:220-132716:436',
##     '132716:440-132716:487',
##     '132716:491-132716:586',
##     '132959:326-132959:334',
##     '132960:1-132960:124',
##     '132961:1-132961:222',
##     '132961:226-132961:230',
##     '132961:237-132961:381',
##     '132965:1-132965:68',
##     '132968:1-132968:67',
##     '132968:75-132968:169',
##     '133029:101-133029:115',
##     '133029:129-133029:332',
##     '133031:1-133031:18',
##     '133034:132-133034:287',
##     '133035:1-133035:63',
##     '133035:67-133035:302',
##     '133036:1-133036:222',
##     '133046:1-133046:43',
##     '133046:45-133046:210',
##     '133046:213-133046:227',
##     '133046:229-133046:323',
##     '133158:65-133158:786',
##     '133321:1-133321:383',
##     '133446:105-133446:266',
##     '133448:1-133448:484',
##     '133450:1-133450:329',
##     '133450:332-133450:658',
##     '133474:1-133474:95',
##     '133483:94-133483:159',
##     '133483:161-133483:591',
##     '133874:166-133874:297',
##     '133874:299-133874:721',
##     '133874:724-133874:814',
##     '133874:817-133874:864',
##     '133875:1-133875:20',
##     '133875:22-133875:37',
##     '133876:1-133876:315',
##     '133877:1-133877:77',
##     '133877:82-133877:104',
##     '133877:113-133877:231',
##     '133877:236-133877:294',
##     '133877:297-133877:437',
##     '133877:439-133877:622',
##     '133877:624-133877:853',
##     '133877:857-133877:1472',
##     '133877:1474-133877:1640',
##     '133877:1643-133877:1931',
##     '133881:1-133881:71',
##     '133881:74-133881:223',
##     '133881:225-133881:551',
##     '133885:1-133885:132',
##     '133885:134-133885:728',
##     '133927:1-133927:44',
##     '133928:1-133928:645',
##     '135059:59-135059:67',
##     )



process.myanalysis = cms.EDAnalyzer('EcalTree',
    ebRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
    ebClusterCollection = cms.InputTag("hybridSuperClusters","hybridBarrelBasicClusters","RECO"),
    #ebClusterCollection = cms.InputTag("multi5x5BasicClusters","multi5x5BarrelBasicClusters","EGammaCommissioning"),
    ebDigiCollection = cms.InputTag("ecalDigis","ebDigis"),
    L1InputTag =  cms.InputTag("gtDigis"),
    ak5CaloJets =  cms.InputTag("ak5CaloJets"),

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
    fileName = cms.string("SpikesCommissioning2010_GOODCOLL_V9.root")
)

process.p = cms.Path(

    process.skimming*
    process.goodcollisions*
        
    process.ecalDigis*
    process.myanalysis
    )
