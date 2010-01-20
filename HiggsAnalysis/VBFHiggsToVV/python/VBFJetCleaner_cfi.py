import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# CALO JETS
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

VBFCleanedCaloJets = cms.EDFilter(
    "VBFCaloJetCleaning",
    src = cms.InputTag("sisCone5CaloJets"),
    srcElectrons = cms.InputTag("gsfElectrons"),
    srcElectronTkIsoValues  = cms.InputTag("eleIsoFromDepsTk"),
    srcElectronIdValues = cms.InputTag("eidRobustTight"),
    tkIsoCut  = cms.double(0.1),
    maxDeltaR = cms.double(0.1),
    minEleOJetEratio = cms.double(0.9),
    doJetRefCheck = cms.bool(False),
    srcJetsRef = cms.InputTag("sisCone5CaloJets"),
    doElectronRefCheck = cms.bool(False),
    srcElectronsRef = cms.InputTag("gsfElectrons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFCleanedCaloJetsRef = cms.EDFilter(
    "VBFCaloJetCleaningRef",
    src = cms.InputTag("sisCone5CaloJets"),
    srcElectrons = cms.InputTag("gsfElectrons"),
    srcElectronTkIsoValues  = cms.InputTag("eleIsoFromDepsTk"),
    srcElectronIdValues = cms.InputTag("eidRobustTight"),
    tkIsoCut  = cms.double(0.1),
    maxDeltaR = cms.double(0.1),
    minEleOJetEratio = cms.double(0.9),
    doJetRefCheck = cms.bool(False),
    srcJetsRef = cms.InputTag("sisCone5CaloJets"),
    doElectronRefCheck = cms.bool(False),
    srcElectronsRef = cms.InputTag("gsfElectrons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



VBFCleanedCaloJetsSequence = cms.Sequence(
    VBFCleanedCaloJets *
    VBFCleanedCaloJetsRef
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----






# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# PF JETS
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

VBFCleanedPFJets = cms.EDFilter(
    "VBFPFJetCleaning",
    src = cms.InputTag("sisCone5PFJets"),
    srcElectrons = cms.InputTag("gsfElectrons"),
    srcElectronTkIsoValues  = cms.InputTag("eleIsoFromDepsTk"),
    srcElectronIdValues = cms.InputTag("eidRobustTight"),
    tkIsoCut  = cms.double(0.1),
    maxDeltaR = cms.double(0.1),
    minEleOJetEratio = cms.double(0.9),
    doJetRefCheck = cms.bool(False),
    srcJetsRef = cms.InputTag("sisCone5PFJets"),
    doElectronRefCheck = cms.bool(False),
    srcElectronsRef = cms.InputTag("gsfElectrons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFCleanedPFJetsRef = cms.EDFilter(
    "VBFPFJetCleaningRef",
    src = cms.InputTag("sisCone5PFJets"),
    srcElectrons = cms.InputTag("gsfElectrons"),
    srcElectronTkIsoValues  = cms.InputTag("eleIsoFromDepsTk"),
    srcElectronIdValues = cms.InputTag("eidRobustTight"),
    tkIsoCut  = cms.double(0.1),
    maxDeltaR = cms.double(0.1),
    minEleOJetEratio = cms.double(0.9),
    doJetRefCheck = cms.bool(False),
    srcJetsRef = cms.InputTag("sisCone5PFJets"),
    doElectronRefCheck = cms.bool(False),
    srcElectronsRef = cms.InputTag("gsfElectrons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



VBFCleanedPFJetsSequence = cms.Sequence(
    VBFCleanedPFJets *
    VBFCleanedPFJetsRef
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
