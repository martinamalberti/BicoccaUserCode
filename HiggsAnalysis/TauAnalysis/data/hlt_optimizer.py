#bring in the cms configuration classes
import FWCore.ParameterSet.Config as cms

process = cms.Process('hlt_optimizer')

process.analysis = cms.EDAnalyzer('hlt_optimizer')

process.source = cms.Source("PoolSource", 
		fileNames = cms.untracked(cms.vstring('file:/afs/cern.ch/user/d/deguio/scratch0/CMSSW_1_3_1/src/HLTrigger/xchannel/test/EMuon-output.root')),
		maxEvents = cms.untracked(cms.int32(10))
		)

process.p = cms.Path( process.analysis )

process.add_(cms.Service("MessageLogger"))

process.dumpConfig()
