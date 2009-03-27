import FWCore.ParameterSet.Config as cms

process = cms.Process("TowerKiller")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'WARNING'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source("PoolSource",
                            # replace 'myfile.root' with the source file you want to use
                            fileNames = cms.untracked.vstring(
    #'/store/relval/CMSSW_2_2_4/RelValQCD_Pt_80_120/GEN-SIM-RECO/STARTUP_V8_v1/0000/148C3184-90F3-DD11-860E-001D09F2910A.root'
    #'file:/gwtera5/users/data/10TeV-RECO/Summer08-HerwigQCDPt15-GEN-SIM-RECO-IDEAL_V11_redigi_v1-1.root'
    #'file:/gwtera5/users/data/10TeV-RECO/Summer08-Zee-GEN-SIM-RECO-IDEAL_V11_redigi_v1-1.root'
    'file:/gwtera5/users/data/10TeV-RECO/PG/SingleElectronE50_cfi_py_RAW2DIGI_RECO-1.root'
    #'file:/gwtera2/users/sala/10-TeV/CMSSW_2_2_1-RelValWjet_Pt_3000_3500-GEN-SIM-DIGI-RAW-HLTDEBUG-IDEAL_V9_v2-1.root'
    #'file:/gwpool/users/sala/Installations/CMSSW_2_2_5/src/TTAnalysis/TowerAnalysis/test/SingleElectronE50_cfi_py_RAW2DIGI_RECO.root'
    
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
process.EcalTTowerKiller.histofile = cms.string('histos.root')



### Change ECal input collection for reconstruction ###
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.towerMaker.ecalInputs = cms.VInputTag(cms.InputTag("EcalTTowerKiller","EcalRecHitsEB","TowerKiller")
                                              ,cms.InputTag("EcalTTowerKiller","EcalRecHitsEE","TowerKiller")
                                              )


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



####### FOR THE CONVERSION OF TPG COMPRESSED ET
process.load("CalibCalorimetry.Configuration.Ecal_FakeConditions_cff")
#process.load("Configuration.StandardSequences.FakeConditions_cff")
process.load("CalibCalorimetry.EcalTrivialCondModules.EcalTrivialCondRetriever_cfi")



process.tpparams = cms.ESSource("EmptyESSource",
                                    recordName = cms.string('EcalTPGLinearizationConstRcd'),
                                    iovIsRunNotTime = cms.bool(True),
                                    firstValid = cms.vuint32(1)
                                )

process.tpparams2 = cms.ESSource("EmptyESSource",
                                     recordName = cms.string('EcalTPGPedestalsRcd'),
                                     iovIsRunNotTime = cms.bool(True),
                                     firstValid = cms.vuint32(1)
                                 )

process.tpparams3 = cms.ESSource("EmptyESSource",
                                     recordName = cms.string('EcalTPGSlidingWindowRcd'),
                                     iovIsRunNotTime = cms.bool(True),
                                     firstValid = cms.vuint32(1)
                                 )

process.tpparams4 = cms.ESSource("EmptyESSource",
                                     recordName = cms.string('EcalTPGWeightIdMapRcd'),
                                     iovIsRunNotTime = cms.bool(True),
                                     firstValid = cms.vuint32(1)
                                 )

process.tpparams5 = cms.ESSource("EmptyESSource",
                                     recordName = cms.string('EcalTPGWeightGroupRcd'),
                                     iovIsRunNotTime = cms.bool(True),
                                     firstValid = cms.vuint32(1)
                                 )

process.tpparams6 = cms.ESSource("EmptyESSource",
                                     recordName = cms.string('EcalTPGLutGroupRcd'),
                                     iovIsRunNotTime = cms.bool(True),
                                     firstValid = cms.vuint32(1)
                                 )

process.tpparams7 = cms.ESSource("EmptyESSource",
                                     recordName = cms.string('EcalTPGLutIdMapRcd'),
                                     iovIsRunNotTime = cms.bool(True),
                                     firstValid = cms.vuint32(1)
                                 )

process.tpparams8 = cms.ESSource("EmptyESSource",
                                     recordName = cms.string('EcalTPGFineGrainEBIdMapRcd'),
                                     iovIsRunNotTime = cms.bool(True),
                                     firstValid = cms.vuint32(1)
                                 )

process.tpparams9 = cms.ESSource("EmptyESSource",
                                     recordName = cms.string('EcalTPGFineGrainEBGroupRcd'),
                                     iovIsRunNotTime = cms.bool(True),
                                     firstValid = cms.vuint32(1)
                                 )

process.tpparams10 = cms.ESSource("EmptyESSource",
                                      recordName = cms.string('EcalTPGFineGrainStripEERcd'),
                                      iovIsRunNotTime = cms.bool(True),
                                      firstValid = cms.vuint32(1)
                                  )

process.tpparams11 = cms.ESSource("EmptyESSource",
                                      recordName = cms.string('EcalTPGFineGrainTowerEERcd'),
                                      iovIsRunNotTime = cms.bool(True),
                                      firstValid = cms.vuint32(1)
                                  )

process.tpparams12 = cms.ESSource("EmptyESSource",
                                      recordName = cms.string('EcalTPGPhysicsConstRcd'),
                                      iovIsRunNotTime = cms.bool(True),
                                      firstValid = cms.vuint32(1)
                                  )

process.EcalTrigPrimESProducer = cms.ESProducer("EcalTrigPrimESProducer",
                                                    DatabaseFile = cms.untracked.string('TPG_startup.txt.gz')
                                                )


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
    'test.root'
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




  
