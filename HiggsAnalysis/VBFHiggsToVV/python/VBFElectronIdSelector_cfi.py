import FWCore.ParameterSet.Config as cms



VBFIdSelectedElectrons = cms.EDFilter(
    "VBFElectronIdSelection",
    src = cms.InputTag("gsfElectrons"),
    srcElectronIdValues = cms.InputTag("eidRobustLoose"),
    doRefCheck = cms.bool(False),
    srcElectronsRef = cms.InputTag("gsfElectrons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFIdSelectedElectronsRef = cms.EDFilter(
    "VBFElectronIdSelectionRef",
    src = cms.InputTag("gsfElectrons"),
    srcElectronIdValues = cms.InputTag("eidRobustLoose"),
    doRefCheck = cms.bool(False),
    srcElectronsRef = cms.InputTag("gsfElectrons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



VBFIdSelectedElectronsSequence = cms.Sequence(
    VBFIdSelectedElectrons *
    VBFIdSelectedElectronsRef
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
