import FWCore.ParameterSet.Config as cms



DumpJEC = cms.EDAnalyzer(
    "DumpJEC",
    
    etaMin = cms.untracked.double(-5.05),
    etaMax = cms.untracked.double(+5.05),
    nBinsEta = cms.untracked.int32(101),

    ptMin = cms.untracked.double(-0.5),
    ptMax = cms.untracked.double(1000.5),
    nBinsPt = cms.untracked.int32(1001),

    outFileName = cms.untracked.string("JECUncertainty_START42_V13.txt")
)
