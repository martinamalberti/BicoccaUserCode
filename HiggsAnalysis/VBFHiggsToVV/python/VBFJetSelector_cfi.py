import FWCore.ParameterSet.Config as cms



VBFSelectedJets = cms.EDFilter(
    "VBFJetSelection",
    src = cms.InputTag("jets"),
    etMin  = cms.double(30.),
    etaMin = cms.double(-5.0),
    etaMax = cms.double(+5.0),
    doRefCheck = cms.bool(False),
    srcJetsRef = cms.InputTag("jetsRef")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFSelectedJetsRef = cms.EDFilter(
    "VBFJetSelectionRef",
    src = cms.InputTag("jets"),
    etMin  = cms.double(30.),
    etaMin = cms.double(-5.0),
    etaMax = cms.double(+5.0),
    doRefCheck = cms.bool(False),
    srcJetsRef = cms.InputTag("jetsRef")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



VBFSelectedJetsSequence = cms.Sequence(
    VBFSelectedJets *
    VBFSelectedJetsRef
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
