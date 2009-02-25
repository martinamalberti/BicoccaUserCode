import FWCore.ParameterSet.Config as cms 


from TTAnalysis.EgammaClusterProducers.ecalDeadTTowerKiller_cfi import *

ecalDeadTTowerKillerSequence = cms.Sequence(EcalTTowerKiller)

