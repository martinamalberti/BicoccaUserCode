import FWCore.ParameterSet.Config as cms

process = cms.Process("TowerKiller")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'WARNING'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
                            # replace 'myfile.root' with the source file you want to use
                            fileNames = cms.untracked.vstring(
    #'/store/relval/CMSSW_2_2_4/RelValQCD_Pt_80_120/GEN-SIM-RECO/STARTUP_V8_v1/0000/148C3184-90F3-DD11-860E-001D09F2910A.root'
    #'file:/gwtera5/users/data/10TeV-RECO/Summer08-HerwigQCDPt15-GEN-SIM-RECO-IDEAL_V11_redigi_v1-1.root'
    #'file:/gwtera5/users/data/10TeV-RECO/Summer08-Zee-GEN-SIM-RECO-IDEAL_V11_redigi_v1-1.root'
    #'file:/gwtera5/users/data/10TeV-RECO/PhysVal-DiElectron-Ene50-10k.root'
    'file:/gwtera5/users/data/10TeV-RECO/PhysVal-DiElectron-Ene50-phi0_1-eta-0.5_0.5.root'
    
    )
                            )


### Some standard sequences needed for reconstruction ###
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

### BE CAREFUL WITH CONDITIONS!
#process.GlobalTag.globaltag = cms.string('IDEAL_V9::All')
process.GlobalTag.globaltag = cms.string('IDEAL_V11::All')
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

### Make new ECal RecHit collection ###
process.load("TTAnalysis.EgammaClusterProducers.ecalDeadTTowerKiller_cfi")
process.EcalTTowerKiller.EBCollection = cms.InputTag('ecalRecHit','EcalRecHitsEB')
process.EcalTTowerKiller.EECollection = cms.InputTag('ecalRecHit','EcalRecHitsEE')
process.EcalTTowerKiller.reducedHitCollection = cms.string('EcalRecHitsEB')
process.EcalTTowerKiller.rejectedHitCollection = cms.string('EcalRejRecHitsEB')
process.EcalTTowerKiller.DeadChannelsFile = cms.string('TTAnalysis/EgammaClusterProducers/data/DeadTowers_FED.txt')


### Change ECal input collection for reconstruction ###
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.towerMaker.ecalInputs = cms.VInputTag(cms.InputTag("EcalTTowerKiller","EcalRecHitsEB","TowerKiller")
                                              ,cms.InputTag("EcalTTowerKiller","EcalRecHitsEE","TowerKiller")
                                              )

# PAT Layer 0+1
#process.load("PhysicsTools.PatAlgos.patLayer0_cff")
#process.load("PhysicsTools.PatAlgos.patLayer1_cff")

### Taking the new collections
#process.allLayer0Jets.jetSource = cms.InputTag("iterativeCone5CaloJets","","TowerKiller")
#process.allLayer0METs.metSource = cms.InputTag('met','','TowerKiller')
#process.allLayer0Electrons.electronSource = cms.InputTag("pixelMatchGsfElectrons","","TowerKiller")

# to use the modified collections in clustering sequences (RecoEcal_cff)
##hybridClusteringSequence
process.hybridSuperClusters.ecalhitproducer = cms.string("EcalTTowerKiller")
process.correctedHybridSuperClusters.recHitProducer = cms.InputTag("EcalTTowerKiller:EcalRecHitsEB")
##multi5x5ClusteringSequence
process.multi5x5BasicClusters.barrelHitProducer = cms.string("EcalTTowerKiller")
process.multi5x5BasicClusters.endcapHitProducer = cms.string("EcalTTowerKiller")
process.interestingEcalDetIdEB.recHitsLabel = "EcalTTowerKiller:EcalRecHitsEB"
process.reducedEcalRecHitsEB.recHitsLabel = "EcalTTowerKiller:EcalRecHitsEB"
##process.multi5x5SuperClusters
#process.correctedMulti5x5SuperClustersWithPreshower.recHitProducer = cms.InputTag("EcalTTowerKiller:EcalRecHitsEE")
#process.interestingEcalDetIdEE.recHitsLabel = "EcalTTowerKiller:EcalRecHitsEE"
#process.reducedEcalRecHitsEE.recHitsLabel = "EcalTTowerKiller:EcalRecHitsEE"

##process.multi5x5PreshowerClusterShape.preshRecHitProducer = cms.InputTag("ecalPreshowerRecHit","EcalRecHitsES")
# to use the modified collections in egamma sequences (RecoEgamma_cff)
process.pixelMatchGsfElectrons.barrelSuperClusters = cms.InputTag("correctedHybridSuperClusters","ecalDrivenElectronSeeds", "TowerKiller")
process.pixelMatchGsfElectrons.endcapSuperClusters = cms.InputTag("correctedMulti5x5SuperClustersWithPreshower","ecalDrivenElectronSeeds","TowerKiller")
process.pixelMatchGsfElectrons.reducedBarrelRecHitCollection = cms.InputTag("EcalTTowerKiller","EcalRecHitsEB")
process.pixelMatchGsfElectrons.reducedEndcapRecHitCollection = cms.InputTag("EcalTTowerKiller","EcalRecHitsEE")
process.photons.barrelEcalHits = "TowerKiller:EcalRecHitsEB"
process.photons.endcapEcalHits = "TowerKiller:EcalRecHitsEE"


### Analysis
process.TowerAnalysis = cms.EDAnalyzer('TowerAnalysis',
                                       jetLabel = cms.untracked.VInputTag(cms.InputTag("iterativeCone5CaloJets")
                                                                ,cms.InputTag("iterativeCone5CaloJets","","TowerKiller")
                                                                ),
                                       genJetLabel = cms.untracked.InputTag("iterativeCone5GenJets"),
                                       metLabel = cms.untracked.VInputTag(cms.InputTag("met::RECO")
                                                                          ,cms.InputTag("met::TowerKiller")
                                                                          ),
                                       genMetLabel = cms.untracked.InputTag("genMet"),
                                       eleLabel = cms.untracked.VInputTag(cms.InputTag("pixelMatchGsfElectrons::RECO")
                                                                          ,cms.InputTag("pixelMatchGsfElectrons::TowerKiller")
                                                                          ),
                                       mcLabel = cms.untracked.InputTag("source"),
                                       DeltaR = cms.double(0.3),
                                       DEBUG = cms.untracked.int32(1)
                                       )


## Necessary fixes to run 2.2.X on 2.1.X data
#from PhysicsTools.PatAlgos.tools.cmsswVersionTools import run22XonSummer08AODSIM
#run22XonSummer08AODSIM(process)

process.dump=cms.EDAnalyzer('EventContentAnalyzer')

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(
    'allDead_PG_test_50-10k.root'
            )
                                   )



process.p = cms.Path(
    process.EcalTTowerKiller*
    process.hybridClusteringSequence*
    process.multi5x5ClusteringSequence*
    process.caloTowersRec*
    process.vertexreco *
    process.recoJets*
    process.metreco*
    process.siPixelRecHits *
    process.siStripMatchedRecHits *
    process.newSeedFromPairs *
    process.newSeedFromTriplets *
    process.newCombinedSeeds *
    process.electronSequence +
    #process.dump
    #+ process.content # uncomment to get a dump of the output after layer 0
    process.TowerAnalysis
    )




  
