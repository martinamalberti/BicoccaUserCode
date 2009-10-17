import FWCore.ParameterSet.Config as cms



VBFIdSelectedElectrons = cms.EDFilter(
    "VBFElectronIdSelection",
    src = cms.InputTag("pixelMatchGsfElectrons"),
    srcElectronIdValues = cms.InputTag("eidRobustLoose"),
    doRefCheck = cms.bool(False),
    srcElectronsRef = cms.InputTag("pixelMatchGsfElectrons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFIdSelectedElectronsRef = cms.EDFilter(
    "VBFElectronIdSelectionRef",
    src = cms.InputTag("pixelMatchGsfElectrons"),
    srcElectronIdValues = cms.InputTag("eidRobustLoose"),
    doRefCheck = cms.bool(False),
    srcElectronsRef = cms.InputTag("pixelMatchGsfElectrons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



VBFIdSelectedElectronsSequence = cms.Sequence(
    VBFIdSelectedElectrons *
    VBFIdSelectedElectronsRef
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
