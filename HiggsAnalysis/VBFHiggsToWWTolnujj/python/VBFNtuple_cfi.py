import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# CALO JETS
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

VBFNtupleCaloJets = cms.EDAnalyzer(
    "VBFNtupleCaloJets",
    srcGenParticles                = cms.InputTag("genParticles"),
    srcGenJets                     = cms.InputTag("sisCone5GenJets"),
    srcRecoJets                    = cms.InputTag("sisCone5CaloJets"),
    srcElectrons                   = cms.InputTag("pixelMatchGsfElectrons"),
    srcElectronTkIsoValues         = cms.InputTag("eleIsoFromDepsTk"),
    srcElectronEmIsoValues         = cms.InputTag("eleIsoFromDepsEcalFromHits"),
    srcElectronHadIsoValues        = cms.InputTag("eleIsoFromDepsHcalFromHits"),
    srcElectronIdLooseValues       = cms.InputTag("eidLoose"),
    srcElectronIdTightValues       = cms.InputTag("eidTight"),
    srcElectronIdRobustLooseValues = cms.InputTag("eidRobustLoose"),
    srcElectronIdRobustTightValues = cms.InputTag("eidRobustTight"),
    srcMuons                       = cms.InputTag("muons"),   
    srcGenMetNoNuBSM               = cms.InputTag("genMetNoNuBSM"),
    srcCaloMet                     = cms.InputTag("met"),
    srcMuonCorrectedCaloMet        = cms.InputTag("corMetGlobalMuons"),
    srcPFMet                       = cms.InputTag("pfMet"),
    fileName = cms.untracked.string("VBFNtuple"),
    verbosity = cms.untracked.bool(False),
    eventType = cms.untracked.int32(0),
    doElectronRefCheck = cms.bool(False),
    srcElectronsRef    = cms.InputTag("pixelMatchGsfElectrons"),
    doMuonRefCheck = cms.bool(False),
    srcMuonsRef    = cms.InputTag("muons"),
    doRecoJetRefCheck = cms.bool(False),
    srcRecoJetsRef    = cms.InputTag("sisCone5CaloJets")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 






VBFNtupleCaloJetsSequence = cms.Sequence(
    VBFNtupleCaloJets
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----












# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# PF JETS
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

VBFNtuplePFJets = cms.EDAnalyzer(
    "VBFNtuplePFJets",
    srcGenParticles                = cms.InputTag("genParticles"),
    srcGenJets                     = cms.InputTag("sisCone5GenJets"),
    srcRecoJets                    = cms.InputTag("sisCone5PFJets"),
    srcElectrons                   = cms.InputTag("pixelMatchGsfElectrons"),
    srcElectronTkIsoValues         = cms.InputTag("eleIsoFromDepsTk"),
    srcElectronEmIsoValues         = cms.InputTag("eleIsoFromDepsEcalFromHits"),
    srcElectronHadIsoValues        = cms.InputTag("eleIsoFromDepsHcalFromHits"),
    srcElectronIdLooseValues       = cms.InputTag("eidLoose"),
    srcElectronIdTightValues       = cms.InputTag("eidTight"),
    srcElectronIdRobustLooseValues = cms.InputTag("eidRobustLoose"),
    srcElectronIdRobustTightValues = cms.InputTag("eidRobustTight"),
    srcMuons                       = cms.InputTag("muons"),   
    srcGenMetNoNuBSM               = cms.InputTag("genMetNoNuBSM"),
    srcCaloMet                     = cms.InputTag("met"),
    srcMuonCorrectedCaloMet        = cms.InputTag("corMetGlobalMuons"),
    srcPFMet                       = cms.InputTag("pfMet"),
    fileName = cms.untracked.string("VBFNtuple"),
    verbosity = cms.untracked.bool(False),
    eventType = cms.untracked.int32(0),
    doElectronRefCheck = cms.bool(False),
    srcElectronsRef    = cms.InputTag("pixelMatchGsfElectrons"),
    doMuonRefCheck = cms.bool(False),
    srcMuonsRef    = cms.InputTag("muons"),
    doRecoJetRefCheck = cms.bool(False),
    srcRecoJetsRef    = cms.InputTag("sisCone5PFJets")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 






VBFNtuplePFJetsSequence = cms.Sequence(
    VBFNtuplePFJets
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----






