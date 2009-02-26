import FWCore.ParameterSet.Config as cms

process = cms.Process("TowerKiller")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'WARNINGS'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:/gwtera5/users/data/10TeV-RECO/Summer08-Zee-GEN-SIM-RECO-IDEAL_V11_redigi_v1-1.root'

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
#process.EcalTTowerKiller.hitCollection = cms.string("EcalRecHitsEB")
#process.EcalTTowerKiller.hitProducer = cms.string("ecalRecHit")
process.EcalTTowerKiller.EBCollection = cms.InputTag('ecalRecHit','EcalRecHitsEB')
process.EcalTTowerKiller.EECollection = cms.InputTag('ecalRecHit','EcalRecHitsEE')
process.EcalTTowerKiller.reducedHitCollection = cms.string('EcalRecHitsEB')
process.EcalTTowerKiller.rejectedHitCollection = cms.string('EcalRejRecHitsEB')
#process.EcalTTowerKiller.DeadChannelsFile = cms.string('/gwpool/users/sala/Installations/CMSSW_2_2_3/src/TTAnalysis/EgammaClusterProducers/data/DeadTowers_FED.txt')
process.EcalTTowerKiller.DeadChannelsFile = cms.string('/afs/cern.ch/user/a/amartell/scratch0/CMSSW_2_2_3/src/TTAnalysis/EgammaClusterProducers/data/DeadTowers_FED.txt')
   

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
process.pixelMatchGsfElectrons.endcapSuperClusters = cms.InputTag("correctedMulti5x5SuperClustersWithPreshower","ecalDrivenElectronSeeds", "TowerKiller")
process.pixelMatchGsfElectrons.reducedBarrelRecHitCollection = cms.InputTag("EcalTTowerKiller","EcalRecHitsEB")
process.pixelMatchGsfElectrons.reducedEndcapRecHitCollection = cms.InputTag("EcalTTowerKiller","EcalRecHitsEE")
process.photons.barrelEcalHits = "TowerKiller:EcalRecHitsEB"
process.photons.endcapEcalHits = "TowerKiller:EcalRecHitsEE"



process.TowerAnalysis = cms.EDAnalyzer('TowerAnalysis',
                              ecalRecH_EB = cms.InputTag("EcalTTowerKiller","EcalRecHitsEB", "TowerKiller"),
                              ecalRecH_EE = cms.InputTag("EcalTTowerKiller","EcalRecHitsEE", "TowerKiller"),
                              ecalRejH_EB = cms.InputTag("EcalTTowerKiller","EcalRejRecHitsEB", "TowerKiller"),
                              ecalRejH_EE = cms.InputTag("EcalTTowerKiller","EcalRejRecHitsEE", "TowerKiller"),
                              electronCollection = cms.InputTag("pixelMatchGsfElectrons::RECO"),
			      electronReducedCollection = cms.InputTag("pixelMatchGsfElectrons::TowerKiller"),
                              mcTruthCollection = cms.InputTag("source"),
			      Nbinxyz = cms.int32(50),
			      Etamin = cms.double(-2.5),
   			      Etamax = cms.double(2.5),
			      Nbineta = cms.int32(50),
			      Phimin = cms.double(-3.2),
			      Phimax = cms.double(3.2),
			      Nbinphi = cms.int32(64),
			      Pmin = cms.double(0.0),
			      Pmax = cms.double(300.0),
			      Nbinp = cms.int32(50),
			      Ptmin = cms.double(5.0),
			      Ptmax = cms.double(100.0),
			      Nbinpt = cms.int32(50),
			      Nbinpteff = cms.int32(19),
			      MaxPt = cms.double(100.0),
			      DeltaR = cms.double(0.05),
			      MaxAbsEta = cms.double(2.5),
                              outputFile = cms.string('gsfElectronHistos.root'),
                              )


process.dump=cms.EDAnalyzer('EventContentAnalyzer')

process.TFileService = cms.Service("TFileService",
        	                   fileName = cms.string(
				    'deadTowers_FED_Summer08-Zee-redigi.root'
					        	 )
                                   )


### Output file ###	
process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('file:/gwtera1/users/martelli/data/proveTT/myOutputFile.root'),
    outputCommands = cms.untracked.vstring(
	            'keep *'
        	#     'drop *',
                #    'keep CaloJets_*_*_*',
                #     'keep *_iterativeCone5CaloJets_*_*',
                #     'keep *_source_*_*',
                #     'keep *_*_*_HWWPreselection',
                #     'keep *_muons_*_*',
                #     'keep *_pixelMatchGsfElectrons_*_*',
                #     'keep *_pixelMatchGsfFit_*_*',
                #     'keep recoSuperClusters_*_*_*'
                   # 'keep *_correctedHybridSuperClusters_*_*',
                   # 'keep *_hybridSuperClusters_*_*',
                   # 'keep *_multi5x5SuperClustersWithPreshower_*_*'
                  		      )
                         )




process.p = cms.Path(
   process.EcalTTowerKiller*
 
  process.hybridClusteringSequence*
  process.multi5x5ClusteringSequence*
##  process.multi5x5PreshowerClusteringSequence*
##   process.ecalClusters *
    process.caloTowersRec *
    process.vertexreco *
    process.recoJets *
    process.metreco *
    process.siPixelRecHits * 
    process.siStripMatchedRecHits * 
    process.newSeedFromPairs * 
    process.newSeedFromTriplets * 
    process.newCombinedSeeds *
    process.electronSequence +
##    process.pixelMatchGsfElectronSequence
    #+process.dump
     process.TowerAnalysis

    )

process.outpath = cms.EndPath(process.out)

