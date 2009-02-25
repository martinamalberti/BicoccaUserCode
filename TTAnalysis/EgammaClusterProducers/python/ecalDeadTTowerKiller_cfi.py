import FWCore.ParameterSet.Config as cms

EcalTTowerKiller = cms.EDProducer("EcalTTowerKiller",
                                  #hitProducer = cms.string('ecalRecHit'),
                                  #   hitProducer = cms.string('ecalTTowerKiller'),
                                  
                                  #hitCollection = cms.string('EcalRecHitsEB'),
                                  EBCollection = cms.InputTag('ecalRecHit','EcalRecHitsEB'),
                                  EECollection = cms.InputTag('ecalRecHit','EcalRecHitsEE'),
                                  ECollection = cms.VInputTag( cms.InputTag('ecalRecHit','EcalRecHitsEB') , cms.InputTag('ecalRecHit','EcalRecHitsEE')),
                                  reducedHitCollectionEB = cms.string('EcalRecHitsEB'),
                                  rejectedHitCollectionEB = cms.string('EcalRejRecHitsEB'),
                                  reducedHitCollectionEE = cms.string('EcalRecHitsEE'),
                                  rejectedHitCollectionEE = cms.string('EcalRejRecHitsEE'),
                                  
                                  DeadChannelsFile = cms.string('RecoEcal/EgammaClusterProducers/data/DeadTowers.txt')
                                  )

