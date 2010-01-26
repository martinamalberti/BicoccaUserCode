import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# CALO JET CORRECTOR
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

from JetMETCorrections.Configuration.L2L3Corrections_Summer09_cff import *






VBFL2L3SisCone5CaloJets = L2L3CorJetSC5Calo.clone(
    src = cms.InputTag("sisCone5CaloJets"),
    correctors = cms.vstring('L2L3JetCorrectorSC5Calo')
    )

VBFL2L3SisCone5CaloJetsSequence = cms.Sequence(
        VBFL2L3SisCone5CaloJets
        )






VBFL2L3Antikt5CaloJets = L2L3CorJetAK5Calo.clone(
    src = cms.InputTag("antikt5CaloJets"),
    correctors = cms.vstring('L2L3JetCorrectorAK5Calo')
    )

VBFL2L3Antikt5CaloJetsSequence = cms.Sequence(
        VBFL2L3Antikt5CaloJets
        )






VBFL2L3Antikt5PFJets = L2L3CorJetAK5PF.clone(
    src = cms.InputTag("antikt5PFJets"),
    correctors = cms.vstring('L2L3JetCorrectorAK5PF')
    )

VBFL2L3Antikt5PFJetsSequence = cms.Sequence(
        VBFL2L3Antikt5PFJets
        )
