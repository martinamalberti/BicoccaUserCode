import FWCore.ParameterSet.Config as cms

isolatedEElectrons = cms.EDFilter("VBFEleHETIsolation",
                                 src = cms.InputTag("pixelMatchGsfElectrons"),
                                 eleTkIso = cms.InputTag("electronTrackIsolationScone"),
                                 eleEcalIso = cms.InputTag("electronEcalRecHitIsolationScone"),
                                 eleHcalIso = cms.InputTag("electronHcalDepth1TowerIsolationScone"),
#                                  eleTkIso = cms.InputTag("eleIsoFromDepsTk"),
#                                  eleEcalIso = cms.InputTag("eleIsoFromDepsEcalFromHits"),
#                                  eleHcalIso = cms.InputTag("eleIsoFromDepsHcalFromHits"), 
                                 useCombinedCuts = cms.bool(True),
                                 thresholdCombined = cms.double(30), 
                                 thresholdTk = cms.double(99999), 
                                 thresholdEcal = cms.double(99999), 
                                 thresholdHcal = cms.double(99999), 
                                 coefTk = cms.double(0), 
                                 coefEcal = cms.double(1.5), 
                                 coefHcal = cms.double(0),
                                 electronIdCutsLabel = cms.InputTag("egammaIDCutsLoose")
#                                  electronIdCutsLabel = cms.InputTag("eidClassLoose")
                                 )
