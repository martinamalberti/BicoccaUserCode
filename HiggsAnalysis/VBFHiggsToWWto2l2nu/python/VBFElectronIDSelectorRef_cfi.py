import FWCore.ParameterSet.Config as cms

selectedIDElectronsRef = cms.EDFilter("VBFEleIDSelectionRef",
                                 src = cms.InputTag("pixelMatchGsfElectrons"),
                                 electronIdCutsLabel = cms.InputTag("egammaIDCutsLoose"),
                                 electronIdLikelihoodLabel = cms.InputTag("egammaIDLikelihood"),
                                 useCuts = cms.bool(True) #if true than use electronIdCutsLabel as cut
                                 # likelihoodThreshold_ = cms.double(0.5)
                                 )


