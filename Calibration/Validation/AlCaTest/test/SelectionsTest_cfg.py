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


process.selectionsTest = cms.EDAnalyzer("SelectionsTest",
  electronLabel=cms.InputTag("gsfElectrons"),
  HistOutFile = cms.untracked.string("selectionsTest.root"),
  ESCOPinMin=cms.double(0.9),
  ESCOPinMax=cms.double(1.2),
  ESeedOPoutMin=cms.double(0.1),
  ESeedOPoutMax=cms.double(9),
  PinMPoutOPinMin=cms.double(-0.1),
  PinMPoutOPinMax=cms.double(0.5),
  EMPoutMin=cms.double(0.9),
  EMPoutMax=cms.double(1.2)
  )

process.path = cms.Path(process.selectionsTest)


