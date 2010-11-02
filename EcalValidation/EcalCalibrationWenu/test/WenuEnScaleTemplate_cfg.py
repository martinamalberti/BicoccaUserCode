import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

process.inputTree = cms.PSet(
  treeNameDATA = cms.string("ntupleEcalCalibration/myTree"),
  inputFileDATA = cms.string("test/EcalCalibration.root"),
  treeNameMC = cms.string("myTree"),
  inputFileMC = cms.string("test/Wenu_WSUMMED_4analysis.root")
)

process.outputTree = cms.PSet(
  outputFile = cms.string("out.root")
)


process.options = cms.PSet(
  EEorEB = cms.int32(5),
    
  numToyMC = cms.int32(10),
  MinScan = cms.double(-0.1),
  MaxScan = cms.double(0.05),
  iNoSteps = cms.int32(200),

  MinScanRange = cms.double(-0.04),
  MaxScanRange = cms.double(0.01),

  cut = cms.string("(eleFBrem<0.1)"),

  minET = cms.double(20.),

  variableName = cms.string("(E5x5/(pIn-eleES))"),
  minBINS      = cms.double(0.8),
  maxBINS      = cms.double(2.2),
  numBINS      = cms.int32(30)
)


