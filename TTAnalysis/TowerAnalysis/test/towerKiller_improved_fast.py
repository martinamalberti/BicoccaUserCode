import FWCore.ParameterSet.Config as cms

process = cms.Process("TowerKiller")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'WARNING'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(2000) )

process.source = cms.Source("PoolSource",
                            # replace 'myfile.root' with the source file you want to use
                            fileNames = cms.untracked.vstring(
    #'/store/relval/CMSSW_2_2_4/RelValQCD_Pt_80_120/GEN-SIM-RECO/STARTUP_V8_v1/0000/148C3184-90F3-DD11-860E-001D09F2910A.root'
    #'file:/gwtera5/users/data/10TeV-RECO/Summer08-HerwigQCDPt15-GEN-SIM-RECO-IDEAL_V11_redigi_v1-1.root'
    #'file:/gwtera5/users/data/10TeV-RECO/Summer08-Zee-GEN-SIM-RECO-IDEAL_V11_redigi_v1-1.root'
    #'file:/gwtera5/users/data/10TeV-RECO/PhysVal-DiElectron-Ene50-10k.root'
    'file:/gwtera5/users/data/10TeV-RECO/FAST_PhysVal-DiElectron-Ene50-phi0_1-eta-0.4_0.2-10k.root'
    
    )
                            )


### Some standard sequences needed for reconstruction ###
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

### BE CAREFUL WITH CONDITIONS!
#process.GlobalTag.globaltag = cms.string('IDEAL_V9::All')
#process.GlobalTag.globaltag = cms.string('IDEAL_V11::All')
#process.load("Configuration.StandardSequences.Geometry_cff")
#process.load("Configuration.StandardSequences.MagneticField_cff")

### Make new ECal RecHit collection ###
process.load("TTAnalysis.EgammaClusterProducers.ecalDeadTTowerKiller_cfi")
process.EcalTTowerKiller.ECollection = cms.VInputTag( cms.InputTag('caloRecHits','EcalRecHitsEB') , cms.InputTag('caloRecHits','EcalRecHitsEE'))
process.EcalTTowerKiller.DeadChannelsFile = cms.string('TTAnalysis/EgammaClusterProducers/data/DeadTowers_FED.txt')


### Change ECal input collection for reconstruction ###
#process.load("Configuration.FastSimulation.FamosSequences_cff")
#process.load("Configuration.StandardSequences.MagneticField_40T_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True

process.load("FastSimulation/Configuration/RandomServiceInitialization_cff")
process.load("FastSimulation/Configuration/CommonInputs_cff")
process.load("FastSimulation/Configuration/FamosSequences_cff")

process.GlobalTag.globaltag = cms.string('IDEAL_V11::All')

process.hybridSuperClusters.ecalhitproducer = 'EcalTTowerKiller'
process.correctedHybridSuperClusters.recHitProducer = cms.InputTag("EcalTTowerKiller","EcalRecHitsEB")
#process.correctedMulti5x5SuperClustersWithPreshower.preshRecHitProducer = cms.InputTag("EcalTTowerKiller","EcalRecHitsES")
process.reducedEcalRecHitsEB.recHitsLabel = cms.InputTag("caloRecHits","EcalRecHitsEB","TowerKiller")
#process.reducedEcalRecHitsEE.recHitsLabel = cms.InputTag("caloRecHits","EcalRecHitsEE","TowerKiller")
process.interestingEcalDetIdEB.recHitsLabel = cms.InputTag("caloRecHits","EcalRecHitsEB","TowerKiller")
#process.interestingEcalDetIdEE.recHitsLabel = cms.InputTag("caloRecHits","EcalRecHitsEE","TowerKiller")
process.multi5x5BasicClusters.barrelHitProducer = 'EcalTTowerKiller'
#process.multi5x5BasicClusters.endcapHitProducer = 'EcalTTowerKiller'
#process.multi5x5PreshowerClusterShape.preshRecHitProducer = cms.InputTag("caloRecHits","EcalRecHitsES")
#process.multi5x5SuperClustersWithPreshower.preshRecHitProducer = cms.InputTag("caloRecHits","EcalRecHitsES")
#process.correctedMulti5x5SuperClustersWithPreshower.recHitProducer = cms.InputTag("caloRecHits","EcalRecHitsEE")

process.towerMaker.ecalInputs = cms.VInputTag(cms.InputTag("EcalTTowerKiller","EcalRecHitsEB","TowerKiller")
                                              ,cms.InputTag("EcalTTowerKiller","EcalRecHitsEE","TowerKiller")
                                              )

process.calotoweroptmaker.ecalInputs = cms.VInputTag(
    cms.InputTag("caloRecHits","EcalRecHitsEB","TowerKiller"),
    cms.InputTag("caloRecHits","EcalRecHitsEE","TowerKiller")
    )

process.pixelMatchGsfElectrons.hcalRecHits = 'caloRecHits'
process.pixelMatchGsfElectrons.barrelSuperClusters = cms.InputTag("correctedHybridSuperClusters","electronGSPixelSeeds")
process.pixelMatchGsfElectrons.endcapSuperClusters = cms.InputTag("correctedEndcapSuperClustersWithPreshower","electronGSPixelSeeds")



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
    #process.famosWithJets*
    #process.famosWithElectrons*
    process.caloRecHits*
    process.ecalClusters*
    process.caloTowersRec*
#    caloJetMetGen*
    process.caloJetMet*
    process.siTrackerGaussianSmearingRecHits*   
    process.famosElectronSequence*
    #process.dump
    #+ process.content # uncomment to get a dump of the output after layer 0
    process.TowerAnalysis
    )




  
