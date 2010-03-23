import FWCore.ParameterSet.Config as cms

process = cms.Process("SelectionsTest")
process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.options = cms.untracked.PSet(
        wantSummary = cms.untracked.bool(True)
        )

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)

readFiles.extend( [
#	'/store/relval/CMSSW_3_1_0_pre8/RelValZEE/ALCARECO/STARTUP_31X_StreamALCARECOEcalCalElectron_v1/0006/C2CF2E13-DB4D-DE11-A5B1-001D09F2437B.root'
     '/store/relval/CMSSW_3_5_4/RelValZEE/ALCARECO/START3X_V24_EcalCalElectron-v1/0004/2AB6B40D-2D2C-DF11-814B-002618943948.root'	
        ] )


process.alCaSuperClustersTest = cms.EDAnalyzer("AlCaSuperClustersTest",
  EB_SC         = cms.inputTag ("hybridSuperClusters"),
  EB_SC_corr    = cms.inputTag ("correctedHybridSuperClusters"),
  EE_SC         = cms.inputTag ("multi5x5SuperClusters"),
  EE_SC_ES      = cms.inputTag ("multi5x5SuperClustersWithPreshower"),
  EE_SC_corr_ES = cms.inputTag ("correctedMulti5x5SuperClustersWithPreshower"),
  ES_SC         = cms.inputTag ("FIXME"),
  HF_SC         = cms.inputTag ("hfEMClusters"),
  PF_SC         = cms.inputTag ("pfElectronTranslator"),
  merge_SC      = cms.inputTag ("superClusterMerger"),
  electronLabel = cms.InputTag("gsfElectrons"),
  HistOutFile   = cms.untracked.string("alCaSuperClustersTest.root")
  )

process.path = cms.Path(process.alCaSuperClustersTest)


