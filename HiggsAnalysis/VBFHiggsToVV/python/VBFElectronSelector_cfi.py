import FWCore.ParameterSet.Config as cms



VBFSelectedElectrons = cms.EDFilter(
    "VBFElectronSelection",
    src = cms.InputTag("pixelMatchGsfElectrons"),
    ptMin  = cms.double(5.),
    etaMin = cms.double(-2.7),
    etaMax = cms.double(+2.7),
    doRefCheck = cms.bool(False),
    srcElectronsRef = cms.InputTag("pixelMatchGsfElectrons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFSelectedElectronsRef = cms.EDFilter(
    "VBFElectronSelectionRef",
    src = cms.InputTag("pixelMatchGsfElectrons"),
    ptMin  = cms.double(5.),
    etaMin = cms.double(-2.7),
    etaMax = cms.double(+2.7),
    doRefCheck = cms.bool(False),
    srcElectronsRef = cms.InputTag("pixelMatchGsfElectrons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFSelectedElectronsSequence = cms.Sequence(
    VBFSelectedElectrons *
    VBFSelectedElectronsRef
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 