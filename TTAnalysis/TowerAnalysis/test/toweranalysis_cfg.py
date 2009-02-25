import FWCore.ParameterSet.Config as cms

process = cms.Process("TowerKiller")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'WARNINGS'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

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
process.EcalTTowerKiller.DeadChannelsFile = cms.string('/gwpool/users/sala/Installations/CMSSW_2_2_3/src/TTAnalysis/EgammaClusterProducers/data/DeadTowers_FED.txt')
   

### Change ECal input collection for reconstruction ###
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.towerMaker.ecalInputs = cms.VInputTag(cms.InputTag("EcalTTowerKiller","EcalRecHitsEB","TowerKiller")
                                              ,cms.InputTag("EcalTTowerKiller","EcalRecHitsEE","TowerKiller")
                                              )

process.TowerAnalysis = cms.EDAnalyzer('TowerAnalysis',
                              ecalRH_EB = cms.InputTag("EcalTTowerKiller","EcalRecHitsEB","TowerKiller" ),
                              ecalRH_EE = cms.InputTag("EcalTTowerKiller","EcalRecHitsEE","TowerKiller" ),
                              ecalInputs = cms.VInputTag(cms.InputTag("EcalTTowerKiller","EcalRecHitsEB","TowerKiller")
                                                         ,cms.InputTag("EcalTTowerKiller","EcalRecHitsEE","TowerKiller")
                                                         )
                              )

process.dump=cms.EDAnalyzer('EventContentAnalyzer')

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(
    'deadTowers_FED_Summer08-Zee-redigi.root'
    )
                                   )


process.p = cms.Path(
    process.EcalTTowerKiller*process.caloTowersRec
    *process.recoJets*process.metreco
    #+process.dump
    +process.TowerAnalysis
    )
